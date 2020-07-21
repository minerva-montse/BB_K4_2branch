#pragma once

#include <iostream>

using namespace std;

void solve_Root()
{
	//Restart the Values
	sumLik = 0;
	OV_Sub_Z = 0;
	OV_Sub_Y = 0;

	//Solve Subproblem 1. Find the values for all Zik.
	for (int i = 0; i < n; i++)
	{
		for (int k = 2; k < Gi[i]; k++)
		{
			//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
			double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
			if (temp < Lik[i][k - 2] - tolerance)
			{
				Zik[i][k - 2] = 1;
				OV_Sub_Z += temp - Lik[i][k - 2];
			}
			else {
				Zik[i][k - 2] = 0;
			}
			//The sum of ALL Lagrangians is calculated.
			sumLik += Lik[i][k - 2];
		}
	}

	//Solve Subproblem 2. Find the values for all Yi.

	//With vector
	vector <double> sum_Lik_j;
	vector <double> CoefAi;
	CoefAi.resize(n);
	std::fill(CoefAi.begin(), CoefAi.end(), 0);

	for (int i = 0; i < n; i++)
	{
		sum_Lik_j.resize(Gi[i] - k_radious + 1);
		for (int j = Gi[i] - k_radious; j >= 0; j--)
		{
			if (j < Gi[i] - k_radious)
			{
				sum_Lik_j[j] = sum_Lik_j[j + 1] + Lik[i][j];
			}
			else
			{
				sum_Lik_j[j] += Lik[i][j];
			}
		}


		CoefAi[i] += Dik3[i][1][0];
		CoefAi[i] += sum_Lik_j[0];
		for (size_t k = 2; k < Gi[i]; k++)
		{
			double temp_val = 0;
			for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
			{
				CoefAi[(int)Dik3[i][k - 1][j]] += sum_Lik_j[k - 2];
			}
		}
	}

	vector <A_i> Ai;
	Ai.resize(n);

	for (int i = 0; i < n; i++)
	{
		Ai[i].t = i;
		Ai[i].Coef_Yt = CoefAi[i];
	}

	//Sort the vector Ai in decresing order
	sort(Ai.begin(), Ai.end(), sort_A_i);

	//Reset the vector Y to 0's and just add 1's.
	std::fill(Yi.begin(), Yi.end(), 0);

	for (int i = 0; i < p; i++)
	{
		A_i arrayB = Ai[i];
		Yi[arrayB.t] = 1;
		OV_Sub_Y += arrayB.Coef_Yt;//Objective Value of subproblem 2. Problem in variable Y.
	}

	//Objective Value for current iterarion.
	OV = OV_Sub_Z + SumDi2 + sumLik - OV_Sub_Y;

	//Primal solution for current solution of Yi.
	UB = Primal(Yi, n, p, dist);
}

void solve_subpro_Z()
{
	//Restart the variables.
	OV_Sub_Z = 0;
	sumLik = 0;

	//Solve Subproblem 1. Find the values for all Zik.
	for (int i = 0; i < n; i++)
	{
		for (int k = 2; k < Gi[i]; k++)
		{
			//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
			double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
			if (temp < Lik[i][k - 2] - tolerance)
			{
				Zik[i][k - 2] = 1;
				OV_Sub_Z += temp - Lik[i][k - 2];
			}
			else {
				Zik[i][k - 2] = 0;
			}
			//The sum of ALL Lagrangians is calculated.
			sumLik += Lik[i][k - 2];
		}
	}
	

	int branch_i;
	int branch_k;

	//When we need to set the variable Zik=1
	for (int i = 0; i < (*this_active_node).branch_zi_1.size(); i++)
	{
		branch_i = (*this_active_node).branch_zi_1[i];
		branch_k = (*this_active_node).branch_zk_1[i];
		for (size_t k = 2; k < branch_k+1; k++)
		{
			if (Zik[branch_i][k - 2] == 0)
			{
				Zik[branch_i][k - 2] = 1;
				OV_Sub_Z += Dik3[branch_i][k][0] - Dik3[branch_i][k - 1][0] - Lik[branch_i][k - 2];

			}

		}
		
		//if (Zik[branch_i][branch_k - 2] == 0)
		//{
		//	Zik[branch_i][branch_k - 2] = 1;
		//	OV_Sub_Z += Dik3[branch_i][branch_k][0] - Dik3[branch_i][branch_k - 1][0] - Lik[branch_i][branch_k - 2];
		//}
	}

	//When we need to set the variable Zik=0
	for (int i = 0; i < (*this_active_node).branch_zi_0.size(); i++)
	{
		branch_i = (*this_active_node).branch_zi_0[i];
		branch_k = (*this_active_node).branch_zk_0[i];
		for (int k = branch_k; k < (Gi[branch_i]); k++)
		{
			if (Zik[branch_i][k - 2] == 1)
			{
				Zik[branch_i][k - 2] = 0;
				OV_Sub_Z -= Dik3[branch_i][k][0] - Dik3[branch_i][k - 1][0] - Lik[branch_i][k - 2];
			}
		}
	}


}

