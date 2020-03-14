#INCLUDE <p18f8722.inc>
CONFIG OSC = HS, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF




 L1  udata 0X20
 L1

 L2  udata 0X21
 L2

 flag1 udata 0X22
 flag1

 flag2 udata 0X23
 flag2

 flag3 udata 0X24
 flag3

 flag4 udata 0X25
 flag4

 flag5 udata 0X26
 flag5

 flag6 udata 0X27
 flag6

 flag7 udata 0X28
 flag7

 flag8 udata 0X29
 flag8

level udata 0X30
level

head udata 0X31
head

head2 udata 0X32
head2

head3 udata 0X33
head3

kick udata 0X34
kick

 org    0

goto init

DELAY                            ; Time Delay Routines
     MOVLW d'244'                        ; Copy 50 to W
     MOVWF L2                    ; Copy W into L2
LOOP2
     MOVLW d'252'                   ; Copy 255 into W
     MOVWF L1                    ; Copy W into L1
     
LOOP1
     btfsc PORTA, 4
     setf  kick,0
     decfsz L1,F                    ; Decrement L1. If 0 Skip next instruction
         GOTO LOOP1                ; ELSE Keep counting down
     decfsz L2,F                    ; Decrement L2. If 0 Skip next instruction
         GOTO LOOP2                ; ELSE Keep counting down
     return


mydelay

    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY



        return

init
   

    clrf    LATA    ;clear the content of LATA
    clrf    TRISA   ;clear the content of TRISA
    movlw   b'00010000'
    movwf   TRISA

    clrf    LATB
    clrf    TRISB
    movlw   b'00100000'
    movwf   TRISB

    clrf    LATC
    clrf    TRISC

    clrf    LATD
    clrf    TRISD

    movlw   b'00001111'
    movwf   LATA

    movlw   b'00001111'
    movwf   LATB

    movlw   b'00001111'
    movwf   LATC

    movlw   b'00001111'
    movwf   LATD

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag3
    movwf flag4
    movwf flag5
    movwf flag6
    movwf flag7
    movwf flag8
    movwf level
    clrf head
    clrf head2
    clrf head3
    clrf kick

    
    call mydelay
    call mydelay
    call mydelay
    call mydelay
    call mydelay
    call DELAY
    call DELAY
    call DELAY
    call DELAY
    call DELAY

    goto pin11

RA4direction:
 clrf kick
 btfsc PORTA,4
 goto RA4direction
 btfsc level,0
 goto RA41 ; level 0sa RA42 ye gider
 goto RA42


RA41:

btfsc head2,4
goto pin1C
btfsc head2,5
goto pin1B
btfsc head2,6
goto pin1A
btfsc head2,7
goto pin19

btfsc head3,0
goto pin18
btfsc head3,1
goto pin17
btfsc head3,2
goto pin16
btfsc head3,3
goto pin15

btfsc head3,4
goto pin14
btfsc head3,5
goto pin13
btfsc head3,6
goto pin12
btfsc head3,7
goto pin11


RA42:
btfsc head,0
goto pin2C
btfsc head,1
goto pin2B
btfsc head,2
goto pin2A
btfsc head,3
goto pin29
btfsc head,4
goto pin28
btfsc head,5
goto pin27
btfsc head,6
goto pin26
btfsc head,7
goto pin25
btfsc head2,0
goto pin24
btfsc head2,1
goto pin23
btfsc head2,2
goto pin22
btfsc head2,3
goto pin21





pin11:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag2
    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag1

    movlw d'0'
    movwf level
    movlw   b'00000001'
    movwf   LATA
    movlw   b'00000001'
    movwf   LATB
    clrf    LATC
    clrf    LATD

    movlw   b'00000001'
    movwf   head
    clrf  head2
    clrf  head3

    call mydelay
    call mydelay
           

pin12:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag2
    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag1

    movlw d'0'
    movwf level
    movlw   b'00000001'
    movwf   LATB
    movlw   b'00000010'
    movwf   head
    movlw   b'00000001'
    movwf   LATC
    clrf    LATA
    clrf    LATD
    clrf  head2
    clrf  head3

    call mydelay
    call mydelay

pin13:
   movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag2
    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag1

    movlw d'0'
    movwf level
    movlw   b'00000001'
    movwf   LATC
    movlw   b'00000100'
    movwf   head
    movlw   b'00000001'
    movwf   LATD
    clrf    LATA
    clrf    LATB
    clrf  head2
    clrf  head3

    goto stay



