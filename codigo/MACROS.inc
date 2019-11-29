.macro STACK_INIT	; setup stack
	ldi TEMP, HIGH(RAMEND)
	out SPH, TEMP
	ldi TEMP, LOW(RAMEND)
	out SPL, TEMP
.endmacro

.macro stsi						;CARGA UN NUMERO EN UN REGISTRO
	ldi TEMP, @1
	sts	@0, TEMP
.endmacro

.macro stsi16					;CARGA UN NUMERO DE 16 BIT EN 2 REGISTROS 
	ldi TEMP, HIGH(@1)
	sts	@0H, TEMP
	ldi TEMP, LOW(@1)
	sts @0L, TEMP
.endmacro

.macro PORTB_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi TEMP, @0
	out DDRB, TEMP ; seteo la/s patita/s que me pidieron
	clr TEMP
	out PORTB, TEMP ; inicializo en 0 el puerto B	
.endmacro

.macro PORTC_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi TEMP, @0
	out DDRC, TEMP ; seteo la/s patita/s que me pidieron
	clr TEMP
	out PORTC, TEMP ; inicializo en 0 el puerto C	
.endmacro

.macro PORTD_AS_OUTPUT ; @0 es la/s patita/s que quiero configurar (mandame 0bxxxxxxxx y x=1 es patita para salida)
	ldi TEMP, @0
	out DDRD, TEMP ; seteo la/s patita/s que me pidieron
	clr TEMP
	out PORTD, TEMP ; inicializo en 0 el puerto D	
.endmacro