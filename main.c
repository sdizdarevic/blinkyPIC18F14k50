
#include <p18cxxx.h>

// 14K50 KONFIGURACIJSKI BITI**************************************************/
// Konfiguracijski biti za bootloader ne smiju biti izmijenjeni
        #pragma config CPUDIV = NOCLKDIV
        #pragma config USBDIV = OFF
        #pragma config FOSC   = HS
        #pragma config PLLEN  = ON
        #pragma config FCMEN  = OFF
        #pragma config IESO   = OFF
        #pragma config PWRTEN = OFF
        #pragma config BOREN  = OFF
        #pragma config BORV   = 30
        #pragma config WDTEN  = OFF
        #pragma config WDTPS  = 32768
        #pragma config MCLRE  = OFF
        #pragma config HFOFST = OFF
        #pragma config STVREN = ON
        #pragma config LVP    = OFF
        #pragma config XINST  = OFF
        #pragma config BBSIZ  = OFF
        #pragma config CP0    = OFF
        #pragma config CP1    = OFF
        #pragma config CPB    = OFF
        #pragma config WRT0   = OFF
        #pragma config WRT1   = OFF
        #pragma config WRTB   = OFF
        #pragma config WRTC   = OFF
        #pragma config EBTR0  = OFF
        #pragma config EBTR1  = OFF
        #pragma config EBTRB  = OFF       


// prototip funkcije
void dly1mS(void);
void delay_mS(unsigned int dly);


/** VECTOR REMAPPING ***********************************************/
// Remapiranje vektora na nove lokacije za Microchip PIC18 bootloader
// Isti okupira adrese 0x00-0xFFF

	
	#define REMAPPED_RESET_VECTOR_ADDRESS				0x1000
	#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
	#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018

	void YourHighPriorityISRCode();
	void YourLowPriorityISRCode();
	
	extern void _startup (void);         
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}

	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	#pragma code
	
	
	//Interapt rutine
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{

		//Koji interapt flag uzrokuje interapt itd. 
	}

	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Koji interapt flag uzrokuje interapt itd. 
	}	

	#pragma code

#define LED4	LATCbits.LATC4			// LED4 -> RC4
#define LED3	LATCbits.LATC5			// LED3 -> RC5



void main(void)
{   
	ANSEL = 0;						// svi pinovi digitalni
	ANSELH = 0;

	TRISB = 0xff;					// svi pinovi ulazni;
	TRISC = 0xef;					// C4 (RC4 -> Led4) izlaz
	T3CON = 0xb1;					// 16 bit, 8:1 prescale, 

	while(1) {
		LED4 = 0;
		delay_mS(400);
		LED4 = 1;
		delay_mS(300);
		 
		
		
	}	
}


void dly1mS(void)
{
	TMR3H = (0-1500)>>8;						// 1500 * (8/12) = 1000uS
	TMR3L = (0-1500)&0x0ff;					// TMR3
	PIR2bits.TMR3IF = 0;
	while(!PIR2bits.TMR3IF);										// čekaj 1mS
//	while(!PIR2bits.TMR3IF)	if(!PORTAbits.RA3) Reset();	
}
	
	
void delay_mS(unsigned int dly)
{
unsigned int x;
	for(x=0; x<dly; x++) dly1mS();
}	