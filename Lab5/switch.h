// switch.h
// Runs on TM4C123
// Jeremiah Bartlett
// Feb 24, 2016

//********Switch_Init*****************
// Initialize TLV5616CP 12-bit DAC
// inputs: initial voltage output
// outputs:none
void Switch_Init(unsigned int numberSwitches, unsigned int output);