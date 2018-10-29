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
#include "datastructs.h"
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

   int bitstring_to_int(string key)
   {
   	string::iterator end_pos = remove(key.begin(), key.end(), ' ');
   	key.erase(end_pos, key.end());
   	unsigned int int_key=stoi(key, 0, 2);
   	return int_key;
   }


   double vectors_distance(string metric,vector<double> a, vector<double> b)
   {
   	double distance;
   	if(metric.compare("{cosine}")==0)
   	{
   		distance=cosine_distance(a,b);
   	}
   	else
   	{
   		distance=euclidean_distance(a,b);
   	}
   	return distance;
   }
   double	euclidean_distance(vector<double> a, vector<double> b)
   {
   	vector<double>	auxiliary;

	std::transform (a.begin(), a.end(), b.begin(), std::back_inserter(auxiliary),//
		[](double element1, double element2) {return pow((element1-element2),2);});

	return  sqrt(std::accumulate(auxiliary.begin(), auxiliary.end(), 0.0));
} 

int hamming_distance(int x, int y) {
	int z  = x ^ y;
	int r = 0;
	for (; z > 0; z >>= 1) {
		r += z & 1;
	}
	return r;
}

double cosine_distance(vector<double> a, vector <double> b)
{
	double av = 0.0;
	double bv = 0.0;
	double dot = 0.0;
	for (unsigned int i = 0; i < a.size(); ++i) {
		dot+=a[i]*b[i];
		av += a[i] * a[i];
		bv += b[i] * b[i];
	}
	double normA = sqrt(av);
	double normB = sqrt(bv);
	double cosine_distance=dot/(normB*normA);
	return 1.0 - cosine_distance;
}

set <DataVector *> rangesearch(int L, int k,HashTable * hashtables,double radius,DataVector *querypoint,string metric)
{
	set <DataVector * > neighbours; //to avoid duplicates
	for (int i=0;i<L;i++)
	{
		
		string key= querypoint->key_accessor(i,k);
		/*string::size_type sz;
		unsigned long long_key;
		if(metric.compare("{cosine}")==0)
		{
			string::iterator end_pos = remove(key.begin(), key.end(), ' ');
			key.erase(end_pos, key.end());
			long_key=stoull(key, 0, 2);
		}*/
		for (auto v : hashtables[i][key])
		{
			double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
			if(distance<radius)
			{
				cout << v->name_accessor() << endl;
				neighbours.insert(v);
			}
		}
	}
	return neighbours;
	
}
set <DataVector *> cube_rangesearch(int M,int probes,int k, int vertices,list <DataVector *> * hypercube,double radius,DataVector * querypoint,string metric)
{
	set <DataVector * > neighbours; //to avoid duplicates
	int points_checked=0;
	string key= querypoint->key_accessor(0,k);
	int int_key= bitstring_to_int( key);
	for(int i=0; i<vertices; ++i) 
	{
		//cout << "i " <<i << "vs long_key" << long_key << endl;
		//cout << "hamming " << hamming_distance(i,long_key) << " vs " << probes << endl;
		if(hamming_distance(i,int_key)<=probes){
			probes--;
			points_checked=0;
			for (auto v : hypercube[i])
			{

				if(points_checked<M)
				{
					points_checked++;
					double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
					if(distance<radius)
					{
						//cout << v->name_accessor() << endl;
						neighbours.insert(v);
					}
				}
				else
				{
					break; // move on to the next cell (enough points from this cell)
				}
			}
		}
		else
		{
			continue; //go to next cell
		}
	}
	return neighbours;
}

map <DataVector *,double> approximateNN(int L, int k,HashTable * hashtables,DataVector *querypoint,string metric)
{
	DataVector * neighbour;
	double distance;
	map <DataVector * , double> nearest_neighbour;
	int points_checked;
	double minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN'
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
			distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
			if(distance<minimum_distance)
			{
				minimum_distance=distance;
				neighbour=v;
			}
			points_checked++;
		}
	}
	cout << neighbour->name_accessor() <<" is near " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;
	
}

map <DataVector *,double> cube_approximateNN(int M,int probes,int k, int vertices,list <DataVector *> * hypercube,DataVector * querypoint,string metric)
{
	DataVector * neighbour;
	double minimum_distance=numeric_limits<double>::max(); 
	map <DataVector * , double> nearest_neighbour;
	int points_checked=0;
	string key= querypoint->key_accessor(0,k);
	int int_key= bitstring_to_int(key);
	for(int i=0; i<vertices; ++i) 
	{
		//cout << "i " <<i << "vs long_key" << long_key << endl;
		//cout << "hamming " << hamming_distance(i,long_key) << " vs " << probes << endl;
		if(hamming_distance(i,int_key)<=probes){
			probes--;
			points_checked=0;
			for (auto v : hypercube[i])
			{

				if(points_checked<M)
				{
					points_checked++;
					double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
					if(distance<minimum_distance)
					{
						minimum_distance=distance;
						neighbour=v;
					}
					
				}
				else
				{
					break; // move on to the next cell (enough points from this cell)
				}
			}
		}
		else
		{
			continue; //go to next cell
		}
	}
	cout << neighbour->name_accessor() <<" is near " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;
}

map <DataVector *,double> trueNN(vector <DataVector *> dataset, DataVector * querypoint,string metric)
{
	DataVector * neighbour;
	map <DataVector * , double> nearest_neighbour;
	double distance;
	double minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN
	for (int i=0;i<dataset.size();i++)
	{
		distance=vectors_distance(metric,querypoint->point_accessor(),dataset[i]->point_accessor());
		if(distance<minimum_distance)
		{
			minimum_distance=distance;
			neighbour=dataset[i];
		}
	}
	cout << neighbour->name_accessor() <<" has nearest neighbour " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;

}