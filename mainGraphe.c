// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include "Graphe/Graphe.h"
#include "ArbreQuat/ArbreQuat.h"

int main(void){

    Chaines* ch = generationAleatoire(10, 10, 500, 500);
    Reseau* res = reconstitueReseauArbre(ch);

    Graphe* graphe = creerGraphe(res);
    printf("chemin plus court : %d\n", cheminLePlusCourt(graphe, 14, 15));
    libererGraphe(graphe);
    libererRes(res);
    libererChaine(ch);

    return 0;
}