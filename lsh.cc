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
	int L=-1;
	string line;
	string vector_value;
	int number_of_hashfunctions;
	int number_of_hashtables;
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

/* 1. READ ARGUREMENTS */
	while ((option = getopt (argc, argv, "d:q:k:L:o:")) != -1)
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
			case 'L':
			L=atoi(optarg);
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
		number_of_hashfunctions = 4;
	else
		number_of_hashfunctions = k;
	if (L == -1)
		number_of_hashtables = 5;
	else
		number_of_hashtables = L;
	const int w = 400;


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
	dataset.close();
	/* 4. CREATE HASHTABLES */
	HashTable hashtables_vector[number_of_hashtables];
	for (int x=0;x<dataset_vectors.size();x++)
	{
		DataVector * datapoint=dataset_vectors[x];
		for (int i=0;i< number_of_hashtables; i++)
		{
			
			if(metric.compare("{cosine}")==0)
			{
				string key = datapoint->key_accessor(i,number_of_hashfunctions);
				string::iterator end_pos = remove(key.begin(), key.end(), ' ');
				key.erase(end_pos, key.end());
				unsigned long long_key=stoull(key, 0, 2);
				string name = datapoint->name_accessor();
				hashtables_vector[i][long_key].push_back(datapoint);


			}
			else
			{
				string key = datapoint->key_accessor(i,number_of_hashfunctions);
				string name = datapoint->name_accessor();
				//hashtables_vector[i][key].push_back(datapoint);
			}
		}
	}
	for (int x=0;x<number_of_hashtables;x++)
	{
			
		cout << "hash table  " << x <<" has " << hashtables_vector[x].bucket_count() <<" buckets" << endl;
	}
	/* 5. READ QUERYSET AGAIN AND AGAIN*/
	do{
   		if(!queryset_path.compare("no")) //end of program - Time to delete some structs
   		{
   			//delete dataset
   			for(dataset_iterator = dataset_vectors.begin(); dataset_iterator != dataset_vectors.end(); dataset_iterator++)    {
   				delete *(dataset_iterator);
   			}


 			if(metric.compare("{cosine}")==0) //cosine metric
 			{
 				//delete r table
 				for(int i = 0;i<number_of_hashfunctions;i++)
 				{
 					delete [] hr[i];
 				}

 			}
 			else
 			{
 				//delete t & v table
 				for(int i = 0;i<number_of_hashfunctions;i++)
 				{
 					delete [] ht[i];
 					delete [] hv[i];
 				}
 				delete [] ht;
 				delete [] hv;
 			}


 			return 0;
 		}
		queryset.open(queryset_path.c_str());  //convert string to const char *
		if (!queryset.is_open())
		{
			cout << "Failed to open file." << endl;
			return 1;
		}
		n=0;
   		while (getline(queryset, line))  //read dataset line by line
   		{
   			//see if radius is defined 
   			if (n==0)
   			{
   				radius=find_radius(line);
   				output.open (output_path);
   				n++;
   				continue;

   			}
   			query_number++;
   			if(metric.compare("{cosine}")==0)
   			{
   				querypoint = new Cosine(line,"item_idS",number_of_hashfunctions,number_of_hashtables,hr);

   			}
   			else
   			{
   				querypoint = new Euclidean(line,"item_idS",number_of_hashfunctions,number_of_hashtables,hv,ht,w);
   			}
   			/* NN ALGORITHMS */
   			neighbours_rangesearch=rangesearch(number_of_hashtables,number_of_hashfunctions,hashtables_vector,radius,querypoint,metric);
   			int start_approximate=clock();
   			approximate_neighbour=approximateNN(number_of_hashtables,number_of_hashfunctions,hashtables_vector,querypoint,metric);
   			int stop_approximate=clock();
   			int start_true=clock();
   			true_neighbour=trueNN(dataset_vectors,querypoint,metric);
   			int stop_true = clock();
   			mean_time+=(stop_approximate-start_approximate)/double(CLOCKS_PER_SEC)*1000;

   			if(approximate_neighbour.begin()->second/true_neighbour.begin()->second > fraction)
   			{
   				fraction=approximate_neighbour.begin()->second/true_neighbour.begin()->second;
   				max_fraction=  to_string(approximate_neighbour.begin()->second) + string("/") + to_string(true_neighbour.begin()->second);    			
   			}

   			/* WRITE OUTPUT */
   			output << querypoint->name_accessor() <<endl;
   			output << "R-near neighbours:" <<endl;
   			for(neighbours_iterator = neighbours_rangesearch.begin(); neighbours_iterator != neighbours_rangesearch.end(); neighbours_iterator++)
   			{
   				output << (*neighbours_iterator)->name_accessor() << endl;
   			}
   			output << "LSH nearest neighbour: " << approximate_neighbour.begin()->first->name_accessor() << endl;
   			output << "distanceLSH: " << approximate_neighbour.begin()->second << endl;
   			output << "True nearest neighbour: " << true_neighbour.begin()->first->name_accessor() << endl;
   			output << "distanceTrue: " << true_neighbour.begin()->second <<endl;
   			output << "tLSH: " << (stop_approximate-start_approximate)/double(CLOCKS_PER_SEC)*1000 << endl;
   			output << "tTrue: " << (stop_true-start_true)/double(CLOCKS_PER_SEC)*1000 << endl;
   			getchar();
   			/* DELETE STRUCT */
   			delete querypoint;
   		}
   		output << "Max fraction: " << max_fraction << " = " << fraction<<endl;
   		output << "Mean time tLSH " << mean_time/query_number << endl;
   		output.close();
   		queryset.close();
   		cout << "Do you want to continue with different query_set?" <<endl;

   	}while (getline(cin,queryset_path)); 


   	return 0;
   }