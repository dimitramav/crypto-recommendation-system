#include <string>
#include <unordered_map>

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
	




