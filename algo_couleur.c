#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

const int NBTOTALESSAI = 15;

enum {
    Rouge, Jaune, Vert, Bleu, Orange, Blanc, Violet, Fushia
} couleur;
typedef enum couleur boule;

int EntierVersCouleur(int c) {

    switch (c) {
        case 0:
            return 9;
            break;
        case 1:
            return 11;
            break;
        case 2:
            return 10;
            break;
        case 3:
            return 12;
            break;
        case 4:
            return 208;
            break;
        case 5:
            return 255;
            break;
        case 6:
            return 129;
            break;
        case 7:
            return 201;
            break;
    }
    return 0;
}

int EntierVersIndicateur(int c) {

    switch (c) {
        case 0:
            return 0;
            break;
        case 1:
            return 7;
            break;
        case 2:
            return 9;
            break;
    }
    return 0;
}

/* Affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 * parametre hide : booléen cachant la combinaison
 * parametre ind : booléen concernant les indicateurs
 */

void affiche(int T[], unsigned int l, int hide, int ind) {
    /* affiche les l premiers elements du tableau T */

    unsigned int i;

    printf("[ ");
    if (hide == 0 && ind == 0) {
        for (i = 0; i < l; i++) {
            printf("\x1B[48;5;%dm  ", EntierVersCouleur(T[i]));
            printf("\x1B[0m ");
            //         	printf(" %d ", T[i]);
        };
    } else {
        if (hide == 0 && ind == 1) {
            for (i = 0; i < l; i++) {
                //     	          	printf("%d ", T[i]);
                printf("\x1B[48;5;%dm ", EntierVersIndicateur(T[i]));
                printf("\x1B[0m ");
            }
        } else {
            for (i = 0; i < l; i++) {
                printf("* ");
            };
        }
    }

    printf("]");
}

/**
 * Remplit les l premieres cases du tableau par un nombre aléatoire entre 0 et 7
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

// int CouleurVersEntier(char *coul) {
// 
//     if (strcmp(coul, "Rouge") == 0) {
//         return 0;
//     } else {
//         if (strcmp(coul, "Jaune") == 0) {
//             return 1;
//         } else {
//             if (strcmp(coul, "Vert") == 0) {
//                 return 2;
//             } else {
//                 if (strcmp(coul, "Bleu") == 0) {
//                     return 3;
//                 } else {
//                     if (strcmp(coul, "Orange") == 0) {
//                         return 4;
//                     } else {
//                         if (strcmp(coul, "Blanc") == 0) {
//                             return 5;
//                         } else {
//                             if (strcmp(coul, "Violet") == 0) {
//                                 return 6;
//                             } else {
//                                 return 7;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// int CouleurVersEntier(int c) {
// 
// switch (c) {
// case 9:
//   return 0;
//   break;
// case 11:
//   return 1;
//   break;
// case 10:
//   return 2;
//   break;
// case 12:
//   return 3;
//   break;
// case 208:
//   return 4;
//   break;
// case 255:
//   return 5;
//   break;
// case 129:
//   return 6;
//   break;
// case 201:
//   return 7;
//   break;
//  }
//  return 0;
// }

/**
 * Recherche un entier dans un tableau et renvoie sa position
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
 * Compare la proposition avec le code secret et renvoie un tableau d'indicateurs
 * @param T_prop : tableau proposé par le joueur
 * @param T_code : tableau du code secret
 * @param T_indic : tableau des indicateurs
 * @param level : taille des tableaux
 */
