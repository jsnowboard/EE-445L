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
#include "SysTick.h"

#define PF0  (*((volatile unsigned long *)0x40025004))
#define PF1  (*((volatile unsigned long *)0x40025008))
#define PF2  (*((volatile unsigned long *)0x40025010))
#define PF3  (*((volatile unsigned long *)0x40025020))
#define PF4  (*((volatile unsigned long *)0x40025040))
#define PF5  (*((volatile unsigned long *)0x40025080))
#define PF6  (*((volatile unsigned long *)0x40025100))
#define PF7  (*((volatile unsigned long *)0x40025200))

int PAWN[32] = {1,12,18};
int KNIGHT[32] = {2,6,8,11,15,23,27,29,31};
int BISHOP[32] = {1,2,7,13,17,18,19,20,21,25,30};
int ROOK[32] = {3,5,9,12,14,19,24,26,32};
int QUEEN[32] = {1,3,5,7,9,12,14,19,24,26,32};
int KING[32] = {2,3,12,13,14,18,19,20,24,25,26};

int outputArray[32];
int myOutput;
int i;

void arrayOutput(int output[], int arraySize){
	myOutput = 0;
	for(i = 0; i < arraySize; i++){
		switch(output[i]){
			case 0: break;
			case 1: myOutput += 0x001; break;
			case 2: myOutput += 0x002; break;
			case 3: myOutput += 0x004; break;
			case 4: myOutput += 0x008; break;
			case 5: myOutput += 0x010; break;
			case 6: myOutput += 0x020; break;
			case 7: myOutput += 0x040; break;
			case 8: myOutput += 0x080; break;
			case 9: myOutput += 0x100; break;
			case 10: myOutput += 0x200; break;
			case 11: myOutput += 0x400; break;
			case 12: myOutput += 0x800; break;
			case 13: myOutput += 0x1000; break;
			case 14: myOutput += 0x2000; break;
			case 15: myOutput += 0x4000; break;
			case 16: myOutput += 0x8000; break;
			case 17: myOutput += 0x10000; break;
			case 18: myOutput += 0x20000; break;
			case 19: myOutput += 0x40000; break;
			case 20: myOutput += 0x80000; break;
			case 21: myOutput += 0x100000; break;
			case 22: myOutput += 0x200000; break;
			case 23: myOutput += 0x400000; break;
			case 24: myOutput += 0x800000; break;
			case 25: myOutput += 0x1000000; break;
			case 26: myOutput += 0x2000000; break;
			case 27: myOutput += 0x4000000; break;
			case 28: myOutput += 0x8000000; break;
			case 29: myOutput += 0x10000000; break;
			case 30: myOutput += 0x20000000; break;
			case 31: myOutput += 0x40000000; break;
			case 32: myOutput += 0x80000000; break;
			default: break;
		}
	}
	Port_Out(myOutput);
}

uint32_t status;
int mode = 0;
int specialOut = 1;
int state = 0;
int main(void) {
	PLL_Init(Bus50MHz); //set up PLL
	Board_Init(); //PF0, PF4
	Port_OutInit(); // set up 74HC595 to output serial data 
	
	Port_Out(0xAAAAAAAA);
	SysTick_Wait10ms(100);
	Port_Out(0x00000000);
	SysTick_Wait10ms(100);
	
	while(1) {
		switch(status){                    // switches are negative logic on PF0 and PF4
		  case 0x01: 
				Switch_Debounce(); 
				if(state < 6){
					state++;
				} else { 
					state = 0;
				}
				break;    // SW1 pressed
				
		  case 0x10: 
				Switch_Debounce(); 
				if(mode > 0){
					state--;
				} else {
					state = 5;
				}
				break;     // SW2 pressed
				
		  case 0x00: 
				Switch_Debounce(); 
				if(mode < 2){
					mode++;
				} else {
					mode = 0;
				}
				break;  // both switches pressed
				
		  case 0x11: break;  // neither switch pressed
		  default: break;		 // unexpected return value
		}
		
		switch(mode){
			case 0:
				switch(state){
					case 0: arrayOutput(PAWN,3); break;
					case 1: arrayOutput(KNIGHT,9); break;
					case 2: arrayOutput(BISHOP,11); break;
					case 3: arrayOutput(ROOK, 9); break;
					case 4: arrayOutput(QUEEN, 11); break;
					case 5: arrayOutput(KING, 11); break;
					default: break;
				}
				break;
				
			case 1:
				Port_Out(specialOut);
				specialOut = specialOut<<1;
				if(specialOut >= 0x80000000){
					specialOut = 1;
				}
				break;
				
			case 2:
				Port_Out(specialOut);
				specialOut++;
				if(specialOut >= 0xFFFFFFFF){
					specialOut = 1;
				}
				break;
			
			default:
				break;
		}
	}
}
