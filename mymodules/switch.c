// switch.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "switch.h"
#include "music.h"
#include "..//inc//tm4c123gh6pm.h"

#include <stdint.h>

int rewind = 0;
int playing = 1;

void Switch_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x08;           // Port E clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles
  GPIO_PORTD_DIR_R &= ~0x07;        // PD2,1,0 input 
  GPIO_PORTD_AFSEL_R &= ~0x17;      // not alternative
  GPIO_PORTD_AMSEL_R &= ~0x17;      // no analog
  GPIO_PORTD_PCTL_R &= ~0x00000FFF; // bits for PD2,PD1,PD0
  GPIO_PORTD_DEN_R |= 0x07;         // enable PD2,PD1,PD0
	
	GPIO_PORTD_IS_R &= ~0x07;     
  GPIO_PORTD_IBE_R &= ~0x07;    
  GPIO_PORTD_IEV_R &= ~0x07;
	GPIO_PORTD_ICR_R = 0x07;
	GPIO_PORTD_IM_R |= 0x07;
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00F)|0x00000A00; // (g) priority 5
	NVIC_EN0_R = 0x08;	
	rewind = 0;
}

void GPIOPortD_Handler(void){
	if(GPIO_PORTD_ICR_R&0x01){
		Switch_Handler1();
	}
	if(GPIO_PORTD_ICR_R&0x02){
		Switch_Handler2();
	}
	if(GPIO_PORTD_ICR_R&0x04){
		Switch_Handler3();
	}
}

//Play
void Switch_Handler1(void){
	GPIO_PORTD_ICR_R = 0x01;
	//TIMER0_CTL_R |= TIMER_CTL_TAEN;
  playing = 1;
}

//Stop
void Switch_Handler2(void){
	GPIO_PORTD_ICR_R = 0x02;
	//TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	playing = 0;
}

void Switch_Handler3(void){
	GPIO_PORTD_ICR_R = 0x04;
	if (rewind == 1){
		next_note = 1;
		rewind = 0;
	} else {
		next_note = -1;
		rewind = 1;
	}
}
