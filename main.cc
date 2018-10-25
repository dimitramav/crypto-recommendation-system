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
#include "lsh_euclidean.h"

using namespace std;

int main(int argc, char * argv[])
{
	string dataset_path,queryset_path,output_path;	
	ifstream dataset,queryset;		
	int option;
	int k=-1;
	int L=-1;
	string line;
	string vector_value;
	int number_of_hashfunctions;
	int number_of_hashtables;
	int n=0;
	vector <double> ** hv;
	double radius;
	vector <DataVector *> dataset_vectors;
	set <DataVector *> neighbours_rangesearch;
	//vector <HashTable *> hashtables_vector;
	//unordered_map <string,DataVector*> test;


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

	/* 2. TABLE FOR t */
	double ** ht;
	ht = new double * [number_of_hashfunctions];
	for(int i = 0;i<number_of_hashtables;i++)
		ht[i] = new double[number_of_hashtables];
	make_table_ht(ht,w,number_of_hashtables,number_of_hashfunctions);
	//print_table_ht(ht,number_of_hashtables,number_of_hashfunctions);

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
		n++;
		//first vector
		if(d==0){
			for(int i = 0; i < line.length(); i++){
				if (isblank(line[i])){
					d++;               //find dimension
				}
			}
			/*4. TABLE FOR v */
			hv = new vector <double> * [number_of_hashfunctions];
			for(int i = 0;i<number_of_hashtables;i++)
				hv[i] = new vector <double> [number_of_hashtables];
			make_table_hv(hv,d,number_of_hashtables,number_of_hashfunctions);
			//print_table_hv(hv,d,number_of_hashtables,number_of_hashfunctions);
		}
		DataVector * datapoint = new DataVector(line,"item_id",number_of_hashfunctions,number_of_hashtables,hv,ht,w);
		dataset_vectors.push_back(datapoint);
		/*for (int i=0;i<number_of_hashtables; i++)
		{
			string key = datapoint->g_accessor(i,number_of_hashfunctions);
			string name = datapoint->name_accessor();
			//(hashtables_vector.at(i)->hashtable_accessor())[key]=name;
			//cout << (hashtables_vector.at(i)->hashtable_accessor())[key];
 			hashtables_vector[i]->insert(make_pair(key,datapoint));
 			//test.insert(make_pair(key,datapoint)); It works!

		}*/	
		//test.print_vector();   

		
	}
	/* 4. CREATE HASHTABLES 
	for (int i=0; i< number_of_hashtables ;i ++)
	{
		//HashTable * hash= new HashTable();
		HashTable * hash = new HashTable;
		hashtables_vector.push_back(hash);
	}*/
	HashTable hashtables_vector[number_of_hashtables];

	for (int x=0;x<dataset_vectors.size();x++)
	{
		DataVector * datapoint=dataset_vectors[x];
		for (int i=0;i< number_of_hashtables; i++)
		{
			string key = datapoint->g_accessor(i,number_of_hashfunctions);
			string name = datapoint->name_accessor();
			//hashtables_vector[i]->insert(make_pair(key,datapoint));
			hashtables_vector[i][key].push_back(datapoint);


		}
	}
	/* PRINT HASHTABLES 
	for (int i=0; i<number_of_hashtables; i++)
	{
		cout << i << " HASH TABLE"<< endl;
		print_hashtable(hashtables_vector[i]);
		getchar();
	}*/
	/* 5. READ QUERYSET */
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
   			n++;
   			continue;

   		}
   		DataVector * querypoint = new DataVector(line,"item_idS",number_of_hashfunctions,number_of_hashtables,hv,ht,w);
   		neighbours_rangesearch=rangesearch(number_of_hashtables,number_of_hashfunctions,hashtables_vector,radius,querypoint);
   		for (  set<DataVector *>::iterator it = neighbours_rangesearch.begin(); it != neighbours_rangesearch.end(); ++it) {
   			cout << (*it)->name_accessor();
   		}
   		approximateNN(number_of_hashtables,number_of_hashfunctions,hashtables_vector,querypoint);
   		trueNN(dataset_vectors,querypoint);
   		getchar();
   	}



   	return 0;
   }