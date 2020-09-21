#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50

typedef enum { head, entry } tagfield;
typedef struct matrix_node* matrix_pointer;
typedef struct entry_node {
	int row;
	int col;
	int value;
}entry_node;

typedef struct matrix_node {
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;
	union {
		matrix_pointer next;
		entry_node entry;
	} u;
}matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer mread(FILE*);
void mwrite(FILE* , matrix_pointer);
void merase(matrix_pointer*);
matrix_pointer mswitch(matrix_pointer, int, int, int);
matrix_pointer mtranspose(matrix_pointer);

void main()
{
	int x;
	int y;
	int z;
	
	scanf("%d %d %d", &x, &y, &z);
	
	matrix_pointer a, d;
	FILE*fp;

	fp = fopen("A.txt", "r");
	a = mread(fp);

	mwrite(NULL, a);

	d = mswitch(a, x, y, z);

	fp = fopen("switch_20141515.txt", "w");
	mwrite(NULL, d);
	mwrite(fp, d);
	merase(&d);	

	fp = fopen("transpose_20141515.txt", "w");
	d = mtranspose(a);
	mwrite(NULL, d);
	mwrite(fp, d);
	fclose(fp);
	merase(&d);
	merase(&a);
	mwrite(NULL, d);
	mwrite(NULL, a);
}

matrix_pointer mread(FILE* fp)
{
	int numRows, numCols, numHeads;
	int i, j;
	int row, col, value, currentRow;
	matrix_pointer temp, last, node;

	fscanf(fp, "%d %d", &numRows, &numCols);
	numHeads = (numCols>numRows) ? numCols : numRows;
	node = (matrix_pointer)malloc(sizeof(matrix_node));
	node->tag = entry;
	node->u.entry.row = numRows;	node->u.entry.col = numCols;

	if (!numHeads)	node->right = node;
	else { /*헤더 노드들을 초기화한다.*/
		for (i = 0; i < numHeads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix_node));
			hdnode[i] = temp;	hdnode[i]->tag = head;
			hdnode[i]->right = temp;	hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0]; 
		
		for (i = 0; i<numRows; i++) {
			for (j = 0; j<numCols; j++) {
				fscanf(fp, "%d", &value);
				row = i;	col = j;
				if (row>currentRow) {
					last->right = hdnode[currentRow];
					currentRow = row; last = hdnode[row];
				}
				if (value != 0) {
					temp = (matrix_pointer)malloc(sizeof(matrix_node));
					temp->tag = entry;
					temp->u.entry.row = row;	temp->u.entry.col = col;	temp->u.entry.value = value; 
					last->right = temp; last = temp;
					hdnode[col]->u.next->down = temp;	hdnode[col]->u.next = temp;
				}
			}
		}
		last->right = hdnode[currentRow];

		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	return node;
}

void mwrite(FILE*fp, matrix_pointer node)
{
	int i;
	matrix_pointer temp, head;

	if (node != NULL) {
		printf("\n");
		head = node->right;
		if (fp != NULL) {
			for (i = 0; i < node->u.entry.row; i++)
			{
				for (temp = head->right; temp != head; temp = temp->right)
					fprintf(fp,"%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
				head = head->u.next;
				}
		}
		else {;
			for (i = 0; i < node->u.entry.row; i++)
			{
				for (temp = head->right; temp != head; temp = temp->right)
					printf("%d %d %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
				head = head->u.next;
			}
		}
	}
	else 
		printf("Error occurred or Matrix Erased (Node = NULL)\n");
}

void merase(matrix_pointer* node)
{
	if (*node == NULL) {
		printf("Already Erased (Node = NULL)\n");
		return;
	}
	matrix_pointer x, y, head = (*node)->right;
	int i;
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y;	y = y->right;	free(x);
		}
		x = head;	head = head->u.next;	free(x);
	}
	y = head;
	while (y != *node) {
		x = y;
		y = y->u.next;	
		free(x);
	}
	free(*node);	
	*node = NULL;
}

