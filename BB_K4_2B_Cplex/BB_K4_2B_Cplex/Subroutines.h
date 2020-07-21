#pragma once

double solve_subpro_Z_select(int Zi, int Zk);
//double solve_subpro_Z_Lik_0();
//double solve_subpro_Y_Lik_0();
//void Solve_Lik_Node();

double Primal(vector<int> solY, int m, int p, matrixDoubles& Distances)
{
	int j, IteYi1;
	double minNum, Distij, maxNum, PrimalOV;
	j = 0; minNum = 0; PrimalOV = 0; IteYi1 = 0; Distij = 0; maxNum = 0;

	for (int i = 0; i < m; i++)
	{
		if (solY[i] == 0)
		{
			while (j < m && IteYi1 < m)
			{
				if (solY[j] == 1)
				{
					IteYi1++;
					if (j < i)
						Distij = Distances[j][i - j];
					else
						Distij = Distances[i][j - i];
					//Distij = Distances[i][j]; //for square matrix
					if (minNum == 0)
						minNum = Distij;
					else
					{
						maxNum = Distij;
						if (maxNum < minNum) {
							minNum = maxNum;
						}
					}
				}
				j++;
			}
			PrimalOV += minNum;
		}
		j = 0; minNum = 0; IteYi1 = 0;
	}
	return PrimalOV;
}

void output_BB()
{
	ofstream outfile;
	outfile.open("Results/BB_results.csv", std::fstream::app);
	puts("Writing to the file");

	//header of file
	outfile << "File," << char(9);
	outfile << "n," << char(9);
	outfile << "p," << char(9);
	outfile << "Ite_Root," << char(9);
	outfile << "Ite_general_nodes," << char(9);
	outfile << "alpha," << char(9);
	outfile << "BUB," << char(9);
	outfile << "Global BLB," << char(9);
	outfile << "Incuumbent," << char(9);
	outfile << "Next Node LB," << char(9);
	outfile << "Number Nodes," << char(9);
	outfile << "Active Nodes," << char(9);
	outfile << "Feasible Nodes," << char(9);
	outfile << "Infeasible Nodes," << char(9);
	outfile << "Prune Nodes," << char(9);
	outfile << "Time," << char(9);
	outfile << "SetUP time," << char(9);
	outfile << "Solve time," << char(9);
	outfile << "Subgradient time," << char(9);
	outfile << "BUB time," << char(9);
	outfile << "Incumbent time," << endl;

	//Results.
	outfile << file_name << char(9);
	outfile << "," << n << char(9);
	outfile << "," << p << char(9);
	outfile << "," << Ite_Root << char(9);
	outfile << "," << Ite_general_nodes << char(9);
	outfile << "," << alpha2 << char(9);
	outfile << "," << global_BUB << char(9);
	outfile << "," << BLB_tree << char(9);
	outfile << "," << incumbent_sol << char(9);
	outfile << "," << WLB << char(9);
	outfile << "," << NodeNumber << char(9);
	outfile << "," << active_nodes << char(9);
	outfile << "," << FeasibleNodes << char(9);
	outfile << "," << InfeasibleNodes << char(9);
	outfile << "," << PruneNodes << char(9);
	outfile << "," << BB_time << char(9);
	outfile << "," << setUP_once_model_time << char(9);
	outfile << "," << solve_model_time << char(9);
	outfile << "," << subgradient_model_time << char(9);
	outfile << "," << BUB_time << char(9);
	outfile << "," << incumbent_time << endl;

	//close the opened file.
	outfile.close();
}

void output_Feasible_Solutions(double ver_OF)
{
	ofstream outfile;
	outfile.open("Results/BB_Feasible_Solution.csv", std::fstream::app);
	puts("Writing to the file");

	//header of file
	outfile << "File," << char(9);
	outfile << "n," << char(9);
	outfile << "p," << char(9);
	outfile << "Ite_Root," << char(9);
	outfile << "Number Nodes," << char(9);
	outfile << "BUB," << char(9);
	outfile << "BLB," << char(9);
	outfile << "Incumbent," << char(9);
	outfile << "Time," << char(9);
	//outfile << "Gap," << char(9);
	outfile << "Y's," << endl;

	//Results.
	outfile << file_name << char(9);
	outfile << "," << n << char(9);
	outfile << "," << p << char(9);
	outfile << "," << Ite_Root << char(9);
	outfile << "," << NodeNumber << char(9);
	outfile << "," << global_BUB << char(9);
	outfile << "," << ver_OF << char(9);
	outfile << "," << incumbent_sol << char(9);
	outfile << "," << incumbent_time << char(9);
	//outfile << "," << Gap << char(9);

	outfile << ",";
	for (int i = 0; i < n; i++)
	{
		if (Yi[i] == 1)
		{
			outfile << i << ", ";
		}

	}
	outfile << endl;
}

