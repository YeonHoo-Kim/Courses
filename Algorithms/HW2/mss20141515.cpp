#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void SelectionSort(int* arr, int size);
void QuickSort(int* arr, int left, int right);
void HeapSort(int* arr, int size);
void RadixSort(int* arr, int size);

int main(int argc, char* argv[])
{
	int inputSize;
	int* data;
	time_t start, end;
	char resName[40];

	FILE* fp = fopen(argv[1],"r");
	fscanf(fp, "%d", &inputSize);
	data = (int*)malloc(inputSize * sizeof(int));
	for (int i = 0; i < inputSize; i++)
		fscanf(fp, "%d", &data[i]);
	if (atoi(argv[2]) == 1) {
		start = clock();
		SelectionSort(data, inputSize);
		end = clock();
	}
	if (atoi(argv[2]) == 2) {
		start = clock();
		QuickSort(data, 0, inputSize - 1);
		end = clock();
	}
	if (atoi(argv[2]) == 3) {
		start = clock();
		HeapSort(data, inputSize);
		end = clock();
	}
	if (atoi(argv[2]) == 4) {
		start = clock();
		RadixSort(data, inputSize);
		end = clock();
	}

	strcpy(resName, "result_");
	strcat(resName, argv[2]);
	strcat(resName, "_");
	strcat(resName, argv[1]);
	fp = fopen(resName, "w");
	fprintf(fp, "%s\n%s\n",argv[1],argv[2]);
	fprintf(fp, "%d\n%lf\n",inputSize, (double)(end-start)/CLOCKS_PER_SEC);
	for (int i = 0; i < inputSize; i++) {
		fprintf(fp, "%d ", data[i]);
	}

	free(data);
	fclose(fp);
	return 0;
}

void SelectionSort(int* arr, int size) {
	int min, tmp;
	for (int i = 0; i < size - 1; i++) {
		min = i;
		for (int j = i + 1; j < size; j++)
			if(arr[j]<arr[min])
				min = j;
		tmp = arr[min];
		arr[min] = arr[i];
		arr[i] = tmp;
	}
}

void QuickSort(int* arr, int left, int right) {
	int l = left, r = right;
	int p = arr[(left + right) / 2];
	int tmp;
	do {
		while (arr[l] < p)
			l++;
		while (arr[r] > p)
			r--;
		if (l <= r) {
			tmp = arr[l];
			arr[l] = arr[r];
			arr[r] = tmp;
			l++;
			r--;
		}
	} while (l <= r);

	if (left < r)
		QuickSort(arr, left, r);
	if (l < right)
		QuickSort(arr, l, right);
}

void HeapSort(int* arr, int size) {
	//make heap
	for (int i = 1; i < size; i++) {
		int child = i;
		while (child > 0) {
			int root = (child - 1) / 2;
			if (arr[root] < arr[child]) {
				int tmp = arr[root];
				arr[root] = arr[child];
				arr[child] = tmp;
			}
			child = root;
		}
	}
	// sorting
	for (int j = size - 1; j >= 0; j--) {
		int tmp = arr[j];
		arr[j] = arr[0];
		arr[0] = tmp;
		int root = 0;
		int child = 1;
		do {
			child = 2 * root + 1;
			if (child < j - 1 && arr[child] < arr[child + 1])
				child++;
			if (child < j&&arr[root] < arr[child]) {
				tmp = arr[root];
				arr[root] = arr[child];
				arr[child] = tmp;
			}
			root = child;
		} while (child < j);
	}
}

void RadixSort(int* arr, int size) {
	int max = arr[0];
	for (int i = 1; i < size; i++)
		if (arr[i] > max)
			max = arr[i];
	for (long long int digit = 1; digit < max; digit *= 10) {
		int cnt[10] = {0,0,0,0,0,0,0,0,0,0};
		int* tmp = (int*)malloc(size*sizeof(int));
		for (int i = 0; i < size; i++)
			cnt[(arr[i] / digit) % 10]++;
		for (int i = 1; i < 10; i++)
			cnt[i] += cnt[i - 1];
		for (int i = size - 1; i >= 0; i--) {
			int j = (arr[i] / digit) % 10;
			tmp[cnt[j] - 1] = arr[i];
			cnt[j]--;
		}
		for (int i = 0; i < size; i++)
			arr[i] = tmp[i];
		free(tmp);
	}
}
