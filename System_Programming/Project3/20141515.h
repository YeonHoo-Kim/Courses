#ifndef __20141515__
#define __20141515__

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include"dirent.h"

typedef struct Node_ {

	char strcmd[500];
	struct Node_ *link;

}Node;

typedef struct bp_ {
	int point;
	struct bp_*link;
}bp;

typedef struct HashTable_ {

	char mne[10];
	int format;
	int opcode;
	struct HashTable_ *link;

}HashTable;

typedef struct SymTab_ {
	char sym[10];
	int value;
	int flag;
	struct SymTab_ *link;
}SymTab;

typedef struct EsymTab_ {
	char ctrl[7];
	char esym[7];
	int addr;
	int len;
	struct EsymTab_ *link;
}EsymTab;

Node *head;
Node *rear;
EsymTab *esymtab;
bp *bphead;

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
int getFormat(HashTable **mne, char *str, int index);
void insertSym(SymTab **symtab, char *label, int locctr);
int searchSym(SymTab **symtab, char *symbol);
void printSym(SymTab **symtab);
void freeSym(SymTab **symtab);
int findSym(SymTab **symtab, char *str, char *inst);
int findSymAddr(SymTab **symtab, char *symbol);
void hexTostring(int n, char *str, int size);
int makeObjectcode(SymTab **symtab, int locctr, int format, int opcode, char *inst, char *baseSym);
void displayFile(char *fname);
int assembleFile(char *fname, HashTable **mne, SymTab **symtab);
void insertEsym(char *ctrl, char *esym, int addr, int len);
int loader(unsigned char *memory, char *loadstr, int sAddr);
int getTA(int n, int i, int x, int b, int p, int e, int disp, int *reg, unsigned char *memory, int sAddr);
int getDisp(int b, int p, int e, int *reg, unsigned char *memory);
int run(unsigned char *memory, int sAddr);

#endif
