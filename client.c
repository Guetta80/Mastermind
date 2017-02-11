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

#include "fon.h"   		/* primitives de la boite a outils */

#define SERVICE_DEFAUT "1111" //1111 initialement
#define SERVEUR_DEFAUT "localhost"

int masocket;


void client_appli(char *serveur, char *service);


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
    printf("le buffer d'emission qui a été envoyé contient %s", data);
}

/*****************************************************************************/
void client_appli(char *serveur, char *service)

/* procedure correspondant au traitement du client de votre application */ {
    // etablissement de la connexion
    int connecte = connexion(serveur, service);
    if (connecte != 0) {
        printf("Désolé, Impossible de jouer. Probleme lors de l'établissement de la connexion. \n");
    } else {
        int level;
        choixNiveauJeu(&level);



        sleep(20);
    }
    //h_bind(socket, p_sockaddr_local);
    //


    // Code du jeu

}
/*****************************************************************************/

