// portf_init.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "..//inc//tm4c123gh6pm.h"
#include "portf_init.h"
#include <stdint.h>

void PortF_Init(int inPins[], int numInPins, int outPins[], int numOutPins){
	int input = pinstohex(inPins, numInPins);
	int output = pinstohex(outPins, numOutPins);
	
	SYSCTL_RCGCGPIO_R |= 0x20;       				// activate port F
  while((SYSCTL_PRGPIO_R&0x0020) == 0){}; // ready?
	GPIO_PORTF_DIR_R &= ~input;      				// set inputs
  GPIO_PORTF_DIR_R |= output;        			// set outputs
  GPIO_PORTF_AFSEL_R &= ~(output + input);// disable alt functions
  GPIO_PORTF_DEN_R |= output + input;     // enable digital I/O
																					// configure as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          				// disable analog functionality on PF
}

int pinstohex(int pins[], int numPins){
	int number = 0;
	for (int i = 0; i < numPins; i++){
		number += pins[i] + 1;
	}
	return number;
}
