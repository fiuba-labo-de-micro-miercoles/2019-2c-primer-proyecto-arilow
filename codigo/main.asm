.INCLUDE 	"M328PDEF.INC"

.CSEG

.ORG 	0x00
	RJMP		MAIN

;INTERRUPCIONES



.ORG 	INT_VECTORS_SIZE
MAIN:
	LDI		R16,LOW(RAMEND)
	OUT		SPL,R16
	LDI 	R16,HIGH(RAMEND)
	OUT 	SPH,R16

	RCALL	MOVEMENT_SETUP


LOOP:
	RJMP	LOOP



;error hell
ERRLOOP:
	RJMP	ERRLOOP


.INCLUDE 	"MOVEMENT.INC"
