LIST    P=18F8722

#INCLUDE <p18f8722.inc>
CONFIG OSC = HS PLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

v1 udata 0x26
i
v2 udata 0x27
save_wreg
v3 udata 0x28
switch_var
v4 udata 0x29
inc_var
v5 udata 0x30
ra4_pressed
 
org    0x00
goto main
    
init
    CLRF WREG
    
    clrf    PORTA   ;clear the content of PORTA
    clrf    LATA    ;clear the content of LATA
    clrf    TRISA   ;clear the content of TRISA
    movlw   b'00010000';RA4 is input
    movwf   TRISA
    
    clrf    PORTB   ;clear the content of PORTB
    clrf    LATB    ;clear the content of LATB
    clrf    TRISB   ;clear the content of TRISB
    
    clrf    PORTC   ;clear the content of PORTC
    clrf    LATC    ;clear the content of LATC
    clrf    TRISC   ;clear the content of TRISC
    
    clrf    PORTD   ;clear the content of PORTD
    clrf    LATD    ;clear the content of LATD
    clrf    TRISD   ;clear the content of TRISD
    
    clrf    PORTE   ;clear the content of PORTE
    clrf    LATE    ;clear the content of LATE
    clrf    TRISE   ;clear the content of TRISE
    clrf    ra4_pressed
    movlw   b'00011000';RE3-4 are inputs
    movwf   TRISE
    goto loop 
    
one_sec_delay
    MOVFF WREG, save_wreg
    lp_cnt3 equ 0x23
    lp_cnt4 equ 0x24
    lp_cnt5 equ 0x25

    movlw D'10'
    movwf lp_cnt3,A
    loop3 movlw D'200'
    movwf lp_cnt4,A
    loop4 movlw D'250'
    movwf lp_cnt5,A
    loop5 
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    decfsz lp_cnt5,F,A
    bra loop5
    decfsz lp_cnt4,F,A
    bra loop4
    decfsz lp_cnt3,F,A
    bra loop3
    MOVFF save_wreg, WREG 
    return
    
half_sec_delay
    MOVFF WREG, save_wreg
    lp_cnt0 equ 0x20
    lp_cnt1 equ 0x21
    lp_cnt2 equ 0x22
    movlw D'5'
    movwf lp_cnt0,A
    loop0 movlw D'200'
    movwf lp_cnt1,A
    loop1 movlw D'250'
    movwf lp_cnt2,A
    loop2 
    BTFSC ra4_pressed,0
    goto check_ra4_depressed
return_from_clear_ra4:
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    btfsc PORTA, 4
    BSF ra4_pressed , 0
    decfsz lp_cnt2,F,A
    bra loop2
    decfsz lp_cnt1,F,A
    bra loop1
    decfsz lp_cnt0,F,A
    bra loop0
    MOVFF save_wreg, WREG 
    return
two_sec_delay
    call one_sec_delay
    call one_sec_delay
    return
open_all_leds:
    MOVLW   b'00001111'
    MOVWF   LATA
    MOVWF   LATB
    MOVWF   LATC
    MOVWF   LATD
    goto l1
close_all_leds:
    MOVLW   b'00000000'
    MOVWF   LATA
    MOVWF   LATB
    MOVWF   LATC
    MOVWF   LATD
    goto l2
turn_on_A:
onA_0:    MOVLW b'00000001'
    MOVWF LATA
    MOVLW d'0'
    MOVWF switch_var
    call half_sec_delay

      
onA_1:    MOVLW b'00000011'
    MOVWF LATA
    MOVLW d'1'
    MOVWF switch_var
    call half_sec_delay
      
onA_2:    MOVLW b'00000111'
    MOVWF LATA
    MOVLW d'2'
    MOVWF switch_var
    call half_sec_delay

      
onA_3:    MOVLW b'00001111'
    MOVWF LATA
    MOVLW d'3'
    MOVWF switch_var
    call half_sec_delay
      
    goto turn_on_B
	
turn_on_B:
onB_0:    MOVLW d'1'
    MOVWF LATB
    MOVLW d'4'
    MOVWF switch_var
    call half_sec_delay
      
onB_1:    MOVLW b'00000011'
    MOVWF LATB
        MOVLW d'5'
    MOVWF switch_var
    call half_sec_delay
      
