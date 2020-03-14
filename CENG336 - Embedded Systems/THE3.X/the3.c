//2237709 Ege Ozbek
//2171320 Azad Baykara

/*
  Basic structure of the code can easily be understood from the main function,
 * basically board goes through state changes controlled by variable digitStage,
 * In ISR routine instead of handling interrupts inside ISR , we handle them in 
 * main routine with the help of indicator flags
 * 
 * DigitStages:
 * 
 * digitStage = 0 Board starts in this state, waits 3 sec,
 * gets first digit for number of votes, upon pressing RB7 state changes to digitStage=1
 * 
 * digitStage = 1 In this stage we get the second digit for number of votes,
 * upon pressing RB7 stage changes to digitStage = 2
 * 
 * digitStage = 2 Voting starts and board remains in this stage until the end.
 * When voting ends , only ADC and timer0 interrupts remain available  
 * 
 *init1() - Initializes the board, sets i/o status of each port, writes out
 *starting LCD screen   
 * 
 * numToStringPrint() - helper function for updating number of votes on LCD screen
 * 
 * refreshTimeLeft() - helper function for updating time left each second
 * 
 * refreshPerson() - helper function that prints out nominee data upon ADC
 * interrupt or upon casting a vote
 * 
 * init2() - Sets the board for entering number of voters, prints out enter
 * votes text
 * 
 * enableTimers() - Helper function for enabling timer0 and timer 1 interrupts 
 * for voting stage
 * 
 * enableADC() - Helper function for enabling ADC interrupt
 * 
 * refreshSevenSegment() - Helper function for refreshing seven segment display,
 * to properly display id/vote of nominees without flicker, we refresh one part
 * of display each time timer0 interrupt is called
 * 
 * wait3s() - Busy wait function made out of 3 nested loops
 * 
 * incrementDigit(unsigned char digit) - Helper function that makes digit loop 
 * after 9 -> 0
 * 
 * decrementTime() - Helper function that decrements time left upon timer1 interrupt
 * 
 * 
 */
#include <p18cxxx.h>
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#define _XTAL_FREQ   40000000

#include "Includes.h"
#include "LCD.h"

unsigned char i;
volatile unsigned char timer1Flag=0;
volatile unsigned char numToStringPrintFlag=0;
volatile unsigned char refreshPersonFlag = 0;
volatile unsigned char timer0Flag=0;
volatile unsigned int tmr0count=0;
volatile unsigned char adcCount=0;
volatile unsigned char tmr1count=0;
volatile unsigned char blinkStatus = 0;
volatile unsigned char rb_var;
const char electroVote[16] = " #Electro Vote#";
const char enterVotersText[16] = " #Enter Voters#";
const char emptyline [16]=" ##############";
char ids [8][3] ={"18","64","34","23","33","67","63","99"};
const char names [8][7] = {"Poe   ","Galib ","Selimi","Nesimi","Hatayi","Zweig ","Nabi  ","Austen"};
unsigned char sevenSegmentTable [11] = {0X3F,0X06,0X5B,0X4F,0X66,0X6D,0X7C,0X07,0X7F,0X6F,0X40};
unsigned char votes [8] = {0,0,0,0,0,0,0,0};
volatile unsigned char onlarDigit=0;
volatile unsigned char birlerDigit=0;
volatile unsigned char digitStage=0; //0 for 10lar , 1 for 1 ler
volatile unsigned char timeH=9;
volatile unsigned char timeL=1;
volatile unsigned char currentPerson=0;
volatile unsigned char voteEnds=0;
volatile unsigned char ledPointer;
volatile unsigned char totalVotes = 0;
volatile unsigned char maxVotedPerson = 0;
void init1() {
    tmr0count=0;
    tmr1count=0;
    blinkStatus = 0;
    for (i=0; i < 8; i++)
        votes[i] = 0;
    onlarDigit=0;
    birlerDigit=0;
    digitStage=0;
    timeH=9;
    timeL=1;
    currentPerson=0;
    voteEnds=0;
    totalVotes = 0;
    INTCON = 0x00;
    RCONbits.IPEN = 1;
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
    TRISE = 0x02;
    TRISF = 0;
    TRISG = 0;
    TRISH = 0;
    TRISJ = 0;
    LATA = 0;
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    LATF = 0;
    LATG = 0;
    LATH = 0;
    LATJ = 0;
    LATH0=0x01;
        ///////////////// Reset process from datasheet //////////////
    __delay_ms(15);
    __delay_ms(15);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();

    __delay_ms(6);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();

    __delay_us(300);
   
	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x30;			  // Write 0x3 value on data bus
	ToggleEpinOfLCD();

    __delay_ms(2);

	PORTD &= 0x0F;			  // Make Data pins zero
	PORTD |= 0x20;			  // Write 0x2 value on data bus
	ToggleEpinOfLCD();

	__delay_ms(2);
  /////////////// Reset Process End ////////////////
	WriteCommandToLCD(0x2C);    //function set  //2C ya da 2D
	WriteCommandToLCD(0x0C);    //display on,cursor off,blink off //OxOC cursor offf
	WriteCommandToLCD(0x01);    //clear display
    WriteCommandToLCD(0x80);
    WriteStringToLCD(electroVote);
    WriteCommandToLCD(0xC0);
    WriteStringToLCD(emptyline);
}

