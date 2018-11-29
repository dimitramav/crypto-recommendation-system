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
#include "datastructs.h"


/* DataVector */

int DataVector::no_of_dataset = 0;

DataVector::DataVector(){}
DataVector::DataVector(vector <double> new_vector,int cluster)
{
	v=new_vector;
	name="external_centroid" + to_string(cluster);

}
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
void DataVector::print_vector()
{

	vector<double>::iterator it;


	cout << name << " contains:";
	for ( it=v.begin() ; it < v.end(); it++ )
		cout << " " << *it;

	cout << endl;
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

/* Euclidean */

Euclidean::Euclidean(vector <double> line,string vector_name,int k, int L,vector <double> ** hv, double ** t, int w)
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
	//copy(g.begin(),g.end(),std::ostream_iterator<double>(std::cout, "  " ));

}
Euclidean::~Euclidean(){}



/* Cosine */

Cosine::Cosine(vector <double> line,string vector_name,int k, int L,vector <double> ** hr)
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

void Cluster::print_cluster()
{
	for (auto v : cluster_content)
	{
		cout << v->name_accessor() << "  ";
	}
	cout<< cluster_content.size() << endl;
	getchar();

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
	int dimension = cluster_content.front()->point_accessor().size(); //dimension of mean vector
	for (int i =0; i<dimension;i++)  //initialize mean vector
	{
		mean_vector.push_back(0);
	}
	for ( auto v : cluster_content)
	{
		for (int i =0; i< v->point_accessor().size();i++)
		{
			mean_vector[i]= mean_vector[i] +v->point_accessor()[i];
		}
	}
	for (int i =0; i<dimension;i++)
	{
		mean_vector[i]= mean_vector[i] / cluster_content.size();
		//cout << mean_vector[i] << "  " ;
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



/*GENERAL FUNCTIONS*/


double ** make_table_hnumber(double ** ht,int w,int rows,int columns)
{
	ht = new double * [rows];
	for(int i = 0;i<rows;i++)
		ht[i] = new double[columns];
	default_random_engine generator;
	uniform_real_distribution<double> distribution(0.0,w);
	for (int i=0; i<rows; i++)
		for(int x=0; x<columns; x++)
			ht[i][x]=distribution(generator);
		return ht;
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

	vector <double> ** make_table_hvector( vector <double> ** hv, int dimension, int rows, int columns)
	{	
		hv = new vector <double> * [rows];
		for(int i = 0;i<rows;i++)
			hv[i] = new vector <double> [columns];

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
			return hv;
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

				}			
				return;
			}


			int find_dimension(string line)
			{
				stringstream ss(line);
				int d =0;
				int i;
				istringstream input(line);
				string word;
				while(getline(input,word,','))
				{
    	d++;  //number of commas is the dimension of the vector
    }
    return d;
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

   void find_parameter(string line,map<string,double> &parameters)
   {
   	string delimiter = ":";
   	size_t pos = 0;
   	string parameter_name;
   	while ((pos = line.find(delimiter)) != string::npos) {
   		parameter_name = line.substr(0, pos);
   		line.erase(0, pos + delimiter.length());
   	}
	parameters[parameter_name] = stod(line); //insert parameter in the map
	return;
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




int initialize_params(string configuration_path, map <string,double>& parameters)
{
	string line;               
	ifstream configuration;
	configuration.open(configuration_path.c_str());  //convert string to const char *
	if (!configuration.is_open())
	{
		return 0;
	}
	while (getline(configuration,line))
	{
		find_parameter(line,parameters);
	}
	return 1;
}

vector <double> string_to_stream(string line)// convert line to a stream
{
	stringstream in( line );
	int counter = 0;
	string word; 
	vector <double> v;

	while(getline(in,word,',')){
		if(counter ==0 )
		{
			counter++; 
			continue;  //ignore dimension
		}
		v.push_back(atof(word.c_str()));
	}
	return v;

}

void initialize_tables(string metric, double ** & ht,vector <double> ** & hr, vector <double> ** & hv, int dimension,int w,int number_of_hashtables ,int number_of_hashfunctions)
{
	if(metric.compare("cosine")==0) //cosine metric
	{
				/* 2. TABLE FOR r */	
		hr = make_table_hvector(hr,dimension,number_of_hashtables,number_of_hashfunctions);
	}
	else
	{
				/* 2. TABLE FOR t */
		ht = make_table_hnumber(ht,w,number_of_hashtables,number_of_hashfunctions);
				//print_table_hnumber(ht,1,parameters["number_of_hashfunctions"]);
				/*4. TABLE FOR v */
		hv = make_table_hvector(hv,dimension,number_of_hashtables,number_of_hashfunctions);
				//print_table_hvector(hv,dimension,1,parameters["number_of_hashfunctions"]);
	}
	return;
}

DataVector * create_datapoint(string line,string metric, double **  ht,vector <double> **  hr, vector <double> ** hv, int w,int number_of_hashtables ,int number_of_hashfunctions)
{
	DataVector * datapoint;
	if(metric.compare("cosine")==0)
	{
		vector <double> v = string_to_stream(line);
		datapoint = new Cosine(v,"item_id",number_of_hashfunctions,number_of_hashtables,hr);
	}			
	else
	{
		vector <double> v = string_to_stream(line);
		datapoint = new Euclidean(v,"item_id",number_of_hashfunctions,number_of_hashtables,hv,ht,w);
	}
	return datapoint;
}