void readData2(int argc, char ** argv)
{
	////If we want to start without indicating the file.
	//strcat_s(file_name, "data/pmed00.dat");
	//n = 12;
	//p = 3;
	////Create an input file stream
	//ifstream in(file_name, ios::in);
	//int number;  //Variable to hold each number as it is read
	//printf("The file is:%s\n", file_name);

	//We need to indicate the file to start.
	//Define the name/location of the file acording to the input parameter.
	strcat_s(file_name, "data/");
	strcat_s(file_name, argv[1]);
	printf("The file is:%s\n", file_name);
	strcat_s(instance, argv[1]);
	//Create an input file stream
	ifstream in(file_name, ios::in);
	int number;  //Variable to hold each number as it is read
	in >> number;
	n = number;// size of the problem, number of costumers
	in >> number;
	p = number; // facilite to locate using the value in the file
	//const int p = atoi(argv[3]);// facilite to locate using the value in the input parameters

	printf("The value of m: %d\n", n);
	printf("The value of p: %d\n", p);
	
	//para que sirve la siguiente linea?
	//_itoa(alpha, alpha_string, 10);
	//Read number using the extraction (>>) operator, using the triangular form of the data.
	//For index j, we need (n - 1 - i), because the we only had n-1 elements in the first row (we don't consider 0's).
	for (int i = 0; i < n; i++) {
		vector<double> temp;
		temp.push_back(0);
		for (int j = 0; j < n - 1 - i; j++)
		{
			in >> number;
			temp.push_back(number);
		}
		dist.push_back(temp);
	}
	//Close the file stream
	in.close();

	/*loop to sort and eliminate repetead distances.
	Define the size and initialize the arrys for Lik, Dik, Denik.*/
	Gi.resize(n);
	SumDi2 = 0;
	for (int i = 0; i < n; i++)
	{
		//cout << "i= " << i << " ";
		vector<double> dist_temp;
		dist_temp.resize(n);
		for (int j = 0; j < n; j++)
		{
			if (j < i)
			{
				dist_temp[j] = dist[j][i - j];
			}
			else
			{
				dist_temp[j] = dist[i][j - i];
			}
		}

		//cout << endl;
		vector<double> dist_temp2(dist_temp);

		sort(dist_temp.begin(), dist_temp.end()); //Sort the distances in ascending order.
		dist_temp.erase(dist_temp.end()-p+1,dist_temp.end());//Eliminate de 'p-1' largest distances.
		dist_temp.erase(unique(dist_temp.begin(), dist_temp.end()), dist_temp.end()); // Eliminates repetead distances

		Gi[i] = dist_temp.size();
		//Gi[i] = 400;
		//cout << "Gi[" << i << "]: " << Gi[i] << endl;

		//Give the size to the needes vectors.
		Lik.push_back(vector <double>(Gi[i] - k_radious + 1, 0));
		Denik.push_back(vector <double>(Gi[i] - k_radious + 1, 0));
		Zik.push_back(vector <double>(Gi[i] - k_radious + 1, 0));
		Zik_fix.push_back(vector <bool>(Gi[i] - k_radious + 1, false));

		matrixDoubles Dik;
		rji.resize(n, vector <int>(n, n + 1));

		for (int j = 0; j < dist_temp.size(); j++)
		{
			vector<double> dist_temp3;
			double tempDik = dist_temp[j];
			dist_temp3.push_back(tempDik);
			for (int k = 0; k < dist_temp2.size(); k++)
				if (dist_temp2[k] == tempDik)
					dist_temp3.push_back(k);

			Dik.push_back(dist_temp3);
			for (int t = 1; t < dist_temp3.size(); t++)
			{
				rji[i][dist_temp3[t]] = j;
			}
		}
		Dik3.push_back(Dik);

		//cout << "rij= ";
		//for (size_t j = 0; j < rji.size(); j++)
		//{
		//	cout << rji[i][j] << ",";
		//	
		//}
		//cout << endl;
		//cin.get();


		//cout << "Dik3= " << endl;
		//cout << "Dik[" << i << "][" << 2 << "]= " << i << endl;
		//for (size_t k = 2; k < Gi[i]+1; k++)
		//{
		//	cout << "Dik[" << i << "][" << k << "]= ";
		//	for (size_t j = 1; j < Dik3[i][k-1].size(); j++)
		//	{
		//		cout << Dik3[i][k-1][j] << ",";
		//	}
		//	cout << endl;
		//}
		//cout << endl;
		//cin.get();

		//SumDi2 is a constant, we can calculated only once.
		SumDi2 += Dik3[i][1][0];
	}

	//cin.get();
	int smallest_element = Gi[0]; //let, first element is the smallest one
	int largest_element = Gi[0]; //also let, first element is the biggest one
	for (int i = 1; i < n; i++)  //start iterating from the second element
	{
		if (Gi[i] < smallest_element)
		{
			smallest_element = Gi[i];
		}
		if (Gi[i] > largest_element)
		{
			largest_element = Gi[i];
		}
	}
	max_Gi = largest_element;


	//Initialize the Multipliers to Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0];
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < Lik[i].size(); k++)
		{
			Lik[i][k] = Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0];
		}
	}

	initial_Lik = Lik;

	//for checking.
	//for (int i = 0; i < n; i++)
	//{
	//	for (int k = 2; k < Gi[i]; k++)
	//	{
	//		cout << i << "," << k << ", ";
	//		for (int kk = 0; kk < k; kk++)
	//		{
	//			for (int lenK = 1; lenK < Dik3[i][kk].size(); lenK++)
	//			{
	//				cout << Dik3[i][kk][lenK] << ", ";
	//			}
	//		}
	//		cout << endl;
	//	}
	//	cout << endl;
	//}

	//for checking.
	//for (int i = 0; i < n; i++)
	//{
	//	for (int k = 2; k < Gi[i]; k++)
	//	{
	//		cout << i << "," << k <<","<< Dik3[i][k][0] << "," << Dik3[i][k -1][0] << endl;
	//	}
	//}

}

double myopic(int n, int p, matrixDoubles& Distances)
{
	int j, IteYi1;
	double minNum, Distij, maxNum, PrimalOV;
	matrixDoubles temp_dist;
	int fix_p = 0;
	double sum_Yj;
	double temp_min_Yj = INFINITY;
	vector <int> heuristic_Y;
	heuristic_Y.resize(p);

	for (size_t i = 0; i < n; i++)
	{
		vector <double> temp_dist_i;
		for (size_t j = 0; j < n; j++)
		{
			if (j < i)
				temp_dist_i.push_back(Distances[j][i - j]);
			else
				temp_dist_i.push_back(Distances[i][j - i]);
		}
		temp_dist.push_back(temp_dist_i);
	}

	for (size_t t = 0; t < p; t++)
	{
		for (size_t j = 0; j < n; j++)
		{
			sum_Yj = 0;
			for (size_t i = 0; i < n; i++)
			{
				if (fix_p > 0)
				{
					temp_dist[i][j] = min(temp_dist[i][j], temp_dist[i][heuristic_Y[fix_p - 1]]);
					sum_Yj += temp_dist[i][j];
				}
				else
				{
					sum_Yj += temp_dist[i][j];
				}
			}

			if (sum_Yj < temp_min_Yj)
			{
				temp_min_Yj = sum_Yj;
				heuristic_Y[fix_p] = j;
			}


		}
		++fix_p;
		//cout << "fix_p= " << fix_p << endl;
		//cout << "The min is Y[" << heuristic_Y[fix_p-1] << "]= " << temp_min_Yj << endl;
	}
	cout << "The min for p= " << p << " is= " << temp_min_Yj << endl;
	//cin.get();
	return temp_min_Yj;
}

void updateLik()
{
	//Update the Lagrange Multipliers.
	for (int i = 0; i < n; i++)
	{
		for (int k = 0; k < (Gi[i] - k_radious + 1); k++)
		{
			Lik[i][k] = max(0.0, Lik[i][k] + Stepsize_tn * Denik[i][k]);
		}
	}
}

