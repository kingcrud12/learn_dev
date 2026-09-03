# Exercices — API REST

Support : [../lessons/01_rest_api.md](../lessons/01_rest_api.md)

Vingt-deux exercices, du plus simple au plus exigeant.

> **Lesquels peux-tu faire aujourd'hui ?**
>
> Les niveaux **1, 2, 3 et 6** ne demandent que ton terminal et ta tête. `curl` est
> déjà installé sur ton Mac : tu peux les faire tout de suite.
>
> Les niveaux **4 et 5** supposent un langage de haut niveau que tu n'as pas encore
> vu. Lis-les, réponds aux questions de conception, garde le code pour plus tard.

**Un carnet.** Beaucoup d'exercices demandent une réponse écrite plutôt que du code.
Crée `mobile_dev/backend/rest_api/exercices/reponses.md` et écris-y tes réponses —
les relire dans un mois sera instructif.

---

# Niveau 1 — Lire une API

Tout est faisable dans ton terminal, maintenant.

## 1.1 — Ta première requête

```sh
curl https://api.github.com/users/torvalds
```

Tu obtiens du JSON. Repère : le nom, le nombre de dépôts publics, la date de
création du compte.

Combien de champs l'objet contient-il, environ ? Combien en utiliserais-tu si tu
affichais une fiche de profil ?

*C'est le problème de l'*over-fetching* de la section 12 — vu de tes yeux.*

## 1.2 — Voir les en-têtes

```sh
curl -i https://api.github.com/users/torvalds
```

Le `-i` ajoute les en-têtes. Identifie : le code de statut, le `Content-Type`, et
l'en-tête qui indique combien de requêtes il te reste avant d'être bloqué.

## 1.3 — Toute la conversation

```sh
curl -v https://api.github.com/users/torvalds
```

Les lignes commençant par `>` sont ta requête, celles par `<` la réponse.

Recopie dans ton carnet la ligne de requête complète et les trois premiers en-têtes
envoyés. Quel verbe HTTP `curl` utilise-t-il par défaut ?

## 1.4 — Le code de statut seul

```sh
curl -s -o /dev/null -w "%{http_code}\n" https://api.github.com/users/torvalds
```

Décompose cette commande : que fait `-s` ? `-o /dev/null` ? `-w` ?

*Indice : `man curl`, puis `/` pour chercher chaque option.*

Quel rapport avec le `echo $?` que tu utilises après `gcc` ?

## 1.5 — Provoquer des erreurs

```sh
curl -i https://httpbin.org/status/404
curl -i https://httpbin.org/status/401
curl -i https://httpbin.org/status/500
```

Pour chacun : le client ou le serveur est-il en tort ? Faut-il réessayer ?

## 1.6 — Une ressource inexistante

```sh
curl -i https://api.github.com/users/ce_compte_nexiste_absolument_pas_12345
```

Quel code ? Le corps de la réponse est-il utile ? Compare avec la qualité d'erreur
recommandée en section 14.

---

# Niveau 2 — Concevoir des URL

Sur papier ou dans ton carnet. Aucun code.

## 2.1 — Corriger des URL

Ces URL violent les conventions REST. Réécris-les correctement, avec le bon verbe :

```
GET  /getUser?id=7
POST /createNewArticle
GET  /deleteArticle/42
POST /article/42/update
GET  /listAllCommentsForArticle?articleId=42
```

## 2.2 — Une bibliothèque

Conçois les URL et verbes pour une application de bibliothèque qui doit permettre de :

- lister tous les livres
- consulter un livre précis
- ajouter un livre
- modifier le titre d'un livre
- supprimer un livre
- lister les emprunts d'un adhérent
- emprunter un livre

**Le dernier est piégeux.** Emprunter est une action, pas une ressource. Trouve au
moins deux façons de le modéliser et choisis, en justifiant.

## 2.3 — Les sous-ressources

Écris l'URL pour :

- les commentaires de l'article 42
- le commentaire 7 de l'article 42
- les articles écrits par l'utilisateur 3
- la photo de profil de l'utilisateur 3

## 2.4 — PUT ou PATCH

Pour chaque cas, lequel choisis-tu et pourquoi ?

1. L'utilisateur change uniquement son adresse email
2. Un formulaire d'édition renvoie l'article entier
3. On marque un article comme publié
4. On remplace complètement une fiche produit importée d'un fournisseur

