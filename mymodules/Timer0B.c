// Timer0A.c
// Runs on LM4F120/TM4C123
// Use Timer0A in periodic mode to request interrupts at a particular
// period.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015
   Volume 1, Program 9.8

  "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 7.5, example 7.6

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
#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"
#include "Timer0B.h"



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void (*TimerBInitTask)(void);   // user function


// ***************** Timer0A_Init ****************
// Activate TIMER0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq), 32 bits
// Outputs: none
void Timer0B_Init(void(*task)(void), uint32_t period){long sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   			// 0) activate TIMER0
  TimerBInitTask = task;          			// user function
  TIMER0_CTL_R = 0x00000000;    			// 1) disable TIMER0B during setup
  TIMER0_CFG_R = TIMER_CFG_16_BIT;    // 2) configure for 16-bit mode
  TIMER0_TBMR_R = 0x00000002;   			// 3) configure for periodic mode, default down-count settings
  TIMER0_TBILR_R = period-1;    			// 4) reload value
  TIMER0_TBPR_R = 0;            			// 5) bus clock resolution
  TIMER0_ICR_R = TIMER_ICR_TBTOCINT;  // 6) clear TIMER0B timeout flag
  TIMER0_IMR_R = TIMER_IMR_TBTOIM;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFFFF00)|0x00000040; // 8) bits 5-7
  NVIC_EN0_R = 1<<20;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  EndCritical(sr);
}

void Timer0B_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*TimerBInitTask)();                // execute user task
}
