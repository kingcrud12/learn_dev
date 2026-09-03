# Exercices — Firebase

Ces exercices accompagnent la leçon
[../lessons/01_firebase.md](../lessons/01_firebase.md). Relis-la avant de
commencer, et garde-la ouverte à côté.

**Comment travailler.** Les niveaux 1, 2 et 6 se font **sur papier ou dans un
fichier texte**, sans ordinateur allumé si tu veux. C'est volontaire. En NoSQL,
99 % des erreurs coûteuses sont des erreurs de modélisation, et une erreur de
modélisation ne se corrige pas avec une ligne de code : elle se corrige en
migrant des données de production. Le temps que tu passes à réfléchir avant de
taper est le temps le mieux investi de tout ce module.

**Prérequis :** les niveaux 4 et 5 demandent d'écrire du JavaScript et de
comprendre la programmation asynchrone, que tu n'as pas encore étudiée. Ils sont
là pour que tu saches où tu vas. Fais-les **en lecture** pour l'instant : lis les
énoncés, comprends ce qui est demandé, et reviens les faire réellement après le
cours sur JavaScript et l'asynchrone. Les niveaux 1 à 3 et 6, eux, sont
entièrement faisables **maintenant**.

---

## Niveau 1 — Lire et comprendre le modèle

### Exercice 1.1 — Collection ou document ?

Pour chacun des chemins suivants, dis s'il désigne une collection ou un document,
et justifie ta réponse en une phrase.

```
utilisateurs
utilisateurs/aBc123
utilisateurs/aBc123/commandes
utilisateurs/aBc123/commandes/cmd_001
boutiques/paris_11/rayons/fruits/produits
```

Rappelle-toi la règle de parité vue dans la leçon. Vérifie ensuite que ta règle
tient aussi pour les chemins de fichiers de ton terminal — la comparaison est-elle
parfaite, ou y a-t-il une différence ?

### Exercice 1.2 — Lire un document

Voici un document JSON. Réponds aux questions.

```json
{
  "titre": "Les bases du terminal",
  "auteur_nom": "Alice",
  "auteur_id": "u42",
  "publie": true,
  "vues": 1287,
  "tags": ["unix", "debutant", "shell"],
  "meta": {
    "temps_lecture_min": 8,
    "langue": "fr"
  },
  "commentaires_recents": [
    { "auteur": "Bob", "texte": "Très clair !" },
    { "auteur": "Chloé", "texte": "Merci" }
  ]
}
```

1. Combien ce document a-t-il de champs de premier niveau ?
2. Quel est le type de `tags` ? de `meta` ? de `commentaires_recents` ?
3. Comment désignerais-tu la valeur `8` par un chemin de champs ?
4. Quels champs seraient **impossibles** dans une colonne d'une table SQL
   classique, et pourquoi ?
5. Le champ `auteur_nom` est-il redondant avec `auteur_id` ? Est-ce une erreur ?

### Exercice 1.3 — Repérer une structure impossible

Un débutant propose la structure suivante pour ranger les messages d'un chat.
Trois choses ne vont pas. Trouve-les.

```
/chat
   /messages
       msg_001 : { texte: "Salut", auteur: "Alice" }
       msg_002 : { texte: "Ça va ?", auteur: "Alice" }
   /tous_les_utilisateurs
       liste: [ {nom: "Alice", ...}, {nom: "Bob", ...}, ... 12000 entrées ]
```

Indices à considérer : la limite de taille d'un document, ce qui se passe quand
un seul utilisateur change de nom, et ce que ça coûte de lire un seul
utilisateur.

### Exercice 1.4 — SQL contre Firestore

Reprends le tableau comparatif de la section 3.5 de la leçon, **cache-le**, et
reconstitue-le de mémoire sur au moins cinq lignes. Puis compare. Ce que tu n'as
pas su restituer est ce que tu dois relire.

---

## Niveau 2 — Concevoir une structure de données

Pour chacun de ces exercices, écris ta structure sous forme d'arborescence
commentée (comme dans la section 3.4 de la leçon), puis **liste les écrans** de
l'application et, pour chacun, **compte le nombre de documents à lire**. C'est ce
comptage qui valide ou invalide ton modèle.

### Exercice 2.1 — Une todo-list personnelle

