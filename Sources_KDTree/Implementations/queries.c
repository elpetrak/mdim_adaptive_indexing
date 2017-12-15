#include "../Framework/queries.h"

static void same_selectivity_random(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries 	= ndimensions * 2;
	int	step 		= (int)(ntuples * pow(selectivity, 1.0 / ndimensions));

	for(int i = 0; i < nqueries; i++)
	{
		for(int j = 0; j < nboundaries; j += 2)
		{
			queries[i][j]     = rand() % (ntuples - step -1);
			queries[i][j + 1] = queries[i][j] + step;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
}

static void random_selectivity_random(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries	= ndimensions * 2;

	for(int i = 0; i < nqueries; i++)
	{
		int	temp;

		for(int j = 0; j < nboundaries; j += 2)
		{
			queries[i][j]     = rand() % ntuples;
			queries[i][j + 1] = rand() % ntuples;
			if(queries[i][j + 1] < queries[i][j])  //be sure that hgh >= low
			{
				temp              = queries[i][j];
				queries[i][j]     = queries[i][j + 1];
				queries[i][j + 1] = temp;
			}
			/*try other range predicates if lowX == hghX or lowY == hghY*/
			if(queries[i][j] == queries[i][j + 1])
			{
				i--;
				continue;
			}
		}
		#ifdef PRINTQUERIES
		for(int j = 0; j < nboundaries; j += 2)
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
		printf("\n");
		#endif	
	}
}

static void same_selectivity_random_spacially_biased(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries 	= ndimensions * 2;
	int	step 		= (int)(ntuples * pow(selectivity, 1.0 / ndimensions));

	for(int i = 0; i < nqueries; i++)
	{
		for(int j = 0; j < nboundaries; j += 2)
		{
			if((i%2) == 0)
				queries[i][j] = rand() % (ntuples/4);
			else
				queries[i][j] = rand() % (ntuples - step - 1);
			queries[i][j + 1] = queries[i][j] + step;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
}

static void same_selectivity_skewed(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries 	= ndimensions * 2;
	int	step 		= (int)(ntuples * pow(selectivity, 1.0 / ndimensions));

	for(int i = 0; i < nqueries; i++)
	{
		for(int j = 0; j < nboundaries; j += 2)
		{
			if(i <= (nqueries * 0.8))
				queries[i][j] = rand() % (ntuples/2);
			else
				queries[i][j] = (rand() % (ntuples/2)) + (ntuples/2);
			queries[i][j + 1] = queries[i][j] + step;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
}

static void same_selectivity_sequential(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries	= ndimensions * 2;
	int	*min;
	int	step 		= ntuples / nqueries;

	if((min = (int*)malloc(ndimensions * sizeof(int))) == NULL )
	{
		fprintf(stderr, "same_selectivity_sequential: cannot allocate memory for min.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ndimensions; i++)
		min[i] = 0;

	for(int i = 0; i < nqueries; i++)
	{
		for(int j = 0; j < nboundaries; j += 2)
		{
			queries[i][j] = min[j/2];
			queries[i][j + 1] = queries[i][j] + step;
			min[j/2] = queries[i][j + 1] + 1;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
	free(min);
}

static void same_selectivity_periodical(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries 	= ndimensions * 2;
	int 	*min;
	int 	step 		= (ntuples / nqueries) * 10;

	if((min = (int*)malloc(ndimensions * sizeof(int))) == NULL )
	{
		fprintf(stderr, "same_selectivity_periodical: cannot allocate memory for min.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < ndimensions; i++)
		min[i] = 0;

	for(int i = 0; i < nqueries; i++)
	{
		if((i % (nqueries/10)) == 0)
		{
			for(int k = 0; k < ndimensions; k++)
				min[k] = 0;
		}
		for(int j = 0; j < nboundaries; j += 2)
		{
			queries[i][j] = min[j/2];
			queries[i][j + 1] = queries[i][j] + step;
			min[j/2] = queries[i][j + 1] + 1;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
	free(min);
}

static void different_selectivity_zoomin(int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	int	nboundaries 	= ndimensions * 2;
	int 	*min;
	int 	*max;
	int 	jump 		= ntuples / (nqueries * 2);

	if((min = (int*)malloc(ndimensions * sizeof(int))) == NULL )
	{
		fprintf(stderr, "different_selectivity_zoomin: cannot allocate memory for min.\n");
		exit(EXIT_FAILURE);
	}
	if((max = (int*)malloc(ndimensions * sizeof(int))) == NULL )
	{
		fprintf(stderr, "different_selectivity_zoomin: cannot allocate memory for max.\n");
		exit(EXIT_FAILURE);
	}


	for(int i = 0; i < ndimensions; i++)
	{
		min[i] = 0;
		max[i] = ntuples;
	}

	/*Query 0*/
	for(int j = 0; j < nboundaries; j += 2)
	{
		queries[0][j]     = min[j / 2] + jump;
		queries[0][j + 1] = max[j / 2] - jump;
		#ifdef PRINTQUERIES
		printf("%d\t%d\t", queries[0][j],queries[0][j + 1]);
		#endif
	}
	#ifdef PRINTQUERIES
	printf("\n");
	#endif

	for(int i = 1; i < nqueries; i++)
	{
		for(int j = 0; j < nboundaries; j += 2)
		{
			queries[i][j]     = queries[i-1][j] + jump;
			queries[i][j + 1] = queries[i-1][j + 1] - jump;
			#ifdef PRINTQUERIES
			printf("%d\t%d\t", queries[i][j],queries[i][j + 1]);
			#endif
		}
		#ifdef PRINTQUERIES
		printf("\n");
		#endif
	}
	free(min);
	free(max);
}

void create_queries(const char *distribution, int **queries, int ntuples, float selectivity, int nqueries, int ndimensions)
{
	if(strcmp("randomDsameS", distribution) == 0)
		same_selectivity_random(queries, ntuples, selectivity, nqueries, ndimensions);
	else if(strcmp("randomDrandomS", distribution) == 0)
		random_selectivity_random(queries, ntuples, 0, nqueries, ndimensions);
	else if(strcmp("randomDSBsameS", distribution) == 0)
		same_selectivity_random_spacially_biased(queries, ntuples, selectivity, nqueries, ndimensions);
	else if(strcmp("skewedDsameS", distribution) == 0)
		same_selectivity_skewed(queries, ntuples, selectivity, nqueries, ndimensions);
	else if(strcmp("sequentialDsameS", distribution) == 0)
		same_selectivity_sequential(queries, ntuples, 0, nqueries, ndimensions);
	else if(strcmp("periodicalDsameS", distribution) == 0)
		same_selectivity_periodical(queries, ntuples, 0, nqueries, ndimensions);
	else if(strcmp("zoominDdifferentS", distribution) == 0)
		different_selectivity_zoomin(queries, ntuples, 0, nqueries, ndimensions);
	else
	{
		fprintf(stderr, "Wrong distribution name.\n");
		exit(EXIT_FAILURE);
	}
}
