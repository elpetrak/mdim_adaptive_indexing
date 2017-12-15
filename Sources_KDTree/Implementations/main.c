#include "../Framework/sideways_cracking.h"
#include "../Framework/scan.h"
#include "../Framework/queries.h"
#include "../Framework/kdtree.h"
#include "../Framework/interface.h"

int main(int argc, char* argv[])
{
    	int 			**data;
	int			**copy_data; 
    	int 			j, q;
	long			i;
    	int 			**queries;
	long 			*map;
    	cracker_map_t 		*CrackerMap;
    	int 			first_query 	= 1;
	struct timeval 		t0, t1;
	long			t_swcrack, t_scan, t_kdcrack;
	long 			ntuples 	= atoi(argv[2]);
	float 			selectivity 	= atof(argv[3]);
	int			nqueries	= atoi(argv[4]);
	int 			ndimensions	= atoi(argv[5]);
	int			nmaps		= 0;
	kdnodeptr 		kdroot   	= NULL;

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

	/* allocate memory to array of cracker_maps */
	nmaps = ndimensions - 1;
	if((CrackerMap = (cracker_map_t*)malloc(nmaps * sizeof(cracker_map_t))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for *CrackerMap.\n");
		exit(EXIT_FAILURE);
	}

	/* answer queries */
	for(q = 0; q < nqueries; q++)
	{
		long swcrackQualifyingTuples = 0, scanQualifyingTuples = 0, kdcrackQualifyingTuples = 0;

/*-------------------------------------------------------------------------------------------------------------------------*/
		/* answer query with SIDEWAYS CRACKING */
/*-------------------------------------------------------------------------------------------------------------------------*/

		gettimeofday (&t0, NULL);
		unsigned long int *resbv;
        	int bvs;

		#ifdef DEBUG
		printf("Query %d: lowcrack=%d, hghcrack=%d\n",q,queries[q][0],queries[q][1]);
		#endif

    		/* first query creates cracker maps by cracking data out of place */
    		if(first_query == 1)
    		{
			/* crack cracker maps and insert in avl tree */
			for(i = 0; i < nmaps; i++)
			{
				bvs = crack_first(data[0], data[i+1], &CrackerMap[i], queries[q][0], queries[q][1], queries[q][2*i+2], queries[q][2*i+3], ntuples);
		
			}
			first_query = 0;
		}
		else /* subsequent queries reorganize cracker maps in place */
		{
			/* crack cracker maps and insert in avl tree */
			for(i = 0; i < nmaps; i++)
			{
				bvs = swcrack(&CrackerMap[i], queries[q][0], queries[q][1], queries[q][2*i+2], queries[q][2*i+3]);
			}
		}
		/* allocate and initiallize res_bv*/	
		resbv = (unsigned long int*)malloc(bvs * sizeof(unsigned long int));
		for(j = 0; j < bvs; j++)
			resbv[j] = CrackerMap[0].bv[j];

		/* populate result bv with a conjunction between all bvs */
		for(i = 1; i < nmaps; i++)
		{
			for(j = 0; j < bvs; j++)
				resbv[j] &= CrackerMap[i].bv[j];
		}

		/* count  cracking qualifying tuples */
		for(j = 0; j < bvs; j++)
		{
			for(i = 0; i < 64; i++)
			{
				swcrackQualifyingTuples += ((resbv[j]>>i)&1);
			}	
		}
  	     	first_query = 0;
		free(resbv);
    		gettimeofday (&t1, NULL);
		t_swcrack = ((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec;

		#ifdef DEBUG
		/* print AVL tree of each cracker map */
		for(i = 0; i < nmaps; i++)
		{
			printf("PRE-order tree 1:\n");
    			preorder(CrackerMap[i].avl_index);
 	   		printf("\n");
		}
		#endif
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
		gettimeofday (&t0, NULL);
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
		/* count qualifying tuples */
		kdcrackQualifyingTuples = scan_part(copy_data, queries[q], kdstart, kdend, ndimensions);
		gettimeofday (&t1, NULL);
		t_kdcrack = ((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec;

/*-------------------------------------------------------------------------------------------------------------------------*/
		/* answer query with SCAN */
/*-------------------------------------------------------------------------------------------------------------------------*/

		gettimeofday (&t0, NULL);
		scanQualifyingTuples = scan(data, queries[q], ntuples, ndimensions);
		gettimeofday (&t1, NULL);
		t_scan = ((t1.tv_sec - t0.tv_sec)*1000000L + t1.tv_usec) - t0.tv_usec;

		printf("Query\t%d\t%ld\t%ld\t%ld\n", q+1, t_scan, t_swcrack, t_kdcrack);
		if(swcrackQualifyingTuples != scanQualifyingTuples)
		{
			fprintf(stderr, "Query %d: number of qualifying tuples differs; scan: %ld, swcrack: %ld\n", q, scanQualifyingTuples, swcrackQualifyingTuples);
			exit(EXIT_FAILURE);
		}
		if(kdcrackQualifyingTuples != scanQualifyingTuples)
		{
			fprintf(stderr, "Query %d: number of qualifying tuples differs; scan: %ld, kdcrack: %ld\n", q, scanQualifyingTuples, kdcrackQualifyingTuples);
			exit(EXIT_FAILURE);
		}
	}

    	return 0;
}