Une application de tâches, chaque utilisateur ne voit que les siennes. Une tâche
a un titre, un état (faite ou non), une date d'échéance optionnelle et une
priorité.

Écrans : la liste des tâches du jour, la liste de tout ce qui reste à faire, le
détail d'une tâche.

Question de fond : les tâches vont-elles dans une collection racine `/taches`
avec un champ `proprietaire`, ou dans une sous-collection
`/utilisateurs/{uid}/taches` ? Donne au moins deux arguments pour chaque option,
et tranche.

### Exercice 2.2 — Un chat de groupe

Des salons, des membres par salon, des messages. Un message a un texte, un
auteur, un horodatage.

Écrans : la liste de mes salons avec, pour chacun, le dernier message et le nombre
de non-lus ; l'intérieur d'un salon avec les 50 derniers messages.

Questions à traiter explicitement :
1. Où ranges-tu la liste des membres d'un salon ? Un tableau dans le document du
   salon, ou une sous-collection ? Que se passe-t-il à 5 membres ? à 10 000 ?
2. Comment affiches-tu le dernier message de chaque salon **sans** lire les
   messages de chaque salon ?
3. Le nombre de non-lus est propre à chaque utilisateur. Où le stockes-tu ?

### Exercice 2.3 — Un mini réseau social

Des utilisateurs, des publications, des abonnements, des likes.

Écrans : le profil d'un utilisateur avec ses publications, un fil d'actualité
regroupant les publications des gens que je suis, le nombre de likes sous chaque
publication.

Questions :
1. Comment modélises-tu « Alice suit Bob » ? Réfléchis à ce dont tu as besoin :
   la liste des gens que je suis, et la liste de mes abonnés. Une seule structure
   peut-elle servir les deux ?
2. Le compteur de likes : un champ `nb_likes` sur la publication, ou une
   sous-collection `/likes` qu'on compte ? Quels sont les avantages de chacun, et
   comment saurais-tu si un utilisateur donné a déjà liké ?
3. Le fil d'actualité est le problème difficile. Si je suis 300 personnes,
   comment récupérer leurs publications récentes ? Cherche pourquoi Firestore ne
   permet pas de faire ça en une requête simple, et propose une approche.

### Exercice 2.4 — Critiquer ton propre modèle

Reprends ta structure de l'exercice 2.2 et réponds par écrit :

1. Quels champs y sont dupliqués ? Pour chacun, qui est responsable de le tenir
   à jour quand la source change ?
2. Si un salon atteint 200 000 messages, quelque chose casse-t-il ?
3. Quelle est la requête la plus chère de ton application, en nombre de lectures ?

---

## Niveau 3 — Écrire des Security Rules

Écris les règles dans un fichier `firestore.rules`. Le niveau 4 t'apprendra à les
tester ; pour l'instant, l'exercice est de les écrire correctement.

### Exercice 3.1 — Trouver les failles

Ces règles sont dangereuses. Trouve **au moins trois** problèmes distincts, et
explique pour chacun ce qu'un utilisateur malveillant pourrait faire.

```js
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /utilisateurs/{uid} {
      allow read, write: if request.auth != null;
    }
    match /articles/{articleId} {
      allow read: if true;
      allow write: if request.auth != null;
    }
    match /admin/{doc=**} {
      allow read: if request.auth.token.email == "moi@exemple.fr";
    }
  }
}
```

### Exercice 3.2 — Le profil privé

