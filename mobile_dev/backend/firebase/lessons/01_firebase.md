# Firebase — le backend qu'on n'écrit pas

## 1. Le problème avant la solution

Quand tu écris un programme en C, tout se passe sur ta machine. Tu compiles avec
`clang`, tu lances `./a.out`, et le programme vit et meurt dans ton terminal. Les
données qu'il manipule disparaissent quand il se termine, sauf si tu les écris
dans un fichier.

Une application mobile a un problème que ton `hello world` n'a pas : elle tourne
sur le téléphone de quelqu'un d'autre, et souvent sur les téléphones de milliers
de gens en même temps. Si tu construis une application de messagerie, le message
que Alice envoie doit apparaître sur le téléphone de Bob. Ces deux téléphones ne
se connaissent pas et ne peuvent pas se parler directement. Il faut donc une
machine au milieu, allumée en permanence, que les deux téléphones savent joindre.

Cette machine au milieu, c'est ce qu'on appelle un **serveur**, et le programme
qui tourne dessus, c'est le **backend**. Historiquement, si tu voulais construire
une application, tu devais :

1. Louer une machine quelque part (un serveur chez un hébergeur).
2. Y installer un système, configurer les accès, gérer les permissions de
   fichiers — exactement les notions de `chmod` et de propriétaire que tu as vues
   dans le terminal, mais à l'échelle d'une machine que tu ne touches jamais
   physiquement.
3. Écrire un programme qui écoute les connexions réseau et répond aux demandes.
4. Installer et administrer une base de données.
5. Gérer les sauvegardes, les pannes, les mises à jour de sécurité, la montée en
   charge quand tu as plus d'utilisateurs.

C'est énorme. C'est un métier à part entière. Et pour quelqu'un qui veut juste
faire une application mobile, c'est un mur.

Le **BaaS** — *Backend as a Service*, « le backend en tant que service » — est la
réponse à ce mur. L'idée : quelqu'un d'autre a déjà écrit et administre ce
backend générique. Tu ne loues plus une machine vide, tu loues un backend déjà
fonctionnel. Ton application mobile parle directement à ce service, et tu ne
gères aucun serveur.

```
     SANS BaaS                              AVEC BaaS

  [App mobile]                           [App mobile]
       |                                      |
       | réseau                               | réseau
       v                                      v
  [TON serveur]  <- tu l'écris           [Firebase]  <- déjà écrit
       |          tu l'administres            |         déjà administré
       v          tu le paies                 v
  [TA base de données]                   [Base gérée]
```

**Prérequis :** cette leçon parle beaucoup de « requêtes réseau », de HTTP et de
JSON, que tu ne connais pas encore en profondeur. Je vais te donner juste ce
qu'il faut pour comprendre Firebase, mais le vrai cours sur HTTP et REST est dans
`../../rest_api/lessons/`. Si une phrase te semble floue, ce n'est pas grave à ce
stade : reviens-y après.

---

## 2. Firebase n'est pas un produit, c'est une boîte à outils

Première source de confusion pour les débutants : on dit « j'utilise Firebase »
comme on dirait « j'utilise Word ». C'est faux. Firebase est une **plateforme
Google** qui regroupe une quinzaine de services indépendants. Tu peux en utiliser
un seul, ou cinq, ou aucun.

Voici le panorama de ceux qui comptent pour du développement mobile :

| Service | À quoi ça sert | Fréquence d'usage |
|---|---|---|
| **Firestore** | Base de données, stocke tes données | Très fréquent |
| **Authentication** | Gérer les comptes et les connexions | Très fréquent |
| **Cloud Storage** | Stocker des fichiers (photos, vidéos, PDF) | Fréquent |
| **Cloud Functions** | Exécuter ton code sur les serveurs Google | Fréquent |
| **Cloud Messaging (FCM)** | Envoyer des notifications push | Très fréquent |
| **Realtime Database** | L'ancêtre de Firestore, encore utilisé | Rare, projets anciens |
| **Hosting** | Héberger un site web statique | Occasionnel |
| **Crashlytics** | Recevoir les rapports de plantage de l'app | Fréquent |
| **Remote Config** | Changer des réglages sans republier l'app | Occasionnel |
| **Analytics** | Mesurer l'usage de l'application | Fréquent |

