#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 81
#define POST_FIX 4


/**
 * Struct definition for a macro.
 * A macro consists of a mcr name,content and a pointer to the next macro in a linked list.
 */
typedef struct macro{
	char* name;
	char* content;
	struct macro* next;
	} macro;

/*
 * This function preprocesses a given file, extracts macros from and writes 
 * the result to an output file.
 *
 * Command line arguments:
 * - source_name: name of the source file/s to preprocess.
 */
int preprocessor(char *);

/*Simple for loop to check if there are any chars that aren't blanks*/
int	isblank(char *);

/**
 * Searches for a macro with the given name in the macro table and writes its contents
 * to the given file stream.
 * @param table pointer to the first element of the macro table
 * @param name pointer to the name of the macro to search for
 * @param post pointer to the file stream where the contents of the macro should be written
 * @return 1 if the macro was found and its contents were successfully written to the file, 0 otherwise
 */	
int search_macro(macro *,char *,FILE *);

/** 
 * Copies any contet of the source file that  into the after macro file
 * @param strr *pointer for the line to tokenize.
 * @param pre FILE  copies its content
 * @param macro *pointer of the first value of the macro table.
 * @throws FileNotFoundException If memory cannot allocated
 * @returns dynamically allocated string of the mcr name 
 */

void copy_content(char *,FILE *,macro *);


/** 
 * Adds a name and a mcro content using  - find_macro_name -find_macro_content
 * Implemented as a linked list therfore each node will get allocated and added separately
 * @param line pointer of the line the macro strats value of the macro table.
 * @param table ptr of a ptr of the macros table head
 * @param pre FILE.
 * @throws FileNotFoundException If memory cannot allocated
 *  
 */
void add_to_table(char *,macro **,FILE *);

/** 
 * Free any allocated memory of the macro table 
 * Implemented as a linked list therfore each node will get deallocated separately
 * @param macro *pointer of the first value of the macro table.
 * @throws FileNotFoundException If memory cannot allocated
 * @returns dynamically allocated string of the mcr name 
 */

void free_table(macro *);

/** 
 * Finds a macro a name after "mcr" 
 * It dynamically allocates memory for the content based on the length of the token after mcr.
 * @param pre FILE.
 * @throws FileNotFoundException If memory cannot allocated
 * @returns dynamically allocated string of the mcr name 
 */
 
char *find_macro_name(char *,FILE* );

/** 
 * Finds a macro content bounded by "mcr"...content..-"endemcr"
 * It dynamically allocates memory for the content based on the length of the bounded content.
 * @param pre FILE.
 * @throws FileNotFoundException If memory cannot allocated
 * @returns dynamically alloacted string of the mcr content
 */
char *find_macro_content(FILE *);

/** 
 * Opens a temp file with all macro names for further analysis in first pass
 * @param table macro table.
 * @throws FileNotFoundException If the source file cannot be opened.
 */
void print_macro_names(macro *);
	
