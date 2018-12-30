#include "datastructs.h"
#include <iostream>
#include <string>
#include <sstream>
/* CRYPTOCURRENCIES */

int read_coins(string coins_path, vector<string> & cryptocurrencies)
{
	ifstream coins_file;
	string line;
	coins_file.open(coins_path.c_str());  //convert string to const char *
	if (!coins_file.is_open())
	{
		cout << "Fail to open coins file" << endl;
		return 0;
	}
	while (getline(coins_file,line))
	{
		cryptocurrencies.push_back(line);
	}

	return 1;

}

int read_lexicon(string lexicon_path, map<string,double> &lexicon)
{
	ifstream lexicon_file;
	string line;
	lexicon_file.open(lexicon_path.c_str());  //convert string to const char *
	string key;
	string::size_type sz;     // alias of size_t
	double value;
	if (!lexicon_file.is_open())
	{
		cout << "Fail to open lexicon file" << endl;
		return 0;
	}
	int counter=0;
	while (getline(lexicon_file,line))
	{
		stringstream linestream(line);
		string data;
		while( getline(linestream, data, '\t'))
		{
			if(counter%2==0)
			{
				key=data;
			}
			else
			{ 
				value=stod(data,&sz);
			}
			lexicon[key]=value;
			counter++;
		}

	}
	return 1;
}