Le cœur, celui que tu dois vraiment comprendre, ce sont les cinq premiers. On les
prend un par un.

---

## 3. Firestore : la base de données sans tables

### 3.1 Ce qu'est une base de données

Une base de données, c'est un programme dont le seul métier est de ranger des
données de façon durable et de te les retrouver vite. En C, si tu voulais stocker
une liste d'utilisateurs, tu écrirais dans un fichier et tu le relirais. Une base
de données fait ça, mais en gérant les accès simultanés de milliers de clients,
les recherches rapides, et la cohérence des données.

### 3.2 Le modèle relationnel, pour comparaison

Le modèle historique, celui du SQL, s'organise en **tables**. Une table est un
tableau rigide : des colonnes fixées à l'avance, des lignes qui remplissent ces
colonnes. Toutes les lignes ont exactement la même forme.

```
Table "utilisateurs"
+----+---------+-------------------+
| id | nom     | email             |
+----+---------+-------------------+
| 1  | Alice   | alice@exemple.fr  |
| 2  | Bob     | bob@exemple.fr    |
+----+---------+-------------------+

Table "messages"
+----+-------------+---------------+
| id | auteur_id   | texte         |
+----+-------------+---------------+
| 1  | 1           | "Salut Bob"   |
+----+-------------+---------------+
```

Pour afficher « Alice a écrit : Salut Bob », la base fait une **jointure** : elle
va chercher dans `messages` la ligne, voit `auteur_id = 1`, retourne dans
`utilisateurs` chercher la ligne 1, et recolle les deux. C'est puissant, et c'est
le socle de la plupart des applications sérieuses depuis cinquante ans.

### 3.3 Le modèle de Firestore : documents et collections

Firestore n'a ni tables, ni colonnes, ni jointures. Il a deux concepts :

Un **document** est un ensemble de paires clé/valeur. C'est très proche de ce que
tu appellerais une `struct` en C, sauf qu'il n'y a aucune déclaration préalable :
chaque document peut avoir les champs qu'il veut. Un document porte un
identifiant unique (une chaîne de caractères).

Une **collection** est un conteneur de documents. Elle porte un nom, et c'est
tout : elle n'impose aucune forme à ce qu'elle contient.

En pratique, un document ressemble à ceci — écrit ici en **JSON**, le format
texte universel pour représenter des données structurées :

```json
{
  "nom": "Alice",
  "email": "alice@exemple.fr",
  "age": 28,
  "actif": true,
  "centres_interet": ["escalade", "cuisine"],
  "adresse": {
    "ville": "Lyon",
    "code_postal": "69003"
  }
}
```

**Prérequis :** le JSON est un format que tu n'as pas encore étudié. Retiens pour
l'instant : les accolades `{}` groupent des paires `"clé": valeur`, les crochets
`[]` font une liste, et une valeur peut elle-même être un objet ou une liste.
C'est tout. Le cours complet arrive dans `../../rest_api/lessons/`.

Note ce qui est impossible en SQL et normal ici : le champ `centres_interet` est
une liste, et `adresse` est un objet imbriqué. Un document n'est pas plat.

### 3.4 L'arborescence

Firestore range collections et documents en alternance, exactement comme le
système de fichiers que tu manipules avec `cd` et `ls` range dossiers et
fichiers :

```
/utilisateurs                    <- collection
   /aBc123XyZ                    <- document (identifiant auto-généré)
       nom: "Alice"
       email: "alice@exemple.fr"
       /messages                 <- sous-collection
           /msg001               <- document
               texte: "Salut"
   /dEf456UvW                    <- autre document
       nom: "Bob"
```

Le chemin `utilisateurs/aBc123XyZ/messages/msg001` désigne un document précis.
C'est un **chemin absolu**, la même notion que `/Users/toi/projet/fichier.c` dans
ton terminal. Et comme dans un système de fichiers, un chemin de longueur impaire
(`utilisateurs`, ou `utilisateurs/aBc/messages`) désigne une collection, un
chemin de longueur paire désigne un document.

### 3.5 Le contraste, résumé

