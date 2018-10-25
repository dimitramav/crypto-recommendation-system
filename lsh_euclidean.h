#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>
#include <list>
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

	

typedef unordered_map <string, list <DataVector *> >HashTable;

void make_table_ht(double ** ,int,int,int);
void make_table_hv(vector <double> **, int, int, int);
void print_table_ht(double ** , int ,int);
void print_table_hv(vector <double> **, int, int, int);
void print_hashtable(HashTable *);
double find_radius(string line);
double	vectors_distance(vector<int> ,vector<int>);
set <DataVector *> rangesearch(int,int,HashTable *, double,DataVector *);
map <DataVector *, double> approximateNN( int,int,HashTable * ,DataVector *);
map <DataVector *, double> trueNN(vector <DataVector *>, DataVector *);


