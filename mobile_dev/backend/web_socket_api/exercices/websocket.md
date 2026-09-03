# Exercices — Les WebSockets

Ces exercices accompagnent la leçon [01_websocket.md](../lessons/01_websocket.md). Relis-la avant de commencer, et garde-la ouverte à côté : elle contient tout ce dont tu as besoin.

Ils sont classés en six niveaux de difficulté croissante. **Ne saute pas les niveaux 1 et 2** parce qu'ils ne contiennent pas de code — ce sont les plus importants. Un développeur qui sait choisir le bon outil vaut mieux qu'un développeur qui sait coder n'importe quoi.

**Sur ton niveau actuel :** tu sors du `hello world` en C et des bases du terminal. Les niveaux 1, 2, 3 et 6 sont **entièrement faisables aujourd'hui** — ils demandent du raisonnement, de la lecture et du terminal. Les niveaux 4 et 5 demandent un langage de haut niveau que tu ne connais pas encore : traite-les d'abord **sur papier**, en pseudo-code ou en français, et reviens y coder quand tu auras fait la leçon sur JavaScript. Écrire l'algorithme en français est un vrai exercice de développeur, pas un lot de consolation.

Chaque exercice indique entre crochets ce qu'il demande : `[papier]`, `[terminal]`, `[code]`, `[réflexion]`.

---

## Niveau 1 — Comprendre le protocole

### Exercice 1.1 — Le distributeur `[papier]`

Explique avec tes propres mots, en cinq phrases maximum et sans jargon technique, pourquoi un serveur HTTP ne peut pas prévenir spontanément un client qu'un nouveau message est arrivé. Trouve une analogie du quotidien **différente** de celle du distributeur de boissons utilisée dans la leçon.

### Exercice 1.2 — Lire un handshake `[papier]`

Voici une requête interceptée sur le réseau :

```http
GET /ws/notifications HTTP/1.1
Host: api.monapp.io
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==
Sec-WebSocket-Version: 13
Origin: https://monapp.io
```

Réponds à ces questions :

1. Quelle méthode HTTP est utilisée, et pourquoi celle-là plutôt qu'une autre ?
2. Quel en-tête, précisément, exprime la demande de changer de protocole ?
3. À quoi sert `Sec-WebSocket-Key` ? Est-ce que ça sécurise quelque chose ?
4. Que se passerait-il si le serveur ne connaissait pas les WebSockets et traitait cette requête comme un `GET` ordinaire ?

### Exercice 1.3 — Le code 101 `[papier]`

Le serveur répond `HTTP/1.1 101 Switching Protocols`.

1. Que signifie exactement le mot « Switching » ici — qu'est-ce qui change, et qu'est-ce qui ne change pas ?
2. La connexion TCP est-elle refermée puis rouverte entre le handshake et le premier message ? Justifie.
3. Cite deux autres codes HTTP que tu connais et explique en quoi `101` est d'une nature différente.

### Exercice 1.4 — Repérer l'intrus `[papier]`

Parmi ces en-têtes, lesquels sont **obligatoires** dans une requête de handshake WebSocket, lesquels sont optionnels, et lequel n'a rien à faire là ?

```
Upgrade: websocket
Content-Length: 0
Connection: Upgrade
Sec-WebSocket-Version: 13
Sec-WebSocket-Protocol: chat
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Origin: https://monapp.io
```

L'en-tête `Sec-WebSocket-Protocol` n'est pas décrit dans la leçon : cherche à quoi il sert. Savoir chercher une réponse qu'on ne t'a pas donnée fait partie de l'exercice.

### Exercice 1.5 — Le vocabulaire `[papier]`

Explique la différence entre ces paires de termes. Une ou deux phrases chacune.

| Terme A | Terme B |
|---|---|
| half-duplex | full-duplex |
| `ws://` | `wss://` |
| frame texte | frame binaire |
| `ping` | `pong` |
| code de fermeture `1000` | code de fermeture `1006` |

---

## Niveau 2 — Choisir le bon outil

Pour chaque scénario ci-dessous : choisis entre **REST**, **SSE**, **WebSocket** et **polling**, puis **justifie en trois phrases**. La justification compte plus que la réponse. Certains scénarios admettent plusieurs bonnes réponses selon les hypothèses que tu poses — dis alors explicitement quelle hypothèse tu poses.

### Exercice 2.1 — Le catalogue `[réflexion]`

Une application mobile de vente affiche une liste de produits. Le catalogue est mis à jour une fois par jour par l'équipe commerciale.

### Exercice 2.2 — Le suivi de livraison `[réflexion]`

