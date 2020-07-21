#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdio>

#include "global.h"
#include "Aux_functions.h"
#include "BB_functions.h"
#include "root_node.h"
#include "general_node.h"


using namespace std;

int main(int argc, char *argv[])
{
	//Check for the input parameters and in case of wrong imput gives some notes.
	if (Read_Parameters(argc, argv) == false)
	{
		printf("ERROR: wrong command line.\n");
		printf("The structure is: pmedian.exe <data_file> -p p\n");
		printf("\n");
		return 0;
	}

	clock_initial_setUp_once = clock();
	//Read data and creat the needed structures
	readData2(argc, argv);
	//myopic(n, p, dist);
	setUP_once_model_time = (double)(clock() - clock_initial_setUp_once) / CLOCKS_PER_SEC;

	//Vector of solutions Yi is given the size n.
	Yi.resize(n);

	//Start time BB.
	clock_initial_BB = clock();

	//Solve root node.
	Solve_Root_Node();
	total_nodes = 1;

	//while (active_nodes>0 || Gap> 0.1 + tolerance)
	//while ((active_nodes>0 && abs(BLB-4093)>0.01))
	//while (active_nodes > 0 && global_BUB > 4093 - tolerance)
	//while (active_nodes > 0 && Gap > Gap_limit_global + tolerance)
	while (active_nodes > 0)
	{
		the_first_node_tree = the_first_active_node;
		//cout << "setting Z[" << (*the_first_active_node).the_info->branching_Zi << "][" << (*the_first_active_node).the_info->branching_Zk << "]." << endl;
		//Zik_fix[(*the_first_active_node).the_info->branching_Zi][(*the_first_active_node).the_info->branching_Zk - 2] = true;
		//cout << "Zik_fix= " << Zik_fix[(*the_first_active_node).the_info->branching_Zi][(*the_first_active_node).the_info->branching_Zk - 2] << endl;

		//Point the father node.
		this_active_node = (*the_first_active_node).the_info;

		/*Mid son. Lik=0.*/
		if ((*this_active_node).Num_children[1]==1)
		{
			//Create the node for the MID son, set the information for the parent node and set to NULL the children nodes.
			++total_nodes;
			(*this_active_node).mid_son = Create_n_i();
			(*this_active_node).mid_son->parent_node = this_active_node;
			this_active_node = (*this_active_node).mid_son;
			(*this_active_node).branch = n + 1;
			(*this_active_node).right_son = NULL;
			(*this_active_node).mid_son = NULL;
			(*this_active_node).left_sons.resize(1); //We don't know how many left sons will have, but at least 1.
			(*this_active_node).left_sons[0] = NULL;
			++NodeNumber;
			(*this_active_node).number_node = NodeNumber;
			//for (size_t i = 0; i < n; i++)
			//{
			//	(*this_active_node).zik_fix_node.push_back(vector <bool>(Gi[i] - k_radious + 1, 0));
			//}
			
			prune = false;
			//Call the funciton to solve a general node.
			Solve_General_Node();
		}


		/*Right son. Zik=1.*/
		if (!terminateBB)
		{
			//Point the father node.
			this_active_node = (*the_first_active_node).the_info;

			//if (((*this_active_node).Num_children[1] == 1 && (*this_active_node).Num_children[2] == 1) || (*this_active_node).Num_children[1] == 0)
			if ((*this_active_node).Num_children[2] == 1)
			{
				//Create the node for the RIGHT son, set the information for the parent node and set to NULL the children nodes.
				++total_nodes;
				(*this_active_node).right_son = Create_n_i();
				(*this_active_node).right_son->parent_node = this_active_node;
				this_active_node = (*this_active_node).right_son;
				(*this_active_node).branch = 0;
				(*this_active_node).right_son = NULL;
				(*this_active_node).mid_son = NULL;
				(*this_active_node).left_sons.resize(1);
				(*this_active_node).left_sons[0] = NULL;
				++NodeNumber;
				(*this_active_node).number_node = NodeNumber;
				//for (size_t i = 0; i < n; i++)
				//{
				//	(*this_active_node).zik_fix_node.push_back(vector <bool>(Gi[i] - k_radious + 1, 0));
				//}
				prune = false;
				//Call the funciton to solve a general node.
				Solve_General_Node();

				//If during the Solve_General_Node(), prune is set to TRUE, then call the Prune function.
				if (prune) { Prune_node(this_active_node); }
			}
			else
			{
				cout << endl;
				cout << "Right node is not created." << endl;
			}
		}



		//Left sons. Zik=0.
		if (!terminateBB)
		{
			//Point the father node.
			this_active_node = (*the_first_active_node).the_info;

			if ((*this_active_node).Num_children[0] == 1)
			{
				cout << "Only one left son is created." << endl;
				//Create the node for the left son, set the information for the parent node and set to NULL the Left sons & Right son.
				++total_nodes;
				(*this_active_node).left_sons[0] = Create_n_i();
				(*this_active_node).left_sons[0]->parent_node = this_active_node;
				this_active_node = (*this_active_node).left_sons[0];
				(*this_active_node).branch = 1;
				(*this_active_node).right_son = NULL;
				(*this_active_node).mid_son = NULL;
				(*this_active_node).left_sons.resize(1);
				(*this_active_node).left_sons[0] = NULL;
				++NodeNumber;
				(*this_active_node).number_node = NodeNumber;
				//for (size_t i = 0; i < n; i++)
				//{
				//	(*this_active_node).zik_fix_node.push_back(vector <bool>(Gi[i] - k_radious + 1, 0));
				//}
				prune = false;
				//Call the funciton to solve a general node.
				Solve_General_Node();
				//If during the Solve_General_Node(), prune is set to TRUE, then call the Prune function.
				if (prune) { Prune_node(this_active_node); }
			}
			else if ((*this_active_node).Num_children[0] == 0)
			{
				cout << "Left node is not created." << endl;
			}
			else
			{
				//Resize the vector with the number of left sons and point all them to NULL.
				int temp_size = (*this_active_node).Num_children[0];
				cout << "temp_size= " << temp_size << endl;
				(*this_active_node).left_sons.resize(temp_size);
				for (size_t i = 0; i < temp_size; i++)
				{
					(*this_active_node).left_sons[i] = NULL;
				}

				//Create the node.
				for (size_t i = 0; i < temp_size; i++)
				{
					if (!terminateBB)
					{
						//Point the father node.
						this_active_node = (*the_first_active_node).the_info;
						//Create the node for the left son, set the information for the parent node and set to NULL the Left sons & Right son.
						++total_nodes;
						(*this_active_node).left_sons[i] = Create_n_i();
						(*this_active_node).left_sons[i]->parent_node = this_active_node;
						this_active_node = (*this_active_node).left_sons[i];
						(*this_active_node).branch = i + 1;
						(*this_active_node).right_son = NULL;
						(*this_active_node).mid_son = NULL;
						(*this_active_node).left_sons.resize(1);
						(*this_active_node).left_sons[0] = NULL;
						++NodeNumber;
						(*this_active_node).number_node = NodeNumber;
						//for (size_t i = 0; i < n; i++)
						//{
						//	(*this_active_node).zik_fix_node.push_back(vector <bool>(Gi[i] - k_radious + 1, 0));
						//}
						prune = false;
						//Call the funciton to solve a general node.
						Solve_General_Node();
						//If during the Solve_General_Node(), prune is set to TRUE, then call the Prune function.
						if (prune) { Prune_node(this_active_node); }

						//Point the father node. To create the next son or at the end of the loop to remove parent node.
						this_active_node = (*the_first_active_node).the_info;

					}
				}
			}
		}




		//if (!terminateBB)
		//{
		//	//Point the father node.
		//	this_active_node = (*the_first_active_node).the_info;
		//
		//	if ((*this_active_node).Num_children[1] == 0 && (*this_active_node).Num_children[0] == 0)
		//	{
		//		//Resize the vector with the number of left sons and point all them to NULL.
		//		int temp_size;
		//		temp_size = (*this_active_node).Num_children[0];
		//		cout << "Number left sons: " << temp_size;
		//		cin.get();
		//		if (!(*this_active_node).mid)
		//			temp_size = (*this_active_node).active_children - 1; //Number of left sons, one for each Y=1. The '-1' is because the active children includes the right child.
		//		else
		//			temp_size = (*this_active_node).active_children - 2; //Number of left sons, one for each Y=1. The '-2' is because the active children includes the right and mid child.

		//		(*this_active_node).left_sons.resize(temp_size);
		//		for (size_t i = 0; i < temp_size; i++)
		//		{
		//			(*this_active_node).left_sons[i] = NULL;
		//		}

		//		//Create the node.
		//		for (size_t i = 0; i < temp_size; i++)
		//		{
		//			//Point the father node.
		//			this_active_node = (*the_first_active_node).the_info;
		//			//Create the node for the left son, set the information for the parent node and set to NULL the Left sons & Right son.
		//			++total_nodes;
		//			(*this_active_node).left_sons[i] = Create_n_i();
		//			(*this_active_node).left_sons[i]->parent_node = this_active_node;
		//			this_active_node = (*this_active_node).left_sons[i];
		//			(*this_active_node).branch = i + 1;
		//			(*this_active_node).right_son = NULL;
		//			(*this_active_node).mid_son = NULL;
		//			(*this_active_node).left_sons.resize(1);
		//			(*this_active_node).left_sons[0] = NULL;
		//			++NodeNumber;
		//			(*this_active_node).number_node = NodeNumber;
		//			for (size_t i = 0; i < n; i++)
		//			{
		//				(*this_active_node).zik_fix_node.push_back(vector <bool>(Gi[i] - k_radious + 1, 0));
		//			}
		//			prune = false;
		//			//Call the funciton to solve a general node.
		//			Solve_General_Node();
		//			//If during the Solve_General_Node(), prune is set to TRUE, then call the Prune function.
		//			if (prune) { Prune_node(this_active_node); }

		//			//Point the father node. To create the next son or at the end of the loop to remove parent node.
		//			this_active_node = (*the_first_active_node).the_info;
		//		}

		//		//if (mid_son==true)
		//		//{
		//		//	cout << "(*this_active_node).number_node= " << (*this_active_node).number_node << endl;
		//		//	cout << "(*this_active_node).lower_bound= " << (*this_active_node).lower_bound << endl;

		//		//	//Create the node for the mid son (Lik=0), set the information for the parent node and set to NULL their Lefts & Right son.
		//		//	++total_nodes;
		//		//	(*this_active_node).mid_son = Create_n_i();
		//		//	(*this_active_node).mid_son->parent_node = this_active_node;
		//		//	this_active_node = (*this_active_node).mid_son;
		//		//	(*this_active_node).branch = n + 1;
		//		//	(*this_active_node).right_son = NULL;
		//		//	(*this_active_node).mid_son = NULL;
		//		//	(*this_active_node).left_sons.resize(1);
		//		//	(*this_active_node).left_sons[0] = NULL;
		//		//	++NodeNumber;
		//		//	(*this_active_node).number_node = NodeNumber;
		//		//	prune = false;
		//		//	//Call the funciton to solve a general node.
		//		//	Solve_Lik_Node();

		//		//	//If during the Solve_General_Node(), prune is set to TRUE, then call the Prune function.
		//		//	if (prune) { Prune_node(this_active_node); }
		//		//}
		//	}
		//}
		


		//Point the father node.
		this_active_node = (*the_first_active_node).the_info;

		//Remove parent node from the list. (Parent node NOT has always a lower bound that children)
		--active_nodes;

		if ((active_nodes > 0) && (the_first_node_tree == the_first_active_node) && ((*the_first_active_node).previous == NULL))
		{
			//cout << "Case 1" << endl;
			/*El nodo padre al eliminar esta al inicio del arbol después de insertar sus hijos al árbol.
			Por tanto:
			the_first_node_tree = the_first_active_node //Los dos nodos son iguales.
			(*the_first_active_node).previous == NULL //El nodo previo al nodo padre esta vacio.*/

			active_n *temp;
			temp = the_first_active_node;
			the_first_active_node = (*the_first_active_node).next;
			(*the_first_active_node).previous = NULL;

			(*temp).next = NULL;

			this_active_node = temp->the_info; //Copy the node_i of temp node, to be able to prune that node.
			delete temp;
		}
		else if ((active_nodes > 0) && (the_first_node_tree != the_first_active_node) && ((*the_first_active_node).previous != NULL) && ((*the_first_active_node).next != NULL))
		{
			//cout << "Case 2" << endl;
			//cin.get();
			/*El nodo padre al eliminar NO esta al inicio del arbol después de insertar sus hijos al árbol. El nodo padre esta entre 2 nodos en el árbol.
			Por tanto:
			the_first_node_tree != the_first_active_node //Los dos nodos NO son iguales.
			(*the_first_active_node).previous != NULL    //El nodo previo al nodo padre NO esta vacio.
			(*the_first_active_node).next != NULL        //El nodo siguiente al nodo padre NO esta vacio. */

			active_n *temp_previous;
			temp_previous = (*the_first_active_node).previous;
			active_n *temp_next;
			temp_next = (*the_first_active_node).next;
			(*the_first_active_node).previous->next = temp_next;
			(*the_first_active_node).next->previous = temp_previous;
			the_first_active_node = the_first_node_tree;

			temp_previous = NULL;
			temp_next = NULL;
			delete temp_previous;
			delete temp_next;
		}
		else if ((active_nodes > 0) && (the_first_node_tree != the_first_active_node) && ((*the_first_active_node).previous != NULL) && ((*the_first_active_node).next == NULL))
		{
			//cout << "Case 3" << endl;
			//cin.get();
			/*El nodo padre al eliminar NO esta al inicio del arbol después de insertar sus hijos al árbol. El nodo padre esta al final del árbol.
			Por tanto:
			the_first_node_tree != the_first_active_node //Los dos nodos NO son iguales.
			(*the_first_active_node).previous != NULL    //El nodo previo al nodo padre NO esta vacio.
			(*the_first_active_node).next == NULL        //El nodo siguiente al nodo padre esta vacio. */

			(*the_first_active_node).previous->next = NULL;
			the_first_active_node = the_first_node_tree;
		}
		else
		{
			cout << "Case 4 (Eliminar nodo padre)." << endl;
			cout << "The tree is empty." << endl;
			cout << "active_nodes: " << active_nodes << endl;
			//cin.get();

			this_active_node = (*the_first_active_node).the_info;
			(*the_first_active_node).the_info = NULL;
			Prune_node(this_active_node);
			delete the_first_active_node;
			
			empty_tree = true;
		}

		//Remove nodes that have a LB grater that the BUB.
		if (update_list_new_BUB)
		{
			update_list_node();
			update_list_new_BUB = false;
		}
		
		BB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;

		//Terminate the B&B because we have a Feasible and a Gap less than the gap_limit.......
		if (terminateBB)
		{
			//LB of the first node on the tree.
			WLB = (*the_first_active_node).the_info->lower_bound;
			cout << "wlb: " << WLB << endl;
			cout << "Program terminate, gap below " << Gap_limit << endl;
			cout << "Feasible Found." << endl;
			cout << "Time BB= " << BB_time << " secs" << endl;
			cout << "Time Solve= " << solve_model_time << " secs" << endl;
			cout << "Time Subgradient= " << subgradient_model_time << " secs" << endl;
			cout << "Time Initial= " << setUP_once_model_time << " secs" << endl;
			strcpy_s(TerminateCondition, "Time Limit");
			if (BLB_tree < tolerance)
			{
				best_lower_bound_tree();
			}
			output_BB();
			DeleteTree();
			return 0;
		}

		//Terminate the B&B because there is no more nodes to explore. The tree is empty
		if (empty_tree)
		{
			cout << "Program terminate, gap below " << Gap_limit << endl;
			cout << "Feasible Found." << endl;
			cout << "Time BB= " << BB_time << " secs" << endl;
			cout << "Time Solve= " << solve_model_time << " secs" << endl;
			cout << "Time Subgradient= " << subgradient_model_time << " secs" << endl;
			cout << "Time Initial= " << setUP_once_model_time << " secs" << endl;
			strcpy_s(TerminateCondition, "Time Limit");
			output_BB();
			//DeleteTree();
			return 0;
		}

		//Terminate the B&B because the time_limit was reached.
		if (BB_time > time_limit - tolerance)
		{
			//LB of the first node on the tree.
			WLB = (*the_first_active_node).the_info->lower_bound;
			cout << "Program terminate, time limit of " << time_limit << " secs." << endl;
			cout << "Time BB= " << BB_time << " secs" << endl;
			cout << "Time Solve= " << solve_model_time << " secs" << endl;
			cout << "Time Subgradient= " << subgradient_model_time << " secs" << endl;
			cout << "Time Initial= " << setUP_once_model_time << " secs" << endl;
			strcpy_s(TerminateCondition, "Time Limit");
			if (BLB_tree < tolerance)
			{
				best_lower_bound_tree();
			}
			output_BB();
			DeleteTree();
			return 0;
		}
	}


	//WLB is the LB of the first node on the tree.
	WLB = (*the_first_active_node).the_info->lower_bound;
	cout << "Program terminates, Gap below 0.001, but Feasbile solution wasn't found." << endl;
	
	if (BLB_tree < tolerance)
	{
		best_lower_bound_tree();
	}
	output_BB();
	//Delete the tree and remaining pointers.
	DeleteTree();

	return 0;
}