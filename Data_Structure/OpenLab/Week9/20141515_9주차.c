#include<stdio.h>
#include<stdlib.h>

#define MAX_VERTICES 30
#define MIN2(x,y) ((x) < (y) ? (x) : (y) )
short int dfn[MAX_VERTICES];
short int low[MAX_VERTICES];
int num;
void init();
void bicon(int, int);

typedef struct node* node_pointer;
typedef struct node {
	int data;
	struct node* link;
} node;

typedef struct EDGE {
	int x, y;
	struct EDGE* next;
}EDGE;
EDGE*top;
node_pointer graph[MAX_VERTICES];

void push(int x, int y) {
	EDGE*New = (EDGE*)calloc(1, sizeof(EDGE));
	New->x = x;
	New->y = y;
	if(top==NULL)
		top = New;
	else {
		New->next = top;
		top = New;
	}
	return;
}
void pop(int*x, int*y) {
	if(top==NULL)
		exit(-1);
	*x = top->x;
	*y= top->y;
	EDGE*del = top;
	top = top->next;
	free(del);
	return;
}
void main() {
	
	int i,j=0,size,vertex;
	node_pointer temp, tempNode;
	FILE *fp;
	fp = fopen("input.txt", "r");
	fscanf(fp, "%d", &size);
	for(i=0; i<size; i++) {
		fscanf(fp, "%d", &vertex);
		graph[vertex] = (node_pointer)malloc(sizeof(node));
		while(fgetc(fp)==' ') {
			if(j==0) {
				graph[vertex]->data = fgetc(fp)-'0';
				graph[vertex]->link = NULL;
				tempNode = graph[vertex];
				j++;
			}
			else {
				temp = (node_pointer)malloc(sizeof(node));
				temp->data = fgetc(fp)-'0';
				temp->link = NULL;
				if(j==1) {
					graph[vertex]->link = temp;
					j++;
				}
				else
					tempNode->link = temp;
				tempNode = temp;
			}
		}
		j=0;
	}
	num = size;
	init();
	bicon(3,-1);
}

void init() {
	int i;
	for (i = 0; i < num; i++)
		dfn[i] = low[i] = -1;
	num = 0;
}

void bicon(int u, int v)
{
	node_pointer ptr;
	int w, x, y;
	dfn[u] = low[u] = num++;
	for(ptr=graph[u]; ptr; ptr=ptr->link) {
		w = ptr->data;
		if(v != w && dfn[w] < dfn[u])
			push(u, w); /* add edge to stack */
		if(dfn[w] < 0) { /* w is an unvisited vertex */
			bicon(w, u);
			low[u] = MIN2(low[u], low[w]);
			if(low[w] >= dfn[u]) {
				printf("New biconnected component: ");
				do { /* delete edge from stack */
					pop(&x, &y);
					printf(" <%d, %d>", x, y);
				} while(!((x == u) && (y == w)));
				printf("\n");
			}
		}
	
		else if(w != v) low[u] = MIN2(low[u], dfn[w]);
	}
}