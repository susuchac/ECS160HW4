#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 500

// parse_header
//
// this function parses the first line of a csv file and returns
// the column number where the string "name" is.
//
// arguments -
//   char* headerLine: the first line of a csv file
//
// returns -
//   int: the column number of the string "name" or -1 if not found
static int parse_header(const char *headerLine){
	int nameColumn = -1;
	char strCopy[MAX_LINE_LENGTH];
	strncpy(strCopy, headerLine, strlen(headerLine));


	/* get the first token */
	char *token = strtok(strCopy, ",");
	int iter = 0;

	/* walk through other tokens */
	while(token != NULL && strcmp(token, "\"name\"") !=0) {
		iter++;
		token = strtok(NULL, ",");
	}

	if(token != NULL){
		nameColumn = iter;
	}

	return nameColumn;
}


struct tweeter{
	char* name;
	int count;
};

// check_if_line_too_long
//
// checks if the line has a new line character at the end
// if it does not then return -1, otherwise 0
//
// **NOTE** removes new line character from string
//
// arguments -
//   char* line: the line to check
//
// returns -
//   int: 0 or 1 if there is a new line or not respectively

static int check_if_line_too_long(char *line){
	if(strtok(line, "\n") == NULL){
		return -1;
	} else {
		return 0;
	}

}


//finds a tweeter and returns the index to that tweeter in the tweeters array, or returns -1 on failing
int findTweeter (struct tweeter* tweeters, int numtweeters, char* name){
	if (tweeters == NULL || name == NULL) return -1;

	int i;
	for (i = 0; i < numtweeters; i++){
		if (strcmp(tweeters[i].name, name) == 0){
			return i;
		}
	}
	return -1;
}

void printTopTen(FILE *file, int nameInd){
	char buf[MAX_LINE_LENGTH];
	struct tweeter* tweeters = NULL;
	int numtweeters = 0;

	int numLines = 1;

	while (1){
		fgets(buf, MAX_LINE_LENGTH, file);

		if(check_if_line_too_long(buf) != 0){
			fprintf(stderr, "ERROR: Line is too long (must be < %d)", MAX_LINE_LENGTH);
			exit(-1);
		}

		//Break out of loop at end of file
		if (feof(file)){
			break;
		}

		numLines++;
		if (numLines >= 20000){
			fprintf(stderr, "File has more than 20,000 lines\n");
		}

		//strtok up to "name" column
		int i;
		strtok(buf, ",");
		for (i = 1; i < nameInd; i++){
			strtok(NULL, ",");
		}


		char* thisName = strtok(NULL, ",");
		if (thisName == NULL){
			fprintf(stderr, "Tweet found with no name\n");
			return;
		}

		//handle either incrementing already existing tweet count for tweeter or creating a new one
		int tweeterInd = findTweeter(tweeters, numtweeters, thisName);
		if (tweeterInd == -1){
			numtweeters++;
			tweeters = realloc(tweeters, sizeof(struct tweeter) * numtweeters);
			tweeters[numtweeters-1].name = (char*)malloc(sizeof(char) * (strlen(thisName) + 1));
			strcpy(tweeters[numtweeters-1].name, thisName);
			tweeters[numtweeters-1].count = 1;
		} else{
			tweeters[tweeterInd].count++;
		}
	}

	//find top 10 tweeters
	int max;
	if (numtweeters > 10) max = 10;
	else max = numtweeters;
	int i;
	for (i = 0; i < max; i++){
		int maxind = 0;
		int j;
		for (j = 1; j < numtweeters; j++){
			if (tweeters[maxind].count < tweeters[j].count){
				maxind = j;
			}
		}
		printf("%s: %d\n", tweeters[maxind].name, tweeters[maxind].count);
		tweeters[maxind].count = -1;
	}
}

int main(int argv, char **argc) {
	char *filepath;
	FILE *fd;
	if (argv != 2) {
		fprintf(stderr, "ERROR: This program takes one argument:"
				" maxTweeter.out <path to csv>\n");
		return -1;
	}

	filepath = argc[1];
	fd = fopen(filepath, "r");

	if (fd == NULL) {
		fprintf(stderr, "ERROR: Could not open file: %s for reading\n", filepath);
		return -1;
	}

	char line[MAX_LINE_LENGTH];

	if(fgets(line, MAX_LINE_LENGTH, fd) == NULL){
		fprintf(stderr, "ERROR: File is empty\n");
		return -1;
	}

	if(check_if_line_too_long(line) != 0){
		fprintf(stderr, "ERROR: Line is too long (must be < %d)", MAX_LINE_LENGTH);
		return -1;
	}

	int nameColumn = parse_header(line);


	if(nameColumn < 0){
		fprintf(stderr, "ERROR: Could not find name column");
		return -1;
	}

	if (fgets(line, MAX_LINE_LENGTH, fd) == NULL) {
		fprintf(stderr, "ERROR: File only has header\n");
		return -1;
	}

	//rewind file back to start and remove header
	rewind(fd);
	fgets(line, MAX_LINE_LENGTH, fd);

	printTopTen(fd, nameColumn);

	return 0;
}
