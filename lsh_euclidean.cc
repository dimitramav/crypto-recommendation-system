#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <random>
#include <cmath>
#include <numeric>
#include <vector>
#include "lsh_euclidean.h"



using namespace std;

/* helper functions */

void make_table_ht(double ** ht,int w,int rows,int columns)
{
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0,w);
	for (int i=0; i<rows; i++)
		for(int x=0; x<columns; x++)
			ht[i][x]=distribution(generator);
   	return;
}

void print_table_ht(double ** ht,int rows,int columns)
{
	for (int i=0; i<rows; i++)
	{
		for(int x=0; x<columns; x++)
			cout << ht[i][x]<< "   ";
		cout << endl;
	}
   	return;	
}
void make_table_hv( vector <double> ** hv, int dimension, int rows, int columns)
{	
	default_random_engine generator;
  	normal_distribution<double> distribution(0.0,1.0);
	for (int i=0; i<rows; i++)
		for(int x=0; x<columns; x++)
		{
			for (int z=0; z<dimension; z++)
			{
  				hv[i][x].push_back(distribution(generator));  				
			}
		}
	return;
}

void print_table_hv( vector <double> ** hv, int dimension, int rows, int columns)
{	
	for (int i=0; i<rows; i++)
		for(int x=0; x<columns; x++)
		{
			for (int z=0; z<dimension; z++)
			{				
				copy(hv[i][x].begin(),hv[i][x].end(),std::ostream_iterator<double>(std::cout, "  " ));
			}
			getchar();
		}
	return;
}
/* h function */

FunctionH::FunctionH(int k){

}

FunctionH::~FunctionH(){

}




/* DataVector */
int DataVector::no_of_queryset =0 ;
int DataVector::no_of_dataset = 0;


DataVector::DataVector(string line,string vector_name,int k, vector <double> ** hv, double ** t, int w)
{
	int number;
	/* 1.initialize name */
	if (vector_name.compare("item_d") == 0)
	{
		no_of_queryset++;
		name="item_d" + to_string(no_of_queryset);
	}
	/* 2.initialize vector */
	
	// convert line to a stream
	stringstream in( line );
	while ( in >> number )
		v.push_back( number );

	/* 3. initialize array of h */
	for (int i=0;i<k;i++)
	{

		h.push_back(floor((inner_product(v.begin(),v.end(),hv[0][i].begin(),0) + t[0][i] )/w));   //ALLAKSE TO GIA L PINAKES 
	}
	copy(h.begin(),h.end(),std::ostream_iterator<double>(std::cout, "  " ));
	getchar();

}

DataVector::~DataVector()
{
	
}

void DataVector::print_vector()
{

	vector<int>::iterator it;


	cout << name << " contains:";
	for ( it=v.begin() ; it < v.end(); it++ )
		cout << " " << *it;

	cout << endl;
}
