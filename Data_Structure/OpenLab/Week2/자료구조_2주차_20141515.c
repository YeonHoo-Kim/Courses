#include<stdio.h>
#define MAX_TERMS 100
#define COMPARE(x,y)((x)>(y)? 1:(((x)==(y))? 0:-1))

void padd (int, int, int, int, int*, int*);
void readPoly (int*, int*, int*, int*);
void printPoly (int, int);

typedef struct Poly {
	float coef;
	int exp;
}Poly;
Poly terms[MAX_TERMS];
int avail = 0;

int main ()
{
	int stA = 0, finA = 0, stB = 0, finB = 0, stD = 0, finD = 0;
	readPoly(&stA, &finA, &stB, &finB);
	padd (stA, finA, stB, finB, &stD, &finD);
	printPoly (stD, finD);
	return 0;
}
void padd (int startA, int finA, int startB, int finB, int* startD, int* finD)
{
	int i = 0;
	*startD = avail;
	i = *startD;
	while (startA <= finA && startB <= finB)
	{
		switch (COMPARE (terms[startA].exp, terms[startB].exp))
		{
		case -1:
			terms[i].coef = terms[startB].coef;
			terms[i].exp = terms[startB].exp;
			startB++;
			break;
		case 0:
			terms[i].coef = terms[startA].coef + terms[startB].coef;
			terms[i].exp = terms[startA].exp;
			startA++;
			startB++;
			if (terms[i].coef == 0) {
				i--;
				break;
			}
			break;
		case 1:
			terms[i].coef = terms[startA].coef;
			terms[i].exp = terms[startA].exp;
			startA++;
			break;
		}
		i++;
	}
	while (startA <= finA || startB <= finB) {
		if (startA <= finA)
		{
			terms[i].coef = terms[startA].coef;
			terms[i].exp = terms[startA].exp;
			startA++;
		}
		if (startB <= finB)
		{
			terms[i].coef = terms[startB].coef;
			terms[i].exp = terms[startB].exp;
			startB++;
		}
		i++;
	}
	*finD = i-1;
	avail = i;
}
void readPoly (int* startA, int* finA, int* startB, int* finB)
{
	int i = 0;
	FILE *fp;
	fp = fopen ("A.txt", "r");
	*startA = 0;
	while(fscanf (fp, "%f %d", &terms[i].coef, &terms[i].exp) == 2)
		i++;
	*finA = i - 1;
	fclose (fp);
	fp = fopen ("B.txt", "r");
	*startB = i;
	while (fscanf (fp, "%f %d", &terms[i].coef, &terms[i].exp) == 2)
		i++;
	*finB = i - 1;
	avail = i;
	fclose (fp);
}
void printPoly (int startD, int finD)
{
	int i = 0;
	for (i = startD; i <= finD; i++)
	{
		printf ("(%.0f,%d)",terms[i].coef, terms[i].exp);
		if (i == finD)
			break;
		printf (", ");
	}
}