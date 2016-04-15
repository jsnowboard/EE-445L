#include "temperature.h"

unsigned long tempValue;
unsigned long tempValuePrev;
long adcOffset = 0;
long tempOffset = 0;

int const Tdata[53] = {4000,4000,3940,3880,3820,3760,3700,3640,3580,3520,3460,		
     3400,3340,3280,3220,3160,3100,3040,2980,2920,2860,		
     2800,2740,2680,2620,2560,2500,2440,2380,2320,2260,		
     2200,2140,2080,2020,1960,1900,1840,1780,1720,1660,		
     1600,1540,1480,1420,1360,1300,1240,1180,1120,1060,1000,1000};		

int const Rdata[53]={5716,5716,5827,5939,6055,6173,6294,6418,6544,6674,6807,								
     6943,7082,7225,7371,7520,7673,7830,7991,8156,8325,								
     8498,8675,8857,9043,9234,9430,9631,9837,10048,10265,								
     10487,10715,10949,11188,11435,11687,11947,12213,12486,12767,								
     13055,13351,13654,13966,14287,14616,14954,15302,15659,16026,16403,16403};								

int const ADCdata[53]={0,163,192,221,251,282,313,345,377,410,443,							
     478,512,548,584,621,658,696,735,775,815,							
     856,898,941,984,1028,1073,1119,1166,1213,1261,							
     1311,1361,1412,1464,1516,1570,1625,1680,1737,1794,							
     1853,1913,1973,2035,2097,2161,2225,2291,2358,2425,2494,4096};							

unsigned long adcToTemp(unsigned long adcValue){
	int i = 0;
	adcValue += adcOffset;
	if(adcValue == 1024){
		return Tdata[0] + tempOffset;
	}
	for(i = 0; i < 52; i++){
		if(adcValue == ADCdata[i]){
			return Tdata[51 - i] + tempOffset;
		}else if(adcValue > ADCdata[i] && adcValue < ADCdata[i+1]){
			tempValue = Tdata[51-i];
			tempValuePrev = Tdata[50-i];
			return tempValue + (tempValuePrev - tempValue)*(adcValue%19)/19 + tempOffset;
		}
	}
	return 0;
}
