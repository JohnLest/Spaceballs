// TODO Refactoriser le code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "GrilleSDL/GrilleSDL.h"
#include "Ressources/Ressources.h"
#include "Bille/Bille.h"

// Dimensions de la grille de jeu
#define NB_LIGNES 19
#define NB_COLONNES 19
#define NB_BILLES 12

// Macros utilisees dans le tableau tab
#define VIDE 0
#define MUR 1
#define BILLE 2
#define NB_MAX_BILLES_ZONE 3

#define TRUE 1

// #region Déclaration variable global

typedef struct TABLE
{
    int tab[NB_LIGNES][NB_COLONNES];
    int tabPointBilles[NB_BILLES];
    int nbBilles;
    pthread_t tabThreadsBilles[NB_BILLES];
    pthread_mutex_t mutexTab;
    pthread_mutex_t mutexBilles;
    pthread_cond_t condTab;
    sigset_t sigset_int;
    sigset_t sigset_trap;
    pthread_mutex_t freezeTab;
    pthread_cond_t freezeCond;

} TABLE;

static TABLE table = {
    .tab =
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
    .mutexBilles = PTHREAD_MUTEX_INITIALIZER,
    .nbBilles = 0,
    .freezeTab = PTHREAD_MUTEX_INITIALIZER,
    .freezeCond = PTHREAD_COND_INITIALIZER};
pthread_key_t keyBille;
int freeze = 1;

// Threads
void *lanceBille_t(void *);
void *bille_t(S_BILLE *);
void *verrou_t(void *);
void *event_t(pthread_t *);
void *pause_t(void *);
// Gestion Zone
void initGrille();
char ZoneRestreinte(int l, int c);
int NbBillesZone();
int changeZoneRest(S_BILLE *);
// Autre
int *regarde(S_BILLE *);
void verrou(int);
void pauseJeu(int);
void setSigaction();
void setSigset();
void mutexFreeze();

// #endregion

int main(int argc, char *argv[])
{
    pthread_t _lanceBille_t, _verrou_t, _event_t, _pause_t;

    // Ouverture de la fenetre graphique
    printf("(THREAD MAIN %d) Ouverture de la fenetre graphique\n", pthread_self());
    fflush(stdout);

    if (OuvertureFenetreGraphique() < 0)
    {
        printf("Erreur de OuvrirGrilleSDL\n");
        fflush(stdout);
        exit(1);
    }

    // Initialisation de la grille de jeu
    initGrille();
    setSigaction();
    setSigset();

    pthread_create(&_lanceBille_t, NULL, lanceBille_t, NULL);
    pthread_create(&_verrou_t, NULL, verrou_t, NULL);
    pthread_create(&_pause_t, NULL, pause_t, NULL);
    pthread_create(&_event_t, NULL, event_t, _pause_t);

    pthread_join(_event_t, NULL);

    pthread_cancel(_verrou_t);
    pthread_cancel(_lanceBille_t);
    pthread_cancel(_pause_t);

    // Fermeture de la grille de jeu (SDL)
    printf("(THREAD MAIN %d) Fermeture de la fenetre graphique...", pthread_self());
    fflush(stdout);
    FermetureFenetreGraphique();
    printf("OK\n");
    fflush(stdout);

    exit(0);
}

// #region Thread

void *lanceBille_t(void *arg)
{
    int couleur = ROUGE;
    int dir = HAUT;
    sigprocmask(SIG_BLOCK, &table.sigset_int, NULL);
    pthread_key_create(&keyBille, NULL);
    for (int i = 0; i < NB_BILLES; ++i)
    {
        S_BILLE *bille = NewBille(couleur, dir);
        mutexFreeze();
        pthread_create(&table.tabThreadsBilles[i], NULL, bille_t, bille);
        if (couleur == MAGENTA)
            couleur = ROUGE;
        else
            couleur++;
        if (dir == GAUCHE)
            dir = HAUT;
        else
            dir++;
        table.nbBilles++;
    }
    /*
    printf("1");
    for (int i = 0; i < NB_BILLES; ++i) {
        pthread_cancel(table.bille_t[i]);
    }
    
    printf("2");
    pthread_join(table.tabThreadsBilles[0], NULL);
    for (int i = 0; i < NB_BILLES; ++i) {
        pthread_join(table.tabThreadsBilles[i], NULL);
    }
    printf("3");
    */
    return NULL;
}

void *bille_t(struct S_BILLE *bille)
{
    sigprocmask(SIG_BLOCK, &table.sigset_int, NULL);
    pthread_mutex_lock(&table.mutexTab);
    bille->generate(bille, table.tab);
    pthread_mutex_unlock(&table.mutexTab);
    printf("Bille au coord %d - %d de couleur %d\n", bille->L, bille->C, bille->couleur);
    table.tab[bille->L][bille->C] = BILLE;

    while (TRUE)
    {
        mutexFreeze();
        int time = randTool(200, 1000);
        if (time == 1000)
        {
            waiting(1, 0);
        }
        else
            waiting(0, time);
        waiting(1, 0);
        int *coord;
        coord = regarde(bille);
        int etatZone = changeZoneRest(bille);
        if (table.tab[*(coord)][*(coord + 1)] != VIDE)
        {
            bille->changeDir(bille);
        }
        else
        {
            sigprocmask(SIG_BLOCK, &table.sigset_trap, NULL);
            pthread_mutex_lock(&table.mutexTab);
            if (etatZone == 1 && NbBillesZone() >= 3)
            {
                pthread_cond_wait(&table.condTab, &table.mutexTab);
            }
            else if (etatZone == 2)
            {
                pthread_cond_signal(&table.condTab);
            }
            table.tab[bille->L][bille->C] = VIDE;
            pthread_mutex_unlock(&table.mutexTab);
            sigprocmask(SIG_UNBLOCK, &table.sigset_trap, NULL);
            bille->move(bille);
            sigprocmask(SIG_BLOCK, &table.sigset_trap, NULL);
            pthread_mutex_lock(&table.mutexTab);
            table.tab[bille->L][bille->C] = BILLE;
            pthread_mutex_unlock(&table.mutexTab);
            sigprocmask(SIG_UNBLOCK, &table.sigset_trap, NULL);
            pthread_setspecific(keyBille, (void *)bille);
        }
    }
    return NULL;
}

