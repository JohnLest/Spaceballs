#include "Ressources.h"
#include "GrilleSDL.h"

// Macros pour les sprites
#define S_MUR                      300000
#define S_BILLE_GRISE              300001
#define S_BILLE_ROUGE              300002
#define S_BILLE_BLEUE              300003
#define S_BILLE_JAUNE              300004
#define S_BILLE_MAUVE              300005
#define S_BILLE_VERTE              300006
#define S_BILLE_MAGENTA            300007    
#define S_RAQUETTE_BAS             300008
#define S_RAQUETTE_HAUT            300009
#define S_RAQUETTE_GAUCHE          300010
#define S_RAQUETTE_DROITE          300011
#define S_VERROU                   300012
#define S_NUMERO_UN                300013
#define S_NUMERO_DEUX              300014
#define S_NUMERO_TROIS             300015
#define S_NUMERO_QUATRE            300016

void ChargementImages()
{
  // Definition de l'image de fond
  DessineImageFond("./images/ImageFond.bmp");

  // Sprites
  AjouteSprite(S_MUR,"./images/Mur32.bmp");
  AjouteSpriteAFondTransparent(S_BILLE_GRISE,"./images/BilleGrise32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_ROUGE,"./images/BilleRouge32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_BLEUE,"./images/BilleBleue32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_JAUNE,"./images/BilleJaune32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_MAUVE,"./images/BilleMauve32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_VERTE,"./images/BilleVerte32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_BILLE_MAGENTA,"./images/BilleMagenta32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_VERROU,"./images/Verrou32.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_NUMERO_UN,"./images/Numero1.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_NUMERO_DEUX,"./images/Numero2.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_NUMERO_TROIS,"./images/Numero3.bmp",255,255,255);
  AjouteSpriteAFondTransparent(S_NUMERO_QUATRE,"./images/Numero4.bmp",255,255,255);
  AjouteSprite(S_RAQUETTE_BAS,"./images/RaquetteVerte32_160.bmp");     
  AjouteSprite(S_RAQUETTE_HAUT,"./images/RaquetteJaune32_160.bmp");     
  AjouteSprite(S_RAQUETTE_GAUCHE,"./images/RaquetteRouge32_160.bmp");     
  AjouteSprite(S_RAQUETTE_DROITE,"./images/RaquetteMauve32_160.bmp");  
}

// Fonctions Ressources
int OuvertureFenetreGraphique()
{
  if (OuvrirGrilleSDL(19,19,32,"!!! SPACE BALLS !!!") < 0)
    return -1;
 
  ChargementImages();
  return 0;
}

int FermetureFenetreGraphique()
{
  FermerGrilleSDL();
  return 0;
}

void DessineMur(int l,int c)
{
  DessineSprite(l,c,S_MUR);
}

void DessineBille(int couleur,int l,int c)
{
  switch(couleur)
  {
    case GRISE : DessineSprite(l,c,S_BILLE_GRISE); break;
    case ROUGE : DessineSprite(l,c,S_BILLE_ROUGE); break;
    case BLEUE : DessineSprite(l,c,S_BILLE_BLEUE); break;
    case JAUNE : DessineSprite(l,c,S_BILLE_JAUNE); break;
    case MAUVE : DessineSprite(l,c,S_BILLE_MAUVE); break;
    case VERTE : DessineSprite(l,c,S_BILLE_VERTE); break;
    case MAGENTA : DessineSprite(l,c,S_BILLE_MAGENTA); break;
    default    : DessineSprite(l,c,S_BILLE_GRISE); break;    
  }
}

void DessineVerrou(int l,int c)
{
  DessineSprite(l,c,S_VERROU);
}

void DessineRaquetteBas(int l,int c)
{
  DessineSprite(l,c-2,S_RAQUETTE_BAS);
}

void DessineRaquetteHaut(int l,int c)
{
  DessineSprite(l,c-2,S_RAQUETTE_HAUT);
}

void DessineRaquetteGauche(int l,int c)
{
  DessineSprite(l-2,c,S_RAQUETTE_GAUCHE);
}

void DessineRaquetteDroite(int l,int c)
{
  DessineSprite(l-2,c,S_RAQUETTE_DROITE);
}

void DessineChiffre(int chiffre,int l,int c)
{
  switch(chiffre)
  {
    case 1 : DessineSprite(l,c,S_NUMERO_UN); break;
    case 2 : DessineSprite(l,c,S_NUMERO_DEUX); break;
    case 3 : DessineSprite(l,c,S_NUMERO_TROIS); break;
    case 4 : DessineSprite(l,c,S_NUMERO_QUATRE); break;
    default : break;    
  }
}
