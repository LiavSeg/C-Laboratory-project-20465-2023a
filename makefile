assembler: assembler.o preprocessor.o macrolist.o find_macro_content.o find_macro_name.o isblank.o copy_content.o
	gcc -g assembler.o preprocessor.o macrolist.o find_macro_content.o find_macro_name.o isblank.o copy_content.o -Wall -ansi -pedantic -o assembler
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
preprocessor.o: preprocessor.c preprocessor.h
	gcc -c -Wall -ansi -pedantic preprocessor.c -o preprocessor.o
macrolist.o: macrolist.c preprocessor.h
	gcc -c -Wall -ansi -pedantic macrolist.c -o macrolist.o
find_macro_content.o: find_macro_content.c macrofunc.h
	gcc -c -Wall -ansi -pedantic find_macro_content.c -o find_macro_content.o
find_macro_name.o: find_macro_name.c macrofunc.h
	gcc -c -Wall -ansi -pedantic find_macro_name.c -o find_macro_name.o
isblank.o: isblank.c isblank.h
	gcc -c -Wall -ansi -pedantic isblank.c -o isblank.o
copy_content.o: copy_content.c preprocessor.h
	gcc -c -Wall -ansi -pedantic copy_content.c -o copy_content.o
