#include<stdio.h>
#include<string.h>
#define max_string_size 100
#define max_pattern_size 100
int failure[max_pattern_size];
int string[max_string_size];
int pat[max_pattern_size];
int KMP (char*, char*);
void fail (char*);

int main ()
{
	FILE*fp;
	fp = fopen ("kmp.txt", "r");
	fgets (string, max_string_size, fp);
	fgets (pat, max_pattern_size, fp);
	fail (pat);
	printf("%d\n",KMP (string, pat));
	fclose (fp);
	return 0;
}

int KMP (char*string, char*pat)
{
	int i = 0, j = 0;
	int lens = strlen (string);
	int lenp = strlen (pat);
	while (i < lens&&j < lenp)
	{
		if (string[i] == pat[j])
		{
			i++;
			j++;
		}
		else if (j == 0)
			i++;
		else
			j = failure[j - 1] + 1;
	}
	return ((j == lenp) ? (i - lenp) : -1);
}

void fail (char*pat)
{
	int n = strlen (pat);
	int i, j;
	failure[0] = -1;
	for (j = 1; j < n; j++)
	{
		i = failure[j - 1];
		while (pat[j] != pat[i + 1] && i >= 0)
			i = failure[i];
		if (pat[j] == pat[i + 1])
			failure[j] = i + 1;
		else
			failure[j] = -1;

		/*for (int k = 0; k < n; k++)
			printf ("%2d ", failure[k]);
		printf ("\n");*/
	}
}
