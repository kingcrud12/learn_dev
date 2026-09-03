# 1. L'architecture générale

Où passe la frontière entre le téléphone et le serveur, et pourquoi.

---

## 1. La question qu'on doit se poser

L'application pourrait, techniquement, appeler PRIM directement. Flutter sait
faire des requêtes HTTP. Pourquoi ajouter un serveur Go au milieu ?

Ce n'est pas une question rhétorique. **Ajouter un composant a un coût** : il
faut l'écrire, l'héberger, le surveiller, le déployer. Un bon architecte ajoute
un serveur seulement s'il peut nommer ce qu'il apporte.

Alors nommons-le.

## 2. Les cinq raisons du backend

### Raison 1 — La clé API (rédhibitoire)

Une application mobile est un fichier téléchargeable. `.apk` ou `.ipa`, on peut
l'ouvrir, la décompiler, lire les chaînes de caractères qu'elle contient.

Une clé API embarquée dans l'application est **publique**. Pas « difficile à
trouver » : publique. L'obscurcir ne change rien — elle doit de toute façon être
en clair au moment de l'envoi de la requête.

Conséquence : ta clé PRIM finit sur GitHub dans un dépôt d'analyse d'APK,
quelqu'un l'utilise, et **ton compte est suspendu**.

Cette raison seule suffit. Les quatre suivantes sont du bonus.

> C'est le même principe que la leçon REST : *« une application mobile est
> téléchargeable et décompilable : rien de ce qu'elle contient n'est secret »*.

### Raison 2 — Le nombre d'appels

Rappelle-toi l'étape 2 du document précédent : PRIM répond **un quai à la fois**.

Dix arrêts autour de toi, chacun avec deux ou trois quais : **25 requêtes** pour
un seul écran.

| Où ça se passe | Conséquence |
|---|---|
| Sur le téléphone | 25 requêtes sur un réseau 4G médiocre, batterie, latence, et un échec partiel à gérer dans l'interface |
| Sur le serveur | 25 requêtes en parallèle sur une fibre, en quelques dizaines de millisecondes, et **une seule** réponse au téléphone |

Le serveur transforme *un problème de réseau mobile* en *un problème de réseau
de datacenter*. Ce n'est pas le même problème.

### Raison 3 — Le référentiel géographique

Trouver les arrêts proches demande le référentiel complet : ~60 000 arrêts.

L'embarquer dans l'application, c'est plusieurs mégaoctets à télécharger, et
surtout **une donnée qui vieillit** : à chaque nouvelle station, il faut publier
une mise à jour sur les stores et attendre que les utilisateurs l'installent.

Sur le serveur, le référentiel se met à jour **sans toucher à l'application**.

### Raison 4 — Le cache partagé

Mille utilisateurs à Châtelet demandent les mêmes horaires dans la même minute.

Un cache sur le téléphone ne sert qu'à *son* utilisateur. Un cache sur le
serveur sert à **tout le monde** : la première requête paie, les 999 suivantes
sont gratuites. C'est un facteur 1000 sur ton quota PRIM.

### Raison 5 — Le point de contrôle

Le serveur te permet de corriger un bug, changer une règle métier ou absorber un
changement d'API de PRIM **sans publier de version mobile**.

Publier sur l'App Store prend des jours de validation. Déployer un serveur prend
des minutes. **Mets la logique volatile là où le déploiement est rapide.**

## 3. La vue d'ensemble

```
┌──────────────────────────┐
│      FLUTTER (mobile)    │
│                          │
│  Affiche, capte le GPS,  │
│  gère l'état de l'écran  │
└────────────┬─────────────┘
             │  1 requête HTTPS
             │  GET /v1/departures?lat=..&lon=..
             ▼
┌──────────────────────────┐
│       GO / GIN (serveur) │
│                          │
│  Géo · Agrégation ·      │
│  Métier · Cache · Secret │
└────┬────────────────┬────┘
     │                │
     ▼                ▼
┌──────────┐   ┌──────────────┐
│Référentiel│   │  API PRIM    │
│ (statique)│   │ (temps réel) │
└──────────┘   └──────────────┘
```

## 4. La règle de répartition

La question « ce code, il va où ? » revient toutes les heures dans un projet.
Voici la règle, en une phrase :

> **Le client affiche. Le serveur décide.**

Plus précisément :