onB_2:    MOVLW b'00000111'
    MOVWF LATB
        MOVLW d'6'
    MOVWF switch_var
    call half_sec_delay

      
onB_3:    MOVLW b'00001111'
    MOVWF LATB
        MOVLW d'7'
    MOVWF switch_var
    call half_sec_delay
    goto turn_on_C
    
turn_on_C:
onC_0:    MOVLW b'00000001'
    MOVWF LATC
        MOVLW d'8'
    MOVWF switch_var
    call half_sec_delay
      
onC_1:    MOVLW b'00000011'
    MOVWF LATC
        MOVLW d'9'
    MOVWF switch_var
    call half_sec_delay

onC_2:    MOVLW b'00000111'
    MOVWF LATC
        MOVLW d'10'
    MOVWF switch_var
    call half_sec_delay
      
onC_3:    MOVLW b'00001111'
    MOVWF LATC
        MOVLW d'11'
    MOVWF switch_var
    call half_sec_delay
      
    goto turn_on_D
    
turn_on_D:
onD_0:    MOVLW b'00000001'
    MOVWF LATD
        MOVLW d'12'
    MOVWF switch_var
    call half_sec_delay
      
onD_1:    MOVLW b'00000011'
    MOVWF LATD
        MOVLW d'13'
    MOVWF switch_var
    call half_sec_delay

      
onD_2:    MOVLW b'00000111'
    MOVWF LATD
        MOVLW d'14'
    MOVWF switch_var
    call half_sec_delay

      
onD_3:    MOVLW b'00001111'
    MOVWF LATD
        MOVLW d'15'
    MOVWF switch_var
    call half_sec_delay

    goto wait_for_ra4
    
turn_off_D:
offD_3:    MOVLW b'00000111'
    MOVWF LATD
        MOVLW d'16'
    MOVWF switch_var
    call half_sec_delay

      
offD_2:    MOVLW b'00000011'
    MOVWF LATD
        MOVLW d'17'
    MOVWF switch_var
    call half_sec_delay

      
offD_1:    MOVLW b'00000001'
    MOVWF LATD
        MOVLW d'18'
    MOVWF switch_var
    call half_sec_delay

      
offD_0:    MOVLW b'00000000'
    MOVWF LATD
        MOVLW d'19'
    MOVWF switch_var
    call half_sec_delay

    goto turn_off_C
    
turn_off_C:
offC_3:    MOVLW b'00000111'
    MOVWF LATC
        MOVLW d'20'
    MOVWF switch_var
    call half_sec_delay

      
offC_2:    MOVLW b'00000011'
    MOVWF LATC
        MOVLW d'21'
    MOVWF switch_var
    call half_sec_delay

      
offC_1:    MOVLW b'00000001'
    MOVWF LATC
        MOVLW d'22'
    MOVWF switch_var
    call half_sec_delay

      
offC_0:    MOVLW b'00000000'
    MOVWF LATC
        MOVLW d'23'
    MOVWF switch_var
    call half_sec_delay

    goto turn_off_B
turn_off_B:
offB_3:    MOVLW b'00000111'
    MOVWF LATB
        MOVLW d'24'
    MOVWF switch_var
    call half_sec_delay

      
offB_2:    MOVLW b'00000011'
    MOVWF LATB
        MOVLW d'25'
    MOVWF switch_var
    call half_sec_delay

      
offB_1:    MOVLW b'00000001'
    MOVWF LATB
        MOVLW d'26'
    MOVWF switch_var
    call half_sec_delay

      
offB_0:    MOVLW b'00000000'
    MOVWF LATB
        MOVLW d'27'
    MOVWF switch_var
    call half_sec_delay

    goto turn_off_A
    
turn_off_A:
offA_3:    MOVLW b'00000111'
    MOVWF LATA
        MOVLW d'28'
    MOVWF switch_var
    call half_sec_delay

      
offA_2:    MOVLW b'00000011'
    MOVWF LATA
        MOVLW d'29'
    MOVWF switch_var
    call half_sec_delay

      
offA_1:    MOVLW b'00000001'
    MOVWF LATA
        MOVLW d'30'
    MOVWF switch_var
    call half_sec_delay

      
offA_0:    MOVLW b'00000000'
    MOVWF LATA
        MOVLW d'31'
    MOVWF switch_var
    call half_sec_delay
      
    goto wait_for_ra4
    
