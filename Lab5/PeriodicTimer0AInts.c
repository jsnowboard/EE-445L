// PeriodicTimer0AInts.c
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope or LED connected to PF3-1 for period measurement
// When using the color wheel, the blue LED on PF2 is on for four
// consecutive interrupts then off for four consecutive interrupts.
// Blue is off for: dark, red, yellow, green
// Blue is on for: light blue, blue, purple, white
// Therefore, the frequency of the pulse measured on PF2 is 1/8 of
// the frequency of the Timer0A interrupts.

#include "..//inc//tm4c123gh6pm.h"

#include "..//mymodules//DAC.h"
#include "..//mymodules//music.h"
#include "..//mymodules//portf_init.h"
#include "..//mymodules//Timer0A.h"
#include "..//mymodules//Timer0B.h"
#include "..//mymodules//PLL.h"
#include "..//mymodules//switch.h"

#include <stdint.h>

#define PF1       (*((volatile uint32_t *)0x40025008))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int current_note = 0;
int current_sin = 0;

void TimerATask(void){
  if (current_note < 28 && current_note >= 0){
		DAC_Out(sinWave[current_sin] * MaryHadALittleLamb[current_note]);
		current_sin++;
		if (current_sin == 120){
			current_sin = 0;
		}
	} else {
		current_note = 0;
	}
}

void TimerBTask(void){
  current_note += next_note;
	current_sin = 0;
}

// if desired interrupt frequency is f, Timer0A_Init parameter is busfrequency/f
#define F16HZ (50000000/16)
#define F20KHZ (50000000/20000)
//debug code

int PortFinput[1];
int PortFoutput[1] = {1};

int main(void){ 
  PLL_Init(Bus80MHz);              						  // bus clock at 50 MHz
  PortF_Init(PortFinput, 0, PortFoutput, 1); 
	DAC_Init(2048);
  Timer0A_Init(&TimerATask, F20KHZ);     			  // initialize timer0A (20,000 Hz) Play note every interupt
  Timer0B_Init(&TimerBTask, F16HZ);  						// initialize timer0B (16 Hz) Go to next note
	Switch_Init();
  EnableInterrupts();

  while(1){
    WaitForInterrupt();
  }
}
