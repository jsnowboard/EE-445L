// filename ******** Lab3.c ************** 
// made for the TM4C123G and ST7735 LCD display
// Lab 3 is an alarm clock made with the ST7735 display,
// a pn2222 n-channel bjt, a 1n914b 4ns swutching diode, and tactile switches.
// TestCases to the LCD.
// 
// Jeremiah Bartlett -- JJB2954
// Ty Winkler -- TAW2328
// Initial Creation: 1/25/2016
// Last Revision: 1/31/2016
// Lab Number: 16325
//
// Lab 3
// TA: Ryan Chow
//
// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"
#include "Lab2.h"
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"

#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCvalue;
int updateTime = 0;
int currentNanoSeconds = 0;
int currentSeconds = 0;
int currentMinutes = 0;
int currentHours = 0;
int i = 0;

// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 0xC40FF;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}

void Timer1_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count
	TIMER1_TAILR_R = 0xFFFFFFFF;  // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_CTL_R = 0x00000001;    // 10) enable 
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
	updateTime = 1;
}

unsigned int Errors,AnError;

int main(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
  Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
  //Timer1_Init();
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                     	 				// turn off LED
	EnableInterrupts();
	Output_Init();              					// initialize output device
	ST7735_SetCursor(0,0);
	printf("Current Time");
	ST7735_SetCursor(0,1);
	printf("%02d:%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds, currentNanoSeconds);
  while(1){
    //PF1 ^= 0x02;  // toggles when running in main
		if(updateTime == 1){
			currentNanoSeconds++;
			if(currentNanoSeconds == 100){
				currentSeconds++;
				currentNanoSeconds = 0;
				if(currentSeconds == 60){
					currentMinutes++;
					currentSeconds = 0;
					if(currentMinutes == 60){
						currentHours++;
						currentMinutes = 0;
						if(currentHours == 24){
							currentHours = 0;
						}
					}
				}
			}
			ST7735_SetCursor(0,1);
			printf("%02d:%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds, currentNanoSeconds);
			updateTime = 0;
		}
		Errors = 0;
	}
}
