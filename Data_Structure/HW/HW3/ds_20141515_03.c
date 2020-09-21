#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#define RIGHT 0
#define LEFT 1

typedef struct threadedTree *threadedPointer;
typedef struct threadedTree {
	short int leftThread;
	threadedPointer leftChild;
	char data;
	threadedPointer rightChild;
	short int rightThread;
} threadedTree;

threadedPointer insucc(threadedPointer tree); //Program 5.10
void tinorder(threadedPointer tree); //Program 5.11
void insertRight(threadedPointer s, threadedPointer r); //Program 5.12

void construct_tree(threadedPointer tree, FILE *fp); // 주어진 threaded binary tree 생성
void insertLeft(threadedPointer s, threadedPointer r); 
void insert(threadedPointer s, char data, short int insertFlag); //insertRight를 수행하기 전에 노드생성과 생성된 노드에 data를 저장
threadedPointer inpredec(threadedPointer tree);
threadedPointer tinorderSearch(threadedPointer tree, char searchingData);
void tpreorder(threadedPointer tree);

int main()
{
	/* initialize */
	FILE *fp;
	threadedPointer root;
	root = (threadedPointer)malloc(sizeof(threadedTree));

	root->leftThread = true;
	root->rightThread = true;
	root->leftChild = root;
	root->rightChild = root;
	root->data = 0;

	/* construction */
	fp = fopen("input.txt", "r");
	construct_tree(root, fp);
	tinorder(root);
	printf("\n");
	tpreorder(root);
	printf("\n");
	return 0;
}

void construct_tree(threadedPointer tree, FILE *fp) // 주어진 threaded binary tree 생성
{
	threadedPointer temp_root = tree;
	char opt, data, lr;

	while ((opt = fgetc(fp)) != EOF) {
		fgetc(fp);// ' ' skip
		data = fgetc(fp);// data
		if (opt == 'I' || opt == 'i') {
			fgetc(fp);// ' ' skip
			lr = fgetc(fp);// left or right
			if (lr == 'L' || lr == 'l')
				insert(temp_root, data, LEFT);
			else if (lr == 'R' || lr == 'r')
				insert(temp_root, data, RIGHT);
		}
		else if (opt == 'S' || opt == 's') {
			temp_root = tinorderSearch(tree, data);
			if (temp_root == NULL) {
				printf("input.txt error (Empty tree!)\n");
				break;
			}
		}
		else {
			printf("input.txt error (Insert(I) or Search(S))\n");
			break;
		}
		fgetc(fp);// '\n' skip
	}
}

threadedPointer insucc(threadedPointer tree) //Program 5.10
{
	threadedPointer temp;
	temp = tree->rightChild;
	if (tree->data == 0 || !tree->rightThread)
		while (!temp->leftThread)
			temp = temp->leftChild;
	return temp;
}

threadedPointer inpredec(threadedPointer tree) {
	threadedPointer temp;
	temp = tree->leftChild;
	if (tree->data == 0 || !tree->leftThread)
		while (!temp->rightThread)
			temp = temp->rightChild;
	return temp;
}

void tinorder(threadedPointer tree) //Program 5.11
{
	threadedPointer temp = tree;
	while (1) {
		temp = insucc(temp);
		if (temp == tree)	break;
		printf("%c  ", temp->data);
	}
}

void tpreorder(threadedPointer tree) {
	threadedPointer temp = tree;
	if (!temp->leftThread)
		temp = temp->leftChild;
	else {
		while (temp->rightThread && temp != tree)
			temp = temp->rightChild;
		temp = temp->rightChild;
	}
	while (temp != tree) {
		printf("%c  ", temp->data);
		if (!temp->leftThread)
			temp = temp->leftChild;
		else {
			while (temp->rightThread && temp != tree)
				temp = temp->rightChild;
			temp = temp->rightChild;
		}
	}
}

void insert(threadedPointer s, char data, short int insertFlag) //insertRight를 수행하기 전에 노드생성과 생성된 노드에 data를 저장
{
	threadedPointer temp;
	temp = (threadedPointer)malloc(sizeof(threadedTree));
	temp->data = data;
	if (insertFlag == RIGHT)
		insertRight(s, temp);
	else if (insertFlag == LEFT)
		insertLeft(s, temp);
	else {
		printf("insertFlag error\n");
		exit(0);
	}
}

void insertRight(threadedPointer s, threadedPointer r) //Program 5.12
{
	threadedPointer temp;
	r->rightChild = s->rightChild;
	r->rightThread = s->rightThread;
	r->leftChild = s;
	r->leftThread = true;
	s->rightChild = r;
	s->rightThread = false;
	if (!r->rightThread) {
		temp = insucc(r);
		temp->leftChild = r;
	}
}

void insertLeft(threadedPointer s, threadedPointer r)
{
	threadedPointer temp;
	r->leftChild = s->leftChild;
	r->leftThread = s->leftThread;
	r->rightChild = s;
	r->rightThread = true;
	s->leftChild = r;
	s->leftThread = false;
	if (!r->leftThread) {
		temp = inpredec(r);
		temp->rightChild = r;
	}
}

threadedPointer tinorderSearch(threadedPointer tree, char searchingData) {
	threadedPointer temp = tree;
	while (1) {
		temp = insucc(temp);
		if (temp->data == searchingData)
			return temp;
	}
}