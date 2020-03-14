#include "p18f8722.inc"
; CONFIG1H
  CONFIG  OSC = HSPLL, FCMEN = OFF, IESO = OFF
; CONFIG2L
  CONFIG  PWRT = OFF, BOREN = OFF, BORV = 3
; CONFIG2H
  CONFIG  WDT = OFF, WDTPS = 32768
; CONFIG3L
  CONFIG  MODE = MC, ADDRBW = ADDR20BIT, DATABW = DATA16BIT, WAIT = OFF
; CONFIG3H
  CONFIG  CCP2MX = PORTC, ECCPMX = PORTE, LPT1OSC = OFF, MCLRE = ON
; CONFIG4L
  CONFIG  STVREN = ON, LVP = OFF, BBSIZ = BB2K, XINST = OFF
; CONFIG5L
  CONFIG  CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CP4 = OFF, CP5 = OFF
  CONFIG  CP6 = OFF, CP7 = OFF
; CONFIG5H
  CONFIG  CPB = OFF, CPD = OFF
; CONFIG6L
  CONFIG  WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF, WRT4 = OFF
  CONFIG  WRT5 = OFF, WRT6 = OFF, WRT7 = OFF
; CONFIG6H
  CONFIG  WRTC = OFF, WRTB = OFF, WRTD = OFF
; CONFIG7L
  CONFIG  EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF, EBTR4 = OFF
  CONFIG  EBTR5 = OFF, EBTR6 = OFF, EBTR7 = OFF
; CONFIG7H
  CONFIG  EBTRB = OFF

;*******************************************************************************
; Variables & Constants
;*******************************************************************************
UDATA_ACS
  t1	res 1	; used in delay
  t2	res 1	; used in delay
  t3	res 1	; used in delay
  state res 1	; controlled by RB0 button
;*******************************************************************************
; Reset Vector
;*******************************************************************************

RES_VECT  CODE    0x0000            ; processor reset vector
    GOTO    START                   ; go to beginning of program

;*******************************************************************************
; MAIN PROGRAM
;*******************************************************************************

MAIN_PROG CODE	; let linker place main program

 
START
    call INIT	; initialize variables and ports
    call DELAY	; wait a second

    
MAIN_LOOP        
    CLRF LATC	; turn off all portC leds
    CLRF LATF	; turn off all portF leds
    
    MOVLW 0x00	; prepare WREG before table lookup
    CALL  TABLE	; 0's bit settings for 7-seg. disp. returned into WREG
    MOVWF LATJ	; apply correct bit settings to portJ (7-seg. disp.)
    
    call DELAY	; wait a second
  
    CLRF WREG	; check whether state is 0x00 or 0xFF
    CPFSEQ state
    GOTO _stateF
    _state0:
	MOVLW   0xFF
	MOVWF   LATC	; turn on all portC leds
	MOVWF   LATF	; turn on all portF leds
	
	MOVLW   0x05	; prepare WREG before table lookup
	CALL    TABLE	; 5's bit settings for 7-seg. disp. returned into WREG
	MOVWF   LATJ	; apply correct bit settings to portJ (7-seg. disp.)
	GOTO	_state_end
    _stateF:
	MOVLW   0x81
	MOVWF   LATC	; turn on first and last portC leds
	MOVWF   LATF	; turn on first and last portF leds
	
	MOVLW   0x09	; prepare WREG before table lookup
	CALL    TABLE	; 9's bit settings for 7-seg. disp. returned into WREG
	MOVWF   LATJ	; apply correct bit settings to portJ (7-seg. disp.)
    _state_end:
    
    call DELAY	; wait a second
    
    call BUTTON_TASK; check RB0 button press (portB)
    
    GOTO MAIN_LOOP  ; loop forever
    
    
DELAY	; Time Delay Routine with 3 nested loops
    MOVLW 82	; Copy desired value to W
    MOVWF t3	; Copy W into t3
    _loop3:
	MOVLW 0xA0  ; Copy desired value to W
	MOVWF t2    ; Copy W into t2
	_loop2:
	    MOVLW 0x9F	; Copy desired value to W
	    MOVWF t1	; Copy W into t1
	    _loop1:
		decfsz t1,F ; Decrement t1. If 0 Skip next instruction
		GOTO _loop1 ; ELSE Keep counting down
		decfsz t2,F ; Decrement t2. If 0 Skip next instruction
		GOTO _loop2 ; ELSE Keep counting down
		decfsz t3,F ; Decrement t3. If 0 Skip next instruction
		GOTO _loop3 ; ELSE Keep counting down
		return

		
INIT
    CLRF    state		
    MOVLW   0xFF
    CLRF    LATB
    MOVWF   LATC
    MOVWF   LATD
    MOVWF   LATE
    MOVWF   LATF
    CLRF    LATG
    MOVWF   LATH    ; selects display permutation
    MOVWF   LATJ    ; on/off segment in a display
    MOVLW   0xFF
    MOVWF   TRISB
    CLRF    TRISC
    CLRF    TRISD
    CLRF    TRISE
    CLRF    TRISF
    CLRF    TRISG
    CLRF    TRISH
    CLRF    TRISJ
    return
    
    
BUTTON_TASK ; very primitive button task
    BTFSS PORTB,0
    return
    _debounce:
	BTFSC PORTB,0
	goto _debounce	; busy waiting. FIXME !!!
	COMF state, 1
	return
    
	
TABLE
    MOVF    PCL, F  ; A simple read of PCL will update PCLATH, PCLATU
    RLNCF   WREG, W ; multiply index X2
    ADDWF   PCL, F  ; modify program counter
    RETLW b'00111111' ;0 representation in 7-seg. disp. portJ
    RETLW b'00000110' ;1 representation in 7-seg. disp. portJ
    RETLW b'01011011' ;2 representation in 7-seg. disp. portJ
    RETLW b'01001111' ;3 representation in 7-seg. disp. portJ
    RETLW b'01100110' ;4 representation in 7-seg. disp. portJ
    RETLW b'01101101' ;5 representation in 7-seg. disp. portJ
    RETLW b'01111101' ;6 representation in 7-seg. disp. portJ
    RETLW b'00000111' ;7 representation in 7-seg. disp. portJ
    RETLW b'01111111' ;8 representation in 7-seg. disp. portJ
    RETLW b'01100111' ;9 representation in 7-seg. disp. portJ

    
END
