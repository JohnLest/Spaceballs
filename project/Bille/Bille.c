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
    This->move = moveBille;
    This->changeDir = changeDirection;
    This->free = freeBille;
    This->L = 0;
    This->C = 0;
    This->couleur = 0;
    This->dir = 0;
    This->redZone = 0;
}

void generateBille(S_BILLE *This, int tab[19][19]){
    while (1){
        This->L = randTool(4, 14);
        This->C = randTool(4, 14);
        if(((This->L < 6 || This->L > 12) || (This->C < 6 || This->C > 12)) && tab[This->L][This->C] != 2 )
            break;
    }
    DessineBille(GRISE, This->L, This->C);
    waiting(0, 500);
    DessineBille(This->couleur, This->L, This->C);
}

void moveBille(S_BILLE *This){
    EffaceCarre(This->L, This->C);
    switch (This->dir) {
        case HAUT:
            This->L--;
            break;
        case BAS:
            This->L++;
            break;
        case GAUCHE:
            This->C--;
            break;
        case DROITE:
            This->C++;
            break;
    }
    DessineBille(This->couleur, This->L, This->C);
}

void changeDirection(S_BILLE *This){
    switch (This->dir) {
        case HAUT:
            This->dir = BAS;
            break;
        case BAS:
            This->dir = HAUT;
            break;
        case GAUCHE:
            This->dir = DROITE;
            break;
        case DROITE:
            This->dir = GAUCHE;
            break;
    }
}

void freeBille(S_BILLE* This){
    free(This);
}