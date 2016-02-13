// filename ******** Lab1.c ************** 
// made for the TM4C123G and ST7735 LCD display
// Test the functions in fixed.c by printing the
// TestCases to the LCD.
// 
// Jeremiah Bartlett -- JJB2954
// Ty Winkler -- TAW2328
// Initial Creation: 1/25/2016
// Last Revision: 1/31/2016
// Lab Number: 16325
//
// Lab 1
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
void makePMF(int x[], int y[]);
void storeFrequency();

volatile uint32_t ADCvalue;
int array1[1000];
int array2[1000];
int array3[1000];
int frequency[1000];
int pmf[1000];
int oldtime;
int i = 0;

int pointXTest1[5] = {1,2,3,4,5};
int pointYTest1[5] = {1,2,3,4,5};

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
  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
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

void Timer2_Init(void){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x03;   // 0) activate TIMER1
  delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, down-count
	TIMER1_TAILR_R = 799000;  // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
	NVIC_PRI5_R = 0x00000000; 		// 8) priority 1
  TIMER1_CTL_R = 0x00000001;    // 10) enable 
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
  ADCvalue = ADC0_InSeq3();
  PF2 ^= 0x04;                   // profile
	
  if (i <= 999){
		int a = TIMER1_TAR_R;
		int b = oldtime - a;	
		array1[i] = a;
		array2[i] = b;
		array3[i] = ADC0_InSeq3();
		oldtime = a;
		i++;
	}
	
}

unsigned int Errors,AnError;

int main(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
  Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
  Timer1_Init();
	//Timer2_Init();											//Enable this for part D.
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                      // turn off LED
	//ADC0_SAC_R = ADC_SAC_AVG_4X;		 //enable 4 times average before obtaining result
	//ADC0_SAC_R = ADC_SAC_AVG_16X;    //enable 16 times average before obtaining result
	//ADC0_SAC_R = ADC_SAC_AVG_64X;    //enable 64 times average before obtaining result
	EnableInterrupts();
  while(1){
    PF1 ^= 0x02;  // toggles when running in main
		//GPIO_PORTF_DATA_R ^= 0x02;  // Uncomment this for part C. and comment the line above.
		//PF1 = (PF1*12345678)/1234567+0x02;  // Uncomment this for part D.
		if(i==1000){
			int largest = 0;
			for(int j; j < 1000; j++){
				if(largest < array2[i]){
					largest = array2[i];
					i++;
				}
			}
			Output_Init();              // initialize output device
			Errors = 0;
			int x;
			int y;
			DisableInterrupts();
			storeFrequency();
			makePMF(array3, pmf);
			ST7735_XYplotInit("Lab 2 PMF", 0, 4095, 0, 1000);
			//ST7735_Line(0, 0, 100, 100, ST7735_YELLOW) ;
			for(int j = 0; j < 1000; j += 8){
			 	ST7735_PlotBar(pmf[j]);
				ST7735_PlotNext();
			}
			break;
		}
  }
	while(1);
}

void storeFrequency(){
	int i = 0;
	for(i = 0; i < 1000; i+=1){
			int j = 0;
			int freq = 0;
			for(j=0; j < 1000; j++){
				if(array3[i] == array3[j]){
						freq +=1;
				}
			}
			pmf[i] = freq;
	}
}

void makePMF(int x[], int y[]){
	int i, j, t;                                                                                                     
  for(i = 1; i < 1000; i++){                               
		for(j = 1; j <= 1000 - i; j++){                             
			if(x[j-1] >= x[j]) {
        t = x[j-1];                                    
        x[j-1] = x[j];                                 
        x[j] = t;
				t = y[j-1];                                    
        y[j-1] = y[j];                                 
        y[j] = t;
      }
		}
	}
}
