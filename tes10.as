
MAIN: mov r3, LENGTH	
LOOP: jmp L1(#-1,r6)
			mcr m1
			sub r1, r2
			bne END
			endmcr
			
			mcr m4
			sub r3, r4
			endmcr
			prn #-5
			bne LOOP(r4,r5)
		  m1
		  
		  m4
L1: inc K
			bne loop(K,STR)
			
END: 	stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: 		.data 22

