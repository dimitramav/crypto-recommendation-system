//algorithms
void random_initialization( vector <DataVector *> &,vector <Cluster *> &,int);
double lloyds_assignment(vector <DataVector *> &,vector <Cluster *> &,string,vector <DataVector *> centroid_vector);
void lloyds_update(vector <Cluster *> &,int,int,double ** ,vector <double> **,vector <double> **,int,string);
vector <double> silhouette_evaluation(vector <DataVector *> &,vector <Cluster *> &,string);
void plus_initialization(vector <DataVector *> &, vector <Cluster *> &, int,string);
void set_centroid(vector <DataVector *> & ,vector <Cluster*> & , int);
void pam_update(vector <Cluster *> &,string);
double lsh_assignment(int,int,HashTable *,vector <Cluster *> &,string,vector <DataVector *> &, vector <DataVector *> &);
int is_nearest(double ,DataVector * ,int);
int update_cluster_vector(DataVector * ,double , vector <Cluster *> &,int);
double find_lsh_radius(vector <DataVector *> &, string);
double cube_assignment(list <DataVector *> * ,int ,int ,int , int ,vector <Cluster *> & ,string ,vector <DataVector *> & ,	vector <DataVector *> &);
int is_second_nearest(double ,DataVector * ,int );


