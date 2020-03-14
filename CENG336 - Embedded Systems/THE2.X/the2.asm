list P=18F8722

#include <p18f8722.inc>
config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

phase   udata 0x21
phase

lasercol0 udata 0x22
lasercol0

lasercol1 udata 0x23
lasercol1
 
lasercol2 udata 0x24
lasercol2
 
lasercol3 udata 0x25
lasercol3
 
lasercol4 udata 0x26
lasercol4
 
lasercol5 udata 0x27
lasercol5
 
astroidcol0 udata 0x28
astroidcol0
 
astroidcol1 udata 0x29
astroidcol1
 
astroidcol2 udata 0x30
astroidcol2
 
astroidcol3 udata 0x31
astroidcol3
 
astroidcol4 udata 0x32
astroidcol4
 
astroidcol5 udata 0x33
astroidcol5
 
shiplocation udata 0x34
shiplocation
 
newastroid udata 0x35
newastroid
 
 
collisionNewAstroid udata 0x36
collisionNewAstroid
 
 
org     0x00
goto    main

org     0x08
goto    isr             ;go to interrupt service routine

init:
    ;Disable interrupts
    clrf    INTCON
    clrf    RCON

    ;Configure Output Ports
    clrf    TRISA
    clrf    TRISB
    clrf    TRISC
    clrf    TRISD
    clrf    TRISE
    clrf    TRISF
    movlw   h'0f'
    movwf   TRISG
    
    movlw   b'00111111'
    movwf   LATA
    movwf   LATB
    movwf   LATC
    movwf   LATD
    movwf   LATE
    movwf   LATF
    clrf    LATG
    
    
    

    
    goto loop:
   
main:
    goto init
loop:
    btfss PORTG, 0 ; is RG0 pressed
    goto loop
    goto start
    
mapupdate:
    ;reset
    goto createastroid;sets newastroid and w
    astroidcreated:
    ANDWF lasercol5,0
    TSTFSZ W ; no collision = 0 , collision = non-zero
    BSF collisionNewAstroid,0 ; flag shows a collision occured
    COMF astroidcol5,0
    ANDWF lasercol4,0
    MOVWF lasercol5
    ;move astroids tricky
    
    normal:
    COMF astroidcol4,0
    ANDWF lasercol3,0
    MOVWF lasercol4
    COMF lasercol3,0
    ANDWF astroidcol4,0
    MOVWF astroidcol3
    
    COMF astroidcol3,0
    ANDWF lasercol2,0
    MOVWF lasercol3
    COMF lasercol2,0
    ANDWF astroidcol3,0
    MOVWF astroidcol2
    
    COMF astroidcol2,0
    ANDWF lasercol1,0
    MOVWF lasercol2
    COMF lasercol1,0
    ANDWF astroidcol2,0
    MOVWF astroidcol1
    
    XORWF astroidcol1,1 ; astroidcol1 after collisions
    MOVFW shiplocation
    XORWF astroidcol1,0
    TSTFSZ WREG
    goto mapupdatecontinue
    goto startingwait
mapupdatecontinue:
    
    collisionAtEnd:
    
    
start:
    movlw   b'01001000' ; presclar not assigned
    movwf   T0CON	;Initialize Timer0
    movlw   d'6'
    movwf   TMR0	; TMR0 counts 250 clock cycles
    movlw   b'10100100'
    movwf   INTCON	;Enable interrupts
    
    bsf     T0CON, 7    ;Enable Timer0 by setting TMR0ON to 1

MOVLW b'00000111' ;  bitmask
ANDWF timer1seedlow,0  ;get 3 bits of seedlow
MOVWF valueNeedsModulo ;save it to register
RRNCF timer1seedlow,0  ;rotate right
RRNCF timer1seedhigh
RRNCF timer1seedlow
MOVLW b'00000000'
CPFSGT valueNeedsModulo
goto tmr0;
MOVLW b'00000001'
CPFSGT valueNeedsModulo
goto tmr1;
MOVLW b'00000010'
CPFSGT valueNeedsModulo
goto tmr2;
MOVLW b'00000011'
CPFSGT valueNeedsModulo
goto tmr3;
MOVLW b'00000100'
CPFSGT valueNeedsModulo
goto tmr4;
MOVLW b'00000101'
CPFSGT valueNeedsModulo
goto tmr5;
MOVLW b'00000110'
CPFSGT valueNeedsModulo
goto tmr0;
MOVLW b'00000111'
CPFSGT valueNeedsModulo
goto tmr1;
tmr0:
    MOVLW b'00000001'
    goto complementcheck
tmr1:
    MOVLW b'00000010'
    goto complementcheck
tmr2:
    MOVLW b'00000100'
    goto complementcheck
tmr3:
    MOVLW b'00001000'
    goto complementcheck
tmr4:
    MOVLW b'00010000'
    goto complementcheck
tmr5:
    MOVLW b'00100000'
    goto complementcheck
complementcheck:
    BTFSS tenthPhase
    COMF timer1seedhigh
    BTFSS tenthPhase
    COMF timer1seedlow
    BCF tenthPhase,0
    goto biryer
    



