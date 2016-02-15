// filename ******** Lab1.c ************** 
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
#include <math.h>
#include "ST7735.h"
#include "fixed.h"
#include "Lab2.h"
#include "lab3.h"
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"

#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
#define M_PI 						acos(-1.0)
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void increaseSeconds(void);
void increaseMinutes(void);
void increaseHours(void);
void increaseAlarmMinutes(void);
void increaseAlarmHours(void);
void updateDigitalTime(void);
void updateAlarmTime(void);
void checkInput(void);
void drawClock(void);
void checkAlarm(void);
void switchInit(void);
void checkButtons(void);
void drawMode(void);
void drawAlarm(void);

volatile uint32_t ADCvalue;
int updateTime = 0;
int updateSeconds = 0;
int updateMinutes = 0;
int updateHours = 0;
//int currentMilliSeconds = 0;
int currentSeconds = 0;
int currentMinutes = 0;
int currentHours = 0;

int alarmMinutes = 0;
int alarmHours = 0;
int alarmSet = 0;

int modeSet = 0;

int i = 0;
int cx = 64;
int cy = 90;
int radius = 50;

static int xVals[60] = {64,70,76,81,87,
93,97,102,103,104,
106,107,109,110,112,
114,112,110,109,107,
106,104,103,102,97,
93,87,81,76,70,
64,58,52,47,41,
35,31,26,25,23,
21,19,18,17,15,
14,15,17,18,19,
21,23,25,26,31,
35,41,47,52,58};

static int yVals[60] = {40,42,43,45,47,
48, 52, 56, 61, 65,
70, 76, 81, 87, 90,
93, 99, 104, 110, 115,
106, 111, 115, 124, 128,
132, 133, 135, 137, 138,
140, 138, 137, 135, 133,
132, 128, 124, 115, 111,
106, 115, 110, 104, 99,
93, 90, 87, 81, 76,
70, 65, 61, 56, 52, 
48, 47, 45, 43, 42};

static int xValh[24] = {64, 72, 79,
84, 77, 70,
64, 56, 49,
44, 51, 58,
64, 72, 79,
84, 77, 70,
64, 56, 49,
44, 51, 58
};

static int yValh[24] = {70, 78, 85,
90, 97, 104,
110, 104, 97,
90,	85, 78,
70, 78, 85,
90, 97, 104,
110, 104, 97,
90,	85, 78
};

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
  TIMER0_TAILR_R = 0x2FF0000;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
	updateTime = 1;
}

void switchInit(void){
	SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  long delay = SYSCTL_RCGC2_R;      // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R &= ~0x3E;        // PE4,3,2,1 input 
  GPIO_PORTE_AFSEL_R &= ~0x3E;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x3E;      // no analog
  GPIO_PORTE_PCTL_R &= ~0x000FFFFF; // bits for PE4,PE3,PE2,PE1
  GPIO_PORTE_DEN_R |= 0x3E;         // enable PE4,PE3,PE2,PE1
}

void alarmInit(){
	volatile uint32_t delay;
	
  SYSCTL_RCGCGPIO_R |= 0x00000008;  // 1) activate clock for Port D
  delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
  GPIO_PORTD_CR_R |= 0x02;          // allow changes to PD1
	GPIO_PORTD_DIR_R |= 0x02;         // make PD1 output
  GPIO_PORTD_AFSEL_R &= ~0x02;      // disable alt funct on PD1
  GPIO_PORTD_DEN_R |= 0x02;         // enable digital I/O on PD1
																		// configure PD1 as GPIO
  GPIO_PORTD_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFFF0F);
  GPIO_PORTD_AMSEL_R &= ~0x02;
	
	
}

unsigned int Errors,AnError;

