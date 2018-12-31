#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <map>
#include "datastructs.h"
#include "algorithms.h"
#include <cmath>
#include <ctime>
using namespace std;

int main(int argc, char * argv[])
{
	vector<string> cryptocurrencies;
	string input_path,configuration_path,output_path,config_path1,config_path2;
	map <string,double> clustering_parameters;
	map <string,string> file_parameters;
	map <string,double> lexicon;
	vector <Twitter *> twitter_vector;
	map<int,vector<int>> user_has_tweets;
	int num_of_users,num_of_cryptos;
	vector< vector<double> > uj;
	int validate = -1;
	/* 1. READ ARGUREMENTS */
	if(read_arguements(argc, argv, input_path,configuration_path,validate,output_path,config_path1,config_path2)==-1)
		return 1;
	/* 2. READ CONFIGURATION FILE FOR CLUSTERING PARAMETERS */
	if(!initialize_params(config_path1,clustering_parameters,file_parameters,"cluster_config"))
		return 1;	 
	/* 3. READ CONFIGURATION FILE FOR FILE PARAMETERS */
	if(!initialize_params(config_path2,clustering_parameters,file_parameters,"file_config"))
		return 1;	
	/* 4. CREATE COINS ARRAY */ 
	if(!read_coins(file_parameters["coins"],cryptocurrencies))
		return 1;
	/* 5. CREATE SENTIMENT DICTIONARY */	
	if(!read_lexicon(file_parameters["lexicon"],lexicon))
		return 1;
	/* 6. SAVE TWITTERS IN AN ARRAY */
	if(!twitter_analysis(input_path,twitter_vector,lexicon,cryptocurrencies,user_has_tweets))
		return 1;
	num_of_users = user_has_tweets.size();
	num_of_cryptos = cryptocurrencies.size();
	/*for(auto user : user_has_tweets)
	{
   		std::cout <<" user "<< user.first << " has " ;
   		for (int i=0;i<user.second.size();i++)
   		{
   			cout << twitter_vector[user.second[i]]->get_twitterid() << " " ;
   		}
   		cout << endl;
	}*/
	/* 7. CREATE Uj VECTOR */
	for (int i = 0; i < num_of_users; i++) 
	{
    	vector<double> user; // Create an empty row
    	for (int j = 0; j < num_of_cryptos; j++) 
    	{
        	user.push_back(0.0); // Add an element (column) to the row
    	}
    	uj.push_back(user); // Add the row to the main vector
	}
	for(auto user : user_has_tweets)
	{
		cout << "user " << user.first << " ";
   		for (int tweet_num=0;tweet_num<user.second.size();tweet_num++)  //tweets of this user
   		{
   			Twitter * user_twitter = twitter_vector[user.second[tweet_num]];
   			cout << " tweet " << user_twitter->get_twitterid()<< " has crypto ";
   			for(auto crypto_num: user_twitter->get_crypto_mentioned())
   			{
   				cout << crypto_num << " " ;
   				uj[tweet_num][crypto_num]+=user_twitter->get_twitter_score();
   			}
   			cout << endl;
   		}
   		getchar();
	}


}