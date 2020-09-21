#include<stdio.h>
#include<time.h>

void Hanoi(int, char, char ,char);
void Move(char, char);

int main()
{
	int n;
	time_t start, end;
	printf("n = ");
	scanf("%d", &n);
	start = clock();
	Hanoi(n,'A','C','B');
	end = clock();
	printf("\n\nTime : %.0lf ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000);
	return 0;
}

void Hanoi(int n, char start, char through, char end)
{
	if (n == 1)
	{
		printf("원판 1 : ");
		Move(start, end);
	}
	else
	{
		Hanoi(n - 1, start, end, through);
		printf("원판 %d : ",n);
		Move(start, end);
		Hanoi(n - 1, through, start, end);
	}
}

void Move(char start, char end)
{
	printf("%c -> %c\n", start, end);
}