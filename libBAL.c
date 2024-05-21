//Code source pour librairie libBAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libBAL.h"


//Fonction qui rajoute un boite vide à la fin de la liste des boites.
//Si la boite existe déjà dans la liste, la fonction ne fait rien.
//
//Paramètres d'entrée:
//BAL: Pointeur sur la struct qui pointe vers la tête de la liste dans laquelle
//on va ajouter une boite.
//Cible: le numéro de boite de la boite que l'on va ajouter.
//
//Valeur retournée:
//Nombre des boites dans la liste après le rajout dans le cas où on ajoue un boite, -1 dans le cas où le boite était déjà dans la liste (aucun rajout).
int ajouteBoite(struct BAL* BAL, int cible) 
{
		int returnval = -1; //Valeur à retourner

		//Case where list is empty.
		if(BAL -> premiere_Boite == NULL)
		{
				BAL -> premiere_Boite = (boiteCellule*) malloc(sizeof(boiteCellule)); //Creating a new cell
				
				//Adding values to cell:
				BAL -> premiere_Boite -> nbr_boite = cible;
				BAL -> premiere_Boite -> nombre_des_lettres = 0;
				BAL -> premiere_Boite -> prochaineBoite = NULL;
				BAL -> premiere_Boite -> premLettre = NULL;

				//On augmente le compteur de boîte de la structure BAL
				(BAL -> nombre_des_boites)++;
				returnval = 1;
		}

		//Cas dans laquelle la liste n'est pas vide
		else{
				
				boiteCellule temp_cell = {-1, -1,BAL -> premiere_Boite , NULL}; //Cellule temporaire pour itérer plus facilement dans la liste

				boiteCellule* aux = &temp_cell; //Pointeur auxiliaire pour itérer parmis la liste
				int boite_existe_deja = 0;
				while ((aux -> prochaineBoite != NULL) && (boite_existe_deja == 0))
				{
						if(aux -> prochaineBoite -> nbr_boite == cible) //Vérifier si la boîte que l'on veut ajouter est déjà dans la liste 
						{
								boite_existe_deja = 1;
						}
						else
						{
								aux = aux -> prochaineBoite; //Si non, on continue dans la liste
						}
				} 
				
				//A ce moment, soit nous sommes à la fin de la liste ou la boîte à ajouter est déjà dans la liste
				//Si nous commes à la fin de la liste, nous ajouton un nouvel élément et donnons sa valeur, comme nous avons fait pour ajouter une liste vide
				if(boite_existe_deja == 0)
				{
					aux -> prochaineBoite = (boiteCellule*) malloc(sizeof(boiteCellule));	
					aux -> prochaineBoite -> nbr_boite = cible;
					aux -> prochaineBoite -> nombre_des_lettres = 0;
					aux -> prochaineBoite -> prochaineBoite = NULL;
					aux -> prochaineBoite -> premLettre = NULL;

					(BAL -> nombre_des_boites)++;
					returnval = (BAL -> nombre_des_boites); //de la même manière que nous avons ajouté le nombre de boîtes, 
										//nous changeons la valeur de retour de la nouvelle valeur de boîte 
															
				}
		    }

		return returnval;
}



//Fonction qui affiche toutes le boites dans un liste des boites,
//avec leur numéro de boite, et combien lettres elles contiennent.
//
//Paramètre d'entrée:
//Tete: Struct qui pointe vers la tete de la liste que l'on va afficher.
void afficherBoites(struct BAL tete)  
{
		boiteCellule* aux = tete.premiere_Boite;

		printf("\n");

		while(aux != NULL)
		{
				printf("\n");
				printf("=====================================\n");
				printf("Boite à lettres numéro: %i\n", aux -> nbr_boite);
				printf("La boite contient %i lettres\n", aux -> nombre_des_lettres);
				printf("=====================================\n");
				aux = aux -> prochaineBoite;
		}
		printf("\n");
}


//Fonction qui affiche toutes les lettres dans un liste des lettres pour une boite ciblée.
//
//Paramètres d'entrée:
//tete: structure qui pointe sur la première boite des lettres
//dans la liste
//cible: numéro de la boite ciblée
void afficherLettres(struct BAL tete, int cible) 
{
		//Pointeurs pour itérer dans la liste
		boiteCellule* auxBoite = tete.premiere_Boite;
		lettreCellule* auxLettre;

		while ((auxBoite -> nbr_boite) != cible && auxBoite -> prochaineBoite != NULL) //Itérer parmis la liste de boîte en cherchant pour la boîte visée
		{	
				auxBoite = auxBoite -> prochaineBoite;
		}
		//auxBoite pointe vers la boîte ciblée

		auxLettre = auxBoite -> premLettre; //Pointeur pour itérer au-dessus de la liste des lettres
		int i = 1;
		printf("\n");
		printf("La boite %i contient %s\n",cible, (auxLettre == NULL) ? "rien" : "les lettres suivantes:");
		while (auxLettre != NULL)
		{
				printf("\n");
				printf("============================================\n");
				printf("Le %i-ieme lettre contient:\n", i);
				printf("%s\n",auxLettre -> Contenu);
				printf("============================================\n");
				i++;
				auxLettre = auxLettre -> prochaineLettre;
		}
		printf("\n");
}


