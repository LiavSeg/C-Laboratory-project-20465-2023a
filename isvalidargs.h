#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define FIRST_GROUP(x) (((x)>=0&&(x)<=3)||x==6)
#define SECOND_GROUP(x) (((x)>=4&&(x)<=13)&&x!=6)
#define THIRD_GROUP(x) ((x)==15||(x)==14)		
#define MAX_LINE_SIZE 81


/* 
 * This struct represents essential information about a command line
 */
typedef struct command_data{
	char *name;/*Name - command's name */
	char *arg1;/*Argument no.1 [may not be existed]*/
	char *arg2;/*Argument no.2 [may not be existed]*/
	int opcode;/*Command's operation code*/
	int ic;	/*"Local" IC variable -its an individual dc counter for each guidance line*/
	}command_data;

/* Function to analyze a command with one argument and update the command data
 * This function takes in a string representing a single argument command, a pointer to a command_data struct,
 * and a pointer to an integer to keep track of errors.
 * It processes the argument and updates the ic value in the command_data struct.
 * If there is an error, it returns NULL.
 */
command_data *arg_analysis_one(char *,command_data *,int *);

/* Function to analyze two arguments of an assembly command [when its valid]
* This function takes in two strings representing arguments of an assembly command,
* and a pointer to a command_data struct. It checks whether the arguments are valid registers,
* and updates the instruction counter in the command_data struct accordingly.
* If both arguments are valid registers, the ic is updated to 2. Otherwise, it is updated to 3.
* If both argument1 and argument2 are NULL, ic is updated to 1.
* Returns a pointer to the updated command_data struct.
*/
command_data *arg_analysis_two(char *,char *,command_data *);

/* This func checks if a label/symbol is valid "boolean" func for meticulous inpesction of the assembler*/
int islabel(char *);

/*Simple for loop to check if there are any chars that aren't blanks*/
int	isblank(char *); 

/*Tokenzing a string  to check if there are any chars that aren't blanks*/
int exccesive_content(char *);
