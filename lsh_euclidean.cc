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




/* DataVector */
int DataVector::no_of_queryset =0 ;
int DataVector::no_of_dataset = 0;


Euclidean::Euclidean(string line,string vector_name,int k, int L,vector <double> ** hv, double ** t, int w)
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
	for (int x=0;x<k; x++){
		for (int i=0;i<L;i++)
		{
			g.push_back(floor((inner_product(v.begin(),v.end(),hv[x][i].begin(),0) + t[x][i] )/w));   
		
		}
	}
	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));

}
Euclidean::~Euclidean(){}

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

vector<double> DataVector::point_accessor()
{
	return v;
}
string DataVector::name_accessor(){
	return name;
}
DataVector::DataVector(){}
DataVector::~DataVector()
{
	cout << "delete " << name_accessor()<<endl;
}

void DataVector::print_vector()
{

	vector<double>::iterator it;


	cout << name << " contains:";
	for ( it=v.begin() ; it < v.end(); it++ )
		cout << " " << *it;

	cout << endl;
}
