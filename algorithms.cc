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
#include <random>
#include <ctime>
#include <utility>
#include <cmath>
#include "datastructs.h"
#include "algorithms.h"



double find_lsh_radius(vector <DataVector *> & centroid_vector,string metric)
{
	double centroid_distance;
	double radius = numeric_limits<double>::max();
	for(unsigned int x=0;x<centroid_vector.size();x++) //find radius
	{
		for(unsigned int y=0;y<centroid_vector.size();y++)
		{
			centroid_distance=vectors_distance(metric,centroid_vector[x]->point_accessor(),centroid_vector[y]->point_accessor());
			if(radius> centroid_distance && x!=y)
			{
				radius = centroid_distance;
			}
		}
	}
	return radius/=2;
	
}
int is_nearest(double distance,DataVector * querypoint,int new_cluster) //calculates the cluster and the neighbour cluster of a datavector
{
	int current_cluster = querypoint->cluster_number_accessor().first;
	double first_minimum_distance = querypoint->cluster_number_accessor().second;
	int neighbour_cluster = querypoint->neighbour_cluster_accessor().first;
	double second_minimum_distance = querypoint->neighbour_cluster_accessor().second;
	if(querypoint->is_assigned()==0)
	{
		first_minimum_distance = numeric_limits<double>::max();
	}
	if(distance<first_minimum_distance)
	{
		if(neighbour_cluster == new_cluster && querypoint->is_assigned()==1) //swap neighbour cluster with current cluster
			querypoint->change_neighbour_cluster(current_cluster,first_minimum_distance); 
		querypoint->change_cluster_number(new_cluster,distance);
		querypoint->change_assigned(1);
		//cout << querypoint->cluster_number_accessor().second << " " << querypoint->neighbour_cluster_accessor().second<< endl; 
		return 1;
	}
	else if(distance>first_minimum_distance && distance < second_minimum_distance)
	{
		if(current_cluster != new_cluster)  //avoid neighbour cluster to be equal to current cluster
			querypoint->change_neighbour_cluster(new_cluster,distance); 
	}
	return 0;
	
}

int is_second_nearest(double distance,DataVector * querypoint,int new_cluster)
{

	int current_cluster = querypoint->cluster_number_accessor().first;
	double first_minimum_distance = querypoint->cluster_number_accessor().second;
	double second_minimum_distance = querypoint->neighbour_cluster_accessor().second;
	if(distance>first_minimum_distance && distance < second_minimum_distance)
	{
		if(current_cluster != new_cluster)  //avoid neighbour cluster to be equal to current cluster
			querypoint->change_neighbour_cluster(new_cluster,distance); 
	}
	return 1;

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
		dataset_vector[*it]->set_centroid(); 
        Cluster * cluster = new Cluster(dataset_vector[*it]);  //initialize new cluster with the random centroid
        cluster_vector.push_back(cluster);
    }
}

void set_centroid(vector <DataVector *> & dataset_vector,vector <Cluster*> & cluster_vector, int new_centroid)
{
	dataset_vector[new_centroid]->set_centroid();
	Cluster * cluster = new Cluster(dataset_vector[new_centroid]);
	cluster_vector.push_back(cluster);
}
void plus_initialization(vector <DataVector *> & dataset_vector, vector <Cluster *> & cluster_vector, int k,string metric)
{
	random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dis(0, dataset_vector.size());
    vector <double> minimum_distance;
    double distance; 
    double max_sum;
    default_random_engine generator;
    //initialize vector with minimum distances
    for ( unsigned 	int point = 0;point<dataset_vector.size();point++)
    {
    	minimum_distance.push_back(numeric_limits<double>::max());
    }
    //initialize first centroid
    int starting_centroid = dis(gen);
    set_centroid(dataset_vector,cluster_vector,starting_centroid);
    for (int i=0; i<k-1 ;i++)  
    {
    	for ( unsigned int point = 0;point<dataset_vector.size();point++)
    	{
    		if(!dataset_vector[point]->centroid_accessor())  //if it is not centroid
    		{
    			for (unsigned int x=0;x<cluster_vector.size();x++)  //find minimum distance from current centroids
    			{
    				distance=vectors_distance(metric,dataset_vector[point]->point_accessor(),cluster_vector[x]->centroid_accessor()->point_accessor());
    				if(distance<minimum_distance[point])
    				{
    					minimum_distance[point] = distance; 
    					max_sum+= minimum_distance[point] * minimum_distance[point];
    				}
    			}
    		}
    		else 
    		{
    			minimum_distance[point] = 0.0;
    		}
    	}
    	uniform_real_distribution<> new_dis(0.0, max_sum);
    	double random_x = new_dis(generator);
    	max_sum=0;
    	for ( unsigned int point = 0;point<dataset_vector.size();point++)
    	{
    		max_sum+=minimum_distance[point];
    		if(max_sum > random_x)
    		{
    			int new_centroid = point; 
    			set_centroid(dataset_vector,cluster_vector,new_centroid);
    			break;
    		}
    	} 	
    }
}

