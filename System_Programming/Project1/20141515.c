#include "20141515.h"

void addNode(char *cmd) {											// when valid command is excuted add the command string to the linked list (for history)

	Node *n = (Node*)malloc(sizeof(Node));
	strcpy(n->strcmd, cmd);
	n->link = NULL;

	if (head == NULL)
		head = n;
	else
		rear->link = n;
	rear = n;
}

void help() {														// display all commands supported by this program
	printf("\n");
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill], start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n\n");
}

void dir() {														// shows the files in the current directory

	DIR *dir;
	struct dirent *dire;
	struct stat statbuf;

	dir = opendir("./");
	printf("\n");
	if (dir != NULL) {
		readdir(dir);
		readdir(dir);
		while ((dire = readdir(dir)) != NULL) {
			printf("%s", dire->d_name);
			if (stat(dire->d_name, &statbuf) < 0) {
				perror("stat error\n");
				return;
			}
			if (S_ISDIR(statbuf.st_mode))	// if it's a directory
				printf("/");

			else if ((statbuf.st_mode & S_IXUSR) != 0 || (statbuf.st_mode & S_IXGRP) != 0 || (statbuf.st_mode & S_IXOTH) != 0)	// if file has execute permission (which means it's executable file)
				printf("*");

			printf("\n");
		}
		closedir(dir);
	}
	else
		perror("\nerror on directory output...(d[ir])\n\n");
	printf("\n");
}

void history() {													// print out history of all valid commands excuted

	Node *temp = head;
	int i = 1;
	printf("\n");
	while (temp != NULL) {
		printf("%d\t%s\n", i, temp->strcmd);
		temp = temp->link;
		i++;
	}

	printf("\n");
}

int charTohex(char c) {												// change character to hexadecimal

	if (c >= '0'&&c <= '9')
		return c - '0';
	else if (c >= 'A'&&c <= 'F')
		return c - 'A' + 10;
	else if (c >= 'a'&&c <= 'f')
		return c - 'a' + 10;
	else
		return -1;
}

int TwoInputsString(char *str, int *first, int *second) {			// get two inputs from str (used for du[mp] and e[dit])

	int i = 0, startSize = 0, comaFlag = 0, endSize = 0, spaceFlag = 0,  f = 0, s = 0;
	while (str[i] != '\0') {
		if (str[i] != ' ' && str[i] != '\t') {
			if ((str[i] >= '0'&&str[i] <= '9') || (str[i] >= 'A'&&str[i] <= 'F') || (str[i] >= 'a'&&str[i] <= 'f') || (str[i] == ',') || (str[i] == '0' && (str[i + 1] == 'X' || str[i + 1] == 'x'))) {

				if ((str[i] == ',')) {
					if (startSize == 0) {							// command , ...
						printf("\nWrong Command!!!\n\n");
						return 0;
					}
					else {											// command start, ...
						if (str[i + 1] == '\0') {					// command start, (error)
							printf("\nWrong Command!!!\n\n");
							return 0;
						}
						comaFlag++;
						spaceFlag = 0;
						if(comaFlag >= 2){							// when there are more inputs (error)
							printf("\nWrong Command!!!\n\n");
							return 0;
						}
					}
				}

				else if (str[i] == '0' && (str[i + 1] == 'X' || str[i + 1] == 'x') && str[i-1] != '0') {
					if (!((str[i + 2] >='0' && str[i + 2] <= '9') || (str[i + 2] >= 'A' && str[i + 2] <= 'F') || (str[i + 2] >= 'a' && str[i + 2] <= 'f'))) {					// command start, 0x (end error)
						printf("\nWrong Command!!!\n\n");
						return 0;
					}
					i++;
				}

				else if ((str[i] >= '0'&&str[i] <= '9') || (str[i] >= 'A'&&str[i] <= 'F') || (str[i] >= 'a'&&str[i] <= 'f')) {

					if (str[i + 1] == '\0' && comaFlag == 0)					// command start 
						s = -1;

					if (comaFlag == 0) {
						f = 16 * f + charTohex(str[i]);
						startSize++;
					}
					if (comaFlag == 1) {
						s = 16 * s + charTohex(str[i]);
						if (s > 0xFFFFF) {
							s = 0xFFFFF;
							break;
						}
						endSize++;
					}
					if (spaceFlag == 1 && (startSize != 0 || endSize != 0)) {	// command s tart... or  dump stard end (errors) 
						printf("\nWrong Command!!! (Input error)\n\n");
						return 0;
					}
				}
			}
			else {
				printf("\nWrong Command!!! (Input error)\n\n");				// other words in string (error)		com mand (error)
				return 0;
			}
		}
		else {
			if (startSize != 0 && comaFlag == 0)
				spaceFlag = 1;
			if (endSize != 0) {									// command start, e nd (error)
				printf("\nWrong Command!!! (Input error)\n\n");
				return 0;
			}
		}
		i++;
	}

	*first = f;
	if (s != -1)
		*second = s;
	else
		*second = -1;

	return 1;
}

