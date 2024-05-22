# Mini-rapport pour le BE

Pour le fonctionnement des programmes. Lire le readme.md

## Positionnement de notre production finale par rapport aux attendus du projet
Partie TCP, UDP : **tsock-2.c**
Les protocoles UDP et TCP fonctionnent sur le réseau local et l'échange d'informations se fait sur les deux machines connectées à internet. Des messages d'erreur apparaissent lorsque l'utilisateur utilise mal les protocoles.  De plus, nous avons respecté le cahier de charges, sauf peut-êtres certains points sur l’affichage.
L'utilisateur peut choisir la longueur des messages envoyés ainsi que la taille de ces derniers.
Nous nous sommes donc arrêté à la version v3 du sujet qui est contenu dans tsock-2.c

Pour la partie BAL : **tsock-3.c**
L'émetteur envoie les messages, la BAL reçoit et stocke les messages en fonction du numéro du récepteur et les envoie à l'émetteur quand ce dernier lui demande. 

## Ce qui marche et ce qui ne marche pas

À la fin du projet, nous avons l’impression d’avoir presque tout réussi. Le programme fonctionne comme demandé, sans aucune erreur “typique” (segfault, memory leak, etc…). Le seul problème important qui reste est le suivante:

Pour la BAL, après avoir traité un grand nombre de requêtes, si nous fermons le programme, et le relancons sur le même port, l'émetteur et le récepteur n’arrivent pas à se connecter au serveur.  
Nous avons le même problème si on lance la BAL, et on la ferme, et enfin on la relance plusieurs fois sur un même port, même sans traiter beaucoup de requêtes.

Nous avons essayé plusieurs approches pour régler ce problème, mais à la fin, nous n’y arrivons pas.

Nous notons aussi que l’un des étudiants dans le binôme n’est pas français. À cause de ça, il peut y avoir des fautes orthographes dans les commentaires et noms de variables dans le code. De plus, il reste certains commentaires et variables en anglais dans le code, notamment dans le code pour la librarie libBAL.c. Nous nous en excusons.


## Choix remarquables

Le seul choix remarquable est que nous avons mis tout le code qui traite la partie BAL (listes chaînées, insertion et extraction de la boîte, etc…) dans notre propre librairie nommée libBAL.c. 
