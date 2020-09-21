#include<stdio.h>
#include<malloc.h>
#define MAX_SIZE 24
#define IS_FULL(ptr)(!(ptr))
#define FALSE 0
#define TRUE 1

typedef struct node* node_pointer;
typedef struct node {
	int data;
	node_pointer link;
}node;

void pop (node_pointer* stack) {
	node_pointer temp;
	if(*stack==NULL)
		return;
	temp = *stack;
	*stack = temp->link;
	free (temp);
}

void push (node_pointer* stack, int data) {
	node_pointer temp;
	temp = (node_pointer)malloc (sizeof (node));
	if(IS_FULL (temp)) {
		fprintf (stderr, "The memory is full\n");
		exit (1);
	}
	temp->data = data;
	temp->link = *stack;
	*stack = temp;
}

void main ()
{
	short int out[MAX_SIZE];
	node_pointer seq[MAX_SIZE];
	node_pointer x, y, top;
	int i, j, n;
	printf ("Enter the size (<= %d) ", MAX_SIZE);
	scanf ("%d", &n);
	for(i=0; i<n; i++) {
		out[i] = TRUE;
		seq[i] = NULL;
	}

	printf ("Enter a pair of numbers (-1 -1 to quit): ");
	scanf ("%d %d", &i, &j);
	while(i>=0) {
		push (seq+i, j);
		push (seq+j, i);
		printf ("Enter a pair of numbers (-1 -1 to quit): ");
		scanf ("%d %d", &i, &j);
	}
	for(i=0; i<n; i++) {
		if(out[i]) {
			printf ("\nNew Class : %5d", i);
			out[i]=FALSE;
			x=seq[i]; top=NULL;
			for(;;) {
				while(x) {
					j = x->data;
					if(out[j]) {
						printf ("%5d", j); out[j]=FALSE;
						push (&top, j);
					}
					else x=x->link;
				}
				if(!top) break;
				x=seq[top->data]; pop(&top);
			}
		}
	}
	printf ("\n");
}