| | SQL / relationnel | Firestore / NoSQL |
|---|---|---|
| Unité de base | Ligne dans une table | Document dans une collection |
| Structure | Schéma fixe, déclaré à l'avance | Libre, chaque document diffère |
| Imbrication | Interdite (valeurs plates) | Naturelle (objets, listes) |
| Relations | Jointures à la lecture | Pas de jointure du tout |
| Données répétées | À éviter (normalisation) | Assumées (dénormalisation) |
| Langage | SQL | Appels de méthodes du SDK |
| Change le schéma | Migration à planifier | Tu écris juste un champ de plus |

---

## 4. Le temps réel : le vrai argument de vente

C'est le point où Firestore se démarque vraiment, et c'est ce qui fait qu'on le
choisit encore aujourd'hui.

Le modèle classique du web, c'est le modèle **question/réponse** : le client
demande, le serveur répond, la connexion se ferme. Si la donnée change ensuite
sur le serveur, le client n'en sait rien. Pour rester à jour, il doit redemander
en boucle — on appelle ça du *polling*, et c'est gaspilleur.

Firestore inverse ça avec les **listeners** (« écouteurs »). Tu ne demandes pas
une donnée une fois : tu déclares que tu veux être *tenu au courant* de cette
donnée. Firestore maintient alors une connexion ouverte et te pousse chaque
changement.

```
    Alice (téléphone)            Firestore            Bob (téléphone)
          |                          |                       |
          |-- écoute /messages ----->|<--- écoute /messages --|
          |                          |                       |
          |-- écrit "Salut Bob" ---->|                       |
          |                          |                       |
          |<-- push (nouveau msg) ---|--- push (nouveau msg)->|
          |                          |                       |
      s'affiche                                          s'affiche
      instantanément                                     instantanément
```

Concrètement, en JavaScript, la différence tient en une ligne :

```js
// Lecture ponctuelle : je demande une fois, j'obtiens une réponse, c'est fini.
const snapshot = await getDocs(collection(db, "messages"));
snapshot.forEach(doc => console.log(doc.data()));

// Écoute continue : la fonction sera rappelée à CHAQUE changement,
// pour toujours, jusqu'à ce que j'arrête d'écouter.
onSnapshot(collection(db, "messages"), (snapshot) => {
  snapshot.forEach(doc => console.log(doc.data()));
});
```

**Prérequis :** le mot-clé `await` et l'idée d'une fonction passée en argument
qui sera « rappelée plus tard » relèvent de la **programmation asynchrone**, que
tu n'as pas encore vue. En C, tes instructions s'exécutent l'une après l'autre,
point. Ici, `onSnapshot` retourne immédiatement et la fonction que tu lui donnes
sera exécutée à des moments imprévisibles dans le futur. C'est un changement de
raisonnement majeur, et il a son propre cours. Pour l'instant, retiens seulement
le principe : **je m'abonne, on me prévient**.

Ce mécanisme rend triviales des fonctionnalités qui sont pénibles à écrire
soi-même : un chat, un compteur de likes qui bouge en direct, un document
collaboratif, la position d'un livreur sur une carte.

---

## 5. Authentication : le service le plus rentable

Si tu ne devais retenir qu'un seul service de Firebase, ce serait celui-là.

Gérer des comptes utilisateurs correctement est beaucoup plus difficile qu'il n'y
paraît. Il faut stocker les mots de passe sans jamais les stocker en clair
(hachage avec sel), gérer la vérification d'email, la réinitialisation de mot de
passe, la limitation des tentatives pour bloquer les attaques par force brute,
les sessions et leur expiration, et intégrer les protocoles de connexion tiers
(Google, Apple) qui ont chacun leur documentation de cent pages. Une erreur dans
n'importe laquelle de ces étapes, et les comptes de tes utilisateurs fuient.

Firebase Authentication fait tout ça. Tu obtiens en quelques lignes :

| Méthode de connexion | Remarque |
|---|---|
| Email + mot de passe | Le classique, avec vérification et reset inclus |
| Google | Un bouton, l'utilisateur ne saisit rien |
| Apple | **Obligatoire** sur iOS si tu proposes un autre login tiers |
| Anonyme | L'utilisateur a un compte sans le savoir |
| Téléphone (SMS) | Coûteux, mais parfois attendu |
| Facebook, GitHub, Microsoft… | Selon ton public |

