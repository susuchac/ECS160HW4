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
		if(strcmp(tok, str) == 0) {
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
		if(!num--) {
			return tok;
		}
	}
	return NULL;
}

int addToList(char* name, struct tweeter *list, int index) {
	for(int i = 0; i < NUM_TWEETERS; i++) {
		if(list[i].name != NULL && strcmp(list[i].name, name) == 0) {
			list[i].count++;
			return index;
		}
	}
	list[index].name = name;
	list[index].count = 1;
	return ++index;
}

void sortList(struct tweeter *list) {
	for(int i = 0; i < NUM_TWEETERS; i++) {
		for(int j = i+1; j < NUM_TWEETERS; j++) {
			if(list[i].count < list[j].count) {
				int tmpc = list[i].count;
				char* tmpn = list[i].name;
				list[i].count = list[j].count;
				list[i].name = list[j].name;
				list[j].count = tmpc;
				list[j].name = tmpn;
			}
		}
	}
}

int main(int argc, char** argv)
{
	printf("Input file: %s\n", argv[1]);
	FILE* stream = fopen(argv[1], "r");

	if(stream == NULL) {
		printf("Invalid Input Format\n");
		return -1;
	}

	static char colNames[MAX_CHARS_READ];
	fgets(colNames, MAX_CHARS_READ, stream);
 	char* tmpColNames;
	strcpy (tmpColNames, colNames);
	int nameCol = getColNum(colNames, "name");
	//printf("%s", tmpColNames);
	//printf("hello\n");
	
	
	int tweetCol = getColNum(tmpColNames, "text");
	printf("help\n");
	printf("%d\n", tweetCol);

	char line[MAX_CHARS_READ];
	static struct tweeter tweeters[NUM_TWEETERS];
	int nextIndex = 0;
	// printf("HERE\n");

	while(fgets(line, MAX_CHARS_READ, stream))
	{
		char* tmp = strdup(line);
		printf("%s\n", tmp);
		char* tmp2;
		//strcpy (tmp2, line);
		char* field = getField(tmp, nameCol);
		//printf("%s\n", field);
		//printf("HEREEE\n");
		//printf("%s\n", tmp2);	
		//char *tmp2 = strdup(line);
		//printf("%s\n", tmp2);
		//char* tweet = getField(line, tweetCol);
		//if(strpbrk(tweet, ",")) {
		//	printf("Invalid Input Format\n");
		 //	return -1;
		 //}
		nextIndex = addToList(field, tweeters, nextIndex);
	}

	sortList(tweeters);

	printf("TOP 10 TWEETERS\n");
	for(int i = 0; i < 10; i++) {
			printf("<%s>: <%d>\n", tweeters[i].name, tweeters[i].count);
	}
}
