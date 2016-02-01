// filename ******** Lab1.c ************** 
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
//Lab 1
//TA: Ryan Chow
//
// hardware connections
// **********ST7735 TFT and SDC*******************
// ST7735
// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdio.h>
#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"

// const will place these structures in ROM
struct outTestCase{       // used to test routines
  unsigned long InNumber;       // test input number
  char OutBuffer[10];           // Output String  
};

typedef const struct outTestCase outTestCaseType;
outTestCaseType outTests3[16]={ 
{     0,  "  0.00" }, //      0/256 = 0.00  
{     4,  "  0.01" }, //      4/256 = 0.01  
{    10,  "  0.03" }, //     10/256 = 0.03
{   200,  "  0.78" }, //    200/256 = 0.78
{   254,  "  0.99" }, //    254/256 = 0.99
{   505,  "  1.97" }, //    505/256 = 1.97
{  1070,  "  4.17" }, //   1070/256 = 4.17
{  5120,  " 20.00" }, //   5120/256 = 20.00
{ 12184,  " 47.59" }, //  12184/256 = 47.59
{ 26000,  "101.56" }, //  26000/256 = 101.56
{ 32767,  "127.99" }, //  32767/256 = 127.99
{ 32768,  "128.00" }, //  32768/256 = 128
{ 34567,  "135.02" }, //  34567/256 = 135.02
{123456,  "482.25" }, // 123456/256 = 482.25
{255998,  "999.99" }, // 255998/256 = 999.99
{256000,  "***.**" }  // error
};

outTestCaseType outTests8[11]={
{		   0,	  "  0.00"},
{			 2,	  "  0.01"},
{			64,	  "  0.25"},
{		 100,	  "  0.39"},
{		 500,	  "  1.95"},
{		 512,	  "  2.00"},
{		5000,	  " 19.53"},
{	 30000,	  "117.19"},
{	255997,	  "999.99"},
{ 128798,		"503.12"},
{	256000,	  "***.**"}
};

int32_t pointXTest1[3]={
	44,
	45,
	101
};
int32_t pointYTest1[3]={
	49,
	49,
	52
};

unsigned int Errors,AnError;
char Buffer[10];

int main(void){
  unsigned int i;
	Output_Init();              // initialize output device
  Output_Color(ST7735_YELLOW);
	Errors = 0;
	ST7735_XYplotInit("LCD_Output", 0, 100, 0, 100);
	ST7735_XYplot(3,pointXTest1 , pointYTest1);
	for(i=0; i<11; i++){
		
		//------------LCD Screen Tests----------------
		//BookExamples();	//Test function used to make sure the LCD will print with printf
		//ST7735_sDecOut3(outTests3[i].InNumber);
		//ST7735_uBinOut8(outTests8[i].InNumber);
		//ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY);
		//ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]);
		//--------------------------------------------
		
		if(strcmp(Buffer, outTests3[i].OutBuffer)){
			Errors++;
			AnError = i;
		}
	}
	for(;;) {} /* wait forever */
}