La connexion **anonyme** mérite un mot, parce qu'elle est sous-estimée. Elle crée
un vrai compte, avec un identifiant stable, sans demander quoi que ce soit à
l'utilisateur. Il peut donc utiliser ton application immédiatement, avec ses
données sauvegardées, et *ensuite* — s'il le souhaite — rattacher un email à ce
compte sans rien perdre. C'est excellent pour l'expérience utilisateur.

Chaque utilisateur connecté reçoit un identifiant unique, l'**UID**, une chaîne
comme `k3Jx9pQr2LmN`. Cet UID est la clé de voûte de toute la sécurité : c'est
lui que tu utilises pour dire « ce document appartient à cet utilisateur ».

---

## 6. Cloud Storage : les fichiers

Firestore stocke des données structurées, pas des fichiers. Un document Firestore
est limité à **1 Mio**, ce qui exclut les photos, les vidéos, les PDF.

Cloud Storage est l'endroit pour ça. Tu y déposes un fichier, il te rend une URL,
et tu ranges cette URL dans un document Firestore. Le schéma habituel :

```
1. L'app envoie photo.jpg vers Cloud Storage
                 |
                 v
2. Storage répond : "https://.../photos/aBc123.jpg"
                 |
                 v
3. L'app écrit dans Firestore :
   { "auteur": "k3Jx9pQr2LmN",
     "photo_url": "https://.../photos/aBc123.jpg" }
```

Cloud Storage a son propre jeu de Security Rules, séparé de celui de Firestore.
C'est une source d'oubli classique : on sécurise sa base et on laisse ses fichiers
ouverts à tous les vents.

---

## 7. Cloud Functions : du code serveur, sans serveur

Il y a des choses que le téléphone ne doit jamais faire. Deux exemples :

Le premier est la confiance. Si ton application accorde des points à
l'utilisateur, et que c'est le téléphone qui écrit `points: 5000` dans la base,
n'importe qui capable de modifier l'application peut écrire `points: 999999`. Le
code qui tourne sur l'appareil de l'utilisateur n'est **jamais** digne de
confiance.

Le second est le secret. Si tu dois appeler un service payant avec une clé
secrète, cette clé ne peut pas se trouver dans l'application : une application
installée est un fichier, et un fichier se lit.

Les **Cloud Functions** sont ta réponse : du code que tu écris (en JavaScript ou
Python), que tu déposes chez Google, et qui s'exécute sur leurs machines. Tu
n'administres toujours aucun serveur — d'où le nom paradoxal de *serverless*, «
sans serveur » : il y a bien un serveur, mais ce n'est pas ton problème.

Une fonction se déclenche sur un **événement** :

```js
// Se déclenche automatiquement à chaque création de document
// dans la collection "commandes".
exports.surNouvelleCommande = onDocumentCreated(
  "commandes/{commandeId}",
  (event) => {
    const commande = event.data.data();
    // Ici : envoyer un email, notifier le vendeur, calculer un total...
  }
);
```

Les déclencheurs courants : création/modification/suppression d'un document,
inscription ou suppression d'un utilisateur, dépôt d'un fichier dans Storage,
appel HTTP direct, ou une **planification** (« tous les jours à 3 h »).

**Attention :** les Cloud Functions sont le seul service Firebase qui exige un
plan payant (Blaze), même pour un usage minuscule. C'est un point de bascule
budgétaire à connaître avant de bâtir ton architecture dessus.

---

## 8. Cloud Messaging : les notifications push

En mobile, une notification push est ce qui fait revenir l'utilisateur. C'est
aussi techniquement pénible : Apple et Google ont chacun leur infrastructure
(APNs pour iOS, un service Google pour Android), avec leurs certificats et leurs
formats.

**FCM** (Firebase Cloud Messaging) est une couche unique par-dessus les deux. Tu
envoies un message à FCM, FCM se débrouille avec Apple et Google.

Le mécanisme :