void solve_subpro_Y()
{
	//Solve Subproblem 2. Find the values for all Yi.
	//Restart the variables.
	OV_Sub_Y = 0;

	//Reset the vector Y to 0's and just add 1's.
	std::fill(Yi.begin(), Yi.end(), 0);

	//With vector
	vector <double> sum_Lik_j;
	vector <double> CoefAi;
	CoefAi.resize(n);
	std::fill(CoefAi.begin(), CoefAi.end(), 0);

	for (int i = 0; i < n; i++)
	{
		sum_Lik_j.resize(Gi[i] - k_radious + 1);
		std::fill(sum_Lik_j.begin(), sum_Lik_j.end(), 0);
		for (int j = Gi[i] - k_radious; j >= 0; j--)
		{
			if (j < Gi[i] - k_radious)
			{
				sum_Lik_j[j] = sum_Lik_j[j + 1] + Lik[i][j];
			}
			else
			{
				sum_Lik_j[j] += Lik[i][j];
			}
		}


		CoefAi[i] += Dik3[i][1][0];
		CoefAi[i] += sum_Lik_j[0];
		for (size_t k = 2; k < Gi[i]; k++)
		{
			double temp_val = 0;
			for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
			{
				CoefAi[(int)Dik3[i][k - 1][j]] += sum_Lik_j[k - 2];
			}
		}
	}

	vector <A_i> Ai;
	Ai.resize(n);
	for (int i = 0; i < n; i++)
	{
		Ai[i].t = i;
		Ai[i].Coef_Yt = CoefAi[i];
	}


	/*for the case where set y=0. We want to delete from array A, the Y's to be set to 0.
	Because we will select the largest coefficient of A, and we dont want to select these Y, we set their corresponding coefficient to 0.*/
	vector <int> branch_Y_0 = (*this_active_node).branch_y_0;
	for (int i = 0; i < branch_Y_0.size(); i++)
	{
		Ai[branch_Y_0[i]].Coef_Yt = 0;
	}

	vector <int> branch_Y_1 = (*this_active_node).branch_y_1;
	int fixed_p_1=0;
	for (int i = 0; i < branch_Y_1.size(); i++)
	{
		A_i arrayB = Ai[branch_Y_1[i]];
		Yi[arrayB.t] = 1;
		OV_Sub_Y += arrayB.Coef_Yt;//Solution of subproblem 2. Problem in variable Y.

		Ai[branch_Y_1[i]].Coef_Yt = 0;//so it is not selected again.
		++fixed_p_1;
	}

	//Sort the vector Ai in decresing order
	sort(Ai.begin(), Ai.end(), sort_A_i);


	//Set the corresponding Y of the p largest values of (Ai.t) to 1.
	for (int i = 0; i < p- fixed_p_1; i++)
	{
		A_i arrayB = Ai[i];
		Yi[arrayB.t] = 1;
		OV_Sub_Y += arrayB.Coef_Yt;//Solution of subproblem 2. Problem in variable Y.
	}
}


double solve_subpro_Z_select(int Zi, int Zk)
{
	//Restart the variables.
	OV_Sub_Z = 0;
	sumLik = 0;

	Lik = best_Lik;
	//Solve Subproblem 1. Find the values for all Zik.
	for (int i = 0; i < n; i++)
	{
		for (int k = 2; k < Gi[i]; k++)
		{
			//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
			double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
			if (temp < Lik[i][k - 2] - tolerance)
			{
				Zik[i][k - 2] = 1;
				OV_Sub_Z += temp - Lik[i][k - 2];
			}
			else {
				Zik[i][k - 2] = 0;
			}
			//The sum of ALL Lagrangians is calculated.
			sumLik += Lik[i][k - 2];
			//cout << "z[" << i << "][" << k << "]= " << Zik[i][k - 2] << endl;
		}
		//cin.get();
	}


	int branch_i;
	int branch_k;

	//When we need to set the variable Zik=1
	for (int i = 0; i < (*this_active_node).branch_zi_1.size(); i++)
	{
		branch_i = (*this_active_node).branch_zi_1[i];
		branch_k = (*this_active_node).branch_zk_1[i];

		for (size_t k = 2; k < branch_k + 1; k++)
		{
			if (Zik[branch_i][k - 2] == 0)
			{
				Zik[branch_i][k - 2] = 1;
				OV_Sub_Z += Dik3[branch_i][k][0] - Dik3[branch_i][k - 1][0] - Lik[branch_i][k - 2];

			}

		}
	}

	//Adding the candidate to Branch.
	branch_i = Zi;
	branch_k = Zk;
	if (Zik[branch_i][branch_k - 2] == 0)
	{
		Zik[branch_i][branch_k - 2] = 1;
		OV_Sub_Z += Dik3[branch_i][branch_k][0] - Dik3[branch_i][branch_k - 1][0] - Lik[branch_i][branch_k - 2];
	}
	
	return OV_Sub_Z;
}

