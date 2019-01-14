#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <map>
#include "datastructs.h"
#include "algorithms.h"
#include <cmath>
#include <ctime>
#define RECOMMEND_A 5
#define RECOMMEND_B 2
using namespace std;

int main(int argc, char * argv[])
{
	vector<string> cryptocurrencies;
	string input_path,configuration_path,output_path,config_path1,config_path2;
	map <string,double> clustering_parameters;
	map <string,string> file_parameters;
	map <string,double> lexicon;
	vector <Twitter *> twitter_vector;
	map<int,vector<int>> user_has_tweets,cj_user_has_tweets;
	int num_of_users,num_of_cryptos;
	map< int, vector<double> > uj,cj;
	map<int, vector<int>> user_uknown_cryptos,cj_user_unknown_cryptos;
	vector <DataVector *> ready_tweets_vector,datapoints_uj_vector,datapoints_cj_vector;
	vector <double> ** hr;
	vector <double> ** hv;
	double ** ht;
	vector <Cluster *> cluster_vector;
	int validate = -1;
	vector <DataVector *> centroid_vector;
	vector <double> silhouette_vector; 
	map<int,double> mean_uj,mean_cj;
	int P=20;
	ofstream output;
	/* 1. READ ARGUREMENTS */
	if(read_arguements(argc, argv, input_path,configuration_path,validate,output_path,config_path1,config_path2)==-1)
		return 1;
	output.open(output_path.c_str());  //convert string to const char *
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
	if(!twitter_analysis(input_path,twitter_vector,lexicon,cryptocurrencies,user_has_tweets,P))
		return 1;
	num_of_users = user_has_tweets.size();
	num_of_cryptos = cryptocurrencies.size();
	/* 1.READ VECTOR TWEETS FROM DATASET */     //initialize hv ht hr
	if(!initialize_datapoints_ready_tweets_vector(file_parameters["ready_tweets"],file_parameters["metric"],ht,hr,hv,clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],ready_tweets_vector))
	 	return 1;
	/* 2. CREATE HASHTABLES */
	HashTable twitter_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];
	for (unsigned int x=0;x<ready_tweets_vector.size();x++)
	{
		DataVector * datapoint=ready_tweets_vector[x];
		for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
		{
	 			string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
	 			int id = datapoint->id_accessor();
	 			twitter_hashtables_vector[i][key].push_back(datapoint);
	 	}
	}
	/* CREATE TWEETS' CLUSTERS*/
	clustering(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],clustering_parameters["number_of_clusters"],file_parameters["metric"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],clustering_parameters["w"],cluster_vector,ready_tweets_vector,centroid_vector,twitter_hashtables_vector,ht,hv,hr);
	// //silhouette_vector = silhouette_evaluation(ready_tweets_vector,cluster_vector,file_parameters["metric"]);
	// //print_output(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],output_path,cluster_vector,-1,silhouette_vector,file_parameters["metric"],0,ready_tweets_vector.size());
	// /* CREATE Cj VECTOR */
	construct_cj(num_of_cryptos,cluster_vector,twitter_vector,cj_user_has_tweets,cj);
	/* 8. UKNOWN CRYPTOS FOR EACH VIRTUAL USER */
	find_uknown_cryptos(cj,cj_user_unknown_cryptos);
	/* 9. REGULATE cj */
	regulate(cj,cj_user_unknown_cryptos,mean_cj);
	initialize_datapoints_vectors("virtual_user",cj, file_parameters["metric"],ht,hr,hv, clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"] ,datapoints_cj_vector);
	HashTable cj_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];
	for (unsigned int x=0;x<datapoints_cj_vector.size();x++)
	{
		DataVector * datapoint=datapoints_cj_vector[x];
		for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
		{
				string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
				int id = datapoint->id_accessor();  // axristo
				cj_hashtables_vector[i][key].push_back(datapoint);
		}
	}
	replace_uknown_cryptos(cj_hashtables_vector,cj_user_unknown_cryptos,mean_cj,clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],P,file_parameters["metric"]);
	recommend_best_cryptos(cj_hashtables_vector,cj_user_unknown_cryptos,RECOMMEND_B,cryptocurrencies,clustering_parameters["number_of_hashtables"],output);
	// /* 7. CREATE Uj VECTOR */
	// construct_uj(num_of_users,num_of_cryptos,twitter_vector,user_has_tweets,uj);
	// /* 8. UKNOWN CRYPTOS FOR EACH USER */
	// find_uknown_cryptos(uj,user_uknown_cryptos);
	// /* 9. REGULATE uj */
	// regulate(uj,user_uknown_cryptos,mean_uj);
	// initialize_datapoints_vectors("real_user",uj, file_parameters["metric"],ht,hr,hv, clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"] ,datapoints_uj_vector);
	// HashTable uj_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];
	// for (unsigned int x=0;x<datapoints_uj_vector.size();x++)
	// {
	// 	DataVector * datapoint=datapoints_uj_vector[x];
	// 	for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
	// 	{
	// 			string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
	// 			int id = datapoint->id_accessor();  // axristo
	// 			uj_hashtables_vector[i][key].push_back(datapoint);
	// 	}
	// }

	// //1.A
	// replace_uknown_cryptos(uj_hashtables_vector,user_uknown_cryptos,mean_uj,clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],P,file_parameters["metric"]);
	// recommend_best_cryptos(uj_hashtables_vector,user_uknown_cryptos,RECOMMEND_A,cryptocurrencies,clustering_parameters["number_of_hashtables"],output);
	// PAUSE FOR LATER
	//int id = extract_id(ready_tweets_vector[0]->name_accessor());
	
	
}