```
[Ton app sur le téléphone]
   -- demande un jeton --> [FCM] --> renvoie "eXy7...token"
   -- range le jeton dans Firestore -->  { uid: "k3J...", fcm_token: "eXy7..." }

Plus tard, un événement se produit (nouveau message) :

[Cloud Function] --> lit le jeton dans Firestore
                 --> demande à FCM d'envoyer
                 --> [FCM] --> [APNs / Android] --> notification affichée
```

Deux subtilités qui piègent tout le monde : un jeton **change** (réinstallation,
mise à jour, nettoyage par le système), il faut donc le rafraîchir et supprimer
les jetons morts ; et l'utilisateur doit **autoriser** les notifications, ce qui
sur iOS est un refus fréquent.

---

## 9. Les Security Rules : le point critique

C'est la section la plus importante de cette leçon. Lis-la deux fois.

### 9.1 Pourquoi c'est vital

Dans une architecture classique, l'application mobile parle à *ton* serveur, et
seul ton serveur parle à la base de données. Ton serveur est un garde : il vérifie
chaque demande avant de toucher aux données.

Avec Firebase, il n'y a pas de garde. Le téléphone parle **directement** à la
base de données. N'importe qui peut extraire de ton application les informations
de connexion à ton projet Firebase — elles sont censées être publiques — et
écrire son propre programme pour parler à ta base.

La seule chose qui l'en empêche, ce sont les **Security Rules**. Elles ne sont pas
une option de confort. Elles sont ton unique mur porteur.

```
        ARCHITECTURE CLASSIQUE            FIREBASE

           [App]                           [App]  [Curieux malveillant]
             |                                \        /
        [TON serveur]  <- vérifie tout         \      /
             |                              [SECURITY RULES] <- vérifie tout
        [Base de données]                        |
                                            [Firestore]
```

### 9.2 Le piège du mode test

Quand tu crées une base Firestore, la console te propose le **mode test**. Il
génère ceci :

```js
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /{document=**} {
      // DANGER : n'importe qui sur Internet peut TOUT lire
      // et TOUT écrire ou effacer, jusqu'à la date indiquée.
      allow read, write: if request.time < timestamp.date(2026, 10, 3);
    }
  }
}
```

Le `{document=**}` signifie « tout document, à n'importe quelle profondeur ». La
condition ne teste rien d'autre qu'une date. Ces règles sont acceptables les
premières heures d'un projet jouet, et elles sont une catastrophe partout
ailleurs. Le scénario classique : la date approche, Firebase envoie un email
d'alerte, le développeur pressé remplace la date par une plus lointaine, et la
base reste ouverte pendant des années. Des fuites de données massives ont eu
exactement cette cause.

### 9.3 Des règles correctes, commentées

```js
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {

    // --- Profils utilisateurs ---
    // Le chemin /utilisateurs/{uid} capture l'identifiant du document
    // dans la variable uid.
    match /utilisateurs/{uid} {

      // Tout utilisateur CONNECTÉ peut lire un profil.
      // request.auth vaut null si personne n'est connecté.
      allow read: if request.auth != null;

      // Mais on ne peut écrire QUE dans son propre profil :
      // l'UID de celui qui demande doit être celui du document.
      allow write: if request.auth != null
                   && request.auth.uid == uid;
    }

    // --- Messages d'un salon ---
    match /salons/{salonId}/messages/{messageId} {

      allow read: if request.auth != null;

      // À la création, on impose que l'auteur déclaré soit bien
      // l'utilisateur connecté : impossible d'écrire sous un faux nom.
      // request.resource.data = le document tel qu'il serait après écriture.
      allow create: if request.auth != null
                    && request.resource.data.auteur == request.auth.uid
                    && request.resource.data.texte is string
                    && request.resource.data.texte.size() > 0
                    && request.resource.data.texte.size() < 2000;

      // Modifier ou supprimer : réservé à l'auteur d'origine.
      // resource.data = le document tel qu'il est ACTUELLEMENT en base.
      allow update, delete: if request.auth != null
                            && resource.data.auteur == request.auth.uid;
    }

    // --- Tout le reste est refusé ---
    // Ce n'est même pas nécessaire de l'écrire : ce qui n'est pas
    // explicitement autorisé est interdit. Mais l'écrire est plus clair.
    match /{document=**} {
      allow read, write: if false;
    }
  }
}
```

Quatre principes à retenir :

