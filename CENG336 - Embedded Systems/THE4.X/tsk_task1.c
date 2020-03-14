#include "common.h"
#include "defines.h"


/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
void updateMap(void);

unsigned char fromNumberToAscii(unsigned char number)
{
    return (unsigned char)('0'+number);
}


/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/


char sensorBuffer [8];
char sensorReadSize = 0;

char stopCommand[4] = "$S:";
char mapCommand[7];
//volatile unsigned char backtrack [64];

/**********************************************************************
 * ------------------------------ TASK1 -------------------------------
 * 
 * Send stop command and wait for response, upon response send map info back
 * 
 **********************************************************************/
TASK(TASK1) 
{
    PIE1bits.RC1IE = 1;
    PORTB;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    TRISB = 0x10;
    
    RCONbits.IPEN = 1;
    INTCON2bits.RBIP = 0;
    
    while(TXSTA1bits.TXEN == 1){};
    transmitBuffer_push(stopCommand,3);
    LATAbits.LATA1 = 1;
    mapCommand[0]='$';
    mapCommand[1]='M';
    mapCommand[5]=':';
    mapCommand[6]='\0';
    SetRelAlarm(ALARM_TSK1, 40, 40);
	while(1) {
        
        sensorReadSize = receiveBuffer_pop(sensorBuffer);
        LATAbits.LATA2 = 1;
        //TODO size check!
        if(sensorReadSize!= 0 && sensorBuffer[0]=='D')
        {
        LATAbits.LATA3 = 1;
            switch(robotDirection)
            {
                case UP:
                {
                    if(yGridBox != 0){//FRONT{
                        if(maze[yGridBox-1][xGridBox]=='X')
                            unseenBoxes--;
                        maze[yGridBox-1][xGridBox]= (sensorBuffer[1] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox;
                        mapCommand[2]=yGridBox-1;
                        mapCommand[4]=sensorBuffer[1];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(xGridBox != 15){//RIGHT
                        if(maze[yGridBox][xGridBox+1]=='X')
                            unseenBoxes--;
                        maze[yGridBox][xGridBox+1]= (sensorBuffer[2] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox+1;
                        mapCommand[2]=yGridBox;
                        mapCommand[4]=sensorBuffer[2];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 3){//BACK
                        if(maze[yGridBox+1][xGridBox]=='X')
                            unseenBoxes--;
                        maze[yGridBox+1][xGridBox]= (sensorBuffer[3] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox;
                        mapCommand[2]=yGridBox+1;
                        mapCommand[4]=sensorBuffer[3];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }   
                    if(xGridBox != 0){//LEFT
                        if(maze[yGridBox][xGridBox-1]=='X')
                            unseenBoxes--;
                        maze[yGridBox][xGridBox-1]= (sensorBuffer[4] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox-1;
                        mapCommand[2]=yGridBox;
                        mapCommand[4]=sensorBuffer[4];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    break;
                }
                case RIGHT:
                {
                    if(xGridBox != 15){ //FRONT
                        if(maze[yGridBox][xGridBox+1]=='X')
                            unseenBoxes--;
                        maze[yGridBox][xGridBox+1]= (sensorBuffer[1] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox+1;
                        mapCommand[2]=yGridBox;
                        mapCommand[4]=sensorBuffer[1];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 3){ //RIGHT
                        if(maze[yGridBox+1][xGridBox]=='X')
                            unseenBoxes--;
                        maze[yGridBox+1][xGridBox]= (sensorBuffer[2] == 1) ? 0xFF: ' ';
                        mapCommand[3]=xGridBox;
                        mapCommand[2]=yGridBox+1;
                        mapCommand[4]=sensorBuffer[2];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(xGridBox != 0){ //BACK
                        if(maze[yGridBox][xGridBox-1]=='X')
                            unseenBoxes--;
                        maze[yGridBox][xGridBox-1]= (sensorBuffer[3] == 1) ? 0xFF: ' ';
                        mapCommand[3]=(xGridBox-1);
                        mapCommand[2]=(yGridBox);
                        mapCommand[4]=sensorBuffer[3];
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 0){ //LEFT
                        if(maze[yGridBox-1][xGridBox]=='X')
                            unseenBoxes--;
                        maze[yGridBox-1][xGridBox]= (sensorBuffer[4] == 1) ? 0xFF: ' ';
                        mapCommand[3]=(xGridBox);
                        mapCommand[2]=(yGridBox-1);
                        mapCommand[4]=sensorBuffer[4];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    break;
                }
                case DOWN:
                {

                    if(yGridBox != 3){ //FRONT
                        if(maze[yGridBox+1][xGridBox]=='X')
                            unseenBoxes--;
                        maze[yGridBox+1][xGridBox]= (sensorBuffer[1] == 1) ? 0xFF: ' ';
                        mapCommand[3]=(xGridBox);
                        mapCommand[2]=(yGridBox+1);
                        mapCommand[4]=sensorBuffer[1];
                        while(TXSTA1bits.TXEN == 1){};
                        transmitBuffer_push(mapCommand,6);
                    }
                    if(xGridBox != 0){ //RIGHT
                       if(maze[yGridBox][xGridBox-1]=='X')
                           unseenBoxes--;
                       maze[yGridBox][xGridBox-1]= (sensorBuffer[2] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox-1);
                       mapCommand[2]=(yGridBox);
                       mapCommand[4]=sensorBuffer[2];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 0){ //BACK
                       if(maze[yGridBox-1][xGridBox]=='X')
                           unseenBoxes--;
                       maze[yGridBox-1][xGridBox]= (sensorBuffer[3] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox);
                       mapCommand[2]=(yGridBox-1);
                       mapCommand[4]=sensorBuffer[3];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    if(xGridBox != 15){ //LEFT
                       if(maze[yGridBox][xGridBox+1]=='X')
                           unseenBoxes--;
                       maze[yGridBox][xGridBox+1]= (sensorBuffer[4] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox+1);
                       mapCommand[2]=(yGridBox);
                       mapCommand[4]=sensorBuffer[4];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    break;
                }
                case LEFT:
                {

                    if(xGridBox != 0){ //FRONT
                       if(maze[yGridBox][xGridBox-1]=='X')
                           unseenBoxes--;
                       maze[yGridBox][xGridBox-1]= (sensorBuffer[1] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox-1);
                       mapCommand[2]=(yGridBox);
                       mapCommand[4]=sensorBuffer[1];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 0){ //RIGHT
                       if(maze[yGridBox-1][xGridBox]=='X')
                           unseenBoxes--;
                       maze[yGridBox-1][xGridBox]= (sensorBuffer[2] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox);
                       mapCommand[2]=(yGridBox-1);
                       mapCommand[4]=sensorBuffer[2];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    if(xGridBox != 15){ //BACK
                       if(maze[yGridBox][xGridBox+1]=='X')
                           unseenBoxes--;
                       maze[yGridBox][xGridBox+1]= (sensorBuffer[3] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox+1);
                       mapCommand[2]=(yGridBox);
                       mapCommand[4]=sensorBuffer[3];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    if(yGridBox != 3){ //LEFT
                       if(maze[yGridBox+1][xGridBox]=='X')
                           unseenBoxes--;
                       maze[yGridBox+1][xGridBox]= (sensorBuffer[4] == 1) ? 0xFF: ' ';
                       mapCommand[3]=(xGridBox);
                       mapCommand[2]=(yGridBox+1);
                       mapCommand[4]=sensorBuffer[4];
                       while(TXSTA1bits.TXEN == 1){};
                       transmitBuffer_push(mapCommand,6);
                    }
                    break;
                }
                default:
                {
                    LATFbits.LATF4 = 1;
                    break;
                }
            }
            //updateMap();
            SetEvent(TASK0_ID,STOP_SENSOR_EVENT);
            TerminateTask();
            break;
        }
        WaitEvent(ALARM_EVENT);
        ClearEvent(ALARM_EVENT);
	}
	TerminateTask();
}


/* End of File : tsk_task1.c */