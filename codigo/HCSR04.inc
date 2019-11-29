; PORTB
.equ ECHO_PIN		= 0
.equ TRIGGER_PIN	= 2

; CONSTANTES DEL TIMER
.EQU TRIGGER_PERIOD		= 0x3E80 ; 1 -> 4uS 
.EQU TRIGGER_UPTIME		= 0x03	; 3 son los 10uS del pulso que activa el trigger
.EQU THRESHOLD_DISTANCE = 145 ; 145 -prescaler-> 580uS (tiempo_threshold) -cuenta HCSR04-> 10cm 

.EQU 	CHECKING_OBSTACLE 	= 0
.EQU 	OBSTACLE_FOUND 		= 1
.EQU 	OBSTACLE_NOT_FOUND 	= 2


;***********************************************************************************************************************************************************************************************************
; INICIALIZA EL MODULO: EN OC1B PONE UN PWM CON DUTY CYCLE = 12uS, y PERIODO DE (TRIGGER_PERIOD * 4e-6) SEGUNDOS <- (VER PRESCALER) .
; TAMBIEN ACTIVA LA INTERRUPCION POR ICP1. EN DICHA INTERRUPCION SE COMPARA EL TIEMPO DEL TIMER CON EL TIEMPO DE ECHO.
; FINALMENTE ACTIVA INTERRUPCION POR OVERFLOW PARA REINICIAR EL TIMER.
;***********************************************************************************************************************************************************************************************************


HCSR04_SETUP:
	SBI 	DDRB,TRIGGER_PIN

	stsi16 TCNT1, 0xFFFF-TRIGGER_PERIOD ; OVERFLOW - K , K = duraci? de c/trigger.
	stsi16	OCR1A, 0xFFFF ; 2? cambio de estado y fin del periodo de trigger (TOP = MAX para que cambie de Low a High y arranque de nuevo el trigger)
	stsi16	OCR1B, (0xFFFF-TRIGGER_PERIOD)+TRIGGER_UPTIME ; 1? cambio de estado (de High a Low)

	; 1
	;----- x 64(prescaler) x K (numero que cargo) = Segundos "que pasan en c/clock" 		(BORRAR AL MERGEAR)
	; 16Mhz											* EL PRESCALER EST? PUESTO PARA PODER HACER ALGO DE APROX 64ms y 10uS
	;

	stsi	TCCR1A, (1<<COM1B1)|(1<<WGM11)|(1<<WGM10)						
	;Clear OC1B on compare match, FAST-PWM, OCR1A = TOP
	stsi	TCCR1B, (0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10)	
	;Cuando llegue a TOP arranca a contar de nuevo
	stsi	TIMSK1, (1<<ICIE1)|(1<<TOIE1)
	SEI
	ret


;***********************************************************************************************************************************************************************************************************
;INTERRUPCION POR OVERFLOW PARA RESTARTEAR EL TIMER
;***********************************************************************************************************************************************************************************************************


RESTART_TIMER:

	stsi16 TCNT1, 0xFFFF-TRIGGER_PERIOD
	RETI


;***********************************************************************************************************************************************************************************************************
;INTERRUPCION POR CAPTURA: EN UN FLANCO DESCENDENTE DE PB0 (ICP1), COPIA EL TCNT1 EN ICR1 Y 
;LUEGO SE COMPARA ESTE VALOR CON EL CORRESPONDIENTE DE TIEMPO SEGUN LA DISTANCIA UMBRAL SETEADA. 
;***********************************************************************************************************************************************************************************************************


ISR_ICP1:
	stsi	TIMSK1, (0<<ICIE1)|(0<<TOIE1)	;DESACTIVO INTERRUPCION
	lds		TIMEL, ICR1L			;LEO EL TIEMPO QUE PASO DESDE QUE EMPEZO EL CICLO HASTA QUE HUBO FLANCO DESCENDENTE DE ECHO
	lds		TIMEH, ICR1H
	
	subi TIMEL, LOW  ( (0xFFFF - TRIGGER_PERIOD) + TRIGGER_UPTIME)	;Le resto los 12uS correspondientes al trigger y el inicio de TCNT1
	sbci TIMEH, HIGH ( (0xFFFF - TRIGGER_PERIOD) + TRIGGER_UPTIME)	; ya que hice un timer por overflow (y el TCNT1 arranc?siendo grande)

	
	;PARA SETEAR UN UMBRAL DE 10cm, HAY QUE USAR LA FORMULA:
		
		;				 TIEMPO [s] * 344 m/s
		;DISTANCIA [m] = --------------------
		;						2

	;ENTONCES: TIEMPO_THRESHOLD = 581.4 us. 
	;	
	;			581.4us --cargo---> 145.35 REDONDEO 145 

	CLC ; clear-bit por si acaso
	LDI 	TEMP, LOW( 0x7E + THRESHOLD_DISTANCE) ; averiguar por que 0x7E?
	sub 	TIMEL, TEMP
	LDI 	TEMP, HIGH( 0x7E + THRESHOLD_DISTANCE)			
	sbc 	TIMEH, TEMP ; COMPARO TIME CONTRA MI UMBRAL (+ UNA CTE DE ?ERROR?)

	BRLT OBSTACLE_TRUE	; UMBRAL - TIME (si TIME>UMBRAL, se activa el bit de signo)
	RJMP OBSTACLE_FALSE ; Si no se activa, no hay ningun obstaculo (BORRAR AL MERGEAR, va directo a RETI)

OBSTACLE_TRUE:
	LDI 	TEMP, OBSTACLE_FOUND
	STS 	GPIOR1, TEMP
	RCALL 	TURN_ON
	RJMP 	ISR_ICP1_END

OBSTACLE_FALSE: ; (BORRAR AL MERGEAR)
	LDI 	TEMP, OBSTACLE_NOT_FOUND
	STS 	GPIOR1, TEMP
	RCALL 	TURN_OFF
	RJMP 	ISR_ICP1_END

ISR_ICP1_END:	
	RETI

HCSR04_CHECK:
	LDI 	TEMP, CHECKING_OBSTACLE
	STS 	GPIOR1, TEMP
	RCALL 	RESTART_TIMER
	stsi	TIMSK1, (1<<ICIE1)|(1<<TOIE1)	;ACTIVO INTERRUPCION
HCSR04_CHECK_LOOP:
	LDS		TEMP, GPIOR1	
	CPI 	TEMP, CHECKING_OBSTACLE
	BREQ	HCSR04_CHECK_LOOP
	RET