Le **refus par défaut** est la règle : tout ce que tu n'autorises pas est
interdit. Les règles ne sont pas des filtres qui se cumulent en s'affinant ; il
suffit qu'une règle autorise pour que l'accès passe.

Les règles ne sont **pas un filtre de requête**. Si tu écris une règle qui
n'autorise la lecture que des documents dont `public == true`, une requête
demandant *toute* la collection sera refusée en bloc — pas filtrée. Ta requête
doit elle-même demander uniquement ce que les règles autorisent.

**Valide les données**, pas seulement l'identité. Sans contrôle de type et de
taille, un utilisateur légitime peut écrire un champ `texte` de 900 Kio, ou
remplacer un `age` numérique par un objet.

Les règles **ne sont pas des conditions bon marché**. Une règle qui va lire un
autre document (avec `get()`) déclenche une vraie lecture facturée. On limite ces
lectures croisées.

---

## 10. Le mode hors-ligne

C'est un point que les gens venant du web sous-estiment, et qui est décisif en
mobile. Un téléphone perd le réseau : métro, ascenseur, campagne, avion.

Firestore garde une **copie locale** des données que tu as consultées, dans une
base sur l'appareil. Les conséquences sont excellentes :

Une lecture de données déjà connues est servie depuis le cache, instantanément,
sans réseau et **sans coût**. Une écriture faite hors réseau est appliquée
immédiatement au cache — ton interface se met à jour tout de suite — et mise en
file d'attente. Au retour du réseau, la file est envoyée automatiquement.

```
Hors ligne :
  App écrit "Salut" --> cache local (OK, affiché) --> [file d'attente]

Retour du réseau :
  [file d'attente] --> Firestore --> les autres clients reçoivent le push
```

Le comportement par défaut diffère selon la plateforme : le cache est **actif par
défaut** sur mobile (Android, iOS), et doit être **activé explicitement** sur le
web.

La limite honnête : ce n'est pas de la magie. Si deux utilisateurs modifient la
même donnée chacun de leur côté hors ligne, le dernier arrivé écrase l'autre. Il
n'y a pas de résolution de conflit intelligente. Pour les compteurs, on utilise
des opérations spéciales (`increment`) qui, elles, s'additionnent correctement.

---

## 11. Les questions honnêtes : verrouillage et coûts

Toute leçon sur Firebase qui ne parle pas de ces deux points te ment par omission.

### 11.1 Le verrouillage fournisseur

Le *vendor lock-in*, c'est le fait que ton code devienne inséparable d'un
fournisseur. Firebase en est un cas d'école : il n'existe aucun standard
Firestore. Ses requêtes, ses listeners, ses règles de sécurité sont propriétaires.

Si dans trois ans tu veux partir — parce que les prix ont changé, parce qu'une
fonctionnalité manque, parce qu'un client exige que les données restent en Europe
sur une infrastructure choisie — tu ne « changes pas de base ». Tu réécris ta
couche de données, tu réécris tes règles en logique serveur, tu migres les
données, et tu construis un backend que tu n'avais jamais eu.

Il existe une parade partielle, et elle vaut d'être connue dès maintenant :
n'appelle jamais Firebase directement depuis ton interface. Écris une couche
intermédiaire à toi (souvent appelée *repository*) qui expose des fonctions
métier (`getMessages()`, `envoyerMessage()`), et qui seule connaît Firebase. Le
jour du départ, tu ne réécris que cette couche.

### 11.2 Les coûts

Le plan gratuit (Spark) est généreux et suffit largement à apprendre. Le plan
payant (Blaze) facture à l'usage, et c'est là que ça se corse : **Firestore
facture à la lecture de document, pas au volume de données**.

Concrètement : afficher une liste de 100 messages, c'est 100 lectures. Si un
listener est mal écrit et se réabonne à chaque affichage d'écran, tu multiplies.
Si un écran actualise une liste de 500 éléments toutes les secondes, tu es à
1,8 million de lectures à l'heure, par utilisateur.

Les factures accidentelles à quatre chiffres sont un phénomène connu, et elles
viennent presque toujours d'une boucle ou d'un listener oublié, jamais d'une
croissance légitime. Deux réflexes : mets une **alerte de budget** dans la console
Google Cloud dès que tu passes en Blaze, et **pagine** systématiquement tes
requêtes avec `limit()`.

