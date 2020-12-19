//
// Created by john on 19/12/20.
//

#ifndef SPACEBALLS_BILLE_H
#define SPACEBALLS_BILLE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../Tools/Tools.h"
#include "../Ressources/Ressources.h"

typedef struct
{
    int L;
    int C;
    int dir;
    int couleur;
} S_BILLE;

S_BILLE generateBille(int);

#endif //SPACEBALLS_BILLE_H
