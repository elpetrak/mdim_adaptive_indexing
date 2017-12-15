#ifndef _SIDEWAYS_CRACKING_H_
#define _SIDEWAYS_CRACKING_H_

#include "avltree.h"

//-------------------------------------
// FUNCTION DEFINITIONS
//-------------------------------------
/*Cracking funsctions*/
long crack_first(int *head_orig, int *tail_orig, cracker_map_t *cracker_map, int minq_head, int maxq_head, int minq_tail, int maxq_tail, long tuples);
long swcrack(cracker_map_t *cracker_map, int minq_head, int maxq_head, int minq_tail, int maxq_tail);
#endif /*_SIDEWAYS_CRACKING_H_*/
