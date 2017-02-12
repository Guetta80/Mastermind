/******************************************************************************/
/*			Application: Masterming                               */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*							                      */
/*		Auteurs :  ....						      */
/*      Marguerite Edouard 			                              */
/*      Chaillet Maxime                                                       */
/*									      */
/*		Date :  Fevrier 2017                                          */
/*									      */
/******************************************************************************/

#include <stdio.h>
#include <curses.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "fon.h"     		/* Primitives de la boite a outils */
#include <string.h>

#define SERVICE_DEFAUT "1111"
const int NBTOTALESSAI = 15;
int masocket;
int socket_connecte;
int level;


void serveur_appli(char *service); /* programme serveur */
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet);
void genere(int T[], unsigned int l);
void attenteProposition(int tabProposition[], int level);
void reponse(int T_prop[], int T_code[], int T_indic[], int level);
int recherche(int T[], unsigned int l, unsigned x);
int estGagne(int tab[], int level);
void envoieIndicateursEtNoEssai(int tabIndic[], int level, int noEssai);
int envoieTCP(char data[], int nbData);

/******************************************************************************/

/*---------------- programme serveur ------------------------------*/

int main(int argc, char *argv[]) {

    char *service = SERVICE_DEFAUT; /* numero de service par defaut */


    /* Permet de passer un nombre de parametre variable a l'executable */
    switch (argc) {
        case 1:
            printf("defaut service = %s\n", service);
            break;
        case 2:
            service = argv[1];
            printf("Mise en écoute sur le port %s\n", service);
            break;

        default:
            printf("Usage:serveur service (nom ou port) \n");
            exit(1);
    }

    /* service est le service (ou numero de port) auquel sera affecte
    ce serveur*/

    serveur_appli(service);
}

/**
 * Créé la connexion et met le serveur en attente
 * @return 0 si succes, 1 si echec
 */
int attenteConnexion(char *service) {
    struct sockaddr_in *p_sockaddr_locale;
    masocket = h_socket(AF_INET, SOCK_STREAM);
    if (masocket < 0) {
        printf("Echec création de socket");
        return 1;
    } else {
        printf("Socket créée avec succès. id= %d\n", masocket);
    }

    // Remplissage d'une structure sockaddr_in pour ip/port du serveur local
    // port local définit par l'utilisateur
    // nom du pointeur: p_sockaddr_local
    adr_socket(service, NULL, SOCK_STREAM, &p_sockaddr_locale);
    h_bind(masocket, p_sockaddr_locale);
    // Remplissage d'une structure sockaddr_in pour ip/port du serveur distant
    // nom du pointeur: p_sockaddr_distant
    //char* client = "localhost";
    //adr_socket(0, 0, SOCK_STREAM, &p_sockaddr_client);
    //int res = getpeername(socket,p_sockaddr_client, 1);
    //mode passive sur id socket, taille file d'attente =5
    h_listen(masocket, 5);
    printf("Le serveur est en écoute sur le port %s\n", service);
    return 0;
}

void attenteParam(int socket_connecte) {
    char donneeRecue[1];
    lireBufferTCP(socket_connecte, donneeRecue, 1);
    level = donneeRecue[0] - '0';
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
 * compare la proposition avec le code secrète et renvoie un tableau d'indicateurs
 * @param T_prop : tableau de proposition
 * @param T_code : tableau du code secret
 * @param T_indic : tableau des indicateur
 * @param level : taille des tableaux
 */
void reponse(int T_prop[], int T_code[], int T_indic[], int level) {

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

void attenteProposition(int tabProposition[], int level) {
    char donneeRecue[level];
    lireBufferTCP(socket_connecte, donneeRecue, level);
    int i;
    for (i = 0; i < level; i++) {
        tabProposition[i] = donneeRecue[i] - '0';
    }
}

/**
 * Prepare les donnes des indicateurs et des essais pour envoi au client
 * @param tabIndic : tableau d'entier des indicateurs 
 * @param level : taille des tableaux
 * @param noEssai : le numero de l'essai 
 */
void envoieIndicateursEtNoEssai(int tabIndic[], int level, int noEssai) {
    char donneeAEnvoyee[level + 2]; //noessai est sur 2 digits
    int i;
    for (i = 0; i < level; i++) {
        donneeAEnvoyee[i] = tabIndic[i] + '0';
    }
    char chaine[2];
    sprintf(chaine, "%d", noEssai);
    if (noEssai < 10) {
        donneeAEnvoyee[level] = '0';
        donneeAEnvoyee[level + 1] = chaine[0];
    } else {
        donneeAEnvoyee[level] = chaine[0];
        donneeAEnvoyee[level + 1] = chaine[1];
    }
    envoieTCP(donneeAEnvoyee, level + 2);
}

int envoieTCP(char data[], int nbData) {
    printf("ecriture de %d octets dans le buffer d'emission.\n", nbData);
    h_writes(socket_connecte, data, nbData);
    char dataAvecMarqueurFin[nbData + 1];
    strcpy(dataAvecMarqueurFin, data);
    dataAvecMarqueurFin[nbData] = '\0';
    printf("le buffer d'emission qui a été envoyé contient %s \n", dataAvecMarqueurFin);
}
/******************************************************************************/

/*/* Procedure correspondant au traitemnt du serveur de votre application
 */
void serveur_appli(char *service) {
    struct sockaddr_in sockaddr_client;

    attenteConnexion(service);
    //blocage en attente de connexion et accept des qu'un client fait une demande
    socket_connecte = h_accept(masocket, &sockaddr_client);

    attenteParam(socket_connecte);
    printf("le niveau est %d\n", level);

    // creation aléatoire du code secret dans tableau TabPartie
    int tabCode[level];
    srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
    genere(tabCode, level);
    printf("Code secret : ");
    affiche(tabCode, level);

    // boucle d'attente de porpositions
    int boucle = 1;
    int gagne = 0;
    int noEssai = 0; // numéro de l'essai
    int tabProposition[level];
    while (boucle == 1) {
        noEssai++;
        // Vide le tableau des indicateurs (=met 0 dans chaque cases)
        int tabIndic[level];
        rempliAZero(tabIndic, level);

        // attend la proposition du joueur
        attenteProposition(tabProposition, level);
#ifdef DEBUG
        printf("Proposition du joueur : ");
        affiche(tabProposition, level);
#endif
        // Rempli tabIndic en fonction de la proposition faite
        reponse(tabProposition, tabCode, tabIndic, level);
#ifdef DEBUG
        printf("Indicateurs : ");
        affiche(tabIndic, level);
#endif
        envoieIndicateursEtNoEssai(tabIndic, level, noEssai);

        gagne = estGagne(tabIndic, level);
        if (gagne == 1) {
            boucle = 0;
        }

        if (noEssai == 15) {
            boucle = 0;
        }
    }

    // sortie du jeu
    // fermeture Connexion
    h_close(socket_connecte);

}

/******************************************************************************/

