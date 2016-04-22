// filename ******** Lab1.c ************** 
// made for the TM4C123G and ST7735 LCD display
// Test the functions in fixed.c by printing the
// TestCases to the LCD.
// 
// Jeremiah Bartlett -- JJB2954
// Ty Winkler -- TAW2328
// 1/25/2016
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
#include "PLL.h"
#include "Switch.h"
#include "SysTick.h"
#include "PWM.h"
#include "../../inc/tm4c123gh6pm.h"
#include "PeriodMeasure.h"

#define PF0       (*((volatile uint32_t *)0x40025004))
#define PF4       (*((volatile uint32_t *)0x40025040))
#define SWITCHES  (*((volatile uint32_t *)0x40025044))
#define SW1       0x10                      // on the left side of the Launchpad board
#define SW2       0x01                      // on the right side of the Launchpad board
#define DELAY10MS 160

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void plotInit(void){
	ST7735_SetCursor(0,0); 
	ST7735_OutString("Current Duty");
	ST7735_PlotClear(0,55);  // range from 0 to 4095
}

int Period;
int startPeriod = 40000;
int rps = 0;
int desiredrps = 40;

int j = 0;
int N = 1;
void plotPoint(void){
	ST7735_PlotPoints(rps, desiredrps);  // Measured temperature
	if((j&(N-1))==0){          // fs sampling, fs/N samples plotted per second
		ST7735_PlotNextErase();  // overwrites N points on same line
	}
	j++;                       // counts the number of samples
}

uint32_t Speed;      // motor speed in 0.1 Hz, 0.025rps 
int32_t E;           // speed error in 0.1 Hz, 0.025rps 
int32_t U;           // duty cycle 40 to 39960
int32_t Xstar = 1200;
void updateMotor(void){
	Xstar = desiredrps/.025;
	Speed = 800000000/Period;
	E = Xstar-Speed;          // 0.1 Hz, 0.025rps
	U = U + (16*E)/64;         // discrete integral 
	if(U < 100) U=100;        // Constrain output if(U>39900) 
	if(U > 39900) U=39900;
	PWM0B_Duty(U);
}

//PB6 - PWM_In (To the OPA2350)
//PB7 - PWM_Out (To the TIP120)
int main(void){
	int status;
	Output_Init();
	status = Switch_Input(); // 0x00 or 0x20
	status = Switch_Input(); // 0x00 or 0x20	
	Board_Init();
	PLL_Init(Bus80MHz);               // bus clock at 80 MHz
	PeriodMeasure_Init();             // initialize 24-bit timer0A in capture mode
	EnableInterrupts();	
	plotInit();
	PWM0B_Init(startPeriod, 39900);         // initialize PWM0, 1000 Hz, 25% duty
	while(1){
		rps = ( 1000 * 80000)/(4 * Period);
		plotPoint();
		status = Board_Input();
		void Switch_WaitForTouch(void);
		switch(status){                    // switches are negative logic on PF0 and PF4
		  case 0x01: Switch_Debounce(); if(desiredrps + 5 <= 50){desiredrps = desiredrps + 5;} break;    // SW1 pressed
		  case 0x10: Switch_Debounce(); if(desiredrps - 5 >= 0){desiredrps = desiredrps - 5;} break;     // SW2 pressed
		  case 0x00: break;  // both switches pressed
		  case 0x11: break;  // neither switch pressed
		  default: break;		 // unexpected return value
		}
		ST7735_SetCursor(0,1);
		ST7735_OutUDec(U/400);
		ST7735_OutChar(' ');
		ST7735_OutUDec(U);
		ST7735_OutChar(' ');
		ST7735_OutUDec(t2);
		ST7735_SetCursor(0,2);
		ST7735_OutString("RPS"); 
		ST7735_SetCursor(4,2);
		ST7735_OutUDec(rps);
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		ST7735_SetCursor(10,2); 
		ST7735_OutString("DRPS"); 
		ST7735_SetCursor(15,2);
		ST7735_OutUDec(desiredrps);
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		updateMotor();
  }
}