//double solve_subpro_Z_Lik_0()
//{
//	//Restart the variables.
//	OV_Sub_Z = 0;
//	sumLik = 0;
//
//	Lik = best_Lik;
//	//Solve Subproblem 1. Find the values for all Zik.
//	for (int i = 0; i < n; i++)
//	{
//		for (int k = 2; k < Gi[i]; k++)
//		{
//			//Set value for Zik=1 when (D[i][k] -D[i][k-1] < L[i][k]).
//			double temp = Dik3[i][k][0] - Dik3[i][k - 1][0];
//			if (temp < Lik[i][k - 2] - tolerance)
//			{
//				Zik[i][k - 2] = 1;
//				OV_Sub_Z += temp - Lik[i][k - 2];
//			}
//			else {
//				Zik[i][k - 2] = 0;
//			}
//			//The sum of ALL Lagrangians is calculated.
//			sumLik += Lik[i][k - 2];
//			//cout << "z[" << i << "][" << k << "]= " << Zik[i][k - 2] << endl;
//		}
//		//cin.get();
//	}
//
//
//	int branch_i;
//	int branch_k;
//
//	//When we need to set the variable Zik=1
//	for (int i = 0; i < (*this_active_node).branch_zi_1.size(); i++)
//	{
//		branch_i = (*this_active_node).branch_zi_1[i];
//		branch_k = (*this_active_node).branch_zk_1[i];
//
//		for (size_t k = 2; k < branch_k + 1; k++)
//		{
//			if (Zik[branch_i][k - 2] == 0)
//			{
//				Zik[branch_i][k - 2] = 1;
//				OV_Sub_Z += Dik3[branch_i][k][0] - Dik3[branch_i][k - 1][0] - Lik[branch_i][k - 2];
//
//			}
//
//		}
//	}
//
//	return OV_Sub_Z;
//}

double solve_subpro_Y_Lik_0()
{
	//Solve Subproblem 2. Find the values for all Yi.
	//Restart the variables.
	OV_Sub_Y = 0;

	//Reset the vector Y to 0's and just add 1's.
	std::fill(Yi.begin(), Yi.end(), 0);

	//With vector
	vector <double> sum_Lik_j;
	vector <double> CoefAi;
	CoefAi.resize(n);
	std::fill(CoefAi.begin(), CoefAi.end(), 0);

	for (int i = 0; i < n; i++)
	{
		sum_Lik_j.resize(Gi[i] - k_radious + 1);
		for (int j = Gi[i] - k_radious; j >= 0; j--)
		{
			if (j < Gi[i] - k_radious)
			{
				sum_Lik_j[j] = sum_Lik_j[j + 1] + Lik[i][j];
			}
			else
			{
				sum_Lik_j[j] += Lik[i][j];
			}
		}


		CoefAi[i] += Dik3[i][1][0];
		CoefAi[i] += sum_Lik_j[0];
		for (size_t k = 2; k < Gi[i]; k++)
		{
			double temp_val = 0;
			for (size_t j = 1; j < Dik3[i][k - 1].size(); j++)
			{
				CoefAi[(int)Dik3[i][k - 1][j]] += sum_Lik_j[k - 2];
			}
		}
	}

	vector <A_i> Ai;
	Ai.resize(n);
	for (int i = 0; i < n; i++)
	{
		Ai[i].t = i;
		Ai[i].Coef_Yt = CoefAi[i];
	}


	/*for the case where set y=0. We want to delete from array A, the Y's to be set to 0.
	Because we will select the largest coefficient of A, and we dont want to select these Y, we set their corresponding coefficient to 0.*/
	vector <int> branch_Y_0 = (*this_active_node).branch_y_0;
	for (int i = 0; i < branch_Y_0.size(); i++)
	{
		Ai[branch_Y_0[i]].Coef_Yt = 0;
	}

	vector <int> branch_Y_1 = (*this_active_node).branch_y_1;
	int fixed_p_1 = 0;
	for (int i = 0; i < branch_Y_1.size(); i++)
	{
		A_i arrayB = Ai[branch_Y_1[i]];
		Yi[arrayB.t] = 1;
		OV_Sub_Y += arrayB.Coef_Yt;//Solution of subproblem 2. Problem in variable Y.

		Ai[branch_Y_1[i]].Coef_Yt = 0;//so it is not selected again.
		++fixed_p_1;
	}

	//Sort the vector Ai in decresing order
	sort(Ai.begin(), Ai.end(), sort_A_i);


	//Set the corresponding Y of the p largest values of (Ai.t) to 1.
	for (int i = 0; i < p - fixed_p_1; i++)
	{
		A_i arrayB = Ai[i];
		Yi[arrayB.t] = 1;
		OV_Sub_Y += arrayB.Coef_Yt;//Solution of subproblem 2. Problem in variable Y.
	}

	return OV_Sub_Y;
}
