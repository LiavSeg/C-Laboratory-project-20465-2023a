#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 82
#define POST_FIX 4

typedef struct macro{
		
	char* name;
	char* content;
	
	struct macro* next;
	} macro;

	int	isblank(char *);
	int search_macro(macro *,char *,FILE *);
	void preprocessor(char *);
	void copy_content(char *,FILE *,macro *);
	void add_to_table(char *,macro **,FILE *);
	void free_table(macro *);
	char *find_macro_name(char *,FILE* );
	char *find_macro_content(FILE *);
	
