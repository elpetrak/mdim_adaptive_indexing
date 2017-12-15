#include "../Framework/createdot.h"

static int pointLevelKDTree(kdnodeptr root, int *kpoint, int k)
{
	kdnodeptr cur  = root;
	int levelmod      = 0;

	while(cur)
	{
		int cntn = 0;
		for(int j = 0; j < k; j ++)
			cntn += (kpoint[j] == cur->kpoint[j]);
		if(cntn == k)
			return levelmod;	

		if(kpoint[levelmod] < cur->kpoint[levelmod])
			cur = cur->left;
		else 
			cur = cur->right;
		levelmod = (levelmod + 1) % k;
	}
	return -1;
}
	

void bst_print_dot_null(kdnodeptr node, int nullcount, FILE* stream, int k)
{
    	fprintf(stream, "    null%d [shape=point];\n", nullcount);
	fprintf(stream, "    \"");
	for(int i = 0; i < k; i++)
		fprintf(stream, "%d: %d\n", i, node->kpoint[i]);
	fprintf(stream, "P:%ld\n",node->pos);
	fprintf(stream, "\" -> null%d[style=dashed];\n", nullcount);
}

void bst_print_dot_aux(kdnodeptr root, kdnodeptr node, FILE* stream, int k)
{
    	static int nullcount = 0;
	unsigned int color_array[5] = {0xFDDBE3, 0x9BD2F8,0xFAFAD2,0x800080,0xFFA500};
	int levelmod = 0;

    	if (node->left)
    	{

		/*node connection*/
		fprintf(stream, "    \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->pos);
		fprintf(stream, "\" -> \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->left->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->left->pos);
		fprintf(stream, "\";\n");

		/*node color*/
		levelmod = pointLevelKDTree(root, node->kpoint, k);
		fprintf(stream, "    \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->pos);
		fprintf(stream, "\"[style = filled,color=\"#%06x\"];\n",color_array[levelmod]);
	
		levelmod = pointLevelKDTree(root, node->left->kpoint, k);
		fprintf(stream, "     \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->left->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->left->pos);
		fprintf(stream, "\"[style = filled,color=\"#%06x\"];\n",color_array[levelmod]);

	
        	bst_print_dot_aux(root, node->left, stream, k);
    	}
    	else
        	bst_print_dot_null(node, nullcount++, stream, k);

    	if (node->right)
    	{
		/*node connection*/
		fprintf(stream, "    \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->pos);
		fprintf(stream, "\" -> \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->right->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->right->pos);
		fprintf(stream, "\";\n");

		/*node color*/
		levelmod = pointLevelKDTree(root, node->kpoint, k);
		fprintf(stream, "    \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->pos);
		fprintf(stream, "\"[style = filled,color=\"#%06x\"];\n",color_array[levelmod]);

		levelmod = pointLevelKDTree(root, node->right->kpoint, k);
		fprintf(stream, "     \"");
		for(int i = 0; i < k; i++)
			fprintf(stream, "%d: %d\n", i, node->right->kpoint[i]);
		fprintf(stream, "P:%ld\n",node->right->pos);
		fprintf(stream, "\"[style = filled,color=\"#%06x\"];\n",color_array[levelmod]);
		
        	bst_print_dot_aux(root, node->right, stream, k);
    	}
    	else
        	bst_print_dot_null(node, nullcount++, stream, k);
}

void bst_print_dot(kdnodeptr tree, FILE* stream, int k)
{
	unsigned int color_array[5] = {0xFDDBE3, 0x9BD2F8,0xFAFAD2,0x800080,0xFFA500};

    	fprintf(stream, "digraph BST {\n");
    	fprintf(stream, "    node [fontname=\"Arial\"];\n");

    	if (!tree)
        	fprintf(stream, "\n");
    	else if (!tree->right && !tree->left)
	{
		fprintf(stream, "    \"");
		//for(int i = 0; i < k; i++)
			fprintf(stream, "0: %d\n", tree->kpoint[0]);
		fprintf(stream, "P:%ld\n",tree->pos);
		fprintf(stream, "\";\n");
	}
    	else
        	bst_print_dot_aux(tree, tree, stream, k);

	for(int i = 0; i < k; i++)
		fprintf(stream, "\"Dim %d\"[style = filled,color=\"#%06x\"];\n",i,color_array[i]);


    	fprintf(stream, "}\n");
}
