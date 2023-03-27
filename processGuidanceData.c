#include "guidance_analysis.h"
/** 
 * This function processes a data guidance instruction 
 * checks for any errors and compatibility of the input
 * Parameter line - a string that represent the parsed line
 * returns a pointer to a guidance_data struct with the appropriate info
 * if the input is valid, otherwise it returns NULL.
 */
guidance_data* processGuidanceData(char* line){
	/*Declare local variables*/		
	char *token,*flag,*ptr;
	char temp[MAX_LINE_SIZE];/*Data string - will be parsed and modified hence the copy below*/
	char data[MAX_LINE_SIZE];/*Data copy*/
	char tag[TYPE_OF_GDN_LENGTH] = "DT";
	int count,commas,total;
	count=total=0;/*count will be a valid numbers counter,total will be total values of the data line*/
	flag = strdup(line);/* Make a copy of the line for parsing (important info: this func is implemnted below) */
	token = input_parsing(flag,temp);/*input_parsing*/
	
	/* Check for errors in the parsing */
	if((!flag || !token))/*memory managment already happend internally in both cases*/
		return NULL;	
   	else
		commas = count_commas(token);/* Count the number of commas */
	
	/* invalid char was found */
	if(commas<0){
		free(flag);
		return NULL;
	}
	/*single digit/no commas */
	else if(commas == 0){
		count = num_is_valid(token);
		if(!count){
			printf("********Error: Invalid data member: %s ",token);
			free(flag);
			return NULL;
		}
		else
			total++;
	}/*if its valid - at least two numbers */		
	else if(commas>0){
		ptr = token;
		token = strstr(temp,"\n");/* Find the end of the string */
		if(token)
			*token ='\0';
		/* Terminate the string and keep it, if its a valid data it will be return with the struct */
		strcpy(data,temp);
		token = strtok(temp,",");
		/*iterating through the data string and looking for invalid values*/
		while(token!=NULL){
    		count = num_is_valid(token);
   			if(!count){/*Invalid number inform about it*/
    	    	printf("********Error: Invalid data member: %s ",token);
        		free(flag);
        		return NULL;
    		}
    		/*a valid number, increment total by one*/
    		else{
        		total++;
    		}
    		token = strtok(NULL,",");/*look for the next value*/
	}
	strcpy(temp,data);/*switching data's string name to make it fit for single digit case reference - line 47*/
	/* Get the next token to check for excessive valuse/content */
	ptr = strtok(NULL," \n");
	if(ptr != NULL){
    	printf("********Error: excessive content  after a  label ");
    	free(flag);
    	return NULL;
		}
	}
	/*"The fence post problem" - for n numbers must be n-1 commas therefore it has to be 1 */
	if(total-commas==1){
		return set_data(temp,tag,flag,total);
	}
	else{/*invalid data string */
		free(flag);
		printf("********Error:Invalid input -misplaced comma/s  ");
	}
	return NULL;
}    
 	
/*Checks if a number is valid number*/
int num_is_valid(char *num){
 /* Declare local variables */
	char *temp;
	int i = 0;
	int sign =0;
	int total=0;
	
	/* Allocate memory for temp */
	temp = (char *)malloc(strlen(num)+1);
	if(!temp){
    	printf("Memory allocation failed");
      	return 0;
    }	
	else
		strcpy(temp,num);
	/* Check if num is a valid number */
	for(;i<strlen(temp);i++){
		if(temp[i]=='+'||temp[i]=='-'){
			sign++;
			if(i>0){/* one sign per number, if its one sign but in the middle the conditoin below marks it */
				free(temp);
				return 0;
			}	
		}
		/* digit isn't a number or a comma not checking for blanks - they get removed while preproccessed */
		else if(!isdigit(temp[i])&&temp[i]!=','&&!isspace(temp[i])){/*commas number is already known so we can skip them here too*/
				free(temp);
				return 0;
		}
		else{
			total++;
		}
	}
	/* no digits,invalid */
	if(!total){
    	printf("********Error: Number must contain at least one digit.");
        free(temp);
        return 0;
    }
    free(temp);
	return 1;/* Return 1 if num is a valid number */
}


/* counting commas returns the number of commas, -1 indicaets invalid input */
int count_commas(char *input){
	char *temp; 
	int i = 0;
	int commas=0;
	if(!input){
		printf("********Error: Empty input");
		return 0;
	}
	temp = (char *)malloc(strlen(input)+1);
	strcpy(temp,input);
	
	if(temp[i]== ','){
		printf("********Error: Data input can't start with a - ','");
		free(temp);
		return 0;
	}
	if(temp[strlen(temp)-1] == ','){
		printf("********Error: Data input can't end with a - ','");
		free(temp);
		return 0;
	}
	
	for(i=0;i<strlen(temp);i++){
		if(temp[i] ==',')
			commas++;
		}
	free(temp);
	return commas;
}

/* setting the parsed info in the guidance_data struct and returns it */
guidance_data* set_data(char *temp,char *type,char *flag,int total){
	char *end;
	/* a guidance data ptr dynamicaly allocated*/
	guidance_data *gd=(guidance_data *)malloc(sizeof(guidance_data)); 		
    if(!gd){/*Mem alloc check*/ 
    	printf("Memory allocation failed");
      	return NULL;
     }
   	end = strstr(temp,"\n");
		if(end)
			*end='\0';
			
     /*Setting data*/
    strcpy(gd->type,type);
    strcpy(gd->data,temp);
   	gd->dc =total; 
   	free(flag);
	return gd;

}

/**
 *  Parses the input string to extract the string data following the ".string" flag
 * @param flag Input string containing the ".string" flag and the string data
 * @param temp A char pointer to a temporary buffer to store the extracted string data
 * @return char* A pointer to the extracted string data if successful, NULL otherwise
 */
char *input_parsing(char *flag,char *temp){
	char *ptr,*token;
	/*tokenizing the source line when  a label exists prior to the string data*/
	if(strstr(flag,":")){
		token = strtok(flag,":");/*the line without the label*/
 		token = strtok(NULL," ");/*removing the .string*/
   	 	token = strtok(NULL," ");/*here should be the string data if its valid*/
	}	
	else{	
 		/*tokenizing the source line when  a label does not exist prior to the string data*/
 		token = strtok(flag," ");/*removing the .string*/
  		token = strtok(NULL," ");/*here should be the string data if its valid*/
   	}
	/*Checking for an empty string input*/
	if(!token ||*token=='\n'){
   		printf("********Error: Empty Data content");
    	free(flag);/*flag*/
    	return NULL;
   		}
   	else
   		strcpy(temp,token);	
  
  	ptr = strtok(NULL," \n");
	if(ptr){
		printf("********Error: excessive content ---%s--- after a data input", ptr);
		free(flag);
		return NULL;
		}	
   	return token;	
	}

	
		
