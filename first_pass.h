#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**	All functions related to lable table such as -  
 * add_label
 * find_label
 * add_entx
 * mark_label
 * get_label
 */
#include "label_table.h"

/*consts*/
#define NUM_OF_COMMANDS 16
#define NUM_OF_REGISTERS 8
#define NUM_OF_GUIDELINES 4
#define MAX_LINE_SIZE 81
#define MAX_LABELS 156
#define RAM_SIZE 256
#define WORD_SIZE 14
#define NAME_POSTFIX 4
#define MAX_LABEL_LENGTH 30
#define TYPE_OF_CMD_LENGTH 3
#define TYPE_OF_GDN_LENGTH 3

/*
 * Macro for encoding a number into a string of 14 binary digits and storing it in the 'encoding' 
 * array of a 'RAM' *element.
 * The number is broken down into its binary representation and stored in the array from left to right,
 * with the most significant bit
 * at the beginning and the least significant bit at the end.
 */
#define STCODE(number) \
 RAM[i].encoding[0]='0'+((number>>13)&0x01);\
 RAM[i].encoding[1]='0'+((number>>12)&0x01);\
 RAM[i].encoding[2]='0'+((number>>11)&0x01);\
 RAM[i].encoding[3]='0'+((number>>10)&0x01);\
 RAM[i].encoding[4]='0'+((number>>9)&0x01);\
 RAM[i].encoding[5]='0'+((number>>8)&0x01);\
 RAM[i].encoding[6]='0'+((number>>7)&0x01);\
 RAM[i].encoding[7]='0'+((number>>6)&0x01);\
 RAM[i].encoding[8]='0'+((number>>5)&0x01);\
 RAM[i].encoding[9]='0'+((number>>4)&0x01);\
 RAM[i].encoding[10]='0'+((number>>3)&0x01);\
 RAM[i].encoding[11]='0'+((number>>2)&0x01);\
 RAM[i].encoding[12]='0'+((number>>1)&0x01);\
 RAM[i].encoding[13]='0'+((number>>0)&0x01);

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



/* 
 * This struct represents essential information about a guidance line
 */
typedef struct guidance_data{
    char type[TYPE_OF_CMD_LENGTH];/*Type - string/data/extern/entry*/
    char data[MAX_LINE_SIZE];/*Data - stores the actual data as a string prior encodment*/
    int dc; /*"Local" DC variable -its an individual dc counter for each guidance line*/
	}guidance_data;

/* 
 * This struct represents a line of an instruction table for an assembler.
 */
typedef struct encoded_words{
    char command_name[MAX_LABEL_LENGTH]; /*Command/guidance/argument/etc.. */
    char encoding[WORD_SIZE+1]; /*Post encodment [binary base] representaion of the relevant line */
	char type[TYPE_OF_CMD_LENGTH];
    int location; /*RAM location; Range - 0100-0255*/
}encoded_words;


/**
 * Goes over a temporary macro names file and checks if they are not saved words.
 * A file is only opened if at least one macro exists in the file.
 * @param filename The name of the temporary macro names file.
 * @return 0 if there is a memory allocation error or an invalid macro name,
 *         1 if all macro names are valid.
 */
int is_valid_macro();

/*checks if a cmd is a valid cmd*/	
int isopcode(char *,int);

/*Function to check if its a saved word*/ 
int is_saved(char *);

/*Function to init instruction table*/
void init_ints_table(encoded_words *RAM);

/**
 * Check for errors in the given label and return the label if it's valid
 * @param label The label to check for errors
 * @return The label if it's valid, NULL otherwise
 */
char *label_errors(char *);/*label ptr check -  used for descoring errors which lead for procces termination */

/**
 * Check for errors in the given label and return the label if it's valid
 * @param label The label to check for errors
 * @return  1 if it's valid, 0 otherwise
 */
int islabel(char *);/*label boolean check -  used for argument validation */

/** Function to parse a line and generate command data based on it 
 *	@param command_info the command data structure to send the command to
 *	@param lableTable a double pointer of the symbol table
 *	@param line The input line to be parsed.
 *	@param general flag indicating whether the command is a general command or not
 *	@param error The current errors counter.
 *	@return a pointer to the generated command data struct.
 */
