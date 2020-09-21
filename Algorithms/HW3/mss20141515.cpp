#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_DATATYPE 256						//able type of binarys in 1byte (0b00000000 ~ 0b11111111)

typedef struct DataType{
	unsigned char data;
	int num;									// when option -c : number of data in file, when option -d : size the huffman code
	char code[16];
}DataType;

typedef struct TreeNode{
	DataType d;
	TreeNode *lchild;
	TreeNode *rchild;
}TreeNode;

typedef struct Element{
	TreeNode *pTree;
	int key;
}Element;

typedef struct HeapType{
	Element heap[1000];
	int hsize;
}HeapType;

void insertHeap(HeapType *h, Element i);
Element deleteHeap(HeapType *h);
TreeNode* MakeNode(TreeNode *l, TreeNode *r);
void printTree(TreeNode *p, int i, char *cmpstr, DataType *pd, int pdsize);
void writeData(TreeNode *p, int i, FILE *fp);
void eraseTree(TreeNode *pt);
void huffmanTree(DataType *pd, int n, unsigned char *data, int size, char *fileName);
void findCode(DataType *pd, int pdsize, unsigned char d, FILE *fp);
unsigned char BitToChar(char *str);


int main(int argc, char *argv[]){

	FILE *fp = fopen(argv[2], "rb");
	if (!fp) {								// exception : file open error
		printf("Unable to open file (Input file error)\n");
		exit(1);
	}

	if (strcmp(argv[1], "-c") == 0) {
		
		unsigned char *data;
		DataType *pData;

		int size, i, k = 0, flag = 0, numChar = 0;
		unsigned char j = 0b00000000;
		int element[MAX_DATATYPE] = { 0 };

		fseek(fp, 0, SEEK_END);					// finding file size
		size = ftell(fp);						// size of file
		rewind(fp);								// move file pointer to the head of the file

		data = (unsigned char*)malloc(size + 1);
		//printf("%d\n", size);

		fread(data, size, 1, fp);				// read data from file
		
		for (i = 0; i < size; i++) {			// get elements(counts) of each characters (j)
			j = 0;
			for (;;) {
				if (data[i] == j) {
					element[j]++;
					break;
				}
				j++;
				if (j == 0b11111111) {
					if (data[i] == j) {
						element[j]++;
						break;
					}
					break;
				}
			}
		}								

		pData = (DataType*)malloc(MAX_DATATYPE * sizeof(DataType));

		for (i = 0; i < MAX_DATATYPE; i++) {
			if (element[i] != 0) {
				pData[k].data = (unsigned char)i;
				pData[k].num = element[i];
				k++;
			}
		}

		//printf("\n< Character frequency >\n");
		/*for (i = 0; i < k; i++)
			printf("%3d\t%3d\n", pData[i].data, pData[i].num);*/

		//printf("\n< Huffman Code >\n");
		huffmanTree(pData, k, data, size, argv[2]);

		free(pData);
		free(data);
	}
	else if (strcmp(argv[1], "-d") == 0) {

		unsigned char tmp;
		unsigned char *strTmp;
		char pad_size, code[10];
		short bit, zero=0;
		int num,q,r,size, fsize;
		DataType *HuffmanTable;
		char *str, *strTemp;
		char *fName;

		fName = (char*)malloc((strlen(argv[2]) + 5) * sizeof(char));	// compressed file name allocation 
		strcpy(fName, "\0");
		strcat(fName, argv[2]);
		strcat(fName, ".yy");											// Original : fileName.zz -> Compressed file : fileName.zz.yy

		fseek(fp, 0, SEEK_END);				// finding file size
		size = ftell(fp);
		rewind(fp);							// move file pointer to the head of the file

		fread(&tmp, sizeof(unsigned char),1,fp);
		size--;

		num = (int)tmp + 1;

		HuffmanTable = (DataType*)malloc(num * sizeof(DataType));
		
		//printf("\n< Huffman Table >\n");
		//printf("\nNumber of Tables : %d\n", num);

		for (int i = 0; i < num; i++) {						// read data from file
			fread(&tmp, sizeof(unsigned char), 1, fp);
			HuffmanTable[i].data = tmp;						// 0~255 unsigned character
			fread(&tmp, sizeof(unsigned char), 1, fp);
			HuffmanTable[i].num = (int)tmp;					// size of Huffman code
			fread(&bit, sizeof(short), 1, fp);
			strcpy(HuffmanTable[i].code, "\0");				// initialize str

			size -= 4;

			q = (int)bit;
			HuffmanTable[i].code[HuffmanTable[i].num] = '\0';	// initialize str
			for (int j = HuffmanTable[i].num-1; j >= 0; j--){	// making Huffman code
				HuffmanTable[i].code[j] = (q % 2)+'0';
				q /= 2;
			}
			//printf("%3d\t%3d\t%s\n", (int)HuffmanTable[i].data, HuffmanTable[i].num, HuffmanTable[i].code);
		}
		
		fread(&pad_size, sizeof(char), 1, fp);				// get padsize : how many bits added to make a byte string when compressing data
		pad_size -= '0';
		size--;

		strTmp = (unsigned char*)malloc(size * sizeof(unsigned char));				// read Huffman Code string from file;
		for (int i = 0; i < size; i++) {
			fread(&tmp, sizeof(unsigned char), 1, fp);
			strTmp[i] = tmp;
		}
		fp = fopen(fName, "wb");
		if (!fp) {													// exception : file open error
			printf("Unable to open file (Input file error)\n");
			exit(1);
		}

		for (int i = 0; i < size; i++) {							// binary data -> huffman code in 1byte
			q = (int)strTmp[i];
			if (i == size - 1) {
				code[8 - (int)pad_size] = '\0';
				for (int j = 0; j < pad_size; j++)
					q /= 2;
				for (int j = 7 - (int)pad_size; j >= 0; j--) {
					code[j] = (q % 2) + '0';
					q /= 2;
				}
			}
			else {
				code[8] = '\0';
				for (int j = 7; j >= 0; j--) {
					code[j] = (q % 2) + '0';
					q /= 2;
				}
			}
			fprintf(fp, "%s", code);
		}
		fclose(fp);

		fp = fopen(fName, "rb");
		if (!fp) {													// exception : file open error
			printf("Unable to open file (Input file error)\n");
			exit(1);
		}

		fseek(fp, 0, SEEK_END);				// finding file size
		fsize = ftell(fp);					
		rewind(fp);							// move file pointer to the head of the file

		str = (char*)malloc((fsize+1) * sizeof(char));
		strcpy(str, "\0");

		fscanf(fp, "%s", str);

		fp = fopen(fName, "wb");
		if (!fp) {							// exception : file open error
			printf("Unable to open file (Input file error)\n");
			exit(1);
		}

		strTemp = str;
		for (int j = (strlen(str)); j > 0;) {
			for (int i = 0; i < num; i++) {
				if (strncmp(strTemp, HuffmanTable[i].code, strlen(HuffmanTable[i].code)) == 0) {	// Decoding from huffman code to Original data
					fwrite(&HuffmanTable[i].data, sizeof(char), 1, fp);
					strTemp += strlen(HuffmanTable[i].code);
					j -= strlen(HuffmanTable[i].code);
					break;
				}
			}
		}
		free(str);
		free(strTmp);

		printf("\nDecompression Successfully Done!\n");
	}
	else {
		printf("Error : First options must be -c(compression) or -d(decompression)\n");
		return -1;
	}

	fclose(fp);
	
	return 0;
}