double lloyds_assignment(vector <DataVector *> & dataset_vector,vector <Cluster *> & cluster_vector,string metric, vector <DataVector *> centroid_vector)
{
	map <DataVector *,double> true_neighbour;
	double distance;
	double new_objective_distance = 0.0;
	unsigned int x;
	unsigned int i;
	for (i=0;i<dataset_vector.size();i++)  //for each datapoint , find nearest centroid
	{
		for ( x=0;x<centroid_vector.size();x++)
		{
			distance=vectors_distance(metric,centroid_vector[x]->point_accessor(),dataset_vector[i]->point_accessor());
			update_cluster_vector(dataset_vector[i],distance,cluster_vector,x );
		}
		new_objective_distance +=dataset_vector[i]->cluster_number_accessor().second * dataset_vector[i]->cluster_number_accessor().second;
	}
	return new_objective_distance;

}


void lloyds_update(vector <Cluster *> & cluster_vector,int k,int L,double ** t,vector <double> **hv,vector <double> **hr,int w,string metric)
{
	vector <double> centroid_vector;
	DataVector * centroid_point;
	for(unsigned int i=0;i<cluster_vector.size();i++)  //vector with centroids for compatibility reasons
	{

		//cluster_vector[i]->centroid_accessor()->print_vector();
		//getchar();
		centroid_vector = cluster_vector[i]->kmeans(i);
		if (centroid_vector[0]==0.0)
		{
			continue; 
		}
		// for(i=0;i<centroid_vector.size();i++)
		// 	cout <<centroid_vector[i] << " ";
		if (cluster_vector[i]->is_external()==0)
		{
			cluster_vector[i]->make_external();
			if(metric.compare("cosine")==0) //cosine metric
				centroid_point = new Cosine(centroid_vector,"external_centroid",k,L,hr);
			else
				centroid_point = new Euclidean(centroid_vector,"external_centroid",k,L,hv,t,w);
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

void pam_update(vector <Cluster *> & cluster_vector,string metric)
{
	vector <double > centroid_vector;
	double distance;
	DataVector * new_centroid; 
	for(unsigned int i=0;i<cluster_vector.size();i++)  //vector with centroids for compatibility reasons
	{
		double minimum_distance;
		//cout << "old centroid number " << i << " is " <<  cluster_vector[i]->centroid_accessor()->name_accessor();
		minimum_distance = numeric_limits<double>::max();
		if(cluster_vector[i]->is_updated())
		{
			for (auto point_a : cluster_vector[i]->content_accessor() )
			{
				distance = 0.0;
				for (auto point_b : cluster_vector[i]->content_accessor() )	
				{
					distance+=vectors_distance(metric,point_a->point_accessor(),point_b->point_accessor());
				}
				if(distance< minimum_distance)
				{
					minimum_distance = distance;
					new_centroid = point_a; 
					cluster_vector[i]->create_external_centroid(new_centroid);
				}	
			}
			//cout << "new centroid number " << i << " is " <<  cluster_vector[i]->centroid_accessor()->name_accessor();
			//cluster_vector[i]->centroid_accessor()->print_vector();
			//getchar();
		}
	}
	return;
}

vector <double> silhouette_evaluation(vector <DataVector *> & dataset_vector,vector <Cluster *> & cluster_vector,string metric)
{
	int min_cluster;
	int neighbour_cluster;
	double final_silhouette; 
	double distance_a;
	double distance_b;
	vector <double> silhouette_vector;
	int counter = 0;
	cout << "in silhouette" << endl;
	for ( unsigned int i=0;i<cluster_vector.size();i++)
	{
		silhouette_vector.push_back(0);
	}
	for (unsigned int i=0;i<dataset_vector.size();i++)  
	{		
		min_cluster = dataset_vector[i]->cluster_number_accessor().first;
		for (auto point_a : cluster_vector[min_cluster]->content_accessor() )
		{		
			//distance_a += point_a->cluster_number_accessor().second;
			distance_a +=vectors_distance(metric,dataset_vector[i]->point_accessor(),point_a->point_accessor());

		}
		distance_a = distance_a/cluster_vector[min_cluster]->content_accessor().size();
		neighbour_cluster = dataset_vector[i]->neighbour_cluster_accessor().first;
		if (neighbour_cluster == min_cluster )
		{
			counter++;
		}

		if (neighbour_cluster == -1)
		{
			neighbour_cluster = min_cluster;
		}
		for (auto point_a : cluster_vector[neighbour_cluster]->content_accessor() )
		{
			distance_b+=vectors_distance(metric,dataset_vector[i]->point_accessor(),point_a->point_accessor());
			//distance_b += point_a->neighbour_cluster_accessor().second;		
		}
		distance_b = distance_b/cluster_vector[neighbour_cluster]->content_accessor().size();
		final_silhouette = (distance_b - distance_a)/max(distance_b,distance_a);
		silhouette_vector[min_cluster]+= final_silhouette;
    	//cout <<dataset_vector[i]->name_accessor()<< " distance_a " <<distance_a <<"distance_b " << distance_b<< " "<< final_distance << endl;
	}
	return silhouette_vector;
}

int update_cluster_vector(DataVector * v,double distance, vector <Cluster *> &cluster_vector,int cluster_num )
{

	int old_cluster=-1;
	if(v->cluster_number_accessor().first!=-1)
	{
		old_cluster = v->cluster_number_accessor().first;
	}
	if(is_nearest(distance, v ,cluster_num))  
	{
		if (old_cluster!=-1)
		{
			cluster_vector[old_cluster]->remove_from_cluster(v);
		}
		cluster_vector[cluster_num]->add_to_cluster(v); //add point to cluster
		if (old_cluster != cluster_num)  //check if datapoint has changed cluster and affects a different cluster
		{
			if(old_cluster!=-1)
				cluster_vector[old_cluster]->set_update(1);
			cluster_vector[cluster_num]->set_update(1);
		}	
		return 1; 

	}	
	return 0; 
}

double lsh_assignment(int L,int k,HashTable * hashtables_vector,vector <Cluster *> & cluster_vector,string metric,vector <DataVector *> & dataset_vector,vector <DataVector *> & centroid_vector)
{
	double distance=1000.0;
	int points_has_changed;
	set <DataVector *> unassigned_points;
	int big_radius;
	double new_objective_distance = 0.0;
	double radius = find_lsh_radius(centroid_vector,metric);
	if (radius ==0.0)
		radius =0.2;
	do
	{
		points_has_changed =0; 
		big_radius = 0; 
			for(unsigned int cluster_num=0;cluster_num<centroid_vector.size();cluster_num++) //range search for each centroid
			{
				for (int i=0;i<L;i++)
				{

					string key= centroid_vector[cluster_num]->key_accessor(i,k);
					for (auto v : hashtables_vector[i][key])
					{
						if(distance<radius)
						{
							big_radius = 1;

						}
						if(v->is_assigned()==0)
						{
							distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),v->point_accessor());
							if(distance<radius)
							{
								if(update_cluster_vector(v,distance,cluster_vector,cluster_num ))
								{	
									points_has_changed++; 
								}
							}
						}

					}
				}
			}
			radius*=2;
		}while(points_has_changed>0 || big_radius ==0);  //there is no need to increase the radius because each point in centroid's bucket is assigned
		int points = 0;
		
		for(unsigned i=0;i<dataset_vector.size();i++)
		{
			if(dataset_vector[i]->cluster_number_accessor().first == -1){points++;}
			if(dataset_vector[i]->is_assigned()==0) //for unassigned points
			{
				for(unsigned int cluster_num=0;cluster_num<cluster_vector.size();cluster_num++)
				{
					distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),dataset_vector[i]->point_accessor());
					update_cluster_vector(dataset_vector[i],distance,cluster_vector,cluster_num );

				}
			}
			else
			{
				for(unsigned int cluster_num=0;cluster_num<cluster_vector.size();cluster_num++)
				{
					distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),dataset_vector[i]->point_accessor());
					is_second_nearest(distance,dataset_vector[i],cluster_num);

				}
				
			}
			if(dataset_vector[i]->neighbour_cluster_accessor().second>2)
			{
				dataset_vector[i]->change_neighbour_cluster(dataset_vector[i]->cluster_number_accessor().first,dataset_vector[i]->cluster_number_accessor().second);
			}
			new_objective_distance +=dataset_vector[i]->cluster_number_accessor().second;
		}
		cout << "UNASSIGNED " << points<< endl; 
		return new_objective_distance;
	}



	double cube_assignment(list <DataVector *> * hypercube,int M,int probes,int k, int vertices,vector <Cluster *> & cluster_vector,string metric,vector <DataVector *> & dataset_vector,	vector <DataVector *> & centroid_vector)
	{
		double distance=1000.0;
		int int_key;
		int cluster_probes;
		int points_has_changed;
		set <DataVector *> unassigned_points;
		int big_radius;
		int points_checked; 
		double new_objective_distance=0.0;
		double radius = find_lsh_radius(centroid_vector,metric);
		do
		{
			points_has_changed =0; 
			big_radius = 0;
			cluster_probes= probes; 
			for(unsigned int cluster_num=0;cluster_num<centroid_vector.size();cluster_num++) //range search for each centroid
			{
				for(int i=0; i<vertices; ++i) 
				{
					string key= centroid_vector[cluster_num]->key_accessor(0,k);
					int_key = cube_key(key,metric);
					if(hamming_distance(i,int_key)<=cluster_probes)
					{
						cluster_probes--;
						points_checked=0;
						for (auto v : hypercube[i])
						{

							if(points_checked<M)
							{
								if(distance<radius)
								{
									big_radius = 1;

								}
								if(v->is_assigned()==0)
								{
									distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),v->point_accessor());
									if(distance<radius)
									{
										if(update_cluster_vector(v,distance,cluster_vector,cluster_num ))
										{	
											points_has_changed++; 
										}
									}
								}

								points_checked++;
							}
							else
								break;
						}
					}
					else
						continue;
				}
			}

			radius*=2;
		}while(points_has_changed>0 || big_radius==0);  //there is no need to increase the radius because each point in centroid's bucket is assigned
		int points = 0;
		for(unsigned i=0;i<dataset_vector.size();i++)
		{
			if(dataset_vector[i]->cluster_number_accessor().first == -1){points++;}
			if(dataset_vector[i]->is_assigned()==0) //for unassigned points
			{
				for(unsigned int cluster_num=0;cluster_num<cluster_vector.size();cluster_num++)
				{
					distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),dataset_vector[i]->point_accessor());
					update_cluster_vector(dataset_vector[i],distance,cluster_vector,cluster_num );

				}
			}
			else
			{
				for(unsigned int cluster_num=0;cluster_num<cluster_vector.size();cluster_num++)
				{
					distance=vectors_distance(metric,centroid_vector[cluster_num]->point_accessor(),dataset_vector[i]->point_accessor());
					is_second_nearest(distance,dataset_vector[i],cluster_num);

				}
			}
			if(dataset_vector[i]->neighbour_cluster_accessor().second>2)
			{
				
				dataset_vector[i]->change_neighbour_cluster(dataset_vector[i]->cluster_number_accessor().first,dataset_vector[i]->cluster_number_accessor().second);

			}
			new_objective_distance +=dataset_vector[i]->cluster_number_accessor().second;
		}
		return new_objective_distance;

	}

