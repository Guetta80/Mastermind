/******************************************************************************/
/*			Application: Mastermind				      */
/******************************************************************************/
/*									      */
/*			 programme  CLIENT			              */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :                                                     */
/*      Marguerite Edouard                                                    */
/*      Chaillet Maxime                                                       */
/*									      */
/******************************************************************************/


#include <stdio.h>
#include <curses.h> 		/* Primitives de gestion d'écran */
#include <sys/signal.h>
#include <sys/wait.h>
#include<stdlib.h>
#include <string.h>

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111" //1111 initialement
#define SERVEUR_DEFAUT "localhost"

int masocket;


void client_appli(char *serveur, char *service);
void faireProposition(int T[], unsigned int niv, int optCouleur);
int couleurVersInt(char *coul);
void attenteIndicateursEtNoEssai(int tabIndicateurs[], int level, int *noEssai);
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet);
void convTabIntToChar(int tabProposition[], char tabPropositionConverted[], int level);
void afficheProposition(int T[], unsigned int l, int optCouleur);
void afficheIndicateurs(int tabIndic[], int level, int optCouleur);
void afficheLegendeCouleur(int optCouleur);


/*****************************************************************************/
/*--------------- programme client -----------------------*/

/**
 * Creation d'une connexion
 * @param serveur : 
 * @param service : le port utilisé
 * @return : code d'erreur: 0=succes, 1=erreur
 */
int connexion(char *serveur, char *service) {
    //declaration de types
    struct sockaddr_in *p_sockaddr_locale;
    struct sockaddr_in *p_sockaddr_distante;


    // création d'une socket,
    // la structure est crée et associée à un file descriptor
    // renvoi le file descriptor
    masocket = h_socket(AF_INET, SOCK_STREAM);
    if (masocket < 0) {
        printf("Echec création de socket");
        return 1;
    } else {
        printf("Socket créée avec succès. id= %d\n", masocket);
    }

    // Remplissage d'une structure sockaddr_in pour ip/port du serveur local
    // le system choisi le port
    // nom du pointeur: p_sockaddr_local
    adr_socket(0, serveur, SOCK_STREAM, &p_sockaddr_locale);
    h_bind(masocket, p_sockaddr_locale);
    // Remplissage d'une structure sockaddr_in pour ip/port du serveur distant
    adr_socket(service, serveur, SOCK_STREAM, &p_sockaddr_distante);
    h_connect(masocket, p_sockaddr_distante);
    return 0;
}

/**
 * affiche un tableau de proposition dans le terminal en fonction de l'option couleur.
 * @param T : un tableau.
 * @param l : taille du tableau
 * @param optCouleur : option couleur (1 = oui)
 */
void afficheProposition(int T[], unsigned int l, int optCouleur) {
    if (optCouleur == 0) {
        // jeu sans couleur
        unsigned int i;
        printf("[");
        for (i = 0; i < l; i++) {
            printf(" %d ", T[i]);
        };
        printf("] ");
    } else {
        // jeu en couleur
        unsigned int i;
        printf("[ ");
        for (i = 0; i < l; i++) {
            printf("\x1B[48;5;%dm  ", EntierVersCouleur(T[i]));
            printf("\x1B[0m ");
            //         	printf(" %d ", T[i]);
        };
        printf("]");
    }


}

void afficheLegendeCouleur(int optCouleur){
    if (optCouleur ==0) {
        printf("Couleurs possibles : ");
        printf("Rouge,  Jaune, Vert, Bleu, Orange, Blanc, Violet, Fushia\n");
    } else {
        // jeu avec couleur
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
    }
}

/**
 * Affiche un tablau d'indicaeur en fonction de l'option couleur
 * @param tabIndic : le tableua d'indicateurs
 * @param level : taille du tableau
 * @param optCouleur : option couleur (1=oui)
 */
