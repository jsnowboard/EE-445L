// switch.h
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

extern int rewind;
extern int playing;

// ***************** Switch_Init ****************
// Initilize a Switch with different functions
// Inputs: none
// Outputs: none
void Switch_Init(void);

// ***************** Switch_Handler1 ****************
// Handles the switch trigger Handler 1
// Inputs: none
// Outputs: none
void Switch_Handler1(void);

// ***************** Switch_Handler2 ****************
// Handles the switch trigger Handler 2
// Inputs: none
// Outputs: none
void Switch_Handler2(void);

// ***************** Switch_Handler3 ****************
// Handles the switch trigger Handler 3
// Inputs: none
// Outputs: none
void Switch_Handler3(void);
