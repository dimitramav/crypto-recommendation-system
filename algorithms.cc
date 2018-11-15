#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <random>
#include <cmath>
#include <numeric>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>	
#include <cmath> 
#include <numeric>
#include <limits>
#include <map>
#include <set>
#include <list>
#include <stdlib.h>
#include <ctime>
#include "datastructs.h"

map <DataVector *,double> trueNN(vector <DataVector *> dataset, DataVector * querypoint,string metric)
{
	DataVector * neighbour;
	int points_checked=0;
	map <DataVector * , double> nearest_neighbour;
	double distance;
	double minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN
	for (unsigned int i=0;i<dataset.size();i++)
	{
		if(points_checked <10000)
		{
			distance=vectors_distance(metric,querypoint->point_accessor(),dataset[i]->point_accessor());
			if(distance<minimum_distance)
			{
				minimum_distance=distance;
				neighbour=dataset[i];
			}
			points_checked++;
			cout << dataset[i]->name_accessor()<< "      " << distance << endl;
		}
		else
			break;
	}
	//cout << neighbour->name_accessor() <<" has nearest neighbour " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,minimum_distance) );
	return nearest_neighbour;

}


void random_initialization(vector <DataVector *> & dataset_vector,vector <Cluster *> & cluster_vector, int k)
{
	set<int> random_k;
	set<int>::iterator it;

	srand(time(NULL));
	while (random_k.size() < k )
	{
		random_k.insert(rand() % dataset_vector.size());
	}
	for(it = random_k.begin(); it != random_k.end(); it++)
    {
        dataset_vector[*it]->set_centroid();   //set centroid DELETE IT 
        Cluster * cluster = new Cluster(dataset_vector[*it]);  //initialize new cluster with the random centroid
        cluster_vector.push_back(cluster);
	}
}

void lloyds_assignment(vector <DataVector *> & dataset_vector,vector <Cluster *> & cluster_vector,string metric)
{
	map <DataVector *,double> true_neighbour;
	vector <DataVector *> centroid_vector; 
	for(unsigned int i=0;i<cluster_vector.size();i++)  //vector with centroids for compatibility reasons
	{
		centroid_vector.push_back(cluster_vector[i]->centroid_accessor());
	}
	
	for (unsigned int i=0;i<dataset_vector.size();i++)  //for each datapoint , find nearest centroid
	{
		true_neighbour=trueNN(centroid_vector,dataset_vector[i],metric);
		cout<< "true : "<<true_neighbour.begin()->first->name_accessor() << " :" << true_neighbour.begin()->second << endl;
		getchar();
	}
}