pin14:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag2

    movlw d'0'
    movwf level
    movlw   b'00000011'
    movwf   LATD
    movlw   b'00001000'
    movwf   head
    clrf    LATC
    clrf    LATA
    clrf    LATB
    clrf  head2
    clrf  head3

    call mydelay
    call mydelay

pin15:
   movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1


    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag2

    movlw d'0'
    movwf level
    movlw   b'00000110'
    movwf   LATD
    movlw   b'00010000'
    movwf   head
    clrf    LATA
    clrf    LATB
    clrf    LATC
    clrf  head2
    clrf  head3

    call mydelay
    call mydelay

pin16:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag3
    movwf flag4
    movlw d'1'
    movwf flag2

    movlw d'0'
    movwf level
    movlw   b'00001100'
    movwf   LATD
    movlw   b'00100000'
    movwf   head
    clrf    LATA
    clrf    LATB
    clrf    LATC
    clrf  head2
    clrf  head3

    goto stay



    

pin17:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag4
    movlw d'1'
    movwf flag3

    movlw d'0'
    movwf level
    movlw   b'00001000'
    movwf   LATD
    movlw   b'01000000'
    movwf   head
    movlw   b'00001000'
    movwf   LATC
    clrf    LATA
    clrf    LATB
    clrf  head2
    clrf  head3

    call mydelay
    call mydelay

pin18:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag4
    movlw d'1'
    movwf flag3

    clrf  head2
    clrf  head3
    movlw d'0'
    movwf level
    movlw   b'00001000'
    movwf   LATC
    movlw   b'10000000'
    movwf   head
    movlw   b'00001000'
    movwf   LATB
    clrf    LATA
    clrf    LATD

    call mydelay
    call mydelay

pin19:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag4
    movlw d'1'
    movwf flag3
    clrf  head
    clrf  head3
    movlw d'0'
    movwf level
    movlw   b'00001000'
    movwf   LATB
    movlw   b'00000001'
    movwf   head2
    movlw   b'00001000'
    movwf   LATA
    clrf    LATC
    clrf    LATD

  

    goto stay



pin1A:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag3
    movlw d'1'
    movwf flag4

    movlw d'0'
    movwf level
    movlw   b'00001100'
    movwf   LATA
    movlw   b'00000010'
    movwf   head2
    clrf    LATC
    clrf    LATD
    clrf    LATB

    clrf  head
    clrf  head3
    call mydelay
    call mydelay

pin1B:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag3
    movlw d'1'
    movwf flag4

    movlw d'0'
    movwf level
    movlw   b'00000110'
    movwf   LATA
    movlw   b'00000100'
    movwf   head2
    clrf    LATD
    clrf    LATB
    clrf    LATC

    clrf  head
    clrf  head3
    call mydelay
    call mydelay

pin1C:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag1
    movwf flag2
    movwf flag3
    movlw d'1'
    movwf flag4

    movlw d'0'
    movwf level
    movlw   b'00000011'
    movwf   LATA
    movlw   b'00001000'
    movwf   head2
    clrf    LATD
    clrf    LATB
    clrf    LATC
    clrf  head
    clrf  head3

    goto stay


stay:
     btfsc kick,0
 goto  RA4direction


     btfsc PORTB,5
     goto button
     call DELAY
     goto stay




button:

btfsc PORTB,5
goto button
btfsc flag1,0
goto pin14
btfsc flag2,0
goto pin17
btfsc flag3,0
goto pin1A
btfsc flag4,0
goto pin11







    
pin21:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag6
    movwf flag8
    movwf flag7
    movlw d'1'
    movwf flag5

    movlw d'1'
    movwf level
    movlw   b'00000011'
    movwf   LATA
    movlw   b'00010000'
    movwf   head2
    clrf    LATC
    clrf    LATD
    clrf    LATB
    clrf  head
    clrf  head3

    call DELAY
    call mydelay
    
  


pin22:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag6
    movwf flag8
    movwf flag7
    movlw d'1'
    movwf flag5

    movlw d'1'
    movwf level
    movlw   b'00000110'
    movwf   LATA
    movlw   b'00100000'
    movwf   head2
    clrf    LATD
    clrf    LATB
    clrf    LATC
    clrf  head
    clrf  head3
    call DELAY
    call mydelay

