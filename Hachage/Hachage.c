// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966
#include "Hachage.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double clef(double x, double y){
    /* Fonction pour obtenir la clef d'un noeud */
    return y+(x+y)*(x+y+1)/2;
}
// 4.2  La fonction clé donnée nous semble appropriée. 
//      Elle renvoie des clés différentes, à la fois paires et impaires, sur un grand intervalle.


int fonction_H(double k, int M){
    /* Fonction de hachage à partir d'une clé k et de sa taille M */
    double A = (sqrt(5)-1)/2;
    return (int)(M*(k*A-((int)(k*A))));
}

TableHachage* initialise_H(int M){
    TableHachage* th = (TableHachage*)malloc(sizeof(TableHachage));
    th->nbElement = 0;
    th->tailleMax = M;
    CellNoeud** T = (CellNoeud**)malloc(sizeof(CellNoeud*)*M);
    for(int i=0; i<M; i++){
        T[i] = NULL;
    }
    th->T = T;
    return th;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage*H, double x, double y){
    /* Fonction qui retourne un Noeud du réseau R, correspondant au point (x,y) dans la table de hachage H */
    double clefH = clef(x,y);
    int hash = fonction_H(clefH, H->tailleMax);
    CellNoeud* cour = H->T[hash];
    while(cour){ // Parcours de la liste des noeuds
        if (x==cour->nd->x && y==cour->nd->y){
            return cour->nd; // Noeud trouvé
        }
        cour=cour->suiv;
    }
    Noeud* n = creerNoeud(R->nbNoeuds+1, x, y);
    CellNoeud* cell = (CellNoeud*)malloc(sizeof(CellNoeud));
    cell->nd = n;
    cell->suiv=H->T[hash];
    H->T[hash]=cell;
    (H->nbElement)++; // Augmentation du nombre d'éléments
    return n;
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma; // recup du gamma de la chaine
    res->commodites = NULL;
    res->noeuds = NULL;
    CellCommodite* com = NULL; //faut d ́eterminer, de manière récursive, dans quelle cellule de l’arbre placer le nœud du réseau*/

    TableHachage* H = initialise_H(M);

    CellChaine* chaine = C->chaines;
    while(chaine){ // Parcours de la chaine
        CellPoint* point = chaine->points;
        while(point->suiv){ // Parcours des points
            // Recherches des points dans le réseau pour les attribuer aux commodités
            Noeud* n1 = rechercheCreeNoeudHachage(res,H,point->x,point->y);
            Noeud* n2 = rechercheCreeNoeudHachage(res,H,point->suiv->x,point->suiv->y);
            if(point == chaine->points){ // Premier point de la chaine
                com = (CellCommodite*)malloc(sizeof(CellCommodite));
                com->extrA = n1; // noeudA
            }
            if(point->suiv == NULL || point->suiv->suiv == NULL){ // Dernier point de la chaine
                com->extrB = n2; // noeudB
                com->suiv = res->commodites;
                res->commodites = com;
                com = NULL;
            }
            
            rechercheCreeVoisin(n1,n2); // Si n2 n'est pas encore dans les voisins de n1 le rajoute au voisin de n1 sinon ne fais rien
            rechercheCreeVoisin(n2,n1); // Si n1 n'est pas encore dans les voisins de n2 le rajoute au voisin de n2 sinon ne fais rien

            point = point->suiv;
        }
        chaine = chaine->suiv;
    }
    libererHachage(H);
    return res;
}


void libererHachage(TableHachage* tab) {
    if (tab != NULL) {
        if (tab->T != NULL) {
            for (int i = 0; i < tab->tailleMax; i++) {
                CellNoeud* current = tab->T[i];
                while (current != NULL) {
                    CellNoeud* next = current->suiv;
                    free(current);
                    current = next;
                }
            }
            free(tab->T);
        }
        free(tab);
    }
}