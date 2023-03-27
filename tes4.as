.entry LENGTH
.entry LOOP
.entry STR
.extern W
MAIN: mov r3, LENGTH	
LOOP: jmp L1(#-1,r6)
			mcr m1
			sub r1, r2
			bne END
			endmcr
			prn #-5
.entry LOOP			
			bne LOOP(r4,r5)
		  m1
L1: inc K
			bne LOOP(W,STR)
			
END: 	stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: 		.data 22

