#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <set>
#include <utility>
#include <fstream>
#include <cmath>
using namespace std;


class DataVector{
protected:
	string name;
	vector<double> v;
	vector<int> h;
	int is_centroid; 
	pair <int,double> cluster_number; //belongs to a cluster (is not centroid)
	pair <int,double> neighbour_cluster;
	int assigned; 
public:
	DataVector();
	~DataVector();
	static int no_of_dataset;
	void print_vector(ofstream &);
	string key_accessor(int,int);  
	string name_accessor();
	vector <double> point_accessor();
	void set_point(vector <double>);
	void set_centroid();
	void remove_centroid();
	int centroid_accessor();
	void change_cluster_number(int,double);
	void change_neighbour_cluster(int,double);
	pair <int,double> cluster_number_accessor();
	pair <int,double> neighbour_cluster_accessor();
	int is_assigned();
	void change_assigned(int);
};


class Euclidean : public DataVector{
	public:
		Euclidean(vector <double>,string,int,int,vector <double> **, double **,int);
		~Euclidean();
	
};

class Cosine : public DataVector{
	public:
		Cosine(vector <double>,string,int,int,std::vector<double> **);
		~Cosine();

};

class Cluster{
	protected:
		DataVector * centroid;
		list<DataVector*> cluster_content;
		bool centroid_is_external;
		int modified; 
	public:
		Cluster(DataVector *);
		~Cluster();
		DataVector * centroid_accessor();
		list <DataVector *> content_accessor();
		void add_to_cluster(DataVector *);
		void print_cluster(ofstream &);
		void create_external_centroid(DataVector *);
		void change_external_centroid(vector <double>);
		vector <double> kmeans(int);
		void remove_from_cluster(DataVector *);
		bool is_external();
		void make_external();
		void set_update(int);
		int is_updated();
		
};

typedef unordered_map <string, list <DataVector *> >HashTable;

//general functions for both lsh and cube
double ** make_table_hnumber(double ** ,int,int,int);
vector <double> ** make_table_hvector(vector <double> **, int, int, int);
void print_table_hnumber(double ** , int ,int);
void print_table_hvector(vector <double> **, int, int, int);
double	vectors_distance(string,vector<double> ,vector<double>);
double euclidean_distance(vector<double>,vector<double>);
double cosine_distance(vector<double>, vector <double>);
int find_dimension(string);
void find_parameter(string ,map<string,double> & ,map <string,string> & , string );
int initialize_params(string , map <string,double>& , map <string,string>& ,string);
vector <double> string_to_stream(string );
void initialize_tables(string,double ** & , vector <double> ** & , vector <double> ** & ,int,int,int,int);
DataVector * create_datapoint(string ,string , double ** ,vector <double> ** , vector <double> ** , int ,int ,int);
int hamming_distance(int, int);
string string_to_bitstring(string );
int bitstring_to_int(string ,string );
int cube_key(string ,string);
int read_arguements(int, char **, string &, string &, int & , string & ,string &, string & );
void print_output(int,int,int,ofstream &,vector <Cluster * > &,int,vector <double> &,string,double,int);
void call_initialization(int , vector <DataVector *> &, vector <Cluster *> &,int ,string );
void call_update(int ,int , vector <Cluster *> & ,int ,int ,double ** ,vector <double> ** , vector <double> ** ,int ,string);
double call_assignment(int , list <DataVector *> * ,int ,int ,int , int, vector <Cluster *> ,string ,vector <DataVector *>,vector <DataVector*>,HashTable *,int);
void delete_data(vector <DataVector *> &,string , vector <double> ** ,double ** ,vector <double> ** ,int);
void change_data(vector <Cluster *> &,vector <DataVector *> &);
void reset_distances(vector <DataVector *> & dataset_vector);
map <DataVector *,double> trueNN(vector <DataVector *> , DataVector * ,string );
void read_crypto(vector<string> &);




