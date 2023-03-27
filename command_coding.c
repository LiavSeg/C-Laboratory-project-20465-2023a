#include "command_coding.h"

/* 
 * This function takes a pointer to a command_data struct, a pointer to a 
 * label_table struct, an integer IC, a pointer to an encoded_words struct 
 * for the RAM, and a pointer to an eflag integer. The function encodes the 
 *  command in the RAM according to its opcode and arguments, using various 
 *  helper functions. Returns 1 on success, 0 on failure.
 */

int command_coding(command_data *cd,label_table *lt,int IC,encoded_words *RAM,int *eflag){
	/* Declare local variables */
	char *command = cd->name;/*pointer to command string */
	char *argu1 = cd->arg1;/* pointer to first argument string */
	char *argu2 = cd->arg2;/* pointer to second argument string */
	int i,j,flag;
	char arg_type1[TYPE_SIZE] = "X";/* character array for first argument type */
	char arg_type2[TYPE_SIZE] = "X";/* character array for second argument type */
	char *token = NULL;/* pointer to token string */
	i = IC;
	
	init_code_block(cd->ic,IC,RAM);
	strcpy(RAM[i].command_name,	cd->name);
		
	/* Check the opcode group 3 */
	if(THIRD_GROUP(cd->opcode)){
		strcpy(RAM[IC].command_name,command);
		OPCODE(cd->opcode);
		RAM[i].location = IC;			
	}
	/* Check the opcode group 2 */
	else if(SECOND_GROUP(cd->opcode)){
		j = is_jump_command(command,&argu1,&argu2,&token);
		/* Jump command  label with args case*/
		if(j>0){
			flag = in_command_analysis(argu1,argu2,arg_type1,arg_type2,RAM,&i,cd->opcode,JUMP);
			if(!flag)
				return 0;
			label_command_coding(token,lt,RAM,&i);
			arg_command_coding(argu1,argu2,arg_type1,arg_type2,lt,RAM,&i);	
		}		
		/* Jump command label case*/
		else if(j<0){		
			flag =	in_command_analysis(argu1,argu2,arg_type1,arg_type2,RAM,&i,cd->opcode,JUMP);
			if(!flag)
				return 0;
			arg_command_coding(argu1,argu2,arg_type1,arg_type2,lt,RAM,&i);		
		}
		/* Not a jump command */
		else if(j==0){		
			flag=in_command_analysis(argu1,argu2,arg_type1,arg_type2,RAM,&i,cd->opcode,0);
			if(!flag)
				return 0;
			arg_command_coding(argu1,argu2,arg_type1,arg_type2,lt,RAM,&i);
		}
	}
	/* Check the opcode group 1 */
	else if(FIRST_GROUP(cd->opcode)){	
		flag =	in_command_analysis(argu1,argu2,arg_type1,arg_type2,RAM,&i,cd->opcode,0);
		if(!flag)
			return 0;
		arg_command_coding(argu1,argu2,arg_type1,arg_type2,lt,RAM,&i);
	}
	return 1;
}			
	
		
/* Jump command cases parsing label returns -1
 * label with args in it return 1
 * invalid/not a jump cmd return 0 for further parsing
 */		
int is_jump_command(char *command,char **arg1,char **arg2,char **token){
	if(!strcmp(command,"jmp")||!strcmp(command,"bne")||!strcmp(command,"jsr")){	
		if(islabel(*arg1))/*  flag has the index of the label in the label_table if it exsist */
			return -1;	
		*token = strstr(*arg1,"|");
		if(token){
			*token = strtok(*arg1,"|");
			*arg1 = strtok(NULL,"|");
			*arg2 = strtok(NULL,"|");
			return 1;	
		}		
	}
	return 0;
}					
		
/* This function takes in a string argument, a label_table pointer,
 * an encoded_words pointer, and an integer pointer.
 * encodes '*' in case this is a valid label but yet to be found 
 * encodes the lables code if it exists - copies it
 * infroms an error as it should have been a label as an argument 
 */
void label_command_coding(char *arg,label_table *lt,encoded_words *RAM,int *ic){
	int flag = -1;/* Initialize flag variable to -1 */
	int i = (*ic);/* Initialize i variable to the value pointed by ic */
	/* If the argument is a label, try to find it in the label_table */
	if(islabel(arg)){
		flag = find_label(lt,arg);
	
     	/*If the label is not found, set the encoding of the current encoded word to '*', 
          copy the command name to the current encoded word, set the location to the current value of i, 
          and increment i by 1 */	
		if(flag==-1){
			memset(RAM[i].encoding,'*',WORDSIZE);
			strcpy(RAM[i].command_name,arg);	
			RAM[i].location = i;
			(*ic)++;				
			}	
		
		/* If the label is found, copy the encoding of the label's corresponding encodment*/
		else if(flag>-1){
			LABELCODE(flag);
			flag = 0;
			RAM[i].location = flag;
			strcpy(RAM[i].command_name,arg);
			(*ic)++;
		}				
	}
	else{
		/* If the argument is not a label, print an error message and increment i by 1 to keep parsing */
		printf("Invalid argument - it is not a label as excpected");
		(*ic)++;
	}
}			
				
