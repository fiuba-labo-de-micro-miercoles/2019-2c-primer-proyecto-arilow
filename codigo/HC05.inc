HC05_SETUP:
	;SET TRANSMITION SPEED 9600 BAUDS
	LDI 	TEMP, HIGH(16000000/(8*9600) - 1)
	STS 	UBRR0H, TEMP
	LDI 	TEMP, LOW(16000000/(8*9600) - 1)
	STS 	UBRR0L, TEMP

	;SET DOUBLE SPEED
	LDI 	TEMP, (1<<U2X0)
	STS 	UCSR0A, TEMP

	;ENABLE RECEPTION AND INTERRUPT
	LDI 	TEMP, (1<<RXCIE0)|(1<<RXEN0)
	STS 	UCSR0B, TEMP

	;SET 8 BIT FORMAT + PAIR BIT
	LDI 	TEMP, (1<<UCSZ01)|(1<<UCSZ00)
	STS 	UCSR0C, TEMP

	SEI

	RET

HC05_RECEPTION_HANDLER:
	CLI
	LDS 	TEMP, UDR0
	SUBI 	TEMP, '0'
	STS 	GPIOR0, TEMP
	RETI