| Responsabilité | Client | Serveur | Pourquoi |
|---|---|---|---|
| Capter la position GPS | ✅ | | Seul le téléphone a le capteur |
| Demander la permission GPS | ✅ | | Concerne l'OS mobile |
| Trouver les arrêts proches | | ✅ | Nécessite le référentiel |
| Appeler PRIM | | ✅ | La clé est secrète |
| Regrouper les quais en arrêts | | ✅ | Règle métier |
| Trier par distance | | ✅ | Règle métier |
| Filtrer les passages passés | | ✅ | Règle métier |
| Calculer « dans 3 min » | | ⚠️ | **Voir ci-dessous** |
| Choisir la couleur d'une ligne | | ✅ | Donnée, pas décision d'affichage |
| Animer, défiler, mettre en page | ✅ | | C'est le métier du client |
| Se souvenir des favoris | ✅ | | Local, pas de compte utilisateur |

### Le cas du « dans 3 minutes »

Une subtilité qui vaut d'être comprise, parce qu'elle revient dans tous les
projets qui manipulent du temps.

Le serveur **ne doit pas** envoyer `"dans 3 min"`. Il doit envoyer l'heure
absolue de passage : `2026-09-03T18:42:00+02:00`.

Pourquoi ? Parce que « dans 3 min » est **faux dès la seconde suivante**. Si la
réponse met 400 ms à arriver et que l'utilisateur regarde l'écran pendant une
minute, l'affichage ment.

Le serveur envoie un **fait** (l'heure). Le client calcule un **affichage**
(l'écart avec maintenant), et peut le rafraîchir chaque seconde sans rappeler le
serveur.

> **Principe général : transmets des faits, pas des rendus.** Une date, pas une
> chaîne formatée. Un code de mode, pas une icône. Un booléen, pas une couleur.
> Le fait survit ; le rendu dépend de l'écran, de la langue et de l'instant.

## 5. Ce qui traverse la frontière

Le contrat sera détaillé au document 5, mais posons le principe maintenant.

**Une requête, une réponse.** Le client envoie sa position, reçoit tout ce qu'il
faut pour peindre l'écran. Pas de « d'abord les arrêts, puis un appel par arrêt » :
ce serait rejouer sur le réseau mobile le problème qu'on a déplacé sur le serveur.

```
GET /v1/departures?lat=48.8566&lon=2.3522&radius=400

→ [
    { arrêt, distance, [ passages... ] },
    { arrêt, distance, [ passages... ] }
  ]
```

**La réponse est déjà dans l'ordre d'affichage.** Le client ne retrie pas : le
serveur a décidé.

## 6. La question du cache, posée tôt

On ne l'implémentera pas tout de suite, mais l'architecture doit lui laisser sa
place — un cache ajouté après coup se plante toujours au mauvais endroit.

Deux données, deux durées de vie radicalement différentes :

| Donnée | Change | Cache | Où |
|---|---|---|---|
| Référentiel des arrêts | Quelques fois par an | Chargé au démarrage, gardé en mémoire | Serveur |
| Passages temps réel | En permanence | **20 à 30 secondes**, pas plus | Serveur |

30 secondes peut sembler court. Fais le calcul : à Châtelet, 200 requêtes par
minute deviennent **2 appels à PRIM**. Le facteur 100 est déjà gagné, sans jamais
mentir à l'utilisateur de plus d'une demi-minute.

**Où le cache se branche-t-il ?** Entre le métier et PRIM — jamais dans le
métier. C'est ce que permet la structure du document suivant : le cache sera un
adaptateur qui en enveloppe un autre, et le domaine ne saura même pas qu'il existe.

## 7. Ce qu'on ne met pas dans l'architecture aujourd'hui

Pour chacun, la raison de l'exclure **et** la porte laissée ouverte :

| Écarté | Pourquoi | La porte ouverte |
|---|---|---|
| Base de données | Rien à persister : le référentiel est un fichier, le temps réel est éphémère | Le domaine parle à des *interfaces* ; brancher Postgres plus tard ne le touchera pas |
| WebSocket | Un rafraîchissement toutes les 30 s suffit largement | Le domaine ne sait pas comment il est appelé ; un adaptateur WebSocket s'ajoutera à côté de HTTP |
| Authentification | Pas de comptes en v1 | Un middleware Gin, ajouté sans toucher au métier |
| Microservices | Un seul développeur, un seul domaine | Un binaire bien découpé se scinde ; un plat de spaghettis, non |

Remarque le motif : **chaque exclusion est sans regret parce que la découpe la
rend réversible.** C'est exactement ce qu'on achète avec l'architecture du
document suivant.

## 8. Résumé

1. Le backend existe pour **cinq raisons nommées**, dont une rédhibitoire : la
   clé API ne peut pas vivre dans l'application.
2. **Le client affiche, le serveur décide.**
3. Le serveur transmet des **faits** (heures absolues), le client produit les
   **rendus** (« dans 3 min »).
4. **Un écran = une requête.**
5. Le cache a sa place réservée entre le métier et PRIM, dès la conception.

---

**Suite :** [02_backend_go_architecture.md](02_backend_go_architecture.md) — la
structure interne du serveur Go.
