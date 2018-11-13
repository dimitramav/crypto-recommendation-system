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
	map<string,int> parameters;
	vector <DataVector *> dataset_vectors;
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
	configuration.open(configuration_path.c_str());  //convert string to const char *
	if (!configuration.is_open())
	{
		cout << "Failed to open file." << endl;
		return 1;
	}
	while (getline(configuration,line))
	{
		find_parameter(line,parameters);
	}
	

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
				
				hr = make_table_hvector(hr,dimension,1,parameters["number_of_hashfunctions"]);
				print_table_hvector(hr,dimension,1,parameters["number_of_hashfunctions"]);
			}
			else
			{
				/* 2. TABLE FOR t */
				ht = new double * [1];
				for(int i = 0;i<1;i++)
					ht[i] = new double[parameters["number_of_hashfunctions"]];
				make_table_hnumber(ht,parameters["w"],1,parameters["number_of_hashfunctions"]);
				//print_table_hnumber(ht,1,parameters["number_of_hashfunctions"]);
				/*4. TABLE FOR v */
				hv = new vector <double> * [1];
				for(int i = 0;i<1;i++)
					hv[i] = new vector <double> [parameters["number_of_hashfunctions"]];
				make_table_hvector(hv,dimension,1,parameters["number_of_hashfunctions"]);
				//print_table_hvector(hv,d,number_of_hashtables,number_of_hashfunctions);
			}
		}
		if(metric.compare("cosine")==0)
		{
			datapoint = new Cosine(line,"item_id",parameters["number_of_hashfunctions"],1,hr);
		}			
		else
		{
			datapoint = new Euclidean(line,"item_id",parameters["number_of_hashfunctions"],1,hv,ht,parameters["w"]);

		}
	dataset_vectors.push_back(datapoint);   

	}
}