---

## 12. Firebase ou une API maison ?

| Critère | Firebase | API REST maison |
|---|---|---|
| Temps avant la première version | Quelques jours | Quelques semaines |
| Compétences requises | Le mobile seul | Mobile + serveur + base + ops |
| Temps réel | Inclus, gratuit à écrire | À construire (WebSocket) |
| Requêtes complexes, rapports | Faible, très limité | Excellent (SQL) |
| Coût à petite échelle | Gratuit ou quelques euros | Un petit serveur, ~5 €/mois |
| Coût à très grande échelle | Peut exploser | Prévisible, maîtrisable |
| Maîtrise de l'hébergement | Nulle | Totale |
| Portabilité | Faible (lock-in) | Élevée |
| Charge d'administration | Nulle | Réelle et permanente |

**Choisis Firebase** pour un prototype, un projet personnel, une application dont
le cœur est le temps réel (chat, collaboration, suivi en direct), une petite
équipe sans développeur backend, ou une startup qui doit valider une idée vite.

**Écris ton API** si tu as des requêtes analytiques complexes, des transactions
financières, des contraintes réglementaires sur la localisation des données, une
volumétrie très importante et prévisible, ou une équipe backend déjà en place.

**Le compromis fréquent, et souvent le meilleur** : Firebase Authentication (qui
est excellent et peu verrouillant) et FCM, avec ta propre API et ta propre base
pour les données métier. On prend le meilleur des deux.

---

## 13. Modéliser en NoSQL : la dénormalisation

C'est le sujet qui déroute le plus, et celui où les débutants se plantent le plus
souvent. **En NoSQL, tu ne modélises pas selon la nature de tes données, tu
modélises selon les écrans de ton application.**

En SQL, on range proprement chaque information à un seul endroit
(*normalisation*), et on recolle avec des jointures au moment de lire. Comme
Firestore n'a pas de jointures, cette approche impose de faire N requêtes en
cascade — lent et cher.

Exemple. Une liste d'articles de blog affichant, pour chaque article, son titre et
le nom de son auteur.

Approche normalisée, réflexe SQL :

```json
// /articles/art001
{ "titre": "Débuter en C", "auteur_id": "u42" }
// /utilisateurs/u42
{ "nom": "Alice", "email": "alice@exemple.fr" }
```

Pour afficher 20 articles : 1 requête pour les articles, puis 20 requêtes pour
les auteurs. Soit 21 lectures facturées, et 21 allers-retours réseau.

Approche dénormalisée, réflexe Firestore :

```json
// /articles/art001
{
  "titre": "Débuter en C",
  "auteur_id": "u42",
  "auteur_nom": "Alice"     // <- DUPLIQUÉ, volontairement
}
```

Pour afficher 20 articles : 1 requête, 20 lectures, un seul aller-retour. L'écran
s'affiche d'un coup.

Le prix à payer est réel et il faut l'accepter en conscience : si Alice change de
nom, tu dois mettre à jour tous ses articles. C'est typiquement le travail d'une
Cloud Function déclenchée sur la modification du profil.

La règle de décision est un arbitrage de fréquence :

```
La donnée est-elle lue beaucoup plus souvent qu'elle n'est modifiée ?
      |
   OUI --> duplique-la (dénormalise)
      |
   NON --> garde une référence (auteur_id) et lis en deux temps
```

Un nom d'auteur : lu des milliers de fois, changé une fois par an. On duplique.
Le solde d'un compte : lu et modifié en permanence, et une valeur périmée serait
un bug grave. On ne duplique jamais.

Le deuxième réflexe à acquérir : **compte tes lectures dès la conception**. Pour
chaque écran de ton application, demande-toi combien de documents il faut lire
pour l'afficher. Si la réponse dépasse quelques dizaines, ton modèle est mauvais,
et il faut le changer *avant* d'écrire la moindre ligne de code.

---

## 14. Installation sur macOS

Firebase s'utilise à deux endroits : la **console web** (l'interface graphique,
sur `console.firebase.google.com`) et la **ligne de commande**, que tu connais
déjà.

