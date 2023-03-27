#include "isvalidargs.h"
/*
 * The function  checks if the arguments provided for a command are valid or not,
 * depending on the opcode of the command. It parses the command string and returns the 
 * command_data struct pointer after analyzing the arguments, or NULL and prints an error 
 * message if there is an error in the arguments provided.
 * 
 * Parameters:
 * command - a string representing the command to be analyzed
 * cd - a pointer to the command_data struct that will be updated with the arguments analyzed
 * error - a pointer to an integer that will be set to an error code if an error occurs
 * 
 * Returns: If the arguments are valid, the function returns a pointer to the command_data struct after 
 * analyzing the arguments. Otherwise, it returns NULL and prints an error message.
 */


command_data *isvalidargs(char *command,command_data *cd,int *error){
	char *argu1,*argu2,*temp;
	char token[MAX_LINE_SIZE];
	int i =0;
	int flag;
	/*get the first arg*/
	argu1 = strtok(NULL," ");
	if(!argu1&&!THIRD_GROUP(cd->opcode)){
		printf("********Error: Too few arguments for %s command ",command);
		return NULL;
	}
	strcpy(token,argu1);
	/*check if command belongs to third group*/
	if(THIRD_GROUP(cd->opcode)){
		if(!argu1 || !strcmp(argu1,"\n")){ 
			cd->arg1 = NULL;
			cd->arg2 = NULL;
			return arg_analysis_two(cd->arg1,cd->arg2,cd);
		}
		else{
			printf("********Error: Too many arguments for %s command ",command);
			free(cd->name);
			free(cd);
			return NULL;
		}
	}	
	/*check if command belongs to second group*/
	else if(SECOND_GROUP(cd->opcode)){
		if(argu1){
			if((argu2 = strtok(NULL," "))&&strcmp(argu2,"\n")){
				printf("********Error: Exccesive content or too many arguments for %s command ",command);
				free(cd->name);
				free(cd);
				return NULL;
				}
			else{
				/* remove any trailing spaces from argu1*/
				while(!isspace(argu1[i]))
			 		i++;
			 	argu1[i]='\0';
				/*set arguments in command_data struct and call analysis one function*/
				cd->arg1 = argu1;
				cd->arg2 = NULL;
				return arg_analysis_one(argu1,cd,error);
			}
		}
	}
	/*check if command belongs to first group*/
	else if(FIRST_GROUP(cd->opcode)&&argu1){
		if((temp = strchr(argu1,','))){
			*temp = '\0';			
			cd->arg1 = argu1;
			temp++;
		    argu2 = temp;
			temp = strchr(argu2,',');
		    i=0;
		   /*ignore any spaces*/
		    while(!isspace(argu2[i]))
				i++;
			/* check for excessive content and error handle*/
			flag = exccesive_content(&argu2[i]); 
			if(flag||temp){
				printf("********Error: Too many arguments for %s command",command);		
				free(cd->name);
				free(cd);
				return NULL;
			}		
			/*call arg_analysis_two function after assuring its the right amount of args*/
			argu2[i]='\0';
			cd->arg2 = argu2;
			return arg_analysis_two(cd->arg1,cd->arg2,cd);
		}
		/* error missing arg*/
		else{
			printf("********Error: Missing argument for %s command",command);	
			free(cd->name);
			free(cd);	
			return NULL;				
		}		
	}	
	return cd;
}
	
	
	
	
/*checks for exccesive content*/	
int exccesive_content(char *arg){
	char *token = NULL;
	char temp[MAX_LINE_SIZE];
	int flag = 0;
	strcpy(temp,arg);
	if((token=strstr(temp,",")))
		flag =1;
	else
		token =	strtok(NULL," ");
	while(token){
		if(!isspace(*token)||*token!='\n')
			flag++;
		token =	strtok(NULL," ");
	}
	return flag;
}

/* Function to analyze a command with one argument and update the command data
 * This function takes in a string representing a single argument command, a pointer to a command_data struct,
 * and a pointer to an integer to keep track of errors.
 * It processes the argument and updates the ic value in the command_data struct.
 * If there is an error, it returns NULL.
 */
