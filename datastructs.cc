#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include <string>
#include <random>
#include <cmath>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>	
#include <cmath> 
#include <numeric>
#include <limits>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <cstdlib>
#include <getopt.h>
#include "datastructs.h"


/* DataVector */

int DataVector::no_of_dataset = 0;

DataVector::DataVector(){}

DataVector::~DataVector()
{
	//cout << "delete " << name_accessor()<<endl;
}

string DataVector::key_accessor(int current_hashtable, int k)  //convert g vector for a particular hash table L to string 
{
	//cout << "whole g " << endl;

	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));
	//getchar();
	vector<int>::const_iterator first = h.begin() + k*current_hashtable;
	vector<int>:: const_iterator last = h.begin() + k*current_hashtable +k;
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

void DataVector::set_point(vector <double> new_v)
{
	v = new_v;
	return;
}
void DataVector::print_vector(ofstream & output)
{

	vector<double>::iterator it;


	for ( it=v.begin() ; it < v.end(); it++ )
		output << " " << *it;
	return;
}

void DataVector::set_centroid()
{
	is_centroid=1; 
}

void DataVector::remove_centroid()
{
	is_centroid=0;
}

int DataVector::centroid_accessor()
{
	return is_centroid;
}

void DataVector::change_cluster_number(int new_cluster,double distance)
{
	cluster_number.first= new_cluster;
	cluster_number.second=distance;
	return;
}

void DataVector::change_neighbour_cluster(int new_cluster,double distance)
{
	neighbour_cluster.first= new_cluster;
	neighbour_cluster.second = distance;
	return;
}

pair <int,double> DataVector::cluster_number_accessor()
{
	return cluster_number;

}

pair <int,double> DataVector::neighbour_cluster_accessor()
{
	return neighbour_cluster;

}

int DataVector::is_assigned()
{
	return assigned;
}

void DataVector::change_assigned(int i)
{
	assigned=i;
}
/* Euclidean */

Euclidean::Euclidean(vector <double> line,string vector_name,int k, int L,vector <double> ** hv, double ** t, int w)
{
	/* 1.initialize name */
	if (vector_name.compare("item_id") == 0)
	{
		no_of_dataset++;
		name="item_id" + to_string(no_of_dataset);
	}
	else
	{
		name = vector_name;
	}
	/* 2.initialize vector */
	
	v = line;

	/* 3. initialize array of h */
	for (int x=0;x<L; x++){
		for (int i=0;i<k;i++)
		{
			h.push_back(floor((inner_product(v.begin(),v.end(),hv[x][i].begin(),0.0) + t[x][i]  )/w));   
		}
	}

	cluster_number.first=-1;
	cluster_number.second = numeric_limits<double>::max();
	neighbour_cluster.first = -1;
	neighbour_cluster.second = numeric_limits<double>::max();
	is_centroid = 0; 
	assigned = 0;
	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));

}
Euclidean::~Euclidean(){}



/* Cosine */

Cosine::Cosine(vector <double> line,string vector_name,int k, int L,vector <double> ** hr)
{
	/* 1.initialize name */
	if (vector_name.compare("item_id") == 0)
	{
		no_of_dataset++;
		name="item_id" + to_string(no_of_dataset);
	}
	else
	{
		name = vector_name;
	}
	
	/* 2.initialize vector */
	
	v=line;

	/* 3. initialize array of h */
	for (int x=0;x<L; x++){
		for (int i=0;i<k;i++)
		{
			if((inner_product(v.begin(),v.end(),hr[x][i].begin(),0.0)) >= 0)
				h.push_back(1);  
			else 
				h.push_back(0); 
		}
	}
	cluster_number.first=-1;
	cluster_number.second = numeric_limits<double>::max();
	neighbour_cluster.first = -1;
	neighbour_cluster.second = numeric_limits<double>::max();
	is_centroid = 0; 
	assigned = 0;
	//copy(v.begin(),v.end(),std::ostream_iterator<double>(std::cout, "  " ));

}

Cosine::~Cosine(){}


/* Cluster */

Cluster::Cluster(DataVector * point){
	centroid = point;
	centroid_is_external = 0; 
	
	//cluster_content.push_back(point);
}

Cluster::~Cluster(){}

DataVector * Cluster::centroid_accessor()
{
	return centroid;
}

list <DataVector *> Cluster::content_accessor()
{
	return cluster_content;
}

void Cluster::add_to_cluster(DataVector * point)
{
	cluster_content.push_back(point);
}

void Cluster::print_cluster(ofstream & output)
{
	for (auto v : cluster_content)
	{
		output << v->name_accessor() << "  ";
	}
	return;

}

void Cluster::create_external_centroid(DataVector * new_centroid)
{
	centroid = new_centroid;
	return;
}

void Cluster::change_external_centroid(vector <double> centroid_vector)
{
	centroid->set_point(centroid_vector);

}
vector <double> Cluster::kmeans(int cluster)
{
	vector <double> mean_vector;
	int dimension;
	if(cluster_content.size()==0)
	{
		mean_vector.push_back(0.0);

	}
	else
	{
		dimension = cluster_content.front()->point_accessor().size(); //dimension of mean vector
		for (int i =0; i<dimension;i++)  //initialize mean vector
		{
			mean_vector.push_back(0);
		}
		for ( auto v : cluster_content)
		{	
			for (unsigned int i =0; i< v->point_accessor().size();i++)
			{
				mean_vector[i]= mean_vector[i] +v->point_accessor()[i];
			}
		}
		for (int i =0; i<dimension;i++)
		{
			if(cluster_content.size()==0)
				mean_vector[i]=0.0;
			else
				mean_vector[i]= mean_vector[i] / cluster_content.size();
		//cout << mean_vector[i] << "  " ;
		}
	}
	//getchar();
	return mean_vector; 
}

void Cluster::remove_from_cluster(DataVector * point)
{
	cluster_content.remove(point);
}

bool Cluster::is_external()
{
	return centroid_is_external;
}

void Cluster::make_external()
{
	centroid_is_external = 1; 
}

void Cluster::set_update(int i)
{
	modified = i;
	return;
}

int Cluster::is_updated()
{
	return modified;
}



