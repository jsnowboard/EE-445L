// Main.c
// Built to test hardware
// 4/24/2016 
// Jeremiah Bartlett And Ty Winkler

#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
#include "74HC595.h"
#include "74HC165.h"
#include "../../inc/tm4c123gh6pm.h"
#include "Switch.h"

uint8_t code;
uint32_t status;
int main(void) {
	
	PLL_Init(Bus50MHz); //set up PLL
	Port_InInit(); //set up 74HC165 for input serial data
	code=Port_In();
	Port_OutInit(); // set up 74HC595 to output serial data 
	
	while(1) {
		Port_Out(code);// output
		code=Port_In();
		
	}
}