matrix_pointer mswitch(matrix_pointer node, int opt , int num1, int num2) {
 	int numRows, numCols, numHeads;
	int i, currentRow, swap;
	matrix_pointer temp, first, last, nodeD, tempNode, tempSwap1;
	matrix_pointer hdnodeD[MAX_SIZE];

	numRows = node->u.entry.row;	numCols = node->u.entry.col;
	numHeads = (numCols>numRows) ? numCols : numRows;
	nodeD = (matrix_pointer)malloc(sizeof(matrix_node));
	nodeD->tag = entry;
	nodeD->u.entry.row = numRows;	nodeD->u.entry.col = numCols;
	if (opt != 0 && opt != 1) {
		printf("Error incorrect option input(row change : 0, col change : 1)\n");
		exit(-1);
	}

	if (!numHeads)	nodeD->right = nodeD;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix_node));
			hdnodeD[i] = temp;	hdnodeD[i]->tag = head;
			hdnodeD[i]->right = temp;	hdnodeD[i]->u.next = temp;
		}
		last = hdnodeD[0];
		currentRow = 0;

		if (num1 > num2) {
			swap = num1;	num1 = num2;	num2 = swap;
		}

		first = node->right;
		if (opt == 0) {
			for (i = 0; i < node->u.entry.row; i++) {
				if (i > currentRow) {
					last->right = hdnodeD[currentRow];
					currentRow = i; last = hdnodeD[i];
				}
				if (num1 >= numRows || num2 >= numRows) {
					printf("Error out of range (row)\n");
					exit(-1);
				}
				if (first->right->u.entry.row == num1) {
					tempSwap1 = hdnode[num2];
					for (tempNode = tempSwap1->right; tempNode != tempSwap1; tempNode = tempNode->right) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = num1;	temp->u.entry.col = tempNode->u.entry.col;
						temp->u.entry.value = tempNode->u.entry.value;
						last->right = temp; last = temp;
						hdnodeD[tempNode->u.entry.col]->u.next->down = temp;	hdnodeD[tempNode->u.entry.col]->u.next = temp;
					}
				}
				else if (first->right->u.entry.row == num2) {
					tempSwap1 = hdnode[num1];
					for (tempNode = tempSwap1->right; tempNode != tempSwap1; tempNode = tempNode->right) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = num2;	temp->u.entry.col = tempNode->u.entry.col;
						temp->u.entry.value = tempNode->u.entry.value;
						last->right = temp; last = temp;
						hdnodeD[tempNode->u.entry.col]->u.next->down = temp;	hdnodeD[tempNode->u.entry.col]->u.next = temp;
					}
				}
				else {
					for (tempNode = first->right; tempNode != first; tempNode = tempNode->right) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = tempNode->u.entry.row;	temp->u.entry.col = tempNode->u.entry.col;
						temp->u.entry.value = tempNode->u.entry.value;
						last->right = temp; last = temp;
						hdnodeD[tempNode->u.entry.col]->u.next->down = temp;	hdnodeD[tempNode->u.entry.col]->u.next = temp;
					}
				}
				first = first->u.next;
			}
			last->right = hdnodeD[currentRow];
			for (i = 0; i < numCols; i++)
				hdnodeD[i]->u.next->down = hdnodeD[i];
			for (i = 0; i < numHeads - 1; i++)
				hdnodeD[i]->u.next = hdnodeD[i + 1];
			hdnodeD[numHeads - 1]->u.next = nodeD;
			nodeD->right = hdnodeD[0];
		}
		if (opt == 1) {
			for (i = 0; i < node->u.entry.col; i++) {
				if (i > currentRow) {
					last->down = hdnodeD[currentRow];
					currentRow = i; last = hdnodeD[i];
				}
				if (num1 >= numCols || num2 >= numCols) {
					printf("Error out of range (row)\n");
					exit(-1);
				}
				if (first->down->u.entry.col == num1) {
					tempSwap1 = hdnode[num2];
					for (tempNode = tempSwap1->down; tempNode != tempSwap1; tempNode = tempNode->down) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = tempNode->u.entry.row;	temp->u.entry.col = num1;
						temp->u.entry.value = tempNode->u.entry.value;
						last->down = temp; last = temp;
						hdnodeD[tempNode->u.entry.row]->u.next->right = temp;	hdnodeD[tempNode->u.entry.row]->u.next = temp;
					}
				}
				else if (first->down->u.entry.col == num2) {
					tempSwap1 = hdnode[num1];
					for (tempNode = tempSwap1->down; tempNode != tempSwap1; tempNode = tempNode->down) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = tempNode->u.entry.row;	temp->u.entry.col = num2;
						temp->u.entry.value = tempNode->u.entry.value;
						last->down = temp; last = temp;
						hdnodeD[tempNode->u.entry.row]->u.next->right = temp;	hdnodeD[tempNode->u.entry.row]->u.next = temp;
					}
				}
				else {
					for (tempNode = first->down; tempNode != first; tempNode = tempNode->down) {
						temp = (matrix_pointer)malloc(sizeof(matrix_node));
						temp->tag = entry;
						temp->u.entry.row = tempNode->u.entry.row;	temp->u.entry.col = tempNode->u.entry.col;
						temp->u.entry.value = tempNode->u.entry.value;
						last->down = temp; last = temp;
						hdnodeD[tempNode->u.entry.row]->u.next->right = temp;	hdnodeD[tempNode->u.entry.row]->u.next = temp;
					}
				}
				first = first->u.next;
			}
			last->down = hdnodeD[currentRow];
			for (i = 0; i < numCols; i++)
				hdnodeD[i]->u.next->right = hdnodeD[i];
			for (i = 0; i < numHeads - 1; i++)
				hdnodeD[i]->u.next = hdnodeD[i + 1];
			hdnodeD[numHeads - 1]->u.next = nodeD;
			nodeD->right = hdnodeD[0];
		}
	}	
	return nodeD;
}

