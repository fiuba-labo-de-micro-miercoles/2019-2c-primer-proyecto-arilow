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
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG	
	;PATA 1 A 135�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG	
	;PATA 2 A 135�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG	
	;PATA 3 A 45�
	;RCALL	MOVEMENT_DELAY

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG	
	;PATA 4 A 45�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	

	RCALL	WATCH_FRONT

	RET	

MOVEMENT_SWITCH:	;ACTUA COMO M�QUINA DE ESTADOS
	LDS 	TEMP, GPIOR0
	CPI 	TEMP, 0
	BREQ 	TRY_TURN_LEFT
	CPI 	TEMP, 1
	BREQ 	TRY_MOVE_FORWARD
	CPI 	TEMP, 2
	BREQ 	TRY_TURN_RIGHT
	CPI 	TEMP, 3
	BREQ 	TRY_INITIAL_POSITION
	CPI 	TEMP, 6
	BREQ 	TRY_STAND_GREET_LEFT
	CPI 	TEMP, 7
	BREQ 	TRY_PLAY_DEAD


	RET




TRY_PLAY_DEAD:
	RCALL 	WATCH_RIGHT
	RCALL	DELAY_100_MS
	RCALL 	WATCH_LEFT
	RCALL	DELAY_100_MS

	RCALL	PLAY_DEAD

	RET



TRY_TURN_RIGHT:
	RCALL 	WATCH_RIGHT
	RCALL 	HCSR04_CHECK
	LDS		TEMP, GPIOR1
	CPI		TEMP, OBSTACLE_NOT_FOUND
	BRNE	INITIAL_POSITION_LABEL
	RCALL 	TURN_RIGHT
	RET

TRY_TURN_LEFT:
	RCALL 	WATCH_LEFT
	RCALL 	HCSR04_CHECK
	LDS		TEMP, GPIOR1
	CPI		TEMP, OBSTACLE_NOT_FOUND
	BRNE	INITIAL_POSITION_LABEL
	RCALL 	TURN_LEFT
	RET

TRY_MOVE_FORWARD:
	RCALL 	WATCH_FRONT
	RCALL 	HCSR04_CHECK
	LDS		TEMP, GPIOR1
	CPI		TEMP, OBSTACLE_NOT_FOUND
	BRNE	INITIAL_POSITION_LABEL
	RCALL 	MOVE_FORWARD
	RET

TRY_INITIAL_POSITION:
	RCALL	INITIAL_POSITION
	SLEEP
	RET

TRY_STAND_GREET_LEFT:
	RCALL 	STAND_GREET_LEFT
	RET


INITIAL_POSITION_LABEL:
	RJMP 	INITIAL_POSITION

WATCH_FRONT:
	LDI 	SERVO_NUMBER, 0
	LDI 	POSITION_LOW ,LOW(1000)
	LDI 	POSITION_HIGH, HIGH(1000)
	RCALL 	PCA9685_MOVE
	RET

WATCH_RIGHT:
	LDI 	SERVO_NUMBER, 0
	LDI 	POSITION_LOW ,LOW(500)
	LDI 	POSITION_HIGH, HIGH(500)
	RCALL 	PCA9685_MOVE
	RET

WATCH_LEFT:
	LDI 	SERVO_NUMBER, 0
	LDI 	POSITION_LOW, LOW(1500)
	LDI 	POSITION_HIGH, HIGH(1500)
	RCALL 	PCA9685_MOVE
	RET


ROLL_LEG:	;MUEVE PATA EN PLANO VERTICAL
	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_L
	RCALL 	PCA9685_MOVE	

	RET


YAW_LEG:	;MUEVE PATA EN PLANO HORIZONTAL
	RCALL	FIND_ANG_LEG
	MOV 	R23,LEG_H
	RCALL 	PCA9685_MOVE	
	
	RET

MOVE_FORWARD:	;CAMINA HACIA ADELANTE
	RCALL	MOVE_RIGHT_FORWARD
	RCALL	MOVE_LEFT_FORWARD
	RET

MOVE_LEFT_FORWARD:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 1 ABAJO

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PARA 2 ARRIBA

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA

	RCALL	DELAY_100_MS	;ACHICAR DELAY

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_30)
	LDI		XH,HIGH(ANG_YAW_30)
	RCALL	YAW_LEG
	;PATA 4 A 30�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 150�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 3 A 90�

	RCALL DELAY_100_MS
	RET


MOVE_RIGHT_FORWARD:

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PARA 1 ABAJO
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO

	RCALL	DELAY_100_MS

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
;	RCALL 	DELAY_10_S		
	;PATA 3 A 45�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG
	;PATA 1 A 150�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 4 A 90�


	RCALL DELAY_100_MS
	RET

