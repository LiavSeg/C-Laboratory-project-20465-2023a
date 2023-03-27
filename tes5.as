.entry LENGTH
.extern W
MAIN: mov r3, LENGTH	
LOOP: jmp L1(#-1,r6)
			mcr m1
			sub r5, r4
			bne L3
			endmcr
		
		  m1
L1: inc K
.entry LOOP			
			bne LOOP(K,W)
			
END: 	stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: 		.data 22
.extern L3
