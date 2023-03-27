#include "guidance_analysis.h"
/* Function to generate and validate a string guidance line info 
 * This function takes in a pointer to a label_table struct, a string representing a label,
 * and a string representing a line of assembly code. It processes the guidance data in the
 * line and returns a pointer to a guidance_data struct with the encoded data.
 * If 1 the line is invalid or contains an invalid command, NULL is returned.
 */ 
guidance_data* processGuidanceString(char* label,char *line){
	guidance_data * gd=NULL;/*a stucrt which hold neccecary data of a valid guidance line; gd = guidance data */
	char temp[MAX_LINE_SIZE];
	char data[MAX_LINE_SIZE];
	char *end;
	int i=0;/*counter which used as a counter for the "local" dc if the guidance line is valid  */
	char *token,*flag;/*a token to proccess the input, a flag which be used to dynamiclly copy the source input*/
 	strcpy(temp,label);/*keeping a copy of the label which may be needed  */
 	flag = strdup(line);/*a self implement for a known fucntion -*/
	if(!flag)
		return NULL;
	if(strstr(flag,":")){/*tokenizing the source line when  a label exists prior to the string data*/
		token = strtok(flag,":");/*the line without the label*/
 		if(*token == '.')
 				token = strtok(NULL," ");
 		else{	
 			token = strtok(NULL," ");/*removing the .string*/
   			token = strtok(NULL," ");/*here should be the string data if its valid*/
		}
	}	
	else{	/*tokenizing the source line when  a label does not exist prior to the string data*/
 		token = strtok(flag," ");/*removing the .string*/
   		token = strtok(NULL," ");/*here should be the string data if its valid*/
   	}
	end = strstr(token,"\n");
	if(end)
		*end='\0';
	if(!token || *token=='\n'){/*Checking for an empty string input*/
   		printf("********Error: Empty string content");
    	free(flag);/*flag*/
    	return NULL;
   	}

    if(strstr(token,"\""))/*checks if a " starts the string*/
    	token++;
	else{
		printf("********Error: A string must start with a \"");
		free(flag);
    	return NULL;
   	}	
   	if(strstr(token,"\""))/*checks if a " ends the string*/
   		strcpy(data,token);/*in this case it might be a valid string*/
    else{
		printf("********Error: A string must end with a \"");
    	free(flag);
    	return NULL;
	}
    while(token){/*checking if the rest of the line is empty which may lead to a valid\invalid output*/
    	token = strtok(NULL," \n");
    	if(token){
    		printf("********Error: exccesive content ---%s--- after a %s label ",token,temp);
    		free(flag);
    		return NULL;
    	}
		else{/*in this case its a valid string */
			gd = (guidance_data *)malloc(sizeof(guidance_data));
			gd->dc = 0;/*init local dc*/
			strcpy(gd->type,"ST");/*tagging it as a string*/
			token = data;
			i=0;
			while(*token!='\"'){/*removing "*/
				token++;
				i++;
			}
			*token	='\0';
			i++;
			gd->dc=strlen(data)+1;/*total number of chars- "local dc"*/
			strcpy(gd->data,data);/*keeping the string which needs to be encoded"*/
			free(flag);
			return gd;
		}
	}
	return gd;
} /*END OF processGuidanceString "*/         

