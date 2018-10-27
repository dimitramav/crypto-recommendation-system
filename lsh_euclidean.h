#include <string>
#include <unordered_map>

using namespace std;


class DataVector{
protected:
	string name;
	vector<double> v;
	vector<int> h;
public:
	//virtual void func() const=0;
	//DataVector(string,string,int,int,vector <double> **, double **,int);
	DataVector();
	~DataVector();
	static int no_of_queryset;
	static int no_of_dataset;
	void print_vector();
	virtual string key_accessor(int,int);  //is implemented in derrived classes 
	string name_accessor();
	vector <double> point_accessor();
};


class Euclidean : public DataVector{
	public:
		Euclidean(string,string,int,int,vector <double> **, double **,int);
		~Euclidean();
		string key_accessor(int,int);
		void print();
	
};

class Cosine : public DataVector{
	public:
		Cosine(string,string,int,int,std::vector<double> **);
		~Cosine();
		string key_accessor(int,int);
		void print();

};
typedef unordered_map <string, list <DataVector *> >HashTable;
	



