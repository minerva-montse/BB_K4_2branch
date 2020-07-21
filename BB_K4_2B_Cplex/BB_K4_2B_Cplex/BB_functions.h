#pragma once

void Insert_node_List(node_i *node_to_insert)
{
	//Add node to the active list.
	cout << "Insert node to list" << endl;


	++active_nodes; //Increase the number of active nodes.

	active_n *temp;  //active_n for the first active node in the list.
	//temp = the_first_active_node;
	temp = the_first_node_tree;
	double temp_node_to_insert_LB = (*node_to_insert).lower_bound; //LB of the node to insert.

	//cout << "First active node LB: " << (*the_first_active_node).the_info->lower_bound << endl;
	//cout << "First active node on tree LB: " << (*the_first_node_tree).the_info->lower_bound << endl;
	//cout << "Node to insert LB: " << temp_node_to_insert_LB << endl;

	/* Best node search: lower bounds go first. Should there be a tie, the newest node goes last. */
	while (((*temp).next != NULL) && (*temp).the_info->lower_bound < temp_node_to_insert_LB + tolerance)
	{
		temp = (*temp).next; //temp is the last node in the list.
	}

	if (((*temp).next == NULL) && (*temp).the_info->lower_bound < temp_node_to_insert_LB + tolerance)
	{
		//Insert the new node at the end of the list.
		(*temp).next = Create_a_n();
		(*temp).next->previous = temp;
		temp = (*temp).next;
		(*temp).next = NULL;
	}
	else if (((*temp).previous == NULL) && temp_node_to_insert_LB < ((*temp).the_info->lower_bound) + tolerance)
	{
		//Insert node at the first position.
		active_n *temp2 = Create_a_n();
		(*temp2).previous = NULL;
		(*temp2).next = temp;
		(*temp).previous = temp2;
		temp = temp2;
		temp2 = NULL;
		delete temp2;

		the_first_node_tree = temp;
	}
	else
	{
		//Insert the new node between 2 nodes in the list.

		//If we insert node C between A and B. temp = B

		temp = (*temp).previous; //temp = A
		active_n *temp2 = Create_a_n(); //Node C
		(*temp2).previous = temp; //C.previous = A
		(*temp2).next = (*temp).next; //C.next = B
		(*temp).next->previous = temp2; //Node B.previous = C
		(*temp).next = temp2; //Node A. next = C
		temp = (*temp).next; //temp = C

		temp2 = NULL;
		delete temp2;
	}
	//Node temp get the info of the node_to_insert.
	(*temp).the_info = node_to_insert;

	temp = NULL;
	delete temp;


	//To check the full tree.
	//active_n *temp3;
	//temp3 = the_first_node_tree;
	//while ((*temp3).next != NULL)
	//{
	//	cout << "Node " << (*temp3).the_info->number_node << ": " << (*temp3).the_info->lower_bound << endl;
	//	cout << "Next node" << (*temp3).next->the_info->number_node << ": " << (*temp3).next->the_info->lower_bound << endl;
	//	cout << "Previous: " << (*temp3).previous << endl;
	//	cin.get();
	//	temp3 = (*temp3).next;
	//}
	//cin.get();
	//temp3 = NULL;
	//delete temp3;
	//cout << "End insert function" << endl;
	//cin.get();

	return;
}

void Prune_node(node_i *node_to_prune)
{
	//Count the number of nodes that are prune.
	++PruneNodes;

	node_i *parent_node = (*node_to_prune).parent_node;
	return;
	if (parent_node == NULL)
	{
		delete node_to_prune;
	}
	else
	{
		if ((*node_to_prune).branch == 0) //set to NULL the coresponding branch of the parent node.
		{
			for (size_t i = 0; i < (*parent_node).left_sons.size(); i++)
			{
				(*parent_node).left_sons[i] = NULL;
			}
		}
		else
			(*parent_node).right_son = NULL;

		(*node_to_prune).parent_node = NULL;
		delete node_to_prune;

		--(*parent_node).active_children;

		//If the parent node doesn't have any children, parent node is prune.
		if ((*parent_node).active_children == 0)
			Prune_node(parent_node);
	}
	return;
}

