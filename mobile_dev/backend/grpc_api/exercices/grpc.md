# Exercices — gRPC

Ces exercices accompagnent la leçon [01_grpc.md](../lessons/01_grpc.md). Garde-la ouverte à côté : le service de messagerie de la section 8 sert de référence à plusieurs exercices.

Les six niveaux sont de difficulté croissante. Ne saute pas les premiers même s'ils te paraissent faciles — lire du `.proto` couramment est la compétence qui rend tout le reste possible.

> **Prérequis :** les niveaux 1 à 3 se font avec un éditeur de texte seul, aucune installation. Le niveau 4 demande `protoc` (voir §12 de la leçon). Le niveau 5 demande en plus un langage de haut niveau que tu n'as pas encore vu — il est écrit pour que tu y reviennes plus tard, et c'est normal de le laisser de côté aujourd'hui. Le niveau 6 se fait sur papier.

Une convention dans ce fichier : les exercices marqués **[papier]** se réfléchissent et s'écrivent en français avant toute ligne de code. Ne les expédie pas, c'est là que se joue la vraie compréhension.

---

## Niveau 1 — Lire un .proto

Le but est de savoir ce que tu regardes avant de savoir l'écrire.

### Exercice 1.1 — Inventaire

Ouvre le `.proto` de la section 8 de la leçon. Sans rien exécuter, dresse dans un fichier texte la liste de tous les `message` qu'il définit, et pour chacun le nombre de champs. Compte aussi combien de méthodes `rpc` contient le service.

### Exercice 1.2 — Données ou transport ?

