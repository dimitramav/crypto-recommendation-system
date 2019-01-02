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
	vector <DataVector *> ready_tweets_vector;
	vector <double> ** hr;
	vector <double> ** hv;
	double ** ht;
	vector <Cluster *> cluster_vector;
	int validate = -1;
	vector <DataVector *> centroid_vector;
	vector <double> silhouette_vector; 
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
	//regulate(uj,user_uknown_cryptos);
	// PAUSE FOR LATER
	/* 1.READ VECTOR TWEETS FROM DATASET */
	if(!initialize_ready_tweets_vector(file_parameters["ready_tweets"],file_parameters["metric"],ht,hr,hv,clustering_parameters["w"],clustering_parameters["number_of_hashtables"],clustering_parameters["number_of_hashfunctions"],ready_tweets_vector))
		return 1;
	//int id = extract_id(ready_tweets_vector[0]->name_accessor());
	/* 2. CREATE HASHTABLES */
	HashTable twitter_hashtables_vector[ int(clustering_parameters["number_of_hashtables"])];

	for (unsigned int x=0;x<ready_tweets_vector.size();x++)
	{
		DataVector * datapoint=ready_tweets_vector[x];
		for (int i=0;i< clustering_parameters["number_of_hashtables"]; i++)
		{
				string key = datapoint->key_accessor(i,clustering_parameters["number_of_hashfunctions"]);
				string name = datapoint->name_accessor();
				twitter_hashtables_vector[i][key].push_back(datapoint);
		}
	}
	/* CREATE TWEETS' CLUSTERS*/
	clustering(clustering_parameters["initialization"],ready_tweets_vector,cluster_vector,clustering_parameters["number_of_clusters"],file_parameters["metric"])
	int counter =0;
	double new_objective_distance = 0.0;
	double previous_objective_distance = 1.0;
	//call_initialization(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],ready_tweets_vector,cluster_vector,clustering_parameters["number_of_clusters"],file_parameters["metric"]);
	for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
	{
		if(!centroid_vector.empty() && i==0)
		{
			centroid_vector.clear();
		}
		centroid_vector.push_back(cluster_vector[i]->centroid_accessor());
	}
	do
	{	
		reset_distances(ready_tweets_vector);
		if (counter !=0)
		{
			previous_objective_distance = new_objective_distance;
		}
		for (unsigned int i = 0;i<cluster_vector.size();i++)
		{
			cluster_vector[i]->set_update(0);
		}
		new_objective_distance = call_assignment(clustering_parameters["assignment"],clustering_parameters["number_of_hashfunctions"],cluster_vector,file_parameters["metric"],ready_tweets_vector,centroid_vector,twitter_hashtables_vector,clustering_parameters["number_of_hashtables"]);
		cout << "objective_distance " << new_objective_distance << "/"<< previous_objective_distance<< endl;	
		call_update(clustering_parameters["update"],clustering_parameters["assignment"],cluster_vector,clustering_parameters["number_of_hashfunctions"],clustering_parameters["number_of_hashtables"],ht, hv,hr,clustering_parameters["w"],file_parameters["metric"]);
		for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
		{
			centroid_vector[i] = cluster_vector[i]->centroid_accessor();
		}
		counter++;
	}
	while((new_objective_distance/previous_objective_distance<(double)0.999 && counter<25)||counter==1);
	//silhouette_vector = silhouette_evaluation(ready_tweets_vector,cluster_vector,file_parameters["metric"]);
	//print_output(clustering_parameters["initialization"],clustering_parameters["assignment"],clustering_parameters["update"],output_path,cluster_vector,1,silhouette_vector,file_parameters["metric"],0,ready_tweets_vector.size());
}