#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getColNum(char* line, char* str)
{
	const char* tok;
	int col = 0;
	for(tok = strtok(line, ",");  ; tok = strtok(NULL, ",\n"))
	{
		printf("tok: %s\n", tok);
		if(strcmp(tok, str) == 0) {
			printf("str: %s\n", str);
			return col;
		}
		col++;
		printf("colnum: %d\n", col);
	}
	return -1;
}

const char* getField(char* line, int num)
{
	const char* tok;
	for(tok = strtok(line, ",");  ; tok = strtok(NULL, ",\n"))
	{
		printf("field\n");
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

	char line[1024];


	while(fgets(line, 1024, stream))
	{
		char* tmp = strdup(line);

		const char* out = getField(tmp, nameCol);
		printf("Name: %s\n", out);
		break;
	}
}