void DeleteTree()
{
	cout << "Deleting the tree." << endl;
	while (the_first_active_node->next != NULL)
	{
		active_n *temp;
		temp = the_first_active_node;
		the_first_active_node = the_first_active_node->next; //Pointing to the SECOND node in the tree.
		the_first_active_node->previous = NULL; //Setting to NULL the FIRST node in the tree.

		//Setting to NULL all the information of the temp active_n.
		temp->next = NULL;
		this_active_node = temp->the_info; //Copy the node_i of temp node, to be able to prune that node.
		temp->the_info = NULL;
		temp = NULL;
		Prune_node(this_active_node);
		delete temp;
	}

	/*
	Deleting the nodes:
	 - this_active_node
	 - the_first_active_node
	 - the_first_node_tree
	*/

	this_active_node = the_first_active_node->the_info;
	the_first_active_node->the_info = NULL;
	Prune_node(this_active_node);
	this_active_node = NULL;
	delete(this_active_node);
	the_first_node_tree = NULL;
	delete(the_first_node_tree);
	the_first_active_node = NULL;
	delete(the_first_active_node);

	return;
}

void update_list_node()
{
	//To check the full tree.
	active_n *temp;
	temp = the_first_active_node;

	bool last_node = false;

	//cout << "Active node= " << active_nodes << endl;
	while (last_node == false)
	{
		//cout << "Node " << (*temp).the_info->number_node << ": " << (*temp).the_info->lower_bound << endl;

		if ((*temp).the_info->lower_bound > global_BUB)
		{
			if ((*temp).previous != NULL && (*temp).next != NULL)
			{
				//cout << "Prune node. Is in between 2 nodes" << endl;

				active_n *temp_previous;
				temp_previous = (*temp).previous;
				active_n *temp_next;
				temp_next = (*temp).next;
				(*temp).previous->next = temp_next;
				(*temp).next->previous = temp_previous;

				temp_previous = NULL;
				temp_next = NULL;
				delete temp_previous;
				delete temp_next;
				--active_nodes;
			}
			else if ((*temp).next == NULL)
			{
				//cout << "Prune node. Is the last node of list." << endl;
				(*temp).previous->next = NULL;
				--active_nodes;
			}
		}

		if ((*temp).next == NULL)
			last_node = true;
		else
			temp = (*temp).next;

	}

	temp = NULL;
	delete temp;
	cout << "Active node= " << active_nodes << endl;

	cout << "End insert function" << endl;
	//cin.get();
}

void best_lower_bound_tree()
{
	//To check the full tree.
	active_n *temp;
	temp = the_first_active_node;
	BLB_tree = (*temp).the_info->lower_bound_tree;

	while ((*temp).next != NULL)
	{
		if ((*temp).next->the_info->lower_bound_tree < BLB_tree +tolerance)
		{
			BLB_tree = (*temp).next->the_info->lower_bound_tree;
		}
		temp = (*temp).next;
	}
	temp = NULL;
	delete temp;
}

void feasible_zik()
{
	for (int i = 0; i < n; i++)
	{
		int temp_Yj = 0;
		temp_Yj = Yi[i];
		for (size_t k = 2; k < Gi[i]; k++)
		{
			for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
			{
				temp_Yj += Yi[Dik3[i][k - 1][j]];
				//cout << Dik3[i][k - 1][j] << ", ";
			}
			//cout << "tempY_[" << i << "][" << k << "]: " << temp_Yj << endl;
			if (temp_Yj > 0)
			{
				Zik[i][k - 2] = 0;
			}
			else
			{
				Zik[i][k - 2] = 1;
			}
		}
		//cin.get();
	}

	double temp_sumDi2 = 0;
	for (size_t i = 0; i < n; i++)
	{
		temp_sumDi2 += Dik3[i][1][0] * (1 - Yi[i]);
	}

	double temp_DikZik = 0;
	for (int i = 0; i < n; i++)
	{
		for (int k = 2; k < Gi[i]; k++)
		{
			temp_DikZik += (Dik3[i][k][0] - Dik3[i][k - 1][0])*Zik[i][k - 2];
		}
	}
	cout << "temp_sumDi2: " << temp_sumDi2 << endl;
	cout << "temp_DikZik: " << temp_DikZik << endl;
	cout << "total: " << temp_sumDi2 + temp_DikZik << endl;
	//cin.get();

	//for (int i = 0; i < n; i++)
	//{
	//	for (size_t k = 2; k < Gi[i]; k++)
	//	{
	//		cout << "Zik[" << i << "][" << k << "]; " << Zik[i][k - 2] << endl;
	//	}
	//	cin.get();
	//}
}