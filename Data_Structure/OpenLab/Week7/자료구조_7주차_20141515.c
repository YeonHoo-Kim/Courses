#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct threadedTree *threadedPointer;
typedef struct threadedTree {
	short int leftThread;
	threadedPointer leftChild;
	char data;
	threadedPointer rightChild;
	short int rightThread;
} threadedTree;

void construct_tree(threadedPointer tree); // 주어진 threaded binary tree 생성
threadedPointer insucc(threadedPointer tree); //Program 5.10
void tinorder(threadedPointer tree); //Program 5.11
void insert(threadedPointer s, char data); //insertRight를 수행하기 전에 노드생성과 생성된 노드에 data를 저장
void insertRight(threadedPointer s, threadedPointer r); //Program 5.12

int main()
{
	threadedPointer head;
	head=(threadedPointer)malloc (sizeof (threadedTree));

	head->leftThread=true;
	head->rightThread=false;
	head->leftChild=head;
	head->rightChild=head;
	head->data=0;

	construct_tree(head);
	insert(head->leftChild->rightChild, 'E');
	insert(head->leftChild->leftChild->rightChild, 'F');
	insert(head->leftChild->leftChild, 'G');
	tinorder(head);
	printf ("\n");
	return 0;
	return 0;
}

void construct_tree(threadedPointer tree) // 주어진 threaded binary tree 생성
{
	threadedPointer temp, temp_root;

	temp_root = tree;
	/* insert node 'A' to the left of the head */
	temp = (threadedPointer)malloc (sizeof (threadedTree));
	temp->leftThread = temp->rightThread = true;
	temp->leftChild = temp->rightChild = temp;
	temp->data = 'A';

	temp_root->leftChild = temp;
	temp_root->leftThread = false;

	temp_root = tree->leftChild;
	/* insert node 'B' to the left of the temp_head */
	temp = (threadedPointer)malloc (sizeof (threadedTree));
	temp->rightThread = temp->leftThread = true;
	temp->leftChild = tree;
	temp->rightChild = temp;
	temp->data = 'B';

	temp_root->leftChild = temp;
	temp_root->leftThread = false;

	/* insert node 'C' to the right of the temp_head */
	temp = (threadedPointer)malloc (sizeof (threadedTree));
	temp->leftThread = temp->rightThread = true;
	temp->leftChild = temp_root;
	temp->rightChild = tree;
	temp->data = 'C';

	temp_root->rightChild = temp;
	temp_root->rightThread = false;

	temp_root = temp_root->leftChild;
	/* insert node 'D' to the right of the temp_head */
	temp = (threadedPointer)malloc (sizeof (threadedTree));
	temp->leftThread = temp->rightThread = true;
	temp->leftChild = temp_root;
	temp->rightChild = tree->leftChild;
	temp->data = 'D';

	temp_root->rightChild = temp;
	temp_root->rightThread = false;
}

threadedPointer insucc(threadedPointer tree) //Program 5.10
{
	threadedPointer temp;
	temp = tree->rightChild;
	if(!tree->rightThread)
		while(!temp->leftThread)
			temp = temp->leftChild;
	return temp;
}

void tinorder(threadedPointer tree) //Program 5.11
{
	threadedPointer temp = tree;
	while(1) {
		temp = insucc (temp);
		if(temp == tree)	break;
		printf ("%3c", temp->data);
	}
}

void insert(threadedPointer s, char data) //insertRight를 수행하기 전에 노드생성과 생성된 노드에 data를 저장
{
	threadedPointer temp;
	temp=(threadedPointer)malloc (sizeof (threadedTree));
	temp->data=data;
	insertRight (s, temp);
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
	if(!r->rightThread) {
		temp = insucc (r);
		temp->leftChild = r;
	}
}