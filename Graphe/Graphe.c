// MONO 3
// Maëlle LIU 21204734
// Thibaut MARCQ 21202966

#include <stdio.h>
#include <stdlib.h>

#include "Graphe.h"

Arete* creerArete(int u, int v){
    /* Crée une arête de numéro de sommets u et v */
    Arete* ar = (Arete*)malloc(sizeof(Arete));
    ar->u=u;
    ar->v=v;
    return ar;
}

Cellule_arete* creerCelluleArete(Arete* ar){
    /* Crée une Cellule_arete à partir d'une arête */
    Cellule_arete* ca = (Cellule_arete*)malloc(sizeof(Cellule_arete));
    ca->a = ar;
    ca->suiv = NULL;
    return ca;
}

Sommet* creerSommet(int num, int x, int y){
    /* Crée un sommet à partir de son numéro et ses coordonnées */
    Sommet *som = malloc(sizeof(Sommet));
    som->num = num;
    som->x = x;
    som->y = y;
    som->L_voisin = NULL;
    return som;
}

Commod *creerCommod(int e1, int e2){
    /* Crée une commodité entre les extremités e1 et e2*/
    Commod *com = malloc(sizeof(Commod));
    com->e1 = e1;
    com->e2 = e2;
    return com;
}

///////////////

Graphe* creerGraphe(Reseau* r){
    /* Crée un graphe à partir d'un réseau */
    Graphe* g = (Graphe*)malloc(sizeof(Graphe));
    g->nbsom = r->nbNoeuds;
    g->T_som = (Sommet**)malloc(g->nbsom * sizeof(Sommet*));
    g->gamma = r->gamma;
    g->nbcommod = nbCommodites(r);
    g->T_commod = (Commod *)malloc(g->nbcommod * sizeof(Commod));

    int* map = (int *)malloc((r->nbNoeuds + 1) * sizeof(int));

    CellCommodite* com = r->commodites;

    // Initialisation des sommets et des commodites
    CellNoeud* nd = r->noeuds;
    for (int i = 0; i < g->nbsom; i++, nd = nd->suiv){
        g->T_som[i] = creerSommet(i, nd->nd->x, nd->nd->y);
        map[nd->nd->num] = i;
        for(int j = 0, k = 0 ; j < g->nbcommod || k >= 2; j++){
            if(nd->nd == com->extrA){
                g->T_commod[j].e1 = i; 
                k++;
            }
            if(nd->nd == com->extrB){
                g->T_commod[j].e2 = i;
                k++; 
            }
        }
    }

    // Initialisation des arêtes
    for (int i = 0; i < g->nbsom; i++) {
        CellNoeud* nd = r->noeuds;
        for (CellNoeud* voisin = nd->nd->voisins; voisin != NULL; voisin = voisin->suiv){
            int j = map[voisin->nd->num];
            if (j > i) {
                Arete* ar = creerArete(i, j);
                Cellule_arete* cell_ar = creerCelluleArete(ar);
                cell_ar->suiv = g->T_som[i]->L_voisin;
                g->T_som[i]->L_voisin = cell_ar;
                cell_ar = creerCelluleArete(ar);
                cell_ar->suiv = g->T_som[j]->L_voisin;
                g->T_som[j]->L_voisin = cell_ar;
            }
        }
    }

    free(map);
    return g;
}


int cheminLePlusCourt(Graphe* g, int u, int v) {
    /* Renvoie le plus petit nombre d'arêtes entre deux sommets u et v de g */
    int* visite = (int*)calloc(g->nbsom, sizeof(int)); // tab des sommets visités
    int* distance = (int*)calloc(g->nbsom, sizeof(int)); // tab des distances entre sommets

    S_file* file = (S_file*)malloc(sizeof(S_file)); // file des sommets à visiter
    Init_file(file);

    // début à partir de u
    enfile(file, u);
    visite[u] = 1;
    distance[u] = 0;

    while (!estFileVide(file)) { // tant qu'il y a des sommets à visiter
        int courant = defile(file);
        Cellule_arete* voisins = g->T_som[courant]->L_voisin;

        while (voisins != NULL) {
            int voisin = (voisins->a->u == courant) ? voisins->a->v : voisins->a->u;
            if (!visite[voisin]) {
                enfile(file, voisin);
                visite[voisin] = 1;
                distance[voisin] = distance[courant] + 1;

                if (voisin == v) {
                    int shortestDistance = distance[v];
                    free(visite);
                    free(distance);
                    free(file);
                    return shortestDistance;
                }
            }
            voisins = voisins->suiv;
        }
    }

    free(visite);
    free(distance);
    free(file);
    return -1; // retourne -1 s'il n'y a pas de chemin entre u et v
}