void select_Zik()
{
	branch_varZ_val = 0;
	branch_varZ_i = 0;
	branch_varZ_k = 0;

	for (int k = 0; k < 10; k++)
	{
		//cout << "HERE 3, k= " << 0+2 << endl;
		if (branch_varZ_i == 0 && branch_varZ_k == 0)
		{
			for (int i = 0; i < n; i++)
			{
				//cout << "HERE 4, i=" << i << ", k= " << k+2 << endl;
				double temp_branch_varZ_val = branch_varZ_val;
				double temp_valZ = Lik[i][k] * Denik[i][k];
				//cout << "temp_valZ: " << temp_valZ << endl;
				if (temp_valZ > temp_branch_varZ_val + tolerance)
				{
					branch_varZ_i = i;
					branch_varZ_k = k + 2;
					branch_varZ_val = temp_valZ;
					//cout << "branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]= " << branch_varZ_val << endl;
				}
			}
		}
		else
		{
			k = n - 1;
		}
	}

	cout << "End select_Zik routine" << endl;
	cout << "branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]= " << branch_varZ_val << endl;
}

void select_Zik_2()
{
	//cout << "select_Zik_2." << endl;
	branch_varZ_val = 0;
	branch_varZ_i = 0;
	branch_varZ_k = 0;

	for (int k = 0; k < (n - k_radious + 1); k++)
	{
		cout << endl;
		//cout << "k= " << k + 2 << endl;
		if (branch_varZ_i == 0 && branch_varZ_k == 0)
		{
			for (int i = 0; i < n; i++)

			{
				if (k < (Gi[i] - k_radious + 1))
				{
					//printf("Ahora %i %i %lf %lf\n", i, k, (double)Zik[i][k], (double)Denik[i][k]);
					//printf("primera %i\n", abs(Denik[i][k])<tolerance);
					//printf("segunda %i\n", Zik[i][k] > 1 - tolerance);
					double temp_branch_varZ_val = branch_varZ_val;
					double temp_valZ = best_Lik[i][k] * best_Denik[i][k];
					double temp_changeOF = abs(Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0] - best_Lik[i][k]);
					//if (Denik[i][k] < 1 && Zik[i][k] == 0)
					if ((best_Denik[i][k] < tolerance) && (best_Zik[i][k] < tolerance))
					{
						cout << "case 1:" << "\t Zik[" << i << "][" << k + 2 << "]: " << best_Zik[i][k] << "\t Denik[i][k]: " << best_Denik[i][k] << "\t Lik[" << i << "][" << k + 2 << "]: " << best_Lik[i][k] << "\t + OF: " << temp_valZ << endl;
						cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0] << endl;
					}
					//else if (Denik[i][k] ==0 && Zik[i][k] == 1)
					else if ((abs(best_Denik[i][k]) < tolerance) && (best_Zik[i][k] > 1 - tolerance))
					{
						cout << "case 4:" << "\t Zik[" << i << "][" << k + 2 << "]: " << best_Zik[i][k] << "\t Denik[i][k]: " << best_Denik[i][k] << "\t Lik[" << i << "][" << k + 2 << "]: " << best_Lik[i][k] << "\t + OF: " << temp_valZ << endl;
						cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0] << endl;
					}
					//					else if (Denik[i][k] == 1 && Zik[i][k] == 0)
					else if ((best_Denik[i][k] > 1 - tolerance) && (best_Zik[i][k] < tolerance))
					{
						cout << "case 2:" << "\t Zik[" << i << "][" << k + 2 << "]: " << best_Zik[i][k] << "\t Denik[i][k]: " << best_Denik[i][k] << "\t Lik[" << i << "][" << k + 2 << "]: " << best_Lik[i][k] << "\t + OF: " << temp_valZ << endl;
						cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0] << endl;
						branch_varZ_i = i;
						branch_varZ_k = k + 2;
						temp_branch_VarZ = temp_changeOF;

						//This Variable is selected to branch, the loop will terminate.
						i = n;
						k = n;
					}
					//else if (Denik[i][k] < 1 && Zik[i][k] == 1)
					else if ((best_Denik[i][k] < -1 + tolerance) && (best_Zik[i][k] > 1 - tolerance))
					{
						cout << "case 3:" << "\t Zik[" << i << "][" << k + 2 << "]: " << best_Zik[i][k] << "\t Denik[i][k]: " << best_Denik[i][k] << "\t Lik[" << i << "][" << k + 2 << "]: " << best_Lik[i][k] << "\t + OF: " << temp_valZ << endl;
						cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][k + 2][0] - Dik3[i][k + 2 - 1][0] << endl;
						branch_varZ_i = i;
						branch_varZ_k = k + 2;
						temp_branch_VarZ = temp_changeOF;

						//This Variable is selected to branch, the loop will terminate.
						i = n;
						k = n;
					}
					else { printf("nada\n"); }
				}



			}
		}

	}

	cout << "End select_Zik routine" << endl;
	cout << "branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]= " << branch_varZ_val << endl;

	//In the case when there's nothing to branch (Zik[0][0]). We check if the multipliers are set to zero, if we get the optimal.
	if (branch_varZ_i == 0 && branch_varZ_k == 0)
	{
		cout << endl;
		cout << "The branching variable is Zik[0][0]:" << endl;
		cout << "If we make the Multipliers 0:" << endl;

		OV_Sub_Z = 0;
		OV_Sub_Y = 0;
		sumLik = 0;

		for (int k = 0; k < (n - k_radious + 1); k++)
		{
			for (int i = 0; i < n; i++)
			{
				if (k < (Gi[i] - k_radious + 1))
				{
					if (Denik[i][k] < 1 && Zik[i][k] == 0)
					{
						Lik[i][k] = 0;
					}
					else if (Denik[i][k] == 0 && Zik[i][k] == 1)
					{
						Lik[i][k] = 0;
					}
				}
			}
		}

		double DY = 0;
		//Solve Subproblem 1. Find the values for all Zik.
		for (int i = 0; i < n; i++)
		{
			for (int k = 2; k < Gi[i]; k++)
			{
				//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
				double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
				if (Zik[i][k - 2] == 1)
				{
					OV_Sub_Z += temp - Lik[i][k - 2];
				}
				//The sum of ALL Lagrangians is calculated.
				sumLik += Lik[i][k - 2];

				OV_Sub_Y += Lik[i][k - 2] * Denik[i][k - 2];
			}
			DY -= Yi[i] * Dik3[i][1][0];
		}
		cout << "OV_Sub_Z: " << OV_Sub_Z << endl;
		cout << "OV_Sub_Y: " << OV_Sub_Y << endl;
		cout << "Di,2 * Yi: " << DY << endl;
		cout << "SumDi2: " << SumDi2 << endl;
		cout << "sumLik: " << sumLik << endl;

		double ver_OF;
		ver_OF = OV_Sub_Z + OV_Sub_Y + DY + SumDi2 + sumLik;
		cout << "OF= " << ver_OF << endl;

		if ((global_BUB - ver_OF < tolerance))
		{
			cout << "Optimal found." << endl;
			Gap = (global_BUB - ver_OF) / global_BUB;
			cout << "GAP= " << Gap << endl;
			BLB = ver_OF;
			//cin.get();
		}
	}
}

