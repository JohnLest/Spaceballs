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


void  initGrille();
char  ZoneRestreinte(int l,int c);
int   NbBillesZone();

S_BILLE tabBill[12];

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc,char* argv[])
{
  EVENT_GRILLE_SDL event;
  char ok;

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
    if (event.type == CLIC_GAUCHE) {
        S_BILLE *bille = NewBille(ROUGE, 0);
        bille->generate(bille, tab);
        printf("Bille au coord %d - %d de couleur %d\n",bille->L, bille->C, bille->couleur);
        tab[bille->L][bille->C] = BILLE;
        /*
        int l,c,nb=0;
        for (l=0 ; l<=18 ; l++) {
            printf("\n");
            for (c = 0; c <= 18; c++)
                printf("%d ", tab[l][c]);
        }
        */
    }
  }

  // Fermeture de la grille de jeu (SDL)
  printf("(THREAD MAIN %d) Fermeture de la fenetre graphique...",pthread_self()); fflush(stdout);
  FermetureFenetreGraphique();
  printf("OK\n"); fflush(stdout);

  exit(0);
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

