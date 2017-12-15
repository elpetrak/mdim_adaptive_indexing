#ifndef _QUERIES_H_
#define _QUERIES_H_

#include "interface.h"

//-------------------------------------
// FUNCTION DEFINITIONS
//-------------------------------------
/*scan functions*/
void create_queries(const char *distribution, int **queries, int ntuples, float selectivity, int nqueries, int ndimensions);
#endif /*_QUERIES_H_*/
