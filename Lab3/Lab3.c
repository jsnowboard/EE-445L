// Lab3.c
// made for the TM4C123G and ST7735 LCD display
// Test the functions in fixed.c by printing the
// TestCases to the LCD.
// 
// Jeremiah Bartlett -- JJB2954
// Ty Winkler -- TAW2328
// Initial Creation: 1/25/2016
// Last Revision: 1/31/2016
// Lab Number: 16325
//
// Lab 3
// TA: Ryan Chow

#include <stdio.h>
#include <stdint.h>
#include "lab3.h"
#include "ST7735.h"

#define PD1       (*((volatile uint32_t *)0x40007008))

const unsigned short Wave[32] = {  
  125,143,159,175,189,200,208,213,215,213,208,
  200,189,175,159,143,125,107,91,75,61,50,
  42,37,35,37,42,50,61,75,91,107
};  


void soundAlarm(void){
	for(int j = 0; j <= 100; j++){
		for(int i = 0; i <= 240; i++){
			PD1 = Wave[i];
		}
	}
}
