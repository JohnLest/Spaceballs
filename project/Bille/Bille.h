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

typedef struct S_BILLE
{
    void(*generate)(struct S_BILLE*, int m[19][19]);
    void (*free)(struct S_BILLE*);

    pthread thread;
    int L;
    int C;
    int dir;
    int couleur;
} S_BILLE;

S_BILLE* NewBille(int, int);
static void InitBille(S_BILLE*);
void generateBille(S_BILLE*, int m[19][19]);
void freeBille(S_BILLE*);

#endif //SPACEBALLS_BILLE_H