void select_Zik_3()
{
	temp_branch_VarZ = 0;
	branch_varZ_i = 0;
	branch_varZ_k = 0;
	double sum_LR = 0;



	for (int k = 0; k < (n - k_radious + 1); k++)
	{
		if (branch_varZ_i == 0 && branch_varZ_k == 0)
		{
			for (int i = 0; i < n; i++)
			{
				if (k < Gi[i] - k_radious + 1)
				{
					int temp = (*this_active_node).branch_lower_level[i] - 2 + k;
					int branchink_k = temp;
					if (branchink_k < (Gi[i] - k_radious + 1))
					{
						double temp_branch_varZ_val = temp_branch_VarZ;
						double temp_valZ = best_Lik[i][branchink_k] * best_Denik[i][branchink_k];
						double temp_changeOF = abs(Dik3[i][branchink_k + 2][0] - Dik3[i][branchink_k + 2 - 1][0] - best_Lik[i][branchink_k]);
						if ((best_Denik[i][branchink_k] > 1 - tolerance) && (best_Zik[i][branchink_k] < tolerance))
						{
							//cout << "case 2:" << "\t Zik[" << i << "][" << branchink_k + 2 << "]: " << best_Zik[i][branchink_k] << "\t Denik[i][k]: " << best_Denik[i][branchink_k] << "\t Lik[" << i << "][" << branchink_k + 2 << "]: " << best_Lik[i][branchink_k] << "\t + OF: " << temp_valZ << endl;
							//cout << "case 2:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][branchink_k + 2][0] - Dik3[i][branchink_k + 2 - 1][0] << endl;
							//cout << "Absolute: " << temp_changeOF << endl;

							if (temp_changeOF > temp_branch_varZ_val + tolerance)
							{
								
								branch_varZ_i = i;
								branch_varZ_k = branchink_k + 2;
								temp_branch_VarZ = temp_changeOF;
							}
						}
						else if ((best_Denik[i][branchink_k] < -1 + tolerance) && (best_Zik[i][branchink_k] > 1 - tolerance))
						{
							cout << "case 3:" << "\t Zik[" << i << "][" << branchink_k + 2 << "]: " << best_Zik[i][branchink_k] << "\t Denik[i][k]: " << best_Denik[i][branchink_k] << "\t Lik[" << i << "][" << branchink_k + 2 << "]: " << best_Lik[i][branchink_k] << "\t + OF: " << temp_valZ << endl;
							//cout << "case 3:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][branchink_k + 2][0] - Dik3[i][branchink_k + 2 - 1][0] << endl;
							//cout << "Absolute: " << temp_changeOF << endl;
							if (temp_changeOF > temp_branch_varZ_val + tolerance)
							{
								branch_varZ_i = i;
								branch_varZ_k = branchink_k + 2;
								temp_branch_VarZ = temp_changeOF;
							}
						}
						else if ((best_Denik[i][branchink_k] < tolerance) && (best_Zik[i][branchink_k] < tolerance))
						{
							cout << "case 1:" << "\t Zik[" << i << "][" << branchink_k + 2 << "]: " << best_Zik[i][branchink_k] << "\t Denik[i][k]: " << best_Denik[i][branchink_k] << "\t Lik[" << i << "][" << branchink_k + 2 << "]: " << best_Lik[i][branchink_k] << "\t + OF: " << temp_valZ << endl;
							//cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][branchink_k + 2][0] - Dik3[i][branchink_k + 2 - 1][0] << endl;
							sum_LR += best_Lik[i][branchink_k] * best_Denik[i][branchink_k];
							if (best_Lik[i][branchink_k] > tolerance && (best_Denik[i][branchink_k] < 0 - tolerance))
							{
								cout << "Lik[" << i << "][" << branchink_k + 2 << "]: " << best_Lik[i][branchink_k] << "\t Dik: " << best_Denik[i][branchink_k] << endl;
							}

						}
						else if ((abs(best_Denik[i][branchink_k]) < tolerance) && (best_Zik[i][branchink_k] > 1 - tolerance))
						{
							//cout << "case 4:" << "\t Zik[" << i << "][" << branchink_k + 2 << "]: " << best_Zik[i][branchink_k] << "\t Denik[i][k]: " << best_Denik[i][branchink_k] << "\t Lik[" << i << "][" << branchink_k + 2 << "]: " << best_Lik[i][branchink_k] << "\t + OF: " << temp_valZ << endl;
							//cout << "case 1:" << "Dik[i][k] - Dik[i][k-1]= " << Dik3[i][branchink_k + 2][0] - Dik3[i][branchink_k + 2 - 1][0] << endl;
						}
						else
						{
							cout << "Checar que esta pasando en este caso." << endl;
							cin.get();
						}
					}
				}
			}
			if (branch_varZ_i != 0 && branch_varZ_k != 0)
			{
				//This Variable is selected to branch, the loop will terminate.
				k = n;
			}
		}

	}

	//cout << "End select_Zik routine" << endl;
	cout << "branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]" << endl;
	//cin.get();
	//In the case when there's nothing to branch (Zik[0][0]). We check if the multipliers are set to zero.
	if (branch_varZ_i == 0 && branch_varZ_k == 0)
	{
		cout << endl;
		cout << "The branching variable is Zik[0][0]:" << endl;
		cout << "If we make the Multipliers 0:" << endl;
		cout << "The sum_LR= " << sum_LR << endl;
		cout << "UB current solution: " << UB << endl;
		//cin.get();

		OV_Sub_Z = 0;
		OV_Sub_Y = 0;
		sumLik = 0;

		//Restart Multipliers to 0.
		for (int k = 0; k < (n - k_radious + 1); k++)
			for (int i = 0; i < n; i++)
				if (k < (Gi[i] - k_radious + 1))
					Lik[i][k] = 0;

		double DY = 0;

		//Solve Subproblem 1. Find the values for all Zik.
		for (int i = 0; i < n; i++)
		{
			for (int k = 2; k < Gi[i]; k++)
			{
				//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
				double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
				if (Zik[i][k - 2] == 1)
				{
					OV_Sub_Z += temp - Lik[i][k - 2];
				}
				//The sum of ALL Lagrangians is calculated.
				sumLik += Lik[i][k - 2];
				OV_Sub_Y += Lik[i][k - 2] * Denik[i][k - 2];
			}
			DY -= Yi[i] * Dik3[i][1][0];
		}
		double ver_OF;
		ver_OF = OV_Sub_Z + OV_Sub_Y + DY + SumDi2 + sumLik;
		double primal_Feasible_Solution = Primal(Yi, n, p, dist);

		cout << "OV_Sub_Z: " << OV_Sub_Z << endl;
		cout << "OV_Sub_Y: " << OV_Sub_Y << endl;
		cout << "Di,2 * Yi: " << DY << endl;
		cout << "SumDi2: " << SumDi2 << endl;
		cout << "sumLik: " << sumLik << endl;
		cout << "OF= " << ver_OF << endl;
		cout << "Global_BUB: " << global_BUB << endl;
		cout << "Best LB on node: " << best_OV << endl;
		//cout << "Primal= " << primal_Feasible_Solution << endl;
		cout << "Gap: " << Gap << endl;
		if ((abs(global_BUB - ver_OF) < tolerance) && Gap > Gap_limit + tolerance)
		{
			/*
			Feasible found. The Gap between the BUB and BLB is greater than the Gap_limit.
			The Multipliers will be reset, to restart the node and be able to found a Zik to Branch.
			*/

			cout << endl;
			cout << "Feasible found." << endl;
			cout << "The Gap between the BUB and BLB is greater than the Gap_limit." << endl;
			cout << "The Multipliers will be reset to zero, to restart the node and be able to found a Zik to Branch." << endl;
			cout << "Best LB on node: " << best_OV << endl;
			cout << "Primal solution (or Multipliers 0): " << ver_OF << endl;
			//Lik = initial_Lik;
			Ite = 0;

			output_Feasible_Solutions(ver_OF);
		}
		else if ((abs(global_BUB - ver_OF) < tolerance) && Gap < Gap_limit + tolerance)
		{
			/*
			Feasible found. The Gap between the BUB and BLB is less than the Gap_limit.

			The progam will terminate, likely to be the Optimal.
			*/
			cout << endl;
			cout << "Feasible found (maybe Optimal??)." << endl;
			cout << "The Gap between the BUB and BLB is less than the Gap_limit." << endl;
			cout << "Best LB on node: " << best_OV << endl;
			cout << "Primal solution (or Multipliers 0): " << ver_OF << endl;
			terminateBB = true;

			output_Feasible_Solutions(ver_OF);
		}
		else
		{
			/*
			Feasible found. But with a primal above the Global_BUB.
			The Multipliers will be reset, to restart the node and be able to found a Zik to Branch.
			*/
			cout << endl;
			cout << "Feasible solution, but with a primal above the Global_BUB." << endl;
			cout << "BLB: " << BLB << endl;
			cout << "Best LB on node: " << best_OV << endl;
			cout << "Best OV_Sub_Z on node: " << best_OV_Sub_Z << endl;
			cout << "Best OV_Sub_Y on node: " << best_OV_Sub_Y << endl;
			cout << "Best sumLik on node: " << best_sumLik << endl;
			cout << "Best SumDi2 on node: " << best_SumDi2 << endl;
			cout << "BLB: " << BLB << endl;
			//Lik = initial_Lik;
			Ite = 0;
			Ite = 0;

			output_Feasible_Solutions(ver_OF);
		}
	}

	//if (NodeNumber == 124)
	//{
	//	cout << "Node 124" << endl;
	//	cin.get();
	//}
}

