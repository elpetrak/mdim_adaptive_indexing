#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "interface.h"

typedef struct node
{
    int data;
    long pos;
    struct node *left,*right;
    int ht;
}node;

typedef struct cracker_map
{
    int *head;			/* head column */
    int *tail;			/* tail column */
    long ntuples;		/* number of tuples */
    unsigned long int *bv;	/* result bit vector */
    node *avl_index;		/* AVL tree index*/
}cracker_map_t;

//-------------------------------------
// FUNCTION DEFINITIONS
//-------------------------------------
/*AVLtree functions*/
node * insert(node *T, int x, long crack_pos);
void preorder(node *T);
node * swcrack_insert(cracker_map_t *cracker_map, int x, long *crack_pos);
#endif /*_AVLTREE_H_*/
