#include "Chaines/Chaine.h"
#include "Hachage/Hachage.h"
#include "ArbreQuat/ArbreQuat.h"

#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv){
    clock_t temps_initC;
    clock_t temps_finC;
    double temps_totC;
    
    clock_t temps_initH1;
    clock_t temps_finH1;
    double temps_totH1;

    clock_t temps_initH2;
    clock_t temps_finH2;
    double temps_totH2;

    clock_t temps_initH3;
    clock_t temps_finH3;
    double temps_totH3;
    
    clock_t temps_initAQ;
    clock_t temps_finAQ;
    double temps_totAQ;
    
    srand(time(NULL));

    printf("i\tChaine\tHachage500\tHachage5000\tHachage10000\tArbre\n");
    for(int i = 500; i<=5000; i+=500){
        Chaines* ch = generationAleatoire(i, 100, 5000, 5000);
        // Temps chaines
        temps_initC = clock(); 
        Reseau* RC = reconstitueReseauListe(ch);
        temps_finC = clock ();
        temps_totC = ((double)(temps_finC - temps_initC))/CLOCKS_PER_SEC;
        libererRes(RC);

        // Temps hachage 1
        temps_initH1 = clock();
        Reseau* RH1 = reconstitueReseauHachage(ch, 500);
        temps_finH1 = clock ();
        temps_totH1 = ((double)(temps_finH1 - temps_initH1))/CLOCKS_PER_SEC;
        libererRes(RH1);
        
        // Temps hachage 2
        temps_initH2 = clock();
        Reseau* RH2 = reconstitueReseauHachage(ch, 5000);
        temps_finH2 = clock ();
        temps_totH2 = ((double)(temps_finH2 - temps_initH2))/CLOCKS_PER_SEC;
        libererRes(RH2);

        // Temps hachage 3
        temps_initH3 = clock();
        Reseau* RH3 = reconstitueReseauHachage(ch, 10000);
        temps_finH3 = clock ();
        temps_totH3 = ((double)(temps_finH3 - temps_initH3))/CLOCKS_PER_SEC;
        libererRes(RH3);
        
        // Temps arbre quad
        temps_initAQ = clock();
        Reseau* RAQ = reconstitueReseauArbre(ch);
        temps_finAQ = clock ();
        temps_totAQ = ((double)(temps_finAQ - temps_initAQ))/CLOCKS_PER_SEC;
        libererRes(RAQ);

        // printf("%d\t%f\t%f\t%f\n", i, temps_totC, temps_totH1, temps_totH2);

        // double temps_totC = 1;
        printf("%d\t%f\t%f\t%f\t%f\t%f\n", i, temps_totC, temps_totH1, temps_totH2, temps_totH3,temps_totAQ);
        
    
        libererChaine(ch);
    }
    return 0;
}