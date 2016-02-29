// DAC.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016
// Modified by Jeremiah Feb 29, 2016

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

//********DAC_Chord*****************
// Create a chord to output via DAC_Out
// inputs:  wave voltages for multiple notes
// outputs: voltage output
unsigned int DAC_Chord(unsigned int wave1, unsigned int wave2);
