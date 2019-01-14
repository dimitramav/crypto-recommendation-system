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
	int id;
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
	int id_accessor();
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
	void point_mutator(int,double);
};


class Euclidean : public DataVector{
	public:
		Euclidean(string,vector <double>,int,int,int,vector <double> **, double **,int);
		~Euclidean();
	
};

class Cosine : public DataVector{
	public:
		Cosine(string,vector <double>,int,int,int,std::vector<double> **);
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

class Twitter{
	protected:
		int twitter_id;
		int user_id;
		double total_score;
		set<int> crypto_mentioned;
	public:
		Twitter(int,int,double,set<int>);
		int get_twitterid();
		int get_userid();
		double get_twitter_score();
		set<int> get_crypto_mentioned();
		~Twitter();
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
DataVector * create_datapoint(string ,int,vector <double> , double ** ,vector <double> ** , vector <double> ** , int ,int ,int);
int hamming_distance(int, int);
string string_to_bitstring(string );
int bitstring_to_int(string ,string );
int cube_key(string ,string);
int read_arguements(int, char **, string &, string &, int & , string & ,string &, string & );
void print_output(int,int,int,string,vector <Cluster * > &,int,vector <double> &,string,double,int);
void call_initialization(int , vector <DataVector *> &, vector <Cluster *> &,int ,string );
void call_update(int ,int , vector <Cluster *> & ,int ,int ,double ** ,vector <double> ** , vector <double> ** ,int ,string);
double call_assignment(int, int, vector <Cluster *>,string ,vector <DataVector *> ,vector <DataVector*> ,HashTable * ,int);
void delete_data(vector <DataVector *> &,string , vector <double> ** ,double ** ,vector <double> ** ,int);
void change_data(vector <Cluster *> &,vector <DataVector *> &);
void reset_distances(vector <DataVector *> & dataset_vector);
map <DataVector *,double> trueNN(vector <DataVector *> , DataVector * ,string );
int extract_id(string);
double mypower(vector <double> );

//twitter analysis
int read_coins(string , vector<string> &);
int read_lexicon(string, map<string,double> &);
int twitter_analysis(string , vector<Twitter* > &,map<string,double> &,vector<string> &,map<int,vector<int>> &,int &);
int check_coins(string, vector<string> & );
double calculate_score(string ,map<string,double> & );
void construct_uj(int , int , vector<Twitter *> , map<int,vector<int>> ,map< int,vector<double> > &);
void find_uknown_cryptos(map<int,vector<double>>,map<int,vector<int>> &);
void regulate(map<int,vector<double>> & ,map<int,vector<int>>,map<int,double> &);
int initialize_datapoints_ready_tweets_vector(string ,string ,double ** & ,vector <double> ** & , vector <double> ** & ,int ,int  ,int , vector <DataVector *> &);
void construct_cj(int,vector <Cluster *> , vector <Twitter *> ,	map<int,vector<int>>& ,map< int, vector<double> >&);
void initialize_datapoints_vectors(string,map<int,vector<double>> , string ,double ** ,vector <double> ** , vector <double> ** ,int ,int  ,int , vector <DataVector *> &);
void replace_uknown_cryptos(vector <DataVector *>,HashTable * ,map<int,vector<int>> ,map<int,double> ,int,int,int,string);
void recommend_best_cryptos(vector <DataVector *> ,map<int,vector<int>> ,int ,vector<string> ,int,ofstream & );
string get_cryptoname(int ,vector<string>);
void print_recommended_output(ofstream &,int ,int ,multimap<double,int,greater<double>>,vector<string> );
void clear_regulated_datapoints_vector(vector<DataVector *>&,map<int,vector<double>>);