matrix_pointer mtranspose(matrix_pointer node ) {
	int numRows, numCols, numHeads, numTerms = 0;
	int i, currentRow;
	matrix_pointer temp, first, last, nodeD, tempNode;
	matrix_pointer hdnodeD[MAX_SIZE];

	numRows = node->u.entry.col;	numCols = node->u.entry.row;
	numHeads = (numCols>numRows) ? numCols : numRows;
	nodeD = (matrix_pointer)malloc(sizeof(matrix_node));
	nodeD->tag = entry;
	nodeD->u.entry.row = numRows;	nodeD->u.entry.col = numCols;

	if (!numHeads)	nodeD->right = nodeD;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix_node));
			hdnodeD[i] = temp;	hdnodeD[i]->tag = head;
			hdnodeD[i]->right = temp;	hdnodeD[i]->u.next = temp;
		}
		last = hdnodeD[0];
		currentRow = 0;

		first = node->right;
		for (i = 0; i < node->u.entry.col; i++) {
			if (i > currentRow) {
				last->right = hdnodeD[currentRow];
				currentRow = i; last = hdnodeD[i];
			}
			for (tempNode = first->down; tempNode != first; tempNode = tempNode->down) {
				temp = (matrix_pointer)malloc(sizeof(matrix_node));
				temp->tag = entry;
				temp->u.entry.row = tempNode->u.entry.col;	temp->u.entry.col = tempNode->u.entry.row;
				temp->u.entry.value = tempNode->u.entry.value;
				last->right = temp; last = temp;
				hdnodeD[tempNode->u.entry.col]->u.next->down = temp;	hdnodeD[tempNode->u.entry.col]->u.next = temp;
			}
			first = first->u.next;
		}
		last->right = hdnodeD[currentRow];
		for (i = 0; i < numCols; i++)
			hdnodeD[i]->u.next->down = hdnodeD[i];
		for (i = 0; i < numHeads - 1; i++)
			hdnodeD[i]->u.next = hdnodeD[i + 1];
		hdnodeD[numHeads - 1]->u.next = nodeD;
		nodeD->right = hdnodeD[0];
	}	
	return nodeD;
}