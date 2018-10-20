#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include "lsh_euclidean.h"

using namespace std;


/* h function */

FunctionH::FunctionH(int k){

}





/* DataVector */
int DataVector::no_of_queryset =0 ;
int DataVector::no_of_dataset = 0;


DataVector::DataVector(string line,string vector_name,int k)
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
	for (int i=0;i<number_of_hashfunctions;i++)
	{
		cout << "hi" << endl;
	}
}

void DataVector::print_vector()
{

	vector<int>::iterator it;


	cout << name << " contains:";
	for ( it=v.begin() ; it < v.end(); it++ )
		cout << " " << *it;

	cout << endl;
}