void init_code_block(int lines,int ic,encoded_words *RAM){
	int i,j;
	j= ic+lines;
	for(i = ic;i<=j;i++){
		memset(RAM[i].command_name,'0',1);
		memset(RAM[i].encoding,'0',WORDSIZE);
		RAM[i].location = -1;
	}
}

        

/*
 * Function to encode the arguments of a command and store them in the RAM array.
 * This function takes in two strings representing the arguments, two strings representing
 * the types of the arguments, a pointer to a label_table struct, a pointer to an encoded_words
 * struct representing the RAM array, and a pointer to an integer representing the current index
 * in the RAM array. It encodes the arguments according to their types and stores them in the RAM
 * array.
 * When both args are register a specific func is called otherwise dest and source args are proccessed in sperate 
 * functions - first_arg second_arg
 */
void arg_command_coding(char *arg1,char *arg2,char *arg_type1,char *arg_type2,label_table *lt,encoded_words *RAM,int *ic){
	int i = *ic;
	if(!strcmp(arg_type1,"r") &&!strcmp(arg_type2,"r")){
		reg_args(arg1,arg2,arg_type1,arg_type2,RAM,i);
		(*ic)++;
		strcpy(RAM[i].command_name,arg2);
		return;
		}
		first_arg(arg1,arg_type1,lt,RAM,ic);
		if(arg2){
			second_arg(arg2,arg_type2,lt,RAM,ic);	
		}
	}

/* Function to encode two register arguments
 *
 * This function takes in two register arguments, their argument types, and a pointer
 * to an encoded_words struct array. It encodes the two arguments and stores them in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */

void reg_args(char *arg1,char *arg2,char *arg_type1,char *arg_type2,encoded_words *RAM,int i){
	int binary;/*integer to store the converted register number*/
	char *token;/*character pointer to store tokens of the register argument*/
	ERA('0','0');/*first two bits macro */
	/*source arg encoding  */
	token = strtok(arg1,"r");
	binary = atoi(token);
	REG_SOURCE(binary);
	/*dest arg encoding  */
	token = strtok(arg2,"r");
	binary = atoi(token);
	REG_DEST(binary);
	/*same location in this case*/
	RAM[i].location = i;
	strcpy(RAM[i].command_name,arg1);
	return;	
}	
	


/* Function to encode source argument
 * This function takes in  a source argument,  argument type, and a pointer
 * to an encoded_words struct array. It encodes the  argument and store it in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */

void first_arg(char *arg,char *arg_type,label_table *lt,encoded_words *RAM,int *ic){
	int binary,i;
	unsigned int num;
	char *token;
	i = *ic;
	/*Source arg is a register*/
	if(!strcmp(arg_type,"r")){
		token = strtok(arg,"r");
		ERA('0','0');	
		binary = atoi(token);
		REG_SOURCE(binary);
		RAM[i].location = i;
		strcpy(RAM[i].command_name,arg);
		(*ic)++;
		}
	/*Source arg  is a number*/
	else if	(!strcmp(arg_type,"n")){
		token = strtok(arg,"#");
		num = (unsigned int)atoi(token);
		ERA('0','0');
		NUMCODE(num);
		RAM[i].location = i;
		strcpy(RAM[i].command_name,arg);
		(*ic)++;
		}
	/*Source arg is a label*/
	else if(!strcmp(arg_type,"l")){
		label_command_coding(arg,lt,RAM,ic);
		strcpy(RAM[i].command_name,arg);
		}
	/*Source arg  is invalid*/
	else if(!strcmp(arg_type,"X")){
		memset(RAM[i].encoding,'X',WORDSIZE);	
		(*ic)++;
	}
}
					
/* Function to encode dest argument
 * This function takes in  a dest argument, argument type, and a pointer
 * to an encoded_words struct array. It encodes the argument and store it in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */	
void second_arg(char *arg,char *arg_type,label_table *lt,encoded_words *RAM,int *ic){
	int binary,i;
	unsigned int num;
	char *token;
	i = *ic;
	if(!arg)
		return;
	/*Dest arg is a register*/
	if(!strcmp(arg_type,"r")){
		token = strtok(arg,"r");
		ERA('0','0');	
		binary = atoi(token);
		REG_DEST(binary);
		strcpy(RAM[i].command_name,arg);
		(*ic)++;
	}
	/*Dest arg is a number*/
	else if(!strcmp(arg_type,"n")){
		token = strtok(arg,"#");
		num = (unsigned int)atoi(token);
		ERA('0','0');
		NUMCODE(num);
		strcpy(RAM[i].command_name,arg);
		(*ic)++;
	}
	/*Dest arg is a label*/
	else if(!strcmp(arg_type,"l")){
		ERA('1','0');
		label_command_coding(arg,lt,RAM,ic);		
		strcpy(RAM[i].command_name,arg);
		(*ic)++;
	}
	/*Dest arg is invalid*/
	else if(!strcmp(arg_type,"X")){
		memset(RAM[i].encoding,'X',WORDSIZE);	
		strcpy(RAM[i].command_name,"X");
		(*ic)++;
	}	
	return;		
}    








