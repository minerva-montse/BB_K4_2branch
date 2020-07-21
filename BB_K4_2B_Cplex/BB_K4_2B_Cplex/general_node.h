#pragma once

void Solve_General_Node()
{
	//Copying information from parent node.
	int parent_zi = (*this_active_node).parent_node->branching_Zi;
	int parent_zk = (*this_active_node).parent_node->branching_Zk;
	
	cout << endl;

	vector <int> parent_zik_y = (*this_active_node).parent_node->branch_zik_y;
	vector <int> V_parent_zi_1 = (*this_active_node).parent_node->branch_zi_1;
	vector <int> V_parent_zk_1 = (*this_active_node).parent_node->branch_zk_1;
	vector <int> V_parent_zi_0 = (*this_active_node).parent_node->branch_zi_0;
	vector <int> V_parent_zk_0 = (*this_active_node).parent_node->branch_zk_0;
	vector <int> V_parent_y_1 = (*this_active_node).parent_node->branch_y_1;
	vector <int> V_parent_y_0 = (*this_active_node).parent_node->branch_y_0;
	vector <int> V_parent_Li_0 = (*this_active_node).parent_node->branch_Li_0;
	vector <int> V_parent_Lk_0 = (*this_active_node).parent_node->branch_Lk_0;

	//Fix variables according to left, rigt o mid node.
	if ((*this_active_node).branch == 0)
	{
		//This is the RIGHT NODE, then we need to add zik=1 & Y=0.
		cout << "Solving right node." << endl;
		cout << "Parent number node: " << (*this_active_node).parent_node->number_node << endl;
		cout << "Number node: " << (*this_active_node).number_node << endl;
		cout << "Fixing Zik[" << parent_zi << "][" << parent_zk << "]= " << 1 << endl;

		//Add the zik=1.
		V_parent_zi_1.push_back(parent_zi);
		V_parent_zk_1.push_back(parent_zk);
		
		//Add the Yi=0.
		for (int i = 0; i < parent_zik_y.size(); i++)
		{
			V_parent_y_0.push_back(parent_zik_y[i]);
		}

		//In case of adding repeated Y, only keep one of them.
		sort(V_parent_y_0.begin(), V_parent_y_0.end());
		V_parent_y_0.erase(unique(V_parent_y_0.begin(), V_parent_y_0.end()), V_parent_y_0.end()); // Eliminates repetead Y's

		//When Zik=1. The branching_lower_level gets updated, but the branching_upper_level remains the same.
		(*this_active_node).branch_lower_level = (*this_active_node).parent_node->branch_lower_level;
		(*this_active_node).branch_lower_level[parent_zi] = parent_zk + 1;
		(*this_active_node).branch_upper_level = (*this_active_node).parent_node->branch_upper_level;
		
	}
	else if ((*this_active_node).branch == n + 1)
	{
		cout << "Solving mid node." << endl;
		cout << "Parent number node: " << (*this_active_node).parent_node->number_node << endl;
		cout << "Number node: " << (*this_active_node).number_node << endl;
		cout << "Fixing Lik[" << parent_zi << "][" << parent_zk << "]= " << 0 << endl;

		//The branching_lower_level are the same as the parent node.
		(*this_active_node).branch_lower_level = (*this_active_node).parent_node->branch_lower_level;
		(*this_active_node).branch_upper_level = (*this_active_node).parent_node->branch_upper_level;

		//Fix values of Lik.
		V_parent_Li_0.push_back(parent_zi);
		V_parent_Lk_0.push_back(parent_zk);
	}
	else
	{
		//This is the LEFT NODE, then we need to add zik=0 & Y=1.
		cout << "Solving left node." << endl;
		cout << "Parent number node: " << (*this_active_node).parent_node->number_node << endl;
		cout << "Number node: " << (*this_active_node).number_node << endl;
		cout << "Fixing Zik[" << parent_zi << "][" << parent_zk << "]= " << 0 << endl;

		//Add the zik=0.
		V_parent_zi_0.push_back(parent_zi);
		V_parent_zk_0.push_back(parent_zk);

		//Set Y=0 or Y=1 depending the case.
		if ((*this_active_node).parent_node->Num_children[1]==1 && (*this_active_node).parent_node->Num_children[0] == 1)
		{
			//For the case of "feasible branch" where we had fixed a Yi=1 previously, then we only need the branch of Zik=1 & sum Yi=0.
			//Using Num_children[1]==1 ensure we have a feasbile branch.
			cout << "Only fixing y=0. The corresponding Y=1 was fixed previuosly." << endl;

			for (size_t i = 0; i < parent_zik_y.size(); i++)
			{
				if (parent_zik_y[i] != (*this_active_node).parent_node->Num_children[3])
				{
					// The vector 'parent_zik_y' are all the variables Y's linked to the constraint to branch. We need to save all of them except the y=1.
					cout << parent_zik_y[i] << ", ";
					V_parent_y_0.push_back(parent_zik_y[i]);
				}
			}
			cout << endl;
		}
		else
		{
			//fix one of the Y's to 1.
			V_parent_y_1.push_back(parent_zik_y[(*this_active_node).branch - 1]);
			cout << "Fixing y= " << parent_zik_y.at((*this_active_node).branch - 1) << endl;

			int temp_size_1 = V_parent_y_1.size();

			//In case of adding repeated Y, only keep one of them.
			sort(V_parent_y_1.begin(), V_parent_y_1.end());
			V_parent_y_1.erase(unique(V_parent_y_1.begin(), V_parent_y_1.end()), V_parent_y_1.end()); // Eliminates repetead Y's

			int temp_size_2 = V_parent_y_1.size();

			if (temp_size_2 < temp_size_1)
			{
				cout << "La Y=1 esta repetida. Prune???" << endl;
				cout << "temp_size_1= " << temp_size_1 << endl;
				cout << "temp_size_2= " << temp_size_2 << endl;
				prune = true;
				cout << "prune node" << endl;
				cin.get();
			}

			//If more than 'p' variables Y are set to 1, we need to prune. The problem is ifeasible.
			if (V_parent_y_1.size() > p)
			{
				prune = true;
				cout << "prune node" << endl;
				//cin.get();
			}
		}

		//When Zik=0. The branching_upper_level gets updated, but the branching_lower_level remains the same.
		(*this_active_node).branch_lower_level = (*this_active_node).parent_node->branch_lower_level;
		(*this_active_node).branch_upper_level = (*this_active_node).parent_node->branch_upper_level;
		(*this_active_node).branch_upper_level[parent_zi] = parent_zk;
	}

	//Adding the information into the structure node.
	(*this_active_node).branch_zi_1 = V_parent_zi_1;
	(*this_active_node).branch_zk_1 = V_parent_zk_1;
	(*this_active_node).branch_zi_0 = V_parent_zi_0;
	(*this_active_node).branch_zk_0 = V_parent_zk_0;
	(*this_active_node).branch_y_1 = V_parent_y_1;
	(*this_active_node).branch_y_0 = V_parent_y_0;
	(*this_active_node).branch_Li_0 = V_parent_Li_0;
	(*this_active_node).branch_Lk_0 = V_parent_Lk_0;

	//Starts with the parent last multipliers.
	Lik = (*this_active_node).parent_node->final_Lik;

	//Fix the multipliers to 0.
	for (size_t i = 0; i < (*this_active_node).branch_Li_0.size(); i++)
	{
		Lik[(*this_active_node).branch_Li_0[i]][(*this_active_node).branch_Lk_0[i] - 2] = 0;
	}


	Ite = 1;
	while (Ite <= Ite_general_nodes && !prune)
	{
		//cout << "Ite: " << Ite << "\t alpha: " << alpha << endl;
		clock_t start_solve_time = clock();
		//Call the solve rutine for each subproblem. Find the OV for the current value of Lik.
		solve_subpro_Z(); //Solve the subproblem on the variable Z using inspection.
		solve_subpro_Y(); //Solve the subproblem on the variable Y using inspection.

		solve_model_time += (double)(clock() - start_solve_time) / CLOCKS_PER_SEC;

		//Objective Value for current iterarion.
		OV = OV_Sub_Z + SumDi2 + sumLik - OV_Sub_Y;
		////Primal solution for current solution of Yi.
		//UB = Primal(Yi, n, p, dist);
		//Check if the OV is  below the BUB, if not, prune the node.
		if (OV > global_BUB + tolerance)
		{
			Ite = Ite_general_nodes;
			cout << "quite while loop and prune node" << endl;
			prune = true;
			BLB = 0;
			Gap = INFINITY;
		}
		else
		{
			clock_t start_subgradient_time = clock();
			//Call the Subgradient rutine. Update Lik.
			subgradient();
			subgradient_model_time += (double)(clock() - start_subgradient_time) / CLOCKS_PER_SEC;

			////Check if there is a new BUB.
			//if (BUB < global_BUB - tolerance)
			//{
			//	global_BUB = BUB;
			//	BUB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			//	cout << "New BUB= " << global_BUB << "\t Time: " << BUB_time << endl;
			//	//feasible_zik();
			//	//cin.get();
			//	update_list_new_BUB = true;
			//}

			//Update the Gap.
			Gap = (BUB - BLB) / BUB;
		}

		//Increase the counter of Number of iterations.
		++Ite;
	}

	//If the node is not prune.
	if (!prune)
	{
		//Primal solution for current solution of Yi.
		UB = Primal(Yi, n, p, dist);
		//Check if there is a new BUB.
		if (BUB < global_BUB - tolerance)
		{
			global_BUB = BUB;
			BUB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			cout << "New BUB= " << global_BUB << "\t Time: " << BUB_time << endl;
			//feasible_zik();
			//cin.get();
			update_list_new_BUB = true;
		}


		//Save the varibales Y's associated with the restriction of Zik.
		for (int kk = 0; kk < branch_varZ_k; kk++)
		{
			for (int lenK = 1; lenK < Dik3[branch_varZ_i][kk].size(); lenK++)
			{
				(*this_active_node).branch_zik_y.push_back(Dik3[branch_varZ_i][kk][lenK]);

			}
		}

		//Number of children for this active node.
		int num_Y = (*this_active_node).Num_children[0] + (*this_active_node).Num_children[1] + (*this_active_node).Num_children[2];
	
		(*this_active_node).active_children = num_Y; //Number of children.
		(*this_active_node).branching_Zi = branch_varZ_i; //Index i of the selected Zik to branch.
		(*this_active_node).branching_Zk = branch_varZ_k; //Index k of the selected Zik to branch.
		(*this_active_node).lower_bound = BLB;
		//cout << "alpha: " << alpha << endl;
		//(*this_active_node).alpha = alpha;

		//Check if there is a new lower bound for the tree.
		if ((*this_active_node).parent_node->lower_bound_tree > BLB+tolerance)
		{
			(*this_active_node).lower_bound_tree = (*this_active_node).parent_node->lower_bound_tree;
		}
		else
		{
			(*this_active_node).lower_bound_tree = BLB;
		}

		if ((*this_active_node).Num_children[0] >1)
		{
			cout << "Branch Y's= ";
			for (size_t i = 0; i < (*this_active_node).branch_zik_y.size(); i++)
			{
				cout << (*this_active_node).branch_zik_y[i] << ", ";
			}
			cout << endl;
		}


		if (!terminateBB)
		{
			cout << "Num childrens: " << (*this_active_node).active_children << endl;
			cout << "lower_bound: " << (*this_active_node).lower_bound << endl;
			cout << "Global BUB: " << global_BUB << endl;
			cout << "BUB node: " << BUB << endl;
			cout << "Gap: " << Gap << endl;
			//cout << "Final alpha: " << (*this_active_node).alpha << endl;
		}
		
		//Add Node to the list if it has a bound lower than the BUB>
		if (BLB < global_BUB + tolerance)
		{
			Insert_node_List(this_active_node);
		}
	}

	
	//cin.get();
	return;
}