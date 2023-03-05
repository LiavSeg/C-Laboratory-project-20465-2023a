#include "macrofunc.h"

char *find_macro_name(char *start,FILE *pre){
	char *temp,*name;
	name = (char*)malloc(MAX_LINE_LENGTH);
	temp = strtok(start," \t\n");
	temp = strtok(NULL," \t\n");
	strcpy(name,temp);
	while(temp != NULL){
		temp = strtok(NULL," \t");
		if(temp){
			free(name);
			printf("Excessive text after macro's name");
			fclose(pre);	
			exit(0);	
			}
		}
	return name;
	}
