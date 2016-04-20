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
#include "PWM.h"
#include "../../inc/tm4c123gh6pm.h"
#include "PeriodMeasure.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//PB6 - PWM_In (To the OPA2350)
//PB7 - PWM_Out (To the TIP120)
int main(void){
	Output_Init();
  PLL_Init(Bus80MHz);               // bus clock at 80 MHz
  PeriodMeasure_Init();            // initialize 24-bit timer0A in capture mode
  EnableInterrupts();	
	PWM0B_Init(1000, 900);         // initialize PWM0, 1000 Hz, 25% duty
//  PWM0B_Duty(4000);    // 10%
//  PWM0B_Duty(10000);   // 25%
//  PWM0B_Duty(30000);   // 75%
	
//  PWM0B_Init(4000, 2000);         // initialize PWM0, 10000 Hz, 50% duty
//  PWM0B_Init(1000, 900);          // initialize PWM0, 40000 Hz, 90% duty
//  PWM0B_Init(1000, 100);          // initialize PWM0, 40000 Hz, 10% duty
//  PWM0B_Init(40, 20);             // initialize PWM0, 1 MHz, 50% duty
	while(1){
    WaitForInterrupt();
  }
}
