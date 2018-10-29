#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sstream>  
#include <iterator>
#include <cctype>
#include <set>
#include <list>
#include <map>
#include <ctime>
#include <algorithm>
#include <getopt.h>
#include "datastructs.h"
#include "helper.h"

using namespace std;

int main(int argc, char * argv[])
{
	string dataset_path,queryset_path,output_path;	
	ifstream dataset,queryset;
	ofstream output;		
	int option;
	int k=-1;
	int M=-1;
	int p=-1;
	int probes;
	int points_to_check;
	string line;
	string vector_value;
	int number_of_hashfunctions;
	int n=0;
	vector <double> ** hv;
	vector <double> ** hr;
	double ** ht;
	DataVector * datapoint;
	DataVector * querypoint;
	double radius;
	string metric;
	vector <DataVector *> dataset_vectors;
	vector <DataVector *>::iterator dataset_iterator;
	set <DataVector *> neighbours_rangesearch;
	set <DataVector *> ::iterator neighbours_iterator;
	map <DataVector *,double> true_neighbour;
	map <DataVector *,double> approximate_neighbour;
	string command;
	double fraction=0;
	double mean_time=0;
	string max_fraction;
	string::size_type sz;
	int query_number=0;
	static struct option long_options[] = {
        {"d",required_argument,NULL  ,  'd' },
        {"q",required_argument,NULL,  'q' },
        {"k",required_argument, NULL,  'k' },
        {"M",required_argument, NULL,  'M' },
        {"probes",required_argument, NULL,  'p' },
        {"o",required_argument,NULL  ,  'o' },
        {NULL,0,NULL, 0}
    };

/* 1. READ ARGUREMENTS */
	while ((option = getopt_long_only (argc, argv, "d:q:k:L:M:p:o:",long_options,NULL)) != -1)
	{
		switch (option)
		{
			case 'd':
			dataset_path=optarg;
			break;
			case 'q':
			queryset_path=optarg;
			break;
			case 'k':
			k=atoi(optarg);
			break;
			case 'M':
			M=atoi(optarg);
			break;
			case 'p':
			p=atoi(optarg);
			break;
			case 'o':
			output_path=optarg;
			break;
			default: 
			cout << "Given parameters are wrong. Try again." << endl;
			return -1;
		}
	}
		
	if ( dataset_path.length()==0 || queryset_path.length()==0 || output_path.length()==0) 
	{
		cout << "File parameters are mandatory. Try again." << endl;
		return -1;
	}

	/* initialize constants */
	if (k == -1)
		number_of_hashfunctions = 3;
	else
		number_of_hashfunctions = k;
	if (M == -1)
		points_to_check = 10;
	else
		points_to_check = M;
	if (p == -1)
		probes = 2;
	else
		probes = p;
	const int w = 4;
	
	  	/* 3. READ DATASET */
   	dataset.open(dataset_path.c_str());  //convert string to const char *
   	if (!dataset.is_open())
   	{
   		cout << "Failed to open file." << endl;
   		return 1;
   	}
   	int d=0;
	while (getline(dataset, line))  //read dataset line by line
	{
		if(n==0)
		{
			metric=find_metric(line);
			if(metric.compare("{default_euclidean}")!=0)
			{
				n++;
				continue; //first line was just metric so we need to read the next line
			}
		}
		n++;
		//first vector
		if(d==0)
		{
			d=find_dimension(line);
			if(metric.compare("{cosine}")==0) //cosine metric
			{
				/* 2. TABLE FOR r */
				hr = new vector <double> * [number_of_hashfunctions];
				for(int i = 0;i<number_of_hashfunctions;i++)
					hr[i] = new vector <double> [number_of_hashtables];
				make_table_hvector(hr,d,number_of_hashfunctions,number_of_hashtables);
			}
			else
			{
				/* 2. TABLE FOR t */
				ht = new double * [number_of_hashfunctions];
				for(int i = 0;i<number_of_hashfunctions;i++)
					ht[i] = new double[number_of_hashtables];
				make_table_hnumber(ht,w,number_of_hashfunctions,number_of_hashtables);
				//print_table_hnumber(ht,number_of_hashfunctions,number_of_hashtables);
				/*4. TABLE FOR v */
				hv = new vector <double> * [number_of_hashfunctions];
				for(int i = 0;i<number_of_hashfunctions;i++)
					hv[i] = new vector <double> [number_of_hashtables];
				make_table_hvector(hv,d,number_of_hashfunctions,number_of_hashtables);
				//print_table_hvector(hv,d,number_of_hashfunctions,number_of_hashtables);
			}
		}
		if(metric.compare("{cosine}")==0)
		{
			datapoint = new Cosine(line,"item_id",number_of_hashfunctions,number_of_hashtables,hr);
		}		
		else
		{
			datapoint = new Euclidean(line,"item_id",number_of_hashfunctions,number_of_hashtables,hv,ht,w);

		}
		dataset_vectors.push_back(datapoint);    
		
	}
	cout << n;
	dataset.close();
   	return 0;
   }