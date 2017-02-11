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

void serveur_appli (char *service);   /* programme serveur */


/******************************************************************************/
/*---------------- programme serveur ------------------------------*/

int main(int argc,char *argv[])
{

	char *service= SERVICE_DEFAUT; /* numero de service par defaut */


	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
 	{
   	case 1:
		  printf("defaut service = %s\n", service);
		  		  break;
 	case 2:
		  service=argv[1];
		  printf("Mise en �coute sur le port %s\n", service);
            break;

   	default :
		  printf("Usage:serveur service (nom ou port) \n");
		  exit(1);
 	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/

	serveur_appli(service);
}


/******************************************************************************/
void serveur_appli(char *service)

/* Procedure correspondant au traitemnt du serveur de votre application */

{
//declaration de types
struct sockaddr_in *p_sockaddr_locale;
struct sockaddr_in sockaddr_client;
int socket_connecte;

// création d'une socket,
// la structure est crée et associée à un file descriptor
// renvoi le file descriptor
int socket = h_socket(AF_INET,SOCK_STREAM);
if (socket < 0) {
	printf("Echec création de socket");
	} else {
        printf("Socket créée avec succès. id= %d\n",socket);
        }

// Remplissage d'une structure sockaddr_in pour ip/port du serveur local
// port local d�finit par l'utilisateur
// nom du pointeur: p_sockaddr_local
adr_socket(service, NULL, SOCK_STREAM, &p_sockaddr_locale);
h_bind(socket, p_sockaddr_locale);
// Remplissage d'une structure sockaddr_in pour ip/port du serveur distant
// nom du pointeur: p_sockaddr_distant
//char* client = "localhost";
//adr_socket(0, 0, SOCK_STREAM, &p_sockaddr_client);
//int res = getpeername(socket,p_sockaddr_client, 1);
//mode passive sur id socket, taille file d'attente =5
h_listen(socket, 5);

//blocage en attente de connexion et accept des qu'un client fait une demande
socket_connecte = h_accept(socket,&sockaddr_client);

char buf_reception[1]; 
printf("lecture de 1 octets dans le buffer de reception.\n");
h_reads(socket_connecte, buf_reception, 1);
printf("salut");
printf("le buffer de reception contient %c", *buf_reception);


//sleep(200);

}

/******************************************************************************/

