.def        temp =r20            ;worker register
.def        I2C_DATA =r21

; Equate statements
.equ        start        		= $08        ; Start Condition
.equ        Rep_start    		= $10        ; Repeated Start Condition Message
.equ        MT_SLA_ACK   		= $18        ; Master Transmitter Slave Address Acknowledge
.equ        MT_I2C_DATA_ACK 	= $28        ; Master Transmitter I2C_DATA Acknowledge
.equ        MT_I2C_DATA_NACK	= $30        ; Master Transmitter I2C_DATA Not Acknowledge
.equ        MR_SLA_ACK    		= $40        ; Master Receiver Slave Address Acknowledge
.equ        MR_SLA_NACK    		= $48        ; Master Receiver Slave Address Acknowledge
.equ        MR_I2C_DATA_ACK 	= $50        ; Master Receiver I2C_DATA Acknowlede
.equ        MR_I2C_DATA_NACK	= $58        ; Master Receiver I2C_DATA Not Acknowledge
.equ        W            		= 0          ; Write Bit
.equ        R            		= 1          ; Read Bit
;—————————————————————–
; Reset
;—————————————————————–


I2C_SETUP:
;—————————————————————–
;setup DDR/IO
;—————————————————————–
;set pullups, DDRC output
	PUSH	TEMP
	ldi    	temp, 0xff
	out    	DDRC,temp
	out    	PORTC,temp

;—————————————————————–
;setup speed etc. of i2c port
;—————————————————————–
;400Khz=8MHz/(16+2*TWBR*CLKPRS) TWBR and clock presecaler set
;i2c freq.  clock prescaler default is 1, and is set in
;TWSR
	ldi    	temp,72
	sts    	TWBR,temp

	ldi    	temp,0x00
	sts    	TWSR,temp
	POP 	TEMP
	RET

;—————-SEND I2C START—————————————————————–
;—————-This will send a I2C_DATA out as the address—————————————
;—————-sends start condition and address———————————————-
I2C_START:
	PUSH 	TEMP
	ldi    	temp, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
	sts     TWCR,temp

;wait for start condition to be sent.  when TWINT in TWCR is cleared, it is sent
WAIT_START:
	lds     temp,TWCR
	sbrs    temp,TWINT
	rjmp    WAIT_START
;check TWSR for bus status.
;andi masks last three bits, which are 2=? 1:0prescaler value
	lds     temp,TWSR
	andi    temp,0b11111000
	cpi     temp,START
	breq    PC+2
	jmp     errloop
	POP 	TEMP
	ret

;————–PUT I2C————————————————————————–
;————–bytes is stored in r17 aka I2C_DATA————————————————–
;use this by putting the address or I2C_DATA to put on the i2c line in I2C_DATA (r21)
;then this function will disable the TW int, write I2C_DATA to TWDR, and wait for an ack
I2C_PUT:
	PUSH 	TEMP
NEXT1:
	ldi     temp,(0<<TWINT) | (1<<TWEN)
	sts     TWCR,temp
	sts     TWDR,I2C_DATA
	ldi     temp,(1<<TWINT) | (1<<TWEN)
	sts     TWCR,temp
WAIT_DONE:
	lds     temp,TWCR
	sbrs    temp,TWINT
	rjmp    WAIT_DONE
	POP 	TEMP
	ret

I2C_GET:
	PUSH 	TEMP
WAIT_FOR_BYTE:
	lds     temp,TWCR
	sbrs    temp,TWINT
	rjmp    WAIT_FOR_BYTE
	lds     I2C_DATA,TWDR
	POP		TEMP
	ret

;—————-SEND I2C STop—————————————————————–
I2C_STOP:
	PUSH 	TEMP
	ldi     temp,(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)
	sts     TWCR,temp
;check TWCR to see if there is still a transmission- if not, stop bit has been sent
Check1:
	lds     temp,TWCR
	andi    temp,0b00010000        ; Check to see that no transmission is going on
	brne    Check1
	POP 	TEMP
	ret