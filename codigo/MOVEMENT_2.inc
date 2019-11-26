.EQU 		SERVO_MAX = 2600
.EQU 		SERVO_MID = 1000
.EQU 		SERVO_MIN = 500

.EQU		LEG_1 = 0X0102
.EQU		LEG_2 = 0X0304
.EQU		LEG_3 = 0X0506
.EQU		LEG_4 = 0X0708

.EQU		SERVO_TESTER = 1

.INCLUDE 	"PCA9685.INC"






MOVEMENT_SETUP:
	RCALL 	PCA9685_SETUP
	RCALL	INITIAL_POSITION

	RET

INITIAL_POSITION:

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG	
	;PATA 1 A 135�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG	
	;PATA 2 A 135�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG	
	;PATA 3 A 45�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG	
	;PATA 4 A 45�
	;RCALL	MOVEMENT_DELAY

	RET	


ROLL_LEG:
	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_L
	RCALL 	PCA9685_MOVE	

	RET

YAW_LEG:
	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_H
	RCALL 	PCA9685_MOVE	
	
	RET

MOVE_FORWARD:
	RCALL	MOVE_RIGHT_FORWARD
	RCALL	MOVE_LEFT_FORWARD

;	RCALL	MOVEMENT_DELAY
;	RCALL	MOVEMENT_DELAY

	RET

MOVE_LEFT_FORWARD:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 1 ABAJO
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO
	LDI		XL,LOW(ANG_YAW_30)
	LDI		XH,HIGH(ANG_YAW_30)
	RCALL	YAW_LEG
	;PATA 4 A 45�


	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PARA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG
	;PATA 2 A 150�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 3 A 90�

	RCALL DELAY_10_S
	RET


MOVE_RIGHT_FORWARD:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
;	RCALL 	DELAY_10_S		
	;PATA 3 A 45�


	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_45)
	RCALL	ROLL_LEG
	;PARA 1 ABAJO
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG
	;PATA 1 A 150�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 4 A 90�


	RCALL DELAY_10_S
	RET

TURN_RIGHT:
  TURN_RIGHT_MOVE_1:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 2 ABAJO
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 135�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 3 ABAJO
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 3 A 45�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_10_S

  TURN_RIGHT_MOVE_2:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 1 ABAJO
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 135�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG
	;PATA 2 A 180�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		XL,LOW(ANG_YAW_0)
	LDI		XH,HIGH(ANG_YAW_0)
	RCALL	YAW_LEG
	;PATA 3 A 0�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 45�

	RCALL DELAY_10_S

  TURN_RIGHT_MOVE_3:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_10_S

	RCALL	INITIAL_POSITION

	RET


TURN_LEFT:
  TURN_LEFT_MOVE_1:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 2 ABAJO
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 135�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 3 ABAJO
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 3 A 45�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_10_S

  TURN_LEFT_MOVE_2:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 1 ABAJO
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 135�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG
	;PATA 2 A 180�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		XL,LOW(ANG_YAW_0)
	LDI		XH,HIGH(ANG_YAW_0)
	RCALL	YAW_LEG
	;PATA 3 A 0�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 45�

	RCALL DELAY_10_S

  TURN_LEFT_MOVE_3:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_10_S

	RCALL	INITIAL_POSITION

	RET



STAND_GREET_LEFT: ;SALUDA

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_60)
	LDI		XH,HIGH(ANG_YAW_60)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_60)
	LDI		XH,HIGH(ANG_YAW_60)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	RCALL MOVEMENT_DELAY

  GREET_LEFT:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_180)
	LDI		XH,HIGH(ANG_ROLL_180)
	RCALL	ROLL_LEG	

	RCALL MOVEMENT_DELAY

	LDI R16, 3

  LOOP_GREET_LEFT:
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG	

	RCALL DELAY_10_S

	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	

	RCALL DELAY_10_S
	DEC R16

	BRNE LOOP_GREET_LEFT
	RCALL INITIAL_POSITION
	RET

;HAY QUE INICIALIZAR X CON EL VALOR DEL ANGULO	EN EL QUE SE QUIERE PONER LA PATA
MOVEMENT_LEG:
	PUSH	XL
	PUSH	XH

	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)

	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_L
;	LDI 	R24,ANG_L
;	LDI 	R25,ANG_H
	RCALL 	PCA9685_MOVE
	RCALL 	MOVEMENT_DELAY
	
	POP		XH
	POP		XL

	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_H
;	MOV 	R24,ANG_L
;	MOV 	R25,ANG_H
	RCALL 	PCA9685_MOVE
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)

	RCALL	FIND_ANG_LEG

	MOV 	R23,LEG_L
;	LDI 	R24,LOW(500)
;	LDI 	R25,HIGH(500)
	RCALL 	PCA9685_MOVE
	RCALL 	MOVEMENT_DELAY

	RET

ROLL_LEG_TEST:
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_45)
	LDI		XH,HIGH(ANG_ROLL_45)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_90)
	LDI		XH,HIGH(ANG_ROLL_90)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_120)
	LDI		XH,HIGH(ANG_ROLL_120)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_150)
	LDI		XH,HIGH(ANG_ROLL_150)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_ROLL_180)
	LDI		XH,HIGH(ANG_ROLL_180)
	RCALL	ROLL_LEG
	RCALL 	MOVEMENT_DELAY

	RET
	
YAW_LEG_TEST:
	LDI		XL,LOW(ANG_YAW_0)
	LDI		XH,HIGH(ANG_YAW_0)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_15)
	LDI		XH,HIGH(ANG_YAW_15)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY


	LDI		XL,LOW(ANG_YAW_30)
	LDI		XH,HIGH(ANG_YAW_30)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_60)
	LDI		XH,HIGH(ANG_YAW_60)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_75)
	LDI		XH,HIGH(ANG_YAW_75)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY


	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_105)
	LDI		XH,HIGH(ANG_YAW_105)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY


	LDI		XL,LOW(ANG_YAW_120)
	LDI		XH,HIGH(ANG_YAW_120)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_165)
	LDI		XH,HIGH(ANG_YAW_165)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG
	RCALL 	MOVEMENT_DELAY

	RET

MOVEMENT_DELAY:
	push r22
	push r24
	push r25
	ldi r22, 100 ;
MOVEMENT_OUTER_LOOP:
	ldi r24, low(90)
	ldi r25, high(90)
MOVEMENT_DELAY_LOOP:
	adiw r25:r24,1
	brne MOVEMENT_DELAY_LOOP
	nop
	dec r22
	brne MOVEMENT_OUTER_LOOP
	pop r25
	pop r24
	pop r22
	ret


