;;
; HC-SR04.asm
;
; Created: 1/6/2019 19:20:57
; Author : ivanlisman
;

.INCLUDE "m328pdef.inc"

; DEFINES
.def AUX            =   R16
.def OBSTACLE_FLAG	=	R17		;(BORRAR AL MERGEAR)
.def TIMEL		    =	R18     
.def TIMEH		    =	R19

; PORTB
.equ ECHO_PIN		= 0
.equ TRIGGER_PIN	= 2

; CONSTANTES DEL TIMER
.EQU TRIGGER_PERIOD		= 0x3E80 ; 1 -> 4uS 
.EQU TRIGGER_UPTIME		= 0x03	; 3 son los 10uS del pulso que activa el trigger
.EQU THRESHOLD_DISTANCE = 145 ; 145 -prescaler-> 580uS (tiempo_threshold) -cuenta HCSR04-> 10cm 

;***********************************************************************************************************************************************************************************************************
;	MACROS				PASAR LAS QUE SEAN NECESARIAS Y (BORRAR AL MERGEAR)
;***********************************************************************************************************************************************************************************************************

.macro STACK_INIT	; setup stack
	ldi AUX, HIGH(RAMEND)
	out SPH, AUX
	ldi AUX, LOW(RAMEND)
	out SPL, AUX
.endmacro

.macro stsi						;CARGA UN NUMERO EN UN REGISTRO
	ldi AUX, @1
	sts	@0, AUX
.endmacro

.macro stsi16					;CARGA UN NUMERO DE 16 BIT EN 2 REGISTROS 
	ldi AUX, HIGH(@1)
	sts	@0H, AUX
	ldi AUX, LOW(@1)
	sts @0L, AUX
.endmacro

.macro PORTB_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi AUX, @0
	out DDRB, AUX ; seteo la/s patita/s que me pidieron
	clr AUX
	out PORTB, AUX ; inicializo en 0 el puerto B	
.endmacro

.macro PORTC_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi AUX, @0
	out DDRC, AUX ; seteo la/s patita/s que me pidieron
	clr AUX
	out PORTC, AUX ; inicializo en 0 el puerto C	
.endmacro

.macro PORTD_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi AUX, @0
	out DDRD, AUX ; seteo la/s patita/s que me pidieron
	clr AUX
	out PORTD, AUX ; inicializo en 0 el puerto D	
.endmacro

;*********************************************************************************************************************************************************************************************************
;									MAIN-RESTART
;***********************************************************************************************************************************************************************************************************



;.ORG INT_VECTORS_SIZE

;MAIN:
;	STACK_INIT 
;	RCALL HC_SR04_INIT ; configuro el trigger y configuro el timer/wave generator
	SEI

KEEP: RJMP KEEP ;prueba


;***********************************************************************************************************************************************************************************************************
; INICIALIZA EL MODULO: EN OC1B PONE UN PWM CON DUTY CYCLE = 12uS, y PERIODO DE (TRIGGER_PERIOD * 4e-6) SEGUNDOS <- (VER PRESCALER) .
; TAMBIEN ACTIVA LA INTERRUPCION POR ICP1. EN DICHA INTERRUPCION SE COMPARA EL TIEMPO DEL TIMER CON EL TIEMPO DE ECHO.
; FINALMENTE ACTIVA INTERRUPCIÓN POR OVERFLOW PARA REINICIAR EL TIMER.
;***********************************************************************************************************************************************************************************************************


HC_SR04_SETUP:

	PORTB_AS_OUTPUT (1<<TRIGGER_PIN)|(1<<3)|(1<<4) ; 3 y 4 son test pins. (BORRAR AL MERGEAR)

	stsi16 TCNT1, 0xFFFF-TRIGGER_PERIOD ; OVERFLOW - K , K = duración de c/trigger.
	stsi16	OCR1A, 0xFFFF ; 2° cambio de estado y fin del periodo de trigger (TOP = MAX para que cambie de Low a High y arranque de nuevo el trigger)
	stsi16	OCR1B, (0xFFFF-TRIGGER_PERIOD)+TRIGGER_UPTIME ; 1° cambio de estado (de High a Low)

	; 1
	;----- x 64(prescaler) x K (numero que cargo) = Segundos "que pasan en c/clock" 		(BORRAR AL MERGEAR)
	; 16Mhz											* EL PRESCALER ESTÁ PUESTO PARA PODER HACER ALGO DE APROX 64ms y 10uS
	;

	stsi	TCCR1A, (1<<COM1B1)|(1<<WGM11)|(1<<WGM10)						
	;Clear OC1B on compare match, FAST-PWM, OCR1A = TOP
	stsi	TCCR1B, (0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10)	
	;Cuando llegue a TOP arranca a contar de nuevo
	stsi	TIMSK1, (1<<ICIE1)|(1<<TOIE1)

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

	CLI ; desactivo las otras interrupciones

	lds		TIMEL, ICR1L			;LEO EL TIEMPO QUE PASO DESDE QUE EMPEZO EL CICLO HASTA QUE HUBO FLANCO DESCENDENTE DE ECHO
	lds		TIMEH, ICR1H
	
	subi TIMEL, LOW  ( (0xFFFF - TRIGGER_PERIOD) + TRIGGER_UPTIME)	;Le resto los 12uS correspondientes al trigger y el inicio de TCNT1
	sbci TIMEH, HIGH ( (0xFFFF - TRIGGER_PERIOD) + TRIGGER_UPTIME)	; ya que hice un timer por overflow (y el TCNT1 arrancó siendo grande)

	
	;PARA SETEAR UN UMBRAL DE 10cm, HAY QUE USAR LA FORMULA:
		
		;				 TIEMPO [s] * 344 m/s
		;DISTANCIA [m] = --------------------
		;						2

	;ENTONCES: TIEMPO_THRESHOLD = 581.4 us. 
	;	
	;			581.4us --cargo---> 145.35 REDONDEO 145 


	; Inicializo los leds para probar
;	cbi		PORTB, 4; todo esto (BORRAR AL MERGEAR)
;	cbi		PORTB, 3
	
	CLC ; clear-bit por si acaso
	LDI AUX, LOW( 0x7E + THRESHOLD_DISTANCE) ; averiguar por que 0x7E?
	sub TIMEL, AUX
	LDI AUX, HIGH( 0x7E + THRESHOLD_DISTANCE)			
	sbc TIMEH, AUX ; COMPARO TIME CONTRA MI UMBRAL (+ UNA CTE DE ¿ERROR?)

	BRLT OBSTACLE_TRUE	; UMBRAL - TIME (si TIME>UMBRAL, se activa el bit de signo)
	RJMP OBSTACLE_FALSE ; Si no se activa, no hay ningun obstaculo (BORRAR AL MERGEAR, va directo a RETI)

OBSTACLE_TRUE:
	LDI OBSTACLE_FLAG, 1	; (BORRAR AL MERGEAR)
	SET
;	SBI PORTB, 4 ; Si hay un obstaculo, prender PB4 -> (BORRAR AL MERGEAR, poner directo lo que tenga que hacer con el motor)
	RJMP ISR_ICP1_END

OBSTACLE_FALSE: ; (BORRAR AL MERGEAR)
	CLR OBSTACLE_FLAG
	CLT
;	SBI PORTB, 3 ; Si no hay un obstaculo, prender PB3
	RJMP ISR_ICP1_END

ISR_ICP1_END:	
	SEI	; Activo nuevamente las interrupciones
	RETI

