#include "first_pass.h"
/*first group of cmds - two arguments*/
#define FIRST_GROUP(x) (((x)>=0 &&(x)<=3)||x==6)
#define MOV 0
#define CMP 1
#define ADD 2
#define SUB 3
#define LEA 6
#define JMP 9
#define BNE 10
#define JSR 13
/*second group of cmds - one arguments*/
#define SECOND_GROUP(x) (((x)>=4&&(x)<=13)&&x!=6)
#define THIRD_GROUP(x) ((x)==15||(x)==14)	
/*third group of cmds - no arguments*/
#define MAX_LINE_SIZE 81
#define WORDSIZE 14
#define JUMP 1
#define TYPE_SIZE 2
/* This macro sets the encoding of two bits that indicate the era of the instruction 
 * The number is broken down into its binary representation and stored in the array from left to right
 */
#define ERA(X,Y) \
	RAM[i].encoding[12]=X;\
	RAM[i].encoding[13]=Y;
/* This macro sets the encoding of two bits that indicate the dest addresing method of the instruction 
 * The number is broken down into its binary representation and stored in the array from left to right
 * with the most significant bit
 * at the beginning and the least significant bit at the end.
 */
#define AD_DEST(X,Y) \
	RAM[i].encoding[10]=X;\
	RAM[i].encoding[11]=Y;
/* This macro sets the encoding of two bits that indicate the src addresing method of the instruction 
 * The number is broken down into its binary representation and stored in the array from left to right
 */
#define AD_SOURCE(X,Y)\
	RAM[i].encoding[8]=X;\
	RAM[i].encoding[9]=Y;

/* This macro sets the encoding of the six bits that indicate the source register  
 * The number is broken down into its binary representation and stored in the array from left to right
 * with the most significant bit(out of the 6)
 * at the beginning and the least significant bit at the end.
 */
#define REG_SOURCE(regnumber) \
 RAM[i].encoding[0]='0'+((regnumber>>5)&0x01);\
 RAM[i].encoding[1]='0'+((regnumber>>4)&0x01);\
 RAM[i].encoding[2]='0'+((regnumber>>3)&0x01);\
 RAM[i].encoding[3]='0'+((regnumber>>2)&0x01);\
 RAM[i].encoding[4]='0'+((regnumber>>1)&0x01);\
 RAM[i].encoding[5]='0'+((regnumber>>0)&0x01);

/* This macro sets the encoding of the six bits that indicate the dest register number 
 * The number is broken down into its binary representation and stored in the array from left to right,
 * with the most significant bit(out of the 6)
 * at the beginning and the least significant bit at the end.
 */
#define REG_DEST(regnumber) \
 RAM[i].encoding[6]='0'+((regnumber>>5)&0x01);\
 RAM[i].encoding[7]='0'+((regnumber>>4)&0x01);\
 RAM[i].encoding[8]='0'+((regnumber>>3)&0x01);\
 RAM[i].encoding[9]='0'+((regnumber>>2)&0x01);\
 RAM[i].encoding[10]='0'+((regnumber>>1)&0x01);\
 RAM[i].encoding[11]='0'+((regnumber>>0)&0x01);

/* This macro sets the encoding of the 12 bits that indicate a number 
 * The number is broken down into its binary representation and stored in the array from left to right,
 * with the most significant bit
 * at the beginning and the least significant bit at the end.
 */
#define NUMCODE(number) \
 RAM[i].encoding[0]='0'+((number>>11)&0x01);\
 RAM[i].encoding[1]='0'+((number>>10)&0x01);\
 RAM[i].encoding[2]='0'+((number>>9)&0x01);\
 RAM[i].encoding[3]='0'+((number>>8)&0x01);\
 RAM[i].encoding[4]='0'+((number>>7)&0x01);\
 RAM[i].encoding[5]='0'+((number>>6)&0x01);\
 RAM[i].encoding[6]='0'+((number>>5)&0x01);\
 RAM[i].encoding[7]='0'+((number>>4)&0x01);\
 RAM[i].encoding[8]='0'+((number>>3)&0x01);\
 RAM[i].encoding[9]='0'+((number>>2)&0x01);\
 RAM[i].encoding[10]='0'+((number>>1)&0x01);\
 RAM[i].encoding[11]='0'+((number>>0)&0x01);
 
#define LABELCODE(number) \
 RAM[i].encoding[0]='0'+((number>>11)&0x01);\
 RAM[i].encoding[1]='0'+((number>>10)&0x01);\
 RAM[i].encoding[2]='0'+((number>>9)&0x01);\
 RAM[i].encoding[3]='0'+((number>>8)&0x01);\
 RAM[i].encoding[4]='0'+((number>>7)&0x01);\
 RAM[i].encoding[5]='0'+((number>>6)&0x01);\
 RAM[i].encoding[6]='0'+((number>>5)&0x01);\
 RAM[i].encoding[7]='0'+((number>>4)&0x01);\
 RAM[i].encoding[8]='0'+((number>>3)&0x01);\
 RAM[i].encoding[9]='0'+((number>>2)&0x01);\
 RAM[i].encoding[10]='0'+((number>>1)&0x01);\
 RAM[i].encoding[11]='0'+((number>>0)&0x01);\
 RAM[i].encoding[12]='1';

/* This macro sets the encoding of the 12 bits that indicate a number 
 * The number is broken down into its binary representation and stored in the array from left to right,
 * with the most significant bit
 * at the beginning and the least significant bit at the end.
 */
#define OPCODE(opcode)\
 RAM[i].encoding[4]='0'+((opcode>>3)&0x01);\
 RAM[i].encoding[5]='0'+((opcode>>2)&0x01);\
 RAM[i].encoding[6]='0'+((opcode>>1)&0x01);\
 RAM[i].encoding[7]='0'+((opcode>>0)&0x01);