void numToStringPrint() // update the entered voter number
{
    WriteCommandToLCD(0xC0);
    WriteDataToLCD(' ');
    unsigned char temp = (unsigned char)((int)'0'+onlarDigit);
    WriteDataToLCD(temp);
    temp = (unsigned char)((int)'0'+birlerDigit);
    WriteDataToLCD(temp);
}

void refreshTimeLeft() 
{
    WriteCommandToLCD(0x80);
    WriteStringToLCD("  Time left :");
    unsigned char temp = (unsigned char)((int)'0'+timeH);
    WriteDataToLCD(temp);
    temp = (unsigned char)((int)'0'+timeL);
    WriteDataToLCD(temp);
}
void refreshPerson() // refreshes the current nominee info upon a vote cast or ADC change
{
    WriteCommandToLCD(0xC2);
    unsigned char temp;
    
    WriteStringToLCD(ids[currentPerson]);
    WriteDataToLCD(' ');
    WriteStringToLCD(names[currentPerson]);
    WriteDataToLCD(' ');
    WriteDataToLCD(':');
    unsigned char currentVote = votes[currentPerson];
    unsigned char firstDigit = 0;
    unsigned char secondDigit = 0;
    firstDigit = currentVote/10;
    secondDigit = currentVote%10;
    temp = (unsigned char)((int)'0'+ firstDigit);
    WriteDataToLCD(temp);
    temp = (unsigned char)((int)'0'+ secondDigit);
    WriteDataToLCD(temp);
}



void init2() {
    PORTB;
    RBIF = 0;
    TRISB = 0xC0;  // RB7:6 input
    TRISE = 0;
    INTCON2bits.RBIP = 1;   // HIGH
    
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 0;
    INTCON = 0xC8;  // 11001000
    T0CONbits.TMR0ON = 0;
    T1CONbits.TMR1ON = 0;
    // update the screen
    ClearLCDScreen();
    WriteCommandToLCD(0x80);
    WriteStringToLCD(enterVotersText);
    WriteCommandToLCD(0xC0);
    numToStringPrint();
}
void enableTimers() {
    INTCON2bits.TMR0IP = 1; // HIGH
    IPR1bits.TMR1IP = 1;    // HIGH
    
    T0CON = 0xC5;
    T1CONbits.RD16 = 1;
    T1CONbits.T1RUN = 1;
    
    TMR0 = 0;
    TMR1 = 0;
    PIE1bits.TMR1IE = 1;
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
    T1CONbits.TMR1ON = 1;
}

void enableADC()
{
    ADCON0 = 0x33;
    ADCON1 = 0x02;
    ADFM = 1;
    TRISH4 = 1;
    
    ADIP=1;
    ADIE=1;
    ADIF=0;
}

void refreshSevenSegment() 
{
    unsigned char currentVote = votes[maxVotedPerson];
    unsigned char firstDigit = 0;
    unsigned char secondDigit = 0;
    unsigned char idFirst = 0;
    unsigned char idSecond = 0;
    
    if (totalVotes==0){ // set initial state as '----'
        idFirst = 10;   // 10. index the the sevenSegmentTable is set as '-'
        idSecond = 10;
        firstDigit = 10;
        secondDigit = 10;
    }
    else {
        idFirst = (unsigned char)(ids[maxVotedPerson][0]-(int)'0'); // tens digit of id
        idSecond = (unsigned char)(ids[maxVotedPerson][1]-(int)'0'); // units digit of id
        firstDigit = currentVote/10;    // tens digit of current vote
        secondDigit = currentVote%10;   // units digit of current vote
    }
    
    // This part is for the seven segment display
    // for each call, update one of RH<3:0>
    if(LATH3 == 0x01){
        LATH3 = 0X00;
        LATH0 = 0X01;
        LATJ = sevenSegmentTable[idFirst];
    }
    else if(LATH0 == 0x01){
        LATH0 = 0X00;
        LATH1 = 0X01;
        LATJ = sevenSegmentTable[idSecond];    
    }
    else if(LATH1 == 0x01){
        
        LATH1 = 0X00;
        LATH2 = 0X01;
        LATJ = sevenSegmentTable[firstDigit];
    }
    else if(LATH2 == 0x01){
        LATH2 = 0X00;
        LATH3 = 0X01;
        LATJ = sevenSegmentTable[secondDigit];
    }
}

