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
#include <map>
#include <set>
#include <list>
#include "lsh_euclidean.h"
#include "helper.h"


using namespace std;

/* helper functions */

void make_table_hnumber(double ** ht,int w,int rows,int columns)
{
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0,w);
	for (int i=0; i<rows; i++)
		for(int x=0; x<columns; x++)
			ht[i][x]=distribution(generator);
	return;
}

void print_table_hnumber(double ** ht,int rows,int columns)
{
	for (int i=0; i<rows; i++)
	{
		for(int x=0; x<columns; x++)
			cout << ht[i][x]<< "   ";
			cout << endl;
	}
	return;	
}

void make_table_hvector( vector <double> ** hv, int dimension, int rows, int columns)
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



void print_table_hvector( vector <double> ** hv, int dimension, int rows, int columns)
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

void print_hashtable(unordered_map<string,DataVector *> * htable)
{
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

int find_dimension(string line)
{
   	int d=0;
   	for(int i = 0; i < line.length(); i++)
   	{
   		if (isblank(line[i]))
   		{
			d++;               //find dimension
		}
	}
}

string find_metric(string line)
{
	istringstream ss(line);
	string::size_type sz;
	istream_iterator<std::string> begin(ss), end;    		
	vector<string> arrayTokens(begin, end);
	if(arrayTokens[0].compare("@metric")==0)
	{
   		return arrayTokens[1]; //return metric
   	}
   	return "{default_euclidean}"; //default is euclidean
}


double	vectors_distance(vector<double> a, vector<double> b)
{
   vector<double>	auxiliary;

	std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
		[](double element1, double element2) {return pow((element1-element2),2);});

	return  sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} 


set <DataVector *> rangesearch(int L, int k,HashTable * hashtables,double radius,DataVector *querypoint)
{
	set <DataVector * > neighbours; //to avoid duplicates
	for (int i=0;i<L;i++)
	{
		
		string key= querypoint->key_accessor(i,k);
		for (auto v : hashtables[i][key])
		{
			double euclidean_distance=vectors_distance(querypoint->point_accessor(),v->point_accessor());
			if(euclidean_distance<radius)
			{
				//cout << v->name_accessor() << endl;
				neighbours.insert(v);
			}
		}
	}
	return neighbours;
	
}

map <DataVector *,double> approximateNN(int L, int k,HashTable * hashtables,DataVector *querypoint)
{
	DataVector * neighbour;
	double euclidean_distance;
	map <DataVector * , double> nearest_neighbour;
	int points_checked;
	double minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN
	for (int i=0;i<L;i++)
	{
		points_checked=0;
		string key= querypoint->key_accessor(i,k);
		for ( auto v: hashtables[i][key])
		{
			if (points_checked> 3*L)
			{
				break;
			}	 
			euclidean_distance=vectors_distance(querypoint->point_accessor(),v->point_accessor());
			if(euclidean_distance<minimum_distance)
			{
				minimum_distance=euclidean_distance;
				neighbour=v;
			}
			//cout << it->second->name_accessor() << " "<< euclidean_distance << "||";
			points_checked++;
		}
	}
	cout << endl;
	cout << neighbour->name_accessor() <<" is near " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;
	
}

map <DataVector *,double> trueNN(vector <DataVector *> dataset, DataVector * querypoint)
{
	DataVector * neighbour;
	map <DataVector * , double> nearest_neighbour;
	double euclidean_distance;
	double minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN
	for (int i=0;i<dataset.size();i++)
	{
		euclidean_distance=vectors_distance(querypoint->point_accessor(),dataset[i]->point_accessor());
		if(euclidean_distance<minimum_distance)
		{
			minimum_distance=euclidean_distance;
			neighbour=dataset[i];
		}
	}
	cout << neighbour->name_accessor() <<" has nearest neighbour " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;

}