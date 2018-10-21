#include <string>
#include <vector>
#include <sstream>
#include "constants.h"

using namespace std;

void make_table_ht(double ** ,int,int,int);
void make_table_hv(vector <double> **, int, int, int);
void print_table_ht(double ** , int ,int);
void print_table_hv(vector <double> **, int, int, int);

class FunctionH{
private:
	vector<int> vh;
	int t;
public:
	FunctionH(int);
	~FunctionH();
};

class DataVector{
private:
	string name;
	vector<int> v;
	vector<int> h; //function g
public:
	DataVector(string,string,int,vector <double> **, double **,int);
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
};

