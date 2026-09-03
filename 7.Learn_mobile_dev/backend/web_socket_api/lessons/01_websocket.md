# Les WebSockets — parler dans les deux sens

Tu as appris à écrire un `hello world` en C et à te déplacer dans un terminal Unix. Tu sais ce qu'est un fichier, un chemin absolu, une permission. Cette leçon te fait faire un saut : on quitte ta machine et on parle de **communication entre deux machines par le réseau**.

L'objectif n'est pas que tu saches coder un serveur WebSocket ce soir. L'objectif est que tu comprennes **quel problème les WebSockets résolvent**, pourquoi on a inventé ça, et pourquoi c'est central en développement mobile. Le code viendra plus tard, quand tu auras un langage de haut niveau dans les mains.

**Prérequis :** cette leçon suppose que tu as une idée vague de ce qu'est HTTP (le protocole du Web) et de ce qu'est un serveur. Si ce n'est pas le cas, tu peux lire quand même : j'explique le minimum au fur et à mesure. En revanche, les exemples de code sont en JavaScript, un langage que tu ne connais pas encore. Je les commente ligne par ligne — lis-les comme on lit une recette de cuisine dans une langue étrangère : tu comprends la structure sans savoir conjuguer.

---

## 1. Le problème : HTTP ne sait dire que « demande / réponse »

Le Web tout entier repose sur un principe très simple, et très limité.

Un **client** (ton navigateur, ton application mobile) envoie une **requête** à un **serveur**. Le serveur réfléchit, puis renvoie une **réponse**. Et c'est fini. La connexion se ferme, ou en tout cas plus rien ne se passe. Le serveur ne peut plus rien dire.

```
   CLIENT                                  SERVEUR
     |                                        |
     |  --- "donne-moi la page d'accueil" --> |
     |                                        | (il cherche)
     |  <-- "voici le HTML" ----------------- |
     |                                        |
     |          (silence total)               |
     |                                        |
```

Ce modèle s'appelle **requête/réponse**. Il a une propriété fondamentale : **seul le client peut initier la conversation**. Le serveur est passif. Il attend. Il répond. Il ne parle jamais spontanément.

C'est exactement comme un distributeur de boissons. Tu mets une pièce, tu appuies, il te donne un soda. Le distributeur ne te tapera jamais sur l'épaule pour te dire « au fait, il reste deux canettes ».

Pour beaucoup de choses, ça suffit parfaitement. Tu veux la liste des articles d'un blog ? Tu demandes, on te répond. Parfait.

Mais imagine maintenant que tu construis une application de messagerie. Ton ami t'envoie « salut ». Ce message arrive sur le serveur. Le serveur sait que tu dois le recevoir. Mais il n'a **aucun moyen** de te le dire. Il doit attendre que ton téléphone demande.

C'est absurde. C'est le problème que les WebSockets résolvent.

---

## 2. Les solutions bancales d'avant

Avant les WebSockets (normalisées en 2011), les développeurs ont bricolé. Il faut connaître ces bricolages pour deux raisons : d'abord parce qu'ils sont encore utilisés aujourd'hui et qu'ils sont parfois le bon choix ; ensuite parce qu'ils te font comprendre par contraste ce qu'apporte vraiment une WebSocket.

### 2.1 Le polling (interrogation régulière)

L'idée la plus bête possible, et donc la première inventée : **demander en boucle**.

Le client redemande au serveur toutes les 2 secondes : « y a-t-il du nouveau ? ». La plupart du temps, la réponse est « non ».

```
   CLIENT                                  SERVEUR
     |  --- "du nouveau ?" -----------------> |
     |  <-- "non" -------------------------- |
     |         (attente 2 s)                  |
     |  --- "du nouveau ?" -----------------> |
     |  <-- "non" -------------------------- |
     |         (attente 2 s)                  |
     |  --- "du nouveau ?" -----------------> |
     |  <-- "non" -------------------------- |
     |         (attente 2 s)                  |
     |  --- "du nouveau ?" -----------------> |
     |  <-- "OUI : message de Léa" --------- |   <-- enfin !
     |                                        |
```

