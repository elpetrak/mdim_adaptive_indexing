#include "../Framework/interface.h"
#include "../Framework/queries.h"

int main(int argc, char* argv[]) {
	int 			**queries;			/*e.g., query[i][0]:lowX,query[i][1]:hghX,query[i][2]:lowY,query[i][3]:hghY*/
	int 			ntuples 			= atoi(argv[2]);
	float 			selectivity 			= atof(argv[3]);
	int			nqueries			= atoi(argv[4]);
	int 			ndimensions			= atoi(argv[5]);

	/*check command line arguments*/
	if(argc != 6)
	{
		fprintf(stderr, "usage: %s incorrect number of arguments.\n", argv[0]);
		fprintf(stderr, "Give the name of the workload as 1st argument, the number of tuples as 2nd argument, the selectivity as 3rd argument, the number of queries as 4th argument and the number of dimensions as 5th argument.\n For the name of the workload, chose one of the following options:\n");
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

	/*allocate memory to array of queries/predicates*/
	if((queries = (int**)malloc(nqueries * sizeof(int*))) == NULL)
	{
		fprintf(stderr, "main: cannot allocate memory for **queries.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < nqueries; i++)
	{
		queries[i] = (int*)malloc(ndimensions * 2 * sizeof(int));
		if(queries[i] == NULL)
		{
			fprintf(stderr, "main: cannot allocate memory for queries[%d]\n", i);
			exit(EXIT_FAILURE);
		}
	}

	/*create the query predicates*/
	create_queries(argv[1], queries, ntuples, selectivity, nqueries, ndimensions);

	return 0;
}