void afficheIndicateurs(int tabIndic[], int level, int optCouleur) {
    int i;

    printf("       Indicateurs : [ ");
    if (optCouleur == 0) {
        // jeu sans couleur
        unsigned int i;
        printf("[");
        for (i = 0; i < level; i++) {
            printf(" %d ", tabIndic[i]);
        };
        printf("] \n");

        printf("2 = bonne place, 1 = existe mais mal place, 0 = n'existe pas\n");
    } else {
        for (i = 0; i < level; i++) {
            //     	          	printf("%d ", T[i]);
            printf("\x1B[48;5;%dm ", EntierVersIndicateur(tabIndic[i]));
            printf("\x1B[0m ");
        }
        printf("]");
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
    }

}

/* affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */
/*void affichetabInt(int T[], unsigned int l) {*/
    /* affiche les l premiers elements du tableau T */

/*    unsigned int i;
    printf("[");
    for (i = 0; i < l; i++) {
        printf(" %d ", T[i]);
    };
    printf("]\n");
}*/

int main(int argc, char *argv[]) {

    char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
    char *service = SERVICE_DEFAUT; /* numero de service par defaut du serveur (no de port) */

    /* Permet de passer un nombre de parametre variable a l'executable */
    switch (argc) {
        case 1: /* arguments par defaut */
            printf("serveur par defaut: %s\n", serveur);
            printf("service par defaut: %s\n", service);
            break;
        case 2: /* serveur renseigne  */
            serveur = argv[1];

            printf("service par defaut: %s\n", service);
            break;
        case 3: /* serveur, service renseignes */
            serveur = argv[1];
            service = argv[2];
            break;
        default:
            printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
            exit(1);
    }


    /* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
    /* service le numero de port sur le serveur correspondant au  */
    /* service desire par le client */

    client_appli(serveur, service);
}

int choixNiveauJeu(int *level) {
    // choix du niveau de jeu
    char chaine[1];
    printf("Veuillez choisir le niveau de jeu (entre 1 et 8) : ");
    scanf("%d", level);
    if (*level == 0) {
        printf("Valeur entrée incorrecte.");
    } else {
        sprintf(chaine, "%d", *level);
        envoieTCP(chaine, 1);
    }
}

int envoieTCP(char data[], int nbData) {
#ifdef DEBUG
    printf("ecriture de %d octets dans le buffer d'emission.\n", nbData);
#endif
    h_writes(masocket, data, nbData);
#ifdef DEBUG
    char dataAvecMarqueurFin[nbData + 1];
    strcpy(dataAvecMarqueurFin, data);
    dataAvecMarqueurFin[nbData] = '\0';
    printf("le buffer d'emission qui a été envoyé contient %s \n", dataAvecMarqueurFin);
#endif
}

/**
 * Permet au joueur de faire une proposition
 * @param T : tableau de char contenant les chiffres correspondant aux couleurs en ascii
 * @param niv: taille du tableau
 * @param optCouleur: 1=option couluer activée
 */
void faireProposition(int T[], unsigned int niv, int optCouleur) {
    if (optCouleur == 0) {
        // jeu sans couleur
        char Boule[10];
        unsigned int i;

        for (i = 0; i < niv; i++) {
            printf("Proposer une couleur (rang %d) : ", i);
            scanf("%s", Boule);
            T[i] = couleurVersInt(Boule);
        }

    } else {
        // jeu avec couleur
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
}

/**
 * Attend et recupère les indicateurs et le numéro de l'essai
 * @param tabIndicateurs : le tableau des indicateurs recupéré
 * @param level: la taille du tableau indicateur
 * @param noEssai: le numéro de l'essai récupéré
 */
void attenteIndicateursEtNoEssai(int tabIndicateurs[], int level, int *noEssai) {
    char donneeRecue[level + 2];
    lireBufferTCP(masocket, donneeRecue, level + 2);
    int i;
    for (i = 0; i < level; i++) {
        tabIndicateurs[i] = donneeRecue[i] - '0';
    }
    *noEssai = ((int) (donneeRecue[level] - '0'))*10 + (int) (donneeRecue[level + 1] - '0');


}

/**
 * Lit nboctet dans le buffer de reception correspondant à la socket socket_connectee 
 * et stocke le resultat dans un tableau
 * @param socket_connecte : la socket
 * @param donneeRecue : tableau contenant la donnée recue
 * @param nboctet : nb d'octets lus
 */
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet) {
    // char buf_reception[1];
    //int res;
#ifdef DEBUG
    printf("lecture de %d octets dans le buffer de reception.\n", nboctet);
#endif 
    h_reads(socket_connecte, donneeRecue, nboctet);
#ifdef DEBUG
    char dataAvecMarqueurFin[nboctet + 1];
    strcpy(dataAvecMarqueurFin, donneeRecue);
    dataAvecMarqueurFin[nboctet] = '\0';
    printf("La taille du buffer de reception est de %d octets.\n", nboctet);
    printf("Le buffer de reception contient les ascii suivants: %s\n", dataAvecMarqueurFin);
#endif
}

