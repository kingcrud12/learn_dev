# Les API REST

Comment une application mobile parle à un serveur.

> **Prérequis.** Cette leçon est en avance sur ton parcours : elle suppose des
> notions de HTTP, de JSON et d'un langage de haut niveau que tu n'as pas encore
> vues. Elle est écrite pour être **lue et comprise** maintenant, et **pratiquée**
> plus tard. Les sections 1 à 6 sont accessibles immédiatement ; à partir de la 7,
> les exemples de code sont là pour illustrer, pas pour être tapés aujourd'hui.
>
> Les commandes `curl` de la section 9, en revanche, tu peux les lancer tout de
> suite dans ton terminal — c'est le meilleur moyen de rendre tout ceci concret.

---

## 1. Le problème

Une application mobile ne vit pas seule. Elle affiche des messages écrits par
d'autres, un solde bancaire, une carte, une liste de produits. Toutes ces données
sont ailleurs — sur un **serveur**.

Se pose alors une question qui n'a rien d'évident : **comment deux programmes
écrits par des gens différents, dans des langages différents, tournant sur des
machines différentes, se mettent-ils d'accord pour échanger des données ?**

Ton application est peut-être en Swift sur un iPhone. Le serveur en Python sur
une machine Linux à Francfort. Ils ne partagent aucun code, aucune mémoire, aucun
type de données. Le seul lien entre eux est un câble et une convention.

**Cette convention, c'est une API.** REST est la façon la plus répandue de la
concevoir.

## 2. API : le mot d'abord

**API** signifie *Application Programming Interface* — interface de programmation.

Le mot important est **interface** : une façade publique, un contrat. C'est
exactement la notion que tu as déjà rencontrée en C, avec les headers.

Souviens-toi de `stdio.h` : il **déclare** `printf` sans contenir son code. Il te
dit ce que la fonction accepte et ce qu'elle rend, pas comment elle s'y prend.

Une API web fonctionne pareil, mais à travers le réseau :

| | En C | Sur le web |
|---|---|---|
| Le contrat | `stdio.h` | La documentation de l'API |
| L'appel | `printf("%d", 42)` | `GET /articles/42` |
| L'implémentation | libSystem, déjà compilée | Le serveur, ailleurs sur Terre |
| Ce que tu ignores | Comment `printf` écrit à l'écran | Comment le serveur range ses données |

Dans les deux cas, **tu utilises sans savoir comment c'est fait**. C'est tout
l'intérêt : le serveur peut changer entièrement de base de données sans que ton
application s'en aperçoive, tant que le contrat tient.

## 3. Ce que REST veut dire

**REST** = *REpresentational State Transfer*. Le nom est opaque ; l'idée ne l'est pas.

REST n'est ni un langage, ni une bibliothèque, ni un logiciel à installer. C'est un
**style d'architecture** : un ensemble de conventions sur la manière d'organiser
une API. Rien ne t'oblige à les suivre — mais tout le monde les suit, ce qui rend
les API prévisibles.

L'idée centrale tient en une phrase : **on expose des ressources, désignées par des
adresses, manipulées par un petit nombre de verbes standard.**

Une ressource est une chose dont on parle : un utilisateur, un article, une
commande, un message. Elle a une adresse (une URL) et on agit dessus avec les
verbes de HTTP.

## 4. Les ressources et leurs adresses

Une bonne URL REST désigne **une chose**, pas une action :

```
/articles              la collection de tous les articles
/articles/42           l'article numéro 42
/articles/42/commentaires   les commentaires de l'article 42
/utilisateurs/7        l'utilisateur numéro 7
```

Deux conventions à retenir : **des noms, pas des verbes**, et **le pluriel pour
les collections**.

L'erreur classique consiste à mettre l'action dans l'URL :

| À éviter | Préférer |
|---|---|
| `/getArticle?id=42` | `GET /articles/42` |
| `/createArticle` | `POST /articles` |
| `/deleteArticle/42` | `DELETE /articles/42` |
| `/updateUserEmail` | `PATCH /utilisateurs/7` |

Dans la colonne de gauche, chaque nouvelle fonctionnalité invente une nouvelle URL,
et il faut lire la documentation pour deviner ce qui existe. Dans celle de droite,
**l'action est portée par le verbe HTTP**, et les URL restent un simple plan des
données. Quand tu connais une ressource, tu sais déjà comment agir dessus.

## 5. Les verbes HTTP

