#include "is_command.h"
/*
 * Check if the given line is a valid assembly command, and return a pointer to a command_data struct
 * that contains information about the command if it is valid.
 *
 * Parameters:
 *  - line: the line of code to check
 *  - error: a pointer to an integer that will be set to an error code if an error occurs
 *
 * Returns:
 *  - A pointer to a command_data struct that contains information about the command, or NULL if the
 *    line is not a valid command.
 */	
command_data *is_command(char *line,int *error){
	char temp[MAX_LINE_SIZE];
	int opcode;
	command_data *cdptr = NULL;
	memset(temp,'\0',MAX_LINE_SIZE);
	/* Checks if the line is a comment */
	if(line[0]=='.')
		return NULL;
	strcpy(temp,line);
	
	/* checks if the command is a saved word */
	if(!(is_saved(temp))){
		opcode = isopcode(temp,1);/* checks if the command is a valid opcode */

		if(opcode<0)
			return NULL;
		else{
			cdptr =(command_data*)malloc(sizeof(command_data));	
			if(!cdptr){
				printf("memory allocation failed");
				return NULL;
			}
			else{	
				cdptr->name = NULL;
				cdptr->ic = 0;
				cdptr->opcode=opcode;
				cdptr = isvalidargs(temp,cdptr,error);/* checks if the command has valid arguments */
		}
		if(!cdptr){
   	 		free(cdptr);
    		return NULL;
		}
		else{    
    		cdptr->name = (char*)malloc(strlen(temp)+1);    
    		strcpy(cdptr->name,temp);    
    		return cdptr;
		}
		if(cdptr == NULL){
    		free(cdptr->name);
			}
		free(cdptr);		
		}
	}	
	printf("********Error: Command cant be a saved word ");			
	return NULL;
}			
				
	/*checks if a cmd is a valid cmd*/					
int isopcode(char *label,int k){
	int i;

	char *opcodes[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};

	if(!label)
		return -1;
	for(i=0;i<NUM_OF_COMMANDS;i++){
		if(!strcmp(label,opcodes[i]))
			return i;
			}
	if(k==1)/*in case this func was called during a cmd prasing*/
		printf("********Error: Unidentified cmd ");
	return -1;	
	}		

