#include <string>
#include <vector>
#include <sstream>
#include "constants.h"

using namespace std;

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
	vector<FunctionH> h;
public:
	DataVector(string,string,int);
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
};