L'outil en ligne de commande s'installe avec `npm`, le gestionnaire de paquets de
Node.js. Sur un Mac Apple Silicon, le plus simple passe par Homebrew :

```sh
# 1. Node.js (fournit npm), via Homebrew
brew install node
node --version    # doit répondre quelque chose comme v22.x

# 2. L'outil Firebase, installé globalement (-g)
npm install -g firebase-tools

# 3. Vérifier
firebase --version
```

Si `firebase` répond `command not found`, c'est un problème de `PATH` — la liste
des dossiers où le shell cherche les exécutables. Vérifie avec `echo $PATH` que
`/opt/homebrew/bin` y figure : sur Apple Silicon, Homebrew installe là, et non
dans `/usr/local` comme sur les Mac Intel. C'est exactement la même mécanique que
lorsque tu dois taper `./a.out` et non `a.out` pour lancer ton programme C : le
dossier courant n'est pas dans le `PATH`.

Ensuite, la connexion et l'initialisation d'un projet :

```sh
firebase login          # ouvre le navigateur pour t'authentifier
mkdir mon-projet && cd mon-projet
firebase init           # menu interactif : coche Firestore, Emulators, ...
```

### L'émulateur local : à utiliser dès le premier jour

`firebase init` te propose d'installer la **suite d'émulateurs**. Accepte
toujours. C'est une copie de Firestore, Auth, Storage et Functions qui tourne
**sur ta machine**.

```sh
firebase emulators:start
```

Tu obtiens une interface sur `http://localhost:4000` où tu vois tes données,
tes utilisateurs, et — surtout — un onglet qui t'explique *pourquoi* une règle de
sécurité a refusé une requête.

Trois raisons de l'utiliser systématiquement : c'est **gratuit** (aucune lecture
facturée), c'est **rapide** (pas de réseau), et tu peux **tout casser** sans
conséquence. Tester ses Security Rules sur la base de production est une très
mauvaise idée ; l'émulateur est fait pour ça.

Note que les données de l'émulateur disparaissent à l'arrêt, sauf si tu demandes
explicitement de les conserver :

```sh
firebase emulators:start --import=./donnees --export-on-exit=./donnees
```

---

## Mémo

| Notion | L'essentiel |
|---|---|
| **BaaS** | Un backend déjà écrit et administré qu'on loue au lieu de construire |
| **Firebase** | Une plateforme Google réunissant ~15 services indépendants |
| **Firestore** | Base NoSQL : des **documents** rangés dans des **collections** |
| **Document** | Paires clé/valeur, forme libre, imbrication possible, max 1 Mio |
| **Chemin** | Impair = collection, pair = document, comme un chemin de fichiers |
| **Listener** | `onSnapshot` : je m'abonne, Firestore me pousse les changements |
| **Auth** | Comptes gérés pour toi ; chaque utilisateur a un **UID** |
| **UID** | La clé de toute la sécurité : « ce document est à cet utilisateur » |
| **Storage** | Les fichiers (photos, vidéos) ; ses règles sont **séparées** |
| **Functions** | Ton code chez Google, déclenché par événement ; exige le plan Blaze |
| **FCM** | Notifications push, une couche unique pour iOS et Android |
| **Security Rules** | Le **seul** garde entre Internet et tes données. Jamais optionnelles |
| **Refus par défaut** | Ce qui n'est pas explicitement autorisé est interdit |
| **Mode test** | Base ouverte à tous jusqu'à une date. Piège majeur |
| **Hors ligne** | Cache local actif par défaut sur mobile ; écritures mises en file |
| **Dénormalisation** | Dupliquer volontairement pour éviter les lectures en cascade |
| **Modéliser** | Selon les **écrans** de l'app, pas selon la nature des données |
| **Facturation** | Par **document lu**, pas par volume. Toujours `limit()` |
| **Lock-in** | Fort. Parade : isoler Firebase derrière une couche à toi |
| **Émulateur** | `firebase emulators:start` — gratuit, rapide, cassable |

**À faire ensuite :** les exercices dans
[../exercices/firebase.md](../exercices/firebase.md). Commence par le niveau 1 et
ne saute pas le niveau 2 : la modélisation des données est là où tout se joue.