int dump(unsigned char *mem, int s, int e) {		// outputs for command d[ump]

	int eAddr, start, tmps;
	
	printf("\n");
	
	if (s <= e || e == -1) {				// print address from start to end (when e == -1 there is no end address)
		int i = 0;

		if (e == -1)
			e = s + 159;

		if (e > 0xFFFFF)
			e = 0xFFFFF;

		eAddr = e;
		start = s;

		while (start >= 0) {
			start -= 16;
			i++;
		}
		i--;
		start = 0;
		for (int j = 0; j < i; j++)
			start += 16;

		tmps = start;
		for (int j = 0; j < (e - tmps) / 16 + 1; j++) {
			if (tmps <= s && s <= e && e <= tmps + 15) {
				printf("%05X ", start);
				for (int k = tmps; k < s; k++)
					printf("   ");
				for (int k = s; k <= e; k++)
					printf("%02X ", mem[k]);
				for (int k = e + 1; k <= tmps + 15; k++)
					printf("   ");
				printf("; ");
				for (int k = 0; k < 16; k++)
					printf(".");
				printf("\n");
				break;
			}
			if (j < (e - tmps) / 16) {
				printf("%05X ", start);
				for (int k = 0; k < 16; k++) {
					if (j == 0 && k < s - (s / 16) * 16)
						printf("   ");
					else
						printf("%02X ", mem[tmps + 16 * j + k]);
				}
				printf("; ");
				for (int k = 0; k < 16; k++) {
					if (mem[tmps + 16 * j + k] < 0x20 || mem[tmps + 16 * j + k]>0x7E)
						printf(".");
					else
						printf("%c", mem[tmps + 16 * j + k]);
				}
				printf("\n");
				start += 16;
			}
			if (j == (e - tmps) / 16 && e - (e / 16) * 16 >= 0) {
				printf("%05X ", start);
				for (int k = 0; k < 16; k++) {
					if (k <= e - (e / 16) * 16)
						printf("%02X ", mem[tmps + 16 * j + k]);
					else
						printf("   ");
				}
				printf("; ");
				for (int k = 0; k < 16; k++) {
					if (k <= e - (e / 16) * 16) {
						if (mem[tmps + 16 * j + k] < 0x20 || mem[tmps + 16 * j + k]>0x7E)
							printf(".");
						else
							printf("%c", mem[tmps + 16 * j + k]);
					}
					else
						printf(".");
				}
				printf("\n");
			}
		}
	}

	else {
		eAddr = 0;
		printf("Wrong Command!!!(start address must be smaller than end address...)\n");
	}

	printf("\n");
	return eAddr;
}