void wait3s(){
    unsigned char i=0;
    unsigned char j=0;
    unsigned char k=0;
    while (i < 60){
        j=0;
        while (j < 200) {
            k=0;
            while(k < 250) {
                asm("NOP");
                k++;
            }
            j++;
        }
        i++;
    }
}
unsigned char incrementDigit(unsigned char number)
{
    if(number==9)
    {
        return 0;
    }
    return number+1;
}
void decrementTime()
{
    // decrement the 90 second time accordingly
    if(timeL == 0)
    {
        if(timeH==0)
        {
            voteEnds=1;
        }
        else
        {
            timeH--;
            timeL=9;
        }
    }
    else
    {
        timeL--;
    }
}

__interrupt(high_priority) void highISR(void) {
    if (TMR1IE && TMR1IF) {
        TMR1IF = 0;
        tmr1count++;
        if(tmr1count==152) // does it take 1 second?
        {
            tmr1count=0;
            if(digitStage==2) // set the flag to decrement time and update the LCD in main
                timer1Flag=1;
        }
    }
    if (RBIE && RBIF) {
        rb_var = PORTB;
        RBIF = 0;
        if(digitStage==0) // we're currently on the tens digit
        {
            if(rb_var & 0x80) // RB change occurs due to RB7
                digitStage=1;
            if(rb_var & 0x40) // RB change occurs due to RB6
            {
                onlarDigit = incrementDigit(onlarDigit);
                numToStringPrintFlag=1;
            }
        }
        else if(digitStage==1) // we're currently on the units digit
        {
            if(rb_var & 0x80)  // RB change occurs due to RB7
                digitStage=2;  // after digitStage=2, voting will start

            if(rb_var & 0x40) // RB change occurs due to RB6
            {
                birlerDigit = incrementDigit(birlerDigit); // increment units digit
                numToStringPrintFlag=1;     // set the flag to update the number of voters in main
            }
        }
        else // digitStage == 2
        {
            if(!voteEnds)   // if voting process is still in process
            {
                if(rb_var & 0x80){ // if RB change occurs because of RB7
                    votes[currentPerson]++;     // increment the current person's vote
                    totalVotes++;
                    refreshPersonFlag=1;        // set the flag to update LCD in main
                }
                if(votes[currentPerson]>votes[maxVotedPerson]){ // if current person's votes are higher
                    maxVotedPerson = currentPerson;    // update the max voted person
                }
                if (totalVotes == 10*onlarDigit + birlerDigit){  // if all the votes are used 
                    voteEnds = 1;                               // then voting ends
                }
            }
        }
    }
    
    if (TMR0IE && TMR0IF) {
        refreshSevenSegment();  // update seven segment display here
        TMR0IF = 0;
        tmr0count++;
        adcCount++;
        
        if(adcCount==60){ // did it take 100ms?
            adcCount=0;
            if(digitStage==2) // if voting process started, enable ADC for further reading
                enableADC();
        }
        if(!voteEnds && tmr0count==152){ // did it take 250ms?
            tmr0count=0;                 
            if(digitStage==2)   // if voting process started, set flag to change blink state in main function
                timer0Flag=1;
        }
        
    }
    if(ADIE ==1 && ADIF==1){ 
        if(digitStage==2){  // if ADC interrupts occurs in voting process, 
            ADIF=0;
            unsigned int value = 256*ADRESH + ADRESL;   // read the value
            currentPerson=value/128;    // identify the person to display
            refreshPersonFlag=1;        // set the flag to update the LCD in main function
            ADIE = 0;                   // disable the ADC again
        }
    }
}

// Main Function
void main(void)
{
    init1();       // first initializations
    while(!RE1);   // wait until RE1 pressed
    while(RE1);    // wait until RE1 released
    wait3s();      // wait 3s until voting starts
    init2();       // set timer interrupts for voting stage
    
    while(1){
        if(numToStringPrintFlag){
            numToStringPrint();
            numToStringPrintFlag=0;
        }
        if(digitStage==2){   // when the number of voters are entered (digitStage becomes 2)
            enableTimers();  // start the timers and break 
            break;
        }
    }
    while(1){
        if(timer1Flag==1){  // if 1 second passes, decrement time and update screen
            decrementTime();
            refreshTimeLeft();
            timer1Flag=0;
        }
        if(timer0Flag==1){  // if 250ms passes, flip the blink cursor
            if(blinkStatus){
               WriteCommandToLCD(0xC1);
               WriteDataToLCD('>');
               blinkStatus=0x00;
            }
            else{
               WriteCommandToLCD(0xC1);
               WriteDataToLCD(' ');
               blinkStatus=0x01;
            }
        }
        
        if(refreshPersonFlag==1)    // update the LCD with the new person
        {
            refreshPerson();
            refreshPersonFlag=0;
        }
        if(voteEnds==1)
            break;
     
    }
    TMR1IE = 0;
    PORTB;
    RBIE = 0;
    // vote ended, so just update the LCD with the ADC interrupt
    while(1){
        if(refreshPersonFlag==1){
            refreshPerson();
            refreshPersonFlag=0;
        }
    }
    
    
}