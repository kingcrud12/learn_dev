# 6. La feuille de route

Dans quel ordre construire, et pourquoi cet ordre-là.

---

## 1. Le principe d'ordonnancement

Il existe deux façons de construire un logiciel.

**Par couches** : tout le domaine, puis tous les adaptateurs, puis toute
l'interface. Séduisant sur le papier. Le défaut est brutal : **rien ne
fonctionne avant la fin**. On ne découvre les mauvaises hypothèses qu'au moment
de tout brancher, quand il est le plus coûteux de les corriger.

**Par tranches verticales** : une fonctionnalité minuscule qui traverse **toutes**
les couches, de bout en bout. Puis on l'épaissit.

On construit par tranches. La raison :

> **Une chaîne complète qui fait peu vaut mieux qu'une couche parfaite qui ne
> fait rien.** La première te dit si l'architecture tient. La seconde te fait
> croire que tu avances.

## 2. Les étapes

Chaque étape a une **preuve** — une chose observable qui dit qu'elle est finie.
Sans preuve, une étape n'est jamais terminée, juste abandonnée.

### Étape 0 — Le contrat et le squelette

Poser le contrat du document 5 et vérifier que les deux mondes démarrent.

- `api/openapi.yaml` et `api/examples/departures.json`
- Un serveur Go qui répond sur `/health`
- Une application Flutter qui affiche un écran vide
- Les deux dépôts initialisés, `.env.example` en place

> **Preuve :** `curl localhost:8080/health` répond, et l'application se lance sur
> le simulateur.

### Étape 1 — Le domaine Go, seul

Le cœur du document 2, **sans réseau, sans HTTP, sans PRIM**.

- Les types : `Stop`, `Departure`, `Line`, `Position`, `Mode`
- Les fonctions pures : `DistanceMeters`, `KeepUpcoming`, `SortByDistance`
- Les deux interfaces de ports
- `NearbyService`, testé avec deux fausses implémentations

> **Preuve :** `go test ./internal/domain/...` passe **en moins d'une seconde**,
> et `grep -r "net/http\|gin" internal/domain/` ne remonte rien.

C'est le test de la section 4 du document 2. Si cette étape se passe bien, tout
le reste est de la traduction.

### Étape 2 — Le référentiel géographique

Le premier contact avec le monde réel — le plus simple, parce qu'il est local.

- Télécharger le référentiel des arrêts depuis l'open data IDFM
- Le charger en mémoire au démarrage
- Implémenter `StopRegistry` par balayage linéaire
- Traiter la hiérarchie zone/quai du document 0

> **Preuve :** un petit programme en ligne de commande qui, pour des coordonnées
> données, imprime les arrêts proches triés par distance. Pas encore d'horaires.

**Pourquoi le référentiel avant PRIM ?** Parce qu'il est hors ligne,
déterministe et sans quota. On valide la partie géographique dans des conditions
faciles, avant d'y ajouter l'incertitude du réseau.

### Étape 3 — Le client PRIM

La membrane du document 2.

- Appeler `stop-monitoring` pour un quai, avec la clé en en-tête
- Décoder le JSON SIRI dans des types **privés au paquet**
- Traduire en `[]domain.Departure`
- Traduire les erreurs HTTP en erreurs du domaine

> **Preuve :** un test sur une réponse JSON **figée sur disque** produit les
> bons `domain.Departure`. Aucun appel réseau dans les tests.

Garde un vrai échantillon de réponse PRIM dans `testdata/`. C'est ton filet de
sécurité pour toujours.

### Étape 4 — La première chaîne complète

Le moment de vérité : les trois étapes du document 0, bout à bout.

- Brancher registre + PRIM dans `NearbyService` depuis `main.go`
- Le handler Gin, la validation des paramètres, les DTO de sortie
- Les appels aux quais en parallèle, avec limite de concurrence
- La décision « 8 quais sur 10 répondent » : on affiche les 8

> **Preuve :** `curl "localhost:8080/v1/departures?lat=48.85&lon=2.35"` renvoie
> de vrais horaires, dans la forme exacte du document 5.

