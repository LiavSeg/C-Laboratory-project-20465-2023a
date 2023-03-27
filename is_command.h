#include <stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>	
#define NUM_OF_COMMANDS 16
#define NUM_OF_REGISTERS 8
#define NUM_OF_GUIDELINES 4
#define MAX_LINE_SIZE 81


/* essential data of cmd line */	
typedef struct command_data{
	char *name;
	char *arg1;
	char *arg2;
	int opcode;
	int ic;
	}command_data;

/* This func checks if a label/symbol is valid "boolean" func for meticulous inpesction of the assembler*/	
	int islabel(char *);
/* This func checks if a cmd isnt saved word */	
	int is_saved(char *);
/* This func checks if a cmd is  valid */		
	int isopcode(char *,int );
	
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

command_data *isvalidargs(char *,command_data *,int *);
