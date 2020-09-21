#include<stdio.h>
#include<stdlib.h>

#define MAX_SIZE 50

typedef enum { head, entry } tagfield;
typedef struct matrix_node* matrix_pointer;
typedef struct entry_node {
	int row;
	int col;
	int value;
}entry_node;

typedef struct matrix_node {
	matrix_pointer down;
	matrix_pointer right;
	tagfield tag;
	union {
		matrix_pointer next;
		entry_node entry;
	} u;
}matrix_node;

matrix_pointer hdnode[MAX_SIZE];
matrix_pointer mread(FILE*);
void mwrite(matrix_pointer);
void merase(matrix_pointer*);

void main()
{
	matrix_pointer a, b;
	FILE*fp;
	fp = fopen("A.txt", "r");
	a = mread(fp);
	mwrite(a);
	printf("\n");
	fp = fopen("B.txt", "r");
	b = mread(fp);
	mwrite(b);
	merase(&a);
	merase(&b);
}

matrix_pointer mread(FILE* fp)
{
	int numRows, numCols, numHeads;
	int i, j;
	int row, col, value, currentRow;
	matrix_pointer temp, last, node;

	fscanf(fp, "%d %d", &numRows, &numCols);
	numHeads = (numCols>numRows) ? numCols : numRows;
	/*��� ��� ����Ʈ�� ���� ��� ��带 �����Ѵ�.*/
	node = (matrix_pointer)malloc(sizeof(matrix_node));
	node->tag = entry;
	node->u.entry.row = numRows;	node->u.entry.col = numCols;

	if (!numHeads)	node->right = node;
	else { /*��� ������ �ʱ�ȭ�Ѵ�.*/
		for (i = 0; i < numHeads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix_node));
			hdnode[i] = temp;	hdnode[i]->tag = head;
			hdnode[i]->right = temp;	hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0]; // ���� ���� ������ ���

						  /* numTerms ���� ���⿡ ���� �ϳ��ϳ� 0�� �ƴ� value ������ ��� �� ���� �ִ��� �ľ�*/
		for (i = 0; i<numRows; i++) {
			for (j = 0; j<numCols; j++) {
				fscanf(fp, "%d", &value); //value �� �ľ�
				row = i;	col = j;

				if (row>currentRow) {// value�� ���� ���� row�� ���� row(currentRow) ���� ũ��
									 //row�� �ٲ�����Ƿ� currentRow�� 0�� �ƴѰ����� ����. ���� �������� ������ ��带 ����;
					last->right = hdnode[currentRow];
					//currentRow�� ���� row�� ����, ������ ��� ����
					currentRow = row; last = hdnode[row];
				}
				if (value != 0) {// value�� 0�� �ƴϸ� ��忡 �����ؾ� ��
					temp = (matrix_pointer)malloc(sizeof(matrix_node));
					temp->tag = entry;//tag = ��
					temp->u.entry.row = row;	temp->u.entry.col = col;	temp->u.entry.value = value;//�� �� �� ���� 
					last->right = temp; last = temp;//�� ����Ʈ ����
					hdnode[col]->u.next->down = temp;	hdnode[col]->u.next = temp;//�� ����Ʈ ����
				}
			}
		}
		last->right = hdnode[currentRow];//������ �� ����
										 /*��� �� ����Ʈ ����*/
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		/*��� ��� ������ ����*/
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
	}
	fclose(fp);
	return node;
}

void mwrite(matrix_pointer node)
{
	int i, j;
	matrix_pointer temp, head;

	if (node != NULL) {
		for (i = 0; i<node->u.entry.row; i++) {
			head = node->right;
			for (i = 0; i<node->u.entry.row; i++) {
				j = 0;
				for (temp = head->right; temp != head; temp = temp->right) {
					//printf("%d  %d  %d\n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
					if (j == 0) {
						for (j = 0; j < temp->u.entry.col; j++)
							printf("0 ");
					}
					else {
						for (; j < temp->u.entry.col; j++)
							printf("0 ");
					}
					j = temp->u.entry.col+1;
					printf("%d ", temp->u.entry.value);
				}
				for (; j < node->u.entry.col; j++)
					printf("0 ");
				head = head->u.next;
				printf("\n");
			}
		}
	}
}

void merase(matrix_pointer* node)
{
	matrix_pointer x, y, head = (*node)->right;
	int i, numHeads;
	/* ��Ʈ�� ���� ��� ������ �� �켱���� ��ȯ�Ѵ�. */
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y!= head) {
			x = y;	y = y->right;	free(x);
		}
		x = head;	head = head->u.next;	free(x);
	}
	/* ������ ��� ������ ��ȯ�Ѵ�. */
	y = head;
	while (y != *node) {
		x = y;	y = y->u.next;	free(x);
	}
	free(*node);	*node = NULL;
}