void select_Zik_4()
{
	cout << "BLB (OVz-OVy+Lik+Di2)= " << best_OV << " = " << best_OV_Sub_Z << " - " << best_OV_Sub_Y << " + " << best_sumLik << " + " << best_SumDi2  << endl;

	cout << "Solution Y= ";
	for (size_t i = 0; i < n; i++)
	{
		if (Yi[i] == 1)
		{
			cout << i << ", ";
		}
	}
	cout << endl;

	double temp_changeOF = best_OV;
	temp_branch_VarZ = 0;
	branch_varZ_i = 0;
	branch_varZ_k = 0;
	double sum_LR = 0;
	  

	for (int i = 0; i < n; i++)
	{
		bool cut = false;
			//for (int k = k_radious - 1; k < (*this_active_node).branch_upper_level[i]; k++)
			for (int k = (*this_active_node).branch_lower_level[i]; k < (*this_active_node).branch_upper_level[i]; k++)
			{
				if ((best_Denik[i][k-2] > 1 - tolerance) && (best_Zik[i][k-2] < tolerance))
				{
					cut = true;
					//Calculate the OV if this Zik is selected, and we only solve it for the case where Zik=1.
					double temp_OV_Z = solve_subpro_Z_select(i, k);
					double temp_OV_select = temp_OV_Z - best_OV_Sub_Y + best_sumLik + best_SumDi2;
					//cout << "Zik[" << i << "][" << k << "]: " << best_Zik.at(i).at(k - 2) << "\t Denik[i][k]: " << best_Denik.at(i).at(k - 2) << "\t Lik[" << i << "][" << k << "]: " << best_Lik.at(i).at(k - 2) << "\t OV_Z0=" << temp_OV_select << endl;
					if (temp_OV_select > (temp_changeOF + tolerance))
					{
						temp_changeOF = temp_OV_select;
						branch_varZ_i = i;
						branch_varZ_k = k;
					}
				}
				if (cut)
				{
					//This Variable is selected to branch for client i, the loop will terminate.
					k = n;
				}
			}
	}
	cout << "Infeasible branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]" << endl;
	//cin.get();

	if (branch_varZ_i != 0 || branch_varZ_k != 0)
	{
		//A branch Zik was selected. We create a Right node and  a node for each Y of the candidates.
		vector <int> temp_candidate_Y;
		int temp_num_child = 0;
		for (int kk = 0; kk < branch_varZ_k; kk++)
		{
			for (int lenK = 1; lenK < Dik3[branch_varZ_i][kk].size(); lenK++)
			{
				temp_candidate_Y.push_back(Dik3[branch_varZ_i][kk][lenK]);
				++temp_num_child;
			}
		}
		//if ((*this_active_node).number_node==10)
		//{
		//	cout << "temp_num_child= " << temp_num_child << endl;
		//	cin.get();
		//}
		(*this_active_node).Num_children[0] = temp_candidate_Y.size(); //Left node
		//cout << "(*this_active_node).Num_children[0]= " << (*this_active_node).Num_children[0] << endl;
		(*this_active_node).Num_children[1] = 0; //Mid node
		(*this_active_node).Num_children[2] = 1; //Right node	

		//Count the number of nodes that are not feasible.
		++InfeasibleNodes;
	}

	//If all the constraints are feasible. (The branch to select is still Zik[0][0]). A infeasible constraint is selected. The one with the largest L*(b-A).
	if (branch_varZ_i == 0 && branch_varZ_k == 0)
	{
		//Count the number of nodes that are feasible.
		++FeasibleNodes;

		double sum_D_Z = 0;
		double temp = -INFINITY;
		for (int i = 0; i < n; i++)
		{
			for (int k = k_radious - 1; k < Gi[i]; k++)
			{
				sum_D_Z += (Dik3[i][k][0] - Dik3[i][k - 1][0])*best_Zik[i][k - 2];
				double temp_valZ = best_Lik[i][k-2] * best_Denik[i][k-2];
					if (best_Lik[i][k-2] > tolerance && (best_Denik[i][k-2] < 0 - tolerance))
					{
						//cout << "case 1:" << "\t Zik[" << i << "][" << k << "]: " << best_Zik[i][k-2] << "\t (b-A)= " << best_Denik[i][k-2] << "\t Lik= " << best_Lik[i][k-2] << "\t OF= " << temp_valZ << endl;
						//if (temp_valZ > temp+tolerance && (*this_active_node).zik_fix_node[i][k - 2] ==false)
						if (temp_valZ > temp + tolerance)
						{
							//cout << "Select this branch." << endl;
							temp = temp_valZ;
							branch_varZ_i = i;
							branch_varZ_k = k;
						}
					}

			}
		}

		cout << "L*(b-A)=0 branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]" << endl;
		
		//Check for BUB
		//Primal solution for current solution of Yi.
		double Sub_UB = Primal(Yi, n, p, dist);
		//Check if there is a new BUB.
		if (Sub_UB < global_BUB - tolerance)
		{
			global_BUB = Sub_UB;
			BUB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			cout << "New BUB= " << global_BUB << "\t Time: " << BUB_time << endl;
			update_list_new_BUB = true;
		}

		//Check how many Y's have been fixed previously and if has some in common to the candidate constraint to branch.
		vector <int> temp_candidate_Y;
		vector <int> temp_fix_Y;
		cout << "Y's related to branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]:  ";
		for (int kk = 0; kk < branch_varZ_k; kk++)
		{
			for (int lenK = 1; lenK < Dik3[branch_varZ_i][kk].size(); lenK++)
			{
				cout << Dik3[branch_varZ_i][kk][lenK] << ", ";
				temp_candidate_Y.push_back(Dik3[branch_varZ_i][kk][lenK]);
			}
		}
		cout << endl;

		cout << "Fix Y's=1 are:  ";
		for (size_t i = 0; i < (*this_active_node).branch_y_1.size(); i++)
		{
			cout << (*this_active_node).branch_y_1[i] << ", ";
			temp_fix_Y.push_back((*this_active_node).branch_y_1[i]);
		}
		//cout << endl;
		//cout << "Fix Y's=0 are:  ";
		//for (size_t i = 0; i < (*this_active_node).branch_y_0.size(); i++)
		//{
		//	cout << (*this_active_node).branch_y_0[i] << ", ";
		//}
		//cout << endl;
		//cout << "y[19]= " << Yi[19] << endl;

		sort(temp_candidate_Y.begin(), temp_candidate_Y.end());
		sort(temp_fix_Y.begin(), temp_fix_Y.end());

		vector <int> temp_intersection;
		set_intersection(temp_candidate_Y.begin(), temp_candidate_Y.end(), temp_fix_Y.begin(), temp_fix_Y.end(), back_inserter(temp_intersection));

		cout << "Have in common: ";
		for (size_t i = 0; i < temp_intersection.size(); i++)
		{
			cout << temp_intersection[i] << ", ";
		}
		cout << endl;
		
		//Fix the number/type of branch to created according to the Y's that had been fixed.
		if (temp_intersection.size() == 1)
		{
			//If they have 1 common element, we can't create the branch of Zik=1 and Y=0. A Y was previously set to 1. 
			cout << "Branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]= 1 can't be created. A Y was previously set to 1." << endl;
			cout << "Create Branch Z[" << branch_varZ_i << "][" << branch_varZ_k << "]=0 and Y[" << temp_intersection[0] << "]." << endl;
			//Number of children.
			(*this_active_node).Num_children[0] = 1; //Left node
			(*this_active_node).Num_children[1] = 1; //Mid node
			(*this_active_node).Num_children[2] = 0; //Right node
			(*this_active_node).Num_children[3] = temp_intersection[0]; //Create the Left branch for this Y variable.
		}
		else if (temp_intersection.size() == 0)
		{
			//If there's nothing fixed. We create a Right node, a Mid node and the a node for each Y of the candidates.
			(*this_active_node).Num_children[0] = temp_candidate_Y.size(); //Left node
			(*this_active_node).Num_children[1] = 1; //Mid node
			(*this_active_node).Num_children[2] = 1;//Right node
		}
		else
		{
			//2 or more Y variables of the constraint are fixed to 1. Only create branch Lik=0.
			cout << "2 or more Y variables of the constraint are fixed to 1. Only create branch Lik=0." << endl;
			
			(*this_active_node).Num_children[0] = 0; //Left node
			(*this_active_node).Num_children[1] = 1; //Mid node
			(*this_active_node).Num_children[2] = 0; //Right node
		}

		//Number of children for the "mid type" nodes. Set the node to have a child Lik=0
		//(*this_active_node).mid = true;

		//double sumDi2_Yi = 0;
		//for (size_t i = 0; i < n; i++)
		//{
		//	sumDi2_Yi += Dik3[i][1][0] * Yi[i];
		//}
	}
	
	//If it is not possible to select a constaint of the feasible ones.
	//double sum_D_Z = 0;
	//double temp = -INFINITY;
	if (branch_varZ_i == 0 && branch_varZ_k == 0)
	{
		Gap = (BUB - BLB) / BUB;
		UB = Primal(Yi, n, p, dist);
		Gap = (UB - best_OV) / UB;

		cout << "All constraints feasible and optimal, all L*(b-A)=0." << endl;
		cout << "Gap: " << Gap << endl;
		cout << "Primal: " << UB << endl;
		cout << "BLB: " << best_OV << endl;
		cout << "global_BUB: " << global_BUB << endl;
		global_BLB = best_OV;

		if ((abs(global_BUB - best_OV) < 0.1) && Gap > Gap_limit + tolerance)
		{
			/*
			Feasible found. The Gap between the BUB and BLB is greater than the Gap_limit.
			*/
			if (global_BUB<incumbent_sol-tolerance)
			{
				incumbent_sol = global_BUB;
				incumbent_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			}
			cout << endl;
			cout << "Feasible found." << endl;
			cout << "The Gap between the BUB and BLB is greater than the Gap_limit." << endl;
			cout << "Best LB on node: " << best_OV << endl;
			output_Feasible_Solutions(best_OV);
			//terminateBB = true;

			prune = true;
		}
		else if ((abs(global_BUB - best_OV) < 0.1) && Gap < Gap_limit + tolerance)
		{
			/*
			Feasible found. The Gap between the BUB and BLB is less than the Gap_limit.
			The progam will terminate, likely to be the Optimal.
			*/
			if (global_BUB < incumbent_sol-tolerance)
			{
				incumbent_sol = global_BUB;
				incumbent_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			}
			cout << "Feasible found (maybe Optimal??)." << endl;
			cout << "The Gap between the BUB and BLB is less than the Gap_limit of " << Gap_limit << "." << endl;
			cout << "Best LB on node: " << best_OV << endl;
			//terminateBB = true;
			BLB_tree = best_OV;
			output_Feasible_Solutions(best_OV);
			prune = true;
		}
		else
		{
			/*
			Feasible found. But with a primal above the Global_BUB.
			*/
			cout << endl;
			cout << "Feasible solution, but with a primal above the Global_BUB." << endl;
			cout << "BLB: " << BLB << endl;
			cout << "Best LB on node: " << best_OV << endl;
			cout << "Best OV_Sub_Z on node: " << best_OV_Sub_Z << endl;
			cout << "Best OV_Sub_Y on node: " << best_OV_Sub_Y << endl;
			cout << "Best sumLik on node: " << best_sumLik << endl;
			cout << "Best SumDi2 on node: " << best_SumDi2 << endl;
			cout << "BLB: " << BLB << endl;

			terminateBB = true;

			incumbent_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			output_Feasible_Solutions(best_OV);
		}
	}
	


	
}

