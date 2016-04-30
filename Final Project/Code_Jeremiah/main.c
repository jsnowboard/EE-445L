// Main.c
// Built to test hardware
// 4/24/2016 
// Jeremiah Bartlett And Ty Winkler

#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
#include "PortF.h"
#include "74HC595.h"
#include "74HC165.h"
#include "../../inc/tm4c123gh6pm.h"
#include "Switch.h"
#include "SysTick.h"

uint8_t code;
uint32_t status;
int main(void) {
	
	PLL_Init(Bus50MHz); //set up PLL
	//Port_InInit(); //set up 74HC165 for input serial data
	//PortF_Init();
	//Toggle();
	//Toggle();
	//SysTick_Init();
	code=0x02;
	Port_OutInit(); // set up 74HC595 to output serial data 
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(0);// output
	Port_Out(code);// output
	
	while(1) {
		
//		code=0x03;
//		Port_Out(code);
//		code=0x01;
//		Port_Out(code);
//		SysTick_Wait10ms(4);
//		Port_In();
	}
}
