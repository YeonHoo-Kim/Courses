#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int mss1(const int A[], int N);
int mss2(const int A[], int N);
static int mss2_LF(const int A[], int Left, int Right);
int mss3(const int A[], int N);
int indexLeft = 0;
int indexRight = 0;
int MaxSum = 0;

int main(int argc, char* argv[])
{
	char resName[30];
	int* data;
	int inputSize;
	int maxSum;
	time_t start,end;
	int i;

	FILE* fp = fopen(argv[1], "r");						// open file
	fscanf(fp, "%d", &inputSize);						// get inputSize
	data = (int*)malloc(inputSize * sizeof(int));		// data memory allocate
	for (i = 0; i<inputSize; i++) {						
		fscanf(fp, "%d", &data[i]);						// save inputfile's data to data[i];
	}
	if (atoi(argv[2]) == 1) {							// use algorithms by argv[2] value
		start = clock();
		maxSum = mss1(data, inputSize);
		end = clock();
	}
	else if (atoi(argv[2]) == 2) {
		start = clock();
		maxSum = mss2(data, inputSize);
		end = clock();
	}
	else if (atoi(argv[2]) == 3) {
		start = clock();
		maxSum = mss3(data, inputSize);
		end = clock();
	}
	else{
		printf("Please input(1~3)\n");
		free(data);
		fclose(fp);
		return -1;
	}
	strcpy(resName, "result_");							
	strcat(resName, argv[1]);							// make the output file name result_input...
	fp = fopen(resName, "w");
	
	fprintf(fp, "%s\n%s\n", argv[1],argv[2]);			// input file name, algorithm index 
	fprintf(fp, "%d\n%d\n%d\n%d\n%lf\n",inputSize, indexLeft, indexRight, maxSum, (double)(end-start)); 
	// input size, index leftmost, index rightmost, max sum of subsequence, running time in milliseconds

	free(data);
	fclose(fp);
	return 0;
}

int mss1(const int A[], int N) {
	int ThisSum, MaxSum, i, j;
	int left = 0, right = N;

	MaxSum = 0;
	for (i = 0; i<N; i++) {
		ThisSum = 0;
		for (j = i; j<N; j++) {
			ThisSum += A[j];
			if (ThisSum > MaxSum) {					// if the present sum of sequence is larger than the previous one
				MaxSum = ThisSum;					// save the maximum sum of sequence
				left = i;	right = j;				// get left right index
			}
		}
	}
	indexLeft = left;	indexRight = right;
	return MaxSum;
}

int mss2(const int A[], int N) {
	return mss2_LF(A, 0, N - 1);
}

static int mss2_LF(const int A[], int Left, int Right) {
	int MaxLeftSum, MaxRightSum;
	int MaxLeftBorderSum, MaxRightBorderSum;
	int LeftBorderSum, RightBorderSum;
	int Center, i;
	int left = indexLeft, right = indexRight;

	if (Left == Right) /*Base Case*/
		if (A[Left] > 0)
			return A[Left];
		else
			return 0;

	Center = (Left + Right) / 2;
	MaxLeftSum = mss2_LF(A, Left, Center);
	MaxRightSum = mss2_LF(A, Center + 1, Right);

	MaxLeftBorderSum = 0; LeftBorderSum = 0;
	for (i = Center; i >= Left; i--) {				// find the largest subsequence from Center ~ Left
		LeftBorderSum += A[i];
		if (LeftBorderSum > MaxLeftBorderSum) {		// save the maximum sum of subsequence in MaxLeftBorderSum
			MaxLeftBorderSum = LeftBorderSum;
			left = i;								// get left index
		}
	}

	MaxRightBorderSum = 0; RightBorderSum = 0;
	for (i = Center + 1; i <= Right; i++) {			// find the largest subsequence from Center+1 ~ Right
		RightBorderSum += A[i];
		if (RightBorderSum > MaxRightBorderSum) {	// save the maximum sum of subsequence in MaxRightBorderSum
			MaxRightBorderSum = RightBorderSum;
			right = i;								// get right index
		}
	}
	// return the largest one between MaxLeftSum, MaxRightSum, MaxLeftBorderSum+MaxRightBorderSum
	// and save the left and right index when the return value is largest compare to the previous sums
	if (MaxLeftSum >= (MaxLeftBorderSum + MaxRightBorderSum) && MaxLeftSum >= MaxRightSum) {
		if (MaxLeftSum > MaxSum) {
			indexLeft = left; indexRight = Center;
			MaxSum = MaxLeftSum;
		}
		return MaxLeftSum;
	}
	if (MaxRightSum >= (MaxLeftBorderSum + MaxRightBorderSum) && MaxLeftSum < MaxRightSum) {
		if (MaxRightSum > MaxSum) {
			indexLeft = Center + 1; indexRight = right;
			MaxSum = MaxRightSum;
		}
		return MaxRightSum;
	}
	if (MaxLeftSum < (MaxLeftBorderSum + MaxRightBorderSum) && (MaxLeftBorderSum + MaxRightBorderSum) > MaxRightSum) {
		if ((MaxLeftBorderSum + MaxRightBorderSum) > MaxSum) {
			indexLeft = left; indexRight = right;
			MaxSum = MaxLeftBorderSum + MaxRightBorderSum;
		}
		return MaxLeftBorderSum + MaxRightBorderSum;
	}
}

int mss3(const int A[], int N) {
	int ThisSum, MaxSum, j;
	int left = 0, right = N;

	ThisSum = MaxSum = 0;
	for (j = 0; j<N; j++) {
		ThisSum += A[j];
		if (ThisSum > MaxSum) { // if the present sum of subsequence is larger than the previous one
			MaxSum = ThisSum; 	// change
			right = j; 			// save right index
		}
		else if (ThisSum < 0) { // if the sum of subsequence is negative
			ThisSum = 0;		// throw away
			left = j + 1;		// start fresh(subsequence restarting from j+1)
		}
	}
	indexLeft = left; indexRight = right; // get left, right index
	return MaxSum;
}
