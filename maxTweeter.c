#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_TWEETERS 11433
#define MAX_CHARS_READ 1024

struct tweeter {
	char* name;
	int count;
};

char* strtok2(char* line, char const* delim)
{
	static char* src = NULL;
	char *tok, *retval = 0;
	if(line != NULL)
		src = line;
	if(src == NULL) 
		return NULL;
	if((tok = strpbrk(src, delim)) != NULL) {
		*tok = 0;
		retval = src;
		src = ++tok;
	}
	return retval;

}

int getColNum(char* line, char* str)
{
	const char* tok;
	int col = 0;
	for(tok = strtok2(line, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		// printf("colnum: %d\n", col);
		// printf("tok: %s\n", tok);
		if(strcmp(tok, str) == 0) {
			// printf("str: %s\n", str);
			return col;
		}
		col++;
	}
	return -1;
}

char* getField(char* line, int num)
{
	char* tok;
	for(tok = strtok2(line, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		// printf("tok: %s\n", tok);
		// printf("num: %d\n", num);
		if(!num--) {
			return tok;
		}
		// printf("HELLO\n");
	}
	return NULL;
}

int searchList(char* name, struct tweeter *list[], int index) {
	for(int i = 0; i < 7; i++) {
		printf("forloop\n");
		printf("%s\n", list[index]->name);
		printf("meh\n");
		if(list[index]->name != NULL && strcmp(list[index]->name, name) == 0) {
			printf("HITHER\n");
			list[index]->count++;
			return ++index;
		}
	}
	printf("THITHER\n");
	list[index]->name = name;
	// strcpy(list[index].name, name);
	printf("aftercopy\n");
	list[index]->count = 1;
	return ++index;
}

int main(int argc, char** argv)
{
	printf("Input file: %s\n", argv[1]);
	FILE* stream = fopen(argv[1], "r");

	char colNames[MAX_CHARS_READ];
	fgets(colNames, MAX_CHARS_READ, stream);
	int nameCol = getColNum(colNames, "name");
	printf("%d\n", nameCol);

	char line[MAX_CHARS_READ];
	// char* tweeters[NUM_TWEETERS];
	struct tweeter tweeters[NUM_TWEETERS];// = malloc(sizeof(struct tweeter));
	int index = 0;

	while(fgets(line, MAX_CHARS_READ, stream))
	{
		char* tmp = strdup(line);

		char* field = getField(tmp, nameCol);
		printf("Name: %s\n", field);
		// tweeters[index] = out;
		printf("Hi\n");
		// tweeters[index].name = "hi";
		index = searchList(field, &tweeters, index);
			printf("Hiiii\n");
		// index++;
		printf("index: %d\n", index);
		for(int i = 0; i < 7; i++) {
			printf("index: %d, name: %s, count: %d\n", i, tweeters[i].name, tweeters[i].count);
		}
		if(index > 6)
			break;
	}

	// for(int i = 0; i < 10; i++) {
	// 	printf("index: %d, name: %s, count: %d\n", i, tweeters[i].name, tweeters[i].count);
	// }
}
