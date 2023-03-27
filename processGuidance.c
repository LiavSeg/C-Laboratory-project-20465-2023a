#include "guidance_analysis.h"
/* Function  generate and validate a guidance line
 * This function takes in a pointer to a label_table struct, a string representing a label,
 * and a string representing a line of assembly code. It processes the guidance data in the 
 * line and returns a pointer to a guidance_data struct with the encoded data.
 * If the line is invalid or contains an invalid command, NULL is returned.
 */
guidance_data *processGuidance(label_table *lt,char *label,char *line){       
	/*Declare local variables*/	
	guidance_data *gd;/*Declare pointer to guidance_data struct*/

	char *token = strtok(label," ");/*Tokenize label*/
	/*If the tokenized label is empty remove the newline and print an error message  return NULL */
	if(!token)
		return NULL;
	
    /*If the tokenized label is entry or extern */
    if(!strcmp(token, "entry")||!strcmp(token, "extern")){
    	gd = processEntryOrExtern(lt,token,line);
       	return gd;
	}
	/*If the tokenized label is string or extern */
	else if(!strcmp(token, "string")){
    	gd = processGuidanceString(token,line);
    	return gd;
    }
    /*If the tokenized label is data*/
    else if(!strcmp(token,"data")){
    		gd = processGuidanceData(line);
    		return gd;
    }
    /*check if its not an invalid cmd line to avoid duplicated error messages*/	
	else if(isopcode(token,0)!=-1){
		printf("********Error: Invalid guidance line");		
		return NULL;
		}
	 
		printf("********Error: Invalid guidance line");	 
	return NULL;
}

/**
* Encodes the guidance data into binary and updates the encoded RAM.
* @param gd a pointer to a guidance_data struct containing the guidance data to encode.
* @param DC the current value of the data counter.
* @param RAM a pointer to an encoded_words array representing the encoded RAM.
*/
void guidance_coding(guidance_data *gd,int DC,encoded_words *RAM){
	/*Declare local variables*/
	int i = DC;/*Initialize the index i to DC*/
	int k;/* index k for use later*/
	char *token;/*  a pointer to a char to use with strtok()*/
	unsigned int binary=0;/* Initialize binary to zero used for binary encoding*/
	int c= ' ';/*stors a string's char*/
	
	/*Data line encodment with a single number*/
	if(!strcmp(gd->type,"DT")){
		token = strstr(gd->data,",");/*data is a single number*/
			if(!token){
				binary = (unsigned int)atoi(gd->data);/*casting a char to an unsigned int*/
				/* Copy the command name and location to the RAM array*/
				strcpy(RAM[i].command_name,gd->data);
				RAM[i].location = i;
				STCODE(binary);/*binary encodment macro*/
				return;
				}
			else
				token= strtok(gd->data,",");
				for(i = DC; i<DC+gd->dc&&token;i++){
					binary = (unsigned int)atoi(token);/*casting a char to an unsigned int*/
					STCODE(binary);/*binary encodment*/
					 /* Copy the command name and location to the RAM array*/
					strcpy(RAM[i].command_name,token);
					strcpy(RAM[i].type,"DT");
					RAM[i].location = i;
					token = strtok(NULL,",");	
				}	
			}	
	/*String line encodment*/
	else if(!strcmp(gd->type,"ST")){
		token = gd->data;
		for(i = DC,	k=0; i<DC+gd->dc;k++,i++){
			c = (int)token[k];/*casting a char to an int*/
			STCODE(c);/*binary encodment macro*/
			RAM[i].command_name[0]=c;
			strcpy(RAM[i].type,"ST");
			RAM[i].location = i;					
			
		}
				
	}		
}	

		
char* strdup(const char* str){
    size_t len = strlen(str);
    char* copy = malloc(len+1);
    if(copy == NULL){
        printf("Memory allocation failed");
        return NULL;
    }
    strcpy(copy,str);
    return copy;
}






