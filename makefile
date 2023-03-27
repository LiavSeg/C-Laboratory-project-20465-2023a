assembler: assembler.o preprocessor.o macrolist.o first_pass.o labeltable.o islabel.o isvalidargs.o is_command.o command_coding.o  command_coding_analysis.o processGuidance.o processGuidanceData.o processGuidanceString.o processEntryOrExtern.o second_pass.o
	gcc -g assembler.o preprocessor.o macrolist.o first_pass.o  labeltable.o islabel.o isvalidargs.o is_command.o command_coding.o command_coding_analysis.o processGuidance.o processGuidanceData.o processGuidanceString.o processEntryOrExtern.o  second_pass.o -Wall -ansi -pedantic -o assembler
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
preprocessor.o: preprocessor.c preprocessor.h
	gcc -c -Wall -ansi -pedantic preprocessor.c -o preprocessor.o
macrolist.o: macrolist.c preprocessor.h
	gcc -c -Wall -ansi -pedantic macrolist.c -o macrolist.o
first_pass.o: first_pass.c first_pass.h
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o
labeltable.o: labeltable.c
	gcc -c -Wall -ansi -pedantic labeltable.c -o labeltable.o
islabel.o: islabel.c 
	gcc -c -Wall -ansi -pedantic islabel.c -o islabel.o	
isvalidargs.o: isvalidargs.c isvalidargs.h
	gcc -c -Wall -ansi -pedantic isvalidargs.c -o isvalidargs.o					
is_command.o: is_command.c is_command.h 
	gcc -c -Wall -ansi -pedantic is_command.c -o is_command.o														
command_coding.o: command_coding.c command_coding.h
	gcc -c -Wall -ansi -pedantic command_coding.c -o command_coding.o							
command_coding_analysis.o: command_coding_analysis.c command_coding.h
	gcc -c -Wall -ansi -pedantic command_coding_analysis.c -o command_coding_analysis.o									
processGuidance.o: processGuidance.c guidance_analysis.h
	gcc -c -Wall -ansi -pedantic processGuidance.c -o processGuidance.o
processGuidanceData.o: processGuidanceData.c guidance_analysis.h
	gcc -c -Wall -ansi -pedantic processGuidanceData.c -o processGuidanceData.o
processGuidanceString.o: processGuidanceString.c guidance_analysis.h
	gcc -c -Wall -ansi -pedantic processGuidanceString.c -o processGuidanceString.o	
processEntryOrExtern.o: processEntryOrExtern.c guidance_analysis.h
	gcc -c -Wall -ansi -pedantic processEntryOrExtern.c -o processEntryOrExtern.o	
codingGuidance.o: codingGuidance.c guidance_analysis.h
	gcc -c -Wall -ansi -pedantic codingGuidance.c -o codingGuidance.o	
second_pass.o: second_pass.c second_pass.h
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o		
		

