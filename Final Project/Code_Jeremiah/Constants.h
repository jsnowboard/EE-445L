// Constants.h
#if !defined(CONSTANTS_H)
#define CONSTANTS_H 1

//Chess Pieces +# is for player 1 -# is for player 2
const int KING = 6;
const int QUEEN = 5;
const int ROOK = 4;
const int BISHOP = 3;
const int KNIGHT = 2;
const int PAWN = 1;
const int EMPTY = 0;

//Initial Board
const int STARTCHESSBOARD[8][8] = {
	ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
	PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
	-PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN, -PAWN,
	-ROOK, -KNIGHT, -BISHOP, -QUEEN, -KING, -BISHOP, -KNIGHT, -ROOK
};

//Game States
const int GAMESTART = 0;
const int P1STARTTURN = 1;
const int P1TURN = 2;
const int P1CAPTURE = 3;
const int P1ENDTURN = 4;
const int P1PROMOTE = 5;
const int P2STARTTURN = 6;
const int P2TURN = 7;
const int P2CAPTURE = 8;
const int P2ENDTURN = 9;
const int P2PROMOTE = 10;
const int ENDGAME = 11;

//Light State
const int LIGHTSOFF = 0;
const int LIGHTSON = 1;

#endif
