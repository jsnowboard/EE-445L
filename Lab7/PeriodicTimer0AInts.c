// Runs on TM4C123
// Ty Winkler & Jeremiah
// Feb 23, 2016
// Modified Feb 29, 2016

#include "..//inc//tm4c123gh6pm.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "..//mymodules//PLL.h"
#include "..//mymodules//Timer0A.h"
#include "constants.h"
#include "74HC165.h"
#include "74HC595.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int playerTurn = 1; //1 for player 1 or -1 for player 2
int pieceMoving = 0;

int chessboard[8][8];

int lightArrayCount = 0;
int lightArray[64];

int lightState = 0;
int currentState = 0;

void TimerATask(void){

}

void showMoves(int piece, int x, int y){
	lightArray[lightArrayCount] = 8 * y + x;
	lightArrayCount++;
	if(piece == KING){
		if(x-1 >= 0 && y-1 >= 0){
			if(chessboard[x-1][y-1] <= 0){
				lightArray[lightArrayCount] = 8 * (y-1) + (x-1);
				lightArrayCount++;
			}
		}
		if(y-1 >= 0){
			if(chessboard[x][y-1] <= 0){
				lightArray[lightArrayCount] = 8 * (y-1) + x;
				lightArrayCount++;
			}
		}
		if(x+1 < 8 && y-1 <= 0){
			if(chessboard[x+1][y-1] <= 0){
				lightArray[lightArrayCount] = 8 * (y-1) + (x+1);
				lightArrayCount++;
			}
		}
		if(x-1 >= 0){
			if(chessboard[x-1][y] <= 0){
				lightArray[lightArrayCount] = 8 * y + (x-1);
				lightArrayCount++;
			}
		}
		if(x+1 >= 0){
			if(chessboard[x+1][y] <= 0){
				lightArray[lightArrayCount] = 8 * y + (x+1);
				lightArrayCount++;
			}
		}
		if(x-1 >= 0 && y+1 >= 0){
			if(chessboard[x-1][y+1] <= 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x-1);
				lightArrayCount++;
			}
		}
		if(y+1 >= 0){
			if(chessboard[x][y+1] <= 0){
				lightArray[lightArrayCount] = 8 * (y+1) + x;
				lightArrayCount++;
			}
		}
		if(x+1 >= 0 && y+1 >= 0){
			if(chessboard[x+1][y+1] <= 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x+1);
				lightArrayCount++;
			}
		}
	} 
	else if(piece == QUEEN){
		for(int count = 1; count < 8; count++){
			if(x-count >= 0 && y-count >= 0){
				if(chessboard[x-count][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y-count] > 0){
					break;
				}else if(chessboard[x-count][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(y-count >= 0){
				if(chessboard[x][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + x;
					lightArrayCount++;
				}else if(chessboard[x][y-count] > 0){
					break;
				}else if(chessboard[x][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + x;
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0 && y-count >= 0){
				if(chessboard[x+count][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y-count] > 0){
					break;
				}else if(chessboard[x+count][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x-count >= 0){
				if(chessboard[x-count][y] == 0){
					lightArray[lightArrayCount] = 8 * y + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y] > 0){
					break;
				}else if(chessboard[x-count][y] < 0){
					lightArray[lightArrayCount] = 8 * y + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0){
				if(chessboard[x+count][y] == 0){
					lightArray[lightArrayCount] = 8 * y + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y] > 0){
					break;
				}else if(chessboard[x+count][y] < 0){
					lightArray[lightArrayCount] = 8 * y + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x-count >= 0 && y+count >= 0){
				if(chessboard[x-count][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y+count] > 0){
					break;
				}else if(chessboard[x-count][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(y+count >= 0){
				if(chessboard[x][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + x;
					lightArrayCount++;
				}else if(chessboard[x][y+count] > 0){
					break;
				}else if(chessboard[x][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + x;
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0 && y+count >= 0){
				if(chessboard[x+count][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y+count] > 0){
					break;
				}else if(chessboard[x+count][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
	}
	else if(piece == ROOK){
		for(int count = 1; count < 8; count++){
			if(y-count >= 0){
				if(chessboard[x][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + x;
					lightArrayCount++;
				}else if(chessboard[x][y-count] > 0){
					break;
				}else if(chessboard[x][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + x;
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(y+count >= 0){
				if(chessboard[x][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + x;
					lightArrayCount++;
				}else if(chessboard[x][y+count] > 0){
					break;
				}else if(chessboard[x][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + x;
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x-count >= 0){
				if(chessboard[x-count][y] == 0){
					lightArray[lightArrayCount] = 8 * y + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y] > 0){
					break;
				}else if(chessboard[x-count][y] < 0){
					lightArray[lightArrayCount] = 8 * y + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0){
				if(chessboard[x+count][y] == 0){
					lightArray[lightArrayCount] = 8 * y + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y] > 0){
					break;
				}else if(chessboard[x+count][y] < 0){
					lightArray[lightArrayCount] = 8 * y + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
	}
	else if(piece == BISHOP){
		for(int count = 1; count < 8; count++){
			if(x-count >= 0 && y-count >= 0){
				if(chessboard[x-count][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y-count] > 0){
					break;
				}else if(chessboard[x-count][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0 && y-count >= 0){
				if(chessboard[x+count][y-count] == 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y-count] > 0){
					break;
				}else if(chessboard[x+count][y-count] < 0){
					lightArray[lightArrayCount] = 8 * (y-count) + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x-count >= 0 && y+count >= 0){
				if(chessboard[x-count][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x-count);
					lightArrayCount++;
				}else if(chessboard[x-count][y+count] > 0){
					break;
				}else if(chessboard[x-count][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x-count);
					lightArrayCount++;
					break;
				}
			}
		}
		for(int count = 1; count < 8; count++){
			if(x+count >= 0 && y+count >= 0){
				if(chessboard[x+count][y+count] == 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x+count);
					lightArrayCount++;
				}else if(chessboard[x+count][y+count] > 0){
					break;
				}else if(chessboard[x+count][y+count] < 0){
					lightArray[lightArrayCount] = 8 * (y+count) + (x+count);
					lightArrayCount++;
					break;
				}
			}
		}
	}
  else if(piece == KNIGHT){
		if(x-2 >= 0 && y-1 >= 0){
			if(chessboard[x-2][y-1] <= 0){
				lightArray[lightArrayCount] = 8 * (y-1) + (x-2);
				lightArrayCount++;
			}
		}
		if(x-1 >= 0 && y-2 >= 0){
			if(chessboard[x-1][y-2] <= 0){
				lightArray[lightArrayCount] = 8 * (y-2) + (x-1);
				lightArrayCount++;
			}
		}
		if(x+1 >= 0 && y-2 >= 0){
			if(chessboard[x+1][y-2] <= 0){
				lightArray[lightArrayCount] = 8 * (y-2) + (x+1);
				lightArrayCount++;
			}
		}
		if(x+2 >= 0 && y-1 >= 0){
			if(chessboard[x+2][y-1] <= 0){
				lightArray[lightArrayCount] = 8 * (y-1) + (x+2);
				lightArrayCount++;
			}
		}
		if(x-2 >= 0 && y+1 >= 0){
			if(chessboard[x-2][y+1] <= 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x-2);
				lightArrayCount++;
			}
		}
		if(x-1 >= 0 && y+2 >= 0){
			if(chessboard[x-1][y+2] <= 0){
				lightArray[lightArrayCount] = 8 * (y+2) + (x-1);
				lightArrayCount++;
			}
		}
		if(x+1 >= 0 && y+2 >= 0){
			if(chessboard[x+1][y+2] <= 0){
				lightArray[lightArrayCount] = 8 * (y+2) + (x+1);
				lightArrayCount++;
			}
		}
		if(x+2 >= 0 && y+1 >= 0){
			if(chessboard[x+2][y+1] <= 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x+2);
				lightArrayCount++;
			}
		}
	}

	else if(piece == PAWN){
		if(y == 1){
			for(int count = 1; count < 2; count++){
				if(y+count < 8){
					if(chessboard[x][y+count] == 0){
						lightArray[lightArrayCount] = 8 * (y+count) + x;
						lightArrayCount++;
					}else if(chessboard[x][y+count] > 0){
						break;
					}else if(chessboard[x][y+count] < 0){
						lightArray[lightArrayCount] = 8 * (y+count) + x;
						lightArrayCount++;
						break;
					}
				}
			}
		} else {
			if(y+1 < 8){
				if(chessboard[x][y+1] <= 0){
					lightArray[lightArrayCount] = 8 * (y+1) + x;
					lightArrayCount++;
				}
			}
		}
		if(y+1 < 8 && x-1 >= 0){
			if(chessboard[x-1][y+1] < 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x-1);
				lightArrayCount++;
			}
		}
		if(y+1 < 8 && x+1 < 8){
			if(chessboard[x+1][y+1] < 0){
				lightArray[lightArrayCount] = 8 * (y+1) + (x+1);
				lightArrayCount++;
			}
		}
	}

}

void toggleLightsOn(){
	
}

int main(void){ 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
  Timer0A_Init(&TimerATask, 50000);  // initialize timer0A
  EnableInterrupts();
	currentState = 0;
  while(1){
		if(currentState == GAMESTART){
			lightState = LIGHTSON;
			for(int x = 0; x < 8; x++){
				for(int y = 0; y < 8; y++){
					chessboard[x][y] = STARTCHESSBOARD[x][y];
				}
			}
			currentState = P1STARTTURN;
		} else if(currentState == P1STARTTURN){
			for(int x = 0; x < 8; x++){
				for(int y = 0; y < 8; y++){
					if(chessboard[x][y] > 0){
						//showMoves(chessboard[x][y], x, y);
					}
				}
			}
			toggleLightsOn();
		} else if(currentState == P1TURN){
			
		} else if(currentState == P1CAPTURE){
			
		} else if(currentState == P1ENDTURN){
			
		} else if(currentState == P1PROMOTE){
			
		} else if(currentState == P2STARTTURN){
			
		} else if(currentState == P2TURN){
			
		} else if(currentState == P2CAPUTER){
			
		} else if(currentState == P2ENDTURN){
			
		} else if(currentState == P2PROMOTE){
			
		} else if(currentState == ENDGAME){
			
		} 
  }
}