//Fonction qui rajoute un cellule de lettre à la fin d'un liste de lettres,
//en remplissant le contenu de la lettre avec la chaine des caractères
//fourni en entrée
//
//Paramètres d'entrée:
//Head: double pointeur sur la première lettre dans la liste cible.
//mesg: pointeur sur la chaine des caractères qui sera stocké dans la lettre.
//On rappele que la longueur maximale des lettres est defini dans la constante
//LONGUEURLETTRESMAX
void ajouteLettre(struct lettreCellule** head, char* mesg) 
{
		if (*head == NULL) //Case où il n'y a pas de lettres dans la liste
		{
				*head = (lettreCellule*) malloc(sizeof(struct lettreCellule));
				strcpy(((*head) -> Contenu),mesg);
				(*head) -> prochaineLettre = NULL;
		}
		else if ((*head) -> prochaineLettre == NULL) //Case où il n'y a qu'une lettre dans la liste
		{
				((*head) -> prochaineLettre) = (lettreCellule*) malloc(sizeof(struct lettreCellule));
				strcpy((((*head) -> prochaineLettre) -> Contenu),mesg);
				(((*head) -> prochaineLettre) -> prochaineLettre) = NULL;
		}
		else //Il y a plus qu'une lettre, on itére par dessus la liste comme d'habitude
		{
				lettreCellule* aux = *head;
				while(aux -> prochaineLettre != NULL)
				{
						aux = aux -> prochaineLettre;
				}

				//A la fin de la liste on peut maintenant ajouter une boîte
				aux -> prochaineLettre = (lettreCellule*) malloc(sizeof(struct lettreCellule));
				strcpy(((aux -> prochaineLettre) -> Contenu),mesg);
				aux -> prochaineLettre -> prochaineLettre = NULL;
		}
}


//Fonction qui ajoute un suite des lettres à la fin d'une boite ciblée,
//rajoutant la boite si elle n'existe pas déjà. La fonction decoupe un string contient
//toutes les lettres.
//
//Paramètres d'entrée:
//tete: Pointeur sur la structure qui pointe sur la première boite dans la liste.
//mesg: Pointeur sur la chaine de caractères contient toutes les lettres.
//cible: La boite de lettres ciblée.
//nbr_messages: Nombre des lettres contenu dans mesg
//long_messages: Longueur de chaqune de lettres.
void insererLettres(struct BAL* tete, char* mesg, int cible, int nbr_messages, int long_messages)
{
		if (ajouteBoite(tete, cible) != -1) //Ajout de la boîte ciblée dans la liste avec un message indiquant si la boîte existe déjà
		{
				printf("\nOn a Ajouté boite nr: %i\n", cible);
		}

		//On itére dans la liste des boîtes jusqu'à que nous atteignons la boîte ciblée, puis nous ajoutons les lettres à la fin de la liste des lettres
	
		boiteCellule* aux = tete -> premiere_Boite; //pointeur auxiliaire pour itérer par dessus la liste chaîné de boites
		while ((aux -> nbr_boite) != cible && aux -> prochaineBoite != NULL)
		{	
				aux = aux -> prochaineBoite;
		}
		//Aux now points to the target box.

		lettreCellule** head = &(aux -> premLettre); //Pointeur double à utiliser avec AppendLetter()

		char Lettre[long_messages+1]; //Variable utilisées pour couper le string d'entrée en lettres individuels
		int i; 
		int j;
		for (i=0;i<nbr_messages;i++)
		{
				strncpy(Lettre,(mesg+i*long_messages),long_messages);
				Lettre[long_messages] = '\0';
				ajouteLettre(head,Lettre);
				(aux -> nombre_des_lettres)++;
				//printf("Letter number %i added to box %i\n", i, cible);
		}
		printf("Done adding letters to box %i\n", cible);
		printf("Box %i now contains %i letters\n", cible, (aux -> nombre_des_lettres));
} 


//Fonction qui enlève la première lettre dans un boite (suppression au début de la liste)
//La chaine des caractères contenu dans la lettre est stocké dans la chaine
//LettreSortie.
//
//Paramètres d'entrée:
//tete: Structure qui pointe vers la première boite dans la liste.
//cible: numéro de boite de la boite ciblée
//LettreSortie: Pointeur sur la chaine dans lequel la lettre enlevé sera stocké.
//
//Paramètre de sortie:
//-2 Si il n'y a aucune boite dans la liste.
//-1 Si la boite ciblée n'est pas dans la liste.
//-3 Si la boite existe, mais elle est vide.
int enleverLettre(BAL tete, int cible, char* LetterOut)
{
		if (tete.premiere_Boite == NULL) {printf("\nIl n'y a aucune boite dans la liste!\n"); return -2;}

		boiteCellule* aux = tete.premiere_Boite;

		while(aux -> prochaineBoite != NULL && (aux -> nbr_boite) != cible)
		{
				aux = aux -> prochaineBoite;
		}
		//Aux pointe soit sur la boîte ciblée soit sur la dernière boîte. Si il ne pointe pas sur la boîte ciblée, cela veut dire qu'elle n'existe pas

		if (!(aux -> nbr_boite == cible)) {printf("\nLa boite cible n'est pas dans la liste!\n"); return -1;}

		if (aux -> premLettre == NULL) return -3; //Vérifie si la boîte est vide
		else
		{
				strcpy(LetterOut,(aux -> premLettre) -> Contenu);

				lettreCellule* cellule_qui_sera_enleve = aux -> premLettre;
				(aux -> premLettre) = aux ->premLettre -> prochaineLettre;
				free(cellule_qui_sera_enleve);
				aux -> nombre_des_lettres--;
		}


		return 0;
}