## 2.5 — Filtres et pagination

Écris les URL pour :

- la page 3, par 25 éléments
- les articles publiés uniquement
- les articles de l'auteur 7, triés par date décroissante
- les trois critères combinés

---

# Niveau 3 — Envoyer des données

Retour au terminal.

## 3.1 — Un POST

```sh
curl -X POST https://httpbin.org/post \
     -H "Content-Type: application/json" \
     -d '{"titre": "Mon article", "publie": false}'
```

httpbin renvoie ce qu'il a reçu. Retrouve dans la réponse : ton JSON, l'en-tête
`Content-Type`, la méthode utilisée.

## 3.2 — Sans le Content-Type

Relance la même commande **sans** l'en-tête `-H`.

Que devient ton JSON dans la réponse ? Sous quelle clé apparaît-il ? Pourquoi
l'en-tête compte-t-il ?

## 3.3 — Les autres verbes

Reproduis un `PUT`, un `PATCH` et un `DELETE` sur `https://httpbin.org/`.

*Indice : les chemins suivent le nom du verbe.*

## 3.4 — Un en-tête d'authentification

```sh
curl -H "Authorization: Bearer mon_faux_jeton_123" https://httpbin.org/headers
```

Retrouve ton jeton dans la réponse. Que se passerait-il si cette requête partait
en `http://` au lieu de `https://` ?

## 3.5 — Le corps d'un GET

Essaie d'envoyer des données dans un `GET` :

```sh
curl -X GET https://httpbin.org/get -d '{"a": 1}'
```

Observe attentivement la méthode que rapporte httpbin. Que s'est-il passé ?
Pourquoi un `GET` ne devrait-il pas avoir de corps ?

---

# Niveau 4 — Consommer une API dans du code

> **Ces exercices ne sont pas encore praticables.** Ils supposent un langage de
> haut niveau et la programmation asynchrone. Lis-les, réponds aux questions de
> conception, reviens y coder plus tard.

## 4.1 — Anticiper les erreurs

Une application mobile appelle `GET /articles/42`. Liste **tous** les cas d'échec
possibles — au moins six — et ce que l'interface devrait afficher pour chacun.

Pense au réseau, pas seulement au serveur.

## 4.2 — Les trois états

Tout appel réseau met l'interface dans un de trois états. Nomme-les, et décris ce
que l'utilisateur voit dans chacun.

Que se passe-t-il si on oublie le premier ?

## 4.3 — Réessayer, ou pas

Le réseau coupe pendant une requête. Pour chacune, peut-on réessayer sans risque ?

```
GET    /articles/42
DELETE /articles/42
POST   /commandes          (une commande à 400 €)
PUT    /utilisateurs/7
POST   /articles/42/vues   (incrémente un compteur)
```

Relis la propriété d'idempotence en section 5. Pour les cas problématiques,
propose une solution.

## 4.4 — Le jeton expire

Le jeton d'authentification dure une heure. L'utilisateur garde l'application
ouverte toute la journée.

Décris ce qui se passe à la première requête après expiration, et le mécanisme qui
évite de le déconnecter.

## 4.5 — Défilement infini

Une liste charge 20 articles par page. Décris précisément :

- quand déclencher le chargement de la page suivante
- comment éviter de charger deux fois la même page
- quoi faire quand il n'y a plus rien
- quoi faire si le chargement échoue au milieu

---

# Niveau 5 — Construire une API

> **Pas encore praticable.** Conception uniquement.

## 5.1 — Une application de tâches

Conçois l'API complète d'une application de tâches partagées : listes, tâches,
utilisateurs, partage.

Livre : les ressources, les URL, les verbes, les codes de retour attendus, et le
JSON d'une tâche.

## 5.2 — Les codes de retour

Pour chaque situation, quel code renvoies-tu ?

1. Création réussie
2. Suppression réussie, rien à renvoyer
3. Le champ `email` est absent
4. Aucun jeton fourni
5. Jeton valide, mais l'article appartient à quelqu'un d'autre
6. L'article n'existe pas
7. L'email est déjà utilisé
8. La base de données est injoignable
9. Le client a fait 1000 requêtes en une minute

## 5.3 — Un message d'erreur utile