int ThreeInputsString(char *str, int *first, int *second, int *third) {				// get three inputs from str (used for f[ill])

	int i = 0, firstSize = 0, secondSize = 0, thirdSize = 0, comaFlag = 0, spaceFlag = 0, f = 0, s = 0, t = 0;
	while (str[i] != '\0') {
		if (str[i] != ' ' && str[i] != '\t') {
			if ((str[i] >= '0'&&str[i] <= '9') || (str[i] >= 'A'&&str[i] <= 'F') || (str[i] >= 'a'&&str[i] <= 'f') || (str[i] == ',') || (str[i] == '0' && (str[i + 1] == 'X' || str[i + 1] == 'x'))) {

				if ((str[i] == ',')) {
					if (firstSize == 0) {							// command , ...
						printf("\nWrong Command!!!\n\n");
						return 0;
					}
					else {										// command start, ...
						if (str[i + 1] == '\0') {				// command start, (error)
							printf("\nWrong Command!!!\n\n");
							return 0;
						}
						comaFlag++;
						spaceFlag = 0;
						if (comaFlag >= 3){
							printf("\nWrong Command!!!\n\n");
							return 0;
						}
					}
				}

				else if (str[i] == '0' && (str[i + 1] == 'X' || str[i + 1] == 'x')&& str[i-1] != '0') {
					if (!((str[i + 2] >= '0' && str[i + 2] <= '9') || (str[i + 2] >= 'A' && str[i + 2] <= 'F') || (str[i + 2] >= 'a' && str[i + 2] <= 'f'))) {					// command start, 0x (end error)
						printf("\nWrong Command!!!\n\n");
						return 0;
					}
					i++;
				}

				else if ((str[i] >= '0'&&str[i] <= '9') || (str[i] >= 'A'&&str[i] <= 'F') || (str[i] >= 'a'&&str[i] <= 'f')) {

					if (comaFlag == 0) {
						f = 16 * f + charTohex(str[i]);
						firstSize++;
					}
					if (comaFlag == 1) {
						s = 16 * s + charTohex(str[i]);
						secondSize++;
					}
					if (comaFlag == 2) {
						t = 16 * t + charTohex(str[i]);
						thirdSize++;
					}
					if (spaceFlag == 1 && (firstSize != 0 || secondSize != 0 || thirdSize != 0)) {	// command s tart... or  dump stard end (errors) 
						printf("\nWrong Command!!! (Input error)\n\n");
						return 0;
					}
				}
			}
			else {
				printf("\nWrong Command!!! (Input error)\n\n");				// other words in string (error)		com mand (error)
				return 0;
			}
		}
		else {
			if ((firstSize != 0 && comaFlag == 0) || (secondSize != 0 && comaFlag == 1))
				spaceFlag = 1;
			if (thirdSize != 0) {									// command start, e nd (error)
				printf("\nWrong Command!!! (Input error)\n\n");
				return 0;
			}
		}
		i++;
	}
	if (firstSize == 0 || secondSize == 0 || thirdSize == 0) {
		printf("\nWrong Command!!!(Need to write 3 inputs)\n\n");
		return 0;
	}
	*first = f;
	*second = s;
	*third = t;
	if(f>s){
		printf("\nWrong Command!!!(start address must be smaller than end address...)\n\n");
		return 0;
	}
		
	return 1;
}

int hashfunc(char *str) {												// hashfunction : Returns the remainder value of res(sum of values of each characters ASCII code in string) divided by 20
	int i = 0, res = 0;
	while (str[i] != '\0') {
		res += (int)str[i];
		i++;
	}
	return res % 20;
}

int getOpcode(HashTable ** mne, char *str, int index) {					// get (str)'s opcode from hashtable

	HashTable *temp = mne[index];
	while (temp != NULL) {
		if (strcmp(temp->mne, str) == 0)
			return temp->opcode;
		temp = temp->link;
	}
	return -1;
}

void makeHashTable(HashTable **mne) {									// make hashtable of opcode
	FILE *fp = fopen("opcode.txt", "r");
	char m[10], tmp;
	int code, i, j;
	tmp = fgetc(fp);
	while (tmp != EOF) {
		i = 0; j = 0; code = 0;
		for (int k = 0; k < 10; k++)
			m[k] = 0;
		while (tmp != '\n') {
			if (i < 2) {
				if (tmp >= '0'&&tmp <= '9')
					code = code * 16 + tmp - '0';

				if (tmp >= 'A'&&tmp <= 'F')
					code = code * 16 + tmp - 'A' + 10;

				i++;
			}
			else {
				if (tmp >= 'A'&&tmp <= 'Z') {
					m[j] = tmp;
					j++;
				}
			}
			tmp = fgetc(fp);
		}
		HashTable *temp, *node;
		temp = (HashTable*)malloc(sizeof(HashTable));
		strcpy(temp->mne, m);
		temp->opcode = code;
		temp->link = NULL;

		if (mne[hashfunc(m)] == NULL)
			mne[hashfunc(m)] = temp;
		else {
			node = mne[hashfunc(m)];
			while (node->link != NULL)
				node = node->link;
			node->link = temp;
		}

		tmp = fgetc(fp);
	}
}