pin23:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag6
    movwf flag8
    movwf flag7
    movlw d'1'
    movwf flag5

    movlw d'1'
    movwf level
    movlw   b'00001100'
    movwf   LATA
    movlw   b'01000000'
    movwf   head2
    clrf    LATD
    clrf    LATB
    clrf    LATC
    clrf  head
    clrf  head3
      goto stay2



pin24:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag5
    movwf flag7
    movwf flag8
    movlw d'1'
    movwf flag6

    movlw d'1'
    movwf level
    movlw   b'00001000'
    movwf   LATB
    movlw   b'00001000'
    movwf   LATA
    movlw   b'10000000'
    movwf   head2
    clrf    LATC
    clrf    LATD
    clrf  head
    clrf  head3
    call DELAY
    call mydelay

pin25:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    
    movlw d'0'
    movwf flag5
    movwf flag7
    movwf flag8
    movlw d'1'
    movwf flag6

    movlw d'1'
    movwf level
    movlw   b'00001000'
    movwf   LATC
    movlw   b'00001000'
    movwf   LATB
    movlw   b'00000001'
    movwf   head3
    clrf    head
    clrf  head2

    clrf    LATA
    clrf    LATD

    call DELAY
    call mydelay

pin26:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag7
    movwf flag8
    movlw d'1'
    movwf flag6

    movlw d'1'
    movwf level
    movlw   b'00001000'
    movwf   LATD
    movlw   b'00001000'
    movwf   LATC
    movlw   b'00000010'
    movwf   head3
    clrf    LATA
    clrf    LATB
    clrf    head
    clrf  head2

    goto stay2


    

pin27:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction

    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag8
    movlw d'1'
    movwf flag7

    movlw d'1'
    movwf level
    movlw   b'00001100'
    movwf   LATD
    movlw   b'00000100'
    movwf   head3
    clrf    LATA
    clrf    LATC
    clrf    LATB
    clrf    head
    clrf  head2

    call DELAY
    call mydelay


pin28:
   movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag8
    movlw d'1'
    movwf flag7

    movlw d'1'
    movwf level
    movlw   b'00000110'
    movwf   LATD
    movlw   b'00001000'
    movwf   head3
    clrf    LATA
    clrf    LATB
    clrf    LATC
    clrf    head
    clrf  head2

    call DELAY
    call mydelay

pin29:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag8
    movlw d'1'
    movwf flag7

    movlw d'1'
    movwf level
    movlw   b'00000011'
    movwf   LATD
    movlw   b'00010000'
    movwf   head3
    clrf    LATA
    clrf    LATB
    clrf    LATC
    clrf    head
    clrf  head2

    goto stay2


pin2A:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag7
    movlw d'1'
    movwf flag8

    movlw d'1'
    movwf level
    movlw   b'00000001'
    movwf   LATC
    movlw   b'00000001'
    movwf   LATD
    movlw   b'00100000'
    movwf   head3
    clrf    LATA
    clrf    LATB
    clrf    head
    clrf  head2

    call DELAY
    call mydelay

pin2B:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag7
    movlw d'1'
    movwf flag8

    movlw d'1'
    movwf level
    movlw   b'00000001'
    movwf   LATB
    movlw   b'00000001'
    movwf   LATC
    movlw   b'01000000'
    movwf   head3
    clrf    LATA
    clrf    LATD
    clrf    head
    clrf  head2

    call DELAY
    call mydelay

pin2C:
    movlw b'00000000'
    addwf PORTA, 0
    XORWF kick,1
    btfsc kick,4
    goto  RA4direction


    movlw d'0'
    movwf flag5
    movwf flag6
    movwf flag7
    movlw d'1'
    movwf flag8

    movlw d'1'
    movwf level
    movlw   b'00000001'
    movwf   LATA
    movlw   b'00000001'
    movwf   LATB
    movlw   b'10000000'
    movwf   head3
    clrf    LATC
    clrf    LATD
    clrf    head
    clrf  head2

    goto stay2


stay2:
 btfsc kick,0
 goto  RA4direction

 btfsc PORTB,5
 goto button2
 call DELAY
 goto stay2


button2:

btfsc PORTB,5
goto button2
btfsc flag5,0
goto pin24
btfsc flag6,0
goto pin27
btfsc flag7,0
goto pin2A
btfsc flag8,0
goto pin21








end



