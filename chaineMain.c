// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Chaines/Chaine.h"
#include "SVGwriter/SVGwriter.h"

int main(int argc, char** argv){ // ./main <fichier lecture> <fichier enregistrement>
    FILE* fread = NULL;
    FILE* fwrite = NULL;

    if (argc==3){
        fread = fopen(argv[1], "r");
        fwrite = fopen(argv[2], "w");
        if (fread == NULL || fwrite == NULL){
            printf("Erreur d'ouverture\n");
            return 1;
        }
    } else {
        printf("Usage: %s <fichier lecture> <fichier enregistrement>\n", argv[0]);
        return 1;
    }
    // Lecture et écriture des chaines
    Chaines* chaine = lectureChaines(fread);
    fclose(fread);
    ecrireChaines(chaine, fwrite);
    fclose(fwrite);

    afficheChainesSVG(chaine, "afficheChaine");

    printf("Longueur de la chaine: %lf\n", longueurChaine(chaine->chaines));

    printf("Longueur totale: %lf\n", longueurTotale(chaine));

    printf("Nombre total de points: %d\n", comptePointsTotal(chaine));

    libererChaine(chaine);
    return 0;
}



// gcc -Wall -o main Chaine.c ChaineMain.c SVGwriter.c -lm