L'utilisateur regarde une carte où avance le vélo du livreur. La position doit se rafraîchir toutes les 2 à 3 secondes. L'utilisateur ne fait qu'observer, il n'envoie rien.

### Exercice 2.3 — Le chat de support `[réflexion]`

Un client discute avec un conseiller. Les deux tapent, les deux reçoivent, et il y a un indicateur « votre conseiller est en train d'écrire… ».

### Exercice 2.4 — Le tableau de bord `[réflexion]`

Un écran affiche 12 indicateurs de vente qui changent en continu. Il tourne en permanence sur une télévision dans les bureaux, branchée sur secteur, connectée en Ethernet.

### Exercice 2.5 — Le formulaire d'inscription `[réflexion]`

L'utilisateur remplit son adresse e-mail et son mot de passe, puis appuie sur « Créer mon compte ».

### Exercice 2.6 — Le jeu de dessin `[réflexion]`

Quatre joueurs sur mobile. L'un dessine au doigt, les trois autres voient le trait apparaître en direct et devinent le mot en tapant dans un chat.

### Exercice 2.7 — Le piège `[réflexion]`

Un développeur de ton équipe propose : « on va tout passer en WebSocket, y compris la récupération de la liste des produits, comme ça on a un seul canal et c'est plus simple ». Écris-lui une réponse argumentée. Cite au minimum trois inconvénients concrets et un cas où il aurait quand même raison.

---

## Niveau 3 — Observer une vraie connexion

Ce niveau se fait **entièrement dans ton terminal**, avec les compétences que tu as déjà. C'est le moment le plus satisfaisant : tu vas voir le protocole en vrai.

### Exercice 3.1 — Installer l'outil `[terminal]`

Installe `websocat` avec Homebrew et vérifie qu'il répond.

```sh
brew install websocat
websocat --version
```

Ensuite, réponds : où le binaire a-t-il été installé exactement ? Utilise `which websocat` pour le savoir. Le chemin obtenu est-il **absolu** ou **relatif** ? Avec `ls -l` sur ce chemin, regarde les permissions du fichier : quelle permission rend ce fichier exécutable, et pour qui ?

### Exercice 3.2 — Premier écho `[terminal]`

```sh
websocat wss://echo.websocket.org
```

Tape trois phrases, une par une, en appuyant sur Entrée à chaque fois. Observe. Puis quitte avec `Ctrl-C`.

Questions : combien de connexions as-tu ouvertes en tout ? Que ferait un équivalent HTTP de ces trois phrases, en nombre de connexions ?

### Exercice 3.3 — Voir le handshake `[terminal]`

Relance en mode verbeux :

```sh
websocat -v wss://echo.websocket.org
```

Retrouve dans la sortie les éléments de l'exercice 1.2. Note sur papier les en-têtes que tu identifies. Y en a-t-il que tu ne reconnais pas ? Cherche ce qu'ils font.

### Exercice 3.4 — Le handshake à la main avec curl `[terminal]`

Reproduis la commande `curl` de la section 14 de la leçon. Ton but est de voir la ligne `HTTP/1.1 101 Switching Protocols` de tes propres yeux.

Ensuite, la partie intéressante : **retire l'en-tête `Upgrade: websocket`** et relance. Que répond le serveur maintenant ? Explique pourquoi.

### Exercice 3.5 — Une frame binaire `[terminal]`

Envoie un fichier au serveur d'écho au lieu de taper du texte :

```sh
echo "bonjour" | websocat wss://echo.websocket.org
```

Puis compare avec le comportement interactif. Que s'est-il passé quand l'entrée standard s'est terminée ? Quel rapport avec les redirections et les tubes (`|`) que tu as vus dans le terminal ?

### Exercice 3.6 — Observer un ping `[terminal]`

Lance `websocat -v wss://echo.websocket.org` et **ne tape rien pendant deux minutes**. Regarde ce qui apparaît dans la sortie.

Vois-tu passer des `ping` ou des `pong` ? Si oui, à quel intervalle ? Si non, propose une explication : qui, du client ou du serveur, est censé les envoyer ici ?

---

## Niveau 4 — Écrire un client

**Prérequis :** ces exercices demandent JavaScript, que tu n'as pas encore appris. Fais-les **d'abord en pseudo-code ou en français**, en décrivant précisément quoi faire à chaque événement. Reviens y écrire du vrai code après la leçon sur JavaScript. Le pseudo-code d'aujourd'hui sera ta spécification de demain.

### Exercice 4.1 — Le squelette `[papier puis code]`

