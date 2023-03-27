#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
#define AD_POS 100

/*This struct represents a line of a label table for an assembler */
typedef struct label_table{
    char name[MAX_LINE_SIZE];/*Lable's name*/
    char type[TYPE_OF_CMD_LENGTH];/*Lable's type - command/guidance -entry,extern,string,data*/
    int address;/*Lable's address - where it is located on the source file according to DC,IC*/
    int value;/*Lable's Value in case it's a pre data/string label[will be the dc before updating its data]*/
    struct label_table *next;/*Next label in line*/
}label_table;

/*
 * Function to add a label to the label table
 * @param head pointer to the head of the label table
 * @param name name of the label to add
 * @param type type of the label to add (ST, DT, EX, EN)
 * @param address address of the label to add
 * @param value value of the label to add (for ST and DT types)
 * @return a pointer to the head of the updated label table
 * Implemented by a linked list of structs
 */
label_table *add_label(label_table *,char *,char *,int,int);

/*same as above but for entry/extern table*/
label_table *add_entx(label_table *, char *, char *);	

/*Tags a label with a matching type and returns its address*/
int mark_label(label_table *,label_table *);

/*Gets a specific label and return it if not found by a name input retrun null*/
label_table *get_label(label_table *, char *);

/*Print func*/	
void print_labels(label_table *);

/*Finds a label if it exists return its address*/
int find_label(label_table *,char *);

/*Free any dynamically alocted memory for the label table*/
void free_label_table(label_table *);
