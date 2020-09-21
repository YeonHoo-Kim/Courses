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
	/*헤더 노드 리스트에 대한 헤더 노드를 생성한다.*/
	node = (matrix_pointer)malloc(sizeof(matrix_node));
	node->tag = entry;
	node->u.entry.row = numRows;	node->u.entry.col = numCols;

	if (!numHeads)	node->right = node;
	else { /*헤더 노드들을 초기화한다.*/
		for (i = 0; i < numHeads; i++) {
			temp = (matrix_pointer)malloc(sizeof(matrix_node));
			hdnode[i] = temp;	hdnode[i]->tag = head;
			hdnode[i]->right = temp;	hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0]; // 현재 행의 마지막 노드

						  /* numTerms 값이 없기에 직접 하나하나 0이 아닌 value 값들이 어느 행 열에 있는지 파악*/
		for (i = 0; i<numRows; i++) {
			for (j = 0; j<numCols; j++) {
				fscanf(fp, "%d", &value); //value 값 파악
				row = i;	col = j;

				if (row>currentRow) {// value를 비교할 다음 row가 현재 row(currentRow) 보다 크면
									 //row가 바뀌었으므로 currentRow의 0이 아닌값들은 끝남. 따라서 현재행의 마지막 노드를 설정;
					last->right = hdnode[currentRow];
					//currentRow를 다음 row로 변경, 마지막 노드 갱신
					currentRow = row; last = hdnode[row];
				}
				if (value != 0) {// value가 0이 아니면 노드에 포함해야 됨
					temp = (matrix_pointer)malloc(sizeof(matrix_node));
					temp->tag = entry;//tag = 값
					temp->u.entry.row = row;	temp->u.entry.col = col;	temp->u.entry.value = value;//행 열 값 저장 
					last->right = temp; last = temp;//행 리스트 연결
					hdnode[col]->u.next->down = temp;	hdnode[col]->u.next = temp;//열 리스트 연결
				}
			}
		}
		last->right = hdnode[currentRow];//마지막 행 종료
										 /*모든 열 리스트 종료*/
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		/*모든 헤더 노드들을 연결*/
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
	/* 엔트리 노드와 헤더 노드들을 행 우선으로 반환한다. */
	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y!= head) {
			x = y;	y = y->right;	free(x);
		}
		x = head;	head = head->u.next;	free(x);
	}
	/* 나머지 헤더 노드들을 반환한다. */
	y = head;
	while (y != *node) {
		x = y;	y = y->u.next;	free(x);
	}
	free(*node);	*node = NULL;
}