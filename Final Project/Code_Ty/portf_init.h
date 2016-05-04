// portf_init.h
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

// ***************** portf_init ****************
// Activate Portf input and output pins
// Inputs:  inPins[] - array of inputs 0 - 4
//					outPins[] - array of inputs 0 - 4
// Outputs: none
void PortF_Init(int inPins[], int numInPins, int outPins[], int numOutPins);

// ***************** pinstohex ****************
// Creates the input and outputs
// Inputs:  pins[] - array of inputs 0 - 7
// Outputs: int - pin number
int pinstohex(int pins[], int numPins);
