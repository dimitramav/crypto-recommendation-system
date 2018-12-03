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
	string input_path,configuration_path,a,d,output_path;
	vector <double> ** hr;
	vector <double> ** hv;
	double ** ht;
	ofstream output;
	DataVector * datapoint;
	int int_key;
	int complete = 0;
	ifstream input,configuration;
	int dimension=0;
	string metric,bitstring,line;
	map<string,double> parameters;
	vector <DataVector *> dataset_vector;
	vector <Cluster *> cluster_vector;
	vector <DataVector *> centroid_vector;
	vector <double> silhouette_vector; 
	int counter =0;
	int initialization,assignment,update;

	srand (time(NULL));


	/* 1. READ ARGUREMENTS */
	if(read_arguements(argc, argv, input_path,configuration_path,complete,metric,output_path,initialization,assignment,update)==-1)
		return 1;	 
	/* 3. READ CONFIGURATION FILE */
	if(!initialize_params(configuration_path,parameters))
		return 1;
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
	output.open(output_path);

	/* 5. RANDOM INITIALIZATION*/ 
	int ini,up,as;
	int parametric_run=0;
	for (ini=1;ini<3;ini++)
	{
		for(as=1;as<4;as++)
		{
			for(up=1;up<3;up++)
			{
				if(ini==1 && as==1 && up==1) //first in the loop
				{
					if (assignment!=0) //user has chosen the algorithms
						parametric_run=1;
					
				}
				if(parametric_run!=1)
				{
					initialization=ini;
					update=up;
					assignment=as;
				}
				int start_clock=clock();
				double new_objective_distance = 0.0;
				double previous_objective_distance = 1.0;
				counter= 0;
				call_initialization(initialization,dataset_vector,cluster_vector,parameters["number_of_clusters"],metric);
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
					reset_distances(dataset_vector);
					if (counter !=0)
					{
						previous_objective_distance = new_objective_distance;
					}
					for (unsigned int i = 0;i<cluster_vector.size();i++)
					{
						cluster_vector[i]->set_update(0);
					}
					new_objective_distance = call_assignment(assignment,hypercube,parameters["M"],parameters["probes"],parameters["number_of_hashfunctions"], hypercube_dimension,cluster_vector,metric,dataset_vector,centroid_vector,hashtables_vector,parameters["number_of_hashtables"]);
					cout << "objective_distance " << new_objective_distance << "/"<< previous_objective_distance<< endl;	
					call_update(update,assignment,cluster_vector,parameters["number_of_hashfunctions"],parameters["number_of_hashtables"],ht, hv,hr,parameters["w"],metric);
					for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
					{
						centroid_vector[i] = cluster_vector[i]->centroid_accessor();
					}
					counter++;
				}
				while((new_objective_distance/previous_objective_distance<(double)0.999 && counter<25)||counter==1);
				cout << "out objective_distance " << new_objective_distance << "/"<< previous_objective_distance<< endl;	
				int stop_clock = clock();
				double total_time=(stop_clock-start_clock)/double(CLOCKS_PER_SEC);
				silhouette_vector = silhouette_evaluation(dataset_vector,cluster_vector,metric);
				print_output(initialization,assignment,update,output,cluster_vector,complete,silhouette_vector,metric,total_time,dataset_vector.size());
				change_data(cluster_vector,dataset_vector);
				if(parametric_run==1)
					break;
			}
			if(parametric_run==1)
					break;
		}
		if(parametric_run==1)
					break;
	}
	delete_data( dataset_vector, metric, hr,ht, hv,parameters["number_of_hashtables"]);
	output.close();

}