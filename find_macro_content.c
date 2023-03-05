#include "macrofunc.h"

char  *find_macro_content(FILE* pre){
	char *macro_lines,*macro_end;
	char temp[MAX_LINE_LENGTH];
  macro_lines = (char*)malloc(MAX_LINE_LENGTH);
	if(!macro_lines){
		printf("Memory allocation failed");
		fclose(pre);
		exit(0);
		}
	macro_lines[0]='\0';
	fgets(temp,MAX_LINE_LENGTH,pre);
			if(!isblank(temp)){
				if(!(macro_end = strstr(temp,"endmcr")))
					strcat(macro_lines,temp);
					fgets(temp,MAX_LINE_LENGTH,pre);
					}
					
		while(!(macro_end = strstr(temp,"endmcr"))){
			if(!isblank(temp))
					macro_lines = (char*)realloc(macro_lines,MAX_LINE_LENGTH);
			if(macro_lines){										
				strcat(macro_lines,temp);
				fgets(temp,MAX_LINE_LENGTH,pre);
				}
			else{
				printf("Memory allocation failed");
			fclose(pre);
			exit(0);
				}
			}	
	return macro_lines;
}
