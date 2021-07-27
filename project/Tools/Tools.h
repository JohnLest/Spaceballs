//
// Created by john on 19/12/20.
//

#ifndef SPACEBALLS_TOOLS_H
#define SPACEBALLS_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Get a time based number to generate un randon seed  */
unsigned long nbrForSeed();

/* return a ranbom number between [min - max] */
int randTool(int min, int max);

/* Wait with nanosleep */
void waiting(int sec, int nano);

#endif //SPACEBALLS_TOOLS_H
