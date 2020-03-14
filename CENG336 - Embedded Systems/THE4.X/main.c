#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include "common.h"
#include "LCD.h"
#include "defines.h"

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE       0


/**********************************************************************
 * Function prototypes.
 **********************************************************************/
void main (void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);
void InitLCDForMap(void);

void Delay10KTCYx(unsigned char);
void Delay1KTCYx(unsigned char);

unsigned char maze [4] [16];
unsigned int robotAngle = 180;
unsigned char robotDirection = DOWN;
unsigned int xCoordinate = 25;
unsigned int yCoordinate = 25;
unsigned char xGridBox = 0;
unsigned char yGridBox = 0;
volatile unsigned char mapShowsFirstColumns=1;
unsigned char rb_pressed = 0;
unsigned char unseenBoxes = 62;
unsigned char m,n;


extern union Timers Tmr0;


AppModeType SelectedMode;

/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * Setup the different alarms and start the kernel.
 *
 **********************************************************************/
void main(void)
{
  STKPTR = 0;
  SelectedMode = DEFAULT_MODE;
  Init();
  InitLCDForMap();
  
  while(1) {
    StartOS(SelectedMode);
  }
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/

void InitLCDForMap(void)
{
    WriteCommandToLCD(0x80);
    WriteStringToLCD(maze[0]);
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(maze[1]);
}
void Init(void)
{
    PORTB;
    INTCONbits.RBIF = 0;
    LATBbits.LATB0 = 1;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    //TRISE = 0;
    TRISF = 0;
    TRISG = 0;

    TRISH = 0;
    TRISJ = 0;

    PORTA = 0;
    PORTC = 0;
    PORTE = 0;
    PORTF = 0;
    PORTG = 0;

    PORTH = 0;
    PORTJ = 0;


    LATBbits.LATB5  = 0;   // E  = 0
    LATBbits.LATB2  = 0;   // RS = 0
    LATDbits.LATD4  = 0;   // Data bus = 0
    LATDbits.LATD5  = 0;   // Data bus = 0
    LATDbits.LATD6  = 0;   // Data bus = 0
    LATDbits.LATD7  = 0;   // Data bus = 0
    TRISBbits.TRISB5 = 0;   // Make Output
    TRISBbits.TRISB2 = 0;   // Make Output
    TRISDbits.TRISD4 = 0;   // Make Output
    TRISDbits.TRISD5 = 0;   // Make Output
    TRISDbits.TRISD6 = 0;   // Make Output
    TRISDbits.TRISD7 = 0;   // Make Output
    LATBbits.LATB5  = 0;   // E  = 0
    LATBbits.LATB2  = 0;   // RS = 0

        ///////////////// Reset process from datasheet //////////////
    Delay10KTCYx(15);
    Delay10KTCYx(15);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();		  // Give pulse on E pin

   Delay10KTCYx(6);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();		  // Give pulse on E pin

   Delay1KTCYx(3);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();		  // Give pulse on E pin

   Delay10KTCYx(2);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x20;			  // Write 0x2 value on data bus
	ToggleEpinOfLCD();		  // Give pulse on E pin

	Delay10KTCYx(2);
  /////////////// Reset Process End ////////////////
	WriteCommandToLCD(0x2C);    //function set  //2C ya da 2D
	WriteCommandToLCD(0x0C);    //display on,cursor off,blink off //OxOC cursor offf
	WriteCommandToLCD(0x01);    //clear display
    LATBbits.LATB1 = 1;
    
    
	FSR0H = 0;
	FSR0L = 0;
    
	
	/* User setting : actual PIC frequency */
	Tmr0.lt = _40MHZ;
	
	/* Timer OFF - Enabled by Kernel */
	T0CON = 0x08;
	TMR0H = Tmr0.bt[1];
	TMR0L = Tmr0.bt[0];
	
	/* configure I/O ports */
    TRISBbits.RB4 = 1;
    TRISCbits.RC7 = 1; // TX1 and RX1 pin configuration
    TRISCbits.RC6 = 0;
	
	/* configure USART transmitter/receiver */
	SPBRG1 = 21;		// for 40 MHz, to have 115200 baud rate, it should be 21
	TXSTA1 = 0x04;      // (= 00000100) 8-bit transmit, transmitter NOT enabled,
                        // asynchronous, high speed mode
	RCSTA1 = 0x90;      // (= 10010000) 8-bit receiver, receiver enabled,
                        // continuous receive, serial port enabled
	
	/* configure the interrupts */
	INTCON = 0;			// clear interrupt register completely
	PIE1bits.TX1IE = 1;	// enable USART transmit interrupt
	PIE1bits.RC1IE = 1;	// enable USART receive interrupt
	PIR1 = 0;			// clear all peripheral flags
	
	INTCONbits.PEIE = 1;// enable peripheral interrupts
	INTCONbits.GIE = 1;	// globally enable interrupts
    
    INTCONbits.RBIE = 1; //enables rb interrupt
    INTCONbits.RBIF = 0 ;
    RCONbits.IPEN = 1;
    INTCON2bits.RBIP = 0;
    
    for(m=0; m<4; m++){
        for(n=0; n<16; n++){
            maze[m][n] = 'X';
        }
    }
    maze[0][0] = ' ';
    maze[0][15] = ' ';
     //0 for up, 1 for right, 2 for down, 3 for left
        
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the 
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