void clustering(int initialization,int assignment,int update,int number_of_clusters,string metric,int number_of_hashtables,int number_of_hashfunctions,int w,vector <Cluster *> & cluster_vector,vector <DataVector *> & ready_tweets_vector,vector <DataVector *> & centroid_vector,HashTable * twitter_hashtables_vector,double ** ht,vector <double> ** hv,vector <double> ** hr)
{
	int counter =0;
	double new_objective_distance = 0.0;
	double previous_objective_distance = 1.0;
	call_initialization(initialization,ready_tweets_vector,cluster_vector,number_of_clusters,metric);
	for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
	{
		if(!centroid_vector.empty() && i==0)
		{
			centroid_vector.clear();
		}
		centroid_vector.push_back(cluster_vector[i]->centroid_accessor());
	}
	do
	{	
		reset_distances(ready_tweets_vector);
		if (counter !=0)
		{
			previous_objective_distance = new_objective_distance;
		}
		for (unsigned int i = 0;i<cluster_vector.size();i++)
		{
			cluster_vector[i]->set_update(0);
		}
		new_objective_distance = call_assignment(assignment,number_of_hashfunctions,cluster_vector,metric,ready_tweets_vector,centroid_vector,twitter_hashtables_vector,number_of_hashtables);
		cout << "objective_distance " << new_objective_distance << "/"<< previous_objective_distance<< endl;	
		call_update(update,assignment,cluster_vector,number_of_hashfunctions,number_of_hashtables,ht, hv,hr,w,metric);
		for(unsigned int i=0;i<cluster_vector.size();i++)  //initialize vector with centroids for compatibility reasons
		{
			centroid_vector[i] = cluster_vector[i]->centroid_accessor();
		}
		counter++;
	}
	while((new_objective_distance/previous_objective_distance<(double)0.999 && counter<25)||counter==1);
	return;
}