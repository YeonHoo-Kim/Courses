#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"list.h"
#include"hash.h"
#include"bitmap.h"

struct LD {
	struct list_elem elem;
	int data;
};

struct HD {
	struct hash_elem elem;
	int data;
};

unsigned hash_hash_function(const struct hash_elem *e, void *aux);
bool hash_less_function(const struct hash_elem *a, const struct hash_elem *b, void *aux);
bool list_less_function(const struct list_elem *a, const struct list_elem *b, void *aux);
void hash_action_func_square(struct hash_elem *a, void *aux);
void hash_action_func_triple(struct hash_elem *a, void *aux);

struct list* lists[20];
struct hash* hashs[20];
struct bitmap* bitmaps[20];
int listCnt = 0;
int hashCnt = 0;
int bitmapCnt = 0;
char* listName[20];
char* hashName[20];
char* bitmapName[20];


int main() {
	char buf[100];
	char* token;
	int i, j, k, bufSize;
	for (i = 0; i < 20; i++) {							// 20 kinds, each namelen = 30
		listName[i] = (char*)malloc(30 * sizeof(char));
		hashName[i] = (char*)malloc(30 * sizeof(char));
		bitmapName[i] = (char*)malloc(30 * sizeof(char));
	}
	while (1) {
		for (i = 0; i < 100; i++)
			buf[i] = 0;
		fflush(stdin);
		fgets(buf, sizeof(buf), stdin);
		bufSize = strlen(buf);
		if (bufSize == 0)
			continue;
		else {
			buf[bufSize - 1] = '\0';
			token = strtok(buf, " ");
			/* basic common commands */
			if (strcmp(token, "create") == 0) {				// create (list, hashtable, bitmap)
				char *data_str, *name;
				int errorflag = 0;
				data_str = strtok(NULL, " ");
				name = strtok(NULL, " ");
				for (i = 0; i < listCnt; i++) {				// namecheck
					if (strcmp(name, listName[i]) == 0 && strcmp(data_str, "list") == 0) {
						printf("Error : Same name already exists in list\n");
						errorflag = 1;
						break;
					}
				}
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0 && strcmp(data_str, "hashtable") == 0) {
						printf("Error : Same name already exists in hashtable\n");
						errorflag = 1;
						break;
					}
				}
				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0 && strcmp(data_str, "bitmap") == 0) {
						printf("Error : Same name already exists in bitmap\n");
						errorflag = 1;
						break;
					}
				}
				if (data_str == NULL || name == NULL || errorflag == 1)		// error
					continue;
				if (strcmp(data_str, "list") == 0) {		// create list
					struct list* tmpList;
					tmpList = (struct list*)malloc(sizeof(struct list));
					lists[listCnt] = tmpList;
					list_init(lists[listCnt]);
					strcpy(listName[listCnt], name);
					listCnt++;

				}
				if (strcmp(data_str, "hashtable") == 0) {	// create hashtable
					struct hash* tmpHash;
					tmpHash = (struct hash*)malloc(sizeof(struct hash));
					hashs[hashCnt] = tmpHash;
					hash_init(hashs[hashCnt], hash_hash_function, hash_less_function, NULL);
					strcpy(hashName[hashCnt], name);
					hashCnt++;
				}
				if (strcmp(data_str, "bitmap") == 0) {		// create bitmap
					struct bitmap* tmpBitmap;
					int bitNum;
					char *bit_num;
					bit_num = strtok(NULL, " ");
					bitNum = atoi(bit_num);
					tmpBitmap = (struct bitmap*)bitmap_create(bitNum);
					bitmaps[bitmapCnt] = tmpBitmap;
					strcpy(bitmapName[bitmapCnt], name);
					bitmapCnt++;
				}
				continue;
			}
			if (strcmp(token, "dumpdata") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : dumpdata\n");
					continue;
				}
				struct list_elem *lidx;
				struct hash_iterator hidx;
				for (i = 0; i < listCnt; i++) {				// searching data in list
					if (strcmp(name, listName[i]) == 0) {	// found name
						for (lidx = list_begin(lists[i]); lidx != list_end(lists[i]); lidx = list_next(lidx)) {
							struct LD *tmpl;
							tmpl = list_entry(lidx, struct LD, elem);
							printf("%d ", tmpl->data);
						}									// printout
						if (list_size(lists[i]) != 0)
							printf("\n");
						break;
					}
				}
				for (i = 0; i < hashCnt; i++) {				// searching data in hashtable
					if (strcmp(name, hashName[i]) == 0) {	// found name
						hash_first(&hidx, hashs[i]);
						while (hash_next(&hidx) != NULL) {	// printout
							struct HD *tmph;
							tmph = hash_entry(hash_cur(&hidx), struct HD, elem);
							printf("%d ", tmph->data);
						}
						if (hash_size(hashs[i]) != 0)
							printf("\n");
						break;
					}
				}
				for (i = 0; i < bitmapCnt; i++) {			// searching data in bitmap
					if (strcmp(name, bitmapName[i]) == 0) {	// found name
						size_t bsize = bitmap_size(bitmaps[i]);
						for (j = 0; j < bsize; j++) {
							if (bitmap_test(bitmaps[i], j) == true)	// printout
								printf("1");
							else
								printf("0");
						}
						if (bitmap_size(bitmaps[i]) != 0)
							printf("\n");
						break;
					}
				}
			}
			if (strcmp(token, "delete") == 0) {
				char *name;
				int namelen;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : delete\n");
					continue;
				}
				namelen = strlen(name);
				for (i = 0; i < listCnt; i++) {				// searching data in list
					if (strcmp(name, listName[i]) == 0) {	// found name
						for (j = 0; j < namelen; j++)		// delete
							listName[i][j] = 0;
						if (i < listCnt - 1 && listCnt > 1) { // don't care if listCnt = 0,1 or it is a last list data
							lists[i] = lists[listCnt - 1];
							strcpy(listName[i], listName[listCnt - 1]);
						}									// move list data from listCnt-1 to i
						listCnt--;
						break;
					}
				}
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {	// found name
						for (j = 0; j < namelen; j++)		// delete
							hashName[i][j] = 0;
						if (i < hashCnt - 1 && hashCnt > 1) { // don't care if hashCnt = 0,1 or it is a last hashtable data
							hashs[i] = hashs[hashCnt - 1];
							strcpy(hashName[i], hashName[hashCnt - 1]);
						}									// move hasttable data from hasCnt-1 to i
						hashCnt--;
						break;
					}
				}
				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {	// found name
						for (j = 0; j < namelen; j++)		// delete
							bitmapName[i][j] = 0;
						if (i < bitmapCnt - 1 && bitmapCnt > 1) { // don't care if bitmapCnt = 0,1 or it is a last bitmap data
							bitmaps[i] = bitmaps[bitmapCnt - 1];
							strcpy(bitmapName[i], bitmapName[bitmapCnt - 1]);
						}									// move bitmap data from listCnt-1 to i
						bitmapCnt--;
						break;
					}
				}
			}
			if (strcmp(token, "quit") == 0)
				break;
			/* list commands */
			if (strcmp(token, "list_back") == 0) {
				char* name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_back\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						struct LD *tmpl;
						tmp = list_back(lists[i]);
						tmpl = list_entry(tmp, struct LD, elem);
						printf("%d\n", tmpl->data);
						break;
					}
				}
			}
			if (strcmp(token, "list_front") == 0) {
				char* name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_front\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						struct LD *tmpl;
						tmp = list_front(lists[i]);
						tmpl = list_entry(tmp, struct LD, elem);
						printf("%d\n", tmpl->data);
						break;
					}
				}
			}
			if (strcmp(token, "list_push_back") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : list_push_back\n");
					continue;
				}
				val = atoi(data);
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct LD *tmpl;
						tmpl = (struct LD*)malloc(sizeof(struct LD));
						tmpl->data = val;
						list_push_back(lists[i], &(tmpl->elem));
						break;
					}
				}
			}
			if (strcmp(token, "list_push_front") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : list_push_front\n");
					continue;
				}
				val = atoi(data);
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct LD *tmpl;
						tmpl = (struct LD*)malloc(sizeof(struct LD));
						tmpl->data = val;
						list_push_front(lists[i], &(tmpl->elem));
						break;
					}
				}
			}
			if (strcmp(token, "list_pop_back") == 0) {
				char* name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_pop_back\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						tmp = list_pop_back(lists[i]);
						break;
					}
				}
			}
			if (strcmp(token, "list_pop_front") == 0) {
				char* name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_pop_front\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						tmp = list_pop_front(lists[i]);
						break;
					}
				}
			}
			if (strcmp(token, "list_insert") == 0) {
				char *name, *data1, *data2;
				int idx, val;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL) {
					printf("Error : list_insert\n");
					continue;
				}
				idx = atoi(data1);
				val = atoi(data2);

				struct list_elem *tmp;
				struct LD *tmpl;
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						tmp = list_begin(lists[i]);
						tmpl = (struct LD*)malloc(sizeof(struct LD));
						tmpl->data = val;
						break;
					}
				}
				for (i = 0; i < idx; i++)
					tmp = list_next(tmp);
				list_insert(tmp, &(tmpl->elem));
			}
			if (strcmp(token, "list_insert_ordered") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : list_insert_ordered\n");
					continue;
				}
				val = atoi(data);
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct LD *tmpl;
						tmpl = (struct LD*)malloc(sizeof(struct LD));
						tmpl->data = val;
						list_insert_ordered(lists[i], &(tmpl->elem), list_less_function, NULL);
						break;
					}
				}
			}
			if (strcmp(token, "list_empty") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_empty\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						if (list_empty(lists[i]) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
			if (strcmp(token, "list_size") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_size\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0)
						printf("%zu\n", list_size(lists[i]));
				}
			}
			if (strcmp(token, "list_max") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_max\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						struct LD *tmpl;
						tmp = list_max(lists[i], list_less_function, NULL);
						tmpl = list_entry(tmp, struct LD, elem);
						printf("%d\n", tmpl->data);
						break;
					}
				}
			}
			if (strcmp(token, "list_min") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_min\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						struct list_elem *tmp;
						struct LD *tmpl;
						tmp = list_min(lists[i], list_less_function, NULL);
						tmpl = list_entry(tmp, struct LD, elem);
						printf("%d\n", tmpl->data);
						break;
					}
				}
			}
			if (strcmp(token, "list_remove") == 0) {
				char *name, *data;
				int idx;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : list_remove\n");
					continue;
				}
				idx = atoi(data);

				struct list_elem *tmp;
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						tmp = list_begin(lists[i]);
						break;
					}
				}
				for (i = 0; i < idx; i++)
					tmp = list_next(tmp);
				tmp = list_remove(tmp);
			}
			if (strcmp(token, "list_reverse") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_reverse\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						list_reverse(lists[i]);
						break;
					}
				}
			}
			if (strcmp(token, "list_sort") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_sort\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						list_sort(lists[i], list_less_function, NULL);
						break;
					}
				}
			}
			if (strcmp(token, "list_splice") == 0) {
				char *name1, *data1, *name2, *data2, *data3;
				int idx1, idx2, idx3;
				name1 = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				name2 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				data3 = strtok(NULL, " ");
				if (name1 == NULL || data1 == NULL || name2 == NULL || data2 == NULL || data3 == NULL) {
					printf("Error : list_splice\n");
					continue;
				}
				idx1 = atoi(data1);
				idx2 = atoi(data2);
				idx3 = atoi(data3);

				struct list_elem *tmp1;
				struct list_elem *tmp2;
				struct list_elem *tmp3;
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name1, listName[i]) == 0)
						tmp1 = list_begin(lists[i]);
					if (strcmp(name2, listName[i]) == 0) {
						tmp2 = list_begin(lists[i]);
						tmp3 = list_begin(lists[i]);
					}
				}
				for (i = 0; i < idx1; i++)
					tmp1 = list_next(tmp1);
				for (i = 0; i < idx2; i++)
					tmp2 = list_next(tmp2);
				for (i = 0; i < idx3; i++)
					tmp3 = list_next(tmp3);
				list_splice(tmp1, tmp2, tmp3);
			}
			if (strcmp(token, "list_swap") == 0) {
				char *name, *data1, *data2;
				int idx1, idx2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL) {
					printf("Error : list_swap\n");
					continue;
				}
				idx1 = atoi(data1);
				idx2 = atoi(data2);

				struct list_elem *tmp1;
				struct list_elem *tmp2;
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						tmp1 = list_begin(lists[i]);
						tmp2 = list_begin(lists[i]);
						break;
					}
				}
				for (i = 0; i < idx1; i++)
					tmp1 = list_next(tmp1);
				for (i = 0; i < idx2; i++)
					tmp2 = list_next(tmp2);
				list_swap(tmp1, tmp2);
			}
			if (strcmp(token, "list_unique") == 0) {
				char *name1, *name2;
				name1 = strtok(NULL, " ");
				name2 = strtok(NULL, " ");
				if (name1 == NULL) {
					printf("Error : list_unique\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name1, listName[i]) == 0) {
						if (name2 == NULL) {
							list_unique(lists[i], NULL, list_less_function, NULL);
						}
						else {
							for (j = 0; j < listCnt; j++) {
								if (strcmp(name2, listName[j]) == 0) {
									list_unique(lists[i], lists[j], list_less_function, NULL);
									break;
								}
							}
						}
						break;
					}
				}
			}
			if (strcmp(token, "list_shuffle") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : list_shuffle\n");
					continue;
				}
				for (i = 0; i < listCnt; i++) {
					if (strcmp(name, listName[i]) == 0) {
						list_shuffle(lists[i]);
						break;
					}
				}
			}
			/* hash commands */
			if (strcmp(token, "hash_insert") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : hash_insert\n");
					continue;
				}
				val = atoi(data);

				struct HD *tmph;
				tmph = (struct HD*)malloc(sizeof(struct HD));
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						tmph->data = val;
						hash_insert(hashs[i], &(tmph->elem));
						break;
					}
				}
			}
			if (strcmp(token, "hash_apply") == 0) {
				char *name, *option;
				name = strtok(NULL, " ");
				option = strtok(NULL, " ");
				if (name == NULL || option == NULL) {
					printf("Error : hash_apply\n");
					continue;
				}

				struct hash *tmp;
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						tmp = hashs[i];
						break;
					}
				}
				if (strcmp(option, "square") == 0)
					hash_apply(tmp, &hash_action_func_square);
				else if (strcmp(option, "triple") == 0)
					hash_apply(tmp, &hash_action_func_triple);
				else
					continue;
			}
			if (strcmp(token, "hash_delete") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : hash_delete\n");
					continue;
				}
				val = atoi(data);

				struct HD *tmph;
				tmph = (struct HD*)malloc(sizeof(struct HD));
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						tmph->data = val;
						hash_delete(hashs[i], &(tmph->elem));
						break;
					}
				}
			}
			if (strcmp(token, "hash_empty") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : hash_empty\n");
					continue;
				}
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						if (hash_empty(hashs[i]) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
			if (strcmp(token, "hash_size") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : hash_size\n");
					continue;
				}
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						printf("%zu\n", hash_size(hashs[i]));
						break;
					}
				}
			}
			if (strcmp(token, "hash_clear") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : hash_clear\n");
					continue;
				}
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						hash_clear(hashs[i], NULL);
						break;
					}
				}
			}
			if (strcmp(token, "hash_find") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : hash_find\n");
					continue;
				}
				val = atoi(data);

				struct HD *tmph;
				tmph = (struct HD*)malloc(sizeof(struct HD));
				for (i = 0; i < hashCnt; i++) {
					if (strcmp(name, hashName[i]) == 0) {
						tmph->data = val;
						if (hash_find(hashs[i], &(tmph->elem)))
							printf("%d\n", tmph->data);
						break;
					}
				}
			}
			if (strcmp(token, "hash_replace") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : hash_replace\n");
					continue;
				}
				val = atoi(data);

				struct HD *tmph;
				tmph = (struct HD*)malloc(sizeof(struct HD));
				for (i = 0; i < hashCnt;i++) {
					if (strcmp(name, hashName[i]) == 0) {
						tmph->data = val;
						hash_replace(hashs[i], &(tmph->elem));
						break;
					}
				}
			}
			/* bitmap commands */
			if (strcmp(token, "bitmap_mark") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : bitmap_mark\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						bitmap_mark(bitmaps[i], val);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_all") == 0) {
				char *name, *data1, *data2;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL) {
					printf("Error : bitmap_all\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (bitmap_all(bitmaps[i], val1, val2) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_any") == 0) {
				char *name, *data1, *data2;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL) {
					printf("Error : bitmap_any\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (bitmap_any(bitmaps[i], val1, val2) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_contains") == 0) {
				char *name, *data1, *data2, *state;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL || state == NULL) {
					printf("Error : bitmap_contains\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0) {
							if (bitmap_contains(bitmaps[i], val1, val2, true) == true)
								printf("true\n");
							else
								printf("false\n");
						}
						else {
							if (bitmap_contains(bitmaps[i], val1, val2, false) == true)
								printf("true\n");
							else
								printf("false\n");
						}
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_count") == 0) {
				char *name, *data1, *data2, *state;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL || state == NULL) {
					printf("Error : bitmap_count\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0)
							printf("%zu\n", bitmap_count(bitmaps[i], val1, val2, true));
						else
							printf("%zu\n", bitmap_count(bitmaps[i], val1, val2, false));
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_dump") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : bitmap_dump\n");
					continue;
				}

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						bitmap_dump(bitmaps[i]);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_expand") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : bitmap_expand\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						bitmaps[i] = bitmap_expand(bitmaps[i], val);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_flip") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : bitmap_flip\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						bitmap_flip(bitmaps[i], val);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_none") == 0) {
				char *name, *data1, *data2;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL) {
					printf("Error : bitmap_none\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (bitmap_none(bitmaps[i], val1, val2) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_reset") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : bitmap_reset\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						bitmap_reset(bitmaps[i], val);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_scan") == 0) {
				char *name, *data1, *data2, *state;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL || state == NULL) {
					printf("Error : bitmap_scan\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0)
							printf("%u\n", (unsigned int)bitmap_scan(bitmaps[i], val1, val2, true));
						else
							printf("%u\n", (unsigned int)bitmap_scan(bitmaps[i], val1, val2, false));
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_scan_and_flip") == 0) {
				char *name, *data1, *data2, *state;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL || state == NULL) {
					printf("Error : bitmap_scan_and_flip\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if(strcmp(state,"true") == 0)
							printf("%u\n", (unsigned int)bitmap_scan_and_flip(bitmaps[i], val1, val2, true));
						else
							printf("%u\n", (unsigned int)bitmap_scan_and_flip(bitmaps[i], val1, val2, false));
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_set") == 0) {
				char *name, *data, *state;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data == NULL || state == NULL) {
					printf("Error : bitmap_set\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0)
							bitmap_set(bitmaps[i], val, true);
						else
							bitmap_set(bitmaps[i], val, false);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_set_all") == 0) {
				char *name, *state;
				int val;
				name = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || state == NULL) {
					printf("Error : bitmap_set_all\n");
					continue;
				}
				
				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0)
							bitmap_set_all(bitmaps[i], true);
						else
							bitmap_set_all(bitmaps[i], false);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_set_multiple") == 0) {
				char *name, *data1, *data2, *state;
				int val1, val2;
				name = strtok(NULL, " ");
				data1 = strtok(NULL, " ");
				data2 = strtok(NULL, " ");
				state = strtok(NULL, " ");
				if (name == NULL || data1 == NULL || data2 == NULL || state == NULL) {
					printf("Error : bitmap_set_multiple\n");
					continue;
				}
				val1 = atoi(data1);
				val2 = atoi(data2);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (strcmp(state, "true") == 0)
							bitmap_set_multiple(bitmaps[i], val1, val2, true);
						else
							bitmap_set_multiple(bitmaps[i], val1, val2, false);
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_size") == 0) {
				char *name;
				name = strtok(NULL, " ");
				if (name == NULL) {
					printf("Error : bitmap_size\n");
					continue;
				}

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						printf("%zu\n", bitmap_size(bitmaps[i]));
						break;
					}
				}
			}
			if (strcmp(token, "bitmap_test") == 0) {
				char *name, *data;
				int val;
				name = strtok(NULL, " ");
				data = strtok(NULL, " ");
				if (name == NULL || data == NULL) {
					printf("Error : bitmap_test\n");
					continue;
				}
				val = atoi(data);

				for (i = 0; i < bitmapCnt; i++) {
					if (strcmp(name, bitmapName[i]) == 0) {
						if (bitmap_test(bitmaps[i], val) == true)
							printf("true\n");
						else
							printf("false\n");
						break;
					}
				}
			}
		}
	}
}

unsigned hash_hash_function(const struct hash_elem *e, void *aux) {
	struct HD *tmp;
	int data;
	tmp = hash_entry(e, struct HD, elem);
	data = tmp->data;
	return hash_int(data);
}

bool hash_less_function(const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	struct HD* tmpa;
	struct HD* tmpb;
	tmpa = hash_entry(a, struct HD, elem);
	tmpb = hash_entry(b, struct HD, elem);
	return (tmpa->data < tmpb->data) ? true : false;
}

bool list_less_function(const struct list_elem *a, const struct list_elem *b, void *aux) {
	struct LD *tmpa;
	struct LD *tmpb;
	tmpa = list_entry(a, struct LD, elem);
	tmpb = list_entry(b, struct LD, elem);
	return (tmpa->data < tmpb->data) ? true : false;
}

void hash_action_func_square(struct hash_elem *a, void *aux) {
	struct HD *tmpa;
	tmpa = hash_entry(a, struct HD, elem);
	tmpa->data *= (tmpa->data);
}
void hash_action_func_triple(struct hash_elem *a, void *aux) {
	struct HD *tmpa;
	tmpa = hash_entry(a, struct HD, elem);
	tmpa->data *= (tmpa->data)*(tmpa->data);
}