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
#include <functional>
#include <algorithm>	
#include <cmath> 
#include <numeric>
#include <limits>
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

			void print_hashtable(unordered_map<string,DataVector *> * htable){

				cout << "mymap's buckets contain:\n";
				for ( unsigned i = 0; i < htable->bucket_count(); ++i) {
					cout << "bucket #" << i << " contains:";
					for ( auto local_it = htable->begin(i); local_it!= htable->end(i); ++local_it )
						cout << " " << local_it->first << ":" << local_it->second->name_accessor();
					cout << std::endl;

				}
				cout << endl;
			}

			double find_radius(string line)
			{
				istringstream ss(line);
				string::size_type sz;
				istream_iterator<std::string> begin(ss), end;    		
				vector<string> arrayTokens(begin, end);
				if(arrayTokens[0].compare("Radius:")==0)
				{
   		return stod(arrayTokens[1],&sz); //cast string to double
   	}
   	return 0.0;
   }


   double	vectors_distance(vector<int> a, vector<int> b)
   {
   	vector<double>	auxiliary;

	std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
		[](int element1, int element2) {return pow((element1-element2),2);});

	return  sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} 


vector <DataVector *> rangesearch(int L, int k,vector <HashTable * > * hashtables,double radius,DataVector *querypoint)
{
	vector <DataVector * > neighbours;
	for (int i=0;i<L;i++)
	{
		int bucket_num=(*hashtables)[i]->bucket (querypoint->g_accessor(i,k)); //find the bucket with the key of the querypoint
		for ( auto it = (*hashtables)[i]->begin(bucket_num); it!= (*hashtables)[i]->end(bucket_num); ++it)
		{
			double euclidean_distance=vectors_distance(querypoint->point_accessor(),it->second->point_accessor());
			if(euclidean_distance<radius)
			{
				neighbours.push_back(it->second);
				cout<< it->second->name_accessor() ;
			}
			cout << endl;
		}
	}
	return neighbours;
	//getchar(); 
}

DataVector * approximateNN(int * minimum_distance,int L, int k,int current_hashtable,HashTable * hashtable,DataVector *querypoint)
{
	DataVector * nearest_neighbour;
	double euclidean_distance;
	int points_checked;
	int minimum_distance=numeric_limits<int>::max(); //initialize minimum distance for approximateNN
	
	points_checked=0;
	int bucket_num=hashtable->bucket (querypoint->g_accessor(current_hashtable,k)); //find the bucket with the key of the querypoint
	for ( auto it = hashtable->begin(bucket_num); it!= hashtable->end(bucket_num); ++it)
	{
		if (points_checked> 3*L)
		{
			return NULL;
		} 
		euclidean_distance=vectors_distance(querypoint->point_accessor(),it->second->point_accessor());
		if(euclidean_distance<*minimum_distance)
		{
			*minimum_distance=euclidean_distance;
			nearest_neighbour=it->second;
		}
		points_checked++;
	}
	return nearest_neighbour;
	
}

/* DataVector */
int DataVector::no_of_queryset =0 ;
int DataVector::no_of_dataset = 0;


DataVector::DataVector(string line,string vector_name,int k, int L,vector <double> ** hv, double ** t, int w)
{
	int number;
	/* 1.initialize name */
	if (vector_name.compare("item_id") == 0)
	{
		no_of_dataset++;
		name="item_id" + to_string(no_of_dataset);
	}
	else 
	{
		no_of_queryset++;
		name="item_idS" + to_string(no_of_queryset);
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

			g.push_back(floor((inner_product(v.begin(),v.end(),hv[x][i].begin(),0) + t[x][i] )/w));   //ALLAKSE TO GIA L PINAKES 
		}
	}
	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));

}

string DataVector::g_accessor(int current_hashtable, int k)  //convert g vector for a particular hash table L to string 
{
	//cout << "whole g " << endl;

	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));
	//getchar();
	vector<int>::const_iterator first = g.begin() + k*current_hashtable;
	vector<int>:: const_iterator last = g.begin() + k*current_hashtable +k;
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

vector<int> DataVector::point_accessor()
{
	return v;
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