C'est le cœur de REST. Cinq verbes suffisent à presque tout :

| Verbe | Intention | Exemple | Modifie ? |
|---|---|---|---|
| `GET` | Lire | `GET /articles/42` | Non |
| `POST` | Créer | `POST /articles` | Oui |
| `PUT` | Remplacer entièrement | `PUT /articles/42` | Oui |
| `PATCH` | Modifier partiellement | `PATCH /articles/42` | Oui |
| `DELETE` | Supprimer | `DELETE /articles/42` | Oui |

### Deux propriétés qui comptent

**Sûr** (*safe*) : ne modifie rien. Seul `GET` l'est. C'est pourquoi un navigateur
peut recharger une page sans danger, et pourquoi une URL en `GET` ne doit **jamais**
supprimer quoi que ce soit.

**Idempotent** : répéter l'appel donne le même résultat qu'un seul appel. `GET`,
`PUT` et `DELETE` le sont ; `POST` ne l'est pas.

Cette distinction a une conséquence très concrète en mobile. Sur un réseau
instable — un tunnel, un ascenseur — ton application n'obtient pas de réponse et
ne sait pas si le serveur a reçu sa demande. Peut-elle réessayer ?

- Avec `DELETE`, oui : supprimer deux fois le même article revient à le supprimer.
- Avec `POST`, non : tu risques de créer deux commandes et de facturer deux fois.

D'où les mécanismes d'*idempotency key* qu'utilisent les API de paiement. Retiens
que **le réseau mobile échoue en permanence**, et que la conception de l'API doit
en tenir compte.

## 6. Les codes de réponse

Chaque réponse porte un nombre à trois chiffres qui dit comment ça s'est passé.
Le premier chiffre donne la famille :

| Famille | Sens | Codes courants |
|---|---|---|
| **2xx** | Ça a marché | `200` OK, `201` Créé, `204` Rien à renvoyer |
| **3xx** | Va voir ailleurs | `301` Déplacé, `304` Pas modifié |
| **4xx** | **Le client a tort** | `400`, `401`, `403`, `404`, `409`, `422`, `429` |
| **5xx** | **Le serveur a un problème** | `500`, `502`, `503` |

La frontière entre 4xx et 5xx est celle qui compte : **4xx, corrige ta requête ;
5xx, réessaie plus tard, ce n'est pas ta faute.**

Les quatre à connaître par cœur :

- **401 Unauthorized** — tu n'es pas identifié. *Connecte-toi.*
- **403 Forbidden** — tu es identifié, mais tu n'as pas le droit. *Inutile de réessayer.*
- **404 Not Found** — la ressource n'existe pas.
- **500 Internal Server Error** — le serveur a planté.

La confusion entre 401 et 403 est fréquente : le premier concerne **qui tu es**, le
second **ce que tu as le droit de faire**.

Ce système te rappellera quelque chose : c'est exactement le principe du **code de
sortie** de tes programmes C. `return 0` pour le succès, autre chose pour l'échec,
et le shell qui décide de la suite avec `&&`. Même idée, appliquée au réseau.

## 7. Le format des données : JSON

Il reste à s'accorder sur la forme des données échangées. Le format universel
aujourd'hui est **JSON** (*JavaScript Object Notation*).

```json
{
  "id": 42,
  "titre": "Apprendre le C",
  "publie": true,
  "vues": 1503,
  "auteur": {
    "id": 7,
    "nom": "Yann"
  },
  "tags": ["c", "debutant"]
}
```

C'est du **texte**, lisible par un humain, que tous les langages savent lire et
écrire. Six types seulement : nombre, chaîne, booléen, `null`, objet `{}`, tableau `[]`.

Le contraste avec le C est instructif. En C, tu déclares `int age = 30;` et la
valeur occupe 4 octets précis en mémoire. En JSON, `"age": 30` est **du texte** :
les caractères `3` et `0`. Le serveur les écrit, le réseau les transporte, ton
application les relit et les reconvertit en nombre.

C'est plus lent et plus volumineux qu'un format binaire — mais c'est lisible,
débogable et universel. Ce compromis est exactement ce que gRPC remet en question
(voir `../../grpc_api/`).

## 8. Une requête complète, décortiquée

Voici ce qui circule réellement sur le réseau quand ton application crée un article :

```http
POST /articles HTTP/1.1
Host: api.exemple.com
Content-Type: application/json
Authorization: Bearer eyJhbGciOiJIUzI1NiIs...
Content-Length: 52

{"titre": "Mon premier article", "publie": false}
```

