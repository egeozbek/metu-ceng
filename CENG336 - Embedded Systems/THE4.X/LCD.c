#include <p18cxxx.h>
#include "LCD.h"
#include "Includes.h"

void ToggleEpinOfLCD(void)
{
    INTCONbits.RBIE=0x00;
    LATBbits.LATB5 = 1;                // Give a pulse on E pin
    INTCONbits.RBIE=0x01;
    Delay10KTCYx(1);      // so that LCD can latch the
    INTCONbits.RBIE=0x00;
    LATBbits.LATB5 = 0;                // data from data bus
    INTCONbits.RBIE=0x01;
    Delay10KTCYx(1);
}

void WriteCommandToLCD(unsigned char Command)  
{
    LATBbits.LATB2 = 0;                          // It is a command

    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD |(Command&0xF0);      // Write Upper nibble of data
    ToggleEpinOfLCD();                  // Give pulse on E pin

    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | ((Command<<4)&0xF0);// Write Lower nibble of data

    ToggleEpinOfLCD();                  // Give pulse on E pin
}

void WriteDataToLCD(char LCDChar)  
{
    LATBbits.LATB2 = 1;                          // It is data

    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | (LCDChar&0xF0);     // Write Upper nibble of data
    ToggleEpinOfLCD();                  // Give pulse on E pin

    PORTD = PORTD & 0x0F;               // Make Data pins zero
    PORTD = PORTD | ((LCDChar<<4)& 0xF0); // Write Lower nibble of data


    ToggleEpinOfLCD();                  // Give pulse on E pin
}

void InitLCD(void)
{
    LATBbits.LATB0 = 1;
    TRISA = 0;
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

}

void WriteStringToLCD(const char *s)
{
    while(*s)
        WriteDataToLCD(*s++);   // print first character on LCD
}

void ClearLCDScreen(void)       // Clear the Screen and return cursor to zero position
{
    WriteCommandToLCD(0x01);    // Clear the screen
    Delay10KTCYx(2);              // Delay for cursor to return at zero position
}