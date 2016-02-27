// portf_init.c
// Runs on TM4C123
// Ty Winkler
// Feb 23, 2016

#include "music.h"

typedef struct {
	int music[10];
	int frequency[10];
}Song;
Song carolOfTheBells;

