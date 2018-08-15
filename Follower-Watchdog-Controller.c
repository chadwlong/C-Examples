/*
 * File:   Follower-Watchdog-Controller.c
 * Author: Chad Long, Dylan Kuehni, Jacob Bautr
 *
 * Created on March 9, 2014, 8:11 PM
 * Revised for readability on August 14, 2018, 5:14 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p18F2580.h>                                                       //pic version
#include <usart.h>                                                          //usart initialization
#include <delays.h>

// PIC18F2580 Configuration Bit Settings

// CONFIG1H
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF     // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bit (1K words (2K bytes) boot block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)



//Defines
#define delay_cycles 10000
#define RED_LEDS_ON PORTAbits.RA0 = 1; PORTAbits.RA3 = 1;
#define RED_LEDS_OFF PORTAbits.RA0 = 0; PORTAbits.RA3 = 0;
#define BLUE_LEDS_ON PORTAbits.RA1 = 1; PORTAbits.RA4 = 1;
#define BLUE_LEDS_OFF PORTAbits.RA1 = 0; PORTAbits.RA4 = 0;
#define GREEN_LEDS_ON PORTAbits.RA2 = 1; PORTAbits.RA5 = 1;
#define GREEN_LEDS_OFF PORTAbits.RA2 = 0; PORTAbits.RA5 = 0;


#define IRLED_Direction TRISBbits.TRISB0 = 0
#define IRLED PORTBbits.RB0;

/** D E C L A R A T I O N S *******************************************/
extern void init();
extern void usart_init();
extern void watchdogEngage();
extern void sendState(int i);
extern void follower();
extern void FOLLOW();
extern void follow_on(unsigned char a);
extern void follow_off(unsigned char b);
extern void uart_str(char* a);
extern void newline();

/***Prototyping Functions**********************************/
void init();
void usart_init();
void watchdogEngage();
void sendState();
void follower();
void FOLLOW();
void follow_on(unsigned char a);
void follow_off(unsigned char b);
void newline();


//Global variables for use in functions
volatile unsigned long delay_count = 0;
unsigned char in;
unsigned char temp, i;
char* string;


//Chad's method and code
//main while loop
void main()
{
        int state_indicator = 0;				//1 for watchdog mode, 0 for follower mode
	init();							//method to initialize oscillator, I/O, and interrupts
        usart_init();						//method to set baud rate, TX, RX, and interrupt communication

        //loop checking for user input
        while(1)
        {
	    Delay1TCY();
			
            for(in=0;in<1;in++){
                Delay1TCY();
                FOLLOW();					//initialize IR frequency for follower mode
                }

	  if(PORTBbits.RB4 == 1)				//Checking if user input from switch indicating a change to watchdg mode
          {
              //Watchdog mode
              watchdogEngage();					//Red-Blue-Red-Blue flashing lights
              state_indicator = 1;				//set state indicator variable to watchdog value
              sendState(state_indicator);			//send change of variable

          }
          else if(PORTBbits.RB4 == 0)
          {
              //Follower mode
              follower();					//Green-Purple-Green-Purple flashing lights
              state_indicator = 0;				//set state indicator variable to follower value
              sendState(state_indicator);

          }
        }
}

//Chad's method and code
//initalizes internal oscillator, analog I/O, and interrupts
void init()
{
        OSCCONbits.IRCF2 = 1;
        OSCCONbits.IRCF1 = 1;
        OSCCONbits.IRCF0 = 1;                   //sets internal oscillator to 4 MHz

        OSCCONbits.SCS1 = 1;

        ADCON0 = 0x00;                          //disables analog
        ADCON1 = 0x0F;

        TRISBbits.TRISB4 = 1;                   //sets input outputs
        TRISAbits.TRISA0 = 0;
        TRISAbits.TRISA1 = 0;
        TRISAbits.TRISA2 = 0;
        TRISAbits.TRISA3 = 0;
        TRISAbits.TRISA4 = 0;
        TRISAbits.TRISA5 = 0;
        TRISBbits.TRISB0 = 0;

        //floating
        TRISAbits.TRISA6 = 0;
        TRISAbits.TRISA7 = 0;
        TRISBbits.TRISB5 = 0;
        TRISBbits.TRISB3 = 0;
        TRISBbits.TRISB2 = 0;
        TRISBbits.TRISB1 = 0;
        TRISCbits.TRISC0 = 0;
        TRISCbits.TRISC1 = 0;
        TRISCbits.TRISC2 = 0;
        TRISCbits.TRISC3 = 0;
        TRISCbits.TRISC4 = 0;
        TRISCbits.TRISC5 = 0;

        INTCONbits.PEIE = 1;                    //enable peripheral interrupts
        INTCONbits.GIE = 1;                     //enable interrupts
}