Toujours dans le même fichier, classe chaque `message` en deux catégories : ceux qui représentent une **donnée métier** (une chose qui existe et qu'on stocke) et ceux qui ne servent qu'à **transporter** les paramètres ou le résultat d'un appel. Justifie chaque classement en une phrase.

### Exercice 1.3 — Les numéros

Relis la déclaration de `Message`. Pourquoi son champ `texte` porte-t-il le numéro 4 et pas un autre ? Que se passerait-il concrètement, côté client déjà installé sur des téléphones, si le serveur passait ce numéro de 4 à 7 ? Réponds en décrivant ce que lirait le client.

### Exercice 1.4 — Traduire vers le C

Écris l'équivalent en C, sous forme de `struct`, du message `Utilisateur`. Puis fais la même chose pour `Message`. Note ensuite les deux ou trois choses que le C ne sait **pas** exprimer et que le `.proto` exprime.

### Exercice 1.5 — Repérer les modes

Pour chacune des quatre méthodes du service `Messagerie`, dis de quel type d'appel il s'agit (unaire, server streaming, client streaming, bidirectionnel), et à quoi tu l'as reconnu dans la signature. Recopie l'indice exact.

---

## Niveau 2 — Écrire ses propres .proto

Écris ces fichiers à la main dans un dossier de travail. Ils ne seront pas compilés avant le niveau 4, donc concentre-toi sur la structure et la cohérence.

### Exercice 2.1 — Une bibliothèque

Écris un fichier `bibliotheque.proto` avec la syntaxe proto3 et un package versionné. Il doit définir un message `Livre` avec au minimum un identifiant, un titre, un auteur, une année de parution et le nombre de pages. Choisis pour chaque champ le type le plus adapté, et sois prêt à justifier chaque choix.

### Exercice 2.2 — Des listes

Ajoute à ton `Livre` un champ contenant plusieurs genres (« roman », « policier », « historique »). Ajoute ensuite un message `Auteur` contenant plusieurs `Livre`. Quel mot-clé utilises-tu, et pourquoi ne peux-tu pas simplement mettre `string genres = 6;` ?

### Exercice 2.3 — Le bon type pour une date

Ton `Livre` a une année de parution. Écris trois variantes possibles pour ce champ : en `string`, en `int32`, et en `int64` de millisecondes Unix. Pour chacune, note un avantage et un inconvénient concret. Laquelle gardes-tu ici, et pourquoi la réponse serait-elle différente pour un champ « date d'emprunt » ?

### Exercice 2.4 — Un oneof

Un événement de bibliothèque est soit un emprunt, soit un retour, soit une réservation, jamais deux à la fois. Écris un message `EvenementBibliotheque` qui exprime cette contrainte avec `oneof`. Explique en une phrase ce que tu perdrais en écrivant simplement trois champs optionnels côte à côte.

### Exercice 2.5 — Le service

Complète `bibliotheque.proto` avec un `service Bibliotheque` exposant au moins : récupérer un livre par son identifiant, rechercher des livres par mot-clé, et emprunter un livre. Respecte la règle « un message dédié par requête et par réponse », même quand un seul champ suffirait.

---

## Niveau 3 — Types d'appels et choix de conception

Ce niveau est celui où l'on décide, pas où l'on tape.

### Exercice 3.1 — Attribuer un mode **[papier]**

Pour chacune de ces situations, choisis le type d'appel gRPC le plus adapté et justifie en deux phrases :

| Situation | Ton choix et pourquoi |
|---|---|
| Récupérer le profil d'un utilisateur | |
| Suivre en direct la position d'un livreur | |
| Envoyer les 500 photos d'une sauvegarde | |
| Un salon de discussion à plusieurs | |
| Vérifier si un pseudo est disponible | |
| Recevoir les notifications tant que l'app est ouverte | |
| Envoyer les mesures d'un capteur toutes les secondes pendant une heure | |

### Exercice 3.2 — Le piège de la grosse liste **[papier]**

Une méthode `rpc ListerTousLesLivres(Vide) returns (ListeLivres);` renvoie 200 000 livres d'un coup. Décris ce qui se passe côté mémoire du client mobile. Propose ensuite deux corrections différentes : une avec du streaming, une sans. Compare-les.

### Exercice 3.3 — Réécrire en streaming

Reprends ton `bibliotheque.proto`. Transforme la recherche par mot-clé en server streaming. Qu'est-ce qui change dans la signature ? Qu'est-ce qui change pour celui qui appelle la méthode ?

### Exercice 3.4 — Un message vide

Certaines méthodes n'ont besoin d'aucun paramètre (« donne-moi l'heure du serveur »). Comme une méthode gRPC prend obligatoirement un message, comment fais-tu ? Écris la solution, et explique pourquoi elle est préférable à supprimer le paramètre.

### Exercice 3.5 — Le champ qu'on regrette **[papier]**

Ton message `Livre` a un champ `string auteur = 3;`. Six mois plus tard, il faut gérer les livres à plusieurs auteurs. Tu ne peux ni changer le numéro 3, ni casser les applications mobiles déjà installées. Propose une stratégie complète, en précisant ce que fait le serveur pendant la période de transition.

---

## Niveau 4 — Générer du code avec protoc

À partir d'ici, il te faut `protoc` installé (leçon §12). Ces exercices se font entièrement dans ton terminal : c'est l'occasion de réutiliser `cd`, `ls`, les chemins relatifs et absolus.

### Exercice 4.1 — Vérifier l'installation

Depuis ton terminal, affiche la version de `protoc`. Puis trouve où l'exécutable est réellement installé sur ton disque, et affiche le chemin absolu. Si la commande n'est pas trouvée, relis la partie `PATH` de la leçon avant de continuer.

### Exercice 4.2 — Première génération

Crée un dossier de travail avec un sous-dossier `genere/`. Lance `protoc` sur ton `bibliotheque.proto` pour produire du Python dans `genere/`. Utilise `--proto_path` explicitement plutôt que de compter sur le dossier courant.

### Exercice 4.3 — Lire le résultat

Liste les fichiers créés dans `genere/`. Ouvre-les. Tu ne comprendras pas tout, et ce n'est pas le but : repère uniquement l'endroit où apparaissent les noms de tes champs, et l'endroit où apparaissent leurs numéros. Note ce que tu observes.

### Exercice 4.4 — L'erreur volontaire

Introduis délibérément une faute dans ton `.proto` : donne le même numéro à deux champs d'un même message. Relance `protoc`. Lis le message d'erreur en entier et note le numéro de ligne qu'il indique. Répare, relance, vérifie.

### Exercice 4.5 — Les stubs de service

Relance la génération, cette fois avec l'option qui produit aussi les stubs gRPC (`--grpc_python_out`). Compare la liste des fichiers avec celle de l'exercice 4.2. Quel fichier apparaît en plus, et pourquoi n'était-il pas là avant ?

### Exercice 4.6 — Un autre langage

Génère le même `.proto` vers un deuxième langage de sortie (par exemple `--cpp_out`). Compare la taille et le nombre des fichiers produits. Qu'est-ce que ça te dit sur le rôle du `.proto` dans une équipe où le backend et l'app mobile ne sont pas écrits dans le même langage ?

---

## Niveau 5 — Implémenter un service

> **Prérequis :** ce niveau demande un langage de haut niveau (Python, Go, Kotlin…) que tu n'as pas encore travaillé, et pour la partie streaming, la programmation asynchrone. Ne force pas aujourd'hui. Reviens ici après ta première leçon de langage — les exercices t'attendront et ils prendront tout leur sens à ce moment-là.

### Exercice 5.1 — Le serveur minimal

En repartant du code généré, écris un serveur qui implémente **une seule** méthode unaire : celle qui récupère un livre par son identifiant. Renvoie toujours le même livre codé en dur. Le but est uniquement de voir le serveur démarrer et écouter sur un port.

### Exercice 5.2 — Le client minimal

Écris un client qui se connecte à ton serveur, appelle la méthode, et affiche le livre reçu. Vérifie que ce que tu affiches correspond bien à ce que le serveur a codé en dur.

### Exercice 5.3 — Tester sans écrire de client

Avec `grpcurl` depuis ton terminal, appelle ta méthode sans passer par ton propre client. Compare l'effort avec ce que serait la même chose en REST avec `curl`, et note honnêtement laquelle des deux est plus agréable.

### Exercice 5.4 — Le cas d'erreur

Que doit-il se passer quand on demande un identifiant qui n'existe pas ? Cherche la notion de **code de statut gRPC** (`NOT_FOUND` en particulier), et fais renvoyer à ton serveur une erreur propre plutôt qu'un livre vide. Explique pourquoi un livre vide serait un mauvais choix.

### Exercice 5.5 — Un peu de stockage

Remplace le livre codé en dur par une petite collection en mémoire contenant trois ou quatre livres. Implémente maintenant la recherche par mot-clé en unaire, puis, si tu te sens à l'aise, en server streaming.

---

## Niveau 6 — Réflexion et architecture

Tout ce niveau se fait à l'écrit, en français, sans code. Vise un paragraphe argumenté par question, pas une phrase.

### Exercice 6.1 — Le choix pour ton projet **[papier]**

Tu construis une application mobile de suivi d'entraînement sportif. Elle a besoin de : télécharger la liste des séances, envoyer une séance terminée, et afficher en direct la fréquence cardiaque pendant l'effort. Pour chacun de ces trois besoins, choisis entre REST, gRPC et WebSocket, et défends ton choix. As-tu le droit d'en mélanger plusieurs dans une même application ?

### Exercice 6.2 — L'argument contraire **[papier]**

Tu proposes gRPC à une équipe. Un collègue répond : « On perd la possibilité de déboguer avec `curl`, et le nouveau qui arrive ne saura pas lire les échanges réseau. » Ce n'est pas un mauvais argument. Écris une réponse honnête : ce que tu concèdes, et ce que tu mets en face.

### Exercice 6.3 — Versionner **[papier]**

Ton API `messagerie.v1` est déployée dans une application installée sur 50 000 téléphones. Tu dois supprimer un champ devenu inutile. Décris précisément la marche à suivre, dans l'ordre, en tenant compte du fait que tu ne contrôles pas la date de mise à jour des applications installées. Combien de temps garderas-tu les deux versions en parallèle, et qu'est-ce qui déterminera ce délai ?

### Exercice 6.4 — Compatibilité ascendante et descendante **[papier]**

Cherche la différence entre un changement **rétrocompatible** et un changement **cassant** en protobuf. Classe ensuite ces cinq modifications dans l'une des deux catégories, avec une justification : renommer un champ ; changer son numéro ; ajouter un nouveau champ ; supprimer un champ ; changer le type d'un champ de `int32` à `string`.

### Exercice 6.5 — La frontière du navigateur **[papier]**

Ton backend est en gRPC et sert une app mobile native, ce qui marche bien. On te demande d'ajouter un site web qui affiche les mêmes données. Décris les deux architectures possibles (une passerelle REST devant le gRPC, ou grpc-web avec un proxy) et compare-les sur trois critères de ton choix.

### Exercice 6.6 — Les erreurs **[papier]**

En REST, on utilise les codes de statut HTTP (404, 500…). gRPC a son propre jeu de codes. Trouve-en au moins cinq, dis ce que chacun signifie, et associe chacun à une situation concrète dans ton application de bibliothèque.

---

## Auto-évaluation

Coche honnêtement. Une case non cochée n'est pas un échec, c'est juste un endroit où revenir.

**Comprendre**

- [ ] Je peux expliquer en trois phrases ce que gRPC résout, sans utiliser le mot « performance »
- [ ] Je sais ce qu'est un stub et pourquoi je n'ai jamais à l'écrire
- [ ] Je sais faire la différence entre Protocol Buffers et gRPC
- [ ] Je sais pourquoi protobuf n'envoie pas les noms de champs sur le réseau
- [ ] Je peux expliquer le multiplexage HTTP/2 avec un schéma dessiné de mémoire

**Lire et écrire**

- [ ] Je lis un `.proto` inconnu et j'en fais l'inventaire sans aide
- [ ] Je reconnais les quatre types d'appels à leur seule signature
- [ ] J'écris un `.proto` valide de zéro, avec `syntax`, `package`, `message` et `service`
- [ ] Je choisis le bon type pour un champ et je sais le justifier
- [ ] Je sais quand utiliser `repeated` et quand utiliser `oneof`

**Outiller**

- [ ] `protoc --version` répond correctement sur ma machine
- [ ] Je génère du code dans le dossier de mon choix avec `--proto_path` et une option de sortie
- [ ] Je sais lire un message d'erreur de `protoc` et retrouver la ligne fautive
- [ ] Je sais pourquoi il faut deux options de sortie pour avoir aussi les services
- [ ] Je sais qu'on ne modifie jamais le code généré, et je sais dire pourquoi

**Décider**

- [ ] Devant un besoin, je tranche entre REST, gRPC et WebSocket avec des arguments
- [ ] Je cite au moins deux raisons pour lesquelles gRPC convient bien au mobile
- [ ] Je sais expliquer la limite du navigateur et ce que grpc-web ne sait pas faire
- [ ] Je sais quels changements d'un `.proto` cassent les clients déjà déployés
- [ ] Je peux défendre gRPC **et** argumenter contre, selon le contexte

---

## Où chercher les réponses

Ce projet ne fournit pas de corrigés, et c'est délibéré : chercher soi-même laisse une trace que lire une solution ne laisse pas. Voici où chercher, dans l'ordre.

**D'abord la leçon.** [01_grpc.md](../lessons/01_grpc.md) contient de quoi répondre à tout le niveau 1, presque tout le niveau 2 et l'essentiel du niveau 3. Le « Mémo » de fin est un bon point de départ pour retrouver la section utile.

**Ensuite `protoc` lui-même.** C'est ton meilleur correcteur pour les niveaux 2 et 4 : si ton `.proto` compile, sa syntaxe est bonne. `protoc --help` liste toutes les options de sortie disponibles.

**La documentation officielle**, en anglais mais très lisible : le guide du langage proto3 sur `protobuf.dev` pour tout ce qui touche aux types, `grpc.io/docs` pour les concepts et les tutoriels par langage. La page « Core concepts » de `grpc.io` couvre les quatre modes et les codes de statut du niveau 6.

**Les autres leçons du projet** pour les comparaisons : `../../rest_api/lessons/` pour REST, JSON et HTTP, `../../web_socket_api/lessons/` pour le temps réel. Les exercices 6.1, 6.2 et 6.5 sont bien plus faciles une fois ces leçons faites — si tu les trouves difficiles maintenant, c'est probablement le signe qu'il faut y aller d'abord.

**Ce qui n'a pas encore de réponse ici.** Le niveau 5 dépend d'un langage que tu n'as pas commencé, et le streaming dépend de l'asynchrone. Si tu bloques dessus aujourd'hui, ce n'est pas un manque de compréhension de gRPC : c'est simplement une brique qui n'est pas encore posée. Note ta question quelque part et reviens.

Enfin, garde une habitude : quand tu trouves une réponse, réécris-la avec tes mots dans ton propre fichier de notes. Si tu n'arrives pas à la reformuler, c'est que tu ne l'as pas encore comprise.
