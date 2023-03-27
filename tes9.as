
.entry LENGTH
.extern W
MAIN: mov r3, LENGTH	
LOOP: jmp L1(#-1,r6)
			mcr m1
			sub r2, r4
			bne L3
			endmcr
			mcr m2
			sub r1, r6
			bne W
			endmcr
			mcr m3
			sub r1, r4
			bne L3
			endmcr
			mcr m4
			mov r3, LOOP
			bne L3
			endmcr
			prn #-5
			bne W(r4,r5)
		  m1
		  m2
L1: inc K
.entry LOOP			
			bne LOOP(K,W)
			m3
			m4
END: 	stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: 		.data 22
.extern L3
