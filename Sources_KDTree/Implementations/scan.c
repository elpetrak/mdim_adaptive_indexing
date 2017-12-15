#include "../Framework/scan.h"

long scan_part(int **data, int *query, long start, long end, int ndimensions)
{
	int d;
	long i;
	long qualifyingTuples = 0;

	for(i = start; i < end; i++)
	{
		int cnt = 0;
		for(d = 0; d < ndimensions; d++)
		{
			cnt += ((data[d][i] > query[2*d]) & (data[d][i] <= query[2*d + 1]));
		}
		qualifyingTuples += (cnt == ndimensions);
	}
	return qualifyingTuples;
}

long scan(int **data, int *query, long ntuples, int ndimensions)
{
	int d;
	long i;
	long qualifyingTuples = 0;

	for(i = 0; i < ntuples; i++)
	{
		int cnt = 0;
		for(d = 0; d < ndimensions; d++)
		{
			cnt += ((data[d][i] > query[2*d]) & (data[d][i] <= query[2*d + 1]));
		}
		qualifyingTuples += (cnt == ndimensions);
	}
	return qualifyingTuples;
}
