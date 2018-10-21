#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <sstream>  
#include <iterator>
#include "lsh_euclidean.h"
#include <cctype>
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
	const int w = 4;

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
		//first vector
		if(d==0){
			for(int i = 0; i < line.length(); i++){
				if (isblank(line[i])){
					d++;               //find dimension
				}
			}
			/* TABLE FOR v */
			vector <double> ** hv;
			hv = new vector <double> * [number_of_hashfunctions];
			for(int i = 0;i<number_of_hashtables;i++)
				hv[i] = new vector <double> [number_of_hashtables];
			make_table_hv(hv,d,number_of_hashtables,number_of_hashfunctions);
			print_table_hv(hv,d,number_of_hashtables,number_of_hashfunctions);
		}
		DataVector test = DataVector(line,"item_d",number_of_hashfunctions);
		//test.print_vector();    
		
	}  
    //Vector * test= new Vector();


	return 0;
}