void calculateSik()
{
	//clock_t start_calculateSik_time = clock();
	//double calculateSik_time = 0;

	//Stepsize_den = 0;
	//
	//for (int i = 0; i < n; i++)
	//{
	//	for (int k = 2; k < Gi[i]; k++)
	//	{
	//		int temp_sumYj = 0; //Restart the sum of Yj for next iteration.
	//		for (int j = 0; j < n; j++)
	//		{
	//			if (rji[i][j] < k)
	//			{
	//				temp_sumYj += Yi[j];
	//			}
	//			Denik[i][k - 2] = 1 - Zik[i][k - 2] - temp_sumYj; //1 - Zik - Sum Y's
	//			
	//		}
	//		Stepsize_den += (Denik[i][k - 2])*(Denik[i][k - 2]);
	//		//if (k==2)
	//		//{
	//		//	//cout << "den[" << i << "][" << k - 2 << "]= " << temp_sumYj << ",     Sik= " << Denik[i][k - 2] << endl;
	//		//	cout << "den[" << i << "][" << k - 2 << "]= " << Denik[i][k - 2] << " Zik= " << Zik[i][k - 2] << "  sum= " << temp_sumYj << endl;
	//		//}
	//
	//	}
	//	//cin.get();
	//}
	//calculateSik_time = (double)(clock() - start_calculateSik_time) / CLOCKS_PER_SEC;
	//cout << "calculateSik time: " << calculateSik_time << endl;
	//cout << "Stepsize den= " << Stepsize_den << endl;
	//cin.get();

	//start_calculateSik_time = clock();
	Stepsize_den = 0;
	for (int i = 0; i < n; i++)
	{
		int temp_Yj = 0;
		int temp_sumYj = 0;
		temp_sumYj += Yi[i];
		temp_Yj = Yi[i];

		int k = 2;
		for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
		{
			temp_Yj += Yi[Dik3[i][k - 1][j]];
			temp_sumYj += Yi[Dik3[i][k - 1][j]];
			//cout << Dik3[i][k - 1][j] << ", Y=" << Yi[Dik3[i][k - 1][j]] ;
		}
		//cout << endl;
		Denik[i][k - 2] = 1 - Zik[i][k - 2] - temp_Yj; //1 - Zik - Sum Y's
		Stepsize_den += (Denik[i][k - 2])*(Denik[i][k - 2]);
		//cout << "sum[" << i << "][" << k - 2 << "]= " << temp_Yj << ",     Sik= " << Denik[i][k - 2] << endl;

		for (size_t k = 3; k < Gi[i]; k++)
		{
			temp_Yj = 0;
			for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
			{
				temp_Yj += Yi[Dik3[i][k - 1][j]];
				temp_sumYj += Yi[Dik3[i][k - 1][j]];
				//cout << Dik3[i][k - 1][j] << ", Y=" << Yi[Dik3[i][k - 1][j]]  ;
			}
			//cout << endl;

			Denik[i][k - 2] = Denik[i][k - 2 - 1] - temp_Yj - Zik[i][k - 2] + Zik[i][k - 2 - 1]; //1 - Zik - Sum Y's
			//cout << Denik[i][k - 2] << " = " << Denik[i][k - 2 - 1] << " - " << temp_Yj << " + " << Zik[i][k - 2] << " - " << Zik[i][k - 2 - 1] << endl;
			Stepsize_den += (Denik[i][k - 2])*(Denik[i][k - 2]);
			//cin.get();
			//if (k==3)
			//{
			//	cout << "den[" << i << "][" << k - 2 << "]= " << Denik[i][k - 2] << "     Pden= " << Denik[i][k - 2 - 1] << "    Y's= " << temp_Yj <<  "    Zik= " << Zik[i][k - 2] << "   PZik= " << Zik[i][k - 2 - 1] << endl;
			//	//cout << "den[" << i << "][" << k - 2 << "]= " << temp_sumYj << ", " << temp_Yj << ",     Sik= " << Denik[i][k - 2] << endl;
			//}

		}
		//cin.get();
	}
	//calculateSik_time = (double)(clock() - start_calculateSik_time) / CLOCKS_PER_SEC;
	//cout << "calculateSik time: " << calculateSik_time << endl;
	//cout << "Stepsize den= " << Stepsize_den << endl;
	//cin.get();

}

