#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

enum {
    Rouge, Jaune, Vert, Bleu, Orange, Blanc, Violet, Fushia
} couleur;
typedef enum couleur boule;

/* affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */

void affiche(int T[], unsigned int l) {
    /* affiche les l premiers elements du tableau T */

    unsigned int i;
    printf("[");
    for (i = 0; i < l; i++) {
        printf(" %d ", T[i]);
    };
    printf("]\n");
}

/* Rempli les l premieres cases du tableau par un nombre aléatoire entre 0 et 7
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */

void genere(int T[], unsigned int l) {
    /* initialise les l premiers elements du tableau T */

    unsigned int i;
    for (i = 0; i < l; i++) {
        T[i] = (random() % 8); // Genere un nombre aléatoire entre 0 et 7
    };
}

int CouleurVersEntier(char *coul) {

    if (strcmp(coul, "Rouge") == 0) {
        return 0;
    } else {
        if (strcmp(coul, "Jaune") == 0) {
            return 1;
        } else {
            if (strcmp(coul, "Vert") == 0) {
                return 2;
            } else {
                if (strcmp(coul, "Bleu") == 0) {
                    return 3;
                } else {
                    if (strcmp(coul, "Orange") == 0) {
                        return 4;
                    } else {
                        if (strcmp(coul, "Blanc") == 0) {
                            return 5;
                        } else {
                            if (strcmp(coul, "Violet") == 0) {
                                return 6;
                            } else {
                                return 7;
                            }
                        }
                    }
                }
            }
        }
    }
}

int recherche(int T[], unsigned int l, unsigned x) {
    /* Recherche de l'entier x dans le tableau T */

    int existe;

    unsigned int i;

    i = 0;
    while (i < l && T[i] != x) {
        i = i + 1;
    };

    existe = 0;

    if (i < l) {
        existe = 1;
    };

    return existe;
}

int rechercheP(int T[], unsigned int l, unsigned x) {
    /* Recherche de la position de l'entier x dans le tableau T */

    unsigned int i;

    int position = 0;

    i = 0;
    while (i < l && T[i] != x) {
        i = i + 1;
    };

    if (i < l) {
        position = i;
    };

    return position;
}

/* compare la proposition avec la combinaison secrète et renvoie un tableau d'indicateurs */

void Reponse(int T_prop[], int T_partie[], int T_indic[], int level) {

    int Temp_prop[level];
    int Temp_partie[level];

    unsigned int i;

    /* les temporaires */

    for (i = 0; i < level; i++) {

        Temp_partie[i] = T_partie[i];
        Temp_prop[i] = T_prop[i];

    }

    /* Analyse des jetons bien placés */

    for (i = 0; i < level; i++) {
        if (Temp_prop[i] == Temp_partie[i]) {
            T_indic[i] = 2;
            Temp_prop[i] = -1;
            Temp_partie[i] = -1;
        }
    }

    /* Analyse des jetons non présents */

    for (i = 0; i < level; i++) {
        if (Temp_prop[i] != -1) {
            /* Recherche */
            int estPresent;

            estPresent = recherche(Temp_partie, level, Temp_prop[i]);
            if (estPresent == 0) {
                Temp_prop[i] = -1;
            }
        }
    }

    /* Analyse des jetons mal placés */

    for (i = 0; i < level; i++) {
        if (Temp_prop[i] != -1) {
            /* Recherche */
            int estPresent;

            estPresent = recherche(Temp_partie, level, Temp_prop[i]);
            if (estPresent == 1) {
                T_indic[i] = 1;
                Temp_partie[rechercheP(Temp_partie, level, Temp_prop[i])] = -1;
            }
        }
    }

}

void proposition(int T[], unsigned int niv) {

    char Boule[10];

    unsigned int i;
    for (i = 0; i < niv; i++) {

        printf("Proposer une couleur (rang %d) : ", i);
        scanf("%s", Boule);
        T[i] = CouleurVersEntier(Boule);
    }
}

int main() {

    int level;

    // choix du niveau de jeu
    printf("Veuillez choisir le niveau de jeu (entre 1 et 8) : ");
    scanf("%d", &level);

    // creation combinaison à trouver aléatoirement
    int Tab_partie[level];
    srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
    genere(Tab_partie, level);

    printf("Rouge : 0, Jaune : 1, Vert : 2, Bleu : 3, Orange : 4, Blanc : 5, Violet : 6, Fushia : 7\n");

    printf("Code secret : ");
    affiche(Tab_partie, level);

    // boucle de jeu

    int Tab_proposition[level];

    //int x =1;
    //while (x==1) {

    // demander au joueur de faire une proposition
    proposition(Tab_proposition, level);
    printf("Proposition du joueur : ");
    affiche(Tab_proposition, level);
    //}

    int Indic[level];
    int i;
    for (i = 0; i < level; i++) {

        Indic[i] = 0;
    }

    Reponse(Tab_proposition, Tab_partie, Indic, level);

    printf("Indicateurs : ");
    affiche(Indic, level);
    printf("2 : bonne place, 1 : existe mais mal place, 0 : n'existe pas\n");



    return 0;

}