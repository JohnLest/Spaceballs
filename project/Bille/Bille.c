//
// Created by john on 19/12/20.
//

#include "Bille.h"

S_BILLE* NewBille(int color, int dir){
    S_BILLE* This = (S_BILLE*)malloc(sizeof(S_BILLE));
    if(!This) return NULL;
    InitBille(This);
    This->free = freeBille;
    This->couleur = color;
    This->dir = dir;
    return This;
}

static void InitBille(S_BILLE* This){
    This->generate = generateBille;
    This->free = freeBille;
    This->L = 0;
    This->C = 0;
    This->couleur = 0;
    This->dir = 0;
}

void generateBille(S_BILLE *This, int tab[19][19]){
    srand(getSeed());
    while (1){
        This->L = rand() % 11 + 4;
        This->C = rand() % 11 + 4;
        if(((This->L < 6 || This->L > 12) || (This->C < 6 || This->C > 12)) && tab[This->L][This->C] != 2 )
            break;
    }
    DessineBille(This->couleur, This->L, This->C);
}

void freeBille(S_BILLE* This){
    free(This);
}