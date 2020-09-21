#include<stdio.h>
#include<stdlib.h>

void printArr(int**, int);
void snailArr(int**, int);

int main()
{
	int n, i, **arr;
	printf("INPUT N : ");
	scanf("%d", &n);
	arr = (int**)malloc(n * sizeof(int*));
	for (i = 0; i < n; i++) {
		*(arr + i) = (int*)malloc(n * sizeof(int));
	}
	snailArr(arr, n);
	printArr(arr, n);
	for (i = 0; i < n; i++) {
		free(*(arr + i));
	}
	free(arr);
	return 0;
}

void snailArr(int **arr, int size)
{
	int num = 1, i = 0, j = 0, k = 0, sCase;
	sCase = size;
	while (num <= size * size)
	{	
		if (sCase < 0)
		{
			printf("Wrong input (n must be positive integer)\n");
			break;
		}
		if (sCase == size)
		{
			for (; j < sCase; j++)
			{
				arr[i][j] = num;
				num++;
			}
			j--;
			sCase--;
		}
		else
		{
			for (k = 0; k < sCase; k++)
			{
				i++;
				arr[i][j] = num;
				num++;
			}
			for (k = 0; k < sCase; k++)
			{
				j--;
				arr[i][j] = num;
				num++;
			}
			sCase--;
			for (k = 0; k < sCase; k++)
			{
				i--;
				arr[i][j] = num;
				num++;
			}
			for (k = 0; k < sCase; k++)
			{
				j++;
				arr[i][j] = num;
				num++;
			}
			sCase--;
		}
	}
}

void printArr(int **arr, int size)
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			printf("%d\t", arr[i][j]);
		}
		printf("\n");
	}
}