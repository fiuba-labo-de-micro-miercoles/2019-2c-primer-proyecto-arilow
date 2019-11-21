.INCLUDE 	"I2C.INC"


.EQU	PCA9685_DIR 		= 0x40
.EQU 	PCA9685_MODE1		= 0x00
.EQU 	PCA9685_LED0 		= 0x06
.EQU 	PCA9685_PRE_SCALE 	= 0xFE


PCA9685_SETUP:


	PUSH 	R21
	RCALL 	I2C_SETUP

	
	RCALL	I2C_START
	LDI 	R21,(PCA9685_DIR << 1)
	RCALL 	I2C_PUT
	LDI 	R21,PCA9685_MODE1


	RCALL 	I2C_PUT

	LDI 	R21,0x20 ;SET AUTO INCREMENT
	RCALL 	I2C_PUT
	RCALL 	I2C_STOP


	RCALL	I2C_START
	LDI 	R21,(PCA9685_DIR << 1)
	RCALL 	I2C_PUT
	LDI 	R21,PCA9685_PRE_SCALE
	RCALL 	I2C_PUT
	LDI 	R21,0x65 	
	RCALL 	I2C_PUT
	RCALL 	I2C_STOP


	POP 	R21
	RET

;PRE
;R23 -> SERVO NUMBER
;R24 -> POS LOW
;R25 -> POS HIGH
PCA9685_MOVE:
	PUSH	R21
	;MULTIPLY BY 4
	LSL 	SERVO_NUMBER
	LSL 	SERVO_NUMBER
	;ADD 	LED0 DIR
	;CLR 	R21
	LDI 	R21,PCA9685_LED0
	ADD		SERVO_NUMBER,R21
	
	RCALL	I2C_START
	LDI 	R21,(PCA9685_DIR << 1)
	RCALL 	I2C_PUT
	MOV 	R21,SERVO_NUMBER
	RCALL	I2C_PUT
	LDI 	R21,0x00			;LEDN_ON_LOW
	RCALL 	I2C_PUT	
	LDI 	R21,0x00			;LEDN_ON_HIGH
	RCALL 	I2C_PUT
	MOV 	R21,POSITION_LOW	;LEDN_OFF_LOW
	RCALL 	I2C_PUT
	MOV 	R21,POSITION_HIGH	;LEDN_OFF_HIGH
	RCALL 	I2C_PUT
	RCALL 	I2C_STOP

	POP 	R21
	RET