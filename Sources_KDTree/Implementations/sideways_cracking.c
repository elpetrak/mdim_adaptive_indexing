#include "../Framework/sideways_cracking.h"

//-------------------------------------
// PRIVATE FUNCTIONS
//-------------------------------------
static void generate_cracker_map(int *head_orig, int *tail_orig, cracker_map_t cracker_map, int minq, int maxq, long *crack_start, long *crack_end)
{
    long sp[5];		/* start position of each partition */
    long cnt[5];		/* cnt tuples per partition */
			/* partition 001 (<=minq), partition 010 (>minq, <maxq), partition 100 (>=maxq) */
    long i;
 
    for(i = 0; i < 5; i++)
    	cnt[i] = 0;

    /* count tuples per partition */
    for(i = 0; i < cracker_map.ntuples; i++)
    {
        int partition = ((head_orig[i] <= minq) | 
			 (((head_orig[i] > minq) & (head_orig[i] <= maxq)) << 1) |
			 ((head_orig[i] > maxq) << 2));
	cnt[partition] += 1;
    }

    /* set start and end pos of the qualifying piece. Necessary for the bv generation and to insert in AVL tree */
    *crack_start = cnt[1];
    *crack_end   = *crack_start + cnt[2];

    /* partition head data out of place */
    sp[1] = 0;
    sp[2] = sp[1] + cnt[1];
    sp[4] = sp[2] + cnt[2];
    for(i = 0; i < cracker_map.ntuples; i++)
    {
    	int partition = ((head_orig[i] <= minq) | 
			 (((head_orig[i] > minq) & (head_orig[i] <= maxq)) << 1) |
			 ((head_orig[i] > maxq) << 2));
	cracker_map.head[sp[partition]] = head_orig[i];
	sp[partition]++;
    }

    /* align tail with head out of place */
    sp[1] = 0;
    sp[2] = sp[1] + cnt[1];
    sp[4] = sp[2] + cnt[2];
    for(i = 0; i < cracker_map.ntuples; i++)
    {
    	int partition = ((head_orig[i] <= minq) | 
			 (((head_orig[i] > minq) & (head_orig[i] <= maxq)) << 1) |
			 ((head_orig[i] > maxq) << 2));
	cracker_map.tail[sp[partition]] = tail_orig[i];
        sp[partition]++;
    }
}

static void generate_cm_bv(cracker_map_t cracker_map, int minq, int maxq, long crack_start, long crack_end, long bvs)
{
    long i, j;

    /* initialize bv to 0 */
    for(i = 0; i < bvs; i++)
        cracker_map.bv[i] = 0;

    /* populate bv */
    for(j = 0, i = crack_start; i < crack_end; i++, j++)
    {
    	cracker_map.bv[j>>6] |= (((unsigned long int)((cracker_map.tail[i] > minq) & (cracker_map.tail[i] <= maxq))) << (j & 0x3F)); //j/64,j%64
    }
}

//-------------------------------------
// PUBLIC FUNCTIONS
//-------------------------------------
long crack_first(int *head_orig, int *tail_orig, cracker_map_t *cracker_map, int minq_head, int maxq_head, int minq_tail, int maxq_tail, long tuples)
{
	long bvs;
        long qh_tuples = 0, crack_start, crack_end;

        cracker_map->head = (int*)malloc(tuples*sizeof(int));
       	cracker_map->tail = (int*)malloc(tuples*sizeof(int));
       	cracker_map->ntuples = tuples;
	cracker_map->avl_index = NULL;
       
       	/* generate cracker map */
       	generate_cracker_map(head_orig, tail_orig, *cracker_map, minq_head, maxq_head, &crack_start, &crack_end);

        /* insert query values in AVL of cracker map */
        cracker_map->avl_index = insert(cracker_map->avl_index,minq_head,crack_start);
       	cracker_map->avl_index = insert(cracker_map->avl_index,maxq_head,crack_end);

	/* generate intermediate bv */
        qh_tuples = crack_end - crack_start;
        bvs = ((qh_tuples & 0x3F) == 0? (qh_tuples >> 6): (qh_tuples >> 6) + 1); //if qh_tuples%64==0 => bvs=qh_tuples/64
	if(bvs > 0)
        {
		cracker_map->bv = (unsigned long int*)malloc(bvs * sizeof(unsigned long int));
        	generate_cm_bv(*cracker_map, minq_tail, maxq_tail, crack_start, crack_end, bvs);
        }

	return bvs;
}

long swcrack(cracker_map_t *cracker_map, int minq_head, int maxq_head, int minq_tail, int maxq_tail)
{
	long bvs;
        long qh_tuples = 0, crack_start, crack_end;

        /* crack + insert query values in AVL of cracker map */
        cracker_map->avl_index = swcrack_insert(cracker_map,minq_head,&crack_start);
       	cracker_map->avl_index = swcrack_insert(cracker_map,maxq_head,&crack_end);

	/* generate intermediate bv */
        qh_tuples = crack_end - crack_start;
        bvs = ((qh_tuples & 0x3F) == 0? (qh_tuples >> 6): (qh_tuples >> 6) + 1); //if qh_tuples%64==0 => bvs=qh_tuples/64
	if(bvs > 0)
        {
		cracker_map->bv = (unsigned long int*)malloc(bvs * sizeof(unsigned long int));
        	generate_cm_bv(*cracker_map, minq_tail, maxq_tail, crack_start, crack_end, bvs);
        }

	return bvs;
}