TURN_RIGHT:	;DOBLA A LA DERECHA
  TURN_RIGHT_MOVE_1:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 2 ARRIBA
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 1 ABAJO
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 135�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 45�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 3 A 90�

	RCALL DELAY_100_MS

  TURN_RIGHT_MOVE_2:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 2 ABAJO
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_60)
	LDI		XH,HIGH(ANG_ROLL_60)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ABAJO

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 135�

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG
	;PATA 1 A 180�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_30)
	LDI		XH,HIGH(ANG_YAW_30)
	RCALL	YAW_LEG
	;PATA 4 A 0�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 3 A 45�

	RCALL DELAY_100_MS

  TURN_RIGHT_MOVE_3:
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 2 ARRIBA
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 3 A 90�

	RCALL DELAY_100_MS

	RCALL	INITIAL_POSITION

	RET


TURN_LEFT:	;DOBLA A LA IZQUIERDA
  TURN_LEFT_MOVE_1:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 2 ABAJO
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 3 ABAJO
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 2 A 135�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 3 A 45�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_100_MS

  TURN_LEFT_MOVE_2:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_0)
	LDI		XH,HIGH(ANG_ROLL_0)
	RCALL	ROLL_LEG
	;PATA 1 ABAJO
	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 2 ARRIBA
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 3 ARRIBA
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ABAJO

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 135�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG
	;PATA 2 A 180�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_YAW_0)
	LDI		XH,HIGH(ANG_YAW_0)
	RCALL	YAW_LEG
	;PATA 3 A 0�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 45�

	RCALL DELAY_100_MS

  TURN_LEFT_MOVE_3:
	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 1 ARRIBA
	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	;PATA 4 ARRIBA

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG
	;PATA 1 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_YAW_45)
	LDI		XH,HIGH(ANG_YAW_45)
	RCALL	YAW_LEG
	;PATA 4 A 90�

	RCALL DELAY_100_MS

	RCALL	INITIAL_POSITION

	RET


PLAY_DEAD: ;SE HACE EL MUERTO
	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG
	LDI		XL,LOW(ANG_YAW_135)
	LDI		XH,HIGH(ANG_YAW_135)
	RCALL	YAW_LEG

	RCALL	MOVEMENT_DELAY

	RCALL	INITIAL_POSITION

	RET


STAND_GREET_LEFT: ;SALUDA

	LDI		LEG_L,LOW(LEG_1)
	LDI		LEG_H,HIGH(LEG_1)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_2)
	LDI		LEG_H,HIGH(LEG_2)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_150)
	LDI		XH,HIGH(ANG_YAW_150)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_3)
	LDI		LEG_H,HIGH(LEG_3)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
	RCALL	ROLL_LEG	
	;PATA 2 ARRIBA
	LDI		XL,LOW(ANG_YAW_60)
	LDI		XH,HIGH(ANG_YAW_60)
	RCALL	YAW_LEG	
	;PATA 2 A 90�

	LDI		LEG_L,LOW(LEG_4)
	LDI		LEG_H,HIGH(LEG_4)
	LDI		XL,LOW(ANG_ROLL_30)
	LDI		XH,HIGH(ANG_ROLL_30)
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

	LDI 	TEMP, 3
	
  LOOP_GREET_LEFT:
  	PUSH 	TEMP
	LDI		XL,LOW(ANG_YAW_180)
	LDI		XH,HIGH(ANG_YAW_180)
	RCALL	YAW_LEG	

	RCALL DELAY_100_MS

	LDI		XL,LOW(ANG_YAW_90)
	LDI		XH,HIGH(ANG_YAW_90)
	RCALL	YAW_LEG	

	RCALL 	DELAY_100_MS
	
	POP 	TEMP
	DEC 	TEMP

	BRNE LOOP_GREET_LEFT
	RCALL INITIAL_POSITION
	RET



MOVEMENT_DELAY:	;DELAY DE 1 SEGUNDO PARA UN CLOCK DE 16MHz
	PUSH	R26
	LDI		R26,4
LOOP_MOVEMENT_DELAY:
	RCALL	DELAY_100_MS
	DEC	R26
	brne LOOP_MOVEMENT_DELAY
	
	POP		R26
	RET
	
DELAY_100_MS:	;DELAY DE 100 MILISEGUNDOS PARA UN CLOCK DE 16MHz
	PUSH	R25
	LDI		R25,100
LOOP_DELAY_100MS:
	RCALL	DELAY_1_MS
	DEC	R25
	brne LOOP_DELAY_100MS
	
	POP		R25
	RET

DELAY_1_MS:	;DELAY DE 1 MILISEGUNDO PARA UN CLOCK DE 16MHz
	push r22
	push r24
	ldi r22, 100 ;
LOOP_DELAY_1:
	ldi R24, 160
LOOP_DELAY_2:
	DEC	R24
	brne LOOP_DELAY_2
	dec r22
	brne LOOP_DELAY_1

	pop r24
	pop r22
	ret
