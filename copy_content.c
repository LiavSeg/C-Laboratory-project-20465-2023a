#include "preprocessor.h"

void copy_content(char *str,FILE *post,macro *table){
	char *token = strtok(str," \t\n");
	int flag;

	while(token){
		flag = search_macro(table,token,post);
	 	if(!flag){	
			fwrite(token,sizeof(char),strlen(token),post);
			fwrite(" ", sizeof(char), 1, post);
			}		
		token = strtok(NULL," \t\n");
		}	
		if(!flag)
			fwrite("\n", sizeof(char), 1, post);
	}		
