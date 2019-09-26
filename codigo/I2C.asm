/*
* AssemblerApplication5.asm
*
*  Created: 4/21/2014 1:00:03 AM
*   Author: alouie
*/
.device atmega328p
.nolist
.include "m328Pdef.inc"
;.include "macros.inc"
.list
; ISR table

.org $0000
rjmp Reset
;—————————————————————–
; DEFINES
;—————————————————————–

.def        temp =r16            ;worker register
.def        data =r17

; Equate statements
.equ        start        = $08        ; Start Condition
.equ        Rep_start    = $10        ; Repeated Start Condition Message
.equ        MT_SLA_ACK    = $18        ; Master Transmitter Slave Address Acknowledge
.equ        MT_DATA_ACK = $28        ; Master Transmitter Data Acknowledge
.equ        MT_DATA_NACK= $30        ; Master Transmitter Data Not Acknowledge
.equ        MR_SLA_ACK    = $40        ; Master Receiver Slave Address Acknowledge
.equ        MR_SLA_NACK    = $48        ; Master Receiver Slave Address Acknowledge
.equ        MR_DATA_ACK = $50        ; Master Receiver Data Acknowlede
.equ        MR_DATA_NACK= $58        ; Master Receiver Data Not Acknowledge
.equ        W            = 0            ; Write Bit
.equ        R            = 1            ; Read Bit
.equ        SLA            = $D0        ; Slave Address of AMG8852
.equ        Inches        = $50        ; Return result in Inches
.equ        CommandReg    = $80        ; SRF08 Command Register
;—————————————————————–
; Reset
;—————————————————————–

Reset:
;—–Setting Stackpointer—————————————-
ldi        temp,low(RAMEND)            ; Set stackptr to ram end
out        SPL,temp
ldi     temp, high(RAMEND)
out     SPH, temp

;—————————————————————–
;setup DDR/IO
;—————————————————————–
;set pullups, DDRC output
ldi temp, 0xff
out DDRC,temp
out    PORTC,temp

;—————————————————————–
;setup speed etc. of i2c port
;—————————————————————–
;400Khz=8MHz/(16+2*TWBR*CLKPRS) TWBR and clock presecaler set
;i2c freq.  clock prescaler default is 1, and is set in
;TWSR
	ldi    temp,72
	sts    TWBR,temp

	ldi    temp,0x00
	sts    TWSR,temp

	SBI 	DDRB,5
	CBI 	PORTB,5 	

	call I2CSTART
	ldi data,0x80
	call I2CPUT
	ldi data,0x00
	call I2CPUT
	ldi data,0x20
	call I2CPUT
	call I2CSTOP

	
	CALL 	I2CSTART
	LDI 	DATA,0x80
	CALL	I2CPUT
	LDI 	DATA,0xFE
	CALL	I2CPUT
	LDI 	DATA,0x65
	CALL	I2CPUT
	CALL	I2CSTOP

rjmp I2CLOOP

I2CLOOP:
	SBI 	PORTB,5 	
	CALL 	I2CSTART
	LDI 	DATA,0x80
	CALL	I2CPUT
	LDI 	DATA,0x06
	CALL	I2CPUT
	LDI 	DATA,low(0x00)	;LED0_ON_LOW
	CALL	I2CPUT
	LDI 	DATA,HIGH(0x00)	;LED0_ON_HIGH
	CALL	I2CPUT
	LDI 	DATA,LOW(0xAA)	;LED0_OFF_LOW
	CALL	I2CPUT
	LDI 	DATA,HIGH(0xAA)	;LED0_OFF_HIGH
	CALL	I2CPUT
	CALL	I2CSTOP
	
	CALL 	DEMORA
	CBI 	PORTB,5 		

	CALL 	I2CSTART
	LDI 	DATA,0x80
	CALL	I2CPUT
	LDI 	DATA,0x06
	CALL	I2CPUT
	LDI 	DATA,low(0x00)	;LED0_ON_LOW
	CALL	I2CPUT
	LDI 	DATA,HIGH(0x00)	;LED0_ON_HIGH
	CALL	I2CPUT
	LDI 	DATA,LOW(0x258)	;LED0_OFF_LOW
	CALL	I2CPUT
	LDI 	DATA,HIGH(0x258)	;LED0_OFF_HIGH
	CALL	I2CPUT
	CALL	I2CSTOP

	CALL 	DEMORA