Le coût est brutal. Sur mobile, chaque requête réveille la puce radio du téléphone, ce qui consomme de la batterie. Chaque requête transporte des en-têtes HTTP (souvent plusieurs centaines d'octets) pour un contenu utile de… zéro octet. Et malgré tout ce gaspillage, le message peut arriver avec jusqu'à 2 secondes de retard.

Multiplie ça par 10 000 utilisateurs connectés : ton serveur reçoit 5 000 requêtes inutiles par seconde.

### 2.2 Le long polling (interrogation longue)

Amélioration maligne : le client demande, et le serveur **ne répond pas tout de suite**. Il garde la requête ouverte, en suspens, jusqu'à ce qu'il ait vraiment quelque chose à dire.

```
   CLIENT                                  SERVEUR
     |  --- "du nouveau ?" -----------------> |
     |                                        | (il garde la ligne
     |         ... 40 secondes ...            |  ouverte, il attend)
     |                                        |
     |  <-- "OUI : message de Léa" --------- |
     |  --- "du nouveau ?" -----------------> |   <-- on relance aussitôt
     |         ... attente ...                |
```

C'est beaucoup mieux : le message arrive quasi instantanément et il n'y a plus de requêtes vides. Mais chaque message force à refermer puis rouvrir une requête, avec toute la paperasse HTTP à chaque fois. Et pour envoyer un message *vers* le serveur, il faut ouvrir une **deuxième** connexion en parallèle. C'est de la plomberie fragile.

### 2.3 Les SSE (Server-Sent Events)

Une vraie normalisation du besoin « le serveur veut me parler ». Le client ouvre une connexion HTTP qui **reste ouverte**, et le serveur y déverse des événements au fil de l'eau.

```
   CLIENT                                  SERVEUR
     |  --- "abonne-moi au flux" -----------> |
     |  <=== flux ouvert, jamais fermé ====== |
     |  <-- événement 1 -------------------- |
     |  <-- événement 2 -------------------- |
     |  <-- événement 3 -------------------- |
     |                                        |
     |  (mais le client ne peut RIEN envoyer  |
     |   dans ce tuyau — sens unique)         |
```

Les SSE sont excellents et sous-estimés. Ils reconnectent tout seuls, ils passent partout, ils sont simples. Leur limite est dans le nom : **Server-Sent**. C'est un sens unique. Le serveur parle, le client écoute. Pour un flux de notifications ou un cours de bourse, c'est idéal. Pour un chat, il te manque la moitié du tuyau.

---

## 3. Ce qu'est vraiment une WebSocket

Une WebSocket, c'est **une connexion TCP persistante et bidirectionnelle entre un client et un serveur**.

Décortiquons les trois mots importants.

**TCP.** C'est la couche en dessous de HTTP, le vrai « tuyau » du réseau. Quand ton navigateur charge une page, il ouvre une connexion TCP, parle HTTP dedans, puis referme. Une WebSocket garde ce tuyau TCP ouvert et arrête d'y parler HTTP.

**Persistante.** La connexion reste établie. Pas de « je demande, je reçois, je raccroche ». La ligne est ouverte pendant des minutes ou des heures.

**Bidirectionnelle**, et plus précisément **full-duplex** : les deux côtés peuvent envoyer **en même temps**, sans attendre le tour de l'autre.

```
   HTTP (half-duplex, à tour de rôle)
   ────────────────────────────────────
   CLIENT  ────requête────►  SERVEUR
   CLIENT  ◄───réponse────   SERVEUR
   CLIENT  ────requête────►  SERVEUR
   CLIENT  ◄───réponse────   SERVEUR


   WEBSOCKET (full-duplex, simultané)
   ────────────────────────────────────
              ────msg────►
   CLIENT     ◄───msg────      SERVEUR
              ────msg────►
              ◄───msg────
              ◄───msg────      (le serveur peut
              ────msg────►      envoyer 3 messages
              ◄───msg────       d'affilée sans qu'on
                                lui demande rien)
```

La bonne image mentale : HTTP, c'est **le talkie-walkie** — il faut dire « à toi » et lâcher le bouton. Une WebSocket, c'est **le téléphone** — les deux personnes peuvent parler, s'interrompre, se taire, sans protocole de tour de parole.

Une fois la connexion établie, envoyer un message coûte quelques octets de surcoût, contre plusieurs centaines pour une requête HTTP. C'est la différence entre glisser un mot sous une porte déjà entrouverte et sonner, se présenter, montrer ses papiers, puis parler.

---

## 4. Le handshake : ça commence en HTTP

Voici le point le plus élégant du protocole, et celui que les débutants comprennent mal.

**Une WebSocket ne commence pas comme une WebSocket. Elle commence comme une requête HTTP tout à fait normale.**

Pourquoi ? Pour des raisons purement pratiques. Internet est truffé de pare-feux, de proxys et d'équipements réseau qui ne laissent passer que les ports 80 et 443 (HTTP et HTTPS). Si on avait inventé un protocole totalement nouveau sur un port exotique, il aurait été bloqué partout. Alors les concepteurs ont ruse : on frappe à la porte en parlant HTTP, on demande poliment à changer de langue, et si le serveur accepte, on bascule.

Ce moment de bascule s'appelle le **handshake** (la poignée de main).

### 4.1 La requête du client

```http
GET /chat HTTP/1.1
Host: exemple.com
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13
Origin: https://exemple.com
```

C'est un `GET` HTTP normal, mais avec des en-têtes particuliers :

| En-tête | Ce qu'il dit |
|---|---|
| `Upgrade: websocket` | « Je voudrais changer de protocole pour WebSocket » |
| `Connection: Upgrade` | « Cette demande de changement concerne toute la connexion » |
| `Sec-WebSocket-Key` | Une valeur aléatoire, générée à chaque fois, qui sert à prouver que le serveur a vraiment compris la demande |
| `Sec-WebSocket-Version: 13` | La version du protocole (13 est la seule utilisée aujourd'hui) |
| `Origin` | D'où vient la demande — le serveur peut refuser une origine inconnue |

### 4.2 La réponse du serveur

```http
HTTP/1.1 101 Switching Protocols
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
```

Le code `101 Switching Protocols` est rare et précieux : tu ne le croiseras quasiment que là. Il veut dire « d'accord, on arrête de parler HTTP à partir de maintenant ».

Le `Sec-WebSocket-Accept` n'est pas magique. Le serveur prend la clé envoyée par le client, y colle une chaîne fixe définie dans la norme (`258EAFA5-E914-47DA-95CA-C5AB0DC85B11`), calcule une empreinte SHA-1 du tout, et l'encode en base64. Ça ne sécurise rien — c'est juste une preuve que le serveur parle bien WebSocket et n'est pas un cache HTTP qui répond n'importe quoi.

### 4.3 Le schéma complet

```
   CLIENT                                     SERVEUR
     |                                           |
     |  1. Ouverture de la connexion TCP         |
     |  ═══════════════════════════════════════► |
     |                                           |
     |  2. GET /chat HTTP/1.1                    |
     |     Upgrade: websocket          [HTTP]    |
     |  ────────────────────────────────────────►|
     |                                           |
     |  3. HTTP/1.1 101 Switching Protocols      |
     |     Upgrade: websocket          [HTTP]    |
     |  ◄────────────────────────────────────────|
     |                                           |
     |  ═══ ON N'EST PLUS EN HTTP ═══════════════|
     |                                           |
     |  4. frames WebSocket dans les deux sens   |
     |     ◄──────────────────────────────────►  |
     |     ◄──────────────────────────────────►  |
     |                                           |
     |  5. frame de fermeture                    |
     |  ────────────────────────────────────────►|
     |  ◄────────────────────────────────────────|
     |  (fermeture TCP)                          |
```

Remarque bien le point 3 : **la connexion TCP n'est jamais refermée entre l'étape HTTP et l'étape WebSocket**. C'est le même tuyau, on change juste ce qu'on met dedans.

---

## 5. ws:// et wss://

Comme HTTP a `http://` et `https://`, WebSocket a deux schémas d'URL :

| Schéma | Port par défaut | Chiffré ? | À utiliser ? |
|---|---|---|---|
| `ws://` | 80 | Non | Uniquement en développement local |
| `wss://` | 443 | Oui (TLS) | **Toujours**, en production |

Le `wss://` fait exactement ce que fait `https://` : il enveloppe toute la conversation dans une couche de chiffrement TLS, si bien que personne sur le trajet (le WiFi du café, l'opérateur mobile) ne peut lire ni modifier tes messages.

En développement mobile, ce n'est même pas négociable : iOS (App Transport Security) et Android bloquent par défaut les connexions non chiffrées. Une application qui tente un `ws://` vers Internet sera tout simplement refusée par le système.

**Retiens la règle :** en local `ws://localhost:8080`, partout ailleurs `wss://`.

---

## 6. Le cycle de vie d'une connexion

Une WebSocket, du point de vue du code, se résume à quatre moments. Ce sont quatre **événements** auxquels tu vas réagir.

```
                   ┌──────────────┐
     tentative     │  CONNECTING  │
   de connexion ──►│ (handshake)  │
                   └──────┬───────┘
                          │
             ┌────────────┴────────────┐
             │ succès            échec │
             ▼                         ▼
       ┌──────────┐              ┌──────────┐
       │   OPEN   │              │  ERROR   │
       │          │              └────┬─────┘
       │ ◄─ message ─►                │
       │ ◄─ message ─►                │
       └─────┬────┘                   │
             │                        │
             ▼                        ▼
       ┌──────────────────────────────────┐
       │             CLOSED               │
       └──────────────────────────────────┘
```

**`open`** — le handshake a réussi, le tuyau est prêt. C'est seulement à partir de ce moment que tu as le droit d'envoyer quelque chose. Envoyer avant `open` provoque une erreur : c'est l'erreur numéro un des débutants.

**`message`** — des données sont arrivées. Cet événement peut se déclencher des milliers de fois.

**`close`** — la connexion est terminée. Elle peut l'être proprement (l'un des deux a dit au revoir) ou salement (le réseau a disparu). Un code numérique t'indique lequel : `1000` signifie « fermeture normale », `1006` signifie « la connexion a été coupée sans qu'on me prévienne » — c'est celui que tu verras quand ton utilisateur entre dans un tunnel.

**`error`** — quelque chose s'est mal passé. En pratique, `error` est presque toujours suivi de `close`. Ton vrai travail de reconnexion se fait donc dans `close`, pas dans `error`.

---

## 7. Les frames : l'unité de transport

Dans le tuyau, les données ne circulent pas en vrac. Elles sont découpées en **frames** (trames), de petits paquets avec un en-tête minuscule de 2 à 14 octets.

Chaque frame porte un **opcode**, un numéro qui dit de quelle sorte elle est :

| Opcode | Type | À quoi ça sert |
|---|---|---|
| `0x1` | Texte | Du texte UTF-8 — en pratique, presque toujours du JSON |
| `0x2` | Binaire | Des octets bruts : image, son, format compact |
| `0x8` | Close | « Je raccroche » |
| `0x9` | Ping | « Tu es toujours là ? » |
| `0xA` | Pong | « Oui, je suis là » |

**Prérequis :** je viens de dire « JSON ». C'est un format texte pour représenter des données structurées, du type `{"auteur": "Lea", "texte": "salut"}`. Tu le verras en détail dans la leçon sur les API REST. Pour l'instant, retiens juste que c'est du texte lisible, avec des accolades, qui sert à emballer plusieurs informations dans un seul message.

### Ping et pong : garder la ligne vivante

C'est un mécanisme dont on sous-estime l'importance, surtout en mobile.

Le problème : une connexion TCP qui ne transporte rien pendant longtemps **peut être coupée silencieusement**. Un routeur, un pare-feu d'entreprise, ou la passerelle NAT de ton opérateur mobile décident au bout de quelques minutes que cette connexion inactive prend de la place pour rien, et la suppriment de leur table. Personne ne prévient les deux extrémités. Client et serveur croient tous les deux être connectés, alors que plus rien ne passe. On appelle ça une **connexion zombie**.

La parade : envoyer périodiquement une frame `ping`. L'autre côté doit répondre par un `pong`. Ce trafic minuscule (quelques octets) suffit à convaincre les équipements réseau que la connexion sert encore.

```
   CLIENT                                     SERVEUR
     |                                           |
     |         (30 secondes de silence)          |
     |  <-------------- PING -------------------|
     |  --------------- PONG ------------------>|
     |                                           |
     |         (30 secondes de silence)          |
     |  <-------------- PING -------------------|
     |         (pas de PONG… 10 s… 20 s…)        |
     |                                           |
     |                                  le serveur en déduit
     |                                  que le client est mort
     |                                  et libère ses ressources
```

Ce mécanisme sert donc deux choses à la fois : maintenir la connexion ouverte, **et** détecter qu'elle est morte. Sans ping/pong, un serveur finirait par accumuler des milliers de connexions fantômes.

Note importante : dans un navigateur, les ping/pong sont gérés automatiquement par le navigateur, et ton code JavaScript ne les voit pas. Sur mobile ou côté serveur, tu dois souvent t'en occuper toi-même, ou implémenter un « heartbeat » applicatif (un message `{"type":"ping"}` en texte que ton propre code envoie).

---

## 8. Où les WebSockets brillent

Le critère est toujours le même : **est-ce que le serveur a besoin de parler sans qu'on lui demande ?**

**Le chat et la messagerie.** Le cas d'école. Deux flux, dans les deux sens, imprévisibles.

**Les notifications en direct.** « Ta commande est en préparation », « quelqu'un a aimé ta photo ». Note qu'ici, les SSE feraient aussi bien : le flux est à sens unique.

**Les jeux temps réel.** La position de chaque joueur doit circuler dix à soixante fois par seconde. Le surcoût par message devient le facteur déterminant, et c'est là que WebSocket écrase HTTP.

**Les cours de bourse et les tableaux de bord.** Des données qui changent en permanence et que plusieurs clients regardent simultanément.

**L'édition collaborative.** Deux personnes tapent dans le même document. Chaque frappe de l'un doit apparaître chez l'autre. C'est du full-duplex pur.

**La géolocalisation en direct.** Le livreur envoie sa position, le client la reçoit. Typiquement mobile.

---

## 9. Quand ne PAS utiliser une WebSocket

C'est aussi important que le reste, parce que la tentation du débutant qui découvre un nouvel outil est de le mettre partout.

**Si tu lis des données à la demande, REST suffit.** Charger la liste des produits, afficher un profil, récupérer un article : le client demande, le serveur répond, c'est fini. Une WebSocket ici n'apporterait rien et te coûterait une connexion permanente à maintenir.

**Si les données changent rarement.** Une météo qui se met à jour toutes les heures ne justifie pas une connexion ouverte en permanence.

**Si le trafic est purement descendant.** Utilise les SSE. Reconnexion automatique intégrée, plus simple, mieux supporté par les proxys.

**Si tu veux réveiller une application fermée.** Une WebSocket meurt quand l'OS mobile met ton application en arrière-plan. Pour ça, il faut les **notifications push** (APNs sur iOS, FCM sur Android), qui passent par un canal maintenu par le système d'exploitation lui-même. C'est un mécanisme totalement différent, et les deux se complètent : le push réveille, la WebSocket prend le relais.

**Si tu peux mettre en cache.** HTTP a un système de cache très riche (CDN, `Cache-Control`, `ETag`). Une WebSocket court-circuite tout ça : chaque message est unique et non cacheable.

---

## 10. Le tableau comparatif

| Critère | Polling | Long polling | SSE | WebSocket | REST |
|---|---|---|---|---|---|
| Sens de communication | Client → Serveur | Client → Serveur | Serveur → Client | Bidirectionnel | Client → Serveur |
| Connexion permanente | Non | Semi | Oui | Oui | Non |
| Latence | Mauvaise (l'intervalle) | Bonne | Excellente | Excellente | N/A |
| Surcoût par message | Très élevé | Élevé | Faible | Très faible | Élevé |
| Consommation batterie | Très mauvaise | Moyenne | Bonne | Bonne | Bonne |
| Reconnexion automatique | Sans objet | À coder | **Intégrée** | À coder | Sans objet |
| Passe les proxys/pare-feux | Toujours | Toujours | Presque toujours | Parfois bloqué | Toujours |
| Cacheable | Oui | Non | Non | Non | **Oui** |
| Complexité de mise en œuvre | Très faible | Moyenne | Faible | Élevée | Faible |
| Données binaires | Possible | Possible | **Non** | **Oui** | Possible |
| Charge serveur (10 000 clients) | Très élevée | Élevée | Moyenne | Moyenne (mémoire) | Faible |

Lis ce tableau en diagonale : il n'y a pas de vainqueur. Il y a un outil par besoin. La ligne « complexité » explique pourquoi tant d'applications qui *pourraient* utiliser des WebSockets s'en passent très bien.

---

## 11. Les difficultés réelles

Une WebSocket dans un tutoriel s'écrit en dix lignes. Une WebSocket en production, c'est un tout autre métier. Voici ce qui fait mal.

### 11.1 La reconnexion

Le réseau tombera. Ce n'est pas une hypothèse, c'est une certitude. Ton code doit donc se reconnecter tout seul.

Mais attention au piège : si tu te reconnectes immédiatement dans une boucle, et que ton serveur est tombé, tes 10 000 clients vont le marteler en continu et l'empêcher de se relever. On appelle ça un « thundering herd », une ruée.

La bonne pratique s'appelle le **backoff exponentiel avec gigue** : tu attends 1 s, puis 2 s, puis 4 s, 8 s, 16 s… jusqu'à un plafond (30 s par exemple), et tu ajoutes à chaque fois un petit délai aléatoire pour que tous les clients ne reviennent pas exactement au même instant.

```
   Tentative 1 : attendre  1 s  (+ 0 à 300 ms au hasard)
   Tentative 2 : attendre  2 s  (+ hasard)
   Tentative 3 : attendre  4 s  (+ hasard)
   Tentative 4 : attendre  8 s  (+ hasard)
   Tentative 5 : attendre 16 s  (+ hasard)
   Tentative 6 : attendre 30 s  (plafond atteint)
   Tentative 7 : attendre 30 s
```

### 11.2 Les messages perdus pendant la coupure

Tu es déconnecté 20 secondes. Pendant ce temps, trois messages t'étaient destinés. Où sont-ils ?

Nulle part, si tu n'as rien prévu. Le protocole WebSocket ne garantit **aucune** persistance. Il te faut un mécanisme applicatif : numéroter les messages, et à la reconnexion, envoyer « le dernier que j'ai reçu portait le numéro 42, donne-moi la suite ». Le serveur doit alors conserver un historique.

### 11.3 Le scaling horizontal

C'est le problème d'architecture le plus important, et il mérite un schéma.

Quand une seule machine ne suffit plus, on en ajoute. Mais avec HTTP, chaque requête est indépendante : n'importe quel serveur peut la traiter. Avec les WebSockets, chaque client est **collé** à un serveur précis pendant toute la durée de sa connexion.

```
                    ┌─────────────┐
     Alice ────────►│  Serveur A  │
                    └─────────────┘
                          ✗ ?
                    ┌─────────────┐
     Bob   ────────►│  Serveur B  │
                    └─────────────┘

   Alice envoie un message à Bob.
   Le serveur A n'a AUCUNE connexion vers Bob.
   Il ne peut pas lui parler.
```

La solution habituelle est d'ajouter un intermédiaire, un bus de messages (Redis Pub/Sub, NATS, Kafka), auquel tous les serveurs sont abonnés :

```
                    ┌─────────────┐
     Alice ────────►│  Serveur A  │──┐
                    └─────────────┘  │
                                     ▼
                              ┌─────────────┐
                              │ Redis Pub/  │
                              │    Sub      │
                              └─────────────┘
                                     │
                    ┌─────────────┐  │
     Bob   ◄────────│  Serveur B  │◄─┘
                    └─────────────┘
```

Le serveur A publie « message pour Bob » dans le bus, le serveur B, qui détient la connexion de Bob, le reçoit et le lui transmet. Tu n'as pas à savoir implémenter ça aujourd'hui, mais tu dois savoir que **ce problème existe**, parce qu'il transforme complètement une architecture.

### 11.4 L'état côté serveur

Avec une API REST classique, le serveur est **sans état** (stateless) : il ne se souvient de rien entre deux requêtes. C'est ce qui le rend facile à dupliquer, à redémarrer, à mettre à jour.

Avec des WebSockets, le serveur garde en mémoire, pour chaque client connecté : la socket elle-même, l'identité de l'utilisateur, ses abonnements, un tampon de messages. Multiplie par 50 000 connexions et la mémoire devient une ressource critique. Surtout, **redémarrer un serveur déconnecte tout le monde d'un coup** — et déclenche justement la ruée de reconnexions dont on parlait plus haut.

---

## 12. Les spécificités mobiles

Puisque ton objectif est le développement mobile, cette section est celle qui te concernera le plus directement.

**La batterie.** Maintenir une connexion ouverte empêche la puce radio de se rendormir complètement. Un heartbeat trop fréquent (toutes les 5 secondes) vide une batterie de façon visible. Un heartbeat trop rare laisse les NAT couper la connexion. L'usage recommande une fenêtre de 30 à 60 secondes, à ajuster selon le réseau.

**Le réseau instable.** C'est la réalité quotidienne d'un téléphone : tunnel, ascenseur, sous-sol, bascule du WiFi vers la 4G en sortant de chez soi. À chaque bascule WiFi/cellulaire, **l'adresse IP change**, et la connexion TCP meurt. Instantanément. Ton code doit traiter ça comme un événement normal et fréquent, pas comme une exception rare.

**La mise en arrière-plan.** Le point le plus dur, et le plus mal compris.

Quand l'utilisateur quitte ton application, iOS et Android suspendent son exécution. Ta WebSocket est fermée, souvent en quelques secondes sur iOS. Tu ne peux pas lutter, c'est une décision du système d'exploitation pour préserver la batterie, et aucune astuce durable ne le contourne.

```
   App au premier plan      App en arrière-plan     App fermée
   ────────────────────     ───────────────────     ──────────
   WebSocket vivante   ───► suspendue puis      ───► morte
                            fermée par l'OS
                            (~30 s sur iOS)

   Le seul canal qui survit :  NOTIFICATION PUSH
                               (APNs / FCM)
```

La conséquence architecturale est nette : **une application mobile sérieuse utilise les deux mécanismes**. La WebSocket sert à la conversation en direct quand l'utilisateur regarde l'écran. La notification push sert à le prévenir quand il ne regarde pas. Ne compte jamais sur une WebSocket pour livrer un message à une application en arrière-plan.

---

## 13. Un client en JavaScript, ligne par ligne

**Prérequis :** ce code est en JavaScript. Tu ne connais pas encore ce langage. Ne cherche pas à le taper ni à le comprendre en détail — lis-le pour voir la **forme** que prend le cycle de vie de la section 6.

```js
// On crée la connexion. Le "new" fabrique un objet WebSocket.
// L'URL commence par wss:// : la connexion sera chiffrée.
// ATTENTION : à cette ligne, la connexion N'EST PAS encore ouverte.
// Le handshake vient tout juste de partir.
const socket = new WebSocket("wss://echo.websocket.org");

// ─── ÉVÉNEMENT 1 : open ───────────────────────────────────
// Cette fonction sera appelée PLUS TARD, quand le serveur aura
// répondu "101 Switching Protocols".
// C'est le premier moment où on a le droit d'envoyer.
socket.onopen = function () {
  console.log("Connexion ouverte !");
  socket.send("Bonjour serveur");   // envoi d'une frame texte
};

// ─── ÉVÉNEMENT 2 : message ────────────────────────────────
// Appelée à CHAQUE frame reçue. Peut se déclencher des milliers
// de fois. Le paramètre "event" contient les données dans
// event.data — une chaîne de texte, ou des octets bruts.
socket.onmessage = function (event) {
  console.log("Reçu du serveur :", event.data);
};

// ─── ÉVÉNEMENT 3 : close ──────────────────────────────────
// La connexion est terminée, proprement ou non.
// event.code == 1000 : fermeture normale, voulue.
// event.code == 1006 : coupure brutale (réseau perdu, tunnel…).
// C'est ICI qu'on déclenche une tentative de reconnexion.
socket.onclose = function (event) {
  console.log("Fermée. Code :", event.code, "Propre :", event.wasClean);
};

// ─── ÉVÉNEMENT 4 : error ──────────────────────────────────
// Quelque chose a mal tourné. En pratique, "close" suit presque
// toujours "error" : ne mets donc pas ta logique de reconnexion ici.
socket.onerror = function (error) {
  console.log("Erreur :", error);
};

// Pour fermer soi-même la connexion, proprement :
// socket.close(1000, "au revoir");
```

Le point conceptuel à retenir, et le plus déroutant quand on vient du C : **ces fonctions ne s'exécutent pas dans l'ordre où elles sont écrites**. En C, ton programme déroule les instructions les unes après les autres, de haut en bas. Ici, tu te contentes de dire « quand ceci arrivera, exécute cela », puis tu passes à la suite. Le programme continue de tourner, et les fonctions se déclenchent plus tard, dans un ordre imprévisible.

Cette façon de programmer s'appelle la **programmation asynchrone**, et par événements. C'est le fondement de tout le développement d'interfaces et de réseau. Tu y consacreras une leçon entière — c'est probablement le plus gros changement de raisonnement qui t'attend après le C.

---

## 14. Tester une WebSocket depuis ton terminal

Bonne nouvelle : tu peux jouer avec des WebSockets **dès aujourd'hui**, sans écrire une ligne de code, en réutilisant exactement les compétences terminal que tu viens d'acquérir.

### websocat

C'est l'outil le plus pratique. Sur macOS, avec Homebrew :

```sh
# Installer (une seule fois)
brew install websocat

# Vérifier que c'est bien installé
websocat --version

# Se connecter à un serveur d'écho public.
# Tout ce que tu tapes te revient. Tape, appuie sur Entrée, regarde.
# Ctrl-C pour quitter.
websocat wss://echo.websocket.org

# Voir TOUT ce qui se passe, y compris le handshake HTTP
# et les frames ping/pong : c'est le mode le plus instructif.
websocat -v wss://echo.websocket.org
```

Si `brew` n'est pas installé sur ta machine, l'installateur est sur `brew.sh`. Sur Apple Silicon, Homebrew s'installe dans `/opt/homebrew` — c'est un **chemin absolu**, exactement la notion que tu as vue avec le terminal.

### wscat

Une alternative, qui demande Node.js :

```sh
brew install node          # si Node n'est pas déjà là
npm install -g wscat       # -g = installation globale
wscat -c wss://echo.websocket.org
```

### Observer le handshake avec curl

Celui-ci est un excellent exercice, parce qu'il te fait **voir de tes yeux** les en-têtes de la section 4 :

```sh
curl -i -N \
  -H "Connection: Upgrade" \
  -H "Upgrade: websocket" \
  -H "Sec-WebSocket-Version: 13" \
  -H "Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==" \
  https://echo.websocket.org
```

Tu devrais voir apparaître `HTTP/1.1 101 Switching Protocols`. `curl` ne sait pas parler WebSocket au-delà du handshake, donc ça s'arrête là — mais le handshake, lui, tu l'auras vu en vrai. C'est déjà beaucoup.

L'option `-i` affiche les en-têtes de réponse, `-N` désactive la mise en tampon, `-H` ajoute un en-tête. Ce sont des options de ligne de commande, comme les `-l` ou `-a` de `ls` que tu connais déjà.

---

## Mémo

**Le problème.** HTTP est requête/réponse : seul le client peut parler en premier, le serveur ne peut jamais prendre l'initiative.

**Les bricolages d'avant.** Le polling gaspille (des requêtes vides en boucle). Le long polling est mieux mais lourd. Les SSE sont propres mais à sens unique.

**Une WebSocket** est une connexion TCP persistante et full-duplex : les deux côtés parlent quand ils veulent, simultanément. Talkie-walkie contre téléphone.

**Le handshake** commence en HTTP avec `Upgrade: websocket`, le serveur répond `101 Switching Protocols`, et la même connexion TCP change de protocole. Cette ruse permet de passer les pare-feux.

**`wss://` toujours** en production ; `ws://` seulement en local. Les OS mobiles refusent le non chiffré.

**Quatre événements :** `open` (le seul moment où l'envoi devient possible), `message`, `close`, `error`. La reconnexion se code dans `close`, pas dans `error`.

**Les frames** portent du texte (souvent du JSON) ou du binaire. Les `ping`/`pong` maintiennent la connexion vivante face aux NAT et détectent les connexions zombies.

**Utilise WebSocket** pour le chat, les jeux, la collaboration, tout ce qui est bidirectionnel et fréquent. **N'utilise pas WebSocket** pour de la simple lecture de données : REST suffit et se met en cache.

**Les vraies difficultés** sont la reconnexion (avec backoff exponentiel), les messages perdus pendant une coupure, le scaling horizontal (il faut un bus type Redis Pub/Sub), et l'état conservé côté serveur.

**En mobile :** le réseau tombe souvent (tunnels, bascule WiFi/4G qui change l'IP), la batterie impose un heartbeat de 30 à 60 s, et **l'OS ferme ta WebSocket dès que l'app passe en arrière-plan**. Les notifications push sont le complément obligatoire, pas une option.

**Pour pratiquer maintenant :** `brew install websocat`, puis `websocat -v wss://echo.websocket.org`.

---

Passe maintenant aux [exercices](../exercices/websocket.md).
