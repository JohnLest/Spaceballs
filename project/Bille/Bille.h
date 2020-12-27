//
// Created by john on 19/12/20.
//

#ifndef SPACEBALLS_BILLE_H
#define SPACEBALLS_BILLE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../GrilleSDL/GrilleSDL.h"
#include "../Tools/Tools.h"
#include "../Ressources/Ressources.h"

// Macro pour la direction des billes
#define HAUT            0
#define BAS             1
#define GAUCHE          3
#define DROITE          4

typedef struct S_BILLE
{
    void(*generate)(struct S_BILLE*, int m[19][19]);
    void(*move)(struct S_BILLE*);
    void(*changeDir)(struct  S_BILLE*);
    void(*free)(struct S_BILLE*);

    int L;
    int C;
    int dir;
    int couleur;
} S_BILLE;

S_BILLE* NewBille(int, int);
static void InitBille(S_BILLE*);
void generateBille(S_BILLE*, int m[19][19]);
void changeDirection(S_BILLE*);
void freeBille(S_BILLE*);
void moveBille(S_BILLE*);
#endif //SPACEBALLS_BILLE_H
