#include <stdio.h>
#include<string.h>
#include<ctype.h>
#define NUM_OF_COMMANDS 16
#define NUM_OF_REGISTERS 8
#define NUM_OF_GUIDELINES 4
#define MAX_LABEL_LENGTH 30
int is_saved_label(char *input);
/* This func checks if a label/symbol is valid "boolean" func for meticulous inpesction of the assembler*/

int islabel(char *label){
	 int size = strlen(label);
	 int i;
    if(!size||size>MAX_LABEL_LENGTH) 
        return 0;
    
    if(!isalpha(label[0]))
        return -1;
     if(is_saved_label(label))
     	return 0;
    for(i=0;i<size;i++){
        if(!isalnum(label[i])&&!(isspace(label[i]))) 
            return 0;
           }
  return 1;
	}
	

/* This func checks if a label/symbol is valid return NULL for first pass parsing  func*/
char *label_errors(char *label){
	 int size = strlen(label);
	 int i;
    if(!size){
    	printf("********Error: Empty label isn't valid "); 
        return NULL;
    }
    if(size>MAX_LABEL_LENGTH){
    	printf("********Error: Label must contain at most 30 characters "); 
        return NULL;
    }
    if(!isalpha(label[0])){
       printf("********Error: Label must begin with a letter "); 
        return NULL;
     }
    for(i=1;i<size;i++){
        if(!isalnum(label[i])){ 
           	printf("********Error: Label must contain Letters and numbers only "); 
            return NULL;
           }
     }
	if(is_saved_label(label)){
    	printf("********Error: Label cant be a saved word -> %s <- ",label); 
        return NULL;
    }
    
  return label;
}
	
int is_saved_label(char *input){
	int i;
	char *opcodes[] = 			    			   	  {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","stop"};
	char *guid[] = {"data","string","entry","extern"};    /* guidance related saved words */
	char *reg[] = {"r0","r1","r2","r3","r4","r5","r6","r7"};/* register names*/
    /* checks if the input is a saved word*/
	for(i=0;i<NUM_OF_REGISTERS;i++)
			if(!strcmp(input,reg[i]))
				return 1;
	for(i=0;i<NUM_OF_GUIDELINES;i++)
			if(!strcmp(input,guid[i]))
				return 1;		
	for(i=0;i<NUM_OF_COMMANDS;i++)
		if(!strcmp(input,opcodes[i]))
			return 1;
	
	return 0;
}																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																												
 
	

