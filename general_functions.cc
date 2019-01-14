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
#include "algorithms.h"
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


void find_parameter(string line,map<string,double> & clustering_parameters,map <string,string> & file_parameters, string config)
   {
   	string delimiter = ":";
   	size_t pos = 0;
   	string parameter_name;
   	while ((pos = line.find(delimiter)) != string::npos) {
   		parameter_name = line.substr(0, pos);
   		line.erase(0, pos + delimiter.length());
   	}
   	if(config.compare("file_config")==0)
   	{
   		file_parameters[parameter_name] = line;
   	}
   	else
		clustering_parameters[parameter_name] = stod(line); //insert parameter in the map
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




int initialize_params(string configuration_path, map <string,double>& clustering_parameters, map <string,string>& file_parameters,string config)
{
	string line;               
	ifstream configuration;
	configuration.open(configuration_path.c_str());  //convert string to const char *
	if (!configuration.is_open())
	{
		cout << "Fail to open configuration file" << endl;
		return 0;
	}
	while (getline(configuration,line))
	{
		find_parameter(line,clustering_parameters,file_parameters,config);

	}
	if(config.compare("file_config")==0) //file configuration doesn't need more proccessing
		return 1;
	if(clustering_parameters["number_of_hashfunctions"]>18)
	{
		cout << "Cannot allocate memory for the array" << endl;
		return 0;
	}
	/*if(pow(2.0,clustering_parameters["k"])<clustering_parameters["number_of_clusters"])
	{
		cout << "Number of clusters needs to be smaller" << endl;
		return 0;
	}*/
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

DataVector * create_datapoint(string name,int id,vector<double> v,string metric, double **  ht,vector <double> **  hr, vector <double> ** hv, int w,int number_of_hashtables ,int number_of_hashfunctions)
{
	DataVector * datapoint;
	if(metric.compare("cosine")==0)
	{
		datapoint = new Cosine(name,v,id,number_of_hashfunctions,number_of_hashtables,hr);
	}			
	else
	{
		datapoint = new Euclidean(name,v,id,number_of_hashfunctions,number_of_hashtables,hv,ht,w);
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



int read_arguements(int argc, char ** argv, string & input_path, string & configuration_path, int & validate, string & output_path, string & config_path1, string & config_path2)
{
	int option;
	static struct option long_options[] = {
		{"d",required_argument,NULL  ,  'd' },
		{"conf",required_argument,NULL, 'c'},
		{"files",required_argument,NULL, 'e'},
		{"o",required_argument,NULL,  'o' },
		{"validate",optional_argument,NULL, 'a'},
		{NULL,0,NULL, 0}
	};
	while ((option = getopt_long_only (argc, argv, "d:o:a:",long_options,NULL)) != -1)
	{
		switch (option)
		{
			case 'd':
				input_path = optarg;
				break;
			case 'a':
				validate = 1;
				break;
			case 'o':
				output_path = optarg;
				break;
			case 'c':
				config_path1 = optarg;
				break;
			case 'e':
				config_path2 = optarg;
				break;
			default: 
			cout << "Given parameters are wrong. Try again." << endl;
			return -1;
		}
	}
	if ( input_path.length()==0 || config_path1.length()==0 || output_path.length()==0 ||config_path2.length()==0) 
	{
		cout << "File parameters are mandatory. Try again." << endl;
		return -1;
	}
	return 0;
}

void print_output(int initialization,int assignment,int update,string output_path,vector <Cluster * > & cluster_vector ,int complete ,vector <double> & silhouette_vector,string metric,double total_time,int total_dataset)
{
	double mean_silhouette=0.0;
	ofstream output;
	output.open(output_path.c_str());  //convert string to const char *
	output << "I" << initialization <<"A" << assignment <<"U" <<update <<endl;
	output << "Metric : " << metric << endl;
	for (unsigned int i=0;i<cluster_vector.size();i++)
	{
		output << "CLUSTER-" << i+1 << " " ;
		if (complete==1)
		{
			output << "{";
			cluster_vector[i]->print_cluster(output);
			output << "}";
		}
		output << endl;
		output << "{ size : " << cluster_vector[i]->content_accessor().size() << " ,";
		output << " centroid : ";
		if(update == 2)
			output << cluster_vector[i]->centroid_accessor()->id_accessor();
		cluster_vector[i]->centroid_accessor()->print_vector(output);
		output << " }"<< endl;
		output << endl;

	}
	output << "Clustering time: " << total_time << endl;
	output << "Silhouette: ";
	for (unsigned int x=0;x <silhouette_vector.size();x++)
	{
		mean_silhouette+=silhouette_vector[x];
		output << silhouette_vector[x]/cluster_vector[x]->content_accessor().size() <<" ";
	}
	output << endl;
	mean_silhouette/=total_dataset;
	output << mean_silhouette << endl;
	output<< endl;
	return;
}


void call_initialization(int initialization, vector <DataVector *> & dataset_vector, vector <Cluster *> & cluster_vector,int k,string metric)
{
	if(initialization == 1)
	{
		cout << "random_initialization" << endl;
		random_initialization(dataset_vector,cluster_vector,k);
	}	
	else
	{
		cout << "plus_initialization" << endl;
		plus_initialization(dataset_vector,cluster_vector,k,metric);
	}
	return;
}

void call_update(int update,int assignment, vector <Cluster *> & cluster_vector,int k,int L,double ** ht,vector <double> ** hv, vector <double> ** hr,int w,string metric)
{
	if(update == 1)
	{
		cout << "lloyds_update" << endl;
		if (assignment ==3)
			lloyds_update(cluster_vector,k,1,ht, hv,hr,w,metric); //hypercube
		else
			lloyds_update(cluster_vector,k,L,ht, hv,hr,w,metric);
	}	
	else if (update == 2)
	{
		cout << "pam_update" << endl;
		pam_update(cluster_vector,metric);
	}
	return;
}

double call_assignment(int assignment, int k, vector <Cluster *> cluster_vector,string metric,vector <DataVector *> dataset_vector,vector <DataVector*> centroid_vector,HashTable * hashtables_vector,int L)
{
	double new_objective_distance;
	if (assignment==1)
	{
		cout << "lloyds_assignment" << endl;
		new_objective_distance = lloyds_assignment(dataset_vector,cluster_vector,metric,centroid_vector);
	}
	else if (assignment ==2)
	{
		cout << "lsh_assignment" << endl;
		new_objective_distance = lsh_assignment(L,k,hashtables_vector,cluster_vector,metric,dataset_vector,centroid_vector);

	}
	return new_objective_distance;
}	

void delete_data( vector <DataVector *> &dataset_vector,string metric, vector <double> ** hr,double ** ht,vector <double> ** hv,int L)
{
	vector <DataVector *> ::iterator dataset_iterator;
	//delete dataset
   	for(dataset_iterator = dataset_vector.begin(); dataset_iterator != dataset_vector.end(); dataset_iterator++) 
   	{   
   		delete *(dataset_iterator);
   	}
 	if(metric.compare("cosine")==0) //cosine metric
 	{
 		//delete r table
 		for(int i = 0;i<L;i++)
 		{
 			delete [] hr[i];
 		}

 	}
 	else
 	{
 		//delete t & v table
 		for(int i = 0;i<L;i++)
 		{
 			delete [] ht[i];
 			delete [] hv[i];
 		}
 		delete [] ht;
 		delete [] hv;
	}
	return;

}

void change_data(vector <Cluster *> & cluster_vector,vector <DataVector *> & dataset_vector)
{
	vector <Cluster *> ::iterator cluster_iterator;
	//delete clusters
	for(cluster_iterator = cluster_vector.begin(); cluster_iterator != cluster_vector.end(); cluster_iterator++)    
   	{
   		if((*(cluster_iterator))->is_external()==1)
   		{
   			delete (*(cluster_iterator))->centroid_accessor();
   		}
   		delete *(cluster_iterator);
   	}
   	cluster_vector.clear();
   	//redefine datavectors
   	for (unsigned int i=0;i<dataset_vector.size();i++)
   	{
   		dataset_vector[i]->change_cluster_number(-1,numeric_limits<double>::max());
		dataset_vector[i]->change_neighbour_cluster(-1,numeric_limits<double>::max());
   	}

   	return;
}

void reset_distances(vector <DataVector *> & dataset_vector)
{
	for (unsigned int i=0;i<dataset_vector.size();i++)
   	{
   		dataset_vector[i]->change_assigned(0);
   		//dataset_vector[i]->change_neighbour_cluster(-1,numeric_limits <double> ::max());
   	}
   	return;

}

int initialize_datapoints_ready_tweets_vector(string input_path,string metric,double ** & ht,vector <double> ** & hr, vector <double> ** & hv,int w,int number_of_hashtables ,int number_of_hashfunctions, vector <DataVector *> &ready_tweets_vector)
{
	string line;
	ifstream input;
	vector <double> v;
	input.open(input_path.c_str());  //convert string to const char *
	DataVector * datapoint;
	int dimension = 0; 
   	if (!input.is_open())
   	{
   		cout << "Failed to open file." << endl;
   		return 0;
   	}
   	int id = 0;
	while (getline(input, line))  //read dataset line by line
	{
		//first vector
		if(dimension==0)
		{
			dimension=find_dimension(line);
			/* 5. INITIALIZE TABLES */
			initialize_tables(metric,ht,hr,hv,dimension,w,number_of_hashtables,number_of_hashfunctions);

		}
		v= string_to_stream(line);
		datapoint = create_datapoint("twitter",id,v,metric,ht,hr,hv,w,number_of_hashtables,number_of_hashfunctions);
		ready_tweets_vector.push_back(datapoint); 
		id++;  
		v.clear();
	}
	return 1;
}

void initialize_datapoints_vectors(string name,map<int,vector<double>> uj, string metric,double **  ht,vector <double> ** hr, vector <double> ** hv,int w,int number_of_hashtables ,int number_of_hashfunctions, vector <DataVector *> &datapoints_uj_vector)
{
	DataVector * datapoint;
	vector <double> v;
	for(auto user: uj)
	{
		v = user.second;
		datapoint = create_datapoint(name,user.first,v,metric,ht,hr,hv,w,number_of_hashtables,number_of_hashfunctions);
		datapoints_uj_vector.push_back(datapoint); 
	}
}


int extract_id(string str)
{
	size_t i = 0;
	for ( ; i < str.length(); i++ )
	{ 
		if ( isdigit(str[i]) ) break; 
	}

	// remove the first chars, which aren't digits
	str = str.substr(i, str.length() - i );

// convert the remaining text to an integer
	int id = atoi(str.c_str());
	return id;
}


double mypower(vector <double> u)
{
	double sum = 0.0;
	for (auto element: u)
		sum+=element*element;
	return sum;
}