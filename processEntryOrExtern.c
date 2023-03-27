#include "guidance_analysis.h"
/**
 * This function processes an entry or extern label and returns a pointer to a guidance_data struct
 * with the relevant information. It takes in a pointer to a label_table struct, a label string, 
 * and a line string. If it is an entry label, it removes the ".entry" prefix from the line and 
 * extracts the label name. If it is an extern label, it extracts the label name from the line. 
 * It returns NULL if there is an error, otherwise it returns a pointer to a guidance_data struct
 * with the label type, label name, and DC value set.
 */
guidance_data *processEntryOrExtern(label_table *lt,char *label,char* line){ 
	/*Declare local variables*/	
 	const char *ent = ".entry ";
 	const  char *ext = ".extern ";
    char temp[MAX_LINE_SIZE];
    char tag[TYPE_OF_GDN_LENGTH];
    char data[MAX_LINE_SIZE];
	char *token,*flag;

    strcpy(temp,label);
    if(!strcmp(temp,"entry")){
   		remove_prefix(line,ent);
   		strcpy(tag,"EN");
   	}
	else{
		remove_prefix(line,ext);
		strcpy(tag,"EX");
	}
   flag = strdup(line);
   if(!flag)/*Mem failure message happened internally */
		return NULL;
	
    token = strtok(flag," ");
    strcpy(data,token);	
	if(*token =='\n'){
		printf("********Error: empty %s argument is invalid",temp);
    	free(flag);
    	return NULL;
	}
    
	if(!islabel(token)&&*token!='\n'){
		printf("********Error: %s argument has to be a valid label",temp);
    	free(flag);
    	return NULL;
	}
	else if(islabel(data)){
		while(token){
    		token = strtok(NULL," \n");
    		if(token!=NULL&&*token!='\n'){
    			printf("********Error: exccesive content ---%s--- after an %s label ",token,temp);
    			free(flag);
    			return NULL;
    		}
		}
	}

 	 if(!strcmp(temp,"extern"))
    	return set_entx(data,tag,flag);
    
	else if(!strcmp(temp,"entry")&&is_entry(data))
 		return set_entx(data,tag,flag);
 	else 
 		free(flag);
 	   	
	return NULL;
}

/*removes entry/ extern prefix from the input line*/	
void remove_prefix(char *line,const char *prefix){
    size_t prefix_len = strlen(prefix);
    if(!strncmp(line, prefix, prefix_len)){
     	memmove(line,line+prefix_len, strlen(line)-prefix_len+1);
    }
}
		

guidance_data* set_entx(char *data,char *type,char *flag){
	/*Declare local variables*/	
	guidance_data *gd; /*Declare a guidance data ptr for dynamic allocation*/
	gd = (guidance_data *)malloc(sizeof(guidance_data));
 			/*Mem alloc check*/     		
    if(!gd){
    	printf("Memory allocation failed");
      	return NULL;
     }
     /*Setting the data*/
    strcpy(gd->type,type);
    strcpy(gd->data,data);
   	gd->dc =0; 
   	free(flag);
	return gd;

}
/*checks if this entry has only upper case letters *Note* implemented according to forum ansewrs (danny) regarding it*/
int is_entry(char *data){
	char temp[MAX_LABEL_LENGTH];
	int i;
	strcpy(temp,data);
	for(i=0;i<strlen(temp);i++)
		if(!isupper(temp[i])){
			printf("********Error: entry label has to contain upper case letters only ");
			return 0;
		}
	
	return 1;		
}

/**
 * Checks if an entry/extern label with the given name already exists in the given label table.
 * @param lt The label table to search in.
 * @param label The label name to check.
 * @param type The label type to check (must be "EN" or "EX").
 * @return 0 if the label is not defined, 1 if it is defined and has the same type, or -1 if the type is invalid.
 */
int existed_entx(label_table *lt,char *label,char *type){
	char name[MAX_LABEL_LENGTH];
	label_table  *name_ptr;
	int find;
	find = find_label(lt,label);
	strcpy(name,label);
		/*checks if this label already defined*/
	if(find>=0){
		name_ptr = get_label(lt,name);
		if(strcmp(name_ptr->type,type)){
   	 		printf("********Error: Label cant be extern and enrty");   	/*defined*/
   	 		return 1;
   	 		}
		}
	if ((find == -1)&&(!strcmp(type,"EN")||!strcmp(type,"EX")))	
		return 0;
	
	if(strcmp(type,"EN")||strcmp(type,"EX"))
		return -1;

	return 0;
}