void insertHeap(HeapType *h, Element i) {
	int k = ++(h->hsize);
	
	while (k != 1 && i.key < h->heap[k / 2].key) {		// comparing with parent node
		h->heap[k] = h->heap[k / 2];
		k /= 2;
	}

	h->heap[k] = i;										// inserting element (min heap)
}

Element deleteHeap(HeapType *h) {
	int p = 1, c = 2;
	Element data, tmp;

	data = h->heap[p];
	tmp = h->heap[(h->hsize)--];						// deleting heap makes heapSize--

	while (c <= h->hsize) {
		if ((c < h->hsize) && (h->heap[c].key) > h->heap[c + 1].key)		
			c++;

		if (tmp.key <= h->heap[c].key)					
			break;

		h->heap[p] = h->heap[c];
		p = c;	
		c *= 2;											// compare with next child node
	}

	h->heap[p] = tmp;
	return data;										// return deleted element
}

TreeNode* MakeNode(TreeNode *l, TreeNode *r) {	
	TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));

	if (node == NULL) {
		printf("Memory Allocation Error\n");
		exit(-1);
	}
	
	node->lchild = l;
	node->rchild = r;												// node connect

	return node;
}

void printTree(TreeNode *p, int i, char *cmpstr, DataType *pd, int pdsize) {		// printing information
	if (p != NULL) {
		i++;
		cmpstr[i] = '1';
		printTree(p->lchild, i, cmpstr, pd, pdsize);
		cmpstr[i] = '0';
		printTree(p->rchild, i, cmpstr, pd, pdsize);
		cmpstr[i] = '\0';

		if (p->lchild == NULL && p->rchild == NULL) {
			strcpy(p->d.code, cmpstr);
			for (int j = 0; j < pdsize; j++) {										// insert values to pd
				if (pd[j].data == p->d.data) {
					strcpy(pd[j].code, cmpstr);
					break;
				}
			}
			//printf("%3d\t%3d\t%s\n", (int)p->d.data, p->d.num, p->d.code);
		}
	}
}

void writeData(TreeNode *p, int i, FILE *fp) {		// writing information
	if (p != NULL) {
		i++;
		writeData(p->lchild, i, fp);
		writeData(p->rchild, i, fp);

		if (p->lchild == NULL && p->rchild == NULL) {

			char *b = (char*)malloc(20 * sizeof(char));
			strcpy(b, "\0");
			short tbyte = 0;
			char pad = 0;

			if (strlen(p->d.code) < 16) {
			for (int j = 0; j < 16 - strlen(p->d.code); j++) {
			strcat(b, "0");
			}
			strcat(b, p->d.code);
			}

			char *bTemp = b;

			for (int j = 15; j >= 0; j--) {
			tbyte |= (*bTemp - '0') << j;
			bTemp++;
			}

			fprintf(fp, "%c%c", p->d.data, (char)strlen(p->d.code));
			fwrite(&tbyte, sizeof(short), 1, fp);
			free(b);
		}
	}
}

