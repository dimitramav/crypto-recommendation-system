#include "datastructs.h"
#include "algorithms.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <math.h>       /* sqrt */


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
	for (unsigned int i=0;i<coins.size();i++)
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

double calculate_score(string word,map<string,double> & lexicon)
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

int twitter_analysis(string input_path, vector<Twitter *> & twitter_vector,map<string,double> & lexicon, vector<string> &coins,map<int,vector<int>> &user_has_tweets,int & P)
{
	ifstream twitter_file;
	set<int> twitter_has_coins;
	string line,data;
	int cryptoreference;
	int userid,tweet_id;
	double total_score;
	twitter_file.open(input_path.c_str());  //convert string to const char *
	if (!twitter_file.is_open())
	{
		cout << "Fail to open twitter file" << endl;
		return 0;
	}
	int word_num=0;
	int tweet_num = 0;
	int first_line = 1;
	while (getline(twitter_file,line))
	{
		stringstream linestream(line);
		//int print =0;
		total_score = 0;
		while( getline(linestream, data, '\t'))
		{
			if (first_line)
			{
				if(data[0]=='P')
				{
					string delimiter = ":";
					size_t pos = 0;
					while ((pos = line.find(delimiter)) != string::npos)
					{
						line.erase(0, pos + delimiter.length());
						P = stoi(line);
					}
					first_line++;
					continue;
					
				}
			}
			first_line++;
			if(word_num == 0)
			{
				userid =stoi(data);
			}
			else if (word_num == 1)
			{
				tweet_id = stoi(data);
				//if (tweet_id == 3)
				//	print=1;
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
				//if (print==1)
				//	cout << data << " " << total_score << endl;
			}
			word_num++;
		}
		total_score = total_score/sqrt(total_score*total_score+15); //regulate score
		//if(print==1)
		//	cout << "total_score " << total_score << endl;
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
   		for (unsigned int tweet_num=0;tweet_num<user.second.size();tweet_num++)  //tweets of this user
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
   	for(auto user: uj)  //erase vector like [ 0 inf inf 0]
   	{
   	 	int empty=1;
   	 	for(auto crypto: user.second)
   	 	{
   	 		if(crypto!=10000 && crypto !=0)
   	 		{
   	 			empty = 0; 
   	 		}
   	 	}
   		if(empty == 1)
   		{
   			uj.erase(user.first);
   		}
   	}
   	// for(auto user: uj)
   	// {
   	// 	cout <<"USER " << user.first << endl;
   	// 	for(auto crypto: user.second)
   	// 		cout << crypto << " ";
   	// 	cout << endl;
   	// 	cout << endl;
   	// }
   	return;
   }

   void find_uknown_cryptos(map<int,vector<double>> uj,map<int,vector<int>> & user_uknown_cryptos)
   {
   	int crypto_num = 0; 
		for (auto user : uj) //iterate users
		{
			crypto_num = 0; 
			for(auto crypto : user.second)  //iterate cryptos of each user
			{
				if(crypto == 10000)
					user_uknown_cryptos[user.first].push_back(crypto_num);
				crypto_num++;
			}
		}
		return;
	}

	void regulate(map<int,vector<double>> & uj,map<int,vector<int>> user_uknown_cryptos,map<int,double> & mean_uj)
	{
		int total_cryptos,unknown_cryptos,known_cryptos;
		double total_value=0.0;
		double mean_value;
    	for (auto user:uj)   //find mean value and store it in the struct mean_uj
    	{
    		//cout << "USER " << user.first << endl;
    		total_cryptos = user.second.size();
    		unknown_cryptos = user_uknown_cryptos.find(user.first)->second.size();
    		known_cryptos = total_cryptos - unknown_cryptos;
    		total_value = 0.0;
    		for (auto value_crypto : user.second)
    		{
    			if(value_crypto!=10000)
    			{
    				//cout << "value_crypto " <<value_crypto << " ";
    				total_value +=value_crypto;
    			}
    		}
    		//cout << "known cryptos " << known_cryptos << endl;
    		mean_value = total_value/known_cryptos;
    		mean_uj[user.first] = mean_value;
    	}
    	for (auto& user:uj)  //push mean value to uknown cryptos  (10000 becomes mean value for lsh)
    	{
    		for(auto& crypto:user.second)
    		{
    			if (crypto == 10000)
    				crypto= mean_uj.at(user.first);
    		}
    	}
    	
    }

    void construct_cj(int num_of_cryptos, vector <Cluster *> cluster_vector, vector <Twitter *> twitter_vector,	map< int,vector<int>> & cj_user_has_tweets, map<int,vector<double> >& cj)
    {
    	int cluster_tweet_id;
    	Twitter * current_twitter;
	for(unsigned int cluster =0;cluster<cluster_vector.size();cluster++)  //for each cluster
	{
		for (int i=0;i<num_of_cryptos;i++)
		{
			cj[cluster].push_back(10000);
		}
		for(auto cluster_twitter: cluster_vector[cluster]->content_accessor())  //for each cluster's vector
		{
			cluster_tweet_id=cluster_twitter->id_accessor();
			cj_user_has_tweets[cluster].push_back(cluster_tweet_id);  //list of tweets of virtual user
			//cout << "twitter in cluster with id " << cluster_tweet_id << " has crypto ";
			for(auto twitter:twitter_vector )    //twitter reference from cluster to twitter vector to calculate score
			{
				if(twitter->get_twitterid()==cluster_tweet_id)
				{
					current_twitter = twitter;
					break;
				}
			}
			for(auto crypto_num: current_twitter->get_crypto_mentioned())
			{
				//cout << crypto_num << " ";
				if(cj[cluster][crypto_num]==10000)
					cj[cluster][crypto_num] = 0.0;
				cj[cluster][crypto_num]+=current_twitter->get_twitter_score();
			}
   			// for(int i=0;i<num_of_cryptos;i++)
   			// {
   			// 	cout << cj[cluster][i] << " ";
   			// }
			//cout << endl;

		}
		//getchar();
	}
}


void replace_uknown_cryptos(vector <DataVector *> dataset_vector,HashTable * uj_hashtables_vector,map<int,vector<int>> user_unknown_cryptos,map<int,double> mean_uj,int number_of_hashtables,int number_of_hashfunctions,int P,string metric)
{
	int user_id;
	int neighbour_id;
	vector <double> v,u;
	double similarity,absolute_similarity;
	double mean_u,mean_v,relativity_u_v,partial_sum;
	set <DataVector *> P_neighbours;
	for (auto z: dataset_vector) //iterate user tweets
	{
		user_id = z->id_accessor();//cout << "USER " << user_id << endl;
		P_neighbours = rangesearch(number_of_hashtables,number_of_hashfunctions,uj_hashtables_vector ,P ,z,metric );
		//cout << v->point_accessor()[unknown_crypto] << " " ;
		u = z->point_accessor();
		mean_u = mean_uj[user_id];
		for(auto unknown_crypto: user_unknown_cryptos[user_id] ) //iterate list of uknown cryptos
		{
			absolute_similarity=0.0;
			partial_sum = 0.0;
			for(auto z_neighbour:P_neighbours) //check neighbours of v to find the regulated value
			{
				neighbour_id = z_neighbour->id_accessor();
				if(neighbour_id!=user_id) //not same 
				{
							//cout << "USEEEEER " << neighbour_id << endl;
					if(find(user_unknown_cryptos[neighbour_id].begin(), user_unknown_cryptos[neighbour_id].end(), unknown_crypto) == user_unknown_cryptos[neighbour_id].end())
					{
						v = z_neighbour->point_accessor();
						mean_v=mean_uj[neighbour_id];
						similarity = inner_product(std::begin(u), std::end(u), std::begin(v), 0.0)/sqrt(mypower(v))*sqrt(mypower(u));
						relativity_u_v =  z_neighbour->point_accessor()[unknown_crypto];
						absolute_similarity += abs(similarity);
						partial_sum+=similarity*(relativity_u_v-mean_v);
						//cout << z_neighbour->point_accessor()[unknown_crypto] << " " ;
					}
				}
			}
			//new value 
			if(absolute_similarity!=0)
			{
				//cout <<"before "<< z->point_accessor()[unknown_crypto] << " ";
				z->point_mutator(unknown_crypto,mean_u + (1/absolute_similarity) * partial_sum);
				//z->point_accessor()[unknown_crypto]=mean_u + (1/absolute_similarity) * partial_sum;
				//cout << "after "<<z->point_accessor()[unknown_crypto] << endl;
				//getchar();
			}
					
		}

	}
	
	return;
}

// Function to convert a std::map<K,V> to std::multimap<V,K>
template<typename K, typename V>
std::multimap<V,K,greater<V>> invertMap(std::map<K,V> const &map)
{
	std::multimap<V,K,greater<V>> multimap;

	for (auto const &pair: map) {
		multimap.insert(std::make_pair(pair.second, pair.first));
	}

	return multimap;
}

string get_cryptoname(int position,vector<string>cryptocurrencies)
{
	string alt_coin;
	stringstream linestream(cryptocurrencies[position]);
	int counter = 0;
	while( getline(linestream, alt_coin, '\t'))
	{
		if (counter	==4)
			return alt_coin;
		counter++;

	}
	return cryptocurrencies[position].substr(0,cryptocurrencies[position].find('\t'));
}

void recommend_best_cryptos(double total_time,string method,vector <DataVector *> datapoints_vector,map<int,vector<int>> user_unknown_cryptos,int recommend_number,vector<string> cryptocurrencies,int number_of_hashtables,ofstream & output)
{

	int user_id;
	map<int,double> estimated_crypto;
	output << method << endl;
	for (auto z: datapoints_vector) //iterate user tweets
	{
		user_id = z->id_accessor();
		for(auto unknown_crypto: user_unknown_cryptos[user_id] ) //iterate list of uknown cryptos
		{
			estimated_crypto[unknown_crypto]=z->point_accessor()[unknown_crypto];
		}
		multimap<double,int,greater <double>> multimap = invertMap(estimated_crypto);
		print_recommended_output(output,recommend_number,user_id,multimap,cryptocurrencies);				
	}
	output << "Execution time " << total_time << endl;
	output << endl;	
	
}


void print_recommended_output(ofstream & output,int recommend_number,int user_id,multimap<double,int,greater<double>> multimap,vector<string> cryptocurrencies)
{
	int counter =0 ;
	output << user_id << " ";
	for(int i=0;i<recommend_number;i++)
	{
		for(auto pair : multimap)
		{
			if(counter == recommend_number)
				break;
			output << get_cryptoname(pair.second,cryptocurrencies)<< " " ;
						//cout << pair.first <<" "<< pair.second << endl;
			counter ++; 
		}
	}
	output << endl;
}

void clear_regulated_datapoints_vector(vector<DataVector *>& datapoints_vector ,map<int,vector<double>> uj)
{
	for(auto user: uj)
	{
		for(int i=0;i<datapoints_vector.size();i++)
		{
			if(datapoints_vector[i]->id_accessor() == user.first)  //datavector id and user id are the same
			{
				datapoints_vector[i]->set_point(user.second);

			}
		}
	}
}