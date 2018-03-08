#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
		printf("colnum: %d\n", col);
		printf("tok: %s\n", tok);
		if(strcmp(tok, str) == 0) {
			printf("str: %s\n", str);
			return col;
		}
		col++;
	}
	return -1;
}

const char* getField(char* line, int num)
{
	const char* tok;
	for(tok = strtok2(line, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		printf("tok: %s\n", tok);
		// printf("field\n");
		if(!--num)
			return tok;
	}
	return NULL;
}

int main(int argc, char** argv)
{
	printf("Input file: %s\n", argv[1]);
	FILE* stream = fopen(argv[1], "r");

	char colNames[1024];
	fgets(colNames, 1024, stream);
	int nameCol = getColNum(colNames, "name");
	printf("%d\n", nameCol);

	char line[1024];


	while(fgets(line, 1024, stream))
	{
		char* tmp = strdup(line);

		const char* out = getField(tmp, nameCol);
		printf("Name: %s\n", out);
		break;
	}
}