wait_for_ra4:
    BTFSC PORTA,4
    goto wait_for_ra4_release
    goto wait_for_ra4
    wait_for_ra4_release: BTFSS PORTA,4
			  goto table
			  goto wait_for_ra4_release
check_ra4_depressed:
    BTFSS PORTA,4
    goto clear_ra4_variables
    goto return_from_clear_ra4
clear_ra4_variables:
    BCF ra4_pressed ,0 
    goto table
table:
    MOVLW d'0'
    MOVWF inc_var
    MOVF switch_var ,WREG
     CPFSLT inc_var
    GOTO case0
    INCF inc_var
     CPFSLT inc_var 
    GOTO case1
    INCF inc_var
     CPFSLT inc_var 
    GOTO case2
    INCF inc_var
     CPFSLT inc_var
    GOTO case3
    INCF inc_var
     CPFSLT inc_var
    GOTO case4
    INCF inc_var
     CPFSLT inc_var
    GOTO case5
    INCF inc_var
     CPFSLT inc_var
    GOTO case6
    INCF inc_var
     CPFSLT inc_var
    GOTO case7
    INCF inc_var
     CPFSLT inc_var
    GOTO case8
    INCF inc_var
     CPFSLT inc_var
    GOTO case9
    INCF inc_var
     CPFSLT inc_var
    GOTO case10
    INCF inc_var
     CPFSLT inc_var
    GOTO case11
    INCF inc_var
     CPFSLT inc_var
    GOTO case12
    INCF inc_var
     CPFSLT inc_var
    GOTO case13
    INCF inc_var
     CPFSLT inc_var
    GOTO case14
    INCF inc_var
     CPFSLT inc_var
    GOTO case15
    INCF inc_var
     CPFSLT inc_var
    GOTO case16
    INCF inc_var
     CPFSLT inc_var
    GOTO case17
    INCF inc_var
     CPFSLT inc_var
    GOTO case18
    INCF inc_var
     CPFSLT inc_var
    GOTO case19
    INCF inc_var
     CPFSLT inc_var
    GOTO case20
    INCF inc_var
     CPFSLT inc_var
    GOTO case21
    INCF inc_var
     CPFSLT inc_var
    GOTO case22
    INCF inc_var
     CPFSLT inc_var
    GOTO case23
    INCF inc_var
     CPFSLT inc_var
    GOTO case24
    INCF inc_var
     CPFSLT inc_var
    GOTO case25
    INCF inc_var
     CPFSLT inc_var
    GOTO case26
    INCF inc_var
     CPFSLT inc_var
    GOTO case27
    INCF inc_var
     CPFSLT inc_var
    GOTO case28
    INCF inc_var
     CPFSLT inc_var
    GOTO case29
    INCF inc_var
     CPFSLT inc_var
    GOTO case30
    INCF inc_var
     CPFSLT inc_var
    GOTO case31
    goto default
    case0:
    BTFSC PORTE,3 
    goto case0on 
    BTFSC PORTE,4 
    goto case0off 
    goto case0
   case1:
    BTFSC PORTE,3 
    goto case1on 
    BTFSC PORTE,4 
    goto case1off 
    goto case1
   case2:
    BTFSC PORTE,3 
    goto case2on 
    BTFSC PORTE,4 
    goto case2off 
    goto case2
   case3:
    BTFSC PORTE,3 
    goto case3on 
    BTFSC PORTE,4 
    goto case3off 
    goto case3
   case4:
    BTFSC PORTE,3 
    goto case4on 
    BTFSC PORTE,4 
    goto case4off 
    goto case4
   case5:
    BTFSC PORTE,3 
    goto case5on 
    BTFSC PORTE,4 
    goto case5off 
    goto case5
   case6:
    BTFSC PORTE,3 
    goto case6on 
    BTFSC PORTE,4 
    goto case6off 
    goto case6
   case7:
    BTFSC PORTE,3 
    goto case7on 
    BTFSC PORTE,4 
    goto case7off 
    goto case7
   case8:
    BTFSC PORTE,3 
    goto case8on 
    BTFSC PORTE,4 
    goto case8off 
    goto case8
   case9:
    BTFSC PORTE,3 
    goto case9on 
    BTFSC PORTE,4 
    goto case9off 
    goto case9
   case10:
    BTFSC PORTE,3 
    goto case10on 
    BTFSC PORTE,4 
    goto case10off 
    goto case10
   case11:
    BTFSC PORTE,3 
    goto case11on 
    BTFSC PORTE,4 
    goto case11off 
    goto case11
   case12:
    BTFSC PORTE,3 
    goto case12on 
    BTFSC PORTE,4 
    goto case12off 
    goto case12
   case13:
    BTFSC PORTE,3 
    goto case13on 
    BTFSC PORTE,4 
    goto case13off 
    goto case13
   case14:
    BTFSC PORTE,3 
    goto case14on 
    BTFSC PORTE,4 
    goto case14off 
    goto case14
    case15:
    BTFSC PORTE,4 
    goto case15on 
    goto case15
   case16:
    BTFSC PORTE,3 
    goto case16on 
    BTFSC PORTE,4 
    goto case16off 
    goto case16
   case17:
    BTFSC PORTE,3 
    goto case17on 
    BTFSC PORTE,4 
    goto case17off 
    goto case17
   case18:
    BTFSC PORTE,3 
    goto case18on 
    BTFSC PORTE,4 
    goto case18off 
    goto case18
   case19:
    BTFSC PORTE,3 
    goto case19on 
    BTFSC PORTE,4 
    goto case19off 
    goto case19
   case20:
    BTFSC PORTE,3 
    goto case20on 
    BTFSC PORTE,4 
    goto case20off 
    goto case20
   case21:
    BTFSC PORTE,3 
    goto case21on 
    BTFSC PORTE,4 
    goto case21off 
    goto case21
   case22:
    BTFSC PORTE,3 
    goto case22on 
    BTFSC PORTE,4 
    goto case22off 
    goto case22
   case23:
    BTFSC PORTE,3 
    goto case23on 
    BTFSC PORTE,4 
    goto case23off 
    goto case23
   case24:
    BTFSC PORTE,3 
    goto case24on 
    BTFSC PORTE,4 
    goto case24off 
    goto case24
   case25:
    BTFSC PORTE,3 
    goto case25on 
    BTFSC PORTE,4 
    goto case25off 
    goto case25
   case26:
    BTFSC PORTE,3 
    goto case26on 
    BTFSC PORTE,4 
    goto case26off 
    goto case26
   case27:
    BTFSC PORTE,3 
    goto case27on 
    BTFSC PORTE,4 
    goto case27off 
    goto case27
   case28:
    BTFSC PORTE,3 
    goto case28on 
    BTFSC PORTE,4 
    goto case28off 
    goto case28
   case29:
    BTFSC PORTE,3 
    goto case29on 
    BTFSC PORTE,4 
    goto case29off 
    goto case29
   case30:
    BTFSC PORTE,3 
    goto case30on 
    BTFSC PORTE,4 
    goto case30off 
    goto case30
   case31:
    BTFSC PORTE,3 
    goto case31on 
    goto case31
    default:
	goto loop
    return
    
    case0on:
	BTFSC PORTE,3
	goto case0on
	goto onA_1
    case0off:
	BTFSC PORTE,4
	goto case0off
	goto offA_0
    case1on:
	BTFSC PORTE,3
	goto case1on
	goto onA_2
    case1off:
	BTFSC PORTE,4
	goto case1off
	goto offA_1
    case2on:
	BTFSC PORTE,3
	goto case2on
	goto onA_3
