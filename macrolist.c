#include "preprocessor.h"

	void add_to_table(char *line, macro **table, FILE *pre)
	{
		macro* newmcr = (macro*)malloc(sizeof(macro));
		newmcr->name = find_macro_name(line,pre);
		newmcr->content = find_macro_content(pre);
		newmcr->next = *table;
		*table = newmcr;
			
	}
	
	  int search_macro(macro *table, char *name,FILE *post){
	 	macro* temp = table;
	 	char *string;
	 	while (temp){
   	 string = temp->name;
   	 if(!strcmp(string,name)){
    	fwrite(temp->content,sizeof(char),strlen(temp->content),post);
    	return 1;
    	 }
    	temp = temp->next;	
    	}
   	return 0;
		}
		
		void free_table(macro *table){
    macro *temp = table;
    macro *next;
    while(temp) {
  	 next = temp->next;
       free(temp->name);
        free(temp->content);
        free(temp);
        temp = next;
        
		 }
		}
