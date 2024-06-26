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
#define NOMBRE_CLIENTS 30

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
		int longueur_message = DEF_LONGUEUR_MESSAGES; //Nombre de messages à envoyer, par défaut : 30
		int nb_message = -1; /* Nb de messages à envoyer ou à recevoir, par défaut : 10 en émission, infini en réception */
		int num_cible = -1; //Cible pour emetteur ou recepteur
		enum{emetteur = 0, recepteur = 1, bal = 2, pas_defini = -1} Programme = pas_defini;

		while ((c = getopt(argc, argv, "e:r:bn:l:")) != -1) {
				switch (c) {
						case 'l':
								printf("tsock-v3 travaille avec des messages de longueur fixe. Relancez le programme sans option -l svp.\n");
								exit(1);
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

				//Affectuation de valeur défaut pour nombre de messages.
				if (nb_message == -1){
						nb_message = DEF_NOMBRE_MESSAGES;
				}

				//Affichage de nombre de messages à envoyer, plus leur taille
				printf("Nombre de messages à envoyer: %i\n", nb_message);
				printf("Longueur de messages à envoyer: %i, par défaut\n", longueur_message);
				printf("Boite vers laquelle on envoye les messages: %i\n", num_cible);

				//Affichage de numéro de port et machine destinataire:
				printf("Machine destinataire: %s | VERIFIEZ \n", argv[argc-2]);
				printf("Port destinataire: %i | VERIFIEZ\n", atoi(argv[argc-1]));


		}
		else if (Programme == recepteur){

				if (nb_message != -1){
						printf("L'option -n ne sert à rien pour le récepteur, relancer le programme sans -n svp.\n");
						exit(1);
				}

				printf("on est dans le recepteur\n");

				printf("Récuperation de tous les messages dans la boite ciblée, (boite nr. %i)\n", num_cible);
				printf("Longueur de messages à recevoir : %i\n", longueur_message);
				//Affichage de port d'entrée
				printf("Numéro de port d'entrée: %i | VERIFIEZ\n", atoi(argv[argc-1]));

		}

		else{
				printf("On est dans la BAL\n");

				if (nb_message != -1){
						printf("L'option -n ne sert à rien pour la BAL, relancer le programme sans -n svp.\n");
						exit(1);
				}

				//Affichage de port d'entrée
				printf("Numéro de port d'entrée: %i | VERIFIEZ\n", atoi(argv[argc-1]));
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
				}

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
				message_identification message_id_emetteur = {0/*recepteur*/, num_cible, longueur_message, nb_message};

				int lg_emis_ID = write(sock,&message_id_emetteur, sizeof(message_id_emetteur));
				if (lg_emis_ID<1){
						printf("Le message d'identification de s'est pas envoyé\n");
				}

				//Pour recuperer le nombre de messages à recevoir et les éventuels messages d'erreur:
				message_identification nombre_a_recevoir;
				int taille_message_lu_ID = read(sock,&nombre_a_recevoir,sizeof(nombre_a_recevoir));
				if (taille_message_lu_ID < 0){
						printf("Pas bon pour la recpetion d'ID\n");
				}

				int	nbr_reception = nombre_a_recevoir.nbr_message;

				switch(nbr_reception){
						case -2:	//Aucune boite dans la liste
								printf("ERREUR: Il n'y a aucune boite dans la liste\n");
								break;
						case -1:	//Boite n'est pas dans la liste
								printf("ERREUR: La boite cible n'est pas dans la liste\n");
								break;
						case -3:	//Boite est vide
								printf("ERREUR: La boite est vide\n");
								break;
						default:	//Reception et affichage de messages qui arrivent:
									
								char pmesg[100000];
								int taille_max = sizeof(pmesg);

								int nbr_messages_lu = 0;
								ssize_t read_accept = 1;
								while (read_accept !=0){
										read_accept = read(sock,pmesg + nbr_messages_lu, taille_max-nbr_messages_lu);
										nbr_messages_lu += read_accept;
								}

								int j;
								for (j=0;j<nbr_reception;j++){
										printf("Récepteur: Reception nr. %i (%i) ",j+1, longueur_message);
										afficher_message(pmesg + j*(longueur_message) + j, longueur_message);
										printf("\n");
								}
				}//Fin switch
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


				int repetitions = 0; //Pour que la BAL continue a tourner après avoir repondu à plusieurs requêtes
				while(repetitions < NOMBRE_CLIENTS){
						printf("\n==========================================================================\n");
						printf("BAL en attente\n\n");
						if (listen(sock, 10)<0){printf("acceptation de la connexion niveau serveur a échoué\n");}

						socklen_t adr_em = sizeof(struct sockaddr);
						socklen_t *plg_adr_em = &adr_em;
						struct sockaddr *padr_em=malloc(sizeof(struct sockaddr_in));

						int sock_accept;
						sock_accept = accept(sock,padr_em,plg_adr_em); if (sock_accept <0 ){ printf("échec d'acceptation de la connexion au serveur\n");
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

						printf("Type de client: %s, cible: %i\n", ((type_client == emetteur) ? "emetteur" : "recepteur"), cible);

						//Cas où client est un emetteur
						if (type_client == emetteur){

								printf("Nombre de messages à inserer: %i\n", nb);

								char pmesg[1000000];
								int taille_max = sizeof(pmesg);
								int nbr_messages_lu = 0;

								ssize_t read_accept = 1;
								while (read_accept != 0){
										read_accept = read(sock_accept,pmesg + nbr_messages_lu, taille_max-nbr_messages_lu);
										nbr_messages_lu += read_accept;
								}


								insererLettres(&tete,pmesg,cible,nb,lg);
						}
						else if (type_client == recepteur){

								/*
								 * Avant de sortir les messages, on verifie que:
								 * Il y a au moins 1 boite dans la liste
								 * La boite cible est dans la liste
								 * La boite cible est non-vide
								 * 
								 * Pour cela, on utilise la fonction boiteInfo() 
								 *
								 * Avant d'envoyer les messages, on envoye un message d'ID avec un message d'erreur, ou le nombre de messages
								 * à envoyer si il n'y a aucun erreur. Pour envoyer l'information, 
								 * on utilise le champ long_message d'un struct message_identification
								 */

								int check_info_boite = boiteInfo(tete, cible);
								message_identification nombre_messages_pour_emetteur = {0, 0, 0,check_info_boite};

								//Envoyer message d'ID
								int lg_emis_ID_pour_emetteur = write(sock_accept,&nombre_messages_pour_emetteur, sizeof(nombre_messages_pour_emetteur));
								if (lg_emis_ID_pour_emetteur<1){
										printf("Le message d'identification de s'est pas envoyé\n");
								}

								switch(check_info_boite){
										case -2:	//Aucune boite dans la liste
												printf("Il n'y a aucune boite dans la liste\n");
												break;
										case -1:	//Boite n'est pas dans la liste
												printf("La boite cible n'est pas dans la liste\n");
												break;
										case -3:	//Boite est vide
												printf("La boite est vide\n");
												break;
										default:	//Envoi des messages:

												printf("Nombre des lettres à envoyer: %i\n", nombre_messages_pour_emetteur.nbr_message);

												char message_pour_recepteur[longueur_message+1];

												int i;
												for (i=0;i<check_info_boite;i++){
														enleverLettre(tete,cible,message_pour_recepteur);
														if (write(sock_accept,message_pour_recepteur,sizeof(message_pour_recepteur)) < 0){printf("Message ne s'est pas envoyé\n");}
														printf("Succès d'envoi nr. %i\n", i +1);
												}
								}//Fin switch
						}
						if (shutdown(sock_accept, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock accept\n");}
						if (close(sock_accept)<0){printf("échec de fermeture de sock accept\n");}	
						repetitions++;
				}
				if (shutdown(sock, SHUT_RDWR)<0){printf("échec de shutdown de connexion sock\n");}
				if (close(sock)<0){printf("échec de fermeture de sock\n");}
		}//Fin BAL

		return 0;
}
