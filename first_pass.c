#include "first_pass.h"
/* 
 * Perform the first pass of the assembly process, which involves parsing the source file,
 * building a symbol table, and generating a code table with the encoded instructions and data.
 * 
 * @param after_macro_source The name of the source file without the .as extension.
 * @throws FileNotFoundException If the source file cannot be opened.
 * @throws AssemblyErrorException If there are errors in the assembly process.
 */
void first_pass(char *after_macro_source){
    /* Declare local variables */
    FILE *fp;/*Input file*/
    char name[sizeof(after_macro_source)+NAME_POSTFIX]; /* File's name + postfix addition  */
    char line[MAX_LINE_SIZE]; /* Store a line from the input file  */
    char temp[MAX_LINE_SIZE]; /* Copy of the line  - an untokenized copy */
    int i=0, j=0;
    int IC = 0,DC = 0;/*Instruction counter,Data counter*/
    int general= 0;/*General position counter*/
    int line_count=0;/*Line counter*/
    int error_flag=0;/*Flag for errors*/
    int flag=0;/*Secondary flag for errors used in case an error found in command encoding procces*/
    label_table *lableTable = NULL; /* Symbol table */
    label_table *entry_extern_table = NULL; /* Symbol table for entry and extern labels */
    encoded_words instruction_table[RAM_SIZE]; /* Code table for encoded instructions and data */
    command_data *command_info = NULL; /* Analysis info of a command line */
    guidance_data *guidance_info = NULL; /* Analysis info of a guidance line */

    /* Initialize variables */
    IC = DC = i = j = general = error_flag = line_count = flag = 0;
    init_ints_table(instruction_table); /* Initialize the code table */


	/*Checks if all macro names are valid */
	if(!is_valid_macro()){
		printf("**********Program %s ends due to inavlid input file**********\n\n\n",after_macro_source);
		remove(after_macro_source);
		return;
	}
	else
		printf("***********Macro validation passed for %s***********\n",after_macro_source);
		printf("-----------------------------------------------------------------------------\n");		
		printf("***********Procceeds with the first pass for %s*****\n",after_macro_source);
		printf("-----------------------------------------------------------------------------\n");	
	
	/*Create the name of the source file and open it*/
	sprintf(name,"%s.am",after_macro_source);
	fp = fopen(name,"r");
	if(!fp){
		printf("Could not open file");
		exit(0);
	}	
	
	 /* Parse each line in the input file */
	while(fgets(line,MAX_LINE_SIZE,fp)&&general<MAX_LABELS){
		line_count++;
		/*skip comment line*/
		if(*line != ';'){
		strcpy(temp,line);
		command_info = send_to_command(command_info,&lableTable,line,general,&error_flag,IC);
		/*for each invalid command line check if its a guidance line*/
		if(!command_info){
			guidance_info = send_to_guidance(&lableTable,temp,general,DC);
			if(guidance_info){/*Encdoding a valid command line update the DC*/
 				/*if its entry/extern add it to a different table*/
 				flag = existed_entx(entry_extern_table,guidance_info->data,guidance_info->type);
 				if(flag==0)
 					entry_extern_table = add_entx(entry_extern_table,guidance_info->data,guidance_info->type);	
				j = guidance_data_update(guidance_info,entry_extern_table,instruction_table,general);
 				DC+=j;
 				if(flag==1)
 					guidance_info = NULL;
 			}
 		}
		/*Encdoding a valid command line update the IC*/
		if(command_info){
			flag =	command_coding(command_info,lableTable,general,instruction_table,&error_flag);
			j = command_data_update(command_info);
			tag_command_data(instruction_table,j,general);
			IC+=j;
			if(!flag)
				command_info = NULL;	
		}
		if(!guidance_info&&!command_info){/*Invalid line is an error */
			printf("------------->Error line:%d\n",line_count);
			error_flag++;
		}
		else	
			general+=j;/*Update the total counter of DC and IC when its a valid line of any kind*/
		}		
	}

	/*Skips the second pass in case an error was found*/
	if(error_flag){
		has_error(fp,lableTable,entry_extern_table,error_flag,general);
	}
	/*Go to second pass in case no errors were found*/
	else{
		second_pass(after_macro_source,lableTable,entry_extern_table,instruction_table,general,IC,DC);	
		fclose(fp);
	}
}	
		