void *verrou_t(void *arg)
{
    while (TRUE)
    {
        waiting(10, 0);
        mutexFreeze();
        pthread_kill(table.tabThreadsBilles[randTool(0, table.nbBilles - 1)], SIGTRAP);
    }
}

void *event_t(pthread_t *_pause_t){
    EVENT_GRILLE_SDL event;
    sigprocmask(SIG_BLOCK, &table.sigset_int, NULL);
    while (TRUE)
    {
        event = ReadEvent();
        if (event.type == CROIX)
            break;
        else if (event.type == CLAVIER && event.touche == 'q')
            break;
        else if (event.type == CLAVIER && event.touche == 'p')
            pthread_kill(_pause_t, SIGINT);
    }
    pthread_exit(NULL);
}

void *pause_t(void *arg){
    while (TRUE)
    {
        pause();
        pthread_mutex_lock(&table.freezeTab);
        freeze = !freeze;
        if (freeze) 
            pthread_cond_broadcast(&table.freezeCond);
        pthread_mutex_unlock(&table.freezeTab);
    }
    pthread_exit(NULL);
}

// #endregion

// #region Gestion de Zone

int changeZoneRest(S_BILLE *bille)
{
    int *coord;
    coord = regarde(bille);
    if (!bille->redZone)
    {
        if (!ZoneRestreinte(*(coord), *(coord + 1)))
            return 0;
        else if (ZoneRestreinte(*(coord), *(coord + 1)))
        {
            bille->redZone = 1;
            return 1;
        }
    }
    else if (bille->redZone)
    {
        if (!ZoneRestreinte(bille->L, bille->C))
        {
            bille->redZone = 0;
            return 2;
        }
        else if (ZoneRestreinte(bille->L, bille->C))
            return 0;
    }
    return 3;
}

void initGrille()
{
    int l, c;
    for (l = 0; l < NB_LIGNES; l++)
    {
        for (c = 0; c < NB_COLONNES; c++)
        {
            if (table.tab[l][c] == MUR)
            {
                DessineMur(l, c);
            }
        }
    }
}

char ZoneRestreinte(int l, int c)
{
    if ((c < 6) || (c > 12))
        return 0;
    if ((l < 6) || (l > 12))
        return 0;
    return 1;
}

int NbBillesZone()
{
    int l, c, nb = 0;
    for (l = 6; l <= 12; l++)
        for (c = 6; c <= 12; c++)
            if (table.tab[l][c] == BILLE)
                nb++;
    return nb;
}

// #endregion

// #region Autre

void verrou(int signum)
{
    S_BILLE *bille_t = (S_BILLE *)pthread_getspecific(keyBille);
    DessineVerrou(bille_t->L, bille_t->C);
    waiting(randTool(4, 8), 0);

    if (bille_t->dir == GAUCHE)
        bille_t->dir = HAUT;
    else
        bille_t->dir++;
}

void pauseJeu(int signum){}

int *regarde(S_BILLE *bille)
{
    static int coord[2];
    switch (bille->dir)
    {
    case HAUT:
        coord[0] = bille->L - 1;
        coord[1] = bille->C;
        break;
        //return table.tab[bille->L - 1][bille->C];
    case BAS:
        coord[0] = bille->L + 1;
        coord[1] = bille->C;
        //return table.tab[bille->L + 1][bille->C];
        break;
    case GAUCHE:
        coord[0] = bille->L;
        coord[1] = bille->C - 1;
        //return table.tab[bille->L ][bille->C - 1];
        break;
    case DROITE:
        coord[0] = bille->L;
        coord[1] = bille->C + 1;
        //return table.tab[bille->L ][bille->C + 1];
        break;
    }
    return coord;
}

void setSigaction(){
    struct sigaction *act = (struct sigaction *)calloc(1, sizeof(struct sigaction));
    act->sa_handler = verrou;
    act->sa_flags = 0;
    sigaction(SIGTRAP, act, NULL);

    act->sa_handler = pauseJeu;
    sigaction(SIGINT, act, NULL);

    free(act);
}

void setSigset(){
    sigemptyset(&table.sigset_trap);
    sigaddset(&table.sigset_trap, SIGTRAP);

    sigemptyset(&table.sigset_int);
    sigaddset(&table.sigset_int, SIGINT);
}

void mutexFreeze(){
    pthread_mutex_lock(&table.freezeTab);
    if (!freeze)
        pthread_cond_wait(&table.freezeCond, &table.freezeTab); 
    pthread_mutex_unlock(&table.freezeTab);
}

// #endregion
