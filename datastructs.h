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
	string has_centroid; //belongs to a cluster (is not centroid)
	set<DataVector* > cluster; //if it is centroid=> list of datavectors
public:
	//virtual void func() const=0;
	//DataVector(string,string,int,int,vector <double> **, double **,int);
	DataVector();
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
	string key_accessor(int,int);  
	string name_accessor();
	vector <double> point_accessor();
	void set_centroid();
	void remove_centroid();
	int centroid_accessor();
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
		string centroid;
		list<DataVector*> cluster_content;
	public:
		Cluster(DataVector *);
		~Cluster();
		string centroid_accessor();
		list <DataVector *> content_accessor();
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