void Reponse(int T_prop[], int T_code[], int T_indic[], int level) {

    int Temp_prop[level];
    int Temp_code[level];
    unsigned int i;

    /* Les temporaires */
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

    int boule = 0;

    unsigned int i;
    for (i = 0; i < niv; i++) {

        printf("Entrer un numéro correspondant à une couleur (rang %d) : \n", i + 1);
        scanf("%d", &boule);
        T[i] = boule;
        printf("\x1B[48;5;%dm  ", EntierVersCouleur(T[i]));
        printf("\x1B[0m \n");

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

    // Accueil

    printf("Bienvenue dans le jeu MasterMind !\n\n");

    printf("Vous avez 15 tentatives pour trouver la bonne combinaison. Bonne chance !\n\n");

    // Choix du niveau de jeu
    printf("Veuillez choisir le niveau de jeu (entre 1 et 8) : ");
    scanf("%d", &level);

    // Création aléatoire du code secret dans tableau TabPartie
    int tabCode[level];
    srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
    genere(tabCode, level);

    printf("Code secret généré.\n");
    affiche(tabCode, level, 1, 1);
    printf("\n\n");

    // 	printf("0 = Rouge,  1 = Jaune, 2 = Vert, 3 = Bleu, 4 = Orange, 5 = Blanc, 6 = Violet, 7 = Fushia\n");

    printf("Numéros des couleurs possibles : \n");

    // Association des couleurs aux numéros
    printf("\x1B[48;5;%dm  ", 9);
    printf("\x1B[0m ");
    printf("= 0 (Rouge) ");
    printf("\x1B[48;5;%dm  ", 11);
    printf("\x1B[0m ");
    printf("= 1 (Jaune) ");
    printf("\x1B[48;5;%dm  ", 10);
    printf("\x1B[0m ");
    printf("= 2 (Vert) ");
    printf("\x1B[48;5;%dm  ", 12);
    printf("\x1B[0m ");
    printf("= 3 (Bleu) ");
    printf("\x1B[48;5;%dm  ", 16 + (2 * 6) + (5 * 36));
    printf("\x1B[0m ");
    printf("= 4 (Orange) ");
    printf("\x1B[48;5;%dm  ", 255);
    printf("\x1B[0m ");
    printf("= 5 (Blanc) ");
    printf("\x1B[48;5;%dm  ", 16 + (3 * 36) + 5);
    printf("\x1B[0m ");
    printf("= 6 (Violet) ");
    printf("\x1B[48;5;%dm  ", 16 + (5 * 36) + 5);
    printf("\x1B[0m ");
    printf("= 7 (Fushia)\n\n");

    // boucle de jeu
    int boucle = 1;
    int gagne = 0;
    int nbessai = 0;
    int tabProposition[level];
    while (boucle == 1) {

        // Remplir le tableau des indicateurs avec 0;
        int tabIndic[level];
        rempliAZero(tabIndic, level);

        // Demander au joueur de faire une proposition
        proposition(tabProposition, level);
        printf("Votre proposition : ");
        affiche(tabProposition, level, 0, 0);
        // Remplir tabIndic en fonction de la proposition faite
        Reponse(tabProposition, tabCode, tabIndic, level);

        printf("       Indicateurs : ");
        affiche(tabIndic, level, 0, 1);
        printf("\n");

        // Association des couleurs aux indicateurs
        printf("\x1B[48;5;%dm ", 9);
        printf("\x1B[0m ");
        printf("bien place ");
        printf("\x1B[48;5;%dm ", 7);
        printf("\x1B[0m ");
        printf("existe mais mal place ");
        printf("\x1B[48;5;%dm ", 0);
        printf("\x1B[0m ");
        printf("n'existe pas\n");

        //   printf("2 = bonne place, 1 = existe mais mal place, 0 = n'existe pas\n");

        gagne = estGagne(tabIndic, level);
        nbessai++;
        if (gagne == 1) {
            boucle = 0;
            printf("Vous avez gagné en %d coup(s) !\n", nbessai);

            affiche(tabCode, level, 0, 0);

        } else {
            if (nbessai == NBTOTALESSAI) {
                printf("Vous avez perdu !\n ");
                affiche(tabCode, level, 0, 0);
            }
            else {
                printf("Il vous reste %d essais.\n\n", NBTOTALESSAI - nbessai);
            }
        }

    }

    printf("\n");

    return 0;
}

