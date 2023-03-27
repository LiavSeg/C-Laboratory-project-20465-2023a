MAINMAINMAINMAINMAINMAINMAINMAIN: mov r3, LENGTH	
.entry L
.extern L
LOOP: jmp L1(#-1,r6)
			mcr m1
			sub r1, r2
			bne END
			endmcr
			mcr m2
			bne END
			endmcr
			mcr m4
			sub r3, r4
			endmcr
			prn #-5
			bne LOOP(r4,r5)
		  m1
		  m2
		  m4
L1: inc K
			bne LOOP(K,STR)
			
END: 	stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: 		.data 22
1Mylength: jmp K
	my length: jmp K12
	VERYLONGLABELTHATISNTLEAGLITHINKIPASSED30: stop
	mov r3,%length
	add r131,-5
	lea mov,r12
	cmp .string,.data
	sub r1,#3
	add r0,#-5
	
