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
	map<int,vector<int>> user_has_tweets;
	int num_of_users,num_of_cryptos;
	map< int, vector<double> > uj,cj;
	map<int, vector<int>> user_uknown_cryptos;
	vector <DataVector *> ready_tweets_vector,datapoints_uj_vector,datapoints_cj_vector;
	vector <double> ** hr;
	vector <double> ** hv;
	double ** ht;
	vector <Cluster *> cluster_tweet_vector,cluster_uj_vector,cluster_cj_vector;
	int validate = -1;
	vector <DataVector *> centroid_tweet_vector,centroid_uj_vector,centroid_cj_vector;
	vector <double> silhouette_vector; 
	map<int,double> mean_uj,mean_cj;
	int P=20;
	int start_clock,stop_clock;
	double total_time;
	ofstream output;

	srand (time(NULL));

	/* READ ARGUREMENTS */
	if(read_arguements(argc, argv, input_path,configuration_path,validate,output_path,config_path1,config_path2)==-1)
		return 1;
	output.open(output_path.c_str());  //convert string to const char *
	/* READ CONFIGURATION FILE FOR CLUSTERING PARAMETERS */
	if(!initialize_params(config_path1,clustering_parameters,file_parameters,"cluster_config"))
		return 1;	 
	/* READ CONFIGURATION FILE FOR FILE PARAMETERS */
	if(!initialize_params(config_path2,clustering_parameters,file_parameters,"file_config"))
		return 1;	
	/* CREATE COINS ARRAY */ 
	if(!read_coins(file_parameters["coins"],cryptocurrencies))
		return 1;
	/* 5. CREATE SENTIMENT DICTIONARY */	
	if(!read_lexicon(file_parameters["lexicon"],lexicon))
		return 1;
	/* SAVE TWITTERS IN AN ARRAY */
	if(!twitter_analysis(input_path,twitter_vector,lexicon,cryptocurrencies,user_has_tweets,P))
		return 1;
	num_of_users = user_has_tweets.size();
	num_of_cryptos = cryptocurrencies.size();
	/* READ VECTOR TWEETS FROM DATASET */     //initialize hv ht hr
	if(!initialize_datapoints_ready_tweets_vector(file_parameters["ready_tweets"],"cosine",ht,hr,hv,clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],ready_tweets_vector))
	  	return 1;
	/* CREATE HASHTABLES */
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
	/* CREATE TWEETS' CLUSTERS */
	cout << "TWEETS' CLUSTERS" << endl;
	clustering(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],clustering_parameters["number_of_clusters"],"cosine",clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],clustering_parameters["w"],cluster_tweet_vector,ready_tweets_vector,centroid_tweet_vector,twitter_hashtables_vector,ht,hv,hr);
	// //silhouette_vector = silhouette_evaluation(ready_tweets_vector,cluster_vector,"cosine");
	// //print_output(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],output_path,cluster_vector,-1,silhouette_vector,"cosine",0,ready_tweets_vector.size());
	
	/*1.A*/
	/* CREATE Uj VECTOR */
	construct_uj(num_of_users,num_of_cryptos,twitter_vector,user_has_tweets,uj);
	/* UKNOWN CRYPTOS FOR EACH USER */
	find_uknown_cryptos(uj,user_uknown_cryptos);
	/* REGULATE uj */
	regulate(uj,user_uknown_cryptos,mean_uj);
	initialize_datapoints_vectors("real_user",uj, "cosine",ht,hr,hv, clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"] ,datapoints_uj_vector);
	HashTable uj_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];
	for (unsigned int x=0;x<datapoints_uj_vector.size();x++)
	{
		DataVector * datapoint=datapoints_uj_vector[x];
		for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
		{
				string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
				int id = datapoint->id_accessor();  // axristo
				uj_hashtables_vector[i][key].push_back(datapoint);
		}
	}
	start_clock=clock();
	replace_unknown_cryptos_lsh(datapoints_uj_vector,uj_hashtables_vector,user_uknown_cryptos,mean_uj,clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],P,"cosine");
	stop_clock = clock();
	total_time=(stop_clock-start_clock)/double(CLOCKS_PER_SEC);
	recommend_best_cryptos(total_time,"Cosine LSH",datapoints_uj_vector,user_uknown_cryptos,RECOMMEND_A,cryptocurrencies,output);
	/* CLEAR UJ STRUCTURE FOR NEXT METHOD */
	clear_regulated_datapoints_vector(datapoints_uj_vector,uj);

	/* 1B */
	/* CREATE Cj VECTOR */
	// construct_cj(num_of_cryptos,cluster_tweet_vector,twitter_vector,cj_user_has_tweets,cj);
	// /* 8. UKNOWN CRYPTOS FOR EACH VIRTUAL USER */
	// find_uknown_cryptos(cj,cj_user_unknown_cryptos);
	// /* 9. REGULATE cj */
	// regulate(cj,cj_user_unknown_cryptos,mean_cj);
	// initialize_datapoints_vectors("virtual_user",cj, "cosine",ht,hr,hv, clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"] ,datapoints_cj_vector);
	// HashTable cj_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];
	// for (unsigned int x=0;x<datapoints_cj_vector.size();x++)
	// {
	//  	DataVector * datapoint=datapoints_cj_vector[x];
	//  	for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
	//  	{
	//  			string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
	//  			int id = datapoint->id_accessor();  // axristo
	//  			cj_hashtables_vector[i][key].push_back(datapoint);
	//  	}
	// }
	// start_clock=clock();
	// replace_unknown_cryptos_lsh(datapoints_uj_vector,cj_hashtables_vector,user_uknown_cryptos,mean_cj,clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],P,"cosine");
	// stop_clock = clock();
	// total_time=(stop_clock-start_clock)/double(CLOCKS_PER_SEC);
	// recommend_best_cryptos(total_time,"Cosine LSH",datapoints_uj_vector,user_uknown_cryptos,RECOMMEND_B,cryptocurrencies,clustering_parameters["number_of_hashtables"],output);
	// clear_regulated_datapoints_vector(datapoints_uj_vector,uj);

	/* 2A */
	cout << "UJ clustering" << endl; 
	int uj_clusters = int(num_of_users / P);
	//initialize_datapoints_vectors("real_user",uj, "euclidean",ht,hr,hv, clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"] ,euclidean_datapoints_uj_vector);
	clustering(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],uj_clusters,"euclidean",clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],clustering_parameters["w"],cluster_uj_vector,datapoints_uj_vector,centroid_uj_vector,uj_hashtables_vector,ht,hv,hr);
	start_clock=clock();
	replace_unknown_cryptos_clustering(datapoints_uj_vector,cluster_uj_vector,user_uknown_cryptos,mean_uj,P,"euclidean");
	stop_clock = clock();
	total_time=(stop_clock-start_clock)/double(CLOCKS_PER_SEC);
	recommend_best_cryptos(total_time,"Clustering",datapoints_uj_vector,user_uknown_cryptos,RECOMMEND_A,cryptocurrencies,output);
	clear_regulated_datapoints_vector(datapoints_uj_vector,uj);

	// /* 2Β */
	// cout << "CJ clustering" << endl; 
	// int cj_clusters = int(datapoints_cj_vector.size() / P);
	// clustering(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],cj_clusters,"cosine",clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],clustering_parameters["w"],cluster_cj_vector,datapoints_cj_vector,centroid_cj_vector,cj_hashtables_vector,ht,hv,hr);
	// start_clock=clock();
	// replace_unknown_cryptos_clustering(datapoints_uj_vector,cluster_cj_vector,user_uknown_cryptos,mean_cj,P,"euclidean");
	// stop_clock = clock();
	// total_time=(stop_clock-start_clock)/double(CLOCKS_PER_SEC);
	// recommend_best_cryptos(total_time,"Clustering",datapoints_uj_vector,user_uknown_cryptos,RECOMMEND_B,cryptocurrencies,output);
	//delete_data(cluster_tweet_vector,cluster_cj_vector,cluster_uj_vector, datapoints_uj_vector, datapoints_cj_vector,ready_tweets_vector,hr,ht,hv,clustering_parameters["number_of_hashtables"]);

}
