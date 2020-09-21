#include<stdio.h>
#include<stdlib.h>
#define MAX_TERMS 100

typedef struct treeNode* treePtr;
typedef struct treeNode {
	treePtr lchild;
	int data;
	treePtr rchild;
}treeNode;

void inorder(treePtr);
void preorder(treePtr);
void insert(treePtr*, int);
treePtr modifiedSearch(treePtr, int);

void main() {
	int i, n, A[MAX_TERMS];
	treePtr tree = NULL;
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &n);
	for(i=0; i<n; i++)
		fscanf(fp, "%d", &A[i]);
	for(i=0; i<n; i++)
		insert(&tree, A[i]);
	printf("Inorder Traversal\n");
	inorder(tree);
	printf("\nPreorder Traversal\n");
	preorder(tree);
	printf("\n");
}

void inorder(treePtr node) {
	if(node == NULL)
		return;
	inorder(node->lchild);
	printf("%d ", node->data);
	inorder(node->rchild);
	/*if(node->lchild == NULL) {
		free(node);
		return;
	}*/
}

void preorder(treePtr node) {
	if(node == NULL)
		return;
	printf("%d ", node->data);
	preorder(node->lchild);
	preorder(node->rchild);
}

void insert(treePtr *node, int k) {
	treePtr ptr, temp = modifiedSearch(*node, k);
	if(temp || !(*node)) {
		ptr = (treePtr)malloc(sizeof(treeNode));
		ptr->data = k;
		ptr->lchild = ptr->rchild = NULL;
		if(*node) {
			if(k < temp->data)
				temp->lchild = ptr;
			else
				temp->rchild = ptr;
		}
		else
			*node = ptr;
	}
}

treePtr modifiedSearch(treePtr tree, int k) {
	treePtr temp;
	if(tree == NULL)
		return NULL;
	while(tree) {
		if(k == tree->data)
			return NULL;
		if(k < tree->data) {
			temp = tree;
			tree = tree->lchild;
		}
		else {
			temp = tree;
			tree = tree->rchild;
		}
	}
	return temp;
}