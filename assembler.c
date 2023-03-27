#include "assembler.h"
/*Main function of the assembler project*/

int main(int argc, char *argv[]){
	int i;	
	/* Preprocess each file */
	for(i = 1; i<argc;i++){
		if(preprocessor(argv[i]))
			first_pass(argv[i]);/* Perform first pass on each valid file */	
	}

	return 0;
}

