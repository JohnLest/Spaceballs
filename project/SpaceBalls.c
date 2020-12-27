#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "GrilleSDL/GrilleSDL.h"
#include "Ressources/Ressources.h"
#include "Bille/Bille.h"

// Dimensions de la grille de jeu
#define NB_LIGNES   19
#define NB_COLONNES 19
#define NB_BILLES 12

// Macros utilisees dans le tableau tab
#define VIDE                     0
#define MUR                      1
#define BILLE                    2
#define NB_MAX_BILLES_ZONE       3  // Nombre maximum de billes dans la zone restreinte

typedef struct TABLE{
    int tab[NB_LIGNES][NB_COLONNES];
    S_BILLE tabBill[NB_BILLES];
    pthread_t billeThread[NB_BILLES];
    pthread_mutex_t mutexTab;

}TABLE;

static TABLE table = {
        .tab  =
                {{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
        .mutexTab = PTHREAD_MUTEX_INITIALIZER,
};


void initGrille();
char ZoneRestreinte(int l, int c);
int NbBillesZone();
void *mainThread(void *);
void *billeThread(void *);
int regarde(S_BILLE*);

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {
    pthread_t _mainThread;

    // Ouverture de la fenetre graphique
    printf("(THREAD MAIN %d) Ouverture de la fenetre graphique\n", pthread_self());
    fflush(stdout);

    if (OuvertureFenetreGraphique() < 0) {
        printf("Erreur de OuvrirGrilleSDL\n");
        fflush(stdout);
        exit(1);
    }

    // Initialisation de la grille de jeu
    initGrille();


    pthread_create(&_mainThread, NULL, mainThread, NULL);
    pthread_join(_mainThread, NULL);

    // Fermeture de la grille de jeu (SDL)
    printf("(THREAD MAIN %d) Fermeture de la fenetre graphique...", pthread_self());
    fflush(stdout);
    FermetureFenetreGraphique();
    printf("OK\n");
    fflush(stdout);

    exit(0);
}

void *mainThread(void *arg) {
    // Code d'exemple pour le ReadEvent
    EVENT_GRILLE_SDL event;
    char ok;
    ok = 0;
    while (!ok) {
        event = ReadEvent();
        if (event.type == CROIX) ok = 1;
        if (event.type == CLAVIER && event.touche == 'q') ok = 1;
        if (event.type == CLIC_GAUCHE) {
            for (int i = 0; i <NB_BILLES; ++i) {
                pthread_create(&table.billeThread[i], NULL, billeThread, NULL);
            }

        }
    }
    pthread_join(table.billeThread[0], NULL);
    for (int i = 0; i < NB_BILLES; ++i) {
        pthread_join(table.billeThread[i], NULL);
    }
    return NULL;
}

void *billeThread(void *arg){
    S_BILLE *bille = NewBille(randTool(400001, 400006), randTool(0, 3));
    pthread_mutex_lock(&table.mutexTab);
    bille->generate(bille, table.tab);
    pthread_mutex_unlock(&table.mutexTab);
    printf("Bille au coord %d - %d de couleur %d\n", bille->L, bille->C, bille->couleur);
    table.tab[bille->L][bille->C] = BILLE;

    while (1) {
        //mutex_lock(&table.mutexTab);
        if(regarde(bille) != VIDE) { bille->changeDir(bille); }
        else {
            table.tab[bille->L][bille->C] = VIDE;
            bille->move(bille);
            table.tab[bille->L][bille->C] = BILLE;
        }
        //pthread_mutex_unlock(&table.mutexTab);
    }
    return  NULL;
}

int regarde(S_BILLE* bille){
    switch (bille->dir) {
        case HAUT:
            return table.tab[bille->L - 1][bille->C];
        case BAS:
            return table.tab[bille->L + 1][bille->C];
        case GAUCHE:
            return table.tab[bille->L ][bille->C - 1];
        case DROITE:
            return table.tab[bille->L ][bille->C + 1];
    }
}

/*********************************************************************************************/
void initGrille() {
    int l, c;
    for (l = 0; l < NB_LIGNES; l++) {
        printf(" Ligne num %d\n", l);

        for (c = 0; c < NB_COLONNES; c++) {
            printf(" col num %d\n", c);
            if (table.tab[l][c] == MUR) {
                printf("Dessine Mur");
                DessineMur(l, c);
            }
        }
    }
}

/*********************************************************************************************/
char ZoneRestreinte(int l, int c) {
    if ((c < 6) || (c > 12)) return 0;
    if ((l < 6) || (l > 12)) return 0;
    return 1;
}

int NbBillesZone() {
    int l, c, nb = 0;
    for (l = 6; l <= 12; l++)
        for (c = 6; c <= 12; c++)
            if (table.tab[l][c] == BILLE) nb++;
    return nb;
}

