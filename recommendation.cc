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
	map< int, vector<double> > uj;
	map<int, vector<int>> user_uknown_cryptos;
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
	/* 7. CREATE Uj VECTOR */
	construct_uj(num_of_users,num_of_cryptos,twitter_vector,user_has_tweets,uj);
	/* 8. UKNOWN CRYPTOS FOR EACH USER */
	find_uknown_cryptos(uj,user_uknown_cryptos);
	/* 9. REGULATE uj */
	regulate(uj,user_uknown_cryptos);
}