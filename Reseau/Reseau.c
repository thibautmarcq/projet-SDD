// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../SVGwriter/SVGwriter.h"
#include "Reseau.h"

Noeud * creerNoeud(int num,double x,double y){
    /* Crée un noeud */
    Noeud * nd=(Noeud *)malloc(sizeof(Noeud));
    nd->num=num;
    nd->x=x;
    nd->y=y;
    nd->voisins=NULL;
    return nd;
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
    /* Retourne le noeud de coo (x,y) dans le reseau R s'il existe, le crée sinon */
    CellNoeud* lstnoeud = R->noeuds;
    // Parcours des noeuds du réseau
    while(lstnoeud){
        Noeud* n = lstnoeud->nd;
        if(n->x==x && n->y==y)
            return n;
        lstnoeud = lstnoeud->suiv;
    }
    // Noeud non trouvé : création et insertion
    Noeud* n = creerNoeud(R->nbNoeuds+1, x,y);
    CellNoeud* newlst = (CellNoeud*)malloc(sizeof(CellNoeud)); // Création de la liste nds
    newlst->nd = n; // Ajout du nd crée dedans
    newlst->suiv = R->noeuds; // Ajout des noeuds précédents à la suite
    R->noeuds = newlst; // Ajout de la nvelle liste dans le reseau
    R->nbNoeuds += 1; // Augmente le nb de nd du reseau
    return n;
}

int rechercheVoisin(Noeud* n1, Noeud*n2){
    /* Recherche si n2 est présent dans les voisins de n1 si oui renvoie 0 sinon 1*/
    CellNoeud* voisin = n1->voisins;
    while(voisin){ // On parcours les voisins de n1
        if(voisin->nd == n2){
            return 0; // n2 deja dans la liste de voisins de n1
        }
        voisin =voisin->suiv;
    }
    return 1; // Noeud non trouvé dans les voisins
}


Reseau* reconstitueReseauListe(Chaines *C){
    /* Reconstitue un réseau à partir d'une chaine */
    // Initialisation du ementerreseau
    Reseau* res = (Reseau*)malloc(sizeof(Reseau));
    res->nbNoeuds = 0;
    res->gamma = C->gamma; // recup du gamma de la chaine
    res->commodites = NULL;
    res->noeuds = NULL;
    CellCommodite* com = NULL;

    CellChaine* chaine = C->chaines;
    while(chaine){ // Parcours de la chaine
        CellPoint* point = chaine->points;
        while(point->suiv){ // Parcours des points
            // Recherches des points dans le réseau pour les attribuer aux commodités
            Noeud* n1 = rechercheCreeNoeudListe(res,point->x,point->y);
            Noeud* n2 = rechercheCreeNoeudListe(res,point->suiv->x,point->suiv->y);
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

            if(rechercheVoisin(n1,n2)){ // Si n2 n'est pas encore dans les voisins de n1
                CellNoeud* voisinA = (CellNoeud*)malloc(sizeof(CellNoeud));
                voisinA->nd = n2; 
                voisinA->suiv = n1->voisins;
                n1->voisins = voisinA;
            
            }

            if(rechercheVoisin(n2,n1)){ // Si n1 n'est pas encore dans les voisins de n2
                CellNoeud* voisinB = (CellNoeud*)malloc(sizeof(CellNoeud));
                voisinB->nd = n1; 
                voisinB->suiv = n2->voisins;
                n2->voisins = voisinB;
            
            }

            point = point->suiv;
        }
        chaine = chaine->suiv;
    }
    return res;
}


int nbLiaisons(Reseau *R){
    /* Renvoie le nombre de liaisons d'un réseau (cable, repere par ses deux extremités, tous les arcs) */
    int nbLiaisons=0;
    CellNoeud* cellNd = R->noeuds;

    while(cellNd) { // On parcourt les noeuds
        CellNoeud* voisins = cellNd->nd->voisins;
        while(voisins) { // On parcours les voisins du noeud (+on les compte)
            nbLiaisons++;
            voisins = voisins->suiv;
        }
        cellNd = cellNd->suiv;
    }

    return nbLiaisons / 2; // Les voisins sont enregistrés pour les deux noeuds (a est voisin de b, b est voisin de a => voisins 2fois)
}

int nbCommodites(Reseau *R){
    /* Renvoie le nombre de commodités d'un réseau (cable entiers, sans compter les liaisons au milieu) */
    CellCommodite* cellComm = R->commodites;
    int nbCommodites=0;
    while (cellComm){ // On parcourt les commodites et on les compte
        nbCommodites++;
        cellComm=cellComm->suiv;
    }
    return nbCommodites;
}

void ecrireReseau(Reseau *R, FILE *f){
    fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n", R->nbNoeuds, nbLiaisons(R), nbCommodites(R), R->gamma);
    
    CellNoeud* noeuds = R->noeuds;
    while(noeuds){
        fprintf(f, "v %d %lf %lf\n", noeuds->nd->num, noeuds->nd->x, noeuds->nd->y);
        noeuds=noeuds->suiv;
    }
    fprintf(f,"\n");

    CellNoeud* cellNd = R->noeuds;
    while(cellNd) { // On parcourt les noeuds
        CellNoeud* voisins = cellNd->nd->voisins;
        while(voisins) { // On parcours les voisins du noeud 
            if(voisins->nd->num<cellNd->nd->num){ // On vérifie si le num du nd est inf à celui de son voisin (éviter les doubles)
                fprintf(f, "l %d %d\n", voisins->nd->num, cellNd->nd->num);
            }
            voisins = voisins->suiv;
        }
        cellNd = cellNd->suiv;
    }
    fprintf(f,"\n");
    
    CellCommodite* com = R->commodites;
    while(com){ // On parcours les commodités
        fprintf(f, "k %d %d\n", com->extrA->num, com->extrB->num);
        com = com->suiv;
    }
    fprintf(f,"\n");
    
}

void libererNoeuds(CellNoeud* CellNd){
    /* Libère tous les noeuds d'une liste de noeuds*/
    CellNoeud* tmp = NULL;
    while(CellNd){
        CellNoeud* voisins = CellNd->nd->voisins;
        while(voisins){ // Libère les pointeurs vers les voisins d'un reseauR            
            tmp=voisins->suiv;
            free(voisins);
            voisins=tmp;
        }
        tmp=CellNd->suiv;
        free(CellNd->nd);
        free(CellNd);
        CellNd=tmp;
    }    
}

void libererComs(CellCommodite* com){
    /* Libère les pointeurs vers les commodites d'un reseau */
    while(com){
        CellCommodite* tmp = com->suiv;
        free(com);
        com = tmp;
    }
}

void libererRes(Reseau* R){
    /* Libère les pointeurs d'un reseau */
    libererNoeuds(R->noeuds);
    libererComs(R->commodites);
    free(R);
}


void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}