#include "first_pass.h"
/*This macro encodes string/data using a simple assignments*/
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
	
/** 
 * This function processes a data guidance instruction 
 * checks for any errors and compatibility of the input
 * Parameter line - a string that represent the parsed line
 * returns a pointer to a guidance_data struct with the appropriate info
 * if the input is valid, otherwise it returns NULL.
 */
guidance_data *processGuidanceData(char* );

/* Function to generate and validate a string guidance line info 
 * This function takes in a pointer to a label_table struct, a string representing a label,
 * and a string representing a line of assembly code. It processes the guidance data in the
 * line and returns a pointer to a guidance_data struct with the encoded data.
 * If 1 the line is invalid or contains an invalid command, NULL is returned.
 */
guidance_data *processGuidanceString(char* ,char *);


/**
 * Encodes the guidance data into binary and updates the encoded RAM.
 * @param gd a pointer to a guidance_data struct containing the guidance data to encode.
 * @param DC the current value of the data counter.
 * @param RAM a pointer to an encoded_words array representing the encoded RAM.
 */
void guidance_coding(guidance_data *,int ,encoded_words *);

/*A func that mimic strdup which is not included in c90 - dupliactes dynamically a given string */
char* strdup(const char* );


/** 
 * This function processes a data guidance instruction 
 * checks for any errors and compatibility of the input
 * Parameter line - a string that represent the parsed line
 * returns a pointer to a guidance_data struct with the appropriate info
 * if the input is valid, otherwise it returns NULL.
 */
guidance_data *processGuidanceData(char* );

/**
 *  Parses the input string to extract the string data following the ".string" flag
 * @param flag Input string containing the ".string" flag and the string data
 * @param temp A char pointer to a temporary buffer to store the extracted string data
 * @return char* A pointer to the extracted string data if successful, NULL otherwise
 */
char *input_parsing(char *,char *);	

/* counting commas returns the number of commas, -1 indicaets invalid input */
int count_commas(char *);

/*Checks if a number of a data guidanc line is a valid number*/
int num_is_valid(char *);

/* setting the parsed info in the guidance_data struct and returns it */
guidance_data* set_data(char *,char *,char *,int);



/**
 * This function processes an entry or extern label and returns a pointer to a guidance_data struct
 * with the relevant information. It takes in a pointer to a label_table struct, a label string, 
 * and a line string. If it is an entry label, it removes the ".entry" prefix from the line and 
 * extracts the label name. If it is an extern label, it extracts the label name from the line. 
 * It returns NULL if there is an error, otherwise it returns a pointer to a guidance_data struct
 * with the label type, label name, and DC value set.
 */
guidance_data *processEntryOrExtern(label_table *,char *, char* );

void remove_prefix(char *,const char *);

/* setting the parsed info in the guidance_data struct and returns it */
guidance_data* set_entx(char *,char *,char *);

/*removes entry/ extern prefix from the input line*/
void remove_prefix(char *,const char *);

/*checks if this entry has only upper case letters *Note implemented according to forum ansewrs(danny)regarding it*/
int is_entry(char *);





