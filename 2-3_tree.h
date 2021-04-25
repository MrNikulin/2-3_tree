#ifndef __2_3_TREE_H__
#define __2_3_TREE_H__

typedef int type;
typedef struct node {
	int size;//number of keys in the node
	type keys[3];//keys
	struct node* parent;//address of the node's parent
	struct node* sons[4];//addresses of the node's sons
} node;

void del_tree(node** root);
node* insert(node* tree, type k);
node* search(node* tree, type k);
node* del_key(node* p, type k);

#endif