int main(void){
	PLL_Init(Bus80MHz);                   // 80 MHz
	switchInit();
	alarmInit();
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
  Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                     	 				// turn off LED
	Output_Init();              					// initialize output device
	EnableInterrupts();
	ST7735_SetCursor(0,0);
	printf("Current Time*");
	ST7735_SetCursor(0,1);
	printf("%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds);
	ST7735_SetCursor(0,2);
	printf("Alarm Time");
	ST7735_SetCursor(0,3);
	printf("%02d:%02d", alarmHours, alarmMinutes);
  while(1){
    //PF1 ^= 0x02;  // toggles when running in main
		if(updateTime == 1){
			updateDigitalTime();
			updateTime = 0;
		}
		ST7735_SetCursor(0,1);
		printf("%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds);
		updateAlarmTime();
		checkButtons();
		checkInput();
		drawClock();
		drawMode();
		drawAlarm();
		checkAlarm();
		Errors = 0;
	}
}

void drawMode(void){
	if (modeSet == 0){
		ST7735_SetCursor(0,0);
		printf("Current Time*");
		ST7735_SetCursor(0,2);
		printf("Alarm Time ");
	} else {
		ST7735_SetCursor(0,0);
		printf("Current Time ");
		ST7735_SetCursor(0,2);
		printf("Alarm Time*");
	}
}

void drawAlarm(void){
	if (alarmSet == 0){
		ST7735_DrawPixel( 122,  3, ST7735_BLACK);
		ST7735_DrawPixel( 123,  2, ST7735_BLACK);
		ST7735_DrawPixel( 123,  3, ST7735_BLACK);
		ST7735_DrawPixel( 123,  4, ST7735_BLACK);
		ST7735_DrawPixel( 124,  1, ST7735_BLACK);
		ST7735_DrawPixel( 124,  2, ST7735_BLACK);
		ST7735_DrawPixel( 124,  3, ST7735_BLACK);
		ST7735_DrawPixel( 124,  4, ST7735_BLACK);
		ST7735_DrawPixel( 124,  5, ST7735_BLACK);
		ST7735_DrawPixel( 126,  1, ST7735_BLACK);
		ST7735_DrawPixel( 126,  5, ST7735_BLACK);
		ST7735_DrawPixel( 127,  2, ST7735_BLACK);
		ST7735_DrawPixel( 127,  3, ST7735_BLACK);
		ST7735_DrawPixel( 127,  4, ST7735_BLACK);
	} else if (alarmSet == 1){
		ST7735_DrawPixel( 122,  3, ST7735_YELLOW);
		ST7735_DrawPixel( 123,  2, ST7735_YELLOW);
		ST7735_DrawPixel( 123,  3, ST7735_YELLOW);
		ST7735_DrawPixel( 123,  4, ST7735_YELLOW);
		ST7735_DrawPixel( 124,  1, ST7735_YELLOW);
		ST7735_DrawPixel( 124,  2, ST7735_YELLOW);
		ST7735_DrawPixel( 124,  3, ST7735_YELLOW);
		ST7735_DrawPixel( 124,  4, ST7735_YELLOW);
		ST7735_DrawPixel( 124,  5, ST7735_YELLOW);
		ST7735_DrawPixel( 126,  1, ST7735_YELLOW);
		ST7735_DrawPixel( 126,  5, ST7735_YELLOW);
		ST7735_DrawPixel( 127,  2, ST7735_YELLOW);
		ST7735_DrawPixel( 127,  3, ST7735_YELLOW);
		ST7735_DrawPixel( 127,  4, ST7735_YELLOW);
	}
}

void checkButtons(void){
	long hours = GPIO_PORTE_DATA_R&0x02;    // PE1
	long minutes = GPIO_PORTE_DATA_R&0x04;  // PE2
	long mode = GPIO_PORTE_DATA_R&0x08;  		// PE3
	long alarm = GPIO_PORTE_DATA_R&0x20;  	// PE4
	if(mode == 0x08){
		if (modeSet == 1){
			modeSet = 0;
		} else {
			modeSet = 1;
		}
	}
	if(modeSet == 0){
		if(minutes == 0x04){
			increaseMinutes();
		}
		if(hours == 0x02){
			increaseHours();
		}
	} else {
		if(minutes == 0x04){
			increaseAlarmMinutes();
		}
		if(hours == 0x02){
			increaseAlarmHours();
		}
	}
	if(alarm == 0x20){
		if (alarmSet == 1){
			alarmSet = 0;
			ST7735_Line(cx, cy, xVals[alarmMinutes - 1], yVals[alarmMinutes - 1], ST7735_BLACK);
			ST7735_Line(cx, cy, xValh[alarmHours - 1], yValh[alarmHours - 1], ST7735_BLACK);
		} else {
			alarmSet = 1;
		}
	}
}

void checkAlarm(void){
	if (alarmSet == 1) {
		if (currentMinutes == alarmMinutes && currentHours == alarmHours){
			//alarmSet = 0;
			soundAlarm();
		}
	}
}

void drawClock(void){
	ST7735_Circle(cx, cy, radius, ST7735_YELLOW);
	if (currentMinutes != 0) {
		ST7735_Line(cx, cy, xVals[currentMinutes - 1], yVals[currentMinutes - 1], ST7735_BLACK);
	} else {
		ST7735_Line(cx, cy, xVals[59], yVals[59], ST7735_BLACK);
	}
	if (currentHours != 0) {
		ST7735_Line(cx, cy, xValh[currentHours - 1], yValh[currentHours - 1], ST7735_BLACK);
	} else {
		ST7735_Line(cx, cy, xValh[23], yValh[23], ST7735_BLACK);
	}
	if (alarmMinutes != 0) {
		ST7735_Line(cx, cy, xVals[alarmMinutes - 1], yVals[alarmMinutes - 1], ST7735_BLACK);
	} else {
		ST7735_Line(cx, cy, xVals[59], yVals[59], ST7735_BLACK);
	}
	if (alarmHours != 0) {
		ST7735_Line(cx, cy, xValh[alarmHours - 1], yValh[alarmHours - 1], ST7735_BLACK);
	} else {
		ST7735_Line(cx, cy, xValh[23], yValh[23], ST7735_BLACK);
	}
	if (alarmSet == 1) {
		ST7735_Line(cx, cy, xVals[alarmMinutes], yVals[alarmMinutes], ST7735_BLUE);
		ST7735_Line(cx, cy, xValh[alarmHours], yValh[alarmHours], ST7735_BLUE);
	}
	ST7735_Line(cx, cy, xVals[currentMinutes], yVals[currentMinutes], ST7735_YELLOW);
	ST7735_Line(cx, cy, xValh[currentHours], yValh[currentHours], ST7735_YELLOW);
}

void updateDigitalTime(void){
	currentSeconds++;
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
	ST7735_SetCursor(0,1);
	printf("%02d:%02d:%02d", currentHours, currentMinutes, currentSeconds);
}

void updateAlarmTime(void){
	ST7735_SetCursor(0,3);
	printf("%02d:%02d", alarmHours, alarmMinutes);
}

void checkInput(void) {
	if(updateSeconds == 1){
		increaseSeconds();
		updateSeconds = 0;
	}
	if(updateMinutes == 1){
		increaseMinutes();
		updateMinutes = 0;
	}
	if(updateHours == 1){
		increaseHours();
		updateHours = 0;
	}
}

void increaseSeconds(void){
	if(currentSeconds == 59){
		increaseMinutes();
		currentSeconds = 0;
	} else {
		currentSeconds++;
	}
}

void increaseMinutes(void){
	if(currentMinutes == 59){
		increaseHours();
		currentMinutes = 0;
	} else {
		currentMinutes++;
	}
}

void increaseHours(void){
	if(currentHours == 23){
		currentHours = 0;
	} else {
		currentHours++;
	}
}

void increaseAlarmMinutes(void){
	if(alarmMinutes == 59){
		increaseAlarmHours();
		alarmMinutes = 0;
	} else {
		alarmMinutes++;
	}
}

void increaseAlarmHours(void){
	if(alarmHours == 23){
		alarmHours = 0;
	} else {
		alarmHours++;
	}
}