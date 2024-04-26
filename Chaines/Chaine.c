// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Chaine.h"
#include "../SVGwriter/SVGwriter.h"

Chaines* lectureChaines(FILE *f){
    /* Renvoie la chaine correspondante à un fichier .cha */
    Chaines* chaine = (Chaines*)malloc(sizeof(Chaines));
    char buffer[512]; 
    fgets(buffer, 512, f);
    sscanf(buffer, "NbChain: %d", &(chaine->nbChaines)); // Récupération du nombre de chaines
    fgets(buffer, 512, f);
    sscanf(buffer, "Gamma: %d", &(chaine->gamma)); // Recuperation de gamma

    chaine->chaines = NULL; // La liste de chaine est vide au départ    
    for (int n=0; n<chaine->nbChaines; n++){ // Itération sur le nombre de chaines
        CellChaine* cellch = (CellChaine *)malloc(sizeof(CellChaine)); // Création de la liste de chaine
        int nbPoints;
        fscanf(f, "%d %d", &(cellch->numero), &nbPoints); // Extraction des données (numéro, nbpoints)

        cellch->points = NULL; // La liste de points est vide aud départ
        for(int i=0; i<nbPoints; i++){ // itération pour extraire les points 
            CellPoint* points = (CellPoint*)malloc(sizeof(CellPoint));
            fscanf(f, "%lf %lf", &(points->x), &(points->y)); // Lecture des coordonnées x et y

            points->suiv = cellch->points;
            cellch->points = points;
        }
        // Remet cellch au début
        cellch->suiv = chaine->chaines;
        chaine->chaines = cellch;
    }
    return chaine;
}

void ecrireChaines(Chaines *C, FILE *f){
    /* Ecrit les infos de la chaine (liste) dans un fichier */
    fprintf(f, "NbChain: %d\n", C->nbChaines); 
    fprintf(f, "Gamma: %d\n", C->gamma);

    CellChaine* lstCh = C->chaines;

    while (lstCh){ // Parcourt les chaines
        fprintf(f, "%d ", lstCh->numero); // Ajoute le numero de la chaine
        CellPoint* lstPt = lstCh->points;
        int nbPoints = 0; // compteur du nb de points
        char bufferPt[512] = "";
        while (lstPt){
            char tempBuffer[512];
            sprintf(tempBuffer, "%.2lf %.2lf", lstPt->x, lstPt->y); // copie les infos dans le string tempBuffer
            strcat(bufferPt, " "); // Ajoute l'espace
            strcat(bufferPt, tempBuffer); // Ajoute le point au buffer
            nbPoints++;
            lstPt=lstPt->suiv;
        }
        fprintf(f, "%d %s\n", nbPoints, bufferPt); // Ajoute le reste de la ligne
        lstCh=lstCh->suiv;
    }
}

void libererChaine(Chaines* c){
    /* Libère une chaine */
    if (c == NULL) { // Si la chaine est vide
        return;
    }
    // Libération des chaines ⏬
    CellChaine* cellCh = c->chaines; // Ptr vers le début de la chaine
    while (cellCh){ // Parcours des chaines
        // Liberation des points ⏬
        CellPoint* cellPt = cellCh->points;
        while(cellPt){ // Parcours des points
            CellPoint* tmpPt = cellPt->suiv;
            free(cellPt);
            cellPt = tmpPt;
        }
        CellChaine* tmpCh = cellCh->suiv;
        free(cellCh);
        cellCh=tmpCh;
    }
    free(c); // Libère le pointeur de Chaines
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine* c){
    /* Calcule la longueur d'une chaine (distance entre ses points)*/
    CellPoint* cellPt = c->points;
    double res=0;
    CellPoint* prev = NULL;
    while(cellPt){ // Parcours les points
        if (prev!=NULL){ // Si l'element n'est pas le premier
            res+= sqrt(pow(cellPt->x-prev->x,2)+pow(cellPt->y-prev->y,2)); // Calcul de la distance entre 2pts (avant et actuel)
        }
        prev=cellPt;
        cellPt=cellPt->suiv;
    }
    return res;
}

double longueurTotale(Chaines* C){
    /* Calcule la longueur totale d'une chaine (longueur de toutes ses chaines) */
    double res=0;
    CellChaine* cellCh = C->chaines;
    while (cellCh){ // Parcours les chaines
        res+=longueurChaine(cellCh); // Ajoute la longueur de la chaine en cours au résultat
        cellCh=cellCh->suiv;
    }
    return res;
}

int comptePointsTotal(Chaines *C){
    /* Compte le nombre de points total de la chaine */
    CellChaine* cellCh = C->chaines;
    int nbPoints=0;
    while (cellCh){ // Parcours les chaines
        CellPoint* cellPt = cellCh->points;
        while (cellPt){ // Parcours les points de la chaine en cours
            nbPoints++; // Ajoute un point à chaque pt rencontré
            cellPt=cellPt->suiv;
        }
        cellCh=cellCh->suiv;
    }
    return nbPoints;
}

Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
    /* Crée <nbChaines> chaines de <nbPointsChaine> points entre (0,0) et (xmax,ymax) aléatoirement */
    Chaines* ch = (Chaines*)malloc(sizeof(Chaines));
    ch->gamma = nbChaines;
    ch->nbChaines = nbChaines;
    ch->chaines = NULL; // La liste de chaine est vide au départ

    for(int i = 0; i<nbChaines; i++){
        // Création des chaines
        CellChaine* cellch = (CellChaine *)malloc(sizeof(CellChaine));
        cellch->numero = i;
        cellch->points=NULL;
        for(int j = 0; j<nbPointsChaine; j++){
            // Création des points à l'intérieur de la chaine
            CellPoint* points = (CellPoint*)malloc(sizeof(CellPoint));
            points->x = (double)(rand() % (xmax+1));
            points->y = (double)(rand() % (ymax+1));
            points->suiv=cellch->points;
            cellch->points = points;
        }
        cellch->suiv = ch->chaines;
        ch->chaines = cellch;
    }
    return ch;
}
