#include <iostream>
#include <getopt.h>
#include <string>
#include <unistd.h>

using namespace std;

int main(int argc, char * argv[])
{
	string input_path,configuration_path,a,d,output_path;
	int option;
	static struct option long_options[] = {
		{"i",required_argument,NULL  ,  'i' },
		{"c",required_argument,NULL,  'c' },
		{"d",required_argument, NULL,  'd' },
		{"complete",optional_argument,NULL, 'a'},
		{"o",required_argument,NULL  ,  'o' },
		{NULL,0,NULL, 0}
	};

	/* 1. READ ARGUREMENTS */
	while ((option = getopt_long_only (argc, argv, "i:c:a:o:d:",long_options,NULL)) != -1)
	{
		switch (option)
		{
			case 'i':
			input_path = optarg;
			break;
			case 'c':
			configuration_path = optarg;
			break;
			case 'a':
			complete = 1;
			break;
			case 'd':
			d = optarg;
			break;
			case 'o':
			output_path = optarg;
			break;
			default: 
			cout << "Given parameters are wrong. Try again." << endl;
			return -1;
		}
	}

	if ( input_path.length()==0 || configuration_path.length()==0 || output_path.length()==0) 
	{
		cout << "File parameters are mandatory. Try again." << endl;
		return -1;
	}

	cout << input_path << " " << configuration_path << " " << a << " " << d << " "<< output_path<< endl;	
	
}