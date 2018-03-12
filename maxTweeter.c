#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINES 		20000
#define MAX_TWEETERS 	6228
#define MAX_CHARS_READ 	377
#define TWEETER_BUFFER 	7000
#define CHAR_BUFFER		1024

/**
 * 	A tweeter struct to hold the values of each tweeter
 *
 *	@param name: the tweeter's name
 *	@param count: the number of tweeter's tweets
 */
struct tweeter {
	char* name;
	int count;
};

/**
 *	strtok2() function: same as strtok() but doesn't skip the
 *	empty spaces between the delimiters like strtok() does
 *
 *	@param line: the line to tokenize
 *	@param delim: the delimiters
 */
char* strtok2(char* line, char const* delim)
{
	static char* src = NULL;
	char *tok, *retval = 0;
	if(line != NULL)
		src = line;
	if(src == NULL) 
		return NULL;

	/** strpbrk() returns the first instance of the delimiter */
	if((tok = strpbrk(src, delim)) != NULL) {
		*tok = 0;
		retval = src;
		src = ++tok;
	}
	return retval;

}

/**
 *	getNumberOfCols() function: gets the number of columns in a line
 *
 *	@param line: the line to count the number of columns for
 */
int getNumberOfCols(char* line) 
{
	if(line == NULL) {
		return -1;
	}
	char *tok, *commas;
	int count = 0;
	if((commas = strpbrk(line, ",")) != NULL) {
		count = 1;
	}
	while(commas != NULL) {
		commas = strpbrk(commas + 1, ",");
		count++;
	}
	return count;
}

/**
 *	getColNum() function: gets the column number in @param line 
 *	corresponding to the column header @param str
 *	
 *	@param line: the line to search for the column header @param str
 *	@param str: the column header to search for in @param line
 */
int getColNum(char* line, char* str)
{
	if(line == NULL || str == NULL) {
		return -1;
	}
	char* tok;
	int col = 0;

	char strquotes[strlen(str)+3];
	strquotes[0] = '\"';
	strcpy(strquotes+1, str);
	strquotes[strlen(str)+1] = '\"';
	strquotes[strlen(str)+2] = '\0';

	for(tok = strtok2(line, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		if(tok == NULL) {
			return -1;
		}
		if(strcmp(tok, str) == 0 || strcmp(tok, strquotes) == 0) {
			return col;
		}
		col++;
	}
	return -1;
}

/**
 *	getField() function: gets the field in @param line corresponding
 *	to the column number @param num
 *
 *	@param line: the line to search for the column number @param num
 *	@param num: the column number to search for in @param line
 */
char* getField(char* line, int num)
{
	char* tmpline = strdup(line);
	char* tok;
	for(tok = strtok2(tmpline, ",");  ; tok = strtok2(NULL, ",\n"))
	{
		if(tok == NULL) {
			return NULL;
		}
		if(!num--) {
			return tok;
		}
	}
	return NULL;
}

/**
 *	addToList() function: adds @param name to  @param list at @param index
 *
 *	@param name: the name to add to the @param list
 *	@param list: the list that @param name is being added to
 *	@param index: the index of @param list that @param name is added to
 */
int addToList(char* name, struct tweeter *list, int index) {
	if(name == NULL || list == NULL || index < 0) {
		return -1;
	}
	for(int i = 0; i < TWEETER_BUFFER; i++) {
		if(list[i].name != NULL && strcmp(list[i].name, name) == 0) {
			list[i].count++;
			return index;
		}
	}
	list[index].name = name;
	list[index].count = 1;
	return ++index;
}

/**
 *	sortList() function: sorts @param list in descending order of tweet count
 *
 *	@param list: the list to sort
 */
void sortList(struct tweeter *list) {
	for(int i = 0; i < TWEETER_BUFFER; i++) {
		for(int j = i+1; j < TWEETER_BUFFER; j++) {
			if(list[i].count < list[j].count) {
				int tmpc = list[i].count;
				char* tmpn = list[i].name;
				/** swap the values at index i and index j */
				list[i].count = list[j].count;
				list[i].name = list[j].name;
				list[j].count = tmpc;
				list[j].name = tmpn;
			}
		}
	}
}

/** 
 *	getListSize() function: gets the size of @param list
 */
int getListSize(struct tweeter *list)
{
	int count = 0;
	for(int i = 0; i < TWEETER_BUFFER; i++) {
		if(list[i].name != NULL) {
			count++;
		}
	}
	return count;
}

/** 
 *	printInvalid() function: prints the invalid statements
 */
int printInvalid() 
{
	printf("Invalid Input Format\n");
	return -1;
}

/**
 *	main() function
 */
int main(int argc, char** argv)
{
	/** if no input file */
	if(argv[1] == NULL) {
		return printInvalid();
	}
	printf("Input file: %s\n", argv[1]);
	
	FILE* stream = fopen(argv[1], "r");
	/** if fopen() failed  */
	if(stream == NULL) {
		return printInvalid();
	}

	char colNames[CHAR_BUFFER];
	fgets(colNames, CHAR_BUFFER, stream);
	/** if the length of the header line is greater than MAX_CHARS_READ */
	if(strlen(colNames) > MAX_CHARS_READ) {
		return printInvalid();
	}

	char* cnTmp = strdup(colNames);
	int nameCol = getColNum(cnTmp, "name");
	/** if '"name"' or 'name' columns not found */
	if(nameCol == -1) {
		return printInvalid();
	}

	cnTmp = strdup(colNames);
	int tweetCol = getColNum(cnTmp, "text");
	/** if '"text"' or 'text' columns not found */
	if(tweetCol == -1) {
		return printInvalid();
	}

	cnTmp = strdup(colNames);
	int numCols = getNumberOfCols(cnTmp);
	/** if cnTmp parameter is invalid */
	if(numCols == -1) {
		return printInvalid();
	}

	char line[CHAR_BUFFER];
	static struct tweeter tweeters[TWEETER_BUFFER];
	int nextIndex = 0;
	int tweeterCount = 0;
	int lineCount = 1;

	while(fgets(line, CHAR_BUFFER, stream))
	{
		/** if the line length exceeds MAX_CHARS_READ */
		if(strlen(line) > MAX_CHARS_READ) {
			return printInvalid();
		}

		char* tmp = strdup(line);
		char* field = getField(tmp, nameCol);

		/** if field is null */
		if(field == NULL) {
			return printInvalid();
		}

		tmp = strdup(line);
		nextIndex = addToList(field, tweeters, nextIndex);

		/** if nextIndex in list is invalid */
		if(nextIndex == -1) {
			return printInvalid();
		}

		int ncols = getNumberOfCols(tmp);
		/** if number of columns is not equivalent */
		if(ncols != numCols) {
			return printInvalid();
		}

		lineCount++;
		/** if line count exceeds MAX_LINES */
		if(lineCount > MAX_LINES) {
			return printInvalid();
		}
	}

	/** if number of tweeters exceeds MAX_TWEETER */
	int numTweeters = getListSize(tweeters);
	if(numTweeters > MAX_TWEETERS) {
		return printInvalid();
	}

	sortList(tweeters);

	printf("TOP 10 TWEETERS\n");
	for(int i = 0; i < 10; i++) {
		printf("<%s>: <%d>\n", tweeters[i].name, tweeters[i].count);
	}
}
