#ifndef _CREATEDOT_H_
#define _CREATEDOT_H_

#include "kdtree.h"

void bst_print_dot_null(kdnodeptr node, int nullcount, FILE* stream, int k);
void bst_print_dot_aux(kdnodeptr root, kdnodeptr node, FILE* stream, int k);
void bst_print_dot(kdnodeptr tree, FILE* stream, int k);

#endif /*_CREATEDOT_H_*/