/* This macro sets the encoding of the 12 bits that indicate jmp cmd src arg */
#define PRM1(A,B)\
RAM[i].encoding[0]=A;\
RAM[i].encoding[1]=B;
 
/* This macro sets the encoding of the 12 bits that indicate jmp cmd dst arg */
#define PRM2(C,D)\
RAM[i].encoding[2]=C;\
RAM[i].encoding[3]=D;

/*init a code block on the intruction table in the size of "local ic"*/
void init_code_block(int ,int ,encoded_words *);

/* Jump command cases parsing label returns -1
 * label with args in it return 1
 * invalid/not a jump cmd return 0 for further parsing
 */
int is_jump_command(char *,char **,char **,char **);

/**
 * Checks if the given arguments are compatible with the first group of commands two arguments.
 * Also prints an error message if the arguments are not compatible.
 * @param opcode the opcode of the command
 * @param type1 the type of the first argument 
 * @param type2 the type of the second argument 
 * @param arg1 the first argument
 * @param arg2 the second argument
 * @return 1 if the arguments are not compatible, 0 otherwise
 */
int arg_compatibility_first(int ,char *,char *,char *,char *);		

/**
 * Checks if the given arguments are compatible with the second group of commands one operand
 *  prints an error message if the argument insnt compatible.
 * @param opcode the opcode of the command
 * @param type1 the type of the first argument 
 * @param type2 the type of the second argument if it exist - error
 * @param arg1 the first argument
 * @param arg2 the second argument if it exist - error
 * @return 1 if the arguments are not compatible, 0 otherwise
 */
int arg_compatibility_second(int ,char *,char *,char *);

/**

* This function encodes the given command into the memory block starting at the given IC.
* It takes a pointer to a command_data struct that contains information about the command,
* a pointer to a label_table struct that contains information about the labels in the code,
* the current instruction counter (IC), a pointer to an encoded_words struct for the memory block,
* and a pointer to an error flag (eflag).
* @param command_data* cd: pointer to a command_data struct containing information about the command
* @param label_table* lt: pointer to a label_table struct containing information about labels in the code
* @param int IC: the current instruction counter
* @param encoded_words* RAM: pointer to an encoded_words struct for the memory block
* @param int* eflag: pointer to an error flag variable
* @return int: 1 if the encoding was successful, 0 if there was an error
*/

int in_command_analysis(char *,char *,char*,char*,encoded_words *,int *,int,int);

/**
 * Analyzes an argument from the dest operand and tags it as register, label, or immediate value.
 * updates the encoded_words array with the appropriate binary representation for the argument.
 * @param arg the argument to analyze
 * @param arg_type pointer to a string that will store the argument's type ('r' for register, 'l' for label, 'n' for immediate value)
 * @param RAM pointer to the encoded_words array
 * @param i the current index in the encoded_words array
 */
void analyze_arg_dest(char *, char *,encoded_words *,int);

/**
 * Analyzes an argument from the source operand and tags it as register, label, or immediate value.
 * Also updates the encoded_words array with the appropriate binary representation for the argument.
 * @param arg the argument to analyze
 * @param arg_type pointer to a string that will store the argument's type ('r' for register, 'l' for label, 'n' for immediate value)
 * @param RAM pointer to the encoded_words array
 * @param i the current index in the encoded_words array
 */
void analyze_arg_source(char *, char *,encoded_words *,int);


/*checks if a register arg is valid */
void is_valid_reg(char*,encoded_words *,int i);

/**
 * Encodes the source operand of an instruction and updates the corresponding fields in the encoded_words array.
 * @param arg pointer to the string containing the source operand
 * @param arg_type pointer to a string representing the type of the source operand 
 * @param RAM pointer to the encoded_words array where the encoded operand should be written
 * @param i index of the encoded_words array where the encoded operand should be written
 * @return void
 */
void encodment_arg_s(char*,char*,encoded_words *,int);
/**
 * Encodes the destination operand of an instruction and updates the corresponding tags in the encoded_words array.
 * @param arg pointer to the string containing the destination operand
 * @param arg_type pointer to a string representing the type of the destination operand 
 * @param RAM pointer to the encoded_words array where the encoded operand should be written
 * @param i index of the encoded_words array where the encoded operand should be written
 * @return void
 */
 
/*same as above but for dest*/
void encodment_arg_d(char*,char*,encoded_words *,int);

		
/* This function takes in a string argument, a label_table pointer,
 * an encoded_words pointer, and an integer pointer.
 * encodes '*' in case this is a valid label but yet to be found 
 * encodes the lables code if it exists - copies it
 * infroms an error as it should have been a label as an argument 
 */
void label_command_coding(char *,label_table *,encoded_words *,int *);

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

void arg_command_coding(char *,char *,char *,char *,label_table *,encoded_words *,int *);


/* Function to encode two register arguments
 *
 * This function takes in two register arguments, their argument types, and a pointer
 * to an encoded_words struct array. It encodes the two arguments and stores them in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */
void reg_args(char *,char *,char *,char *,encoded_words *,int);

/* Function to encode source argument
 * This function takes in  a source argument,  argument type, and a pointer
 * to an encoded_words struct array. It encodes the  argument and store it in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */

void first_arg(char *,char *,label_table *,encoded_words *,int*);

/* Function to encode dest argument
 * This function takes in  a dest argument, argument type, and a pointer
 * to an encoded_words struct array. It encodes the argument and store it in
 * the encoded_words struct array at the given index. It also updates the index by one.
 */	
void second_arg(char *,char *,label_table *,encoded_words*,int*);
