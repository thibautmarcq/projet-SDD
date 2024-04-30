#include "Graphe/Graphe.h"
#include "Reseau/Reseau.h"
#include "ArbreQuat/ArbreQuat.h"

int main(void){

    Chaines* ch = generationAleatoire(10, 10, 500, 500);
    Reseau* res = reconstitueReseauArbre(ch);

    Graphe* graphe = creerGraphe(res);
    afficherGraphe(graphe);
    // printf("chemin plus court : %d\n", cheminLePlusCourt(graphe, 14, 15));

    return 0;
}