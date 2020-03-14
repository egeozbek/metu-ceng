#include <p18f8722.h>

#include "common.h"
#include "LCD.h"
#include "defines.h"

/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
char data[4];
unsigned char backtrack[32];
unsigned char backtrackEnabled = 0;
unsigned char backtrackIndex=0;
char data_size;
char endCommand[6]="$END:";
void updateMap(void);
unsigned char k=0;
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 *
 * Basic echo function test. Type $hello: in cutecom and receive hello
 *
 **********************************************************************/
TASK(TASK0) 
{   
    PIE1bits.RC1IE = 1;
    PORTB;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    TRISB = 0x10;
    
    RCONbits.IPEN = 1;
    INTCON2bits.RBIP = 0;
    
    SetRelAlarm(ALARM_TSK0, 40, 40);
	while(1) {
        
        data_size = receiveBuffer_pop(data);
        if(data_size != 0 && data[0]=='G' && data[1]=='O')
        {      
            LATAbits.LATA0 = 1;
            break;
        }
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
	}
    while(1) {
        
        while(1)
        {
            //sends a stop signal, gets sensory data about surroundings
            ActivateTask(TASK1_ID);
            WaitEvent(STOP_SENSOR_EVENT);
            ClearEvent(STOP_SENSOR_EVENT);
            
            ActivateTask(TASK2_ID);
            WaitEvent(MOVEMENT_EVENT);
            ClearEvent(MOVEMENT_EVENT);
            
            if(unseenBoxes == 0)
            {
                backtrackEnabled=1;
                if(xGridBox == 15 && yGridBox == 0)
                {
                    while(TXSTA1bits.TXEN == 1){};
                    transmitBuffer_push(endCommand,5);
                    LATFbits.LATF7 = 1;

                    TerminateTask();
                    break;
                }
            }
        }
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
	}
	TerminateTask();
}

/* End of File : tsk_task1.c */