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
#include "cube_funcs.h"


using namespace std;

/*CUBE FUNCTIONS */
   int bitstring_to_int(string key,string metric)
   {
   	
   		string::iterator end_pos = remove(key.begin(), key.end(), ' ');
   		key.erase(end_pos, key.end());

   		//cout << "key "<< key<< endl;
   		unsigned int int_key=stoi(key, 0, 2);
   		//cout << "int_key " << int_key << endl;
   		return int_key;
   }

   string string_to_bitstring(string key)
   {
   	int n;
   	vector<int> values;
   	stringstream stream(key);
   	while(stream >> n)
   	{
   		values.push_back(abs(n%2));
   	}
   	stringstream result;
   	copy(values.begin(), values.end(), std::ostream_iterator<int>(result, " "));
   	string bitstring;
   	bitstring=result.str().c_str();
   	return bitstring;
   }

  

int hamming_distance(int x, int y) {
	int z  = x ^ y;
	int r = 0;
	for (; z > 0; z >>= 1) {
		r += z & 1;
	}
	return r;
}




set <DataVector *> cube_rangesearch(int M,int probes,int k, int vertices,list <DataVector *> * hypercube,double radius,DataVector * querypoint,string metric)
{
	set <DataVector * > neighbours; //to avoid duplicates
	int points_checked=0;
	int int_key;
	string key= querypoint->key_accessor(0,k);
	if(metric.compare("{cosine}")==0)
	{
		int_key= bitstring_to_int( key,metric);
	}
	else
	{
		string bitstring = string_to_bitstring(key);
		int_key=bitstring_to_int(bitstring,metric);
	}
	for(int i=0; i<vertices; ++i) 
	{
		//cout << "i " <<i << "vs long_key" << int_key << endl;
		//cout << "hamming " << hamming_distance(i,int_key) << " vs " << probes << endl;
		if(hamming_distance(i,int_key)<=probes){
			probes--;
			points_checked=0;
			for (auto v : hypercube[i])
			{

				if(points_checked<M)
				{
					points_checked++;
					double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
					if(distance<radius || radius == 0)
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



map <DataVector *,double> cube_approximateNN(int M,int probes,int k, int vertices,list <DataVector *> * hypercube,DataVector * querypoint,string metric)
{
	DataVector * neighbour;
	double minimum_distance=numeric_limits<double>::max(); 
	map <DataVector * , double> nearest_neighbour;
	int points_checked=0;
	int int_key;
	string key= querypoint->key_accessor(0,k);
	if(metric.compare("{cosine}")==0)
	{
		int_key= bitstring_to_int( key,metric);
	}
	else
	{
		string bitstring = string_to_bitstring(key);
		int_key=bitstring_to_int(bitstring,metric);
	}
	for(int i=0; i<vertices; ++i) 
	{
		// cout << "i " <<i << "vs int_key" << int_key << endl;
		// cout << "hamming " << hamming_distance(i,int_key) << " vs " << probes << endl;
		if(hamming_distance(i,int_key)<=probes){
			probes--;
			points_checked=0;
			for (auto v : hypercube[i])
			{
				cout << v->name_accessor() << endl;
				if(points_checked<M)
				{
					points_checked++;
					double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
					// cout << "distance " << distance << "vs minimum_distance" << minimum_distance;
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
	//cout << neighbour->name_accessor() <<" is near " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;
}

