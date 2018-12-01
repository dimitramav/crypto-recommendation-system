#include <iostream>
#include <string>
#include <unistd.h>
#include <fstream>
#include <map>
#include "datastructs.h"
#include <cmath>
#include <ctime>
using namespace std;

int main(int argc, char * argv[])
{
	string input_path,configuration_path,a,d,output_path;
	vector <double> ** hv;
	vector <double> ** hr;
	double ** ht;
	DataVector * datapoint;
	int int_key;
	int complete = 0;
	ifstream input,configuration;
	string line;
	int first_line=0;
	int dimension=0;
	string metric,bitstring;
	map<string,double> parameters;
	vector <DataVector *> dataset_vector;
	vector <Cluster *> cluster_vector;
	srand (time(NULL));


	/* 1. READ ARGUREMENTS */
	if(read_arguements(argc, argv, input_path,configuration_path,complete,metric,output_path)==-1)
		return 1;
	
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
			initialize_tables(metric,ht,hr,hv,dimension,parameters["w"],parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);

		}
		datapoint = create_datapoint(line,metric,ht,hr,hv,parameters["w"],parameters["number_of_hashtables"],parameters["number_of_hashfunctions"]);
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

	/* 5. PRINT HASHTABLES  
	 for ( unsigned i = 0; i < hashtables_vector[0].bucket_count(); ++i) {
    	std::cout << "bucket #" << i << " contains:";
    	for ( auto local_it = hashtables_vector[0].begin(i); local_it!= hashtables_vector[0].end(i); ++local_it )
      		std::cout <<  " ," << local_it->second.size();
    std::cout << std::endl;
  	}*/

  	/* 2. CREATE HYPERCUBE */
	int hypercube_dimension= int(pow(2.0,parameters["number_of_hashfunctions"]));
	list <DataVector * >  hypercube[hypercube_dimension];

	/* 3. FILL HYPERCUBE */
	for (unsigned int x=0;x<dataset_vector.size();x++)
	{
		DataVector * datapoint=dataset_vector[x];
		string key = datapoint->key_accessor(0,parameters["number_of_hashfunctions"]);
		if(metric.compare("euclidean")==0)
		{
			key = string_to_bitstring(key);
		}	
		int_key=bitstring_to_int(key,metric);
		hypercube[int_key].push_back(datapoint);
	}

	
	/* 5. RANDOM INITIALIZATION*/ 
	random_initialization(dataset_vector,cluster_vector,parameters["k"]);
	cube_assignment(hypercube,parameters["M"],parameters["probes"],parameters["number_of_hashfunctions"], hypercube_dimension,cluster_vector,metric,dataset_vector,ht,hv,hr,parameters["w"]);
	//lsh_assignment(parameters["number_of_hashtables"],parameters["number_of_hashfunctions"],hashtables_vector,cluster_vector,metric,dataset_vector,ht,hv,hr,parameters["w"]);
	//lloyds_assignment(dataset_vector,cluster_vector,metric);
	silhouette_evaluation(dataset_vector,cluster_vector,metric);
	//plus_initialization(dataset_vector,cluster_vector,parameters["k"],metric);

}