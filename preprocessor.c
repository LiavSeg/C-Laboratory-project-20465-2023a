#include "preprocessor.h"
void preprocessor(char *source_name){
	char *macro_flag;
	char line[MAX_LINE_LENGTH];
	char name[sizeof(source_name)+POST_FIX];
	FILE *pre,*post;
	macro* macro_table = NULL;
	strcpy(name,source_name);
	strcat(name,".as");
	pre = fopen(name,"r");
	if(!pre){
		printf("Invalid file - Program termination");
		exit(0);
		}
	strcpy(name,source_name);
	strcat(name,".am");
	post = fopen(name,"w");	
	if(!post){
		printf("File's Memory allocation failed- Program termination");
		exit(0);
		}	
	fgets(line,MAX_LINE_LENGTH,pre);
	while(!feof(pre)){
		if((macro_flag = strstr(line,"mcr")))
			add_to_table(macro_flag,&macro_table,pre);
		else{
			copy_content(line,post,macro_table);
	 	 	macro_flag = NULL;
	 	 	}	
		fgets(line,MAX_LINE_LENGTH,pre);
		}
	 	free_table(macro_table);
	 	fclose(pre);
	 	fclose(post);
	} 	
	 
						
								
			 
