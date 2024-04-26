#ifndef __HACHAGE_H__
#define __HACHAGE_H__

#include "../Reseau/Reseau.h"

typedef struct{
	int nbElement; //pas necessaire ici
	int tailleMax;
	CellNoeud** T;
} TableHachage;

double clef(double x, double y);
int fonction_H(double k, int M);
TableHachage* initialise_H(int M);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage*H, double x, double y);
Reseau* reconstitueReseauHachage(Chaines *C, int M);
void libererHachage(TableHachage* tab);

#endif