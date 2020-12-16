#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include "GrilleSDL/GrilleSDL.h"
#include "Ressources/Ressources.h"

// Dimensions de la grille de jeu
#define NB_LIGNES   19
#define NB_COLONNES 19

// Macros utilisees dans le tableau tab
#define VIDE                     0
#define MUR                      1
#define BILLE                    2

#define NB_MAX_BILLES_ZONE       3  // Nombre maximum de billes dans la zone restreinte

int tab[NB_LIGNES][NB_COLONNES]
={ {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
   {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}};

typedef struct
{
  int L;
  int C;
  int dir;
  int couleur;
} S_BILLE;

S_BILLE tabBille[12];

void  generateBille(int);
void  initGrille();
char  ZoneRestreinte(int l,int c);
int   NbBillesZone();
unsigned long getSeed();

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
  EVENT_GRILLE_SDL event;
  char ok;
 
  srand((unsigned)time(NULL));

  // Ouverture de la fenetre graphique
  printf("(THREAD MAIN %d) Ouverture de la fenetre graphique\n",pthread_self()); fflush(stdout);
  if (OuvertureFenetreGraphique() < 0)
  {
    printf("Erreur de OuvrirGrilleSDL\n");
    fflush(stdout);
    exit(1);
  }

  // Initialisation de la grille de jeu
  initGrille();
 
  // Code d'exemple pour le ReadEvent
  ok = 0;
  while(!ok)
  {
    event = ReadEvent();
    if (event.type == CROIX) ok = 1;
    if (event.type == CLAVIER && event.touche == 'q') ok = 1;
    if (event.type == CLIC_GAUCHE) generateBille(ROUGE);
  }

  // Fermeture de la grille de jeu (SDL)
  printf("(THREAD MAIN %d) Fermeture de la fenetre graphique...",pthread_self()); fflush(stdout);
  FermetureFenetreGraphique();
  printf("OK\n"); fflush(stdout);

  exit(0);
}

unsigned long getSeed(){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    return (spec.tv_sec*1000000000 + spec.tv_nsec);
}

void generateBille(int color){
    srand(getSeed());
    int ligne;
    int col;
    while (1){
        ligne = rand() % 11 + 4;
        col = rand() % 11 + 4;
        if((ligne < 6 || ligne > 12) || (col < 6 || col > 12)) break;
    }

    tabBille[0] = {ligne, col, 0, color};
    printf("Bille au coord %d - %d de couleur %d\n",tabBille[0].L, tabBille[0].C, tabBille[0].couleur);
    DessineBille(color, ligne, col);
}

/*********************************************************************************************/
void initGrille()
{
  int l,c;
  // Dessine les murs aux coins de la grille
  DessineMur(0,0);
  DessineMur(18,18);
  DessineMur(0,18);
  DessineMur(18,0);
  for (l=1 ; l<=17 ; l++) { tab[l][17] = MUR; DessineMur(l,17); } // on mure a droite
  for (l=1 ; l<=17 ; l++) { tab[l][1] = MUR; DessineMur(l,1); } // on mure a gauche
  for (c=1 ; c<=17 ; c++) { tab[1][c] = MUR; DessineMur(1,c); } // on mure en haut
  for (c=1 ; c<=17 ; c++) { tab[17][c] = MUR; DessineMur(17,c); } // on mure le bas
}

/*********************************************************************************************/
char ZoneRestreinte(int l,int c)
{
  if ((c < 6) || (c > 12)) return 0;
  if ((l < 6) || (l > 12)) return 0;
  return 1;
}

int NbBillesZone()
{
  int l,c,nb=0;
  for (l=6 ; l<=12 ; l++)
    for (c=6 ; c<=12 ; c++) 
      if (tab[l][c] == BILLE) nb++;
  return nb;
}

