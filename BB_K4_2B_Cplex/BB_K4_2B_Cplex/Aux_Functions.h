#pragma once

#ifndef AUX_FUNCTIONS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define AUX_FUNCTIONS_H 

//Check input on the console.
bool Read_Parameters(int argc, char *argv[])
{
	int data_format = 0;
	int p = 0;
	if (argc < 2) return false;

	char *file_name;
	file_name = argv[1];
	if (strlen(file_name) >= 5)
	{
		printf("The file is:%s\n", file_name);
		if (strcmp(&file_name[strlen(file_name) - 4], ".dat") == 0)
			data_format = 1;
		else
		{
			printf("ERROR: data extension not recognized.\n");
			printf("Valid extensions: .dat\n");
			return false;
		}
	}
	int arg = 2;
	//bool no_p = true;
	while (arg < argc)
	{
		if (strcmp(arg[argv], "-p") == 0)
		{
			++arg;
			if (arg == argc) return false;
			p = atoi(argv[arg]);
			if (p <= 0)
			{
				printf("ERROR: p must be strictly positive.\n");
				return false;
			}
			//no_p = false;
			++arg;
			continue;
		}

		if (strcmp(arg[argv], "-root") == 0)
		{
			++arg;
			if (arg == argc) return false;
			Ite_Root = atoi(argv[arg]);
			if (alpha_root <= 0)
			{
				printf("ERROR: Ite_Root must be strictly positive.\n");
				return false;
			}
			++arg;
			continue;
		}

		if (strcmp(arg[argv], "-ite") == 0)
		{
			++arg;
			if (arg == argc) return false;
			Ite_general_nodes = atoi(argv[arg]);
			if (alpha_root <= 0)
			{
				printf("ERROR: Ite_general_nodes must be strictly positive.\n");
				return false;
			}
			++arg;
			continue;
		}

		/*if (strcmp(arg[argv], "-time") == 0)
		{
		++arg;
		if (arg == argc) return false;
		time_limit = atof(argv[arg]);
		++arg;
		continue;
		}

		if (strcmp(arg[argv], "-r") == 0)
		{
		++arg;
		if (arg == argc) return false;
		int roundedvalue = atoi(argv[arg]);
		if (roundedvalue == 1) roundedc = true;
		++arg;
		continue;
		}

		if (strcmp(arg[argv], "-b") == 0)
		{
		++arg;
		if (arg == argc) return false;
		incumbent_value = atof(argv[arg]);
		++arg;
		continue;
		}

		if (strcmp(arg[argv], "-h") == 0)
		{
		RW = true;
		++arg;
		if (arg == argc) return false;
		RWiter = atoi(argv[arg]);
		++arg;
		continue;
		}

		if (strcmp(arg[argv], "-s") == 0)
		{
		save_solution_info = true;
		++arg;
		if (arg == argc) return false;
		solution_file_name = argv[arg];
		++arg;
		continue;
		}

		if (strcmp(arg[argv], "-d") == 0)
		{
		++arg;
		if (arg == argc) return false;
		int d = atoi(argv[arg]);
		if (d == 1) allD = true;
		else if (d != 0) return false;
		++arg;
		continue;
		}*/

		return false;
	}

	//if (no_p)
	//{
	//	printf("ERROR: No value for p has been given.\n");
	//	return false;
	//}
	return true;
}

//Structures for the Branch-and-Bound.
node_i *Create_n_i(void)
{
	return new node_i;
}

active_n *Create_a_n(void)
{
	return new active_n;
}



#endif