Écris (en français d'abord) un client qui :

1. se connecte à `wss://echo.websocket.org` ;
2. dès que la connexion est ouverte, envoie le texte `"premier message"` ;
3. affiche tout ce qu'il reçoit ;
4. affiche le code de fermeture quand la connexion se termine.

Question de contrôle avant de coder : **peux-tu appeler `send()` juste après avoir créé la socket, sur la ligne suivante ?** Réponds oui ou non, et explique pourquoi. C'est l'erreur la plus fréquente des débutants.

### Exercice 4.2 — L'ordre d'exécution `[réflexion]`

Voici un code. Sans le lancer, prédis dans quel ordre les trois lignes s'affichent, et justifie.

```js
const socket = new WebSocket("wss://echo.websocket.org");
socket.onopen = function () { console.log("B"); };
console.log("A");
socket.onmessage = function () { console.log("C"); };
```

Explique ensuite en quoi ce comportement diffère fondamentalement d'un programme en C où les instructions s'exécutent de haut en bas. C'est le cœur de la programmation asynchrone.

### Exercice 4.3 — Un compteur `[code]`

Écris un client qui envoie un message toutes les 5 secondes contenant un numéro croissant (`"message 1"`, `"message 2"`, …), affiche l'écho reçu, et s'arrête proprement après 10 envois en fermant la connexion avec le code `1000`.

Attention : où dois-tu démarrer la répétition — avant ou après l'ouverture de la connexion ?

### Exercice 4.4 — Deux clients `[terminal]`

Ouvre **deux terminaux** côte à côte et lance `websocat wss://echo.websocket.org` dans chacun.

Tape dans le premier. Le second reçoit-il quelque chose ? Explique ce que ça t'apprend sur ce qu'est un serveur d'écho, et sur ce qu'il faudrait de plus pour construire un vrai chat.

### Exercice 4.5 — Emballer plusieurs informations `[papier]`

Tu veux envoyer trois informations dans un seul message : l'auteur, le texte, l'horodatage.

Le protocole WebSocket ne transporte que du texte brut ou des octets. Propose **deux** façons différentes d'emballer ces trois informations dans une seule chaîne de caractères, et compare-les : laquelle est la plus robuste si le texte du message contient lui-même un point-virgule ou une virgule ?

Tu viens de redécouvrir tout seul le problème que résout JSON. Tu le verras en détail dans la leçon sur les API REST.

---

## Niveau 5 — Les cas difficiles

C'est le niveau qui sépare le tutoriel de la production. Fais chaque exercice **d'abord sur papier** : ce sont des problèmes de conception avant d'être des problèmes de code.

### Exercice 5.1 — Reconnexion naïve `[réflexion]`

Un développeur écrit : « quand la connexion se ferme, je me reconnecte immédiatement ».

Décris précisément ce qui se passe si le serveur tombe en panne pendant 10 minutes et que 5 000 clients appliquent cette règle. Combien de tentatives de connexion le serveur reçoit-il par seconde ? Que se passe-t-il au moment exact où il redémarre ?

### Exercice 5.2 — Le backoff `[papier]`

Écris l'algorithme de reconnexion avec backoff exponentiel, en français, pas en code. Il doit préciser :

- le délai initial ;
- comment il évolue à chaque échec ;
- le délai plafond ;
- ce qui remet le compteur à zéro ;
- pourquoi on ajoute un délai aléatoire, et de quelle amplitude.

### Exercice 5.3 — Le heartbeat applicatif `[papier]`

Ton serveur n'envoie jamais de `ping` de protocole. Conçois un heartbeat au niveau de ton application.

1. Quel message envoies-tu, et à quel intervalle ?
2. Combien de temps attends-tu la réponse avant de déclarer la connexion morte ?
3. Que fais-tu exactement quand tu la déclares morte ?
4. Le client doit-il envoyer le heartbeat, le serveur, ou les deux ? Défends ton choix.

### Exercice 5.4 — La connexion zombie `[réflexion]`

Explique pourquoi une connexion TCP peut être coupée par un équipement réseau intermédiaire **sans que ni le client ni le serveur ne soient prévenus**.

Puis : sans heartbeat, en combien de temps ton application s'en apercevrait-elle ? Qu'est-ce que l'utilisateur verrait à l'écran pendant ce temps-là ?

### Exercice 5.5 — Le tunnel `[réflexion]`

Un utilisateur lit ses messages dans le métro. Il entre dans un tunnel de 90 secondes. Pendant ce temps, quatre messages lui sont envoyés.

Décris ce qui se passe, seconde par seconde, du point de vue du **client** puis du point de vue du **serveur**. Où sont les quatre messages à la sortie du tunnel, si tu n'as rien prévu de spécial ?

### Exercice 5.6 — Ne rien perdre `[papier]`

Conçois un mécanisme qui garantit qu'aucun message n'est perdu pendant une coupure.

Réponds explicitement à : que doit stocker le client ? Que doit stocker le serveur, et pendant combien de temps ? Que s'échangent-ils exactement au moment de la reconnexion ? Que se passe-t-il si le client a été absent trois jours ?

### Exercice 5.7 — Le doublon `[réflexion]`

Le client envoie « virement de 100 € ». Le réseau tombe avant qu'il reçoive une confirmation. Il se reconnecte et, dans le doute, renvoie le message.

Combien de virements ont eu lieu ? Comment garantir qu'il n'y en ait qu'un seul, alors que le message a bel et bien été envoyé deux fois ? Cette propriété a un nom — trouve-le.

### Exercice 5.8 — La bascule WiFi `[réflexion]`

Un utilisateur sort de chez lui : son téléphone passe du WiFi à la 4G.

Que devient la connexion TCP, et pourquoi précisément ? Ton application doit-elle traiter ça comme une erreur ou comme un événement normal ? À quelle fréquence estimes-tu que ça arrive dans une journée d'usage réel ?

---

## Niveau 6 — Architecture

Ces exercices sont de la conception pure. Aucun code. Un schéma ASCII dessiné à la main est une excellente réponse — inspire-toi de ceux de la leçon.

### Exercice 6.1 — Deux serveurs `[papier]`

Alice est connectée au serveur A, Bob au serveur B, derrière un répartiteur de charge. Alice envoie un message à Bob.

Dessine le problème. Puis dessine deux solutions différentes, et compare-les sur trois critères : latence, complexité, résistance aux pannes.

### Exercice 6.2 — Le coût d'un client `[réflexion]`

Estime la mémoire consommée par un serveur pour maintenir **une** connexion WebSocket (tampons réseau, structure de connexion, données de session). Cherche des ordres de grandeur réels.

Puis calcule : combien de connexions simultanées tient une machine avec 8 Go de RAM ? Quelle autre limite système risques-tu d'atteindre avant la mémoire ? (Indice : sous Unix, une socket est un descripteur de fichier, et il y a une limite au nombre de descripteurs ouverts.)

### Exercice 6.3 — Authentifier une WebSocket `[réflexion]`

Une WebSocket, une fois ouverte, reste ouverte des heures. Comment sais-tu qui est à l'autre bout ?

Compare ces trois approches — avantages, inconvénients, risques :

| Approche | Description |
|---|---|
| Jeton dans l'URL | `wss://api.io/ws?token=abc123` |
| Cookie de session | Le navigateur l'envoie automatiquement au handshake |
| Message d'authentification | On se connecte, puis on envoie un premier message contenant le jeton |

Question supplémentaire, la plus importante : que se passe-t-il si le jeton **expire** alors que la connexion est ouverte depuis six heures ? Le serveur doit-il fermer la connexion ? Comment le client renouvelle-t-il son autorisation ?

### Exercice 6.4 — L'origine `[réflexion]`

Les WebSockets ne sont **pas** protégées par la politique de même origine (CORS) comme les requêtes HTTP classiques. N'importe quel site peut ouvrir une WebSocket vers ton serveur.

Cherche le nom de l'attaque qui exploite ça (les initiales sont CSWSH). Explique le scénario en quelques phrases, puis dis ce que le serveur doit vérifier au moment du handshake pour s'en protéger.

### Exercice 6.5 — Le déploiement `[réflexion]`

Tu déploies une nouvelle version de ton serveur. 20 000 clients y sont connectés en WebSocket.

Que leur arrive-t-il au redémarrage ? Que se passe-t-il ensuite, si tous ont implémenté la reconnexion de l'exercice 5.1 ? Propose une stratégie de déploiement qui évite la catastrophe, et compare-la à ce qui se passerait avec une API REST sans état.

### Exercice 6.6 — L'application en arrière-plan `[réflexion]`

Ton application mobile de messagerie doit prévenir l'utilisateur d'un nouveau message, même quand elle est fermée.

Dessine l'architecture complète. Elle doit répondre à : quel canal est utilisé quand l'app est au premier plan ? Quand elle est en arrière-plan ? Quand elle est complètement fermée ? Quel composant décide lequel utiliser, et comment sait-il dans quel état se trouve l'app ?

### Exercice 6.7 — Le budget batterie `[réflexion]`

Tu dois choisir l'intervalle du heartbeat de ton application mobile.

Que se passe-t-il avec 5 secondes ? Avec 10 minutes ? Quel compromis retiens-tu, et sur quel critère ? L'intervalle idéal devrait-il être le même en WiFi et en 4G ? Justifie.

---

## Auto-évaluation

Coche honnêtement. Une case non cochée n'est pas un échec, c'est simplement l'indication d'une section à relire.

**Le protocole**

- [ ] Je peux expliquer en trois phrases pourquoi HTTP ne suffit pas pour un chat.
- [ ] Je sais dire ce que signifie « full-duplex » et donner une analogie.
- [ ] Je sais que le handshake commence en HTTP et je peux citer deux en-têtes de mémoire.
- [ ] Je reconnais le code `101 Switching Protocols` et je sais ce qu'il déclenche.
- [ ] Je sais que la connexion TCP n'est pas rouverte après le handshake.
- [ ] Je connais la différence entre `ws://` et `wss://` et je sais lequel utiliser en production.

**Le fonctionnement**

- [ ] Je peux citer les quatre événements du cycle de vie.
- [ ] Je sais que je ne peux envoyer qu'après l'événement `open`.
- [ ] Je sais dans quel événement écrire la logique de reconnexion, et pourquoi pas dans l'autre.
- [ ] Je sais à quoi servent les frames `ping`/`pong`, et je peux citer leurs **deux** rôles.
- [ ] Je sais ce qu'est une connexion zombie.

**Le choix de l'outil**

- [ ] Devant un scénario, je sais choisir entre REST, SSE, WebSocket et polling, et défendre mon choix.
- [ ] Je peux citer trois cas où une WebSocket est un mauvais choix.
- [ ] Je sais pourquoi les SSE sont parfois préférables aux WebSockets.
- [ ] Je sais pourquoi REST est cacheable et pas WebSocket.

**La pratique**

- [ ] J'ai installé `websocat` et je m'y suis connecté à un serveur d'écho.
- [ ] J'ai vu de mes yeux un `101 Switching Protocols` dans mon terminal.
- [ ] Je sais lire un handshake en mode verbeux.

**Les difficultés réelles**

- [ ] Je peux expliquer le backoff exponentiel et pourquoi il faut y ajouter du hasard.
- [ ] Je sais que le protocole ne garantit aucune persistance des messages.
- [ ] Je sais pourquoi le scaling horizontal est difficile, et je connais le principe d'un bus de messages.
- [ ] Je sais ce que devient une WebSocket quand une app mobile passe en arrière-plan.
- [ ] Je sais pourquoi les notifications push sont un complément obligatoire en mobile.
- [ ] Je sais pourquoi une bascule WiFi/4G tue la connexion.

---

## Où chercher les réponses

Ce projet ne fournit **pas** de corrigés, et c'est un choix délibéré. Chercher une réponse te fait apprendre ; lire une réponse te fait oublier. Un développeur passe l'essentiel de son temps à chercher, pas à réciter.

Voici où chercher, dans l'ordre.

**1. La leçon.** [01_websocket.md](../lessons/01_websocket.md) contient de quoi répondre à la totalité des niveaux 1 et 2, et à l'essentiel des niveaux 5 et 6. Relis-la en cherchant, pas en lisant.

**2. Ton terminal.** Pour tout le niveau 3, la réponse est à l'écran. `websocat --help`, `man curl`, `which`, `ls -l` : les outils que tu connais déjà répondent.

**3. MDN Web Docs.** `developer.mozilla.org`, la référence pour tout ce qui touche au Web. Cherche « WebSocket » et « WebSockets API ». La documentation est disponible en français, mais lis l'anglais quand tu le peux : c'est la version à jour.

**4. La norme elle-même.** La RFC 6455 définit le protocole WebSocket. C'est ardu, dense, et écrit pour des implémenteurs — mais c'est **la** source de vérité. Ne la lis pas en entier : va chercher la section précise qui t'intéresse. Savoir ouvrir une RFC sans peur est une compétence de développeur.

**5. Une autre personne.** Explique ta réponse à quelqu'un, à voix haute. Si tu bafouilles, tu n'as pas compris. C'est la méthode la plus efficace de cette liste, et la plus négligée.

**Ce qu'il ne faut pas faire :** demander la réponse complète à une IA avant d'avoir sérieusement cherché. Demande-lui plutôt de vérifier une réponse que tu as déjà formulée, ou de t'expliquer le point précis qui te bloque. La différence entre les deux usages décide de ce que tu retiendras dans six mois.
