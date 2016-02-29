// music.c
// Runs on TM2C123
// Ty Winkler
// Feb 23, 2016

#include "music.h"
#include "notes.c"

int next_note = 1;

//First value is note frequency, second value is x where the note duration is 1/x
//
const unsigned short carolOfTheBells[500] = {   
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
  Bflat, 2,
	A, 1,
	B, 1,
	G, 2,
	
	Bflat, 2,
	G, 2,//play with previous note
	A, 1,
	B, 1,
	G, 2,
	Bflat, 2,
	G, 2,//play with previous note
	A, 1,
	B, 1,
	G, 2,
	Bflat, 2,
	G, 2,//play with previous note
	A, 1,
	B, 1,
	G, 2,
	Bflat, 2,
	G, 2,//play with previous note
	A, 1,
	B, 1,
	G, 2,
	
	D, 2,
	Bflat, 2,//played with previous note
	C, 1,
	Eflat, 1,
	Bflat, 2,
	G, 2,//played with previous note
	D, 2,
	Bflat, 2,//played with previous note
	C, 1,
	Eflat, 1,
	Bflat, 2,
	G, 2,//played with previous note
	D, 2,
	Bflat, 2,//played with previous note
	C, 1,
	Eflat, 1,
	Bflat, 2,
	G, 2,//played with previous note
	D, 2,
	Bflat, 2,//played with previous note
	C, 1,
	Eflat, 1,
	Bflat, 2,
	G, 2,//played with previous note
	
	G,2,//Octave higher than normal G
	D, 2,
	Bflat, 2,
	G, 2,//played with previous 3 notes
	G, 1, 
	G, 1,//Played with previous note one octave higher
  G, 1,
	G, 1, //Played with previus note one octave higher
	F, 1, 
	F, 1, //Played with the previous note one octave higher
	E, 1, 
	E, 1, //Played with the preious note one octave higher
	
	D, 2,
	G, 6,
	Bflat, 6,
	D, 6,//one octave lower played with previous 3 notes
	D, 1, 
	D, 1, 
	C, 1,
	B, 1,
	
	C, 2,
	G, 6,
	E, 6,//played with previous 2 notes
	C, 1,
	C, 1,
	D, 1,
	C, 1,
	
	C, 2,
	G, 6,
	B, 6,//played with previous 2 notes
	
// 	//played once
// //Trebel cleff
// carolOfTheBells.music[0]=D;
// carolOfTheBells.music[0]=G;
// carolOfTheBells.music[0]=Bflat;//played for 3 counts with previous 3 notes
// carolOfTheBells.music[0]=D;//half note
// carolOfTheBells.music[0]=D;//half note
// carolOfTheBells.music[0]=C;//half note
// carolOfTheBells.music[0]=Bflat;//half note

// //played once
// //Trebel cleff
// carolOfTheBells.music[0]=C;
// carolOfTheBells.music[0]=G;
// carolOfTheBells.music[0]=E;//previous three notes played together, previous two played for 3 counts
// carolOfTheBells.music[0]=C;//half note
// carolOfTheBells.music[0]=C;//half note
// carolOfTheBells.music[0]=D;//half note
// carolOfTheBells.music[0]=C;//half note

// //played once
// //Trebel Cleff
// carolOfTheBells.music[0]=Bflat;
// carolOfTheBells.music[0]=G;
// carolOfTheBells.music[0]=D;//one octave lower played with previous 3 notes, previous 2 notes for 3 counts
// carolOfTheBells.music[0]=A;//half note
// carolOfTheBells.music[0]=Bflat;//half note
// carolOfTheBells.music[0]=G;
 };

const unsigned int sinWave[120] = {
	0x7ff, 0x86a, 0x8d5, 0x93f, 0x9a9, 0xa11, 0xa78, 0xadd, 0xb40, 0xba1,
	0xbff, 0xc5a, 0xcb2, 0xd08, 0xd59, 0xda7, 0xdf1, 0xe36, 0xe77, 0xeb4,
	0xeec, 0xf1f, 0xf4d, 0xf77, 0xf9a, 0xfb9, 0xfd2, 0xfe5, 0xff3, 0xffc,
	0xfff, 0xffc, 0xff3, 0xfe5, 0xfd2, 0xfb9, 0xf9a, 0xf77, 0xf4d, 0xf1f,
	0xeec, 0xeb4, 0xe77, 0xe36, 0xdf1, 0xda7, 0xd59, 0xd08, 0xcb2, 0xc5a,
	0xbff, 0xba1, 0xb40, 0xadd, 0xa78, 0xa11, 0x9a9, 0x93f, 0x8d5, 0x86a,
	0x7ff, 0x794, 0x729, 0x6bf, 0x655, 0x5ed, 0x586, 0x521, 0x4be, 0x45d,
	0x3ff, 0x3a4, 0x34c, 0x2f6, 0x2a5, 0x257, 0x20d, 0x1c8, 0x187, 0x14a,
	0x112, 0xdf, 0xb1, 0x87, 0x64, 0x45, 0x2c, 0x19, 0xb, 0x2,
	0x0, 0x2, 0xb, 0x19, 0x2c, 0x45, 0x64, 0x87, 0xb1, 0xdf,
	0x112, 0x14a, 0x187, 0x1c8, 0x20d, 0x257, 0x2a5, 0x2f6, 0x34c, 0x3a4,
	0x3ff, 0x45d, 0x4be, 0x521, 0x586, 0x5ed, 0x655, 0x6bf, 0x729, 0x794
};	
