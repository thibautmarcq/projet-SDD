// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArbreQuat.h"
#include "../Chaines/Chaine.h"
#include "../Reseau/Reseau.h"


void chaineCoordMinMax(Chaines* C,double* xmin,double* ymin,double *xmax,double *ymax){
    /* Met dans des pointeurs les coordonnées x et y minimales et maximales dans une chaine de points */
    CellChaine *chaine=C->chaines;
    while(chaine){ // Parcourt de la chaine
        CellPoint *points=chaine->points;
        while(points){ // Parcourt des points de la chaine et détermination des max et min
            double x=points->x;
            double y=points->y;
            if(x<*xmin){
                *xmin=x;
            }if(x>*xmax){
                *xmax=x;
            }
            if(y<*ymin){
                *ymin=y;
            }if(y>*ymax){
                *ymax=y;
            }
            points=points->suiv;
        }
        chaine=chaine->suiv;
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
        double coteX = parent->coteX/2; // coté divisé par 2
        double coteY = parent->coteY/2;
        double yc, xc;
        
        if(n->x < parent->xc){
            xc = parent->xc/2;
        }else{
            xc = parent->xc+coteX/2;
        }
        if (n->y >= parent->yc){
            yc = parent->yc+coteY/2;
        } else {
            yc = parent->yc/2;
        }
        *a = creerArbreQuat(xc,yc,coteX, coteY);
        (*a)->noeud = n;
        return;
    }
    //////////////////
    if((*a)->noeud != NULL){ // Feuille
        Noeud* c = (*a)->noeud;
        (*a)->noeud = NULL;

        insererNoeudArbre(n, a,parent);
        insererNoeudArbre(c, a,parent);
        return;
    }
    //////////////////

    // else
    // Cellule interne, on regarde dans quelle brancher placer le noeud en fct du centre de l'arbre actuel
    if ((n->x < (*a)->xc) && (n->y >= (*a)->yc)){
        insererNoeudArbre(n, &((*a)->no), *a);
        return;
    }
    if ((n->x < (*a)->xc)&&(n->y < (*a)->yc)){
        insererNoeudArbre(n, &((*a)->so), *a);
        return;
    }
    if ((n->x >= (*a)->xc)&&(n->y >= (*a)->yc)){
        insererNoeudArbre(n, &((*a)->ne), *a);
        return;
    }
    if ((n->x >= (*a)->xc)&&(n->y < (*a)->yc)){
        insererNoeudArbre(n, &((*a)->se), *a);
        return;
    }
}


Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
    /* Recherche et renvoie un noeud de l'arbre a, le crée et le renvoie si inexistant */
    if ((*a) == NULL){ // l'arbre actuel est vide
        Noeud* newNode = creerNoeud(R->nbNoeuds+1, x,y); // on recherche / crée le noeud dans le reseau
        CellNoeud* newlst = (CellNoeud*)malloc(sizeof(CellNoeud)); // Création de la liste nds
        newlst->nd = newNode; // Ajout du nd crée dedans
        newlst->suiv = R->noeuds; // Ajout des noeuds précédents à la suite
        R->noeuds = newlst; // Ajout de la nvelle liste dans le reseau
        R->nbNoeuds += 1; // Augmente le nb de nd du reseau
        insererNoeudArbre(newNode, a, parent); // on l'insert dans l'arbre
        return newNode;
    }
    if (((*a)->noeud!=NULL)){ // feuille
        Noeud* actualNode = (*a)->noeud;
        if ((actualNode->x == x)&&(actualNode->y == y)){ // le noeud que l'on recherche
            return actualNode; 
        } 
    // else ~ cas de base, on navigue dans l'arbre puisqu'on est sur un noeud
    }
    if ((x <= (*a)->xc) && (y >= (*a)->yc)){
        rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
    }
    if ((x <= (*a)->xc)&&(y <= (*a)->yc)){
        rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
    }
    if ((x >= (*a)->xc)&&(y >= (*a)->yc)){
        rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
    }
    if ((x >= (*a)->xc)&&(y <= (*a)->yc)){
        rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
    }
    return R->noeuds->nd;
}

Reseau* reconstitueReseauArbre(Chaines* C){
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma; // recup du gamma de la chaine
    res->commodites = NULL;
    res->noeuds = NULL;
    CellCommodite* com = NULL; 

    double xmin=0, ymin=0, xmax=0, ymax=0;
    chaineCoordMinMax(C,&xmin,&ymin,&xmax,&ymax);
    double coteX = xmax-xmin; 
    double coteY = ymax-ymin;
    double xc = xmin + coteX/2.0;
    double yc = ymin + coteY/2.0;

 
    ArbreQuat* b = creerArbreQuat(xc,yc,coteX,coteY);


    CellChaine* chaine = C->chaines;

    while(chaine){ // Parcours de la chaine
        CellPoint* point = chaine->points;
        while(point->suiv){ // Parcours des points
            // Recherches des points dans le réseau pour les attribuer aux commodités
            Noeud* n1 = rechercheCreeNoeudArbre(res,&b, NULL ,point->x,point->y);
            Noeud* n2 = rechercheCreeNoeudArbre(res,&b, NULL ,point->suiv->x,point->suiv->y);
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
