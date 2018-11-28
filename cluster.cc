#include <iostream>
#include <getopt.h>
#include <string>
#include <unistd.h>
#include <fstream>
#include <map>
#include "datastructs.h"
using namespace std;

int main(int argc, char * argv[])
{
	string input_path,configuration_path,a,d,output_path;
	vector <double> ** hv;
	vector <double> ** hr;
	double ** ht;
	DataVector * datapoint;
	int option;
	int complete = 0;
	ifstream input,configuration;
	string line;
	int first_line=0;
	int dimension=0;
	string metric;
	map<string,double> parameters;
	vector <DataVector *> dataset_vector;
	vector <Cluster *> cluster_vector;
	static struct option long_options[] = {
		{"i",required_argument,NULL  ,  'i' },
		{"c",required_argument,NULL,  'c' },
		{"d",required_argument, NULL,  'd' },
		{"complete",optional_argument,NULL, 'a'},
		{"o",required_argument,NULL  ,  'o' },
		{NULL,0,NULL, 0}
	};

	/* 1. READ ARGUREMENTS */
	while ((option = getopt_long_only (argc, argv, "i:c:a:o:d:",long_options,NULL)) != -1)
	{
		switch (option)
		{
			case 'i':
			input_path = optarg;
			break;
			case 'c':
			configuration_path = optarg;
			break;
			case 'a':
			complete = 1;
			break;
			case 'd':
			metric = optarg;
			break;
			case 'o':
			output_path = optarg;
			break;
			default: 
			cout << "Given parameters are wrong. Try again." << endl;
			return -1;
		}
	}

	if ( input_path.length()==0 || configuration_path.length()==0 || output_path.length()==0) 
	{
		cout << "File parameters are mandatory. Try again." << endl;
		return -1;
	}
	
	/* 3. READ CONFIGURATION FILE */
	if(!initialize_params(configuration_path,parameters))
		cout << "Failed to open file." << endl;;

 	/* 4. READ DATASET */
   	input.open(input_path.c_str());  //convert string to const char *
   	if (!input.is_open())
   	{
   		cout << "Failed to open file." << endl;
   		return 1;
   	}
	while (getline(input, line))  //read dataset line by line
	{
		//first vector
		if(dimension==0)
		{
			dimension=find_dimension(line);
			/* 5. INITIALIZE TABLES */
			if(metric.compare("cosine")==0) //cosine metric
			{
				/* 2. TABLE FOR r */	
				hr = make_table_hvector(hr,dimension,parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);

				//print_table_hvector(hr,dimension,parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);
			}
			else
			{
				/* 2. TABLE FOR t */
				ht = make_table_hnumber(ht,parameters["w"],parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);
				//print_table_hnumber(ht,1,parameters["number_of_hashfunctions"]);
				/*4. TABLE FOR v */
				hv = make_table_hvector(hv,dimension,parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);
				//print_table_hvector(hv,dimension,1,parameters["number_of_hashfunctions"]);
			}
		}
		if(metric.compare("cosine")==0)
		{
			datapoint = new Cosine(line,"item_id",parameters["number_of_hashfunctions"],parameters["number_of_hashtables"],hr);
		}			
		else
		{
			datapoint = new Euclidean(line,"item_id",parameters["number_of_hashfunctions"],parameters["number_of_hashtables"],hv,ht,parameters["w"]);

		}
		dataset_vector.push_back(datapoint);   
	}

	input.close();
	/* 4. CREATE HASHTABLES */
	HashTable hashtables_vector[ int(parameters["number_of_hashtables"])];
	for (unsigned int x=0;x<dataset_vector.size();x++)
	{
		DataVector * datapoint=dataset_vector[x];
		for (int i=0;i< parameters["number_of_hashtables"]; i++)
		{
				string key = datapoint->key_accessor(i,parameters["number_of_hashfunctions"]);
					string name = datapoint->name_accessor();
				hashtables_vector[i][key].push_back(datapoint);
		}
	}
	/* 5. PRINT HASHTABLES */ 
	/* for ( unsigned i = 0; i < hashtables_vector[0].bucket_count(); ++i) {
    	std::cout << "bucket #" << i << " contains:";
    	for ( auto local_it = hashtables_vector[0].begin(i); local_it!= hashtables_vector[0].end(i); ++local_it )
      		std::cout <<  " ," << local_it->second.size();
    std::cout << std::endl;
  	}*/


	/* 5. RANDOM INITIALIZATION*/ 
	random_initialization(dataset_vector,cluster_vector,parameters["k"]);
	lsh_assignment(parameters["number_of_hashtables"],parameters["number_of_hashfunctions"],hashtables_vector,cluster_vector,metric,dataset_vector);
	//lloyds_assignment(dataset_vector,cluster_vector,metric);
	//silhouette_evaluation(dataset_vector,cluster_vector,metric);
	//plus_initialization(dataset_vector,cluster_vector,parameters["k"],metric);

}