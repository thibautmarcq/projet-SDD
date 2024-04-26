#include "Chaines/Chaine.h"
#include "Hachage/Hachage.h"
#include "ArbreQuat/ArbreQuat.h"

#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv){
    if(argc != 2){
        printf("Usage: %s <taille de la table de Hachage> \n", argv[0]);
        return 1;
    }
    
    clock_t temps_initC;
    clock_t temps_finC;
    double temps_totC;
    
    clock_t temps_initH;
    clock_t temps_finH;
    double temps_totH;
    
    clock_t temps_initAQ;
    clock_t temps_finAQ;
    double temps_totAQ;

    Chaines* ch = generationAleatoire(10, 100, 5000, 5000);

    temps_initC = clock(); 
    Reseau* RC = reconstitueReseauListe(ch);
    temps_finC = clock ();
    temps_totC = ((double)(temps_finC - temps_initC))/CLOCKS_PER_SEC;

    temps_initH = clock(); 
    Reseau* RH = reconstitueReseauHachage(ch, atoi(argv[2]));
    temps_finH = clock ();
    temps_totH = ((double)(temps_finH - temps_initH))/CLOCKS_PER_SEC;

    temps_initAQ = clock(); 
    Reseau* RAQ = reconstitueReseauArbre(ch);
    temps_finAQ = clock ();
    temps_totAQ = ((double)(temps_finAQ - temps_initAQ))/CLOCKS_PER_SEC;

    printf("Temps Chaine : %f\n", temps_totC);
    printf("Temps Hachage : %f\n", temps_totH);
    printf("Temps Arbre Quat : %f\n", temps_totAQ);


    libererChaine(ch);
    libererRes(RC);
    libererRes(RH);
    libererRes(RAQ);
}