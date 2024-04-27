// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SVGwriter/SVGwriter.h"
#include "Chaines/Chaine.h"
#include "Reseau/Reseau.h"
#include "Hachage/Hachage.h"
#include "ArbreQuat/ArbreQuat.h"

int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage: %s <fichier .cha> <1-liste, 2-table de hachage, 3-arbre>\n", argv[0]);
        return 1;
    }
    FILE* fread = fopen(argv[1], "r");
    if (!fread) {
        printf("Erreur d'ouverture\n");
        return 1;
    }
    
    if(atoi(argv[2]) == 1){
        Chaines* ch = lectureChaines(fread);
        Reseau* R = reconstitueReseauListe(ch);
        FILE* fwrite = fopen("txtReseau.txt", "w");
        ecrireReseau(R, fwrite);
        fclose(fwrite);
        afficheReseauSVG(R, "afficheReseau");
        libererRes(R);
        libererChaine(ch);
    }

    if(atoi(argv[2]) == 2){
        Chaines* ch = lectureChaines(fread);
        Reseau* R = reconstitueReseauHachage(ch, 10000);
        FILE* fwrite = fopen("txtHachage.txt", "w");
        ecrireReseau(R, fwrite);
        fclose(fwrite);
        afficheReseauSVG(R, "afficheReseauHachage");
        libererRes(R);
        libererChaine(ch);
        
    }

    if(atoi(argv[2]) == 3){
        Chaines* ch = lectureChaines(fread);
        Reseau* R = reconstitueReseauArbre(ch);
        FILE* fwrite = fopen("txtArbre.txt", "w");
        ecrireReseau(R, fwrite);
        fclose(fwrite);
        afficheReseauSVG(R, "afficheReseauArbre");
        libererRes(R);
        libererChaine(ch);
    }

    fclose(fread);
    return 0;
}