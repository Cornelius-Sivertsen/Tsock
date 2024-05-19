/* librairie standard ... */
#include <stdlib.h>
/* pour getopt */
#include <unistd.h>
/* déclaration des types de base */
#include <sys/types.h>
/* constantes relatives aux domaines, types et protocoles */
#include <sys/socket.h>
/* constantes et structures propres au domaine UNIX */
#include <sys/un.h>
/* constantes et structures propres au domaine INTERNET */
#include <netinet/in.h>
/* structures retournées par les fonctions de gestion de la base de
   données du réseau */
#include <netdb.h>
/* pour les entrées/sorties */
#include <stdio.h>
/* pour la gestion des erreurs */
#include <errno.h>
//Pour faire marcher chiffre():
#include <math.h>

//Valeurs défauts:
#define DEF_NOMBRE_MESSAGES 10
#define DEF_LONGUEUR_MESSAGES 30

//Fonction qui retourne le n-ieme chiffre d'un nombre. n=0 signifie 10^0.
int chiffre(int nombre, int n){
		int diviseur = (int) pow(10.0,(double) n);  
		nombre /= diviseur;
		return nombre % 10;
}

//Fonction pour construire un message:
//nbr est le numéro qui sera ajouté en tete du message. Commence à 0
void construire_message(char* message, char motif, int lg, int nbr){

		//Pour avoir le numéro du message au debut
		int chiffre_non_nul = 0;
		int numero;

		int i;
		for (i=0;i<=4;i++){
				numero = chiffre(nbr,4-i);

				//Remplace les 0 en tete avec des '-'
				if ((numero == 0) && (chiffre_non_nul == 0)){
						message[i] = '-';
				}
				else{
						message[i] = numero + '0';
						chiffre_non_nul = 1;
				}
		}

		for (;i<lg;i++) message[i] = motif;
		message[i] = '\0';
}

//Fonction pour afficher un message:
void afficher_message(char* message, int lg){
		int i;
		printf("message construit: ");
		for (i=0;i<lg;i++) printf("%c",message[i]);
		printf("\n");
}

//Pour constuire les messages.
char Alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


int main(int argc, char **argv){


		//Récuperation des paramètres d'entrée:
		int c;
		extern char *optarg;
		extern int optind;
		int longueur_message = -1; //Nombre de messages à envoyer, par défaut : 30
		int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
		enum{puits = 0, source = 1, pas_defini = -1} Source_ou_puits = pas_defini;
		enum{UDP = 0, TCP = 1} Protocole = TCP; //Par défaut : protocole TCP

		while ((c = getopt(argc, argv, "spul:n:")) != -1) {
				switch (c) {
						case 's':
								if (Source_ou_puits == puits) {
										printf("usage: cmd [-p|-s][-n ##]\n");
										exit(1) ;
								}
								Source_ou_puits = source;

								break;
						case 'p':
								if (Source_ou_puits == source) {
										printf("usage: cmd [-p|-s][-n ##]\n");
										exit(1);
								}
								Source_ou_puits = puits;

								break;
						case 'u':
								Protocole = UDP;
								break;
						case 'l':
								longueur_message = atoi(optarg);
								if ((Source_ou_puits == source) && (longueur_message <=5)){
										printf("Pour la source, la longueur de messages doit être plus grande que 5!\n");
										exit(1);
								}

								break;
						case 'n':
								nb_message = atoi(optarg);

								break;
						default:
								printf("usage: cmd [-p|-s][-n ##]\n");
								exit(1);

								break;
				}
		}

		//Affichage d'information sur paramètres d'entrée
		if (Source_ou_puits == pas_defini) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
		}
		else if (Source_ou_puits == source) {
				printf("On est dans le source\n");

				//Affectuation des valeurs défauts
				if (nb_message == -1){
						nb_message = DEF_NOMBRE_MESSAGES;
				}
				if (longueur_message == -1){
						longueur_message = DEF_LONGUEUR_MESSAGES;
				}

				//Affichage de nombre de messages à envoyer, plus leur taille
				printf("Nombre de messages à envoyer: %i\n", nb_message);
				printf("Longueur de messages à envoyer: %i\n", longueur_message);

				//Affichage de numéro de port et machine destinataire:
				printf("Machine destinataire: %s | VERIFIEZ \n", argv[argc-2]);
				printf("Port destinataire: %i | VERIFIEZ\n", atoi(argv[argc-1]));
		}
		else{
				printf("on est dans le puits\n");

				//Affichage de nombre de messages à récevoir
				if (nb_message == -1){
						printf("Nombre de messages à récevoir : infini, par defaut\n");
				}
				else{
						printf("Nombre de messages à récevoir : %i\n", nb_message);
				}
				
				//Affichage de port d'entrée
				printf("Numéro de port d'entrée: %i | VERIFIEZ\n", atoi(argv[argc-1]));
		}

		//Affichage d'information de protocole:
		if (Protocole == TCP){
				printf("Protocole : TCP\n");
		}
		else{
				printf("Protocole : UDP\n");
		}



//#define TEST_CONSTRUIRE_MESSAGES
#ifdef TEST_CONSTRUIRE_MESSAGES

		if (Source_ou_puits == source){
				int j;
				for (j=0;j<nb_message;j++){
						char message[longueur_message + 1];
						construire_message(message, Alphabet[j % 26], longueur_message, j+1);
						printf("%s\n", message);
				}
		}
#endif
		return 0;
}

