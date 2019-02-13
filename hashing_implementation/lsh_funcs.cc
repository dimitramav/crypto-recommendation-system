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
#include "lsh_funcs.h"


using namespace std;

/*LSH FUNCTIONS*/

set <DataVector *> rangesearch(int L, int k,HashTable * hashtables,double radius,DataVector *querypoint,string metric)
{
	set <DataVector * > neighbours; //to avoid duplicates
	for (int i=0;i<L;i++)
	{
		
		string key= querypoint->key_accessor(i,k);
		for (auto v : hashtables[i][key])
		{
			double distance=vectors_distance(metric,querypoint->point_accessor(),v->point_accessor());
			//cout << distance << "vs " << radius << endl;
			if(distance<radius || radius == 0)
			{
				//cout << v->name_accessor() << endl;
				neighbours.insert(v);
			}
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
	double minimum_distance=numeric_limits<int>::max(); //initialize minimum distance for approximateNN'
	for (int i=0;i<L;i++)
	{
		points_checked=0;
		string key= querypoint->key_accessor(i,k);
		for ( auto v: hashtables[i][key])
		{

			if (points_checked> 3*LIMITS)
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
	//cout << neighbour->name_accessor() <<" is near " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;
	
}