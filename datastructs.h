#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <set>
#include <utility>
#include <fstream>
using namespace std;


class DataVector{
protected:
	string name;
	vector<double> v;
	vector<int> h;
	int is_centroid; 
	pair <int,double> cluster_number; //belongs to a cluster (is not centroid)
	pair <int,double> neighbour_cluster; 
public:
	//virtual void func() const=0;
	//DataVector(string,string,int,int,vector <double> **, double **,int);
	DataVector();
	DataVector(vector <double>,int);
	~DataVector();
	static int no_of_dataset;
	void print_vector();
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
		void print_cluster();
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
string find_metric(string);	
void find_parameter(string,map <string,double> &);
int initialize_params(string , map <string,double> & );
vector <double> string_to_stream(string );
void initialize_tables(string,double ** & , vector <double> ** & , vector <double> ** & ,int,int,int,int);
DataVector * create_datapoint(string ,string , double ** ,vector <double> ** , vector <double> ** , int ,int ,int);
int hamming_distance(int, int);
string string_to_bitstring(string );
int bitstring_to_int(string ,string );
int cube_key(string ,string);



//algorithms
map <DataVector *, double> trueNN(vector <DataVector *>, DataVector *,string);
void random_initialization( vector <DataVector *> &,vector <Cluster *> &,int);
void lloyds_assignment(vector <DataVector *> &,vector <Cluster *> &,string);
void lloyds_update(vector <Cluster *> &,double ** ,vector <double> **,vector <double> **,int,string);
void silhouette_evaluation(vector <DataVector *> &,vector <Cluster *> &,string);
void plus_initialization(vector <DataVector *> &, vector <Cluster *> &, int,string);
void set_centroid(vector <DataVector *> & ,vector <Cluster*> & , int);
void pam_update(vector <Cluster *> &,string);
void lsh_assignment(int,int,HashTable *,vector <Cluster *> &,string,vector <DataVector *> &,double ** ,vector <double> **,vector <double> ** ,int);
int is_nearest(double ,DataVector * ,int);
int update_cluster_vector(DataVector * ,double , vector <Cluster *> &,int);
double find_lsh_radius(vector <DataVector *> &, string);
void cube_assignment(list <DataVector *> * ,int ,int ,int , int ,vector <Cluster *> & ,string ,vector <DataVector *> & ,double ** ,vector <double> **,vector <double> **,int);


