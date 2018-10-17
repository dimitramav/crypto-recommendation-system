#include <iostream>
#include <fstream>
#include <string> 
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int main(int argc, char * argv[])
{
	string dataset_path,queryset_path,output_path;	
	ifstream dataset,queryset;		
	int option;
	int k=-1;
	int L=-1;

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

   	if (k == -1)
   		k = 4;
   	if (L == -1)
   		L = 5;

   	/* 2. READ DATASET */
   	dataset.open(dataset_path.c_str());  //convert string to const char *
   	if (!dataset.is_open())
    {
    	cout << "Failed to open file." << endl;
    	return 1;
    }
             
   	string line;
	while (getline(dataset, line))  //read dataset line by line
	{
    	
	}

	return 0;
}