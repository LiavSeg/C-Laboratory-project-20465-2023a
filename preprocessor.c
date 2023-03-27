#include "preprocessor.h"
/*
 * This function preprocesses a given file, extracts macros from and writes 
 * the result to an output file.
 *
 * Command line arguments:
 * - source_name: name of the source file/s to preprocess.
 */


int preprocessor(char *source_name){
	/*Variable declaration and initialization*/	
	char *macro_flag;
	char line[MAX_LINE_LENGTH];
	char name[sizeof(source_name)+POST_FIX];
	FILE *pre,*post;
	macro* macro_table = NULL;
	/* Name handle and open the input file */
	strcpy(name,source_name);
	strcat(name,".as");
	pre = fopen(name,"r");
	if(!pre){
		printf("Invalid file - Program termination\n");
		return 0;
	}
	/* Name handle and open the output file */
	strcpy(name,source_name);
	strcat(name,".am");
	post = fopen(name,"w");	
	if(!post){
		printf("File's Memory allocation failed- Program termination\n");
		exit(0);
	}	
	/* Read each line of the input file */
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
	print_macro_names(macro_table);/*prints the mcr names to a temp file*/
	free_table(macro_table);
	fclose(pre);
	fclose(post);
	return 1;
} 	

/* This function copies the contents of the given string -str to the file -post,
 * but replaces any macro names found in `str` with their definitions in the  macro table.
 */

void copy_content(char *str,FILE *post,macro *table){
	char *token = strtok(str," \t\n");
	int flag=-1;
	while(token){ /*Iterate through each word of the line*/
		flag = search_macro(table,token,post);
	 	if(!flag){	
			/* comma is 1st char, dont space it and go back 1 btye backwards*/
			if(*token == ','&&strlen(token)>1)
				fseek(post, -1, SEEK_CUR);
			fwrite(token,sizeof(char),strlen(token),post);
			if(!(strstr(token,",")))/* comma isn't part of the word, space it */
				fwrite(" ", sizeof(char), 1, post);	
			}		
		token = strtok(NULL," \t\n");/*next token*/
	}	
	if(!flag)/*end of line go to a new line*/
		fwrite("\n", sizeof(char), 1, post);
}			 
						

 /**
  * This function reads a macro's content from a file pointer and returns it as a string.
  * It dynamically allocates memory for the string based on the length of the content.
  * It concatenates all lines of the macro into a single string, removing whitespaces and newlines.
  * The function stops reading when it reaches the line that contains "endmcr".
  * If the function fails to allocate memory, it prints an error message and exits the program.
  * lines 20~40 first pass to fill thhe dynmically allocated string and if needed lines 45~67 will use strcat
  * to keep and adding it 
    Arguments: pre- a pointer to a FILE, which is the file pointer to the input file.
   *Return value: macro_lines- a pointer to a char, which is the string containing the macro's content.
 */

char *find_macro_content(FILE* pre){
	char *macro_lines,*macro_end,*token	;
	char temp[MAX_LINE_LENGTH];
    
	/*Memory allocation for macro_lines and check if failed*/
    macro_lines = (char*)malloc(MAX_LINE_LENGTH);
	if(!macro_lines){
		printf("Memory allocation failed");
		fclose(pre);
		exit(0);
	}
	/*Clear macro_lines and get the first line of macro content*/
	macro_lines[0]='\0';
	fgets(temp,MAX_LINE_LENGTH,pre);	
		/*do it while its not a blank line*/
		if(!isblank(temp)){
			/*Check if the line contains endmcr stamp*/
			if(!(macro_end = strstr(temp,"endmcr")))
				token = strtok(temp," \t\n");
				while(token){
					strcat(macro_lines,token);
					if((!strstr(token,",")))
						strcat(macro_lines," ");
					token = strtok(NULL," \t\n");
				}
				/*Get the next line*/
				fgets(temp,MAX_LINE_LENGTH,pre);
		}
	/*Loop through the remaining lines until "endmcr" is found*/				
	while(!(macro_end = strstr(temp,"endmcr"))){
		strcat(macro_lines,"\n");
		if(!isblank(temp))
			/*expand the currnt string and add the rest of the macro content*/
			macro_lines = (char*)realloc(macro_lines,MAX_LINE_LENGTH);
			if(macro_lines){											
				token = strtok(temp," \t\n");
			while(token){
				strcat(macro_lines,token);
				if((!strstr(token,",")))
					strcat(macro_lines," ");
				token = strtok(NULL," \t\n");
				}
				fgets(temp,MAX_LINE_LENGTH,pre);
			}
			else{
				printf("Memory allocation failed");
				fclose(pre);
				exit(0);
			}
	strcat(macro_lines,"\n");	
	}	
	return macro_lines;
}

/*
 * Given a string starting from "mcr", find and return the macro name.
 * The macro name is the second string in the "mcr" line.
 * Arguments start - a string that starts with "mcr".
 *  		 pre - a file pointer to the preprocessor input file.
 * Returns A dynamically allocated string containing the macro name - free outside this func
 */

char *find_macro_name(char *start,FILE *pre){
	char *temp,*name;
	name = (char*)malloc(MAX_LINE_LENGTH);
	temp = strtok(start," \t\n");/*tokenizing "mcr"*/
	temp = strtok(NULL," \t\n");/*gets macro's name*/
	strcpy(name,temp);
	/*Excessive text handle*/
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


/*checks if there is exccesive content*/
int isblank(char *line){
	int i;
	for(i = 0; i < MAX_LINE_LENGTH;i++)
	 	if(!isspace(line[i]))
	 		return 0;
	return 1;		
}



								
			 
