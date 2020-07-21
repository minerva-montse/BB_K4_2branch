#pragma once
/* This file contains the global variables */

//hola
using namespace std;

typedef vector <vector <int> > matrixInt;
typedef vector <vector <bool> > matrixBool;
typedef vector <vector <double> > matrixDoubles;
typedef vector <vector <vector <double> > > DoubleMatrix3;

//Problem parameters & data
int n;
int p;
int k_radious = 3;
char file_name[50] = "";
char instance[50] = "";


struct A_i //with typedef struct A_i?
{
	double Coef_Yt;
	int t;
};

bool sort_A_i(A_i lhs, A_i rhs) { return lhs.Coef_Yt > rhs.Coef_Yt; }

//Data
matrixDoubles dist; // bi-vector of distances, all values are initialized to 0.
//matrixInt Denik;
matrixDoubles Denik;
vector <int> Gi;
DoubleMatrix3 Dik3;
//matrixInt Zik;
matrixDoubles Zik;
matrixBool Zik_fix;
vector <int> Yi;
matrixInt rji;



//Time Variables
clock_t clock_initial_BB;
double BB_time = 0;



clock_t clock_initial_solve;
double solve_model_time = 0;

clock_t clock_initial_subgradient;
double subgradient_model_time = 0;

clock_t clock_initial_setUp_once;
double setUP_once_model_time = 0;

double select_root_time;
double others_root_time;

//Solution
vector <int> solY;
double OV_Sub_Z;
double OV_Sub_Y;
double OV;
double sumLik;
double SumDi2;
double LB_OV;
vector <double> iteLB;
vector <double> iteBLB;
vector <double> iteAlpha;

//Lagrange Relaxation
matrixDoubles Lik;
matrixDoubles initial_Lik;

//Subgradient
double UB;
double BUB = 0;
double BLB = 0;
unsigned int Stepsize_den;
double Stepsize_tn;
int IteNoImp = 0;
double initial_LB;
double global_BUB = INFINITY;
double global_BLB = 0;
double WLB = 0;
double BLB_tree = 0;

//Global Parameters
int Ite;
int maxIte;
int Ite_Root = 100;
int Ite_general_nodes = 100;
double tolerance = 1E-6;
double epsilon = 1E-3;
double Gap = 1;
int alpha=10;
int alpha_root = 10;
double alpha2 = alpha;
double dk;
double dk_l = 1;
char alpha_string[10];
int IteBLB_all;
int max_Gi;



//Terminate Conditions
int limIteNoImp = 10;
double factorRedAlpha = 0.5;
double time_limit = 3600;
int IteBLB_limit = 50;
double Gap_limit = 0.01;
double Gap_limit_global = 0.001;
double stepsize_limit = 1E-6;
int IteBLB = 0;
char TerminateCondition[50];

//Structures for tree
int active_nodes = 0; // Number of nodes in the list to branch on.
int total_nodes = 0; // Number of nodes generated so far.
int NodeNumber = 0; //To give a number to each node.
int InfeasibleNodes = 0;
int FeasibleNodes = 0;
int PruneNodes = 0;

typedef struct node_i
{
	int branch; //Right branch = 0 (When Zik=1) , Left branch = 1...num_Y (when Zik=0 and num_Y branches are created).
	short active_children; //1+x, 1 Right children and x left children depending number of Y's. -1 for each son pruned.
	//int branching_y; //Create two children nodes by branching on this variable.
	int branching_Zi; //Create children nodes by branching on this variable (Index i of variables Z to branch).
	int branching_Zk; //Create children nodes by branching on this variable (Index k of variables Z to branch).
	int number_node;

	vector <int> branch_zi_1; //Index i of variables Z's set to 1.
	vector <int> branch_zk_1; //Index k of variables Z's set to 1.
	vector <int> branch_zi_0; //Index i of variables Z's set to 0.
	vector <int> branch_zk_0; //Index k of variables Z's set to 0.
	vector <int> branch_Li_0; //Index i of variables for the Lik's set to 0.
	vector <int> branch_Lk_0; //Index k of variables for the Lik's set to 0.
	vector <int> branch_y_1; //Variables Y's set to 1.
	vector <int> branch_y_0; //Variables Y's set to 0.
	vector <int> branch_zik_y;
	vector <int> branch_lower_level;//The value for k where is branching the node_i.
	vector <int> branch_upper_level;

	
	matrixDoubles final_Lik;

	int Num_children[4]; //[0]=Left Nodes. [1]=Mid Node. [2]=Right Node. [3] Node to branch in case of repetead.
	  
	double lower_bound;
	double lower_bound_tree;
	//double alpha;
	//double y_value;
	node_i *parent_node;
	//node_i *left_son;
	node_i *right_son;
	node_i *mid_son;
	vector <node_i*> left_sons;
	
	//vector <int> prueba;
	//bool mid;
} node_info;

typedef struct active_n
{
	node_i *the_info;
	active_n *previous;
	active_n *next;

} active_node;



active_n *the_first_active_node; //Points to the first node in the list of actives nodes.
active_n *the_first_node_tree; //Points to the first node on the tree.
node_i *this_active_node; //Points to the information of the node being solved.

//Branch and bound
int branch_varZ_i;
int branch_varZ_k;
double branch_varZ_val = 0;
double temp_branch_VarZ;
bool prune; //Variable to indicate that a nood need to be prune.
bool optimal = false;
bool mid_son = false;
bool update_list_new_BUB = false;
bool empty_tree = false;

//Best at node
matrixDoubles best_Lik;
matrixDoubles best_Zik;
vector <int> best_Yi;
matrixDoubles best_Denik;
double best_OV;

double best_OV_Sub_Z;
double best_OV_Sub_Y;
double best_sumLik;
double best_SumDi2;


bool global_check = false;
bool terminateBB = false;


//Incumbent
double incumbent_time = 0;
int incumbent_sol=INFINITY;
double BUB_time = 0;

