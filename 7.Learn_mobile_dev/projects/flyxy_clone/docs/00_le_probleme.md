# 0. Le problème, avant la technique

Comprendre le domaine métier avant d'écrire une ligne de code.

---

## 1. Pourquoi commencer ici

L'erreur la plus commune en début de projet est de commencer par la technique :
« je vais faire du Flutter avec du Go et du Postgres ». Ce sont des réponses. On
n'a pas encore posé la question.

Une architecture n'est jamais bonne dans l'absolu — elle est bonne **pour un
problème donné**. Tant que le problème est flou, tout choix technique est un pari.

Alors posons le problème. En français, sans jargon.

## 2. L'énoncé

> Je suis quelque part dans Paris. Je veux savoir dans combien de temps passent
> les prochains bus, métros, trams et trains **autour de moi**, sans avoir à
> chercher le nom de mon arrêt.

Lis cette phrase lentement. Elle contient déjà toute l'architecture du projet.

## 3. Décortiquer l'énoncé

Reprenons chaque morceau, et voyons ce qu'il impose.

### « Je suis quelque part »

L'entrée du système est une **position GPS** : une latitude et une longitude.
Pas un nom d'arrêt, pas un identifiant. Deux nombres.

Conséquence immédiate : il faut savoir passer de *deux nombres* à *une liste
d'arrêts*. C'est une opération **géographique**, et — retiens ce point — **l'API
temps réel de PRIM ne sait pas la faire**.

### « autour de moi »

« Autour » sous-entend une **distance maximale**. 300 mètres ? 500 ? 1 km ?

Ce n'est pas un détail d'implémentation : c'est une **règle métier**. Si tu mets
5 km, l'utilisateur reçoit 400 arrêts et l'application est inutilisable. Si tu
mets 50 m, il ne voit rien.

« Autour » implique aussi un **tri par distance** : l'arrêt à 80 m avant celui à
400 m. Encore une règle métier.

### « les prochains passages »

Pas *tous* les passages. **Les prochains.** Donc :

