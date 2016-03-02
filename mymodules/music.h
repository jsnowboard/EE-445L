// music.h
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

extern int next_note;
extern const unsigned int carolOfTheBells[60];
extern const unsigned int MaryHadALittleLamb[28];
extern const unsigned int wave[32];
extern const unsigned int testsong[10];

// ***************** Song_Init ****************
// Initilize a Song struct
// Inputs: none
// Outputs: none
void Song_Init(void);