case2off:
	BTFSC PORTE,4
	goto case2off
	goto offA_2

    case3on:
	BTFSC PORTE,3
	goto case3on
	goto onB_0
	case3off:
	BTFSC PORTE,4
	goto case3off
	goto offA_3
    case4on:
	BTFSC PORTE,3
	goto case4on
	goto onB_1
	case4off:
	BTFSC PORTE,4
	goto case4off
	goto offB_0
    case5on:
	BTFSC PORTE,3
	goto case5on
	goto onB_2
	case5off:
	BTFSC PORTE,4
	goto case5off
	goto offB_1
	goto case5
case6on:
	BTFSC PORTE,3
	goto case6on
	goto onB_3
	case6off:
	BTFSC PORTE,4
	goto case6off
	goto offB_2
    case7on:
	BTFSC PORTE,3
	goto case7on
	goto onC_0
	case7off:
	BTFSC PORTE,4
	goto case7off
	goto offB_3
    case8on:
	BTFSC PORTE,3
	goto case8on
	goto onC_1
	case8off:
	BTFSC PORTE,4
	goto case8off
	goto offC_0
    case9on:
	BTFSC PORTE,3
	goto case9on
	goto onC_2
	case9off:
	BTFSC PORTE,4
	goto case9off
	goto offC_1
    case10on:
	BTFSC PORTE,3
	goto case10on
	goto onC_3
	case10off:
	BTFSC PORTE,4
	goto case10off
	goto offC_2
    case11on:
	BTFSC PORTE,3
	goto case11on
	goto onD_0
	case11off:
	BTFSC PORTE,4
	goto case11off
	goto offC_3
