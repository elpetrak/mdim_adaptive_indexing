#include "../Framework/kdtree.h"

typedef struct kdnodestatistics
{
	int	nleft;
	int	nright;
	int	nboth;
	int	isleaf;
}kdnodestatistics;

typedef struct kdtreefunctions{
	kdnodeptr node;
	int n;
	kdnodestatistics* stat;
	void (*printnode)(kdnodeptr node, int n);
	void (*printnodestatistics)(kdnodeptr node, kdnodestatistics* stat);
} kdtreefunctions;

//-------------------------------------
// PRIVATE FUNCTIONS
//-------------------------------------

static inline kdnodeptr newKDNode(int *kpoint, long pos, int k)
{
	kdnodeptr newnode = (kdnodeptr)malloc(sizeof(struct kdnode));
	newnode->kpoint   = (int*)malloc(k*sizeof(int));
	for(int i = 0; i < k; i++)
		newnode->kpoint[i] = kpoint[i];
	//newnode->kpoint   = kpoint;
	newnode->pos      = pos;
	newnode->left     = NULL;
	newnode->right    = NULL;

	return newnode;
}

static inline void insertPosition(long *positions, long newposition, char type)
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

static long firstCrack(int **data, int **data_copy, long *map, long start, long end, int *kpoint, int k)
{
	long i, u, l;
	int j;

	for (i = 0, l = start, u = end; i <= end; i++)
	{
		if(data[0][i] <= kpoint[0]) 
		{
			for(j = 0; j < k; j++)
				data_copy[j][l] = data[j][i];	
			map[l] = i;		
			l++;
		}
		else
		{
			for(j = 0; j < k; j++)
				data_copy[j][u] = data[j][i];
			map[u] = i;
			u--;
		}
	}
	return l;
}

static long crack(int **data_copy, long *map, long start, long end, int *kpoint, int k, int levelmod)
{
	long u, l;
	int j;

	for (l = start, u = end; l <= u;)
	{
		if (data_copy[levelmod][l] > kpoint[levelmod] && data_copy[levelmod][u] <= kpoint[levelmod])
		{
			for(j = 0; j < k; j++)
			{
				int tmp = data_copy[j][l];
				data_copy[j][l] = data_copy[j][u];
				data_copy[j][u] = tmp;
			}
			long tmp_map = map[l];
			map[l] = map[u];
			map[u] = tmp_map;
			l++;
			u--;
		}
		else if (data_copy[levelmod][l] <= kpoint[levelmod])
			l++;
		else //if (data_copy[levelmod][u] > kpoint[levelmod])
			u--;		
	}
	return l;
}

//-------------------------------------
// PUBLIC FUNCTIONS
//-------------------------------------

kdnodeptr insertKDTree(kdnodeptr root, int *kpoint, int **data, int **data_copy, long *map, int ntuples, int k, long *pos)
{
	kdnodeptr prev = NULL;
	kdnodeptr cur  = root;
	int levelmod      = 0;
	char type = ' '; //left:'l', right:'r'
	int j;
	long positions[3];

	if(root == NULL)
	{
		root = newKDNode(kpoint, -1, k);
		//crack (root) and return the split position
		root->pos = firstCrack(data, data_copy, map, 0, ntuples - 1, kpoint, k);
		*pos = root->pos; 

		return root;
	}

	positions[0] = 0;
	positions[1] = root->pos;
	positions[2] = ntuples;
	while(cur)
	{
		for (j = 0; j < k && kpoint[j] == cur->kpoint[j]; j++);

		if (j == k)
		{
			// no need to crack, already cracked
			*pos = cur->pos;
			return cur;
		}

		prev = cur;

		if(kpoint[levelmod] <= cur->kpoint[levelmod])
		{
			cur = cur->left;
			type = 'l';
			/*keep track of the visited positions*/
			if(cur != NULL)
				insertPosition(positions,cur->pos, type);
		}
		else
		{
			cur = cur->right;
			type = 'r';
			/*keep track of the visited positions*/
			if(cur != NULL)
				insertPosition(positions,cur->pos, type);
		}
		levelmod = (levelmod + 1) % k;
	}
	if(type == 'l')
	{
		prev->left = newKDNode(kpoint, -1, k);
		//crack (prev->left) and return the split position
		prev->left->pos = crack(data_copy, map, positions[0], positions[1] - 1, kpoint, k, levelmod);
		*pos = prev->left->pos;
		return prev->left;
	}
	else
	{
		prev->right = newKDNode(kpoint, -1, k);
		//crack (prev->right) and return the split position
		prev->right->pos = crack(data_copy, map, positions[1], positions[2] - 1, kpoint, k, levelmod);
		*pos = prev->right->pos;
		return prev->right;
	}
}

kdnodeptr pointSearchKDTree(kdnodeptr root, int *kpoint, int k)
{
	kdnodeptr cur  = root;
	int levelmod      = 0;

	while(cur)
	{
		int cntn = 0;
		for(int j = 0; j < k; j ++)
			cntn += (kpoint[j] == cur->kpoint[j]);
		if(cntn == k)
			return cur;	

		if(kpoint[levelmod] < cur->kpoint[levelmod])
			cur = cur->left;
		else 
			cur = cur->right;
		levelmod = (levelmod + 1) % k;
	}
	return NULL;
}

int KDTreeHeight(kdnodeptr root)
{
    	int height = -1;
        if (root->left || root->right) 
	{
            int leftheight = 0, rightheight = 0;

            if (root->left)
                leftheight = KDTreeHeight(root->left);

            if (root->right)
                rightheight = KDTreeHeight(root->right);

            if (leftheight > rightheight)
                height = leftheight + 1;
            else
                height = rightheight + 1;
	}
    	return height;
}

