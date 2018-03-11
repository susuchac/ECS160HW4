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
	// if(delim == NULL) {
	// 	printf("Invalid Input Format\n");
	// 	// return -1;
	// }
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

int getNumberOfCols(char* line) 
{
	if(line == NULL) {
		printf("Invalid Input Format\n");
		return -1;
	}
	char* tok;
	// printf("line: %s\n", line);
	char* commas;
	// printf("commas: %s\n", commas);
	int count = 0;
	if((commas = strpbrk(line, ",")) != NULL) {
		count = 1;
	}
	while(commas != NULL) {
		// printf("commas: %s", commas);
		commas = strpbrk(commas+1, ",");
		count++;
	}
	// printf("Count: %d\n", count);
	return count;
}

int getColNum(char* line, char* str)
{
	if(line == NULL || str == NULL) {
		printf("Invalid Input Format\n");
		return -1;
	}
	char* tok;
	int col = 0;
	for(tok = strtok2(line, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		if(tok == NULL) {
			printf("Invalid Input Format\n");
			return -1;
		}
		// printf("%d\n", col);
		if(strcmp(tok, str) == 0) {
			return col;
		}
		col++;
	}
	return -1;
}

char* getField(char* line, int num)
{
	// if(line == NULL || num < 0) {
	// 	printf("Invalid Input Format\n");
	// 	// return -1;
	// }
	char* tmpline = strdup(line);
	char* tok;
	for(tok = strtok2(tmpline, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		if(tok == NULL) {
			printf("Invalid Input Format\n");
			return NULL;
		}
		if(!num--) {
			return tok;
		}
	}
	return NULL;
}

int addToList(char* name, struct tweeter *list, int index) {
	if(name == NULL || list == NULL || index < 0) {
		printf("Invalid Input Format\n");
		return -1;
	}
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
	// if(list == NULL) {
	// 	printf("Invalid Input Format\n");
	// 	// return -1;
	// }
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
	if(argv[1] == NULL) {
		printf("Invalid Input Format\n");
		return -1;
	}
	printf("Input file: %s\n", argv[1]);
	FILE* stream = fopen(argv[1], "r");

	if(stream == NULL) {
		printf("Invalid Input Format\n");
		return -1;
	}

	char colNames[MAX_CHARS_READ];
	fgets(colNames, MAX_CHARS_READ, stream);
	// printf("stream: %d\n", stream);
	// printf("MAX_CHARS_READ: %d\n", MAX_CHARS_READ);

	// printf("colNames: %s\n", colNames);
	char* cnTmp = strdup(colNames);
	// printf("cnTmp: %s\n", cnTmp);
	int nameCol = getColNum(cnTmp, "name");
	// printf("cnTmp: %s\n", cnTmp);
	// printf("nameCol: %d\n", nameCol);

	cnTmp = strdup(colNames);
	// printf("cnTmp2: %s\n", cnTmp2);
	int tweetCol = getColNum(cnTmp, "text");
	// printf("cnTmp2: %s\n", cnTmp2);
	// printf("tweetCol: %d\n", tweetCol);

	cnTmp = strdup(colNames);
	// printf("cnTmp3: %s\n", cnTmp3);
	int numCols = getNumberOfCols(cnTmp);
	// printf("cnTmp3: %s\n", cnTmp3);
	// printf("numCols %d\n", numCols);


	char line[MAX_CHARS_READ];
	static struct tweeter tweeters[NUM_TWEETERS];
	// printf("NUM_TWEETERS: %d\n", NUM_TWEETERS);
	int nextIndex = 0;

	// int cnt = 0;
	while(fgets(line, MAX_CHARS_READ, stream))
	{
	// fgets(line, MAX_CHARS_READ, stream);
		char* tmp = strdup(line);
		// printf("tmp: %s\n", tmp);
		// for(int i = 0; i < 16; ++i) {
			// printf("field %d %s\n", i, getField(tmp, i));
			// tmp = strdup(line);
		// }
		// printf("%s\n", getField(tmp, 11));
		char* field = getField(tmp, nameCol);
		if(field == NULL) {
			printf("Invalid Input Format\n");
			return -1; 
		}


		tmp = strdup(line);
		// printf("field: %s\n", field);
		// printf("HEREEE\n");
		// char* tweet = getField(tmp, tweetCol);
		// tmp = strdup(line);
		// printf("tweet: %s\n", tweet);

		// char* tweetTmp = strdup(tweet);
		// printf("strpbrk: %s\n", strpbrk(tweetTmp, ","));
		// tweetTmp = strdup(tweet);
		// if(strpbrk(tweetTmp, ",") != NULL) {
		// 	printf("Invalid Input Format\n");
		// 	return -1;
		// }
	 	// printf("%s\n", tmp);
		nextIndex = addToList(field, tweeters, nextIndex);
		// printf("nextIndex: %d\n", nextIndex);
		// char* tmpl = strdup(line);
		// printf("tmpl: %s\n", tmpl);
		int ncols = getNumberOfCols(tmp);
		if(ncols != numCols) {
			printf("Invalid Input Format\n");
			return -1;
		}
		// printf("tmpl: %s\n", tmpl);
		// printf("ncols %d\n", ncols);
		// if(cnt > 1) {
		// 	break;
		// }
		// cnt++;
	}

	sortList(tweeters);

	printf("TOP 10 TWEETERS\n");
	for(int i = 0; i < 10; i++) {
		printf("<%s>: <%d>\n", tweeters[i].name, tweeters[i].count);
	}
}