Écris les règles pour `/utilisateurs/{uid}` telles que :
- seul le propriétaire peut lire son propre document ;
- seul le propriétaire peut le modifier ;
- personne ne peut supprimer un profil ;
- le champ `role` ne peut **jamais** être modifié par le client, même par le
  propriétaire (sinon n'importe qui se déclare administrateur).

Ce dernier point est le plus intéressant. Indice : compare
`request.resource.data.role` et `resource.data.role`.

### Exercice 3.3 — Le blog

Structure : `/articles/{articleId}` avec les champs `titre`, `contenu`,
`auteur_id`, `publie` (booléen).

Règles attendues :
- tout le monde, même déconnecté, peut lire un article dont `publie == true` ;
- l'auteur peut lire ses propres brouillons ;
- seul un utilisateur connecté peut créer un article, et `auteur_id` doit
  obligatoirement valoir son propre UID ;
- `titre` doit être une chaîne non vide de moins de 200 caractères ;
- seul l'auteur peut modifier ou supprimer son article.

Ensuite, réponds à cette question sans écrire de code : un client fait la requête
« donne-moi tous les documents de `/articles` ». Que se passe-t-il, et pourquoi ?
Quelle requête devrait-il faire à la place ?

### Exercice 3.4 — Rédiger le commentaire

Reprends tes règles de l'exercice 3.3 et commente **chaque ligne de condition**
en français, en expliquant l'intention, pas la syntaxe. Une règle non commentée
est une règle que personne ne pourra auditer dans six mois — toi le premier.

### Exercice 3.5 — Le coût d'une règle

Cherche dans la documentation ce que fait la fonction `get()` dans les Security
Rules, écris une règle qui vérifie qu'un utilisateur est membre d'un salon avant
de le laisser lire les messages, puis réponds : combien de lectures facturées
cette règle ajoute-t-elle par requête ? Quelle structure de données te
permettrait de vérifier la même chose **sans** `get()` ?

---

## Niveau 4 — L'émulateur local

**Prérequis :** Node.js et `firebase-tools` installés (section 14 de la leçon).
Ce niveau ne demande pas de savoir programmer, seulement d'utiliser le terminal.

### Exercice 4.1 — Installation et vérification

Installe Node puis `firebase-tools`. Vérifie avec `firebase --version`. Si la
commande n'est pas trouvée, résous le problème de `PATH` toi-même : affiche ton
`PATH`, trouve où `npm` a installé le binaire (`npm root -g` est un bon départ),
et comprends pourquoi le shell ne le voyait pas. Note ta démarche par écrit.

### Exercice 4.2 — Initialiser un projet

Crée un dossier, lance `firebase init`, sélectionne Firestore et Emulators. Puis,
**avant de lancer quoi que ce soit**, ouvre chacun des fichiers générés avec
`cat` et écris en une phrase à quoi sert chacun :

```
firebase.json
.firebaserc
firestore.rules
firestore.indexes.json
```

### Exercice 4.3 — Démarrer et explorer

Lance `firebase emulators:start`. Ouvre `http://localhost:4000`. Sans écrire de
code, en utilisant uniquement l'interface :

1. Crée une collection `utilisateurs` et un document dedans.
2. Ajoute-lui un champ de type tableau et un champ de type objet imbriqué.
3. Crée une sous-collection sous ce document.
4. Crée un deuxième document dans `utilisateurs` avec des champs **complètement
   différents** du premier. Que constates-tu ? Que se serait-il passé en SQL ?

### Exercice 4.4 — Persister les données

Arrête l'émulateur, relance-le. Tes données ont disparu. Trouve dans la leçon
l'option qui permet de les conserver, applique-la, et vérifie que ça marche.
Regarde ensuite avec `ls -la` ce que l'export a créé sur ton disque.

### Exercice 4.5 — Voir une règle refuser

Mets des règles restrictives (par exemple `allow read, write: if false;`), tente
une écriture depuis l'interface de l'émulateur, et trouve l'onglet qui t'explique
**quelle règle** a refusé et **pourquoi**. C'est l'outil que tu utiliseras le plus
souvent quand quelque chose ne marche pas.

---

## Niveau 5 — Requêtes et temps réel

**Prérequis :** ce niveau demande du JavaScript et la compréhension de
l'asynchrone. Lis les énoncés maintenant, fais-les après le cours correspondant.
Travaille toujours contre l'émulateur, jamais contre un vrai projet.

### Exercice 5.1 — Lire, écrire

Écris un petit script qui se connecte à l'émulateur, ajoute trois documents dans
une collection `taches`, puis les relit tous et affiche leur contenu.

### Exercice 5.2 — Filtrer et trier

Ajoute à `taches` un champ `priorite` (nombre) et un champ `faite` (booléen).
Écris une requête qui récupère les tâches non faites, triées par priorité
décroissante, limitées à 10.

Puis lance-la et lis attentivement le message d'erreur qui apparaît. Firestore va
te réclamer un **index composite**. Comprends pourquoi, et crée-le. Explique en
deux phrases pourquoi Firestore exige ça alors que SQL ne le demande pas.

### Exercice 5.3 — Le listener

Mets en place un `onSnapshot` sur la collection `taches` qui affiche la liste à
chaque changement. Laisse le script tourner, et modifie un document depuis
l'interface de l'émulateur. Observe.

Puis, dans le callback, affiche uniquement les **changements** (ajouts,
modifications, suppressions) plutôt que la liste entière. Cherche
`docChanges()`. Pourquoi est-ce important pour les performances d'une interface
mobile ?

### Exercice 5.4 — Compter les lectures

Ajoute un compteur dans ton listener qui affiche le nombre de documents reçus à
chaque déclenchement. Modifie un seul document dans une collection de 50. Combien
de documents te sont renvoyés ? Combien de **lectures** sont facturées ? Cherche
la réponse dans la documentation, elle est moins évidente qu'elle n'en a l'air.

### Exercice 5.5 — Se désabonner

`onSnapshot` retourne une valeur. Trouve laquelle et à quoi elle sert. Écris un
script qui écoute pendant dix secondes puis s'arrête proprement.

Réfléchis ensuite : que se passerait-il dans une application mobile si tu ouvrais
un listener à chaque affichage d'écran sans jamais te désabonner ? Écris ta
réponse en termes de mémoire **et** de facture.

### Exercice 5.6 — La pagination

Charge les tâches par pages de 5. Cherche `startAfter()`. Écris une petite boucle
qui affiche page par page. Quel est le lien entre cet exercice et la section 11.2
de la leçon ?

### Exercice 5.7 — Hors ligne

Écris des données, coupe le Wi-Fi de ton Mac, écris à nouveau, lis. Que
constates-tu ? Rallume le réseau et observe. Note précisément la chronologie de ce
que tu vois.

---

## Niveau 6 — Architecture, coûts, décisions

Ces exercices n'ont pas de bonne réponse unique. Ils s'écrivent en prose, en une
demi-page chacun. Ce sont les plus proches de ce qu'on te demandera réellement en
situation professionnelle.

### Exercice 6.1 — Dénormaliser ou pas

Pour chacun de ces champs, décide s'il faut le dupliquer ou le lire par
référence, et justifie avec le critère de fréquence de la section 13 :

| Donnée | Duplique ? Justifie |
|---|---|
| Le nom de l'auteur sous chaque article | |
| Le solde du compte bancaire d'un client | |
| L'URL de l'avatar dans chaque message d'un chat | |
| Le prix d'un produit dans une commande **passée** | |
| Le prix d'un produit dans un panier **en cours** | |
| Le nombre d'abonnés affiché sur un profil | |

Les deux lignes sur le prix sont un piège volontaire, et la différence entre
elles est le cœur de l'exercice.

### Exercice 6.2 — Estimer une facture

Une application de chat avec 1 000 utilisateurs actifs par jour. Chacun ouvre
l'application 5 fois, et chaque ouverture charge la liste de ses 10 salons puis
les 50 derniers messages du salon consulté.

1. Estime le nombre de lectures par jour, puis par mois.
2. Cherche le prix des lectures Firestore et calcule la facture mensuelle.
3. Un développeur ajoute par erreur un listener qui se réabonne à chaque
   changement d'écran, multipliant les lectures par 20. Recalcule.
4. Propose deux optimisations concrètes qui réduiraient la facture initiale.

### Exercice 6.3 — Firebase ou API maison

Pour chacun de ces projets, choisis et défends ton choix en cinq lignes :

1. Une application de suivi de livraison en direct, 3 mois de développement, une
   personne seule.
2. Un logiciel de gestion pour un cabinet médical français, avec obligation
   d'hébergement de données de santé certifié.
3. Un prototype à présenter à des investisseurs dans six semaines.
4. Une plateforme de trading avec des transactions financières et un historique
   à analyser.
5. Un jeu mobile avec un classement mondial mis à jour en direct et 2 millions de
   joueurs.

### Exercice 6.4 — Limiter le verrouillage

Tu construis l'exercice 2.2 (le chat). Décris l'architecture de code qui te
permettrait de remplacer Firestore par une autre base sans réécrire l'interface.
Nomme les fonctions de ta couche intermédiaire. Combien de fichiers auraient à
changer le jour de la migration ?

### Exercice 6.5 — L'architecture hybride

La leçon évoque un compromis fréquent : Firebase Auth et FCM, mais une API et une
base à soi pour les données. Décris comment ton propre serveur pourrait vérifier
qu'un utilisateur est bien connecté alors que c'est Firebase qui gère les
comptes. Cherche ce qu'est un *jeton d'identité* et comment un serveur le vérifie.

### Exercice 6.6 — Le post-mortem

Une équipe reçoit une facture Firebase de 4 200 € pour un mois, contre 12 € le
mois précédent. Le nombre d'utilisateurs n'a pas changé. Écris la liste des cinq
premières choses que tu irais vérifier, dans l'ordre, et dis pour chacune comment
tu la vérifierais concrètement.

---

## Auto-évaluation

Coche honnêtement. Une case non cochée est une section à relire, pas un échec.

**Le modèle de données**
- [ ] Je sais dire, devant un chemin, s'il désigne une collection ou un document
- [ ] Je sais expliquer à quelqu'un pourquoi Firestore n'a pas de jointures
- [ ] Je sais donner deux structures possibles pour un même besoin et arbitrer
- [ ] Je compte le nombre de lectures d'un écran avant d'écrire du code
- [ ] Je sais expliquer la dénormalisation et son prix à payer
- [ ] Je sais reconnaître un document qui va exploser en taille avec le temps

**La sécurité**
- [ ] Je sais expliquer pourquoi les Security Rules sont l'unique barrière
- [ ] Je repère le mode test et je sais pourquoi il est dangereux
- [ ] Je sais écrire une règle basée sur `request.auth.uid`
- [ ] Je sais faire la différence entre `resource` et `request.resource`
- [ ] Je sais qu'une règle filtre un accès, pas le contenu d'une requête
- [ ] Je pense à valider le type et la taille des champs, pas que l'identité
- [ ] Je sais que Cloud Storage a ses règles propres

**Les services**
- [ ] Je peux citer cinq services Firebase et dire à quoi sert chacun
- [ ] Je sais quand une Cloud Function est nécessaire plutôt que du code client
- [ ] Je sais pourquoi le code sur le téléphone n'est jamais digne de confiance
- [ ] Je comprends le trajet d'une notification push jusqu'à l'écran

**Les outils**
- [ ] J'ai installé `firebase-tools` et je sais résoudre un souci de `PATH`
- [ ] Je sais démarrer l'émulateur et y créer des données à la main
- [ ] Je sais conserver les données de l'émulateur entre deux sessions
- [ ] Je sais où l'émulateur m'explique pourquoi une règle a refusé

**Le recul**
- [ ] Je sais expliquer le verrouillage fournisseur et une parade
- [ ] Je sais comment Firestore facture, et donc comment on fait exploser sa note
- [ ] Je peux défendre le choix de Firebase, et aussi le choix contraire

---

## Où chercher les réponses

**Ce document ne fournit pas de solutions, et c'est un choix assumé.** En NoSQL
il n'existe pas de bonne réponse unique : la structure correcte dépend des écrans
de ton application, de ta volumétrie et de ton budget. Une solution toute faite
t'enseignerait un modèle sans t'enseigner le raisonnement, et c'est le
raisonnement qui vaut quelque chose.

Voici où trouver de quoi vérifier ton travail :

**La documentation officielle** (`firebase.google.com/docs`) est de très bonne
qualité et à jour. Sa section « Modéliser les données » et son guide des Security
Rules répondent directement à la moitié des exercices ci-dessus.

**L'émulateur est ton correcteur.** Pour tout ce qui est règle ou requête, il te
dit oui ou non immédiatement, gratuitement, sans risque. Si tu hésites entre deux
formulations, teste les deux.

**Le simulateur de règles**, dans la console Firebase, te laisse simuler une
requête précise avec un UID donné et voir quelle ligne de règle a tranché.

**La tarification** (`firebase.google.com/pricing`) contient les chiffres exacts
dont tu as besoin pour l'exercice 6.2.

**Le meilleur test pour les niveaux 2 et 6 :** explique ta structure à voix haute
à quelqu'un — ou à un mur — en partant des écrans de l'application. Si tu bloques
en disant « et pour afficher ça, il faudrait que je lise… euh… », tu viens de
trouver le défaut de ton modèle. C'est exactement pour ça que ces exercices se
font par écrit et sans clavier.

**Quand tu bloques vraiment**, ne cherche pas la réponse : cherche la question
plus petite. « Comment modéliser un réseau social » n'a pas de réponse. « Où
ranger la liste des abonnés de Bob » en a plusieurs, et tu peux les comparer.