- ceux qui sont **dans le futur** (un bus parti il y a deux minutes ne
  m'intéresse plus)
- **triés par heure croissante**
- **en nombre limité** (les 3 ou 4 prochains par ligne, pas les 40 de la journée)

### « bus, métros, trams et trains »

Il existe plusieurs **modes de transport**. L'utilisateur voudra probablement
filtrer — et l'affichage diffère : une ligne de métro a une couleur officielle,
un bus a un numéro.

### « sans chercher le nom de mon arrêt »

C'est la promesse du produit, et donc la contrainte forte : **zéro saisie**.
L'application doit être utile en trois secondes, une main sur le guidon.

Cela a une conséquence technique brutale : **la latence est une fonctionnalité**.
Une application qui met six secondes à répondre a échoué, même si les données
sont justes.

## 4. Le vocabulaire du domaine

Nommer correctement est la moitié du travail d'architecture. Voici les mots que
l'on utilisera **partout** — dans le code Go, dans le code Flutter, dans les
conversations. Un seul concept, un seul mot.

| Mot | Définition | Contre-exemple à éviter |
|---|---|---|
| **Arrêt** (*Stop*) | Un lieu physique où l'on monte : un quai, un abribus. A un nom et des coordonnées. | Ne pas confondre avec la station entière |
| **Ligne** (*Line*) | Le service commercial : le bus 38, la ligne 4. A un nom court et une couleur. | Pas un « trajet » |
| **Mode** | La catégorie : bus, métro, tram, RER, train. | |
| **Destination** | Le terminus affiché sur le véhicule. C'est ce qui distingue les deux sens. | |
| **Passage** (*Departure*) | **Un** véhicule qui va s'arrêter, à une heure donnée, allant vers une destination. | Pas un « horaire » au sens théorique |
| **Attente** | Le temps restant avant un passage. Dérivé, jamais stocké. | |

### Le piège du mot « arrêt »

Dans la vraie vie, « l'arrêt République » désigne un endroit. Dans les données de
transport, c'est une **hiérarchie** :

```
Zone d'arrêt (« République »)          ← ce que dit l'humain
 ├── Quai direction Nation             ← ce que connaît le temps réel
 ├── Quai direction Pont de Levallois
 └── Quai du tram
```

Les horaires temps réel sont attachés aux **quais**, pas à la zone. Mais
l'utilisateur pense en **zones**.

**Cette tension est le cœur technique du projet.** L'architecture devra
regrouper des quais en zones pour l'affichage, tout en interrogeant les quais
pour les données. Garde-la en tête : elle réapparaîtra au document 2.

## 5. Le problème que PRIM ne résout pas

Voici le point qui détermine toute l'architecture du backend.

L'API temps réel de PRIM (`stop-monitoring`) fonctionne ainsi :

> « Donne-moi l'identifiant d'un quai, je te rends ses prochains passages. »

```
Entrée :  STIF:StopPoint:Q:463158:
Sortie :  bus 38 → Porte d'Orléans, dans 2 min
          bus 38 → Porte d'Orléans, dans 9 min
```

Remarque ce qu'elle **ne prend pas** en entrée : **aucune coordonnée GPS**.

Il n'existe pas d'appel « donne-moi les arrêts autour de ce point ». Cette
information vit dans un **jeu de données statique et séparé** — le *référentiel
des arrêts*, un fichier listant tous les arrêts d'Île-de-France avec leurs
coordonnées.

### Le vrai schéma du problème

```
   Position GPS
        │
        ▼
  ┌─────────────────────┐
  │ ÉTAPE 1 — Géo       │   Référentiel statique (fichier)
  │ trouver les arrêts  │   ~ 60 000 arrêts, change rarement
  │ proches             │
  └─────────────────────┘
        │  liste d'identifiants de quais
        ▼
  ┌─────────────────────┐
  │ ÉTAPE 2 — Temps réel│   API PRIM (réseau)
  │ interroger chaque   │   change à la seconde
  │ quai                │
  └─────────────────────┘
        │  passages bruts
        ▼
  ┌─────────────────────┐
  │ ÉTAPE 3 — Métier    │   Notre valeur ajoutée
  │ regrouper, trier,   │   n'appartient à personne d'autre
  │ filtrer, calculer   │
  └─────────────────────┘
        │
        ▼
   Ce que l'utilisateur voit
```

**Trois étapes, trois natures différentes.** C'est cette découpe — et non une
mode ou un framework — qui va dicter la structure du code Go.

### Ce que chaque étape implique

| Étape | Nature | Fréquence de changement | Où ça vit |
|---|---|---|---|
| 1. Géo | Données statiques, calcul | Quelques fois par an | En mémoire du serveur |
| 2. Temps réel | Appels réseau externes | Chaque seconde | Chez PRIM |
| 3. Métier | Règles à nous | Quand le produit évolue | Notre domaine |

Une architecture qui mélange ces trois-là est condamnée : on ne peut pas tester
la règle métier sans réseau, ni changer de fournisseur sans tout réécrire.

## 6. Les contraintes non fonctionnelles

Ce sont les exigences qui ne sont pas des fonctionnalités, mais qui décident de
l'architecture bien plus souvent qu'elles.

**La clé API doit rester secrète.**
Une application mobile est téléchargeable et décompilable. Tout ce qu'elle
contient est public — y compris une clé API « cachée » dans le code. C'est une
raison **suffisante** à elle seule d'avoir un backend.

**Le réseau mobile est mauvais.**
Tunnel, ascenseur, métro. L'application doit se comporter correctement quand
l'appel échoue — et pas seulement afficher une roue qui tourne à l'infini.

**PRIM a des quotas.**
Un appel par arrêt, dix arrêts autour de toi, mille utilisateurs : le compte
grimpe vite. Il faudra du **cache**, et donc prévoir sa place dès maintenant.

**Le temps réel est périssable.**
Une donnée de 30 secondes est bonne. Une de 5 minutes est un mensonge. Toute
mise en cache doit connaître sa durée de vie.

## 7. Ce qu'on ne fait pas

Décider ce qu'on exclut est aussi structurant que décider ce qu'on inclut. Pour
la première version :

- Pas de calcul d'itinéraire (« comment aller de A à B »). C'est un autre métier,
  bien plus difficile.
- Pas de comptes utilisateurs.
- Pas de favoris (viendra plus tard — mais l'architecture doit pouvoir l'accueillir).
- Pas de notifications push.
- Île-de-France uniquement.

## 8. Le résumé en une page

Ce qu'il faut retenir avant de passer à l'architecture :

1. L'entrée est une **coordonnée GPS**, la sortie une **liste de passages
   groupés par arrêt**.
2. PRIM ne sait faire que **l'étape temps réel**. La géographie et le métier
   sont **à nous**.
3. Les données se divisent en **statiques** (référentiel, lentes) et **temps
   réel** (passages, périssables). Elles n'ont ni la même durée de vie, ni le
   même lieu de stockage, ni la même stratégie de cache.
4. Le vocabulaire du domaine est **arrêt / ligne / mode / destination /
   passage**. Ces mots seront les noms des types dans le code.
5. La latence et le secret de la clé API sont des **contraintes
   d'architecture**, pas des détails.

---

**Suite :** [01_architecture_generale.md](01_architecture_generale.md) — où l'on
décide qui fait quoi entre le téléphone et le serveur.
