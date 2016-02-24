// DAC.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "DAC.h"
#include "..//inc//tm4c123gh6pm.h"

#include <stdint.h>

void DAC_Init(unsigned int output){
  volatile unsigned long delay;
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_SSI0;  // activate SSI0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  delay = SYSCTL_RCGC2_R;               // allow time to finish activating
  GPIO_PORTA_AFSEL_R |= 0x2C;           // enable alt funct on PA2 SSIOClk, PA3 SSIOFss, PA5 SSIOTx
  GPIO_PORTA_DEN_R |= 0x2C;             // enable digital I/O on PA2 SSIOClk, PA3 SSIOFss, PA5 SSIOTx
  SSI0_CR1_R &= ~SSI_CR1_SSE;           // disable SSI
  SSI0_CR1_R &= ~SSI_CR1_MS;            // master mode (default setting)
                                        // clock divider for 3 MHz SSIClk
  SSI0_CPSR_R = (SSI0_CPSR_R&~SSI_CPSR_CPSDVSR_M)+2;
  SSI0_CR0_R &= ~(SSI_CR0_SCR_M |       // SCR = 0 (3 Mbps data rate) (default setting)
                  SSI_CR0_SPH |         // SPH = 0 (default setting)
                  SSI_CR0_SPO);         // SPO = 0 (default setting)
                                        // FRF = Freescale format (default setting)
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_FRF_M)+SSI_CR0_FRF_MOTO;
                                        // DSS = 16-bit data
  SSI0_CR0_R = (SSI0_CR0_R&~SSI_CR0_DSS_M)+SSI_CR0_DSS_16;
  SSI0_DR_R = output;                   // load 'data' into transmit FIFO
  SSI0_CR1_R |= SSI_CR1_SSE;            // enable SSI
}

unsigned int DAC_Out(unsigned int output){
  while((SSI0_SR_R&SSI_SR_TNF)==0){};		// wait until room in FIFO
  SSI0_DR_R = output;                		// data out
  while((SSI0_SR_R&SSI_SR_RNE)==0){};		// wait until response
  return SSI0_DR_R;                  		// acknowledge response
}
