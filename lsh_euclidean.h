#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "constants.h"

using namespace std;


class DataVector{
private:
	string name;
	vector<int> v;
	vector<int> g;
public:
	DataVector(string,string,int,int,vector <double> **, double **,int);
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
	string g_accessor(int,int);
	string name_accessor();
	vector <int> point_accessor();
};

	

typedef unordered_map <string,DataVector * > HashTable;

void make_table_ht(double ** ,int,int,int);
void make_table_hv(vector <double> **, int, int, int);
void print_table_ht(double ** , int ,int);
void print_table_hv(vector <double> **, int, int, int);
void print_hashtable(HashTable *);
double find_radius(string line);
double	vectors_distance(vector<int> ,vector<int>);
vector <DataVector *> rangesearch(int,int,vector <HashTable *> *, double,DataVector *);
DataVector * approximateNN(int *, int,int,int,HashTable *,DataVector *);


