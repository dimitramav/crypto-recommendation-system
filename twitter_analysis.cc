#include "datastructs.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
/* CRYPTOCURRENCIES */

int read_coins(string coins_path, vector<string> & cryptocurrencies)
{
	ifstream coins_file;
	string line;
	coins_file.open(coins_path.c_str());  //convert string to const char *
	if (!coins_file.is_open())
	{
		cout << "Fail to open coins file" << endl;
		return 0;
	}
	while (getline(coins_file,line))
	{
		cryptocurrencies.push_back(line);
	}

	return 1;

}

int read_lexicon(string lexicon_path, map<string,double> &lexicon)
{
	ifstream lexicon_file;
	string line;
	lexicon_file.open(lexicon_path.c_str());  //convert string to const char *
	string key;
	string::size_type sz;     // alias of size_t
	double value;
	if (!lexicon_file.is_open())
	{
		cout << "Fail to open lexicon file" << endl;
		return 0;
	}
	int counter=0;
	while (getline(lexicon_file,line))
	{
		stringstream linestream(line);
		string data;
		while( getline(linestream, data, '\t'))
		{
			if(counter%2==0)
			{
				key=data;
			}
			else
			{ 
				value=stod(data,&sz);
			}
			lexicon[key]=value;
			counter++;
		}

	}
	return 1;
}

int check_coins(string word, vector<string> & coins)
{
	string alt_coin;
	vector <int> twitter_has_crypto;
	for (int i=0;i<coins.size();i++)
	{
		stringstream linestream(coins[i]);
		while( getline(linestream, alt_coin, '\t'))
		{
			if (!word.empty() && (word[word.size() - 1] == '\r' || word[word.size() - 1] == '\n'))
				word.erase(word.size() - 1);
			if (!alt_coin.empty() && (alt_coin[alt_coin.size() - 1] == '\r' || alt_coin[alt_coin.size() - 1] == '\n'))
				alt_coin.erase(alt_coin.size() - 1);
			if(alt_coin.compare(word)==0)
			{
				return i;  //reference for the crypto that is mentioned
			}
		}
	}
	return -1;
}

int calculate_score(string word,map<string,double> & lexicon)
{
	if ( lexicon.find(word) == lexicon.end() ) 
	{
  		// not found
		return 0;
	}
	else 
	{
  		//found
		return lexicon.find(word)->second;
	}
}

int twitter_analysis(string input_path, vector<Twitter *> & twitter_vector,map<string,double> & lexicon, vector<string> &coins,map<int,vector<int>> &user_has_tweets)
{
	ifstream twitter_file;
	set<int> twitter_has_coins;
	string line,data;
	int cryptoreference;
	int userid,tweet_id;
	twitter_file.open(input_path.c_str());  //convert string to const char *
	if (!twitter_file.is_open())
	{
		cout << "Fail to open twitter file" << endl;
		return 0;
	}
	int word_num=0;
	double total_score=0;
	int tweet_num = 0;
	while (getline(twitter_file,line))
	{
		stringstream linestream(line);
		while( getline(linestream, data, '\t'))
		{
			if(word_num == 0)
			{
				userid =stoi(data);
			}
			else if (word_num == 1)
			{
				tweet_id = stoi(data);
				//cout <<"tweet " << tweet_id << endl;
				user_has_tweets[userid].push_back(tweet_num);
				tweet_num++;	
			}
			else
			{
				cryptoreference=check_coins(data,coins);
				if(cryptoreference!=-1)
					twitter_has_coins.insert(cryptoreference);
				total_score+=calculate_score(data,lexicon);
			}
			word_num++;
		}
		total_score = total_score/(total_score*total_score+15); //regulate score
		//cout << total_score<< endl;
		//getchar(); 
		Twitter * twitter = new Twitter(tweet_id,userid,total_score, twitter_has_coins);
		twitter_vector.push_back(twitter);
		word_num = 0;
		twitter_has_coins.clear();

	}
	return 1;
}

void construct_uj(int num_of_users, int num_of_cryptos, vector<Twitter *> twitter_vector, map<int,vector<int>> user_has_tweets,map< int,vector<double> > &uj)
{
	for(auto user : user_has_tweets)
	{

		//cout << "user " << user.first << " ";
		for (int i=0;i<num_of_cryptos;i++)
		{
			uj[user.first].push_back(10000);
		}
   		for (int tweet_num=0;tweet_num<user.second.size();tweet_num++)  //tweets of this user
   		{
   			Twitter * user_twitter = twitter_vector[user.second[tweet_num]];
   			//cout << " tweet " << user_twitter->get_twitterid()<< " " ;
   			//cout << " has crypto " << " ";
   			for(auto crypto_num: user_twitter->get_crypto_mentioned())
   			{
   				//cout << crypto_num << " ";
   				if(uj[user.first][crypto_num]==10000)
   					uj[user.first][crypto_num] = 0.0;
   				uj[user.first][crypto_num]+=user_twitter->get_twitter_score();
   			}
   			//cout << endl;
   		}
   		//getchar();
   	}
   	return;
 }

   void find_uknown_cryptos(map<int,vector<double>> uj,map<int,vector<int>> & user_uknown_cryptos)
   {
   		int crypto_num = 0; 
		for (auto user : uj) //iterate users
		{
			for(auto crypto : user.second)  //iterate cryptos of each user
			{
				if(crypto == 10000)
					user_uknown_cryptos[user.first].push_back(crypto_num);
				crypto_num++;
			}
		}
		return;
   }

    void regulate(map<int,vector<double>> & uj,map<int,vector<int>> user_uknown_cryptos)
    {
    	int total_cryptos,unknown_cryptos,known_cryptos;
    	double total_value=0.0;
    	double mean_value;
    	for (auto user:uj)
    	{
    	 	total_cryptos = user.second.size();
    		unknown_cryptos = user_uknown_cryptos.find(user.first)->second.size();
    		known_cryptos = total_cryptos - unknown_cryptos;
    		total_value = 0.0;
    		for (auto value_crypto : user.second)
    		{
    			cout << value_crypto << " ";
    			if(value_crypto!=10000)
    				total_value +=value_crypto;
    		}
    		cout << endl;
    		mean_value = total_value/known_cryptos;
    		cout <<"mean_value " << mean_value << endl;
    	}
    }