Ce message est inutilisable :

```json
{"error": "Erreur"}
```

Récris-le pour une validation qui a échoué sur deux champs. Il doit permettre à
l'application d'afficher un message **sous le bon champ** du formulaire.

## 5.4 — Versionner

Tu dois renommer le champ `nom` en `nom_complet` dans une API dont trois versions
d'application mobile sont déjà déployées.

Pourquoi ne peux-tu pas simplement changer le nom ? Que fais-tu ?

*C'est un problème spécifiquement mobile : réfléchis à ce qui se passe quand un
utilisateur refuse la mise à jour.*

## 5.5 — Un écran, combien d'appels

Un écran d'accueil affiche : le profil, les 10 derniers articles, le nombre de
notifications, la liste des amis en ligne.

Combien d'appels REST « purs » ? Quel problème sur un réseau mobile lent ?
Propose deux solutions.

---

# Niveau 6 — Décider

De la réflexion, faisable maintenant.

## 6.1 — Le bon outil

Pour chaque fonctionnalité : REST, WebSocket, gRPC ou Firebase ? Justifie en deux
phrases.

1. Afficher un catalogue de produits
2. Un chat en direct
3. Une position GPS partagée en continu
4. Un formulaire d'inscription
5. Un tableau de bord de cours de bourse
6. Un prototype à livrer en deux semaines, seul
7. Deux microservices internes qui s'échangent beaucoup de données

## 6.2 — Stateless

REST est *stateless* : le serveur ne se souvient de rien entre deux requêtes.

Quel est l'inconvénient évident ? Pourquoi ce choix rend-il possible de répartir la
charge sur mille serveurs ? Que se passerait-il si le serveur gardait l'état en
mémoire et qu'un équilibreur de charge envoyait la requête suivante ailleurs ?

## 6.3 — 401 ou 403

Explique la différence en une phrase, puis classe :

1. Aucun jeton envoyé
2. Jeton expiré
3. Jeton valide, ressource appartenant à un autre
4. Jeton valide, mais compte suspendu
5. Jeton malformé

## 6.4 — Sécurité mobile

Trois affirmations. Vraies ou fausses, et pourquoi ?

1. « La clé d'API est dans le code de l'app, donc invisible. »
2. « On utilise HTTPS, donc le jeton peut être stocké n'importe où. »
3. « Le serveur vérifie déjà les droits, l'app n'a pas besoin de contrôler. »

## 6.5 — Critiquer une vraie API

Reprends `https://api.github.com`. Explore trois ou quatre endpoints avec `curl`.

Dans ton carnet : trois choses bien conçues, et deux qui te semblent discutables.
Justifie avec les principes de la section 14.

## 6.6 — La rétrospective

Sans relire la leçon, écris de mémoire :

- les cinq verbes et leur intention
- les quatre familles de codes de statut
- ce que veut dire *idempotent*, avec un exemple
- pourquoi l'action ne doit pas être dans l'URL

Puis compare avec la leçon et note ce qui manquait.

---

# Auto-évaluation

Tu maîtrises la leçon si tu réponds sans hésiter :

- [ ] Pourquoi `/getArticle?id=42` est-il moins bon que `GET /articles/42` ?
- [ ] Quelle est la différence entre `PUT` et `PATCH` ?
- [ ] Que signifie *idempotent*, et pourquoi est-ce crucial en mobile ?
- [ ] Différence entre `401` et `403` ?
- [ ] Que veut dire *stateless*, et quel est l'avantage ?
- [ ] Pourquoi ne jamais mettre de clé secrète dans une application mobile ?
- [ ] Quand REST ne suffit-il pas ?
- [ ] En quoi une API ressemble-t-elle à un header C ?

---

# Solutions

Non fournies — comme partout dans ce projet.

Quatre recours :

1. **Relire la leçon.** Chaque exercice correspond à une section.
2. **Essayer avec `curl`.** Pour tout le niveau 1 et 3, la réponse est dans le
   terminal.
3. **`man curl`**, section EXAMPLES.
4. **Lire de vraies API** : la documentation de GitHub ou de Stripe est un modèle
   de conception.

Un exercice de conception n'a pas *une* bonne réponse. Ce qui compte est de savoir
**justifier** ton choix — c'est exactement ce qu'on te demandera en entretien.
