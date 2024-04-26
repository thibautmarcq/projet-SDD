// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArbreQuat.h"
#include "../Chaines/Chaine.h"
#include "../Reseau/Reseau.h"

double max(double a, double b){
    /* Permet de récupérer le max entre a et b */
    return (a > b) ? a : b;
}

double min(double a, double b){
    /* Permet de récupérer le min entre a et b */
    return (a < b) ? a : b;
}

void chaineCoordMinMax(Chaines* C, double* xmin, double* xmax, double* ymin, double* ymax){
    /* Met dans des pointeurs les coordonnées x et y minimales et maximales dans une chaine de points */
    CellChaine * chaine = C->chaines;
    *xmax = chaine->points->x;
    *ymax = chaine->points->x;
    *xmin = chaine->points->x;
    *ymin = chaine->points->y;
    while(chaine){ // Parcourt de la chaine
        CellPoint *point = chaine->points;
        while(point->suiv){ // Parcourt des points de la chaine et détermination des max et min
            *xmax = max(point->x, *xmax);
            *ymax = max(point->y, *ymax);
            *xmin = min(point->x, *xmin);
            *ymin = min(point->y, *ymin);
            point = point->suiv;
        }
        chaine = chaine->suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
    /* Crée une cellule d'arbre quaternaire de centre xc,yc et de cotés coteX, coteY */
    ArbreQuat* tree = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    tree->xc=xc;
    tree->yc=yc;
    tree->coteX=coteX;
    tree->coteY=coteY;
    tree->noeud=NULL;
    tree->so=NULL;
    tree->se=NULL;
    tree->no=NULL;
    tree->ne=NULL;

    return tree;
}


void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
    if (*a==NULL){ // Arbre vide
        double xc,yc;
        double coteX = parent->coteX/2; // coté divisé par 2
        double coteY = parent->coteY/2;
        int h = 0, g = 0; // booléens utilisés pour placer le noeud
        
        if(n->x <= parent->xc){
            g = 1;
            xc = parent->xc/2;
        }else{
            xc = parent->xc+coteX/2;
        }
        if (n->y >= parent->yc){
            h=1;
            yc = parent->yc+coteY/2;
        } else {
            yc = parent->yc/2;
        }
        *a = creerArbreQuat(xc,yc,coteX, coteY);
        (*a)->noeud = n;
        if(g == 1 && h == 1){
            parent->no = *a;
        }
        if(g==1 && h==0){
            parent->so = *a;
        } 
        if(g == 0 && h == 0){
            parent->se = *a;
        } 
        if(g == 0 && h == 1){
            parent->ne = *a;
        }
        return;
    }
    //////////////////
    if((*a)->noeud != NULL){ // Feuille
        ArbreQuat* b = NULL;
        ArbreQuat* c = NULL;

        insererNoeudArbre(n, &b,*a);
        insererNoeudArbre((*a)->noeud, &c,*a);
        (*a)->noeud = NULL;
        return;
    }
    //////////////////

    // else
    // Cellule interne, on regarde dans quelle brancher placer le noeud en fct du centre de l'arbre actuel
    if ((n->x < (*a)->xc) && (n->y > (*a)->yc)){
        insererNoeudArbre(n, &((*a)->no), *a);
        return;
    }
    if ((n->x < (*a)->xc)&&(n->y < (*a)->yc)){
        insererNoeudArbre(n, &((*a)->so), *a);
        return;
    }
    if ((n->x > (*a)->xc)&&(n->y > (*a)->yc)){
        insererNoeudArbre(n, &((*a)->ne), *a);
        return;
    }
    if ((n->x > (*a)->xc)&&(n->y < (*a)->yc)){
        insererNoeudArbre(n, &((*a)->se), *a);
        return;
    }
}


Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    /* Recherche et renvoie un noeud de l'arbre a, le crée et le renvoie si inexistant */
    if ((*a) == NULL){ // l'arbre actuel est vide
        Noeud* newNode = rechercheCreeNoeudListe(R, x, y); // on recherche / crée le noeud dans le reseau
        insererNoeudArbre(newNode, a, parent); // on l'insert dans l'arbre
        return newNode;
    }
    if (((*a)->noeud!=NULL)){ // feuille
        Noeud* actualNode = (*a)->noeud;
        if ((actualNode->x == x)&&(actualNode->y == y)){ // le noeud que l'on recherche
            return actualNode; 
        } else { // la feuille ne correspond pas au noeud recherché => on insert le noeud
            Noeud* newNode = rechercheCreeNoeudListe(R, x, y); // on recherche / crée le noeud dans le reseau
            insererNoeudArbre(newNode, a, parent); // on l'insert dans l'arbre
            return newNode;
        } 
    } else {
    // else ~ cas de base, on navigue dans l'arbre puisqu'on est sur un noeud
        if ((x < (*a)->xc) && (y > (*a)->yc)){
            return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
        }
        if ((x < (*a)->xc)&&(y < (*a)->yc)){
            return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
        }
        if ((x > (*a)->xc)&&(y > (*a)->yc)){
            return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
        }
        if ((x > (*a)->xc)&&(y < (*a)->yc)){
            return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
        }
    }
    return NULL;
}

Reseau* reconstitueReseauArbre(Chaines* C){
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma; // recup du gamma de la chaine
    res->commodites = NULL;
    res->noeuds = NULL;
    CellCommodite* com = NULL; //faut d ́eterminer, de manière récursive, dans quelle cellule de l’arbre placer le nœud du réseau*/

    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);
    double coteX = xmax-xmin; 
    double coteY = ymax-ymin;
    double xc = xmin + coteX/2.0;
    double yc = ymin + coteY/2.0;

    ArbreQuat* a =  NULL;
    ArbreQuat* b = creerArbreQuat(xc,yc,coteX,coteY);

    CellChaine* chaine = C->chaines;

    while(chaine){ // Parcours de la chaine
        CellPoint* point = chaine->points;
        while(point->suiv){ // Parcours des points
            // Recherches des points dans le réseau pour les attribuer aux commodités
            Noeud* n1 = rechercheCreeNoeudArbre(res,&a, b ,point->x,point->y);
            Noeud* n2 = rechercheCreeNoeudArbre(res,&a, b ,point->suiv->x,point->suiv->y);
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
            if(rechercheVoisin(n1, n2)){ // Si n2 n'est pas encore dans les voisins de n1
                CellNoeud* voisinA = (CellNoeud*)malloc(sizeof(CellNoeud));
                voisinA->nd = n2; 
                voisinA->suiv = n1->voisins;
                n1->voisins = voisinA;
            }
            if(rechercheVoisin(n2, n1)){ // Si n1 n'est pas encore dans les voisins de n2
                CellNoeud* voisinB = (CellNoeud*)malloc(sizeof(CellNoeud));
                voisinB->nd = n1; 
                voisinB->suiv = n2->voisins;
                n2->voisins = voisinB;
            }
            point = point->suiv;
        }
        chaine = chaine->suiv;
    }
    libererArbre(b);
    return res;
}


void libererArbre(ArbreQuat* arbre){
    /* Libère un arbre à partir de sa racine, ne libère pas les noeuds */
    if (arbre != NULL) {
        libererArbre(arbre->so);
        libererArbre(arbre->se);
        libererArbre(arbre->no);
        libererArbre(arbre->ne);
        free(arbre);
    }
}