#include "datastructs.h"
#include <iostream>
#include <string>
#include <sstream>
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
		Twitter * twitter = new Twitter(tweet_id,userid,total_score, twitter_has_coins);
		twitter_vector.push_back(twitter);
		word_num = 0;
		twitter_has_coins.clear();

	}
	return 1;
}
