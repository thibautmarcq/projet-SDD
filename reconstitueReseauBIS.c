// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SVGwriter/SVGwriter.h"
#include "Chaines/Chaine.h"
#include "Reseau/Reseau.h"
#include "Hachage/Hachage.h"
#include "ArbreQuat/ArbreQuat.h"

int main(int argc, char** argv){
    srand(time(NULL));
    if(argc != 2){
        printf("Usage: %s <fichier .cha> <1-liste, 2-table de hachage, 3-arbre>\n", argv[0]);
        return 1;
    }
    
    if(atoi(argv[1]) == 1){
        Chaines* ch = generationAleatoire(500, 100, 5000, 5000);
        Reseau* R = reconstitueReseauListe(ch);
        FILE* fwrite = fopen("txtReseauBIS.txt", "w");
        ecrireReseau(R, fwrite);
        fclose(fwrite);
        afficheReseauSVG(R, "afficheReseauBIS");
        libererRes(R);
        libererChaine(ch);
    }

    if(atoi(argv[1]) == 2){
        Chaines* ch = generationAleatoire(1, 10, 5000, 5000);
        Reseau* R = reconstitueReseauArbre(ch);
        FILE* fwrite = fopen("txtArbreBIS.txt", "w");
        ecrireReseau(R, fwrite);
        fclose(fwrite);
        afficheReseauSVG(R, "afficheReseauArbreBIS");
        libererRes(R);
        libererChaine(ch);
        
    }
    return 0;
}