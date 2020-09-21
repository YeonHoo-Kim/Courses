#include<stdio.h>
#include<stdlib.h>
#define MAX_EXPR_SIZE 100
typedef enum {lparen, rparen, plus, minus, times, divide, mod, eos, operand}
precedence;

typedef struct Node {
	precedence data;
	struct Node *link;
}Node;

Node * stack;
char expr[MAX_EXPR_SIZE];
int isp[] = {0,19,12,12,13,13,13,0};
int icp[] = {20,19,12,12,13,13,13,0};

void push(precedence);
void pop();
void postfix();
precedence getToken(char*, int*);
void printToken(precedence);
void rExpr(FILE*, char*);
void main()
{
	FILE *fp = fopen("expr.txt", "r");
	rExpr(fp, expr);
	postfix();	
}

void push(precedence data)
{
	Node*curr = (Node*)malloc(sizeof(Node));
	curr->data = data;
	curr->link = NULL;
	if (stack != NULL) curr->link = stack;
	stack = curr;
}

void pop()
{
	Node *curr;
	if (stack==NULL)
		return;
	curr = stack;
	stack = curr->link;
	free(curr);
}

void postfix()
{
	char symbol;
	precedence token;
	int n = 0;
	push(eos);
	for (token = getToken(&symbol, &n); token != eos; token = getToken(&symbol, &n)) {
		if (token == operand)
			printf("%c", symbol);
		else if (token == rparen){
			while (stack->data != lparen)
				printToken(stack->data);
			pop();
		}
		else{
			while (isp[stack->data] >= icp[token])
				printToken(stack->data);
			push(token);
		}
	}
	while ((token = stack->data) != eos)
		printToken(token);
	printf("\n");
}

precedence getToken(char* symbol, int *n)
{
	*symbol = expr[(*n)++];
	switch (*symbol)
	{
	case'(': return lparen;
	case')': return rparen;
	case'+': return plus;
	case'-': return minus;
	case'/': return divide;
	case'*': return times;
	case'%': return mod;
	case'\0': return eos;
	default: return operand;
	}
}
void printToken(precedence token)
{
	switch (token)
	{
	case lparen: printf("("); break;
	case rparen: printf(")"); break;
	case plus: printf("+"); break;
	case minus: printf("-"); break;
	case divide: printf("/"); break;
	case times: printf("*"); break;
	case mod: printf("%%"); break;
	case eos: printf(" "); break;
	}
	pop();
}

void rExpr(FILE*fp, char*expr)
{
	int i=0;
	while (fscanf(fp,"%c",&expr[i])!=EOF)
		i++;
}