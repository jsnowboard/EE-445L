// filename ******** fixed.c ************** 
// Jeremiah Bartlett -- JJB2954
// Ty Winkler -- TAW2328
// Lab 1
// TA: Ryan Chow
// Initial Creation: 1/25/2016
// Last Revision: 1/31/2016
// Lab Number: 16325

#include "fixed.h"
#include "ST7735.h"

//Absolute min and max points on LCD
int32_t MinXplot=0;
int32_t MaxXplot=127;
int32_t MinYplot=32;
int32_t MaxYplot=159;

void BookExamples(void){ // examples from the book
  int8_t cc = 0x56; // (‘V’)
  int32_t xx = 100;
  int16_t yy = -100;
  float zz = 3.14159265;
  printf("Hello world\n");      //Hello world
  printf("cc = %c %d\n",cc,cc);  //cc = V 86
  printf("cc = %#x\n",cc);      //cc = 0x56
  printf("xx = %c %d\n",xx,xx);  //xx = d 100
  printf("xx = %#x\n",xx);      //xx = 0x64
  printf("yy = %d\n",yy);        //yy = -100
  printf("%#x   \n",yy);        //yy = 0xffffff9c
  printf("%e \n",zz);            //zz = 3.14159e+00
  printf("%E \n",zz);            //zz = 3.14159E+00
  printf("%f     \n",zz);        //zz = 3.14159
  printf("%g     \n",zz);        //zz = 3.14159 (shorter of two, either f or e)
  printf("%3.2f     \n",zz);    //zz =  3.14
}

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " *.***"
  2345    " 2.345"  
 -8100    "-8.100"
  -102    "-0.102" 
    31    " 0.031" 
-12345    " *.***"
 */ 
void ST7735_sDecOut3(int32_t n){
	int negative = 0;
	if (n < 0) {
		negative = 1;
		n = n*-1;
	}
	int firstDigit = n/1000;
	int secondDigit = (n/100)%10;
	int thirdDigit = (n/10)%10;
	int fourthDigit = n%10;
	if (n > 9999 || n < -9999){
		printf(" *.***\n");
	} else {
		if (negative){
			printf("-");
		} else {
			printf(" ");
		}
		printf("%d",firstDigit);
		printf(".");
		printf("%d",secondDigit);
		printf("%d",thirdDigit);
		printf("%d\n",fourthDigit);
	}
}

void ST7735_sDecOut2(int32_t n){
	int negative = 0;
	if (n < 0) {
		negative = 1;
		n = n*-1;
	}
	int firstDigit = n/1000;
	int secondDigit = (n/100)%10;
	int thirdDigit = (n/10)%10;
	int fourthDigit = n%10;
	if (n > 9999 || n < -9999){
		printf(" *.***\n");
	} else {
		if (negative){
			printf("-");
		} else {
			printf(" ");
		}
		printf("%d",firstDigit);
		printf("%d",secondDigit);
		printf(".");
		printf("%d",thirdDigit);
		printf("%d\n",fourthDigit);
	}
}

/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     2	  "  0.01"
    64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n) {
	if(n>=256000)
	{
		printf("***.**\n");
	}
	else
	{
		int start=0;
		int hundreds=(n/25600)%10;
		int tens=(n/2560)%10;
		int ones=(n/256)%10;
		int tenths=(n*10/256)%10;
		int hundredths=(n*100/256)%10;
		int thousandths=(n*1000/256)%10;
		if(hundreds>0){
			printf("%d", hundreds);
			start=1;
		}
		else{
			printf(" ");
		}
		if(tens>0||start>0){
			printf("%d", tens);
		}
		else{
			printf(" ");
		}
		printf("%d", ones);
		printf(".");
		printf("%d",tenths);
		if(thousandths>=5)
		{
			hundredths=hundredths+1;
		}
		printf("%d",hundredths);
		printf("%d", thousandths);
		printf("\n");
	}
}	

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY) {
	ST7735_InitR(INITR_GREENTAB);
	ST7735_PlotClear(minY, maxY);
	ST7735_DrawString(0, 0, title, ST7735_YELLOW);
	printf("%s", title);
	MinXplot=minX;
	MinYplot=minY+MinYplot;
	MaxXplot=maxX;
	MaxYplot=maxY+MinYplot;
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[]) {

	int n;
	for(n=0; n < num; n++){
		//For the Kentec
		//LCD_DrawPixel(bufX[n], bufY[n], 0xFFE0);
		//For the SS7735
		//ST7735_XYPoint(bufX[n], bufY[n]);
//		int yRange=(MaxYplot-MinYplot);
		int y=MinYplot+bufY[n];
//		int xRange=(MaxXplot-MinXplot);
		int x=MinXplot+bufX[n];
		if(bufX[n]<=MaxXplot && bufY[n]<=MinYplot)
		{
			ST7735_DrawPixel(x, y, ST7735_BLUE);
		}
	}
}

//************* ST7735_Line********************************************
//  Draws one line on the ST7735 color LCD
//  Inputs: (x1,y1) is the start point
//          (x2,y2) is the end point
// x1,x2 are horizontal positions, columns from the left edge
//               must be less than 128
//               0 is on the left, 126 is near the right
// y1,y2 are vertical positions, rows from the top edge
//               must be less than 160
//               159 is near the wires, 0 is the side opposite the wires
//        color 16-bit color, which can be produced by ST7735_Color565() 
// Output: none
void ST7735_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	int currentx = x1;
	int currenty = y1;
	int slopenum = y2 - y1;
	int slopeden = x2 - x1;
	int reduce;
	if (slopeden < slopenum){
		reduce = slopeden;
	} else {
		reduce = slopenum;
	}
	while (slopenum%reduce != 0 && slopeden%reduce != 0 && reduce > 1){
		reduce--;
	}
	slopenum = slopenum / reduce;
	slopeden = slopeden / reduce;
	while(currentx != x2 && currenty != y2){
		ST7735_DrawPixel(currentx, currenty, color);
		currentx += slopeden;
		currenty += slopenum;
	} 
}
