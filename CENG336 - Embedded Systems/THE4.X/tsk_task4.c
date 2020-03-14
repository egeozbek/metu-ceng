#include "common.h"
#include "LCD.h"
#include "defines.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char endCommand[4];
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Type $hello: in cutecom and receive hello
 *
 **********************************************************************/
TASK(TASK4) 
{   
    PORTB;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    TRISB = 0x10;
    
    RCONbits.IPEN = 1;
    INTCON2bits.RBIP = 0;
    
    SetRelAlarm(ALARM_TSK4, 100, 50);
	while(1) {
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
        while(TXSTA1bits.TXEN == 1){};
        transmitBuffer_push(endCommand,3);
	}
	TerminateTask();
}

/* End of File : tsk_task1.c */