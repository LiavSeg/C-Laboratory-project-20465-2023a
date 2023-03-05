#include "isblank.h"

int isblank(char *line){
	int i;
	for(i = 0; i < MAX_LINE_LENGTH;i++)
	 		if(!isspace(line[i]))
	 			return 0;
	 return 1;		
	 }
