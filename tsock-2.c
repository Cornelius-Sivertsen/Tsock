//TEST
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

//Fonction qui retourne le n-ieme chiffre d'un nombre. n=0 signifie 10^0 (le plus pétit chiffre).
int chiffre(int nombre, int n){
		int diviseur = (int) pow(10.0,(double) n);  
		nombre /= diviseur;
		return nombre % 10;
}

//Fonction pour construire un message:
//nbr est le numéro qui sera ajouté en tête du message. Commence à 0
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
		printf("[");
		for (i=0;i<lg;i++) printf("%c",message[i]);
		printf("]");
}

//Pour constuire les messages.
char Alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


int main(int argc, char **argv){


		//Recupération des paramètres d'entrés:
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
						nb_message = 100000;
				}
				else{
						printf("Nombre de messages à récevoir : %i\n", nb_message);
				}
				if (longueur_message == -1){
						printf("Longueur de messages à récevoir : 30, par defaut\n");
						longueur_message = 30;
				}
				else{
						printf("Longueur de messages à récevoir : %i\n", longueur_message);
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

		//Partie UPD Source/Puits
		if (Protocole == UDP){

				//Création du socket, commun pour source et puits
				int sock = socket(AF_INET,SOCK_DGRAM,0);
				if (sock < 0) {printf("La création du socket a échoué\n");}

				//Partie Source (UDP)
				if (Source_ou_puits == source){
						//Construction de l'adresse destinataire
						struct sockaddr_in adresse_destinataire; //Structure pour l'adresse de la machine destinataire.
						adresse_destinataire.sin_family = AF_INET;
						adresse_destinataire.sin_port = htons(atoi(argv[argc-1])); //Numéro de port destinataire.

						struct hostent * machinedest;
						if ((machinedest = gethostbyname(argv[argc-2])) == NULL) //"Nom" de la machine destinataire
						{ printf("erreur gethostbyname\n") ; 
								exit(1) ; } 
						memcpy( (char*)&(adresse_destinataire.sin_addr.s_addr),
										machinedest->h_addr, 
										machinedest->h_length ) ;

						//Construction et envoi des messages
						//message stocké dans le string à envoyer à chaque tour de la boucle
						char message[longueur_message + 1]; //+1 Pour le \0 à la fin
						int taille_message = longueur_message * sizeof(char);

						int i;
						for (i=0;i<nb_message;i++){ //Boucle pour construire et envoyer les messages
								construire_message(message, Alphabet[i % 26], longueur_message, i+1);
								printf("Envoi nr. %i: [%s]\n", i+1, message);
								int lg_emis = sendto(sock,message,taille_message,0,(struct sockaddr*)&adresse_destinataire,sizeof(adresse_destinataire));
								if (lg_emis < 0){
										printf("Le message nr. %i ne s'est pas envoyé\n", i+1);
								}
								else{
										printf("Succes d'envoi numéro %i\n", i+1);
								}
						}
				}//Fin source UDP

				//Partie puits (UDP)
				else if (Source_ou_puits == puits){
						struct sockaddr_in adresse_locale; //Adresse locale
						adresse_locale.sin_port = htons(atoi(argv[argc-1])); //Numéro de port
						adresse_locale.sin_family=AF_INET;
						adresse_locale.sin_addr.s_addr=INADDR_ANY;

						bind(sock,(struct sockaddr*)&adresse_locale,sizeof(adresse_locale)); //Bind
						int taille_max = 100*sizeof(char);
						unsigned int adresse_emetteur = 0;
						unsigned int *plg_adr_em = &adresse_emetteur;
						char *pmesg=malloc(taille_max); //String pour stocker message recu
						struct sockaddr *padr_em=malloc(sizeof(struct sockaddr_in));

						//boucle pour recevoir et afficher les messages
						int i;
						for (i=0; i<nb_message; i++){
								int longueur_message_recu = recvfrom(sock,pmesg,taille_max,0,padr_em,plg_adr_em);
								if (longueur_message_recu<0){
										printf("Pas bon pour le recvfrom nr. %i\n", i);
								}
								else {
										printf("PUITS: Reception nr. %i (%i) [%s]\n",i+1,longueur_message_recu, pmesg);
								}
						}
						free(padr_em);
						free(pmesg);
				}//Fin puits (UDP)
				int close_check = close(sock);
				if (close_check < 0) printf("Erreur de fermeture du socket\n");
		}//Fin UDP

		//Partie TCP source/puits
		else if (Protocole == TCP){

				//Creation du socket, commun pour source et puits
				int sock = socket(AF_INET,SOCK_STREAM,0);
				if (sock < 0) {printf("Le creation du socket a echoue\n");}

				//Partie source TCP
				if (Source_ou_puits == source){

						//Construction de l'adresse destinataire
						struct sockaddr_in adresse_destinataire; //Struct pour l'adresse du machine destinataire.
						adresse_destinataire.sin_family = AF_INET;
						adresse_destinataire.sin_port = htons(atoi(argv[argc-1])); //Numéro de port destinataire.

						struct hostent * machinedest;
						if ((machinedest = gethostbyname(argv[argc-2])) == NULL) //"Nom" de la machine destinataire
						{ printf("erreur gethostbyname\n") ; 
								exit(1) ; } 
						memcpy( (char*)&(adresse_destinataire.sin_addr.s_addr),
										machinedest->h_addr, 
										machinedest->h_length ) ;
						int check_connexion = connect(sock,(struct sockaddr*) &adresse_destinataire, sizeof(adresse_destinataire));
						if (check_connexion < 0){
								printf("erreur de connexion au serveur, envoie du message impossible\n");
						}

						//Construction et envoi des messages
						char message[longueur_message + 1]; //+1 Pour le \0 à la fin
						int taille_message = longueur_message * sizeof(char);

						int i;
						for (i=0;i<nb_message;i++){//Boucle pour construire et envoyer les messages.
								construire_message(message, Alphabet[i % 26], longueur_message, i+1);
								printf("Envoi nr. %i: %s\n", i+1, message);
								int lg_emis = write(sock, message, taille_message);
								if (lg_emis < 0){
										printf("le message %i ne s'est pas envoyé\n", i+1);
								}
								else{
										printf("Succes d'envoi numéro %i\n", i+1);
								}
						}
						
						if (shutdown(sock, SHUT_RDWR)<0){printf("échec de fermeture de connexion sock accept\n");}
						if (close(sock)<0){printf("échec de fermeture de connexion sock\n");}
				}//Fin source TCP

				//Partie puits TCP
				else if (Source_ou_puits == puits){

						//Creation de l'adresse locale
						struct sockaddr_in adresse_locale;
						adresse_locale.sin_port = htons(atoi(argv[argc-1]));
						adresse_locale.sin_family=AF_INET;
						adresse_locale.sin_addr.s_addr=INADDR_ANY;

						bind(sock,(struct sockaddr*) &adresse_locale,sizeof(adresse_locale)); //Bind de l'adresse locale du puits

						if (listen(sock, 10)<0){printf("acceptation de la connexion niveau serveur a échoué\n");}

						socklen_t adr_em = sizeof(struct sockaddr);
						socklen_t *plg_adr_em = &adr_em;
						struct sockaddr *padr_em=malloc(sizeof(struct sockaddr_in));

						int sock_accept;
						sock_accept = accept(sock,padr_em,plg_adr_em); //acceptation de la demande de connexion
						if (sock_accept <0 ){
								printf("échec d'acceptation de la connexion au serveur\n");
						}

						char pmesg[100000]; //tableau pour stocker les messages à l'arrivée
						int taille_max = sizeof(pmesg);
						int nbr_messages_lu = 0;

						ssize_t read_accept = 1;
						while (read_accept !=0){//lis les messages qui sont recus et les stockes dans pmesg
								read_accept = read(sock_accept,pmesg + nbr_messages_lu, taille_max-nbr_messages_lu);
								nbr_messages_lu += read_accept;
						}

						nbr_messages_lu /= longueur_message;
						if (nbr_messages_lu>nb_message){
								nbr_messages_lu = nb_message;
						}

						int j;

						for (j=0;j<nbr_messages_lu;j++){//découpe les messages pour les afficher
								printf("PUITS: Reception nr. %i (%i) ",j+1, longueur_message);
								afficher_message(pmesg + j*longueur_message, longueur_message);
								printf("\n");
						}



						free(padr_em);

						if (shutdown(sock_accept, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock accept\n");}
						if (close(sock_accept)<0){printf("échec de fermeture de connexion sock accept\n");}
						if (shutdown(sock, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock\n");}
						if (close(sock)<0){printf("échec de fermeture de connexion sock\n");}
				}//Fin puits TCP
		}//Fin TCP source/puits
		return 0;
}