Quatre parties :

1. **La ligne de requête** — le verbe, le chemin, la version du protocole
2. **Les en-têtes** — des métadonnées : le format envoyé, le jeton d'authentification
3. **Une ligne vide** — elle sépare les en-têtes du corps
4. **Le corps** — les données, en JSON

Et la réponse :

```http
HTTP/1.1 201 Created
Content-Type: application/json
Location: /articles/43

{"id": 43, "titre": "Mon premier article", "publie": false}
```

Le serveur répond `201` (créé, pas simplement `200`), indique où trouver la
nouvelle ressource avec l'en-tête `Location`, et renvoie l'objet complet — avec
l'`id` que le client ne pouvait pas connaître.

```
    APPLICATION MOBILE                      SERVEUR
           │                                   │
           │  POST /articles                   │
           │  + en-têtes + JSON                │
           ├──────────────────────────────────►│
           │                                   │  vérifie le jeton
           │                                   │  valide les données
           │                                   │  écrit en base
           │       201 Created + JSON          │
           │◄──────────────────────────────────┤
           │                                   │
       affiche                             la connexion
       le résultat                          se referme
```

**Chaque requête est indépendante.** C'est le principe *stateless* de REST : le
serveur ne se souvient de rien entre deux appels. Ton application doit renvoyer son
jeton d'authentification à chaque fois.

Ça paraît inefficace, mais c'est ce qui permet de répartir la charge sur mille
serveurs : n'importe lequel peut traiter n'importe quelle requête, puisqu'aucun ne
détient d'information particulière.

## 9. Essayer maintenant, dans ton terminal

**Cette section est praticable immédiatement.** `curl` est déjà installé sur ton
Mac, et il parle HTTP.

```sh
# Une requête GET simple
curl https://api.github.com/users/torvalds

# Voir les en-têtes de la réponse et le code de statut
curl -i https://api.github.com/users/torvalds

# Uniquement les en-têtes
curl -I https://api.github.com/users/torvalds

# Voir toute la conversation, requête comprise
curl -v https://api.github.com/users/torvalds
```

Un service fait pour l'apprentissage renvoie ce que tu lui envoies :

```sh
# GET
curl https://httpbin.org/get

# POST avec du JSON
curl -X POST https://httpbin.org/post \
     -H "Content-Type: application/json" \
     -d '{"titre": "test", "publie": true}'

# Provoquer un code d'erreur précis
curl -i https://httpbin.org/status/404
curl -i https://httpbin.org/status/500
```

Et pour relier ça à ce que tu connais déjà — le code de sortie :

```sh
curl -s -o /dev/null -w "%{http_code}\n" https://httpbin.org/status/404
```

C'est le même mécanisme que `echo $?` après une compilation.

**Prends dix minutes pour lancer ces commandes.** Elles transforment tout ce qui
précède en quelque chose de tangible.

## 10. L'authentification

Comment le serveur sait-il qui tu es, puisqu'il ne se souvient de rien ?

Ton application envoie une preuve d'identité **à chaque requête**, dans un en-tête :

```http
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9...
```

Ce jeton est obtenu une fois, en s'identifiant :

```
POST /auth/login  {"email": "...", "motdepasse": "..."}
        │
        ▼
   200 OK  {"token": "eyJ...", "expire_dans": 3600}
        │
        ▼
   stocké dans le trousseau sécurisé du téléphone
        │
        ▼
   renvoyé dans chaque requête suivante
```

Le format dominant est le **JWT** (*JSON Web Token*) : un jeton signé
cryptographiquement, que le serveur peut vérifier sans consulter sa base.

Deux règles de sécurité, non négociables :

**Toujours HTTPS.** En HTTP simple, le jeton circule en clair et n'importe qui sur
le même réseau Wi-Fi peut le lire.

**Jamais de secret dans le code de l'application.** Une application mobile est
téléchargeable et décompilable. Une clé d'API qui y est écrite en dur est publique.

## 11. Pagination, filtres, tri

`GET /articles` sur une base d'un million d'articles serait catastrophique. On
découpe, via des paramètres après le `?` :

```
GET /articles?page=2&limite=20
GET /articles?auteur=7&publie=true
GET /articles?tri=date&ordre=desc
GET /articles?page=2&limite=20&tri=date
```

