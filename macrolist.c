#include "preprocessor.h"
/** 
 * Adds a name and a mcro content using  - find_macro_name -find_macro_content
 * Implemented as a linked list therfore each node will get allocated and added separately
 * @param line pointer of the line the macro strats value of the macro table.
 * @param table ptr of a ptr of the macros table head
 * @param pre FILE.
 * @throws FileNotFoundException If memory cannot allocated
 *  
 */
void add_to_table(char *line, macro **table, FILE *pre){
	macro* newmcr = (macro*)malloc(sizeof(macro));
	if(!newmcr){
		printf("Memory allocation failed\n");
		free_table(*table);
		exit(0);
	}
	newmcr->name = find_macro_name(line,pre);
	newmcr->content = find_macro_content(pre);
	newmcr->next = *table;
	*table = newmcr;
}

/**
 * Searches for a macro with the given name in the macro table and writes its contents
 * to the given file stream.
 * @param table pointer to the first element of the macro table
 * @param name pointer to the name of the macro to search for
 * @param post pointer to the file stream where the contents of the macro should be written
 * @return 1 if the macro was found and its contents were successfully written to the file, 0 otherwise
 */	
int search_macro(macro *table, char *name,FILE *post){
	macro* temp = table;/* temporary pointer to traverse the macro table */
	char *string;  /* pointer to the name of the current macro being checked */
	while(temp){
   	 	string = temp->name;
   	 	if(!strcmp(string,name)){/* if the name of the current macro matches the name we're searching for */
    		fwrite(temp->content,sizeof(char),strlen(temp->content),post);
    		if(!strstr(temp->content,"\n"))/* if it doesnt contain newline char */
    			fwrite("\n",sizeof(char),1,post);	
    		return 1;
    	 }
    	temp = temp->next;	
    }
	return 0;
}
/*
 * This function frees the memory allocated for the macro table.
 *
 * @param table A pointer to the first element of the macro table.
 */ 		
void free_table(macro *table){
	macro *temp = table;
    macro *next;
    while(temp){
    	next = temp->next;
        free(temp->name);
        free(temp->content);
        free(temp);
        temp = next;
   	}
}

/**
 * Prints the names of all macros in the macro table to a temporary file, with a max length of MAX_LINE_LENGTH.
 * @param table A pointer to the first macro in the macro table.
 */
void print_macro_names(macro *table){
	macro *temp = table;/*last macro addes - head of the table*/
    macro *next;/*next macro*/
    FILE *temp_macro_names;/*temp file hold macro names*/
    long position;/*current pos*/
    char name[] = "temp_macro_names.txt";/*file's name*/
    if(!temp){/*no macros case*/
    	return;
    }
    temp_macro_names = fopen(name,"w");/*create the temp file + check for failure*/
    if(!temp_macro_names){
    	printf("Could not open macro names file - program termination\n");
    	return ;	
    }
   /*prints to the file each macro while not exceeding the line limit*/ 
    while(temp){
    	position = ftell(temp_macro_names);
    	if(position+strlen(temp->name)+1>=MAX_LINE_LENGTH)/* +1 for the blank space*/ 
    		fwrite("\n",sizeof(char),1,temp_macro_names);
    	else{		
    		fwrite(temp->name,sizeof(char),strlen(temp->name),temp_macro_names);
    		fwrite(" ",sizeof(char),1,temp_macro_names);
    	}
    	next = temp->next;
        temp = next;
   	}
	fclose(temp_macro_names);
	return ;
}


