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
	istringstream input(line);
	string word;
	while(getline(input,word,','))
	{
    	d++;  //number of commas is the dimension of the vector
    }
    return d;
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
		int x = rand()%2;
		values.push_back(x);
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


int cube_key(string key,string metric)
{
	int int_key;
	if(metric.compare("cosine")==0)
	{
		int_key= bitstring_to_int( key,metric);
	}
	else
	{
		string bitstring = string_to_bitstring(key);
		int_key=bitstring_to_int(bitstring,metric);
	}
	return int_key;
}



int read_arguements(int argc, char ** argv, string & input_path, string & configuration_path, int & complete, string & metric,string & output_path)
{
	int option;
	static struct option long_options[] = {
		{"i",required_argument,NULL  ,  'i' },
		{"c",required_argument,NULL,  'c' },
		{"d",required_argument, NULL,  'd' },
		{"complete",optional_argument,NULL, 'a'},
		{"o",required_argument,NULL  ,  'o' },
		{NULL,0,NULL, 0}
	};
	while ((option = getopt_long_only (argc, argv, "i:c:a:o:d:",long_options,NULL)) != -1)
	{
		switch (option)
		{
			case 'i':
			input_path = optarg;
			break;
			case 'c':
			configuration_path = optarg;
			break;
			case 'a':
			complete = 1;
			break;
			case 'd':
			metric = optarg;
			break;
			case 'o':
			output_path = optarg;
			break;
			default: 
			cout << "Given parameters are wrong. Try again." << endl;
			return -1;
		}
	}

	if ( input_path.length()==0 || configuration_path.length()==0 || output_path.length()==0) 
	{
		cout << "File parameters are mandatory. Try again." << endl;
		return -1;
	}
	return 0;
}

void print_output(ofstream & output,vector <Cluster * > & cluster_vector ,int complete ,vector <double> & silhouette_vector,string metric,double total_time,int total_dataset)
{
	double mean_silhouette=0.0;
	output << "Metric : " << metric << endl;
	for (int i=0;i<cluster_vector.size();i++)
	{
		output << "CLUSTER-" << i+1 << " " ;
		output << "{ size : " << cluster_vector[i]->content_accessor().size() << " ,";
		output << " centroid : ";
		cluster_vector[i]->centroid_accessor()->print_vector(output);
		output << " }"<< endl;
		output << endl;

	}
	output << "Clustering time: " << total_time << endl;
	output << "Silhouette: ";
	for (int x=0;x <silhouette_vector.size();x++)
	{
		mean_silhouette+=silhouette_vector[x];
		output << silhouette_vector[x]/cluster_vector[x]->content_accessor().size() << " ";
	}
	output << endl;
	mean_silhouette/=total_dataset;
	output << mean_silhouette << endl;

	return;
}