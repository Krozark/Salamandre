Idées projet Systèmes Complexes
===============================

Stockage robuste, fiable et sˆur de donn ́ees numeriques par syst`emes complexes


tous les sockets en UDP

Périmètre du projet
-------------------

L'application que l'on va développer est le système de sauvegarde de fichiers chiffrés sensibles.
L'application devra connaitre le type de fichier (FEC, FCT, FMT ou FMN) mais ne sera pas capable de comprendre les données (chiffrées)
L'application ne gèrera pas le chiffrage des données, géré par un outil externe (ou alors lancement d'un script permettant de le faire ?)
Le lancement de l'application créera un noeud d'un réseau et sera sous la forme d'un daemon, au moins une socket d'écoute devra être ouverte pour accepter les connexions des autres noeuds souhaitant sauvegarder
L'application devra accepter comme paramètre le port à écouter pour la sauvegarde ainsi que le répertoire à surveiller pour l'envoi des sauvegardes


Connaissance du réseau
----------------------

Pour pouvoir "choisir" les noeuds de sauvegarde, un noeud devra connaitre l'ensemble des autres noeuds. Plusieurs moyens sont proposés pour faire ce travail.
Le plus simple est de donner une liste des noeuds sous forme de fichier texte. moyen rapide et efficace mais pas flexible, l'ajout d'un noeud nécessiterait la modification de tous les autres noeuds.
Une autre solution serait l'auto-discover des autres noeuds, qui présuppose que les autres noeuds soient dans le même sous-réseau que la machine courante. Plusieurs techniques d'auto discover sont possible. Inconvéniant: ne fonctionne pas pour découvrir plusieurs noeuds sur une même machine.
Une autre solution serait de prendre la liste d'un référenciel connu, un serveur maitre contenant la liste des noeuds. Facile à mettre à jour, mais rend le système dépendant du fonctionnement du serveur maitre.

La solution idéale serait d'implémenter toutes les solutions et laisser choisir l'utilisateur au moment du démarrage du serveur.

Auto-discover des noeuds
------------------------

Deux techniques d'auto discover sont possibles, une dite "active" et une autre "passive"

Technique active:
Tous les noeuds écoutent sur un port (en UDP of course). Quand un noeud a besoin de connaitre tous les autres, il envoie une requête en broadcast sur ce port, avec sa propre ip et port auquel il faut répondre.
Chaque noeud reçoit la requête et envoit son ip et son port à l'expéditeur de la requête. Problème : Combien de temps attendre les réponses ?

Technique passive:
Tous les noeuds écoutent sur un port (toujours en UDP). Chaque noeud envoit périodiquement son adresse ip et son port (ou une liste...). À la réception du message, chaque noeud met à jour sa liste locale de noeuds. Chaque noeud enregistré expirera au bout d'un certain temps (2x la période de notification).

Procédure de sauvegarde
-----------------------

Chaque noeud surveille un répertoire du système de fichier. Quand un fichier y est détecté, il enclenche la procédure de sauvegarde.

Le noeud choisi 4 noeuds différents parmi la liste des noeuds disponibles pour chaque dossier à sauvegarder (un par type de fichier). L'algorithme de choix est encore à déterminer. Les fichiers sont alors envoyé à ces noeuds là avec des métadonnées, comme le nom du fichier, sa taille, son type, sa provenance, sa version, un hash... Cet étape peut être répétée autant de fois que configuré (en enlevant de la liste les noeuds déjà choisis...). une fois le fichier envoyé aux différents noeuds, il est supprimé du dossier d'attente. À la réception, le hash est vérifié, si tout est correct, une notification est envoyée à l'expéditeur et le fichier est sauvegardé dans un dossier spécifique à la provenance.
L'expéditeur pourra, à la réception de la notification d'enregistrement, enregistrer cette valeur dans les statistiques du noeud pour alimenter l'indice de fiabilité.

Notion de groupe
----------------

La "provenance" d'un fichier peut être un identifiant de site. De cette façon on pourra avoir plusieurs noeuds pour un même site fonctionnant indépendemment. Dans la configuration des noeuds il faudra ajouter
cette notion afin de permettre cette utilisation. Impact sur le choix des noeuds lors de la sauvegarde: il faudra exclure de la liste les noeuds du même groupe.


Récupération des fichiers
-------------------------

Il peut arriver qu'un noeud perde l'intégralité de ces données. Il faut permettre à ce noeud de les récupérer à l'aide de ce qu'il avait auparavant sauvegardé dans les autres noeuds. pour ça il faut créer un
message de demande de récupération des fichiers, comportant la provenance souhaité. Ainsi, le serveur n'aura qu'à renvoyer l'ensemble des fichiers du répertoire de la provenance.

Authentification des noeuds
---------------------------

Il ne faudrait pas que n'importe qui puisse demander l'intégralité des fichiers d'un noeud. Il est nécessaire d'avoir un moyen d'authentifier les noeuds avant d'accéder à leurs demandes. Peut être via un 
service externe ou une vérification de la signature des fichiers (s'il y en a une)

Format normalisé de fichiers
----------------------------

Comme il est nécessaire de scanner un répertoire pour savoir qu'un fichier doit être sauvegardé, il devrait contenir suffisamment d'informations dans son nom pour permettre le fonctionnement du programme.

Il est proposé :

{TYPE_FICHIER:FEC,FCT,FMT,FMN}_{NUMERO_PATIENT}_v{VERSION}.tar.bz2.crypt

On imposera une limite de 128 caractères au nom de fichier


Reste à voir
------------

- Protocole de transfert
- Configuration des noeuds
- Vérifier le périmètre : on gère le chiffrage ou pas ?
- Connaissance du réseau mixte fichier de noeuds + découverte réseau local
- Chiffrement des fichiers, voir http://linux-attitude.fr/post/chiffrement-de-fichiers
