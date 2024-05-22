# Mini-rapport pour le BE

## Positionnement de notre production finale par rapport aux attendus du projet

## Ce qui marche et ce qui ne marche pas

À la fin du projet, nous avons l’impression d’avoir presque tout réussi. Le programme marche fonctionne comme demandé, sans aucun erreur “typique” (segfault, memory leak, etc…). De plus, nous avons respecté le cahier de charges, sauf peut-êtres certains points sur l’affichage. Le seul problème important qui reste est le suivante:

Pour la BAL, après avoir traité un grand nombre de requêtes, si nous fermons le programme, et le relance sur la même port, l'émetteur et le récepteur n’arrivent pas à se connecter au server.  
Nous avons le même problème si on lance, et ferme, et relance la BAL plusieurs fois sur un même port, même sans traiter beaucoup de requêtes.

Nous avons essayé plusieurs approches pour régler ce problème, mais à la fin, nous n’y arrivons pas.

Nous notons aussi que l’un des étudiants dans le binôme n’est pas français. À cause de ça, il peut y avoir des fautes orthographes dans les commentaires et noms de variables dans le code. De plus, il reste certains commentaires et variables en anglais dans le code, notamment dans le code pour le librarie libBAL.c. Nous nous en excusons.


## Choix remarquables

Le seul choix remarquable est que nous avons mis tout le code qui traite la partie BAL (listes chaînées, insertion et extraction de la boîte, etc…) dans notre propre librairie. 
