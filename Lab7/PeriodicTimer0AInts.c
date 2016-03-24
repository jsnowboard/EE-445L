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

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int playerTurn = 1; //1 for player 1 or -1 for player 2
int pieceMoving = 0;

//Chess Pieces +# is for player 1 -# is for player 2
const int king = 6;
const int queen = 5;
const int rook = 4;
const int bishop = 3;
const int knight = 2;
const int pawn = 1;
const int empty = 0;
//Chess Pieces

int chessboard[8][8] = {
	rook, knight, bishop, queen, king, bishop, knight, rook,
	pawn, pawn, pawn, pawn, pawn, pawn, pawn, pawn,
	empty, empty, empty, empty, empty, empty, empty, empty,
	empty, empty, empty, empty, empty, empty, empty, empty,
	-pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn,
	-rook, -knight, -bishop, -queen, -king, -bishop, -knight, -rook
};

int lightPieces = 0;
int lightPiecesArray[64];


void TimerATask(void){

}

//Returns 1 if the piece can move
int canMove(int piece, int x, int y){
	if(abs(piece) == king){
		//Needs to be redone to check for array out of bounds also needs to check if its an apponents piece it is moving to which is allowed
		if(chessboard[x-1][y-1] == 0 || chessboard[x][y-1] == 0 || chessboard[x+1][y-1] == 0 || chessboard[x-1][y] == 0 || 
			 chessboard[x+1][y] == 0 || chessboard[x-1][y+1] == 0 || chessboard[x][y+1] == 0 || chessboard[x+1][y+1] == 0){
			return 1;
		}
	}else if(abs(piece) == queen){
		
	}else if(abs(piece) == rook){
		
	}else if(abs(piece) == bishop){
		
	}else if(abs(piece) == knight){
		
	}else if(piece == pawn){
		
	}else if(piece == -pawn){
		
	}
	return 0;
}

int main(void){ 
  PLL_Init(Bus80MHz);              // bus clock at 80 MHz
  Timer0A_Init(&TimerATask, 50000);  // initialize timer0A
  EnableInterrupts();
  while(1){
    if(pieceMoving){
			
		} else {
			
			for(int i = 0; i < 8; i++){
				for(int j = 0; j < 8; j++){
					if(canMove(chessboard[i][j], i, j)){
						lightPiecesArray[lightPieces] = (8 * i) + j;
						lightPieces++;
					}
				}
			}
			
		}
  }
}
