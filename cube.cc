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
#include "cube_funcs.h"

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
	int first_line=0;
	int probes;
	int hypercube_dimension;
	int points_to_check;
	int int_key;
	string line;
	string vector_value;
	int number_of_hashfunctions;
	double	 n=0;
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
	int start_approximate;
	int stop_approximate;
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
		if(first_line==0)
		{
			metric=find_metric(line);
			if(metric.compare("{default_euclidean}")!=0)
			{
				first_line++;
				continue; //first line was just metric so we need to read the next line
			}
		}
		//first vector
		if(d==0)
		{
			d=find_dimension(line);
			if(metric.compare("{cosine}")==0) //cosine metric
			{
				/* 2. TABLE FOR r */
				hr = new vector <double> * [1];
				for(int i = 0;i<1;i++)
					hr[i] = new vector <double> [number_of_hashfunctions];
				make_table_hvector(hr,d,1,number_of_hashfunctions);
				//print_table_hvector(hr,d,1,number_of_hashfunctions);
			}
			else
			{
				/* 2. TABLE FOR t */
				ht = new double * [1];
				for(int i = 0;i<1;i++)
					ht[i] = new double[number_of_hashfunctions];
				make_table_hnumber(ht,w,1,number_of_hashfunctions);
				print_table_hnumber(ht,1,number_of_hashfunctions);
				/*4. TABLE FOR v */
				hv = new vector <double> * [1];
				for(int i = 0;i<1;i++)
					hv[i] = new vector <double> [number_of_hashfunctions];
				make_table_hvector(hv,d,1,number_of_hashfunctions);
				//print_table_hvector(hv,d,number_of_hashtables,number_of_hashfunctions);
			}
		}
		if(metric.compare("{cosine}")==0)
		{
			n++;
			datapoint = new Cosine(line,"item_id",number_of_hashfunctions,1,hr);
		}		
		else
		{
			n++;
			datapoint = new Euclidean(line,"item_id",number_of_hashfunctions,1,hv,ht,w);

		}
		dataset_vectors.push_back(datapoint);    
		
	}

	/* 2. CREATE HYPERCUBE */
	hypercube_dimension= int(pow(2.0,number_of_hashfunctions));
	list <DataVector * >  hypercube[hypercube_dimension];

	/* 3. FILL HYPERCUBE */
	for (unsigned int x=0;x<dataset_vectors.size();x++)
	{
		DataVector * datapoint=dataset_vectors[x];
		if(metric.compare("{cosine}")==0)
		{
			string key = datapoint->key_accessor(0,number_of_hashfunctions);
			int_key= bitstring_to_int( key,metric);
			string name = datapoint->name_accessor();
			hypercube[int_key].push_back(datapoint);
		}
		else
		{
			string key = datapoint->key_accessor(0,number_of_hashfunctions);
			string bitstring = string_to_bitstring(key);
			int_key=bitstring_to_int(bitstring,metric);
			hypercube[int_key].push_back(datapoint);
 		}



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
 				for(int i = 0;i<1;i++)
 				{
 					delete [] hr[i];
 				}

 			}
 			else
 			{
 				//delete t & v table
 				for(int i = 0;i<1;i++)
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
		first_line=0;
   		while (getline(queryset, line))  //read dataset line by line
   		{

   			//see if radius is defined 
   			if (first_line==0)
   			{
   				radius=find_radius(line);
   				output.open (output_path);
   				first_line++;
   				continue;

   			}
   			query_number++;
   			if(metric.compare("{cosine}")==0)
   			{
   				querypoint = new Cosine(line,"item_idS",number_of_hashfunctions,1,hr);

   			}
   			else
   			{
   				querypoint = new Euclidean(line,"item_idS",number_of_hashfunctions,1,hv,ht,w);
   			}
   			/* NN ALGORITHMS */

   			//a. cube rangesearch
   			neighbours_rangesearch=cube_rangesearch(points_to_check,probes,number_of_hashfunctions,hypercube_dimension,hypercube,radius,querypoint,metric);
   			if(!neighbours_rangesearch.empty()) // no near neighbours
   			{
   				//b. approximate
   				start_approximate=clock();
   				approximate_neighbour=cube_approximateNN(points_to_check,probes,number_of_hashfunctions,hypercube_dimension,hypercube,querypoint,metric);
   				stop_approximate=clock();
   			}

   			//c. trueNN
   			int start_true=clock();
   			true_neighbour=trueNN(dataset_vectors,querypoint,metric);
   			int stop_true = clock();

   			//d. times
   			if(!neighbours_rangesearch.empty()) // no near neighbours
   			{
   				mean_time+=(stop_approximate-start_approximate)/double(CLOCKS_PER_SEC)*1000;

   				if(approximate_neighbour.begin()->second/true_neighbour.begin()->second > fraction)
   				{
   					fraction=approximate_neighbour.begin()->second/true_neighbour.begin()->second;
   					max_fraction=  to_string(approximate_neighbour.begin()->second) + string("/") + to_string(true_neighbour.begin()->second);    			
   				}
   			}

   			output << querypoint->name_accessor() <<endl;
   			output << "R-near neighbours:" <<endl;
   			if(!neighbours_rangesearch.empty())
   			{
   				for(neighbours_iterator = neighbours_rangesearch.begin(); neighbours_iterator != neighbours_rangesearch.end(); neighbours_iterator++)
   				{
   					output << (*neighbours_iterator)->name_accessor() << endl;
   				}
   				output << "LSH nearest neighbour: " << approximate_neighbour.begin()->first->name_accessor() << endl;
   				output << "distanceLSH: " << approximate_neighbour.begin()->second << endl;
   				output << "tLSH: " << (stop_approximate-start_approximate)/double(CLOCKS_PER_SEC)*1000 << endl;
   			}
   			else
   				output << "element has no near neighbours" << endl;

   			output << "True nearest neighbour: " << true_neighbour.begin()->first->name_accessor() << endl;
   			output << "distanceTrue: " << true_neighbour.begin()->second <<endl;
   			output << "tTrue: " << (stop_true-start_true)/double(CLOCKS_PER_SEC)*1000 << endl;
   			/* DELETE STRUCT */
   			delete querypoint;

   		}
   		output << "Max fraction: " << max_fraction << " = " << fraction<<endl;
   		output << "Mean time tLSH " << mean_time/query_number << endl;	
   		output.close();
   		queryset.close();
   		cout << "Do you want to continue with different query_set?" <<endl;

   	}while (getline(cin,queryset_path)); 

   	dataset.close();
   	return 0;
   }