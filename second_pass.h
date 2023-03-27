#include "first_pass.h"
#define NAME_POSTFIX 4
#define ENTX_POSTFIX 5
#define MAX_LABEL_LENGTH 30
#define ADDRESS_START 100
/* 
 * This macro is used to set the two least significant bits of a given 
 * command's encoding to the values specified by the arguments X and Y.
 * The encoding is stored in the "encoding" field of the command structure
 * at index i in the RAM array.
 * E- external R-relocatable  A-absolute 
 */
#define ERA(X,Y) \
	RAM[i].encoding[12]=X;\
	RAM[i].encoding[13]=Y;	
/* 
 * This macro is used to encode a 12-bits out of 14 and store it in the 
 * "encoding" field of the command structure at index i in the RAM array.
 * The number is passed as the argument "number".
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
/**
 * Encodes the commands and data in the RAM array during the second pass of the assembler
 * @param RAM An array of encoded_words containing the commands and data to be encoded
 * @param lt A pointer to a label_table containing the label information
 * @param entx A pointer to a label_table containing the .entry labels information
 * @param ent A pointer to an open FILE for the .ent output file
 * @param ext A pointer to an open FILE for the .ext output file
 * @param general An integer representing the total number of encoded_words in the RAM array
 * @return An integer representing the success of the function (1 for success, 0 for failure)
 */
int second_pass_endcoding(encoded_words *RAM,label_table *lt,label_table *entx,FILE *,FILE *,int );

/**
 * Generates the final output file from the encoded_words in the RAM array 
 * @param RAM An array of encoded_words containing the commands and data to be encoded
 * @param fname A string representing the filename of the final output file
 * @param IC An integer representing the location of the first instruction in the RAM array
 * @param DC An integer representing the number of data entries in the RAM array
 * @param general An integer representing the total number of encoded_words in the RAM array
 */
void unique_base_coding(encoded_words *,char *,int ,int,int);
				
/* Encodes data for a an entry/extern and if founded prints to a filelabel type)
 * @param RAM: a pointer to an array of encoded_words
 * @param lt: a pointer to a label_table struct
 * @param entx: a pointer to a label_table struct
 * @param ent: a pointer to a file stream for entry file
 * @param ext: a pointer to a file stream for extern file
 * @param i: an integer indicating the index of the current encoded_word in the RAM array
 * @return: 1 if encoding is successful, 0 otherwise
 */
int entx_encode(encoded_words *RAM,label_table *lt,label_table *entx,FILE *ent,FILE *ext,int);

/* This function encodes data for a command  label type(before a cmd/st/data line)
 * @param RAM: a pointer to an array of encoded_words
 * @param lt: a pointer to a label_table struct
 * @param i: an integer indicating the index of the current encoded_word in the RAM array
 * @return: 1 if encoding is successful, 0 otherwise
 */
int cmd_sdata_encode(encoded_words *RAM,label_table *lt,int i);
/**
 * Prints the IC and DC tables to a given file,first instrucitons and then data as requested 
 * @param RAM An array of encoded_words containing the instructions and data to be printed
 * @param fp A pointer to the file to which the IC and DC tables will be printed
 * @param size The size of the RAM array
 */	
void print_ic_dc(encoded_words *RAM,FILE *fp,int size);
void print_entry_file(label_table *lt,label_table *entx,FILE *ent);