void subgradient()
{
	//Compute the values of Sik.
	calculateSik();

	if (Ite == 1)
	{
		BUB = UB;
		BLB = OV;

		//Update the information for the best iteration.
		best_Lik = Lik;
		best_Zik = Zik;
		best_Yi = Yi;
		best_Denik = Denik;
		best_OV = OV;
		best_OV_Sub_Z = OV_Sub_Z;
		best_OV_Sub_Y = OV_Sub_Y;
		best_sumLik = sumLik;
		best_SumDi2 = SumDi2;

		alpha = alpha2;
	}
	else
	{
		if (UB < BUB)
			BUB = UB;
		if (OV > BLB - tolerance)
		{
			BLB = OV;

			//Update the information for the best iteration.
			best_Lik = Lik;
			best_Zik = Zik;
			best_Yi = Yi;
			best_Denik = Denik;
			best_OV = OV;
			best_OV_Sub_Z = OV_Sub_Z;
			best_OV_Sub_Y = OV_Sub_Y;
			best_sumLik = sumLik;
			best_SumDi2 = SumDi2;
		}
	}

	//Count the number of iterations that the Lower Bound doesn't make any improvement.
	if (OV > BLB - tolerance)
		IteNoImp = 0;
	else
		IteNoImp++;
	
	//Reduce the value of alpha.
	if (alpha > 2)
	{
		if (IteNoImp == limIteNoImp) {
			alpha = alpha * factorRedAlpha;
			IteNoImp = 0;
			//cout << "Ite: " << Ite << "\t alpha: " << alpha << endl;
			//cin.get();
		}
	}
	else
	{
		alpha = 1;
	}


	if (Ite == Ite_general_nodes)
	{
		//On the last iteration call the rutine to select the Zik to branch on the node.
		select_Zik_4();

		//Copy the Multipliers of the best iteration to the node information.
		(*this_active_node).final_Lik = best_Lik;
	}

	//Calculate the stepsize for the update of the Lagrange Multipliers in the 'n' iteration.
	Stepsize_tn = 0;
	if (Stepsize_den > 0)
		Stepsize_tn = alpha * (BUB - OV) / Stepsize_den;
	else
		Stepsize_tn = 0;

	//Update the values of the multipliers Lik.
	updateLik();

}