command_data *send_to_command(command_data *,label_table **,char *,int,int *,int);/*command line encodment*/

/**
 * Check if the given line is a valid assembly command and return a pointer to a command_data struct
 * that contains information about the command if it is valid.
 *
 * @param line The line of code to check.
 * @param error A pointer to an integer that will be set to an error code if an error occurs.
 *
 * @return A pointer to a command_data struct that contains information about the command if the line
 *         is a valid command, or NULL otherwise.
 */
command_data *is_command(char *,int*);

/**
 * Encodes the given command_data struct in the RAM, using the given label_table and encoded_words struct
 * and updating the given IC and eflag values accordingly.
 * @param cmd A pointer to the command_data struct to encode.
 * @param labels A pointer to the label_table struct to use for label resolution.
 * @param IC The current value of the Instruction Counter.
 * @param ram A pointer to the encoded_words struct for the RAM.
 * @param eflag A pointer to an integer that will be updated with the encoding flag value.
 *
 * @return 1 if the encoding was successful, or 0 if an error occurred.
 */
int command_coding(command_data *,label_table *,int,encoded_words *,int *);/*command line encodment*/

/*Function to update the instruction table in case of a valid command line*/ 
int command_data_update(command_data *);

/*-------------------Guidance functions---------------*/

/*Function to add entry/extern lables */	
void add_entx_to_ltable(label_table **,label_table *);

/*Marks cmd lines for second pass division */  
void tag_command_data(encoded_words *,int ,int );

/*Function to update the instruction table in case of a valid guidance line*/ 
int guidance_data_update(guidance_data *guide,label_table *data_image,encoded_words *RAM,int general);

/**
 * Checks if an entry/extern label with the given name already exists in the given label table.
 * @param lt The label table to search in.
 * @param label The label name to check.
 * @param type The label type to check (must be "EN" or "EX").
 * @return 0 if the label is not defined, 1 if it is defined and has the same type, or -1 if the type is invalid.
 */
int existed_entx(label_table *lt,char *label,char *type);

/**
 * Encodes the guidance data into binary and updates the encoded RAM.
 * @param gd a pointer to a guidance_data struct containing the guidance data to encode.
 * @param DC the current value of the data counter.
 * @param RAM a pointer to an encoded_words array representing the encoded RAM.
 */
void guidance_coding(guidance_data *,int ,encoded_words *);

/** Function  generate and validate a guidance line
 * This function takes in a pointer to a label_table struct, a string representing a label,
 * and a string representing a line of assembly code. It processes the guidance data in the 
 * line and returns a pointer to a guidance_data struct with the encoded data.
 * If the line is invalid or contains an invalid command, NULL is returned.
 */
guidance_data *processGuidance(label_table *,char *,char *);/*Guidance line parsing*/

/** Function to parse a line and generate guidance data based on it
 *	@param lableTable a double pointer of the symbol table
 *	@param line The input line to be parsed.
 *	@param DC The current data counter value.
 *	@return a pointer to the generated guidance data.
 */	
guidance_data *send_to_guidance(label_table **,char *,int,int);

/** 
 * Performs the second pass of the assembly process, which includes encoding the
 * assembly code into unique machine code, generating entry and extern files, and creating
 * an output file with the encoded program.
 * @param after_macro_source: A string representing the name of the source file.
 * @param lt: A pointer to a label_table struct containing all defined label names.
 * @param entx: A pointer to a label_table struct containing all entry labels.
 * @param RAM: A pointer to an encoded_words struct where the encoded program will be stored.
 * @param general: An integer indicating the addressing method used in the program.
 * @param IC: An integer representing the Instruction Counter at the end of the first pass.
 * @param DC: An integer representing the Data Counter at the end of the first pass.
 */

void second_pass(char *,label_table *,label_table *,encoded_words *,int ,int ,int );

/*Function to terminate correctly and infrom about any errors */ 
void has_error(FILE *,label_table *,label_table *,int ,int );
 
 