/** Function to parse a line and generate command data based on it 
 *	@param command_info the command data structure to send the command to
 *	@param lableTable a double pointer of the symbol table
 *	@param line The input line to be parsed.
 *	@param general flag indicating whether the command is a general command or not
 *	@param error The current errors counter.
 *	@return a pointer to the generated command data.
 */	
command_data *send_to_command(command_data *command_info,label_table **lableTable,char *line,int general,int *error,int IC){
	/* Declare local variables */
	char *token;/*tokenizing the input for further parsing*/
	char *symbol;/*stores a symbol if it exists*/
	char type[] = "C";/*marking the line type as command*/
	int existed;
	/* Symbol and command parsing */
	/* Check if a symbol may exist (indicated by a colon ':') and if it does, continue with further parsing*/
	token = strstr(line, ":"); 		 	  
	token = strstr(line,":");
		if(token){
			token = strtok(line,":");
			if(!label_errors(token))/*return NULL if there is a label error*/
				return NULL;
			/*Check if the first token is a valid label, add it to the label table after cmd validation*/
			if(islabel(token)){
				existed = find_label(*lableTable,token);
				if(existed>=0){
					printf("********Error:Label %s is already found ",token);
					return NULL;
				}
				symbol = (char *)malloc(strlen(token)+1);
				strcpy(symbol,token);	/*keeps a valid symbol*/
				token = strtok(NULL," ");
				command_info = is_command(token,error);	/*command parsing*/
				if(command_info)/*for valid command line add a tagged symbol to the label table*/
					*lableTable=add_label(*lableTable,symbol,type,general,IC);
				free(symbol);
			}
		}
		else{
		/*Parse the command for a command line with no symbol*/
			token = strtok(line," ");
			command_info = is_command(token,error);
			}
	return command_info;
}					
	


/** Function to parse a line and generate guidance data based on it
 *	@param lableTable a double pointer of the symbol table
 *	@param line The input line to be parsed.
 *	@param DC The current data counter value.
 *	@return a pointer to the generated guidance data.
 */		
guidance_data *send_to_guidance(label_table **lableTable,char *line,int general,int DC){
	/* Declare local variables */
	char *token = NULL;/* Tokenizing the input for further parsing */
 	char temp[MAX_LINE_SIZE];/*Copy of the line - an untokenized copy */
 	char *symbol;/*Stores a symbol if it exists*/
 	int existed;
 	guidance_data *guide=NULL;
 	/* Copy the input line to a temporary variable to avoid modifying the original input */
 	strcpy(temp,line);
    
  	/* Symbol and command parsing */
	
 	token = strstr(temp,":");
  	if(!token){/*guidance parsing for a guidance line with no symbol prior to it*/
  		token = strstr(line,".");
  		if(token){
    		token=strtok(line,".");
     	    guide=processGuidance(*lableTable,token,temp);
     	 }
  	}
    /* Check if a symbol may exist (indicated by a colon ':') and if it does, continue with further parsing*/
    else if(token){
 		token = strtok(temp,":");
  		if(!islabel(token))
				return NULL;
  		if(islabel(token)){
  		existed = find_label(*lableTable,token);
			if(existed>=0){
				printf("********Error:Label %s is already found ",token);
				return NULL;
			}
  		symbol = (char *)malloc(strlen(token)+1);
  		strcpy(symbol,token);
  		token = strtok(NULL,".");
  		if(token!=NULL){
				if(isspace(*token))/* Skip spaces after the symbol */
					token = strtok(NULL,".");
       	guide=processGuidance(*lableTable,token,line);
       	if(guide)/* Add the label to the symbol table if the guidance data was generated successfully */
       		*lableTable = add_label(*lableTable,symbol,guide->type,general,DC);		
       			}
       			free(symbol);
       		}
 				}
 	return guide;
 	}

  
  
  
 

