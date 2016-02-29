// switch.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "switch.h"
#include "music.h"
#include "..//inc//tm4c123gh6pm.h"

#include <stdint.h>

int rewind = 0;

void Switch_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x10;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTE_DIR_R &= ~0x07;        // PE2,1,0 input 
  GPIO_PORTE_AFSEL_R &= ~0x17;      // not alternative
  GPIO_PORTE_AMSEL_R &= ~0x17;      // no analog
  GPIO_PORTE_PCTL_R &= ~0x000F0FFF; // bits for PE2,PE1,PE0
  GPIO_PORTE_DEN_R |= 0x07;         // enable PE2,PE1,PE0
	
	GPIO_PORTE_IS_R &= ~0x07;     
  GPIO_PORTE_IBE_R &= ~0x07;    
  GPIO_PORTE_IEV_R &= ~0x07;
	GPIO_PORTE_ICR_R = 0x07;
	GPIO_PORTE_IM_R |= 0x07;
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
	NVIC_EN0_R = 0x10;	
	rewind = 0;
}

void GPIOPortE_Handler(void){
	if(GPIO_PORTE_ICR_R&0x01){
		Switch_Handler1();
	}
	if(GPIO_PORTE_ICR_R&0x02){
		Switch_Handler2();
	}
	if(GPIO_PORTE_ICR_R&0x04){
		Switch_Handler3();
	}
}

//Play
void Switch_Handler1(void){
	GPIO_PORTE_ICR_R = 0x01;
	TIMER0_CTL_R |= TIMER_CTL_TAEN;
	TIMER0_CTL_R |= TIMER_CTL_TBEN;
}

//Stop
void Switch_Handler2(void){
	GPIO_PORTE_ICR_R = 0x02;
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	TIMER0_CTL_R &= ~TIMER_CTL_TBEN;
}

void Switch_Handler3(void){
	GPIO_PORTE_ICR_R = 0x04;
	if (rewind == 1){
		next_note = 1;
		rewind = 0;
	} else {
		next_note = -1;
		rewind = 1;
	}
}