**C'est l'étape la plus importante du projet.** À partir d'ici, tu as un système
qui marche. Tout le reste l'améliore.

### Étape 5 — Flutter, de bout en bout

- Modèles du domaine, DTO, `Result`
- Les deux dépôts : API et localisation
- `NearbyState` scellé, `NearbyController`
- L'écran, avec **tous** les états traités

> **Preuve :** l'application affiche de vrais horaires autour de ta position
> réelle, sur ton téléphone.

Traite les états d'erreur **dans cette étape**, pas plus tard. Le `switch` du
document 4 t'y oblige — c'est exactement pour ça qu'il est scellé.

### Étape 6 — Le cache

L'ajout qui prouve l'architecture.

- `cache.Wrap` implémentant `ports.RealtimeSource`
- Durée de vie de 30 s, clé par quai
- Une ligne modifiée dans `main.go`

> **Preuve :** deux requêtes identiques en 10 s ne déclenchent **qu'un** appel à
> PRIM — et **aucun fichier du domaine n'a été modifié**.

Le second point de la preuve compte autant que le premier.

### Étape 7 — Le raffinement

Une fois la chaîne solide : rafraîchissement automatique, filtres par mode,
favoris locaux, couleurs officielles, gestion du retour au premier plan.

## 3. Le tableau récapitulatif

| Étape | Livre | Preuve |
|---|---|---|
| 0 | Contrat + squelette | `/health` répond, l'app se lance |
| 1 | Domaine Go pur | Tests verts en < 1 s, sans réseau |
| 2 | Référentiel géo | Une CLI liste les arrêts proches |
| 3 | Client PRIM | Test sur JSON figé |
| 4 | **Chaîne complète** | `curl` renvoie de vrais horaires |
| 5 | Application Flutter | Horaires réels sur le téléphone |
| 6 | Cache | 1 appel PRIM pour 2 requêtes, domaine intact |
| 7 | Raffinement | — |

## 4. Les pièges de parcours

Quatre tentations qui reviendront, et la réponse à chacune.

**« Je vais brancher PRIM tout de suite pour voir. »**
Tu le peux — avec `curl`, pour comprendre la forme des données. Mais n'écris pas
le client avant l'étape 1 : le risque est de construire le domaine *d'après* la
forme de PRIM, et donc de souder les deux pour toujours.

**« Le référentiel a une structure bizarre, je vais l'adapter dans le domaine. »**
Non. C'est le travail de l'adaptateur. Si `domain.Stop` commence à ressembler à
une ligne de CSV, la membrane a fui.

**« Je ferai les erreurs à la fin. »**
Les erreurs ne s'ajoutent pas après coup : elles se voient dans les signatures et
dans le type d'état. Le `sealed class` du document 4 est précisément le mécanisme
qui interdit de remettre ça à plus tard.

**« Je vais optimiser la recherche géographique. »**
Pas avant de l'avoir mesurée. 60 000 comparaisons prennent moins d'une
milliseconde. Et si un jour c'est nécessaire, le port du document 2 fait que
l'optimisation ne touchera rien d'autre. **L'architecture est ce qui te permet de
remettre l'optimisation à plus tard sans le regretter.**

## 5. Ce que ce projet t'apprend vraiment

Les technologies passent. Ce que tu emportes de ce projet :

| Ce que tu retiens | Où tu le reverras |
|---|---|
| Séparer le stable du remplaçable | Dans tout projet, quel que soit le langage |
| Faire pointer les dépendances vers le centre | Idem |
| Traduire aux frontières, ne rien laisser fuiter | Idem |
| Injecter le temps, le réseau, le hasard | Partout où tu veux tester |
| Rendre les états impossibles inexprimables | Toute interface utilisateur |
| Transmettre des faits, pas des rendus | Toute API |
| Construire en tranches verticales | Tout projet, du premier jour |

Le métro parisien n'est qu'un prétexte. **La façon de découper un problème est le
vrai contenu du cours.**

---

**Retour :** [README](../README.md) · **Décisions :** [adr/](adr/)
