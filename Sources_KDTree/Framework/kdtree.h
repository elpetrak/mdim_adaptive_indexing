#ifndef _KDTREE_H_
#define _KDTREE_H_

#include "interface.h"

#ifdef DEBUG
#include <assert.h>
#endif

struct kdnode
{
	int           *kpoint;
	long           pos;
        struct kdnode *left;
        struct kdnode *right;
};

typedef struct kdnode* kdnodeptr;

//-------------------------------------
// PUBLIC FUNCTIONS
//-------------------------------------
kdnodeptr insertKDTree(kdnodeptr root, int *kpoint, int **data, int **data_copy, long *map, int ntuples, int k, long *pos);
kdnodeptr pointSearchKDTree(kdnodeptr root, int *kpoint, int k);
int KDTreeHeight(kdnodeptr root);

#endif /*_KDTREE_H_*/
