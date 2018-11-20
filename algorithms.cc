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
	int neighbour_cluster;
	int points_checked=0;
	map <DataVector * , double> nearest_neighbour;
	double distance;
	double first_minimum_distance=numeric_limits<double>::max(); //initialize minimum distance for approximateNN
	double second_minimum_distance = numeric_limits<double>::max();
	for (unsigned int i=0;i<dataset.size();i++)
	{
		if(points_checked <10000)
		{
			distance=vectors_distance(metric,querypoint->point_accessor(),dataset[i]->point_accessor());
			if(distance<first_minimum_distance)
			{
				second_minimum_distance=first_minimum_distance;
				first_minimum_distance=distance;
				querypoint->change_cluster_number(i);
				neighbour=dataset[i];
			}
			else if(distance>first_minimum_distance && distance < second_minimum_distance)
			{
				second_minimum_distance = distance;
				querypoint->change_neighbour_cluster(i); 
			}
			points_checked++;
			//cout << dataset[i]->name_accessor()<< "      " << distance << endl;
		}
		else
			break;
	}
	//cout << neighbour->name_accessor() <<" has nearest neighbour " << minimum_distance << endl;
	nearest_neighbour.insert ( pair<DataVector *,double>(neighbour,first_minimum_distance) );
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
	vector <double> silhouette_vector; 
	for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
	{
		centroid_vector.push_back(cluster_vector[i]->centroid_accessor());
	}
	int counter =0;
	double new_objective_distance = 0.0;
	double previous_objective_distance = 1.0;
	do
	{	
		if (counter !=0)
		{
			previous_objective_distance = new_objective_distance;
		}
		new_objective_distance  = 0.0;
		for (unsigned int i=0;i<dataset_vector.size();i++)  //for each datapoint , find nearest centroid
		{
			if(dataset_vector[i]->cluster_number_accessor()!=-1)
			{
				int old_cluster = dataset_vector[i]->cluster_number_accessor();
				//cout << "old_cluster " << old_cluster << endl;
				cluster_vector[old_cluster]->remove_from_cluster(dataset_vector[i]);
			}
			true_neighbour=trueNN(centroid_vector,dataset_vector[i],metric);
			//cout<< "true : "<<true_neighbour.begin()->first->name_accessor() << " :" << true_neighbour.begin()->second << endl;
					//cout << true_neighbour.begin()->first->name_accessor()<< "vs "<< cluster_vector[cluster]->centroid_accessor()->name_accessor() << endl	;
			int new_cluster = dataset_vector[i]->cluster_number_accessor();
			cluster_vector[new_cluster]->add_to_cluster(dataset_vector[i]); //add point to cluster	
			new_objective_distance += true_neighbour.begin()->second * true_neighbour.begin()->second;
			//getchar();
					
		}
		//getchar();
		/*for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
		{
			cluster_vector[i]->print_cluster();
			getchar();
		}*/
		cout << "objective_distance " << new_objective_distance << "/"<< previous_objective_distance<< endl;	
		lloyds_update(cluster_vector);
		for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
		{
			centroid_vector[i] = cluster_vector[i]->centroid_accessor();
		}
		counter++;
	}while(new_objective_distance!=	previous_objective_distance);
}

void lloyds_update(vector <Cluster *> & cluster_vector)
{
	vector <double > centroid_vector;
	DataVector * centroid_point;
	for(unsigned int i=0;i<cluster_vector.size();i++)  //vector with centroids for compatibility reasons
	{

		//cluster_vector[i]->centroid_accessor()->print_vector();
		//getchar();
		centroid_vector = cluster_vector[i]->kmeans(i);
		if (cluster_vector[i]->is_external()==0)
		{
			cluster_vector[i]->make_external();
			centroid_point = new DataVector(centroid_vector,i);
			cluster_vector[i]->create_external_centroid(centroid_point);
		}
		else
		{
			cluster_vector[i]->change_external_centroid(centroid_vector);
		}
		//cluster_vector[i]->centroid_accessor()->print_vector();
		//getchar();
	}
}

void silhouette_evaluation(vector <DataVector *> & dataset_vector,vector <Cluster *> & cluster_vector,string metric)
{
	double silhouette;
	int min_cluster;
	int neighbour_cluster;
	double final_silhouette; 
	double distance_a;
	double distance_b;
	double mean_silhouette;
	mean_silhouette = 0.0;
	for (unsigned int i=0;i<dataset_vector.size();i++)  //for each datapoint , find nearest centroid
	{		
		min_cluster = dataset_vector[i]->cluster_number_accessor();
		for (auto point_a : cluster_vector[min_cluster]->content_accessor() )
        {		
        	distance_a += vectors_distance(metric,dataset_vector[i]->point_accessor(),point_a->point_accessor());
    	}
    	distance_a = distance_a/cluster_vector[min_cluster]->content_accessor().size();
    	neighbour_cluster = dataset_vector[i]->neighbour_cluster_accessor();
    	if (neighbour_cluster == -1)
    		neighbour_cluster = min_cluster;
    	for (auto point_a : cluster_vector[neighbour_cluster]->content_accessor() )
    	{
        	
        	distance_b += vectors_distance(metric,dataset_vector[i]->point_accessor(),point_a->point_accessor());
    	}
    	distance_b = distance_b/cluster_vector[neighbour_cluster]->content_accessor().size();
    	
    	final_silhouette = (distance_b - distance_a)/max(distance_b,distance_a);
    	mean_silhouette+=final_silhouette;
    	//cout <<dataset_vector[i]->name_accessor()<< " distance_a " <<distance_a <<"distance_b " << distance_b<< " "<< final_distance << endl;
	}
	mean_silhouette = mean_silhouette/dataset_vector.size();
	//cout << mean_silhouette<< endl;
}