#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <cassert>
void InSort (int arr[], int size)
{
	int i, j, temp;
	for (i = 1; i < size; i++)
	{
		temp = arr[i];
		for (j = i-1; j >= 0; j--)
		{
			if (temp < arr[j])
			{
				arr[j+1] = arr[j];
			}
			else
			{
				break;
			}
		}
		arr[j + 1] = temp;
	}
}

int main ()
{
	FILE *fp;
	time_t start, end;
	fp = freopen("lab1_worst.data", "r",stdin);
	int size,i;
	scanf_s("%d", &size);
	int *arr;
	arr = (int*)malloc (size * sizeof (int));

	for (i = 0; i < size; i++)
	{
		scanf_s("%d", &arr[i]);
	}

	/*printf ("Input : ");
	for (i = 0; i < size; i++)
	{
		printf ("%d ", arr[i]);
	}
	printf ("\n");*/

	start = clock ();
	InSort (arr, size);
	end = clock ();

	/*printf ("Sorted : ");
	for (i = 0; i < size; i++)
	{
		printf ("%d ", arr[i]);
	}
	printf ("\n");*/

	for (int i = 0; i+1 < size; i++) {
		assert (arr[i] <= arr[i + 1]);
	}

	printf ("Time = %.2lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	free(arr);
	fclose(fp);
	return 0;
}