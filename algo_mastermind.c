#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int NBTOTALESSAI = 15;


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

/**
 * Rempli les l premieres cases du tableau par un nombre aléatoire entre 0 et 7
 * @param T: le tableau qui contiendra le code secret
 * @param l: le nombre de boules dans le tableau
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

/**
 * Recherche un entier dans un tableau et renvoit sa position
 * @param T: le tableau 
 * @param l  : le nbre d'element du tableau
 * @param x: l'entier recherché
 * @return : position renvoyée; -1 si pas trouvée.
 */
int recherche(int T[], unsigned int l, unsigned x) {
    /* Recherche de l'entier x dans le tableau T */
    int position = -1;
    unsigned int i;
    i = 0;

    while (i < l && T[i] != x) {
        i++;
    };

    if (i < l) {
        position = i;
    };
    return position;
}

/**
 * compare la proposition avec le code secrète et renvoie un tableau d'indicateurs
 * @param T_prop : tableau de proposition
 * @param T_code : tableau du code secret
 * @param T_indic : tableau des indicateur
 * @param level : taille des tableaux
 */
void Reponse(int T_prop[], int T_code[], int T_indic[], int level) {

    int Temp_prop[level];
    int Temp_code[level];
    unsigned int i;

    /* les temporaires */
    for (i = 0; i < level; i++) {
        Temp_code[i] = T_code[i];
        Temp_prop[i] = T_prop[i];
    }

    /* Analyse des jetons bien placés */
    for (i = 0; i < level; i++) {
        if (Temp_prop[i] == Temp_code[i]) {
            T_indic[i] = 2;
            Temp_prop[i] = -1;
            Temp_code[i] = -1;
        }
    }

    /* Analyse des jetons non présents */
    for (i = 0; i < level; i++) {
        if (Temp_prop[i] != -1) {
            /* Recherche */
            int position;
            position = recherche(Temp_code, level, Temp_prop[i]);
            if (position == -1) {
                Temp_prop[i] = -1;
            }
        }
    }

    /* Analyse des jetons mal placés */
    for (i = 0; i < level; i++) {
        if (Temp_prop[i] != -1) {
            /* Recherche */
            int position;
            position = recherche(Temp_code, level, Temp_prop[i]);
            if (position != -1) {
                T_indic[i] = 1;
                Temp_code[position] = -1;
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

void rempliAZero(int tab[], int nbElt) {
    int i;
    for (i = 0; i < nbElt; i++) {
        tab[i] = 0;
    }
}

int estGagne(int tab[], int level) {
    int i = 0;
    while (i < level && tab[i] == 2) {
        i++;
    }
    return i == level;
}

int main() {
    int level;

    // choix du niveau de jeu
    printf("Veuillez choisir le niveau de jeu (entre 1 et 8) : ");
    scanf("%d", &level);

    // creation aléatoire du code secret dans tableau TabPartie
    int tabCode[level];
    srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
    genere(tabCode, level);

    printf("0 = Rouge,  1 = Jaune, 2 = Vert, 3 = Bleu, 4 = Orange, 5 = Blanc, 6 = Violet, 7 = Fushia\n");
    printf("Code secret : ");
    affiche(tabCode, level);

    // boucle de jeu
    int boucle = 1;
    int gagne = 0;
    int nbessai = 0;
    int tabProposition[level];
    while (boucle == 1) {

        // Rempli le tableau des indicateurs avec 0;
        int tabIndic[level];
        rempliAZero(tabIndic, level);

        // demander au joueur de faire une proposition
        proposition(tabProposition, level);
        printf("Proposition du joueur : ");
        affiche(tabProposition, level);

        // Rempli tabIndic en fonction de la proposition faite
        Reponse(tabProposition, tabCode, tabIndic, level);

        printf("Indicateurs : ");
        affiche(tabIndic, level);
        printf("2 = bonne place, 1 = existe mais mal place, 0 = n'existe pas\n");

        gagne = estGagne(tabIndic, level);
        nbessai++;
        if (gagne == 1) {
            boucle = 0;
            printf("Vous avez gagné en %d coups !\n", nbessai);
        } else {
            if (nbessai == 15) {
                printf("Vous avez perdu !\n ");
            } else {
                printf("Il vous reste %d essais.\n", NBTOTALESSAI-nbessai);
            }
        }
    }
    
    return 0;
}

