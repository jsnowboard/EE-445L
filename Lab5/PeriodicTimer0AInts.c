// Runs on TM4C123
// Ty Winkler & Jeremiah
// Feb 23, 2016
// Modified Feb 29, 2016

#include "..//inc//tm4c123gh6pm.h"
#include <stdint.h>
#include "..//mymodules//PLL.h"
#include "..//mymodules//Timer0A.h"
#include "..//mymodules//switch.h"
#include "..//mymodules//DAC.h"
#include "..//mymodules//notes.c"
#include "..//mymodules//music.h"

int count = 0;
int current_note = 0;
int time = 0;
int nexttime = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void playMusic(){
	DAC_Out(wave[count] * 2);
	if(count < 32){
		count++;
	} else {
		count = 0;
	}
}

void TimerATask(void){
	if(playing == 1){
		playMusic();
		time++;
		if(current_note > 34){
			current_note = 0;
		}
		nexttime = MaryHadALittleLambTime[current_note];
		if(time == 1000 * nexttime){
			TIMER0_TAILR_R = (5000000/MaryHadALittleLamb[current_note]) - 1;
			current_note += next_note;
			time = 0;
		}
	}
}

int main(void){ 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
  DAC_Init(1024);
  Switch_Init();
  Timer0A_Init(&TimerATask, C4);  // initialize timer0A (16 Hz)
  EnableInterrupts();
  while(1){
    WaitForInterrupt();
  }
}
