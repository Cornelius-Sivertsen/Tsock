# Projet tsock, 3ième année AE, INSA-Toulouse.

L'objectif du BE est de réaliser une application de boîtes aux lettres sur Internet en utilisant l'API socket. Le langage de programmation utilisé est le C. 
Il y a 2 parties dans ce TP : 
  - 1) Principalement dédiée à la prise en main de l’API socket avec comme objectif de mettre en oeuvre l’application « tsock » utilisée dans le TP d’introduction aux réseaux, l'application tsock consiste à la mise en place de 2 protocoles de communication UDP et TCP.
  - 2) Vise le développement du système de boites aux lettres BAL.

Le projet consiste de différents fichiers :
- readme.md 
- tsock_v2
- tsock_v3
- un librairie qui gère la partie liste chainée de la BAL.t

Les commandes pour utiliser les programmes : 

**TSOCK-V2** :

- Pour compiler tsock-v2:
```
gcc -Wextra -Wall -o tsock-v2.exe tsock-2.c -g -lm

```

- pour la partie **UDP** :

Puit :
```
./tsock-v2.exe -p -u n°port

```
Source :
```
./tsock-v2.exe -s -u [-option n,l] nom_destinataire port_destinataire

```
- pour la partie **TCP** :

Puit :
```
./tsock-v2.exe -p [-option n,l] n°port //laisse la possibilité à l'utilisateur de n'afficher que n message(s) de l longueur(s)

```
Source :
```
./tsock-v2.exe -s [-option n,l] nom_destinataire port_destinataire 

```

**TSOCK-V3** :


  1) compiler la librairie libBAL.h
```
  gcc -Wextra -Wall -o libBAL.o -c libBAL.c

```

  2) Compiler tsock-v3
  ```
  gcc -Wextra -Wall -o tsock-v3.exe tsock-v.c libBAL.o -lm
  
  ```

  3) Utilisation :

    1) Ouvrir la BAL :
       ```
       ./tsock-v3.exe -b n°port

       ```
    2) Emetteur
       ```
        ./tsock-v3.exe -e n°cible nom_machine_BAL n°port_BAL //le n°cible est le numéro du récepteur pour lequel on veut envoyer les messages en passant par la BAL
       
       ```
    3) Récepteur
       ```
       ./tsock-v3.exe -r n°cible nom_machine_BAL n°port_BAL //le n°cible est le numéro du l'émetteur dont on veut recevoir les messages qu'il a envoyé à la BAL
       
       ```
