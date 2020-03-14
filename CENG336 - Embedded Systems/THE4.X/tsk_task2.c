#include "common.h"
#include "defines.h"


/**********************************************************************
 * ----------------------- GLOBAL VARIABLES ---------------------------
 **********************************************************************/
void updateMap(void);
char turnRightCommand[4] = "$R:";
char turnLeftCommand[4] = "$L:";
char moveForwardCommand[4] = "$F:";

unsigned char motionBufferSize = 0;
unsigned char motionEncoderBuffer [2];

unsigned int targetXCoordinate;
unsigned int targetYCoordinate;
unsigned char newDirection;



int isRightFree()
{
    switch(robotDirection)
    {
        case UP:
        {
            if(xGridBox == 15)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox+1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case RIGHT:
        {
            if(yGridBox == 3)
                return 0;
            else
            {
                if(maze[yGridBox+1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case DOWN:
        {
            if(xGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox-1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case LEFT:
        {
            if(yGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox-1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
    }
}

int isLeftFree()
{
    switch(robotDirection)
    {
        case UP:
        {
            if(xGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox-1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case RIGHT:
        {
            if(yGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox-1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case DOWN:
        {
            if(xGridBox == 15)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox+1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case LEFT:
        {
            if(yGridBox == 3)
                return 0;
            else
            {
                if(maze[yGridBox+1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
    }
}

int isForwardFree()
{
    switch(robotDirection)
    {
        case UP:
        {
            if(yGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox-1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case RIGHT:
        {
            if(xGridBox == 15)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox+1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case DOWN:
        {
            if(yGridBox == 3)
                return 0;
            else
            {
                if(maze[yGridBox+1][xGridBox] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
        case LEFT:
        {
            if(xGridBox == 0)
                return 0;
            else
            {
                if(maze[yGridBox][xGridBox-1] == ' ')
                    return 1;
                else
                    return 0;
            }
            break;
        }
    }
}

void moveForward()
{
    LATFbits.LATF3 = 1;
            if(robotDirection == UP)
            {
                targetYCoordinate = yCoordinate -50;
                targetXCoordinate = xCoordinate;
            }
            else if(robotDirection == RIGHT)
            {
                targetYCoordinate = yCoordinate;
                targetXCoordinate = xCoordinate+50;
            }
            else if(robotDirection == DOWN)
            {
                targetYCoordinate = yCoordinate+50;
                targetXCoordinate = xCoordinate;
            }
            else if(robotDirection == LEFT)
            {
                targetYCoordinate = yCoordinate;
                targetXCoordinate = xCoordinate-50;
            }
            while((targetYCoordinate != yCoordinate) || (targetXCoordinate != xCoordinate))
            {
                while(TXSTA1bits.TXEN == 1){};
                transmitBuffer_push(moveForwardCommand,3);
                while(1){
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    motionBufferSize = receiveBuffer_pop(motionEncoderBuffer);
                    LATAbits.LATA4 = 1;
                    if(motionBufferSize >= 2 && motionEncoderBuffer[0] == 'E')
                    {
                        LATAbits.LATA5 = 1;
                        if(robotDirection == UP)
                            yCoordinate -= motionEncoderBuffer[1];
                        else if(robotDirection == RIGHT)
                            xCoordinate += motionEncoderBuffer[1];
                        else if(robotDirection == DOWN)
                            yCoordinate += motionEncoderBuffer[1];
                        else if(robotDirection == LEFT)
                            xCoordinate -= motionEncoderBuffer[1];
                        break;
                    }
                }
            }
            if(robotDirection == UP)
                yGridBox -= 1;
            else if(robotDirection == RIGHT)
                xGridBox += 1;
            else if(robotDirection == DOWN)
                yGridBox += 1;
            else if(robotDirection == LEFT)
                xGridBox -= 1;
}

void turnRight()
{
                
            LATFbits.LATF1 = 1;
            while(newDirection == robotDirection)
            {
                while(TXSTA1bits.TXEN == 1){};
                transmitBuffer_push(turnRightCommand,3);
                
                while(1)
                {
                WaitEvent(ALARM_EVENT);
                ClearEvent(ALARM_EVENT);
                motionBufferSize = receiveBuffer_pop(motionEncoderBuffer);
                if(motionBufferSize != 0 && motionEncoderBuffer[0] == 'E')
                    {
                    LATAbits.LATA2 = 1;
                        if(robotAngle < motionEncoderBuffer[1])
                        {
                            robotAngle+=360;
                            
                        }
                        robotAngle-=motionEncoderBuffer[1];
                        if(robotAngle == 0)
                            newDirection = UP;
                        else if(robotAngle == 90)
                            newDirection = LEFT;
                        else if(robotAngle == 180)
                            newDirection = DOWN;
                        else if(robotAngle == 270)
                            newDirection = RIGHT;
                        break;
                    }
                }

            }
            robotDirection = newDirection;
}

void turnLeft(void)
{
    LATFbits.LATF6 = 1;
            while(newDirection == robotDirection)
            {
                while(TXSTA1bits.TXEN == 1){};
                transmitBuffer_push(turnLeftCommand,3);
                while(1)
                {
                    WaitEvent(ALARM_EVENT);
                    ClearEvent(ALARM_EVENT);
                    motionBufferSize = receiveBuffer_pop(motionEncoderBuffer);
                    if(motionBufferSize != 0 && motionEncoderBuffer[0] == 'E')
                    {
                        LATAbits.LATA7 = 1;
                        robotAngle+=motionEncoderBuffer[1];
                        robotAngle = robotAngle%360;
                        if(robotAngle == 0)
                            newDirection = UP;
                        else if(robotAngle == 90)
                            newDirection = LEFT;
                        else if(robotAngle == 180)
                            newDirection = DOWN;
                        else if(robotAngle == 270)
                            newDirection = RIGHT;
                        break;
                    }

                }
            }
            robotDirection = newDirection;
}
/**********************************************************************
 * ----------------------- LOCAL FUNCTIONS ----------------------------
 **********************************************************************/



/**********************************************************************
 * ------------------------------ TASK3 -------------------------------
 * 
 * Will move or turn until next position
 * 
 **********************************************************************/
TASK(TASK2) 
{
    PIE1bits.RC1IE = 1;
    PORTB;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    TRISB = 0x10;
    
    RCONbits.IPEN = 1;
    INTCON2bits.RBIP = 0;
    
    
    newDirection = robotDirection;
    
	SetRelAlarm(ALARM_TSK2, 40, 40);
	while(1) {
        //WaitEvent(ALARM_EVENT);
        //ClearEvent(ALARM_EVENT);
        LATAbits.LATA0 = 1;
        
        if(backtrackEnabled==0)
        {
            if(isRightFree())
            {        
                turnRight();
                moveForward();
                //****/
            }
            else if(isForwardFree())
            {   
                moveForward();
            }
            else //turn left
            {
                turnLeft();
            }
        }
        else
        {
            if(isLeftFree())
            {        
                turnLeft();
                moveForward();
                //****/
            }
            else if(isForwardFree())
            {   
                moveForward();
            }
            else //turn left
            {
                turnRight();
            }
        }
        
            LATFbits.LATF7 = 1;
            SetEvent(TASK0_ID,MOVEMENT_EVENT);
            TerminateTask();
            break;
        }
	TerminateTask();
}


/* End of File : tsk_task2.c */