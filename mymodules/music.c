// music.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "music.h"
#include "notes.c"

struct song{
	int music[10];
	int frequency[10];
};

void Song_Init(void){
	struct song carolOfTheBells;

	carolOfTheBells.music[0] = A;
	carolOfTheBells.music[1] = A;
	carolOfTheBells.music[2] = A;
	carolOfTheBells.music[3] = A;
	carolOfTheBells.music[4] = A;
	carolOfTheBells.music[5] = A;
	carolOfTheBells.music[6] = A;
}