void findCode(DataType *pd, int pdsize, unsigned char d, FILE *fp) {					//find the right code for data[i]
	for (int i = 0; i < pdsize; i++) {
		if (pd[i].data == d) {
			fprintf(fp, "%s", pd[i].code);
			break;
		}
	}
}

unsigned char BitToChar(char *str) {					// char data of bit information -> char data (8byte->1byte)
	unsigned char tmp = 0;								
	for (int j = 7; j >= 0; j--) {
		tmp |= (*str - '0') << j;
		str++;
	}
	return tmp;
}


void eraseTree(TreeNode *pt) {
	if (pt == NULL)
		return;

	eraseTree(pt->lchild);
	eraseTree(pt->rchild);

	free(pt);											// erase tree node(free)

	return;
}

void huffmanTree(DataType *pd, int n, unsigned char *data, int size, char *fileName) {
	TreeNode *node, *tmp;
	Element e, e1, e2;
	HeapType h;
	unsigned char num;
	char huffmanCode[16];
	char code[10];
	char *fName;
	char *str;
	int fsize;
	int padding_size;												// Number of bits needed to make a byte on the end of bitstring(str)
	char *strTmp;

	fName = (char*)malloc((strlen(fileName) + 5) * sizeof(char));	// compressed file name allocation

	strcpy(fName, fileName);	
	strcat(fName, ".zz");								// Original : fileName -> Compressed file : fileName.zz

	FILE *fp = fopen(fName, "wb");
	if (!fp) {							// exception : file open error
		printf("Unable to open file (Input file error)\n");
		exit(1);
	}
														
	h.hsize = 0;

	for (int i = 0; i < n; i++) {						
		node = MakeNode(NULL, NULL);
		node->d.data = pd[i].data;
		node->d.num = pd[i].num;
		e.key = node->d.num;
		e.pTree = node;									// put head node to tree
		insertHeap(&h, e);
	}

	for (int i = 0; i < n - 1; i++) {
		e1 = deleteHeap(&h);
		e2 = deleteHeap(&h);							// extract two minimum nodes
		
		tmp = MakeNode(e1.pTree, e2.pTree);				// make node which has two nodes

		tmp->d.num = e1.key + e2.key;					// add two nodes
		e.key = tmp->d.num;
		e.pTree = tmp;									

		insertHeap(&h, e);								// insert node to heap
	}

	e = deleteHeap(&h);

	strcpy(huffmanCode, "\0");
	printTree(e.pTree, -1, huffmanCode, pd, n);				// Printing Process and Setting Huffman code to each of characters

	for (int i = 0; i < size; i++)
		findCode(pd, n, data[i], fp);					// find the right code for data[i] and write huffman code string on file
	
	fclose(fp);

	fp = fopen(fName, "rb");							// read Huffman Code String from file
	if (!fp) {											// exception : file open error
		printf("Unable to open file (Input file error)\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);								// finding file size
	fsize = ftell(fp);
	rewind(fp);											// move file pointer to the head of the file

	str = (char*)malloc((fsize+8) * sizeof(char));		// memory allocate considering the padding_size(less than 8) as well
	strcpy(str, "\0");														
	fscanf(fp, "%s", str);								// str : huffman code string(by character)

	fp = fopen(fName, "wb");							// re-open file to write compressed data
	if (!fp) {											// exception : file open error
		printf("Unable to open file (Input file error)\n");
		exit(1);
	}

	num = (unsigned char)(n - 1);
	fwrite(&num, sizeof(unsigned char), 1, fp);

	writeData(e.pTree, -1, fp);

	padding_size = (8 - (fsize % 8)) % 8;
	fprintf(fp, "%c", padding_size+'0');							// write padding_size data

	for (int i = 0; i < padding_size; i++)							// add 0 to string to make a byte for char
		strcat(str, "0");

	strTmp = str;													// strTmp = point str
	for (int i = 0; i*8 < fsize; i++) {
		unsigned char byte = BitToChar(strTmp);						// get 8bits to 1byte
		fwrite(&byte,sizeof(unsigned char), 1, fp);
		strTmp += 8;
	}
	
	eraseTree(e.pTree);					

	fclose(fp);

	fp = fopen(fName, "rb");
	if (!fp) {														// exception : file open error
		printf("Unable to open file (Input file error)\n");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);											// finding file size
	fsize = ftell(fp);
	rewind(fp);														// move file pointer to the head of the file

	free(fName);
	fclose(fp);
	free(str);														// free memory

	printf("\nOriginal file size : %d\n", size);
	printf("Compressed file size : %d\n", fsize);
	printf("Compression rate : %f\n", ((float)size / (float)fsize));

}