int main() {

	char *str;
	unsigned char *memory = (unsigned char*)malloc(0x100000 * sizeof(unsigned char));
	int sAddr = 0, addr[3];
	HashTable **mnemonic = (HashTable**)malloc(20 * sizeof(HashTable*));

	for (int a = 0; a < 0x100000; a++)
		memory[a] = 0x00;
	for (int a = 0; a < 20; a++) {
		mnemonic[a] = (HashTable*)malloc(sizeof(HashTable));
		mnemonic[a] = NULL;
	}

	makeHashTable(mnemonic);
	head = NULL;

	while (1) {																				// program progress
		printf("sicsim> ");
		char *command = (char*)malloc(500 * sizeof(char));
		scanf("%[^\n]", command);

		str = command;
		//////////////////////////////////////////////////////////// erasing indent, space character from both left and right side of the string
		if (command[0] == ' ' || command[0] == '\t') {					
			int i = 1;
			while (command[i] == ' ' || command[i] == '\t')
				i++;
			command = &command[i];
		}

		if (command[strlen(command) - 1] == ' ' || command[strlen(command) - 1] == '\t') {
			int i = (int)strlen(command) - 2;
			while (command[i] == ' ' || command[i] == '\t')
				i--;
			command[i + 1] = '\0';
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (strcmp(command, "h") == 0 || strcmp(command, "help") == 0) {					// h[elp]
			addNode(str);
			help();
		}

		else if (strcmp(command, "q") == 0 || strcmp(command, "quit") == 0)					// q[uit]
			break;

		else if (strcmp(command, "d") == 0 || strcmp(command, "dir") == 0) {
			addNode(str);
			dir();
		}

		else if (strcmp(command, "hi") == 0 || strcmp(command, "history") == 0) {			// hi[story]
			addNode(str);
			history();
		}

		else if (command[0] == 'd' && command[1] == 'u') {									// du[mp]
			if (command[2] == 'm' && command[3] == 'p') {
				if (command[4] == ' ' || command[4] == '\0' || command[4] == '\t') {

					if (command[4] == ' ' || command[4] == '\t') {							// dump A,B
						if (TwoInputsString(&command[5], &addr[0], &addr[1]) == 1) {
							if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF) {
								if (addr[1] > 0xFFFFF)
									addr[1] = 0xFFFFF;
								addNode(str);
								sAddr = dump(memory, addr[0], addr[1]) + 1;
							}
							else
								printf("\nWrong Command!!!(input is outside the bounds)\n\n");
						}
					}
					else { 																	// just dump
						if (sAddr < 0x100000) {
							addNode(str);
							sAddr = dump(memory, sAddr, -1) + 1;
						}
						else
							printf("\nWrong Command!!! (start address is out of range(0 ~ FFFFF))\n\n");
					}

				}
				else
					printf("\nWrong Command!!!\n\n");
			}

			else if (command[2] == ' ' || command[2] == '\0' || command[2] == '\t') {

				if (command[2] == ' ' || command[2] == '\t') {								// du A,B
					if (TwoInputsString(&command[3], &addr[0], &addr[1]) == 1) {
						if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF) {
							if (addr[1] > 0xFFFFF)
								addr[1] = 0xFFFFF;
							addNode(str);
							sAddr = dump(memory, addr[0], addr[1]) + 1;
						}
						else
							printf("\nWrong Command!!!(input is outside the bounds)\n\n");
					}
				}
				else {																		// just du
					if (sAddr < 0x100000) {
						addNode(str);
						sAddr = dump(memory, sAddr, -1) + 1;
					}
					else
						printf("\nWrong Command!!! (start address is out of range(0 ~ FFFFF))\n\n");
				}

			}

			else
				printf("\nWrong Command!!!\n\n");
		}

		else if (command[0] == 'e') {														// e[dit]
			if (command[1] == 'd' && command[2] == 'i' && command[3] == 't') {
				if (command[4] == ' ' || command[4] == '\t') {								// edit A,B

					if (TwoInputsString(&command[5], &addr[0], &addr[1]) == 1) {
						if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF && addr[1] >= 0x00 && addr[1] <= 0xFF) {
							addNode(str);
							memory[addr[0]] = addr[1];
							printf("\n");
						}
						else
							printf("\nWrong Command!!!(input is outside the bounds)\n\n");
					}
				}
				else
					printf("\nWrong Command!!!\n\n");
			}

			else if (command[1] == ' ' || command[1] == '\0' || command[1] == '\t') {

				if (command[1] == ' ' || command[1] == '\t') {								// e A,B
					if (TwoInputsString(&command[2], &addr[0], &addr[1]) == 1) {
						if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF && addr[1] >= 0x00 && addr[1] <= 0xFF) {
							addNode(str);
							memory[addr[0]] = addr[1];
							printf("\n");
						}
						else
							printf("\nWrong Command!!!(input is outside the bounds)\n\n");
					}
				}
				else
					printf("\nWrong Command!!!\n\n");
			}

			else
				printf("\nWrong Command!!!\n\n");
		}
		
		else if (command[0] == 'f') {														// f[ill]
			if (command[1] == 'i' && command[2] == 'l' && command[3] == 'l') {
				if (command[4] == ' ' || command[4] == '\t') {								// fill A,B,C

					if (ThreeInputsString(&command[5], &addr[0], &addr[1], &addr[2]) == 1) {
						if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF && addr[1] >= 0x00000 && addr[1] <= 0xFFFFF && addr[2] >= 0x00 && addr[2] <= 0xFF) {
							addNode(str);
							for (int j = addr[0]; j <= addr[1]; j++)
								memory[j] = addr[2];
							printf("\n");
						}
						else
							printf("\nWrong Command!!!(input is outside the bounds)\n\n");
					}
				}
				else
					printf("\nWrong Command!!!\n\n");
			}

			else if (command[1] == ' ' || command[1] == '\0' || command[1] == '\t') {

				if (command[1] == ' ' || command[1] == '\t') {								// f A,B,C
					if (ThreeInputsString(&command[2], &addr[0], &addr[1], &addr[2]) == 1) {
						if (addr[0] >= 0x00000 && addr[0] <= 0xFFFFF && addr[1] >= 0x00000 && addr[1] <= 0xFFFFF && addr[2] >= 0x00 && addr[2] <= 0xFF) {
							addNode(str);
							for (int j = addr[0]; j <= addr[1]; j++)
								memory[j] = addr[2];
							printf("\n");
						}
						else
							printf("\nWrong Command!!!(input is outside the bounds)\n\n");
					}
				}
				else
					printf("\nWrong Command!!!\n\n");
			}

			else
				printf("\nWrong Command!!!\n\n");
		}

		else if (strcmp(command, "reset") == 0) {											// reset (make all values in the memory 0)
			addNode(str);
			for (int j = 0x00000; j <= 0xFFFFF; j++)
				memory[j] = 0;
			printf("\n");
		}

		else if (strcmp(command, "opcodelist") == 0) {										// print out opcodelist by hashtable
			addNode(str);
			printf("\n");
			for (int i = 0; i < 20; i++) {
				printf("%d : ", i);
				HashTable *temp;
				temp = mnemonic[i];
				while (temp != NULL) {
					printf("[%s, %02X]", temp->mne, temp->opcode);
					if (temp->link != NULL)
						printf(" -> ");
					temp = temp->link;
				}

				printf("\n");
			}
			printf("\n");
		}

		else if (command[0] == 'o' && command[1] == 'p' && command[2] == 'c' && command[3] == 'o' && command[4] == 'd' && command[5] == 'e') {		// opcode command
			int i = 6;
			//////////////////////////////////////////////////////////// erasing indent, space character from both left and right side of the string
			while (command[i] == ' ' || command[i] == '\t')											
				i++;
			command = &command[i];
			if (command[strlen(command) - 1] == ' ' || command[strlen(command) - 1] == '\t') {
				int i = (int)strlen(command) - 2;
				while (command[i] == ' ' || command[i] == '\t')
					i--;
				command[i + 1] = '\0';
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			i = getOpcode(mnemonic, command, hashfunc(command));
			if (i != -1) {
				addNode(str);
				printf("opcode is %02X.\n\n", i);
			}
			else
				printf("Invalid mnemonic... opcode doesn't exist!\n\n");
		}

		else
			printf("\nWrong Command!!!\n\n");

		getchar();
		free(str);
	}

	free(memory);							// free
	for (int a = 0; a < 20; a++) {
		HashTable *temp;
		while (mnemonic[a] != NULL) {
			temp = mnemonic[a];
			mnemonic[a] = mnemonic[a]->link;
			free(temp);
		}
		free(mnemonic[a]);
	}
	free(mnemonic);
	while (head != NULL) {
		Node *temp = head;
		head = head->link;
		free(temp);
	}

	return 0;
}