void subgradient_Root()
{
	clock_t start_root_select_time = clock();

	//Compute the values of Sik.
	calculateSik();
	select_root_time += (double)(clock() - start_root_select_time) / CLOCKS_PER_SEC;

	clock_t start_root_others_time = clock();
	if (Ite == 1)
	{
		double temp_UB=myopic(n,p,dist);
		if (temp_UB < UB - tolerance)
		{
			BUB = temp_UB;
			global_BUB = temp_UB;
			incumbent_sol = temp_UB;
			incumbent_time = 0;
			cout << "New BUB (Heu)= " << BUB << "\t Time: " << incumbent_time << endl;
		}
		else
		{
			BUB = UB;
			BUB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			cout << "New BUB= " << BUB << "\t Time: " << BUB_time << endl;
		}
		
		//BUB = UB;
		//incumbent_sol = UB;
		BLB = OV;
		
		//Saves the information for the best iteration.
		best_Lik = Lik;
		best_Zik = Zik;
		best_Yi = Yi;
		best_Denik = Denik;
		best_OV = OV;
		best_OV_Sub_Z = OV_Sub_Z;
		best_OV_Sub_Y = OV_Sub_Y;
		best_sumLik = sumLik;
		best_SumDi2 = SumDi2;
	}
	else
	{
		if (UB < BUB)
		{
			BUB = UB;
			BUB_time = (double)(clock() - clock_initial_BB) / CLOCKS_PER_SEC;
			cout << "New BUB= " << global_BUB << "\t Time: " << BUB_time << endl;
		}
		if (OV > BLB - tolerance)
		{
			BLB = OV;

			//Update the information for the best iteration.
			best_Lik = Lik;
			best_Zik = Zik;
			best_Yi = Yi;
			best_Denik = Denik;
			best_OV = OV;
			best_OV_Sub_Z = OV_Sub_Z;
			best_OV_Sub_Y = OV_Sub_Y;
			best_sumLik = sumLik;
			best_SumDi2 = SumDi2;
		}
	}

	//Count the number of iterations that the Lower Bound doesn't make any improvement.
	if (OV > BLB - tolerance)
		IteNoImp = 0;
	else
		IteNoImp++;
	
	//Reduce the value of alpha.
	if (alpha_root > 2)
	{
		if (IteNoImp == limIteNoImp) {
			alpha_root = alpha_root * factorRedAlpha;
			IteNoImp = 0;
			cout << "Ite: " << Ite << "\t alpha: " << alpha_root << endl;
		}
		
	}
	else
	{
		alpha_root = 1;
	}

	others_root_time += (double)(clock() - start_root_others_time) / CLOCKS_PER_SEC;
	if (Ite == Ite_Root)
	{
		//On the last iteration call the rutine to select the Zik to branch on the node.
		select_Zik_4();

		//Copy the Multipliers of the best iteration to the node information.
		(*this_active_node).final_Lik = best_Lik;
	}

	//Calculate the stepsize for the update of the Lagrange Multipliers in the 'n' iteration.
	Stepsize_tn = 0;
	if (Stepsize_den > 0)
		Stepsize_tn = alpha_root * (BUB - OV) / Stepsize_den;
	else
		Stepsize_tn = 0;

	//Update the values of the multipliers Lik.
	updateLik();
}
