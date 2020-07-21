#pragma once

#include "Subroutines.h"
#include "solve_rutines.h"


void Solve_Root_Node()
{
	cout << endl;
	//cout << "Initial alpha root: " << alpha_root << endl;
	//clock_t start_root_time = clock();
	//double solve_root_time = 0;
	//double subgradient_root_time = 0;
	//double select_root_time = 0;
	//select_root_time = 0;
	
	clock_initial_solve = clock();

	cout << "Solving Root node" << endl;

	active_nodes = 1;
	node_i *this_node;
	this_node = Create_n_i();
	(*this_node).parent_node = NULL;
	(*this_node).branch = NULL;
	++NodeNumber;
	(*this_node).number_node = NodeNumber;
	this_active_node = this_node;

	//Branching levels. We haven't done any branching, so the levels are set to 2 and Gi, respectively.
	vector <int> temp(n, 2);
	(*this_node).branch_lower_level = temp;
	(*this_node).branch_upper_level = temp;

	
	for (int i = 0; i < n; i++)
	{
		(*this_node).branch_upper_level[i] = Gi[i];
	}

	Ite = 1;
	while (Ite <= Ite_Root)
	{
		clock_t start_root_solve_time = clock();
		//Call the solve rutine. Fin the OV for the current value of Lik.
		solve_Root();

		solve_model_time += (double)(clock() - start_root_solve_time) / CLOCKS_PER_SEC;

		clock_t start_root_subgradient_time = clock();
		//Call the Subgradient rutine. Update Lik.
		subgradient_Root();
		subgradient_model_time += (double)(clock() - start_root_subgradient_time) / CLOCKS_PER_SEC;

		//Global BUB
		if (BUB < global_BUB - tolerance)
		{
			global_BUB = BUB;
			cout << "New BUB= " << global_BUB << "\t ite " << Ite << endl;
			//feasible_zik();
			//cin.get();
		}
		
		//global_BUB = BUB;

		//Increase the counter of Number of iterations.
		++Ite;
	}
	
	//Calculate the Gap.
	Gap = (BUB - BLB) / BUB;

	(*this_node).branching_Zi = branch_varZ_i;
	(*this_node).branching_Zk = branch_varZ_k;
	(*this_node).lower_bound = BLB;
	(*this_node).lower_bound_tree = BLB;
	(*this_node).branch_y_1 = {};
	(*this_node).branch_y_0 = {};
	(*this_node).branch_zi_1 = {};
	(*this_node).branch_zk_1 = {};
	(*this_node).branch_zi_0 = {};
	(*this_node).branch_zk_0 = {};
	//Save the varibales Y's associated with the restriction of Zik.
	int num_Y = 0;
	for (int kk = 0; kk < branch_varZ_k; kk++)
	{
		for (int lenK = 1; lenK < Dik3[branch_varZ_i][kk].size(); lenK++)
		{
			(*this_node).branch_zik_y.push_back(Dik3[branch_varZ_i][kk][lenK]);
			++num_Y;
		}
	}

	cout << "Branch Y's= ";
	for (size_t i = 0; i < (*this_node).branch_zik_y.size(); i++)
	{
		cout << (*this_node).branch_zik_y[i] << ", ";
	}
	cout << endl;

	(*this_node).active_children = num_Y+1;
	cout << "Num childrens: " << (*this_node).active_children << endl;

	//Creates the first node on the tree. Previous and next nodes are empty.
	the_first_active_node = Create_a_n();
	(*the_first_active_node).next = NULL;
	(*the_first_active_node).previous = NULL;
	(*the_first_active_node).the_info = this_node;

	//Is the root node, then the_first_node_tree and the_first_active_node are the same.
	the_first_node_tree = the_first_active_node;

	//cout << endl;

	cout << "active_nodes= " << active_nodes << endl;
	cout << "node number: " << (*this_active_node).number_node << endl;
	cout << "lower_bound: " << (*this_active_node).lower_bound << endl;
	cout << "Global BUB: " << global_BUB << endl;
	cout << "Gap: " << Gap << endl;

	//cout << "Best LB on node: " << best_OV << endl;
	//cout << "Best OV_Sub_Z on node: " << best_OV_Sub_Z << endl;
	//cout << "Best OV_Sub_Y on node: " << best_OV_Sub_Y << endl;
	//cout << "Best sumLik on node: " << best_sumLik << endl;
	//cout << "Best SumDi2 on node: " << best_SumDi2 << endl;

	//cout << "Y's= ";
	//for (int i = 0; i < n; i++)
	//	if (Yi[i] == 1)
	//		cout << i << ", ";
	//cout << endl;

	//double root_time = (double)(clock() - start_root_time) / CLOCKS_PER_SEC;
	//cout << "Time root node: " << root_time << endl;
	//cout << "Solve time: " << solve_root_time << endl;
	//cout << "Subgradient time: " << subgradient_root_time << endl;
	
	//cout << "Final alpha root: " << (*this_node).alpha << endl;
	//cin.get();
	return;
}