/*Function to init instruction table*/
void init_ints_table(encoded_words *RAM){
	int i;
	for(i = 0;i<RAM_SIZE;i++){
		memset(RAM[i].encoding,0,WORD_SIZE+1);
		memset(RAM[i].command_name,0,MAX_LABEL_LENGTH);
		RAM[i].location =-1;
		memset(RAM[i].type,0,TYPE_OF_CMD_LENGTH);		
		
		}
 	}	
 /*Function to add entry/extern lables */	
 void add_entx_to_ltable(label_table **lt,label_table *entx){
 	label_table *temp = entx;
 
 	while(temp){
 		if(!strcmp(temp->type,"EX"))
 			*lt = add_label(*lt,temp->name,"ex",0,0);
 			temp=temp->next;
		 	}
 }
 
 
/*Function to check if its a saved word*/ 
 int is_saved(char *input){
	int i;
	char *guid[] = {"data","string","entry","extern"};    /* guidance related saved words */
	char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};/* register names*/
    /* checks if the input is a saved word*/
	for(i=0;i<NUM_OF_REGISTERS;i++)
			if(!strcmp(input,reg[i]))
				return 1;
	for(i=0;i<NUM_OF_GUIDELINES;i++)
			if(!strcmp(input,guid[i]))
				return 1;		
	return 0;
}																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																												
 
 
 
 
/*Function to update the instruction table in case of a valid guidance line*/ 
int guidance_data_update(guidance_data *guide,label_table *data_image,encoded_words *RAM,int general){
	int j = 0;
	if(!guide)
		return 0;
 	guidance_coding(guide,general,RAM);/*encodmment of the guidance data*/ 
 	j=guide->dc;
 	free(guide);
 	
 	return j;/*how many slots this line "donated" to the instruction table */ 		
}



/*Function to update the instruction table in case of a valid command line*/ 
int command_data_update(command_data *command_info){
	int j = 0;
	j = command_info->ic;
	free(command_info->name);
	free(command_info);	
 	return j;/*how many slots this line "donated" to the instruction table */ 		
}
/*Marks cmd lines for second pass division */  
void tag_command_data(encoded_words *RAM,int j,int IC){
	int i;
	for(i = IC;i<IC+j;i++)
		strcpy(RAM[i].type,"C");
}  
  
 /*Function to terminate correctly and infrom about any errors */ 
 void has_error(FILE *fp,label_table *lableTable,label_table *data_image,int error_flag,int general){
	printf("\n***********************%d Errors found************Assembler won't proceed\n\n",error_flag); 	       			if(general == MAX_LABELS)
			printf("Fatal error********** RAM is fully loaded,cant procced*** \n\n");
		free_label_table(lableTable);
		free_label_table(data_image);
		fclose(fp);	
}
/**
 * Checks if all the macro names in the temporary macro names file are valid-
 * they not saved words
 * @return 1 if all macro names are valid, otherwise 0
 */
int is_valid_macro(){
	FILE * fp;/*temp file*/
	char *token=NULL;/*macro names*/
	char line[MAX_LINE_SIZE];
	char name[] = "temp_macro_names.txt";/*file name*/
	int flag = 1;

	fp = fopen(name,"r");
	/*no macros*/
	if(!fp)
		return 1;

	/*goes over the temp macro names file line by line*/
	while(fgets(line,MAX_LINE_SIZE,fp)){
		token = strtok(line," ");
		while(token){
			if(is_saved(token)||isopcode(token,0)!=-1){
				printf("\n\n**********Fatal error: macro name %s is a saved word**********\n",token);
				flag = remove(name);
				if(flag){
					printf("Could not delete temp macro names file - abort \n");
					exit(0);
				}
				fclose(fp);
				return 0;
			}
			else
				token = strtok(NULL," ");
		}
	}
	flag = remove(name);/*file ended and all mcrs are valid */
	/*error check*/
	if(flag){
		printf("Could not delete temp macro names file - abort \n");
		fclose(fp);
		exit(0);	
	}
	
	fclose(fp);
	return 1;/*valid mcr names*/
}  
  
  
   
  
  
  
  
  
  


  
  
  
  
  
 	 	
