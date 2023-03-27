#include "command_coding.h"
/* This function takes in three string arguments (arg1, arg2, and arg_type1),
 *  two char pointers (arg_type2 and RAM), two integer pointers (ic and opcode),
 * and an integer (jump). 
 * Checks argument compatibility for cmd,encodes, and classifying each argument  
 */


int in_command_analysis(char *arg1,char *arg2,char *arg_type1,char *arg_type2,encoded_words *RAM, int *ic,int opcode,int jump){
	int i = *ic;/* Initialize i variable to the value pointed by ic */
	int error = 0;
	char t1[TYPE_SIZE],t2[TYPE_SIZE]; /* Initialize two character arrays (t1 and t2) to hold argument types */
    
	
	/* If arg2 exists, analyze both arguments */
	if(arg2){
		analyze_arg_dest(arg2,arg_type2,RAM,i);
	  	analyze_arg_source(arg1,arg_type1,RAM,i);
		strcpy(t1,arg_type1);/* Copy the type of arg1 to t1 */
		strcpy(t2,arg_type2);/* Copy the type of arg2 to t2 */
		error = arg_compatibility_first(opcode,arg_type1,arg_type2,arg1,arg2);/* Check argument compatibility */
	}
	/* If arg2 does not exist, analyze first argument */
	if(!arg2){
		analyze_arg_dest(arg1,arg_type1,RAM,i);
		strcpy(t1,arg_type1);/* Copy the type of arg1 to t1 */
		error = arg_compatibility_second(opcode,arg_type1,arg_type2,arg1);/* Check argument compatibility */
	} 
	 /* If there are no argument errors and tagged */
	if(!error){
		if(strcmp(arg_type1,"X") && strcmp(arg_type2,"X")){
  			OPCODE(opcode);/* Encode the opcode */
			if(jump){
				AD_DEST('1','0');/* Encode the addressing mode for the destination argument */
				AD_SOURCE('0','0');/* Encode the addressing mode for the source argument */
				encodment_arg_s(arg1,arg_type1,RAM,i);/* Encode the source argument */
				encodment_arg_d(arg2,arg_type2,RAM,i);/* Encode the destination argument */
			}
			RAM[i].location = i;
			(*ic)++;
			return 1;
		}
		/* If only arg1 is tagged */
		else if(strcmp(arg_type1,"X")){
			OPCODE(opcode);/* Encode the opcode */
			/* If the command is a jump command */
			if(jump){
				AD_DEST('0','1');
				AD_SOURCE('0','0');
			}
			RAM[i].location = i;
			(*ic)++;
			return 1;
		}
	}		
	/* If errors were found */
	else{
		strcpy(arg_type1,"X");
		strcpy(arg_type2,"X");
		memset(RAM[i].encoding,'X',WORDSIZE);
		(*ic)++;
		return 0;
	}
	return 0;
}			
/* Encode an argument of type "source" */
	
void encodment_arg_s(char *arg, char *arg_type,encoded_words *RAM,int i){
	if(!arg)
		return;	
	
	switch(*arg_type){
  		case 'r':
   			PRM1('1', '1');
    	  	break;
    	case 'n':
    		 PRM1('0', '0');
    		 break;
    	case 'l':
    		PRM1('0', '1');
    		break;
    	default: 
    		if(islabel(arg))
      			strcpy(arg_type,"l");      
    }
} 	
	
void is_valid_reg(char *arg,encoded_words *RAM,int i){
	if(arg[1]<'0'||(int)arg[1]>'7'){
	  	printf("********Error: Undefined register operand");
 	   memset(RAM[i].encoding,'X',WORDSIZE);
   	}
}

	
void encodment_arg_d(char *arg, char *arg_type,encoded_words *RAM,int i){
	switch(*arg_type){
 		case 'r':
		PRM2('1', '1');
  	 		break;
  		case 'n':
   			PRM2('0', '0');
    		break;
  		case 'l':
    		PRM2('0', '1');
    		break;	
   	    default: 
    		if(islabel(arg))
   				strcpy(arg_type,"l");          
   		}
} 	
	
