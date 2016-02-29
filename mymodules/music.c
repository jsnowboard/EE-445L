// music.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "music.h"
#include "notes.c"

int next_note = 1;

const unsigned short carolOfTheBells[60] = {   
  
};

//repeat 8 times
//Treble cleff
carolOfTheBells.music[0]=Bflat;
carolOfTheBells.music[0]=A;//half note
carolOfTheBells.music[0]=B;//half note
carolOfTheBells.music[0]=G;

//repeat 4 times
//Trebel cleff
carolOfTheBells.music[0]=Bflat;
carolOfTheBells.music[0]=G;//Played with previous note
carolOfTheBells.music[0]=A;//half note
carolOfTheBells.music[0]=B;//half note
carolOfTheBells.music[0]=G;

//repeat 4 times
//Trebel cleff 
carolOfTheBells.music[0]=D;
carolOfTheBells.music[0]=Bflat;//Played with previous note
carolOfTheBells.music[0]=C;//half note
carolOfTheBells.music[0]=Eflat;//half note
carolOfTheBells.music[0]=Bflat;
carolOfTheBells.music[0]=G;//Played with previous note

//Played once
//Trebel cleff
carolOfTheBells.music[0]=G;//one octave higher
carolOfTheBells.music[0]=D;
carolOfTheBells.music[0]=Bflat;
carolOfTheBells.music[0]=G;//played with previous 3 notes

carolOfTheBells.music[0]=G;//one octave higher
carolOfTheBells.music[0]=G;//played with previous note as half note

carolOfTheBells.music[0]=G;//one octave higher
carolOfTheBells.music[0]=G;//played with previous note as half note

carolOfTheBells.music[0]=F;//one octave higher
carolOfTheBells.music[0]=F;//played with previous note as half note

carolOfTheBells.music[0]=F;//one octave higher
carolOfTheBells.music[0]=F;//played with previous note as half note

//played once
//Trebel cleff
carolOfTheBells.music[0]=D;
carolOfTheBells.music[0]=G;
carolOfTheBells.music[0]=Bflat;//played for 3 counts with previous 3 notes
carolOfTheBells.music[0]=D;//half note
carolOfTheBells.music[0]=D;//half note
carolOfTheBells.music[0]=C;//half note
carolOfTheBells.music[0]=Bflat;//half note

//played once
//Trebel cleff
carolOfTheBells.music[0]=C;
carolOfTheBells.music[0]=G;
carolOfTheBells.music[0]=E;//previous three notes played together, previous two played for 3 counts
carolOfTheBells.music[0]=C;//half note
carolOfTheBells.music[0]=C;//half note
carolOfTheBells.music[0]=D;//half note
carolOfTheBells.music[0]=C;//half note

//played once
//Trebel Cleff
carolOfTheBells.music[0]=Bflat;
carolOfTheBells.music[0]=G;
carolOfTheBells.music[0]=D;//one octave lower played with previous 3 notes, previous 2 notes for 3 counts
carolOfTheBells.music[0]=A;//half note
carolOfTheBells.music[0]=Bflat;//half note
carolOfTheBells.music[0]=G;

//