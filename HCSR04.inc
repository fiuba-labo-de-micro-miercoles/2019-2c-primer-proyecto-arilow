HCSR04_SETUP:
	PUSH	R16
	clr 	r16 ;
	sts 	TCCR1A, r16 ; normal mode timer
	sts 	TCCR1C, r16 ; just set zero, using normal mode
	
	; Next INT0
	; int0 interupt PD2
	cbi 	DDRD, 2 ; set for input
	ldi 	r16, 0b00000001
	sts 	EICRA, r16 ; any change on int0 to trigger interrupt
	CBI 	EIMSK,0
	sbi 	DDRC, 5 ; output Portc5, Arduino pin Analog in 5, trigger the HC-SR04
	POP 	R16
	RET
 	

HCSR04_READ:
	clr 	r22 ; test value, so the led stays on long enough to see will be 0xFF
	clr 	r21 ;test value, will be 0x24 with a 16Mhz clock
	sts 	TCNT1H,r22 ; high byte Order write (and reading) to 16 bit registers must be done in
	sts 	TCNT1L,r21 ; proper order, see datasheet
	CLT
	CLR 	R16
	STS 	TIFR1,R16 	; CLEAR OVF FLAG
	LDI 	R16,0x01
	STS 	TIMSK1, R16	; overflow inturrupt enable
	SBI 	EIMSK,0		;ENBLE INT0
	SEI
	ldi 	r16, 0b00000001
	sts 	TCCR1B, r16 ; start the timer with clk 1
	sei 	;turn interupts back on	
	sbi 	PORTC,5 ; start the trigger
	LDI 	R16,20
HCSR04_LOOP2:
	NOP
	DEC 	R16	
	BRNE 	HCSR04_LOOP2	
	CBI 	PORTC,5 ;stop the trigger	
HCSR04_LOOP:
	BRTC 	HCSR04_LOOP		;IF T==0 => LOOP
	CBI 	EIMSK,0
	CBI 	PORTC, 5 
	RET


	

; connect a led to PORTC5, with the longer delay set above ie: ldi r27, 0x00 ; zero so long enough delay to see a LED
; generic delay so have time to see LEDs flash. If you don't know how to connect a LED yet,Joe Pardue's
;“Aduino Workshop” is a good place to start (lots more than just connecting LEDs).
delay_05:
	push r22
	push r24
	push r25
	ldi r22, 100 ;
outer_loop:
	ldi r24, low(0137)
	ldi r25, high(0137)
delay_loop:
	adiw r25:r24,1
	brne delay_loop
	nop
	dec r22
	brne outer_loop
	pop r25
	pop r24
	pop r22
	ret


EXT_INT0:
	SBI PORTB,5
	CBI 	EIMSK,0
	SET
	clr 	r16 
	sts 	TCCR1B, r16 
	sts 	TCCR1B, r16 ;stop the clock has the time 
	sts 	TCNT1H, r22 ; high byte Order write (and reading) to 16 bit registers must be done in
	sts 	TCNT1L, r21 ; proper order, see datasheet
	reti


TIM1_OVF:
	CBI PORTB,5
	CBI 	EIMSK,0
	clr 	r16 
	sts 	TCCR1B, r16 
	SET
	LDI 	R22, 0xFF
	LDI 	R21, 0xFF
	reti