case12on:
	BTFSC PORTE,3
	goto case12on
	goto onD_1
	case12off:
	BTFSC PORTE,4
	goto case12off
	goto offD_0
    case13on:
	BTFSC PORTE,3
	goto case13on
	goto onD_2
	case13off:
	BTFSC PORTE,4
	goto case13off
	goto offD_1
    case14on:
	BTFSC PORTE,3
	goto case14on
	goto onD_3
	case14off:
	BTFSC PORTE,4
	goto case14off
	goto offD_2
    case15on:
	BTFSC PORTE,4
	goto case15on
	goto offD_3
  case16on:
	BTFSC PORTE,3
	goto case16on
	goto onD_3
	case16off:
	BTFSC PORTE,4
	goto case16off
	goto offD_2 
    case17on:
	BTFSC PORTE,3
	goto case17on
	goto onD_2
	case17off:
	BTFSC PORTE,4
	goto case17off
	goto offD_1 
    case18on:
	BTFSC PORTE,3
	goto case18on
	goto onD_1
	case18off:
	BTFSC PORTE,4
	goto case18off
	goto offD_0 
    case19on:
	BTFSC PORTE,3
	goto case19on
	goto onD_0
	case19off:
	BTFSC PORTE,4
	goto case19off
	goto offC_3 
    case20on:
	BTFSC PORTE,3
	goto case20on
	goto onC_3 
	case20off:
	BTFSC PORTE,4
	goto case20off
	goto offC_2
    case21on:
	BTFSC PORTE,3
	goto case21on
	goto onC_2
case21off:
	BTFSC PORTE,4
	goto case21off
	goto offC_1 
    case22on:
	BTFSC PORTE,3
	goto case22on
	goto onC_1
	case22off:
	BTFSC PORTE,4
	goto case22off
	goto offC_0 
    case23on:
	BTFSC PORTE,3
	goto case23on
	goto onC_0
	case23off:
	BTFSC PORTE,4
	goto case23off
	goto offB_3 
    case24on:
	BTFSC PORTE,3
	goto case24on
	goto onB_3
	case24off:
	BTFSC PORTE,4
	goto case24off
	goto offB_2 
    case25on:
	BTFSC PORTE,3
	goto case25on
	goto onB_2
	case25off:
	BTFSC PORTE,4
	goto case25off
	goto offB_1 
    case26on:
	BTFSC PORTE,3
	goto case26on
	goto onB_1
	case26off:
	BTFSC PORTE,4
	goto case26off
	goto offB_0 
    case27on:
	BTFSC PORTE,3
	goto case27on
	goto onB_0
	case27off:
	BTFSC PORTE,4
	goto case27off
	goto offA_3 
    case28on:
	BTFSC PORTE,3
	goto case28on
	goto onA_3
	case28off:
	BTFSC PORTE,4
	goto case28off
	goto offA_2 
    case29on:
	BTFSC PORTE,3
	goto case29on
	goto onA_2 
	case29off:
	BTFSC PORTE,4
	goto case29off
	goto offA_1
    case30on:
	BTFSC PORTE,3
	goto case30on
	goto onA_1
	case30off:
	BTFSC PORTE,4
	goto case30off
	goto offA_0 
    case31on:
	BTFSC PORTE,3
	goto case31on
	goto onA_0
main:
    goto init
loop:
    goto open_all_leds
    l1:call two_sec_delay
    goto close_all_leds
    l2:call one_sec_delay
    goto turn_on_A
    goto wait_for_ra4
    goto loop
end