/**
 * Converti une couleur au format texte en entier (utiliser en mode sans couleur)
 * @param coul : chaine de caractere ("Rouge")
 * @return entier
 */
int couleurVersInt(char *coul) {

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
 * Convertir un entier correspondant à une couleur en code couleur pour le terminal
 * @param c uun entier représentatn un couleur entré par l'utilisateur
 * @return un code couleur
 */
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

/**
 * Convertir un entier correspondant à une couleur d'indicateur en code couleur pour le terminal
 * @param c uun entier représentant une couleur d'indicateurs
 * @return un code couleur
 */
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

/**
 * Converti un tableau d'entier en tableau d'ascii correspondant pour envoi en CP
 * @param tabProposition : le tableau d'entier
 * @param tabPropositionConverted: le tableau de char
 * @param level: taille des tableaux
 * 
 */
void convTabIntToChar(int tabProposition[], char tabPropositionConverted[], int level) {
    int i;
    for (i = 0; i < level; i++) {
        tabPropositionConverted[i] = tabProposition[i] + '0';
    }

}

int estGagne(int tab[], int level) {
    int i = 0;
    while (i < level && tab[i] == 2) {
        i++;
    }
    return i == level;
}

/*****************************************************************************/
void client_appli(char *serveur, char *service)

/* procedure correspondant au traitement du client de votre application */ {
    // etablissement de la connexion
    int connecte = connexion(serveur, service);
    if (connecte != 0) {
        printf("Désolé, Impossible de jouer. Problème lors de l'établissement de la connexion. \n");
    } else {
        int level;
        choixNiveauJeu(&level);


        // choix de la couleur
        int optCouleur = 0;
        printf("Voulez vous jouer avec la couleur ? (1=oui, 0=non) : ");
        scanf("%d", &optCouleur);

        // boucle de jeu
        int boucle = 1;
        int gagne = 0;
        int tabProposition[level];
        char tabPropositionConverted[level];
        int tabIndic[level];
        int noEssai;
        
        afficheLegendeCouleur(optCouleur);
        

        while (boucle == 1) {
            // Le joueur fait une proposition
            faireProposition(tabProposition, level, optCouleur);
            convTabIntToChar(tabProposition, tabPropositionConverted, level);
            envoieTCP(tabPropositionConverted, level);
            //envoieProposition(tabProposition, level);
            printf("Proposition du joueur : ");
            afficheProposition(tabProposition, level, optCouleur);

            attenteIndicateursEtNoEssai(tabIndic, level, &noEssai);
            afficheIndicateurs(tabIndic, level, optCouleur);

            printf("Il reste %d essais\n", 15 - noEssai);
            gagne = estGagne(tabIndic, level);
            if (gagne == 1) {
                printf("Vous avez gagné en %d coups!\n", noEssai);
                boucle = 0;
            }
            if (noEssai == 15) {
                printf("Vous avez perdu !\n");
                boucle = 0;

            }
        }
    }
}

