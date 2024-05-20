# Projet tsock, 3ième année AE, INSA-Toulouse.

Pour compiler tsock-v2:
```
gcc -Wextra -Wall -o tsock-v2.exe tsock-2.c -g -lm
```

Le projet consiste de:

- tsock_v2
- tsock_v3
- un librairie qui gère la partie liste chainée de la BAL.t 

Les commandes pour utiliser le programme : 

pour la partie UDP :

Puit :
```
./tsock-v2.exe -p -u n°port

```
Source :
```
./tsock-v2.exe -s -u [-option n,l] nom_destinataire port_destinataire

```
pour la partie TCP :

Puit :
```
./tsock-v2.exe -p [-option n,l] n°port //laisse la possibilité à l'utilisateur de n'afficher que n message(s) de l longueur(s)

```
Source :
```
./tsock-v2.exe -s [-option n,l] nom_destinataire port_destinataire 

```
