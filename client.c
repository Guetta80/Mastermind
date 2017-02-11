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
void faireProposition(char T[], unsigned int niv);
char couleurVersChar(char *coul);
void attenteIndicateursEtNoEssai(int tabIndicateurs[], int level, int *noEssai);
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet);

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

/* affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */
void affiche(char T[], unsigned int l) {
    /* affiche les l premiers elements du tableau T */

    unsigned int i;
    printf("[");
    for (i = 0; i < l; i++) {
        printf(" %c ", T[i]);
    };
    printf("]\n");
}

/* affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */
void affichetabInt(int T[], unsigned int l) {
    /* affiche les l premiers elements du tableau T */

    unsigned int i;
    printf("[");
    for (i = 0; i < l; i++) {
        printf(" %d ", T[i]);
    };
    printf("]\n");
}

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
    printf("ecriture de 1 octets dans le buffer d'emission.\n");
    h_writes(masocket, data, nbData);
    char dataAvecMarqueurFin[nbData + 1];
    strcpy(dataAvecMarqueurFin, data);
    dataAvecMarqueurFin[nbData] = '\0';
    printf("le buffer d'emission qui a été envoyé contient %s \n", dataAvecMarqueurFin);
}

/**
 * Permet au joueur de faire une proposition
 * @param T : tableau de char contenant les chiffres correspondant aux couleurs en ascii
 * @param niv: taille du tableau
 */
void faireProposition(char T[], unsigned int niv) {
    char Boule[10];
    unsigned int i;

    for (i = 0; i < niv; i++) {
        printf("Proposer une couleur (rang %d) : ", i);
        scanf("%s", Boule);
        T[i] = couleurVersChar(Boule);
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
    printf("lecture de %d octets dans le buffer de reception.\n", nboctet);
    h_reads(socket_connecte, donneeRecue, nboctet);
    char dataAvecMarqueurFin[nboctet + 1];
    strcpy(dataAvecMarqueurFin, donneeRecue);
    dataAvecMarqueurFin[nboctet] = '\0';
    printf("La taille du buffer de reception est de %d octets.\n", nboctet);
    printf("Le buffer de reception contient les ascii suivants: %s\n", dataAvecMarqueurFin);
}

char couleurVersChar(char *coul) {

    if (strcmp(coul, "Rouge") == 0) {
        return '0';
    } else {
        if (strcmp(coul, "Jaune") == 0) {
            return '1';
        } else {
            if (strcmp(coul, "Vert") == 0) {
                return '2';
            } else {
                if (strcmp(coul, "Bleu") == 0) {
                    return '3';
                } else {
                    if (strcmp(coul, "Orange") == 0) {
                        return '4';
                    } else {
                        if (strcmp(coul, "Blanc") == 0) {
                            return '5';
                        } else {
                            if (strcmp(coul, "Violet") == 0) {
                                return '6';
                            } else {
                                return '7';
                            }
                        }
                    }
                }
            }
        }
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

        // boucle de jeu
        int boucle = 1;
        int gagne = 0;
        char tabProposition[level];
        int tabIndic[level];
        int noEssai;

        printf("0 = Rouge,  1 = Jaune, 2 = Vert, 3 = Bleu, 4 = Orange, 5 = Blanc, 6 = Violet, 7 = Fushia\n");

        while (boucle == 1) {
            // Le joueur fait une proposition
            faireProposition(tabProposition, level);
            envoieTCP(tabProposition, level);
            //envoieProposition(tabProposition, level);
            printf("Proposition du joueur : ");
            affiche(tabProposition, level);

            attenteIndicateursEtNoEssai(tabIndic, level, &noEssai);

            printf("2 = bonne place, 1 = existe mais mal place, 0 = n'existe pas\n");
            printf("les indicateurs sont : ");
            affichetabInt(tabIndic, level);
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