void analyze_arg_dest(char *arg, char *arg_type,encoded_words *RAM,int i){
	if(!arg)
		return ;
	switch(*arg){
   		case '#':
	   		strcpy(arg_type, "n");
     		break;
   		case 'r':
     		strcpy(arg_type, "r");
     		AD_DEST('1','1');
     		break;
   		default: 
 		 	if(islabel(arg)){
   	 			strcpy(arg_type,"l"); 
      			AD_DEST('0','1');
      	  	}
      	  		
  	}			
}
/* source arg analysis and tagging*/
void analyze_arg_source(char *arg, char *arg_type,encoded_words *RAM,int i){
	if(!arg)
		return;
	
	switch(*arg){
   		case '#':
     		strcpy(arg_type, "n");
        	break;
   		case 'r':
     		strcpy(arg_type, "r");
     	  	AD_SOURCE('1','1');
     	 	break;
		default: 
    		if(islabel(arg)){
     			strcpy(arg_type,"l"); 
     			AD_SOURCE('0','1');
    		}   	
     }	  
}
		
/* arg_compatibility for the first cmd group - two arguments*/
int arg_compatibility_first(int opcode,char *type1,char *type2,char *arg1,char *arg2){
	char *opcodes[] = {"mov","cmp","add","sub","","","lea",};

	int error = 0;
	
	if(FIRST_GROUP(opcode)){
		if(!strcmp(type1,"X")){
			printf("********Error: Valid Source operand needed for %s command ",opcodes[opcode]);
			return 1;
		}
	
	if((!strcmp(type1,"l")&&islabel(arg1)<=0)||(!strcmp(type2,"l")&&islabel(arg2)<=0)){
		printf("********Error: Invalid operand for %s command ",opcodes[opcode]);
		return 1;
	}
	
		else if((opcode == MOV)||(opcode == ADD)||(opcode == SUB)){
			if(!(strcmp(type2,"X"))||!strcmp(type2,"n")){	 					
				printf("********Error: Invalid destination operand %s command ",opcodes[opcode]);
				return 1;
			}
		}	
		else if(opcode==CMP){
			if(!strcmp(type2,"X")){
				printf("********Error: Destination operand is not compatible for this or any command ");
				return 1;
			}
		}	
		else if(opcode == LEA){
			if(!strcmp(type1,"r")||!strcmp(type1,"n")){
				printf("********Error: Source operand is not compatible for %s command ",opcodes[opcode]);
				return 1;
			}
			 if(!strcmp(type2,"X")||!strcmp(type2,"n")){
				printf("********Error: Destination operand is not compatible for %s command ",opcodes[opcode]);
				return 1;
			}
		}
	}
	return error;
}
/* arg_compatibility for the second cmd group - one arguments----- jump cmd handled differently*/	
int arg_compatibility_second(int opcode,char *type1,char *type2,char *arg1){			
	int error = 0; 																								
	char *opcodes[] = {"","","","","not","clr","","inc","dec","","","","","","",""};

	if(SECOND_GROUP(opcode)&&(opcode!=JMP||opcode!=BNE||opcode!=JSR)){
		if(strcmp(type2,"X")){
			printf("********Error: Exccesive Source operand %s command ",opcodes[opcode]);
			return 1;
		}
		 if(!strcmp(type1,"X")){
			printf("********Error: %s command is missing a valid Destination operand ",opcodes[opcode]);
			return 1;
		}
		 if((!strcmp(type1,"l")&&islabel(arg1)<=0)){
			printf("********Error: Invalid operand destinaion operand or %s commnd ",opcodes[opcode]);
			return 1;
		 }
		 if((opcode!=12)&&!strcmp(type1,"n")){
			printf("********Error: Invalid Destination operand for %s command ",opcodes[opcode]);
			return 1;
		}
			
	}
	return error;		
}
	
	
	
  
    
    
 
