#include "../Framework/avltree.h"

//-------------------------------------
// PRIVATE FUNCTIONS
//-------------------------------------
static int height(node *T)
{
    int lh,rh;
    if(T==NULL)
        return(0);
    
    if(T->left==NULL)
        lh=0;
    else
        lh=1+T->left->ht;
        
    if(T->right==NULL)
        rh=0;
    else
        rh=1+T->right->ht;
    
    if(lh>rh)
        return(lh);
    
    return(rh);
}

static int BF(node *T)
{
    int lh,rh;
    if(T==NULL)
        return(0);
 
    if(T->left==NULL)
        lh=0;
    else
        lh=1+T->left->ht;
 
    if(T->right==NULL)
        rh=0;
    else
        rh=1+T->right->ht;
 
    return(lh-rh);
}

node * rotateright(node *x)
{
    node *y;
    y=x->left;
    x->left=y->right;
    y->right=x;
    x->ht=height(x);
    y->ht=height(y);
    return(y);
}
 
static node * rotateleft(node *x)
{
    node *y;
    y=x->right;
    x->right=y->left;
    y->left=x;
    x->ht=height(x);
    y->ht=height(y);
    
    return(y);
}
 
static node * RR(node *T)
{
    T=rotateleft(T);
    return(T);
}
 
static node * LL(node *T)
{
    T=rotateright(T);
    return(T);
}
 
static node * LR(node *T)
{
    T->left=rotateleft(T->left);
    T=rotateright(T);
    
    return(T);
}
 
static node * RL(node *T)
{
    T->right=rotateright(T->right);
    T=rotateleft(T);
    return(T);
}

static void insertPosition(long *positions, long newposition, char type)
{
	if(type == 'l')
	{
		positions[2] = positions[1];
		positions[1] = newposition;
	}
	else
	{
		positions[0] = positions[1];
		positions[1] = newposition;
	}
}

static long crack_one(cracker_map_t *cracker_map, long startPos, long endPos, int pivot)
{
	long lowerCursor = startPos;
	long upperCursor = endPos;

	while (lowerCursor < upperCursor)
	{
		if ((cracker_map->head[lowerCursor] > pivot) & (cracker_map->head[upperCursor] <= pivot))
		{
			int tmp = cracker_map->head[lowerCursor];
			cracker_map->head[lowerCursor] = cracker_map->head[upperCursor];
			cracker_map->head[upperCursor] = tmp;

			tmp = cracker_map->tail[lowerCursor];
			cracker_map->tail[lowerCursor] = cracker_map->tail[upperCursor];
			cracker_map->tail[upperCursor] = tmp;

			lowerCursor++;
			upperCursor--;
		}
		else if (cracker_map->head[lowerCursor] <= pivot)
			lowerCursor++;
		else //(cracker_map->head[upperCursor] > pivot)
			upperCursor--;
	}
	if((lowerCursor <= endPos) & (cracker_map->head[lowerCursor] > pivot))
		return lowerCursor;
	else
		return lowerCursor + 1;
}
//-------------------------------------
// PUBLIC FUNCTIONS
//-------------------------------------
node * insert(node *T, int x, long crack_pos)
{
    if(T==NULL)
    {
        T=(node*)malloc(sizeof(node));
        T->data=x;
	T->pos=crack_pos;
        T->left=NULL;
        T->right=NULL;
    }
    else
        if(x > T->data)        // insert in right subtree
        {
            T->right=insert(T->right,x,crack_pos);
            if(BF(T)==-2)
            {
                if(x>T->right->data)
                    T=RR(T);
                else
                    T=RL(T);
            }
        }
        else
            if(x<T->data)
            {
                T->left=insert(T->left,x,crack_pos);
                if(BF(T)==2)
                {
                    if(x < T->left->data)
                        T=LL(T);
                    else
                        T=LR(T);
                }
            }
        
        T->ht=height(T);
        
        return(T);
}

void preorder(node *T)
{
    if(T!=NULL)
    {
        printf("%d,%ld(Bf=%d)",T->data,T->pos,BF(T));
        preorder(T->left);
        preorder(T->right);
    }
}

node * swcrack_insert(cracker_map_t *cracker_map, int x, long *crack_pos)
{
    node *cur  = cracker_map->avl_index;
    char type = ' '; //left:'l', right:'r'
    long pos;
    long positions[3];

    positions[0] = 0;
    positions[1] = cracker_map->avl_index->pos; 
    positions[2] = cracker_map->ntuples;
    
    while(cur)
    {
	if(x == cur->data)
	{
	    *crack_pos = cur->pos;
	    return cracker_map->avl_index;	
	}
        if(x > cur->data)        // insert in right subtree
        {
	    cur = cur->right;
	    type = 'r';
	    /*keep track of the visited positions*/
	    if(cur != NULL)
	        insertPosition(positions,cur->pos,type);
        }
        else
        {
	    cur = cur->left;
	    type = 'l';
	    /*keep track of the visited positions*/
	    if(cur != NULL)
	        insertPosition(positions,cur->pos,type);
        }
     }
     if(type == 'l')
     {
	pos=crack_one(cracker_map, positions[0], positions[1], x);
	cracker_map->avl_index = insert(cracker_map->avl_index,x,pos);
     }
     else
     {
	pos=crack_one(cracker_map, positions[1], positions[2], x);
	cracker_map->avl_index = insert(cracker_map->avl_index,x,pos);
     }
     *crack_pos = pos;
     return cracker_map->avl_index;
}       
       

