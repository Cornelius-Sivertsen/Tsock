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

//Pour la BAL;
#include "libBAL.h"

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
		printf("[");
		for (i=0;i<lg;i++) printf("%c",message[i]);
		printf("]");
}

//Pour constuire les messages.
char Alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

typedef struct struct_message_identification{
		int emetteur_ou_recepteur; //1 pour emetteur, 0 pour recepteur
		int num_cible;
		int long_message;
		int nbr_message;
}message_identification;



int main(int argc, char **argv){


		//Récuperation des paramètres d'entrée:
		int c;
		extern char *optarg;
		extern int optind;
		int longueur_message = -1; //Nombre de messages à envoyer, par défaut : 30
		int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
		int num_cible = -1; //Cible pour emetteur ou recepteur
		enum{emetteur = 0, recepteur = 1, bal = 2, pas_defini = -1} Programme = pas_defini;

		while ((c = getopt(argc, argv, "e:r:bn:l:")) != -1) {
				switch (c) {
						case 'l':
								longueur_message = atoi(optarg);
								if ((Programme == emetteur) && (longueur_message <=5)){
										printf("Pour l'emetteur, la longueur de messages doit être plus grande que 5!\n");
										exit(1);
								}
								break;
						case 'n':
								nb_message = atoi(optarg);
								break;
						case 'e':
								if (Programme == recepteur && Programme == bal){
										printf("usage: cmd [-e|-r|-b][-n ##]\n");
										exit(1);
								}
								Programme = emetteur;
								num_cible = atoi(optarg);
								break;
						case 'r':
								if (Programme == emetteur && Programme == bal){
										printf("usage: cmd [-e|-r|-b][-n ##]\n");
										exit(1);
								}
								Programme = recepteur;
								num_cible = atoi(optarg);
								break;
						case 'b':
								if (Programme == recepteur && Programme == emetteur){
										printf("usage: cmd [-e|-r|-b][-n ##]\n");
										exit(1);
								}
								Programme = bal;
								break;
						default:
								printf("usage: cmd [-e|-r|-b][-n ##]\n");
								exit(1);
								break;
				}
		}

		//Affichage d'information sur paramètres d'entrée
		if (Programme == pas_defini) {
				printf("usage: cmd [-p|-s][-n ##]\n");
				exit(1) ;
		}
		else if (Programme == emetteur) {
				printf("On est dans l'emetteur\n");

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
		else if (Programme == recepteur){
				printf("on est dans le recepteur\n");

				//Affichage de nombre de messages à récevoir
				if (nb_message == -1){
						printf("Nombre de messages à recevoir : infini, par defaut\n");
						nb_message = 100000;
				}
				else{
						printf("Nombre de messages à recevoir : %i\n", nb_message);
				}
				if (longueur_message == -1){
						printf("Longueur de messages à recevoir : 30, par defaut\n");
						longueur_message = 30;
				}
				else{
						printf("Longueur de messages à recevoir : %i\n", longueur_message);
				}


				//Affichage de port d'entrée
				printf("Numéro de port d'entrée: %i | VERIFIEZ\n", atoi(argv[argc-1]));
		}
		else{
				printf("On est dans la BAL\n");
		}


		printf("Protocole : TCP\n");


		//Creation du socket, commune pour emetteur, recepteur et BAL
		int sock = socket(AF_INET,SOCK_STREAM,0);
		if (sock < 0) {printf("Le creation du socket a echoue\n");}

		//Partie emetteur
		if (Programme == emetteur){

				//Construction de l'adresse destinataire
				struct sockaddr_in adresse_destinataire; //Struct pour l'adresse du machine destinataire.
				adresse_destinataire.sin_family = AF_INET;
				adresse_destinataire.sin_port = htons(atoi(argv[argc-1])); //Numéro de port destinataire.

				struct hostent * machinedest;
				if ((machinedest = gethostbyname(argv[argc-2])) == NULL) //"Nom" du machine destinataire
				{ printf("erreur gethostbyname\n") ; 
						exit(1) ; } 
				memcpy( (char*)&(adresse_destinataire.sin_addr.s_addr),
								machinedest->h_addr, 
								machinedest->h_length ) ;
				int check_connexion = connect(sock,(struct sockaddr*) &adresse_destinataire, sizeof(adresse_destinataire));
				if (check_connexion < 0){
						printf("erreur de connexion au serveur, envoie du message impossible\n");
				}

				//Message d'ID
				message_identification message_id_emetteur = {1/*emetteur*/, num_cible, longueur_message, nb_message};

				printf("Em ou recep: %i\n", message_id_emetteur.emetteur_ou_recepteur);
				printf("cible: %i\n", message_id_emetteur.num_cible);
				printf("long: %i\n", message_id_emetteur.long_message);
				printf("nbr: %i\n", message_id_emetteur.nbr_message);


				int lg_emis_ID = write(sock,&message_id_emetteur, sizeof(message_id_emetteur));
				if (lg_emis_ID<1){
						printf("Le message d'identification de s'est pas envoyé\n");
				}

				//Construction et envoie des messages
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
		}//Fin emetteur

		//Partie recepteur
		else if (Programme == recepteur){

				//Creation de l'adresse locale
				struct sockaddr_in adresse_locale;
				adresse_locale.sin_port = htons(atoi(argv[argc-1]));
				adresse_locale.sin_family=AF_INET;
				adresse_locale.sin_addr.s_addr=INADDR_ANY;

				bind(sock,(struct sockaddr*) &adresse_locale,sizeof(adresse_locale));

				//Construction de l'adresse destinataire
				struct sockaddr_in adresse_destinataire; //Struct pour l'adresse du machine destinataire.
				adresse_destinataire.sin_family = AF_INET;
				adresse_destinataire.sin_port = htons(atoi(argv[argc-1])); //Numéro de port destinataire.

				struct hostent * machinedest;
				if ((machinedest = gethostbyname(argv[argc-2])) == NULL) //"Nom" du machine destinataire
				{ printf("erreur gethostbyname\n") ; 
						exit(1) ; } 
				memcpy( (char*)&(adresse_destinataire.sin_addr.s_addr),
								machinedest->h_addr, 
								machinedest->h_length ) ;

				int check_connexion = connect(sock,(struct sockaddr*) &adresse_destinataire, sizeof(adresse_destinataire));
				if (check_connexion < 0){
						printf("erreur de connexion au serveur, envoie du message impossible\n");
				}

				//Message d'ID
				message_identification message_id_emetteur = {0/*recepteur*/, num_cible, 0, 0};

				int lg_emis_ID = write(sock,&message_id_emetteur, sizeof(message_id_emetteur));
				if (lg_emis_ID<1){
						printf("Le message d'identification de s'est pas envoyé\n");
				}


				char pmesg[100000];
				int taille_max = sizeof(pmesg);
				int nbr_messages_lu = 0;

				ssize_t read_accept = 1;
				while (read_accept !=0){
						read_accept = read(sock,pmesg + nbr_messages_lu, taille_max-nbr_messages_lu);
						nbr_messages_lu += read_accept;
				}

				printf("pmesg: %s\n", pmesg);

				nbr_messages_lu /= longueur_message;
				if (nbr_messages_lu>nb_message){
						nbr_messages_lu = nb_message;
				}

				int j;

				for (j=0;j<nbr_messages_lu;j++){
						printf("PUITS: Reception nr. %i (%i) ",j+1, longueur_message);
						afficher_message(pmesg + j*longueur_message, longueur_message);
						printf("\n");
				}
				//free(padr_em);

				if (shutdown(sock, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock\n");}
				if (close(sock)<0){printf("échec de fermeture de connexion sock\n");}
		}//Fin recepteur

		//Partie BAL
		else{
				//Creation du liste chainée pour la BAL:
				BAL tete = {0,NULL};

				//Creation de l'adresse locale
				struct sockaddr_in adresse_locale;
				adresse_locale.sin_port = htons(atoi(argv[argc-1]));
				adresse_locale.sin_family=AF_INET;
				adresse_locale.sin_addr.s_addr=INADDR_ANY;

				bind(sock,(struct sockaddr*) &adresse_locale,sizeof(adresse_locale));


				int compteur = 0;
				while(compteur < 5){
						if (listen(sock, 10)<0){printf("acceptation de la connexion niveau serveur a échoué\n");}

						socklen_t adr_em = sizeof(struct sockaddr);
						socklen_t *plg_adr_em = &adr_em;
						struct sockaddr *padr_em=malloc(sizeof(struct sockaddr_in));

						int sock_accept;
						sock_accept = accept(sock,padr_em,plg_adr_em);
						if (sock_accept <0 ){
								printf("échec d'acceptation de la connexion au serveur\n");
						}

						message_identification message_id_client;
						int taille_message_lu = read(sock_accept,&message_id_client,sizeof(message_identification));
						if (taille_message_lu < 0){
								printf("Pas bon pour la recpetion d'ID\n");
						}

						enum {emetteur = 1, recepteur = 0} type_client = message_id_client.emetteur_ou_recepteur;
						int cible = message_id_client.num_cible;
						int lg = message_id_client.long_message;
						int	nb = message_id_client.nbr_message;

						printf("Type de client: %s, cible: %i, longueur: %i, nombre: %i\n", ((type_client == emetteur) ? "emetteur" : "recepteur"), cible, lg, nb);

						//Cas où client est un emetteur
						if (type_client == emetteur){
								printf("Le client est emetteur\n");

								char pmesg[1000000];
								int taille_max = sizeof(pmesg);
								int nbr_messages_lu = 0;

								ssize_t read_accept = 1;
								while (read_accept != 0){
										printf("test\n");
										read_accept = read(sock_accept,pmesg + nbr_messages_lu, taille_max-nbr_messages_lu);
										nbr_messages_lu += read_accept;
								}

								printf("pmesg: %s\n", pmesg);

								insererLettres(&tete,pmesg,cible,nb,lg);
								printf("Les boites dans la liste:\n");
								afficherBoites(tete);
								printf("\n====================================================\n");
								printf("Les lettres dans la boite ciblée:\n");
								afficherLettres(tete, cible);
						}
						else if (type_client == recepteur){
								printf("Le client est recepteur\n");

								char message_pour_recepteur[LONGUEURLETTRESMAX];

								int sortie_enleverLettre = 0;
								int compt = 0;
								while (sortie_enleverLettre != -3){
										printf("compteur: %i\n", compt);
										sortie_enleverLettre = enleverLettre(tete,cible,message_pour_recepteur);
										if (sortie_enleverLettre == -2){
												printf("La BAL est vide");
												char retour[] = "La BAL est vide";
												if (write(sock_accept,retour,sizeof(retour)) < 0){printf("Message BAL vide ne s'est pas envoyé\n");}
										}
										else if (sortie_enleverLettre == -1){
												printf("La boite n'existe pas");
												char retour[] = "La boite n'existe pas";
												if (write(sock_accept,retour,sizeof(retour)) < 0){printf("Message boite n'existe pas ne s'est pas envoyé\n");}
										}
										else if (sortie_enleverLettre != -3){
												printf("Message à envoyer: %s\n", message_pour_recepteur);
												if (write(sock_accept,message_pour_recepteur,sizeof(message_pour_recepteur)) < 0){printf("Message ne s'est pas envoyé\n");}
										}

										compt++;
								}
						}
						if (shutdown(sock_accept, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock accept\n");}
						if (close(sock_accept)<0){printf("échec de fermeture de connexion sock accept\n");}	
						compteur++;
				}
				if (shutdown(sock, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock\n");}
				if (close(sock)<0){printf("échec de fermeture de connexion sock\n");}
		}//Fin BAL

		return 0;
}