Ces paramètres se combinent avec `&`. Ils appartiennent à l'URL, pas au corps —
un `GET` n'a pas de corps.

En mobile, c'est ce qui rend possible le défilement infini : charger vingt éléments,
puis les vingt suivants quand l'utilisateur approche du bas.

## 12. Ce que REST ne fait pas bien

Aucune technologie n'est bonne partout. Trois limites réelles :

**Le serveur ne peut pas parler spontanément.** REST est requête/réponse : le
client demande, le serveur répond. Pour un chat ou des notifications, il faudrait
demander « du nouveau ? » en boucle — coûteux en batterie et en données. C'est
précisément le problème que résolvent les **WebSockets** (`../../web_socket_api/`).

**Trop ou trop peu de données.** Un écran affiche le nom et la photo d'un
utilisateur ; l'API renvoie ses quarante champs. Ou l'inverse : il faut trois
appels successifs pour composer un seul écran. GraphQL est né de cette frustration.

**JSON est verbeux.** Du texte, avec les noms de champs répétés à chaque objet.
Sur un réseau mobile lent, ça se paie. C'est l'argument de **gRPC**
(`../../grpc_api/`), qui utilise un format binaire compact.

## 13. Choisir

| Besoin | Solution |
|---|---|
| Lire et écrire des données classiques | **REST** |
| Le serveur doit notifier le client | **WebSocket** |
| Performance maximale, mobile ↔ serveur | **gRPC** |
| Éviter d'écrire un backend entier | **Firebase** |
| Le client choisit exactement ses champs | **GraphQL** |

**REST reste le choix par défaut**, et pour de bonnes raisons : c'est le plus
simple, le mieux outillé, le plus documenté, celui que tout développeur comprend.
Les autres se justifient quand un besoin précis le réclame.

Une architecture mobile réelle les combine souvent : REST pour l'essentiel,
WebSocket pour le temps réel, et les notifications push pour réveiller
l'application.

## 14. Ce qui fait une bonne API

Six principes, que tu retrouveras dans toutes les bonnes API :

**La cohérence prime sur la perfection.** Une API imparfaite mais régulière est
plus agréable qu'une API brillante et imprévisible.

**Des messages d'erreur utiles.** `{"erreur": "Erreur"}` ne sert à rien. Dis quel
champ pose problème et pourquoi.

**Versionner dès le début.** `/v1/articles`. Le jour où tu dois changer le contrat,
les anciennes applications continuent de fonctionner — et en mobile, tu ne peux
pas forcer tes utilisateurs à mettre à jour.

**Le bon code de statut.** Renvoyer `200 OK` avec `{"erreur": "..."}` dedans oblige
chaque client à inspecter le corps pour savoir si ça a marché.

**Documenter.** OpenAPI (anciennement Swagger) génère une documentation navigable
à partir d'une description de l'API.

**Limiter le débit.** Sans quoi un client mal codé peut faire tomber ton service.
Code `429 Too Many Requests`.

---

## Mémo

```
RESSOURCES — des noms, au pluriel
  /articles              la collection
  /articles/42           un élément
  /articles/42/commentaires   une sous-ressource

VERBES — l'action est dans le verbe, pas dans l'URL
  GET     lire        sûr + idempotent
  POST    créer       ni l'un ni l'autre
  PUT     remplacer   idempotent
  PATCH   modifier    partiellement
  DELETE  supprimer   idempotent

CODES
  2xx  ça a marché         200 OK  201 Créé  204 Vide
  4xx  le client a tort    400  401  403  404  409  429
  5xx  le serveur a un pb  500  502  503

PARAMÈTRES
  ?page=2&limite=20&tri=date

AUTHENTIFICATION
  Authorization: Bearer <jeton>    à chaque requête (stateless)
```

```sh
curl -i URL                       # réponse + en-têtes
curl -v URL                       # toute la conversation
curl -X POST URL -H "Content-Type: application/json" -d '{"a":1}'
curl -s -o /dev/null -w "%{http_code}\n" URL   # le code seul
```

**Les trois idées à retenir :**

1. **Une API est un contrat**, comme un header en C : elle dit quoi appeler et ce
   qu'on obtient, jamais comment c'est fait.
2. **L'action est dans le verbe HTTP**, pas dans l'URL.
3. **Chaque requête est indépendante** — le serveur ne se souvient de rien.

Exercices : [../exercices/rest_api.md](../exercices/rest_api.md)
