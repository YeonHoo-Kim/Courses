#ifndef __20141515__
#define __20141515__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<dirent.h>

typedef struct Node_ {

	char strcmd[500];
	struct Node_ *link;

}Node;

typedef struct HashTable_ {

	char mne[10];
	int opcode;
	struct HashTable_ *link;

}HashTable;

Node *head;
Node *rear;

void addNode(char *cmd);
void help();
void dir();
void history();
int charTohex(char c);
int TwoInputsString(char *str, int *first, int *second);
int dump(unsigned char *mem, int s, int e);
int ThreeInputsString(char *str, int *first, int *second, int *third);
int hashfunc(char *str);
int getOpcode(HashTable **mne, char *str, int index);


#endif
