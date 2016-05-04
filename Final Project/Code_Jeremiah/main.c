// Main.c
// Built to test hardware
// 4/24/2016 
// Jeremiah Bartlett And Ty Winkler

#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
//#include "portf_init.h"
#include "74HC595.h"
#include "74HC165.h"
#include "../../inc/tm4c123gh6pm.h"
#include "Switch.h"
#include "SysTick.h"

#define PF0  (*((volatile unsigned long *)0x40025004))
#define PF1  (*((volatile unsigned long *)0x40025008))
#define PF2  (*((volatile unsigned long *)0x40025010))
#define PF3  (*((volatile unsigned long *)0x40025020))
#define PF4  (*((volatile unsigned long *)0x40025040))
#define PF5  (*((volatile unsigned long *)0x40025080))
#define PF6  (*((volatile unsigned long *)0x40025100))
#define PF7  (*((volatile unsigned long *)0x40025200))
	

#define PA7  (*((volatile unsigned long *)0x40004200))

uint16_t code;
uint32_t status;
//int outpins[]= {1,2};

void PortA_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000001;     // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
  GPIO_PORTA_AMSEL_R &= ~0x80;      // 3) disable analog on PA7
  GPIO_PORTA_PCTL_R &= ~0xF0000000; // 4) PCTL GPIO on PA7
  GPIO_PORTA_DIR_R |= 0x80;         // 5) PA7 out
  GPIO_PORTA_AFSEL_R &= ~0x80;      // 6) disable alt funct on PA7
  GPIO_PORTA_DEN_R |= 0x80;         // 7) enable digital I/O on PA7
}

int i = 0;
int main(void) {
	PLL_Init(Bus50MHz); //set up PLL
	SysTick_Init();
	Port_OutInit(); // set up 74HC595 to output serial data 
	PortA_Init();
	code = 0xFF;
	Port_Out(code); // output
	SysTick_Wait(100);
  code=0x00;
	Port_Out(code);
	code = 0x01;
	
	while(1) {
		//PA7=~PA7;
		Port_Out(code);
		code = code<<1;
		SysTick_Wait10ms(1000);
		if(code == 0x0000){
			code = 0x0001;
		}
//		Port_Out(0x10);
//		Port_Out(0x20);
//		Port_Out(0x40);
//		Port_Out(0x01);
//		Port_Out(0x04);
	}
}
