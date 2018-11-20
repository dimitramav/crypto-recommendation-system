#include <string>
#include <map>
#include <vector>
#include <unordered_map>
#include <list>
#include <fstream>
#include <set>
using namespace std;


class DataVector{
protected:
	string name;
	vector<double> v;
	vector<int> h;
	bool is_centroid; 
	double first_silhouette_distance; 
	double second_silhouette_distance; 
	int cluster_number; //belongs to a cluster (is not centroid)
	set<DataVector* > cluster; //if it is centroid=> list of datavectors
public:
	//virtual void func() const=0;
	//DataVector(string,string,int,int,vector <double> **, double **,int);
	DataVector();
	DataVector(vector <double>,int);
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
	string key_accessor(int,int);  
	string name_accessor();
	vector <double> point_accessor();
	void set_point(vector <double>);
	void set_centroid();
	void remove_centroid();
	int centroid_accessor();
	void change_cluster_number(int);
	int cluster_number_accessor();
	void set_first_distance(double);
	void set_second_distance(double);
	double get_first_distance();
	double get_second_distance();
};


class Euclidean : public DataVector{
	public:
		Euclidean(string,string,int,int,vector <double> **, double **,int);
		~Euclidean();
	
};

class Cosine : public DataVector{
	public:
		Cosine(string,string,int,int,std::vector<double> **);
		~Cosine();

};

class Cluster{
	protected:
		DataVector * centroid;
		list<DataVector*> cluster_content;
		bool centroid_is_external;
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
		
};

typedef unordered_map <string, list <DataVector *> >HashTable;

//general functions for both lsh and cube
double ** make_table_hnumber(double ** ,int,int,int);
vector <double> ** make_table_hvector(vector <double> **, int, int, int);
void print_table_hnumber(double ** , int ,int);
void print_table_hvector(vector <double> **, int, int, int);
double find_radius(string line);
double	vectors_distance(string,vector<double> ,vector<double>);
double euclidean_distance(vector<double>,vector<double>);
double cosine_distance(vector<double>, vector <double>);
int find_dimension(string);
string find_metric(string);	
void find_parameter(string,map <string,int> &);
int initialize_params(string , map <string,int> & );

//algorithms
map <DataVector *, double> trueNN(vector <DataVector *>, DataVector *,string);
void random_initialization( vector <DataVector *> &,vector <Cluster *> &,int);
void lloyds_assignment(vector <DataVector *> &,vector <Cluster *> &,string);
void lloyds_update(vector <Cluster *> &);
void silhouette_distance(vector <Cluster *> &,string);
void silhouette_evaluation(vector <DataVector *> &);