command_data *arg_analysis_one(char *argument1,command_data *cd ,int *error){
	char *token;
	char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};/*Register names*/
	int i,flag1,flag2,invalid;/*flags for register args and an error flag*/
	char arg[MAX_LINE_SIZE];/*temp argument proccecor*/
	char sub_args[MAX_LINE_SIZE];/*Handles 2nd adressing method label with args*/
	i=flag1=flag2=invalid=0;
	memset(sub_args,'\0',MAX_LINE_SIZE);
	/*Check if the argument is null*/
	if(!argument1)
		return NULL;
	
	/*Check if the argument is a label*/
	if(islabel(argument1)==1){
		cd->ic = 2;
		return cd;	
		}
	/*Check if the argument is an immediate value*/	
	strcpy(arg,argument1);	
	if((token = strstr(arg,"#"))){
		token = strtok(arg,"#");
		if(token[0] == '-'|| isdigit(token[0])||token[0] == '-'){
			cd->ic = 2;
			return cd;
			}
		else 
			invalid =1;/*Invalid argument flag*/
	}
	/*Check if the argument is in the structure of addresing no.2 */
	strcpy(arg,argument1);
		if((token = strtok(arg,"("))){
			if(islabel(token)){
				strcpy(sub_args,token);
				strcat(sub_args,"|");
			}
			else
				invalid = 1;/*Invalid argument flag*/				
		}	
	/*Check if the argument is in the structure of addresing no.2 - first arg inside it*/			
		if((token = strtok(NULL,","))){
			for(i = 0 ; i < 8 ; i++)
				if(!strcmp(token,reg[i]))
					flag1 =1;/*register flag*/	
				strcat(sub_args,token);
				strcat(sub_args,"|");
				}	
		else
			invalid = 1;/*Invalid argument flag*/						
	/*Check if the argument is in the structure of addresing no.2 - second arg inside it*/
		if((token = strtok(NULL,")"))){
			for(i = 0 ; i < 8 ; i++)
				if(!strcmp(token,reg[i]))
					flag2 =1;/*register flag*/
			strcat(sub_args,token);
			strcat(token,"\0");
			if(flag1&&flag2)
				cd->ic = 3;/*register args shares a line*/
			else
				cd->ic = 4;/*at least one  arg isnt a register */
				strcpy(cd->arg1,sub_args);	
			return cd;
			}
		else 
			invalid = 1;/*Invalid argument flag*/	
	/* Handle error */
	if(invalid){		
		printf("********Error: %s is invalid argumnet ",arg);
		free(cd->name);
		free(cd);	
		return NULL;				
	}	
	return cd;						
}

/* Function to analyze two arguments of an assembly command [when its valid]
* This function takes in two strings representing arguments of an assembly command,
* and a pointer to a command_data struct. It checks whether the arguments are valid registers,
* and updates the instruction counter in the command_data struct accordingly.
* If both arguments are valid registers, the ic is updated to 2. Otherwise, it is updated to 3.
* If both argument1 and argument2 are NULL, ic is updated to 1.
* Returns a pointer to the updated command_data struct.
*/
command_data *arg_analysis_two(char *argument1,char *argument2,command_data *cd){
	char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
	int i,flag1,flag2;
	i=flag1=flag2=0;
	/*NULL case - two null args case compatible for the third cmd group 1 line for the cmd */
	if(!argument1&&!argument2){
		cd->ic = 1;
		return cd;
		}
	/*Checks if both args are register with flags updating the ic according to it */
	for(i=0;i<8;i++)
		if(!strcmp(argument1,reg[i]))
			flag1 = 1;
	for(i=0;i<8;i++)
		if(!strcmp(argument2,reg[i]))
			flag2 = 1;
			
	if(flag1&&flag2)/*both args are registers sharing one line + cmd line  */
			cd->ic=2;
		else
			cd->ic=3;/*both args are not registers 2 different lines+cmd line */		
	
	return cd;
	}	
	
	
