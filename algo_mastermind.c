#include <stdio.h>
#include <stdlib.h>
#include <time.h>


enum { Rouge, Jaune, Vert, Bleu, Orange, Blanc, Violet, Fushia } couleur;
typedef enum couleur boule; 


/* affiche le tableau contenant la combinaison de couleur a trouver.
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */
void affiche (int T[], unsigned int l) {
/* affiche les l premiers elements du tableau T */

  unsigned int i ;
  printf("[") ;
  for (i=0; i<l ; i++) {
	printf(" %d ", T[i]) ;
  } ;
  printf("]\n") ;
}


/* Rempli les l premieres cases du tableau par un nombre aléatoire entre 0 et 7
 * parametre T: un tableau contenant le code à trouver.
 * parametre l: le nombre de couleurs à trouver
 */
void genere (int T[], unsigned int l) {
/* initialise les l premiers elements du tableau T */

  unsigned int i ;
  for (i=0; i<l ; i++) {
	T[i] = ( random() % 8) ;  // Genere un nombre aléatoire entre 0 et 7
  } ;
}

int main() {

int level;
char* chaine;

// chaoix du niveau de jeu
printf("Veuillez choisir le niveau de jeu (entre 1 et 8): ");
scanf("%d", &level) ;

// creation combinaison à trouver aléatoirement
int Tab_partie[level];
srandom(time(NULL)); /* germe pour la suite pseudo-aleatoire */
genere(Tab_partie, level) ;
affiche(Tab_partie, level) ;
  
//boucle de jeu
int x =1;
while (x==1) {
      
  // demander au joueur de faire une proposition
  printf("Faites une proposition: ");
  scanf("%s", &chaine);
  }
  
return 0 ;
 
 }
 