//Chad's method and code
//method to initialize usart communication
void usart_init()
{
    TXSTAbits.BRGH = 0;                         //enable the high baud rate mode
    SPBRG = 12;                                 //sets baud rate = to 9600 by formula for 1 Mhz
    BAUDCONbits.BRG16 = 0;

    TXSTAbits.TXEN = 1;                         //enables transmit
    TXSTAbits.SYNC = 0;                         //sets device for asynchronous mode
    RCSTAbits.SPEN = 1;                         //sets pins to RX and TX functionality
    RCSTAbits.CREN = 1;                         //enable continuous receiving across usart

    TRISCbits.TRISC7 = 1;                       //sets port c to input
    TRISCbits.TRISC6 = 0;                       //sets device for asynchronous mode

    PIE1bits.RCIE = 1;                          //enables USART receive interrupt
}

//Chad's method and code
//LED indicators for Watchdog mode
void watchdogEngage()
{
    //turn LED off
    RED_LEDS_OFF				//defined label to set the microcontroller output pins to turn Red off for the RGB LED
    BLUE_LEDS_OFF				//defined label to set the microcontroller output pins to turn Blue off for the RGB LED
    GREEN_LEDS_OFF				//defined label to set the microcontroller output pins to turn Blue off for the RGB LED

    //Watchdog Sequence R-B-R-B
    RED_LEDS_ON																	//defined label to set the microcontroller output pins to turn Red on for the RGB LED
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)			//flexible delay loop
    {
    }
    RED_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_ON																//defined function to set the microcontroller output pins to turn Blue on for the RGB LED
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    RED_LEDS_ON
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    RED_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_ON
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_OFF
}

//Chad's method and code
//LED indicators for follower mode
void follower()
{
    //turn LED off
    RED_LEDS_OFF
    BLUE_LEDS_OFF
    GREEN_LEDS_OFF

    //Follower Sequence G-P-G-P
    GREEN_LEDS_ON															//defined label to set the microcontroller output pins to turn Green on for the RGB LED
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    GREEN_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_ON
    RED_LEDS_ON
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_OFF
    RED_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    GREEN_LEDS_ON
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    GREEN_LEDS_OFF
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_ON
    RED_LEDS_ON
    for(delay_count = 0; delay_count < delay_cycles; delay_count++)
    {
    }
    BLUE_LEDS_OFF
    RED_LEDS_OFF
}

//Dylan's method and code to create newline
void newline()
{
    TXREG = '\r';
    while (TXSTAbits.TRMT == 0) {};
    TXREG = '\n';
    while (TXSTAbits.TRMT == 0) {};
}

//Dylan's method to convert uart input to string with needed delay to prevent malfunction
void uart_2str(rom char* a)
{
    int i = 0;
    while (a[i] != '\0')			
    {
        TXREG = a[i];
        i++;
        while (TXSTAbits.TRMT == 0) {};			//loop to delay until TXREG memory cleared
    }
}

//Dylan's method and code
void follow_on(unsigned char a){
    for (i=0;i<=a;i++){
        PORTBbits.RB0 = 1;
        Delay10TCYx (1);
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
    
        PORTBbits.RB0 = 0;
        Delay10TCYx (2);
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
    }
}

//Dylan's method and code
void follow_off(unsigned char b){
    for (i=0;i<=b;i++){
        PORTBbits.RB0 = 1;
        Delay10TCYx (1);
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
    
        PORTBbits.RB0 = 1;
        Delay10TCYx (2);
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
        Delay1TCY();
    }
}

//Dylan's method and code
void FOLLOW(){
    follow_on(115);
    follow_off(85);
    follow_on(35);
    follow_off(165);
    follow_on(150);
    }


//Jacob's method and code
void sendState(int state_indicator)
{
    //Watchdog to XBEE
    if(state_indicator == 1)
    {
        TXREG = 'W';
        while (TXSTAbits.TRMT == 0) {};
        TXREG = '\r';
        while (TXSTAbits.TRMT == 0) {};
        TXREG = '\n';
        while (TXSTAbits.TRMT == 0) {};
    }

    //Follower to XBEE
    if(state_indicator == 0)
    {
        TXREG = 'F';
        while (TXSTAbits.TRMT == 0) {};
        TXREG = '\r';
        while (TXSTAbits.TRMT == 0) {};
        TXREG = '\n';
        while (TXSTAbits.TRMT == 0) {};
    }
}


