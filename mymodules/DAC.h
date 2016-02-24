// DAC.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

//********DAC_Init*****************
// Initialize TLV5616CP 12-bit DAC
// inputs: initial voltage output
// outputs:none
void DAC_Init(unsigned int output);

//********DAC_Out*****************
// Send data to TLV5616CP 12-bit DAC
// inputs:  voltage output
// outputs: none
unsigned int DAC_Out(unsigned int output);
