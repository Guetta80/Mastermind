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

#include<stdio.h>
#include <curses.h>
#include<sys/signal.h>
#include<sys/wait.h>
#include<stdlib.h>
#include "fon.h"     		/* Primitives de la boite a outils */

#define SERVICE_DEFAUT "1111"
int masocket;
int level;

void serveur_appli(char *service); /* programme serveur */
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet);
void genere(int T[], unsigned int l);



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

void attenteParam(int socket_connecte){
    char donneeRecue[1];
    lireBufferTCP(socket_connecte, donneeRecue, 1);
    level = donneeRecue[0]-'0';
}

/**
 * Lit nboctet dans le buffer de reception correspondant à la socket socket_connectee 
 * et stocke le resultat dans un tableau
 * @param socket_connecte : la socket
 * @param donneeRecue : tableau contenant la donnée recue
 * @param nboctet : nb d'octets lus
 */
void lireBufferTCP(int socket_connecte, char donneeRecue[], int nboctet){
    // char buf_reception[1];
    //int res;
    printf("lecture de %d octets dans le buffer de reception.\n", nboctet);
    h_reads(socket_connecte, donneeRecue, nboctet);
    printf("La taille du buffer de reception est de %d octets.\n", nboctet);
    printf("Le buffer de reception contient les ascii suivants: %c.\n", donneeRecue[0]);
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



/******************************************************************************/

/*/* Procedure correspondant au traitemnt du serveur de votre application
 */
void serveur_appli(char *service) {
    struct sockaddr_in sockaddr_client;

    attenteConnexion(service);
    //blocage en attente de connexion et accept des qu'un client fait une demande
    int socket_connecte;
    socket_connecte = h_accept(masocket, &sockaddr_client);

    attenteParam(socket_connecte);
    printf("le niveau est %d\n", level);
    
    // creation aléatoire du code secret dans tableau TabPartie
    int tabCode[level];
    srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
    genere(tabCode, level);
    printf("0 = Rouge,  1 = Jaune, 2 = Vert, 3 = Bleu, 4 = Orange, 5 = Blanc, 6 = Violet, 7 = Fushia\n");
    printf("Code secret : ");
    affiche(tabCode, level);



    //sleep(200);

}

/******************************************************************************/

