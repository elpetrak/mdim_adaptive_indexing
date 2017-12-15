#include "../Framework/queries.h"
#include "../Framework/kdtree.h"
#include "../Framework/createdot.h"

int main(int argc, char* argv[])
{
    	int 			**data;
	int			**copy_data; 
    	int 			j, q;
	long			i;
    	int 			**queries;
	long 			*map;
	long 			ntuples 	= atoi(argv[2]);
	float 			selectivity 	= atof(argv[3]);
	int			nqueries	= atoi(argv[4]);
	int 			ndimensions	= atoi(argv[5]);
	kdnodeptr 		kdroot   	= NULL;
	FILE *fp;
	fp=fopen("treenodes.dot", "w");

	/*check command line arguments*/
	if(argc != 6)
	{
		fprintf(stderr, "usage: %s incorrect number of arguments.\n", argv[0]);
		fprintf(stderr, "Give the name of the workload as 1st argument, the number of tuples as 2nd argument, the selectivity as 3rd argument, the number of queries as 4th argument and the number of dimensions as 5th argument.\n For the name of the workload, choose one of the following options:\n");
		fprintf(stderr, "-randomDsameS\n-randomDrandomS\n-randomDSBsameS\n-skewedDsameS\n-sequentialDsameS\n-periodicalDsameS\n-zoominDdifferentS\n");
		fprintf(stderr, "For the number of tuples give an integer greater than 0.\n");
		fprintf(stderr, "For the selectivity give a decimal number between 0 and 1, e.g., 0.1 .\n");
		fprintf(stderr, "For the number of queries give an integer greater than 0.\n");
		fprintf(stderr, "For the number of dimensions give an integer greater than 1.\n");
		exit(EXIT_FAILURE);
	}

	if(!(selectivity > 0 && selectivity <= 1))
	{
		fprintf(stderr, "Wrong selectivity. Selectivity must have a value between 0 and 1.\n");
		exit(EXIT_FAILURE);
	}

	if(ntuples <= 0 || nqueries <= 0)
	{
		fprintf(stderr, "The number of tuples or queries cannot be 0 or negative.\n");
		exit(EXIT_FAILURE);
	}

	/* allocate memory to array of data */
	if((data = (int**)malloc(ndimensions * sizeof(int*))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for **data.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ndimensions; i++)
	{
		data[i] = (int*)malloc(ntuples * sizeof(int));
		if(data[i] == NULL)
		{
			fprintf(stderr, "main: cannot allocate memory for data[%d]\n", i);
			exit(EXIT_FAILURE);
		}
	}

	/* allocate memory to copy of array of data */
	if((copy_data = (int**)malloc(ndimensions * sizeof(int*))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for **copy_d.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ndimensions; i++)
	{
		copy_data[i] = (int*)malloc(ntuples * sizeof(int));
		if(copy_data[i] == NULL)
		{
			fprintf(stderr, "main: cannot allocate memory for copy_data[%d]\n", i);
			exit(EXIT_FAILURE);
		}
	}

	/*allocate memory to array of rid (payload)*/
	if((map = (long*)malloc(ntuples * sizeof(long))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for *payload.\n");
		exit(EXIT_FAILURE);
	}

	/* generate data */
	for(i = 0; i < ntuples; i++)
	{
 	   	for(int j = 0; j < ndimensions; j++)
			data[j][i] = rand() % (int)ntuples;
	}

	/* allocate memory to array of queries/predicates */
	if((queries = (int**)malloc(nqueries * sizeof(int*))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for **queries.\n");
		exit(EXIT_FAILURE);
	}

	for(q = 0; q < nqueries; q++)
	{
		queries[q] = (int*)malloc(ndimensions * 2 * sizeof(int));
		if(queries[q] == NULL)
		{
			fprintf(stderr, "main: cannot allocate memory for queries[%d]\n", q);
			exit(EXIT_FAILURE);
		}
	}

	/* create the query predicates */
	create_queries(argv[1], queries, ntuples, selectivity, nqueries, ndimensions);

	/* answer queries */
	for(q = 0; q < nqueries; q++)
	{
/*-------------------------------------------------------------------------------------------------------------------------*/
		/* answer query with KDTREE CRACKING */
/*-------------------------------------------------------------------------------------------------------------------------*/

		/* insert two points in the kd-tree; one with the min query boundaries and one with the max query boundaries */
		int minquery[ndimensions];
		int maxquery[ndimensions];
		long kdstart = 0, kdend = 0;
		for(i = 0, j = 0; i < ndimensions * 2; i+=2, j++)
		{
			minquery[j] = queries[q][i];
			maxquery[j] = queries[q][i+1];
		}
		/* insert minquery */
		if(kdroot == NULL)
		{
			kdroot = insertKDTree(kdroot, minquery, data, copy_data, map, ntuples, ndimensions, &kdstart);
		}
		else
		{
			insertKDTree(kdroot, minquery, data, copy_data, map, ntuples, ndimensions, &kdstart);
		}
		/* insert maxquery */
		insertKDTree(kdroot, maxquery, data, copy_data, map, ntuples, ndimensions, &kdend);
	}

	bst_print_dot(kdroot, fp, ndimensions);
	fclose(fp);
    	return 0;
}