;call I2CSTART
;ldi data,0xD1
;call I2CPUT
;call I2CGETACK
;call I2CGETNACK
;call I2CSTOP
;take a little nop
;good for debugging

rjmp I2CLOOP


DEMORA: 
	ldi R16,255
	ldi R17,255
	ldi R18,255
BACK:
	dec R16
	brne BACK
	ldi R16,255
 	DEC	R17	
	brne BACK
	ldi R16,255
	ldi R17,255
	dec R18
	brne BACK
	ret

;—————-SEND I2C START—————————————————————–
;—————-This will send a DATA out as the address—————————————
;—————-sends start condition and address———————————————-
I2CSTART:
ldi         temp, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
sts             TWCR,temp

;wait for start condition to be sent.  when TWINT in TWCR is cleared, it is sent
WAIT_START:
lds         temp,TWCR
sbrs         temp,TWINT
rjmp         WAIT_START

;check TWSR for bus status.
;andi masks last three bits, which are 2=? 1:0prescaler value
lds         temp,TWSR
andi        temp,0b11111000
cpi         temp,START
breq        PC+2
jmp            errloop
ret

;————–PUT I2C————————————————————————–
;————–bytes is stored in r17 aka data————————————————–
;use this by putting the address or data to put on the i2c line in data (r17)
;then this function will disable the TW int, write data to TWDR, and wait for an ack
I2CPUT:
NEXT1:
ldi         temp,(0<<TWINT) | (1<<TWEN)
sts             TWCR,temp
;ldi         temp,SLA+W
sts             TWDR,data
ldi         temp,(1<<TWINT) | (1<<TWEN)
sts             TWCR,temp

;another wait for the TWINT flag, which lets us know if ACK/NACK is back (received
;but I couldnt help myself with that rhyme)
WAIT_DONE:
lds         temp,TWCR
sbrs         temp,TWINT
rjmp         WAIT_DONE

;check and see if TWSR is ACK, or not.  if it is, keep going
;lds         temp,TWSR
;andi        temp,0b11111000
;cpi         temp,MT_SLA_ACK
;breq        SEND_REG
;jmp            errloop
ret

;—————-GET I2C ———————————————————————
;—————-received data is stored in r17 aka data————————————–
;
I2CGETACK:
;enable TWCR, then wait for TWINT
ldi            temp,(1<<TWINT) | (1<<TWEN) | (1<<TWEA)
sts             TWCR,temp
rjmp I2CGET

I2CGETNACK:
ldi            temp,(1<<TWINT) | (1<<TWEN)
sts             TWCR,temp

I2CGET:
WAIT_FOR_BYTE:
lds         temp,TWCR
sbrs         temp,TWINT
rjmp         WAIT_FOR_BYTE

lds         data,TWDR

;check and see if TWSR is ACK, or not.  if it is, keep going
;lds         temp,TWSR
;andi        temp,0b11111000
;cpi         temp,MT_SLA_ACK
;breq        SEND_REG
;jmp            errloop

ret

;—————-SEND I2C STop—————————————————————–
I2CSTOP:
ldi         temp,(1<<TWINT)|(1<<TWSTO)|(1<<TWEN)
sts            TWCR,temp

;check TWCR to see if there is still a transmission- if not, stop bit has been sent
Check1:
lds            temp,TWCR
andi        temp,0b00010000        ; Check to see that no transmission is going on
brne        Check1
ret

;error hell
errloop:
rjmp errloop
