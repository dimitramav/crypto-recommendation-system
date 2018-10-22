#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <random>
#include <cmath>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <sstream>
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

void print_hashtable(unordered_map<string,DataVector *> htable){

  	cout << "mymap's buckets contain:\n";
  	for ( unsigned i = 0; i < htable.bucket_count(); ++i) {
    	cout << "bucket #" << i << " contains:";
    	for ( auto local_it = htable.begin(i); local_it!= htable.end(i); ++local_it )
      		cout << " " << local_it->first << ":" << local_it->second->name_accessor();
    	cout << std::endl;

	}
	cout << endl;
}

/* HashTable */

HashTable::HashTable(){}

unordered_map<string,string> HashTable::hashtable_accessor(){
	return htable;
}

void HashTable::print_hashtable(){

	cout << "mymap contains:";
  	for ( auto it = htable.begin(); it != htable.end(); ++it )
    	cout << " " << it->first << ":" << it->second;
 	cout << endl;

  	cout << "mymap's buckets contain:\n";
  	for ( unsigned i = 0; i < htable.bucket_count(); ++i) {
    	cout << "bucket #" << i << " contains:";
    	/*for ( auto local_it = htable.begin(i); local_it!= htable.end(i); ++local_it )
      		cout << " " << local_it->first << ":" << local_it->second;
    	cout << std::endl;*/

	}
	cout << endl;
}


/* DataVector */
int DataVector::no_of_queryset =0 ;
int DataVector::no_of_dataset = 0;


DataVector::DataVector(string line,string vector_name,int k, int L,vector <double> ** hv, double ** t, int w)
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
	for (int x=0;x<L; x++){
		for (int i=0;i<k;i++)
		{

			g.push_back(floor((inner_product(v.begin(),v.end(),hv[x][i].begin(),0) + t[0][i] )/w));   //ALLAKSE TO GIA L PINAKES 
		}
	}
	//copy(h.begin(),h.end(),std::ostream_iterator<double>(std::cout, "  " ));

}

string DataVector::g_accessor(int L, int k)  //convert g vector for a particular hash table L to string 
{
	//cout << "whole g " << endl;

	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));
	//getchar();
	vector<int>::const_iterator first = g.begin() + k*L;
	vector<int>:: const_iterator last = g.begin() + k*L +k;
	vector<int> specific_g(first, last);
	//cout << "specific_g" << endl;
	//copy(specific_g.begin(),specific_g.end(),std::ostream_iterator<double>(std::cout, "  " ));
	//getchar();
	stringstream ss;
	for(size_t i = 0; i < specific_g.size(); ++i)
	{
  		if(i != 0)
    		ss << " ";
  		ss << specific_g[i];
		}
		string g_string = ss.str();
		return g_string;
}

string DataVector::name_accessor(){
	return name;
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
