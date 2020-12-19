//
// Created by john on 19/12/20.
//

#include "Bille.h"

S_BILLE generateBille(int color){
    srand(getSeed());
    int ligne;
    int col;
    S_BILLE bille;
    while (1){
        ligne = rand() % 11 + 4;
        col = rand() % 11 + 4;
        if((ligne < 6 || ligne > 12) || (col < 6 || col > 12)) break;
    }

    bille = {ligne, col, 0, color};
    DessineBille(color, ligne, col);
    return bille;
}
