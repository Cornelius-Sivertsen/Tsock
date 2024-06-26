/*
Librarie qui implemente la partie liste chainée du projet boite
aux lettres en Reseaux-C.
La librarie contient les structs utilisées dans les listes chainées, et les
fonctions utilisées pour les manipuler, et pour débuger les problèmes éventuelles.
En fonctionnement normal, la programme principal ne fait appel qu'à deux
fonctions: insererLettres et enleverLettre, pour rajouter et enlever des lettres respectivement.
 */
#ifndef _libBAL_
#define _libBAL_


#define LONGUEURLETTRESMAX 100 //Longueur maximale des lettres stockées dans la boite aux lettres. 
							   //Nécessaire pour la fonctionnement de la librairie.


//Struct pour cellules des lettres dans la liste chainée des lettres.
typedef struct lettreCellule{
		char Contenu[LONGUEURLETTRESMAX];
		struct lettreCellule* prochaineLettre;
} lettreCellule;

//Struct pour cellules des boites dans la liste chainée des boites.
typedef struct boiteCellule{
		int nbr_boite;
		int nombre_des_lettres;
		struct boiteCellule* prochaineBoite;
		struct lettreCellule* premLettre;
} boiteCellule;

//Struct qui pointe sur la tête d'un liste de boites aux lettres
typedef struct BAL{
		int nombre_des_boites;
		struct boiteCellule* premiere_Boite;
} BAL;


//Fonction qui ajoute une suite de lettres à la fin d'une boite ciblée,
//rajoutant la boite si elle n'existe pas déjà. La fonction découpe un string contient
//toutes les lettres.
//
//Paramètres d'entrées:
//tete: Pointeur sur la structure qui pointe sur la première boite dans la liste.
//mesg: Pointeur sur la chaine de caractères contient toutes les lettres.
//cible: La boite de lettres ciblée.
//nbr_messages: Nombre des lettres contenues dans mesg
//long_messages: Longueur de chacune des lettres.
void insererLettres(struct BAL* tete, char* mesg, int cible, int nbr_messages, int long_messages);


//Fonction qui enlève la première lettre dans une boite (suppression au début de la liste)
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
//0 Sinon
int enleverLettre(struct BAL tete, int cible, char* LettreSortie);

//Fonction qui rajoute un boite vide à la fin de la liste des boites.
//Si la boite existe déjà dans la liste, la fonction ne fait rien.
//
//Paramètres d'entrés:
//BAL: Pointeur sur la struct qui pointe vers la tete de la liste dans laquelle
//on va ajouter une boite.
//Cible: le numéro de boite de la boite que l'on va ajouter.
//
//Valeur retournée:
//Nombre des boites dans la liste après le rajout dans le cas où on ajoute un boite, -1 dans le cas où le boite était déjà dans la liste (aucun rajout).
int ajouteBoite(struct BAL * BAL, int cible);


//Fonction qui rajoute un cellule de lettre à la fin d'un liste de lettres,
//en remplissant le contenu de la lettre avec la chaine des caractères
//fourni en entrée
//
//Paramètres d'entrée:
//Head: double pointeur sur la première lettre dans la liste cible.
//mesg: pointeur sur la chaine des caractères qui sera stocké dans la lettre.
//On rappele que la longueur maximale des lettres est definie dans la constante
//LONGUEURLETTRESMAX
void ajouteLettre(struct lettreCellule** head, char* mesg);

//Fonction qui retourne information sur une boite.
//
//Paramètres d'entrée:
//tete: struct qui pointe vers la tete de la liste des boites
//cible: le numéro de boite
//
//Valeurs de sortie:
//-2 Si il n'y a aucune boite dans la liste.
//-1 Si la boite ciblée n'est pas dans la liste.
//-3 Si la boite existe, mais elle est vide.
//Sinon: retourne le nombre de lettres dans la boite.
int boiteInfo(struct BAL tete, int cible);


//Fonction qui affiche toutes les boites dans une liste des boites,
//avec leur numéro de boite, et combien lettres elles contiennent.
//Outil pour debeugage.
//
//Paramètre d'entrée:
//Tete: Struct qui pointe vers la tete de la liste que l'on va afficher.
void afficherBoites(struct BAL tete);

//Fonction qui affiche toutes les lettres dans une liste des lettres pour une boite ciblée.
//
//Paramètres d'entrée:
//tete: structure qui pointe sur la première boite des lettres
//dans la liste
//cible: numéro de la boite ciblée
void afficherLettres(struct BAL tete, int cible);
#endif
