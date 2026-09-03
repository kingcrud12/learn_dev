# Découvrez les réseaux informatiques

## Le problème à résoudre

La leçon précédente décrivait une machine isolée : un processeur, de la mémoire,
un disque. Tout y était local, et tout y était fiable — quand le processeur écrit
un octet en RAM, cet octet arrive.

Un réseau change les deux hypothèses. Les données doivent traverser des
kilomètres de câbles, des équipements appartenant à des tiers, des liaisons qui
tombent. **Rien n'y est fiable par défaut.**

La question à laquelle répond cette leçon est donc :

> Comment faire parvenir une information exacte d'une machine à une autre,
> à travers un système dont aucune partie n'est fiable, sans qu'aucune machine
> ne connaisse le chemin complet ?

La réponse tient en une idée — le découpage en couches — et cette leçon la
déroule.

## Une conversation, décortiquée

Avant la théorie, observons ce qui se passe réellement quand vous tapez
`wikipedia.org` dans un navigateur.

1. Le navigateur demande : « quelle est l'adresse de `wikipedia.org` ? » Il ne le
   sait pas — il connaît un nom, pas une destination.
2. Un service lui répond une adresse numérique : `185.15.58.224`.
   *(Cette adresse peut différer chez vous et changer avec le temps : les grands
   sites en possèdent plusieurs et les font varier. Vérifiez la vôtre avec
   `dig +short wikipedia.org`.)*
3. Le navigateur ouvre une connexion vers cette adresse.
4. Il envoie une requête : « donne-moi la page d'accueil ».
5. Le serveur répond en envoyant du texte.
6. Le navigateur interprète ce texte et l'affiche.

Six étapes, et chacune fait appel à un mécanisme différent. Aucune ne connaît le
détail des autres. C'est déjà le principe des couches, à l'œuvre.

## L'idée centrale : les couches

Un réseau résout plusieurs problèmes très différents :

- comment faire passer un signal électrique dans un câble ?
- comment savoir à quelle machine du bureau s'adresse ce signal ?
- comment atteindre une machine à l'autre bout du monde ?
- comment garantir que rien ne s'est perdu en route ?
- que signifient les données transmises ?

Vouloir traiter tout cela d'un bloc serait ingérable. On **empile** donc des
couches, chacune ayant une responsabilité unique et ne parlant qu'à ses voisines
immédiates.

```
┌──────────────────────────────────────────────┐
│  4. APPLICATION   que signifient les données │  HTTP, DNS, SMTP
├──────────────────────────────────────────────┤
│  3. TRANSPORT     fiabilité, quel programme  │  TCP, UDP
├──────────────────────────────────────────────┤
│  2. RÉSEAU        atteindre n'importe où     │  IP
├──────────────────────────────────────────────┤
│  1. LIAISON       atteindre le voisin        │  Ethernet, Wi-Fi
└──────────────────────────────────────────────┘
```

C'est le **modèle TCP/IP**. Vous rencontrerez aussi le modèle **OSI**, à sept
couches, plus détaillé et plus théorique. Les deux décrivent la même réalité ;
TCP/IP correspond à ce qui est réellement implémenté.

L'intérêt de cette organisation est considérable :

**On peut remplacer une couche sans toucher aux autres.** Passer du câble au
Wi-Fi change la couche 1. Votre navigateur, en couche 4, n'en sait rien.

**Chaque couche est un problème traitable.** Personne ne comprend l'intégralité
de la pile en détail — et personne n'en a besoin.

**On peut travailler à un seul niveau.** Un développeur web vit en couche 4 et
ignore comment un signal se propage dans une fibre. Ce n'est pas de l'ignorance :
c'est le but du système.

### L'encapsulation

Chaque couche ajoute son propre en-tête aux données, comme une série
d'enveloppes emboîtées.

```
     vos données               "Bonjour"
   + en-tête HTTP          [HTTP|Bonjour]
   + en-tête TCP       [TCP|HTTP|Bonjour]
   + en-tête IP     [IP|TCP|HTTP|Bonjour]
   + en-tête Ethernet  [Eth|IP|TCP|HTTP|Bonjour|Eth]
                        └── ce qui circule dans le câble ──┘
```

À l'arrivée, chaque couche retire son enveloppe et transmet le contenu à la
couche du dessus. La machine réceptrice retrouve exactement « Bonjour ».

Chaque en-tête coûte des octets. C'est le prix de l'organisation en couches — et
il est faible au regard de ce qu'elle apporte.

## Couche 1 — La liaison : parler à son voisin

Cette couche gère la communication entre machines **directement reliées** : même
câble, même réseau Wi-Fi, même commutateur.

### L'adresse MAC

Chaque carte réseau possède un identifiant unique, gravé à la fabrication :
l'**adresse MAC**, sur 6 octets.

```
a4:83:e7:2f:1b:9c
```

Les trois premiers octets identifient le fabricant, les trois suivants
l'exemplaire. En principe, deux cartes n'ont jamais la même adresse au monde.

**Une adresse MAC ne permet pas de router.** Elle n'a aucune structure
géographique : rien dans `a4:83:e7:2f:1b:9c` n'indique où se trouve la machine.
Elle ne fonctionne qu'à l'intérieur d'un réseau local, où l'on peut se permettre
de diffuser un message à tout le monde en demandant « qui est-ce ? ».

Pour atteindre l'autre bout du monde, il faut autre chose. D'où la couche
suivante.

### Ethernet et Wi-Fi

**Ethernet** transmet dans un câble. Fiable, rapide, insensible aux
perturbations.

**Le Wi-Fi** transmet par ondes radio. Le problème nouveau est que plusieurs
machines partagent le même espace : si deux émettent en même temps, les signaux
se brouillent. Le Wi-Fi passe donc son temps à écouter avant de parler, et à
retransmettre en cas de collision.

C'est la raison pour laquelle un même débit annoncé donne des performances très
différentes selon le nombre d'utilisateurs connectés.

## Couche 2 — Le réseau : atteindre n'importe où

C'est la couche qui rend Internet possible.

### L'adresse IP

Contrairement à l'adresse MAC, l'**adresse IP** est **structurée** — et cette
structure est ce qui permet de router.

```
192.168.1.42
```

Quatre nombres de 0 à 255, soit 4 octets. C'est **IPv4**, qui offre environ
4,3 milliards d'adresses.

Ce nombre semblait immense en 1981. Il ne l'est plus : il y a aujourd'hui plus
d'appareils connectés que d'adresses disponibles. Deux réponses ont été
apportées.

**Le NAT** (traduction d'adresses). Votre box possède **une** adresse publique.
Toutes vos machines partagent des adresses **privées** derrière elle. La box
traduit dans les deux sens.

Trois plages sont réservées à cet usage privé :

- `10.0.0.0` à `10.255.255.255`
- `172.16.0.0` à `172.31.255.255`
- `192.168.0.0` à `192.168.255.255`

Si votre machine a une adresse commençant par `192.168`, elle est derrière un
NAT. Cette adresse n'est pas unique au monde — des millions de machines ont
exactement la même.

**IPv6**, la vraie solution, utilise 16 octets :

```
2001:0db8:85a3:0000:0000:8a2e:0370:7334
```

Cela fait 3,4 × 10³⁸ adresses — de quoi en attribuer plusieurs milliards à chaque
millimètre carré de la surface terrestre. Le déploiement est en cours depuis
vingt ans, en cohabitation avec IPv4.

### Le masque de sous-réseau

Une adresse IP se divise en deux parties : l'identifiant du **réseau** et celui
de la **machine** dans ce réseau. Le **masque** indique où passe la frontière.

```
adresse : 192.168.1.42
masque  : 255.255.255.0   (ou /24, soit 24 bits à 1)

          192.168.1  .  42
          └─réseau─┘   └machine┘
```

À quoi cela sert-il concrètement ? À répondre à **une seule question**, que votre
machine se pose avant chaque envoi :

> Le destinataire est-il sur mon réseau local, ou ailleurs ?

- **Sur le réseau local** → j'envoie directement, en utilisant l'adresse MAC.
- **Ailleurs** → j'envoie à la **passerelle** (votre box), qui se débrouillera.

Toute la logique de routage démarre par cette question binaire.

### Le routage

Une machine ne connaît **jamais** le chemin complet vers sa destination. Elle
connaît seulement l'équipement suivant à qui transmettre.

```
votre PC → box → réseau du FAI → opérateur → ... → serveur
```

Chaque **routeur** reçoit un paquet, regarde l'adresse de destination, consulte
sa table de routage et le transmet au routeur suivant. Il ne sait pas ce qui se
passera après.

C'est un fonctionnement de proche en proche, comparable au courrier postal :
votre facteur ignore tout du trajet de votre lettre, il sait seulement dans quel
sac la mettre.

Deux propriétés en découlent, et elles sont fondamentales.

**Aucune autorité centrale.** Internet n'a pas de tour de contrôle. Chaque
routeur décide localement.

**Le réseau contourne les pannes.** Si une liaison tombe, les routeurs
recalculent et les paquets empruntent une autre route. C'était l'objectif initial
d'ARPANET, l'ancêtre d'Internet : survivre à la destruction partielle de
l'infrastructure.

Vous pouvez observer ce chemin :

```sh
traceroute wikipedia.org
```

Chaque ligne est un routeur traversé. Vous en verrez une dizaine ou une
vingtaine, et vous constaterez que votre requête change souvent de pays.

### IP ne promet rien

Point essentiel, et contre-intuitif : le protocole IP fonctionne « au mieux »
(*best effort*). Il ne garantit **rien**.

- Un paquet peut se perdre.
- Deux paquets peuvent arriver dans le désordre.
- Un paquet peut arriver en double.
- Aucune confirmation n'est renvoyée.

C'est un choix délibéré, pas une faiblesse. Garantir la fiabilité à ce niveau
imposerait à chaque routeur de mémoriser l'état de chaque connexion — des
millions simultanément. Le réseau ne passerait pas à l'échelle.

La fiabilité est donc reléguée aux **extrémités**, dans la couche suivante. C'est
le *principe de bout en bout*, l'une des grandes décisions de conception
d'Internet.

## Couche 3 — Le transport : fiabilité et destinataire

Cette couche ajoute deux choses à IP : la notion de **programme destinataire**,
et éventuellement la **fiabilité**.

### Les ports

Une adresse IP désigne une machine. Mais une machine fait tourner plusieurs
programmes réseau simultanément : un navigateur, un client mail, une messagerie.

Le **port**, un nombre de 0 à 65535, désigne le programme.

```
185.15.58.224:443
└── machine ──┘ └port┘
```

Certains ports sont conventionnels :

| Port | Service |
|---|---|
| 22 | SSH (connexion distante) |
| 25 | SMTP (envoi de mail) |
| 53 | DNS |
| 80 | HTTP |
| 443 | HTTPS |

Rien n'oblige à les respecter — ce sont des conventions, pas des règles. Mais un
serveur web sur le port 8080 obligera les visiteurs à l'écrire explicitement.

### TCP — la fiabilité

**TCP** transforme le canal peu fiable d'IP en une communication sûre. Il
garantit :

- que tout arrive,
- dans l'ordre,
- sans doublon,
- sans corruption.

Voici comment.

**La connexion s'établit en trois temps** (*three-way handshake*) :

```
Client                          Serveur
  │  ── SYN ─────────────────────►  │   « je veux parler »
  │  ◄──────────────── SYN-ACK ──   │   « d'accord, moi aussi »
  │  ── ACK ─────────────────────►  │   « c'est noté »
  │                                 │
  │  ═══ connexion établie ═══      │
```

Les trois échanges sont nécessaires : les deux machines doivent confirmer
qu'elles peuvent **émettre et recevoir**. Deux échanges laisseraient un doute
dans un sens.

**Chaque octet est numéroté.** Le récepteur peut donc détecter un manque et
remettre les paquets dans l'ordre.

**Chaque réception est accusée.** Sans accusé de réception dans le délai imparti,
l'émetteur retransmet.

**Le débit s'adapte.** TCP commence prudemment, accélère tant que tout passe, et
ralentit brutalement dès qu'il détecte une perte — signe probable de congestion.
C'est ce mécanisme, exécuté par des milliards de connexions, qui empêche
Internet de s'effondrer aux heures de pointe.

Tout cela a un coût : le temps d'établissement, les accusés de réception, les
retransmissions. TCP est fiable, pas rapide.

### UDP — la simplicité

**UDP** fait le strict minimum : il ajoute les ports à IP, et rien d'autre.

Pas de connexion, pas d'accusé de réception, pas de remise en ordre, pas de
retransmission. On envoie, et on espère.

Pourquoi utiliserait-on un protocole qui ne garantit rien ?

**Parce que pour certains usages, la retransmission est pire que la perte.**

Dans un appel vidéo, si un paquet correspondant à 20 millisecondes d'image se
perd, le retransmettre prendrait 100 millisecondes — pendant lesquelles l'image
serait figée. Mieux vaut accepter un micro-artefact et continuer.

| | TCP | UDP |
|---|---|---|
| Connexion | oui | non |
| Fiabilité | garantie | aucune |
| Ordre | garanti | non |
| Latence | plus élevée | minimale |
| Usages | web, mail, fichiers | vidéo, voix, jeux, DNS |

Le critère de choix : **la donnée périmée a-t-elle encore de la valeur ?** Pour
un fichier, oui — il faut tout. Pour une image de visioconférence, non.

## Couche 4 — L'application : donner du sens

Les couches précédentes acheminent des octets. Celle-ci décide de ce qu'ils
signifient.

### DNS — l'annuaire

Les machines s'adressent par nombres, les humains retiennent des noms. Le **DNS**
fait la traduction.

C'est une base de données mondiale, **distribuée** : aucune machine ne contient
l'annuaire complet. La résolution se fait par étapes, du plus général au plus
précis :

```
« wikipedia.org ? »
   → serveur racine : « je ne sais pas, mais voici qui gère .org »
   → serveur .org    : « voici qui gère wikipedia.org »
   → serveur wikipedia : « c'est 185.15.58.224 »
```

Les réponses sont mises en cache à chaque niveau, ce qui évite de refaire ce
parcours à chaque visite.

Essayez :

```sh
dig wikipedia.org        # ou : nslookup wikipedia.org
```

Le DNS est un point critique de l'infrastructure : quand il tombe, tout paraît
hors service alors que les serveurs fonctionnent parfaitement. On ne sait
simplement plus où ils sont.

### HTTP — le web

Le protocole du web repose sur un échange **requête / réponse** en texte lisible.

Une requête :

```http
GET /wiki/Informatique HTTP/1.1
Host: fr.wikipedia.org
User-Agent: Mozilla/5.0
```

Une réponse :

```http
HTTP/1.1 200 OK
Content-Type: text/html; charset=UTF-8
Content-Length: 42317

<!DOCTYPE html>
<html>...
```

Le nombre `200` est le **code de statut**. Les familles à connaître :

| Code | Signification |
|---|---|
| **2xx** | succès (200 = OK) |
| **3xx** | redirection (301 = déplacé définitivement) |
| **4xx** | erreur du client (404 = introuvable, 403 = interdit) |
| **5xx** | erreur du serveur (500 = erreur interne) |

La distinction 4xx / 5xx est importante : **4xx signifie « vous avez mal
demandé », 5xx signifie « le serveur a échoué »**. Un 404 n'est pas une panne ;
un 500 en est une.

Une propriété structurante de HTTP : il est **sans état**. Chaque requête est
indépendante, le serveur ne se souvient de rien. C'est ce qui permet à un site de
répartir les requêtes sur des centaines de serveurs interchangeables — et c'est
pourquoi il a fallu inventer les cookies et les sessions pour simuler une
mémoire.

### HTTPS — le chiffrement

HTTP circule en clair. Toute machine sur le trajet peut lire — et modifier — le
contenu.

**HTTPS** ajoute une couche de chiffrement (TLS) et apporte trois garanties :

**La confidentialité.** Un intercepteur ne voit que du bruit.

**L'intégrité.** Toute modification en cours de route est détectée.

**L'authenticité.** Un **certificat**, signé par une autorité reconnue, atteste
que le serveur est bien celui qu'il prétend être. C'est ce point qui empêche
qu'un site imite votre banque.

Le cadenas du navigateur signale ces trois garanties. Il ne dit rien sur
l'honnêteté du site : un site frauduleux peut parfaitement avoir un certificat
valide. HTTPS garantit *à qui* vous parlez, pas *si cette personne est
digne de confiance*.

## Le parcours complet d'une requête

Rassemblons tout. Vous tapez `https://fr.wikipedia.org` et validez.

**1. Résolution DNS.** Le système cherche l'adresse. Il consulte d'abord son
cache local, puis interroge un serveur DNS. Réponse : `185.15.58.224`.
*(Couche 4, en UDP sur le port 53.)*

**2. Local ou distant ?** Grâce au masque de sous-réseau, la machine détermine
que cette adresse n'est pas sur le réseau local. Le paquet ira donc à la
passerelle. *(Couche 2.)*

**3. Établissement TCP.** Trois échanges avec le serveur, sur le port 443.
*(Couche 3.)*

**4. Négociation TLS.** Le serveur présente son certificat, le navigateur le
vérifie, les deux conviennent d'une clé de chiffrement.

**5. Requête HTTP.** `GET / HTTP/1.1`, chiffrée, découpée en paquets TCP,
encapsulée dans des paquets IP, puis dans des trames Ethernet ou Wi-Fi.
*(Couches 4 → 1.)*

**6. Le routage.** Chaque routeur lit l'adresse de destination et transmet au
suivant. Une quinzaine de sauts, peut-être trois pays traversés.

**7. Le serveur répond.** Le HTML redescend la pile, traverse le réseau, remonte
la pile chez vous.

**8. Le navigateur affiche.** Il interprète le HTML, découvre qu'il lui faut des
images et des feuilles de style, et **recommence** ce cycle pour chacune.

Durée totale : environ 200 millisecondes. Le temps de cligner des yeux.

Ce qui frappe, en le détaillant ainsi, n'est pas que ce soit rapide — c'est que
cela **fonctionne**, à travers des dizaines d'équipements appartenant à des
organisations différentes, sans coordination centrale.

## Les équipements

| Équipement | Couche | Rôle |
|---|---|---|
| **Câble / antenne** | 1 | transporter le signal |
| **Commutateur** (*switch*) | 1 | relier les machines d'un même réseau local |
| **Routeur** | 2 | relier des réseaux différents |
| **Pare-feu** | 2–4 | filtrer selon des règles |
| **Serveur** | 4 | fournir un service |
| **Box Internet** | 1–4 | tout cela à la fois |

Votre box est, à elle seule, un modem, un commutateur, un routeur, un point
d'accès Wi-Fi, un pare-feu, un serveur DHCP et un traducteur NAT. Sept fonctions
dans un boîtier — ce qui explique qu'elle soit si souvent la cause des problèmes.

## Ce qu'il faut retenir

- Un réseau est organisé en **couches**, chacune avec une responsabilité unique.
  C'est ce qui rend le système compréhensible et modifiable.
- **MAC** identifie une carte sur un réseau local ; **IP** identifie une machine
  n'importe où, et sa structure permet le routage.
- **IP ne garantit rien.** C'est délibéré : la fiabilité est traitée aux
  extrémités, ce qui permet au réseau de passer à l'échelle.
- **TCP** apporte la fiabilité, **UDP** la rapidité. Le critère : une donnée en
  retard a-t-elle encore de la valeur ?
- Les **ports** désignent le programme destinataire.
- Le **DNS** traduit les noms en adresses. Sans lui, tout paraît en panne.
- **HTTP** est sans état ; **HTTPS** ajoute confidentialité, intégrité et
  authenticité — mais pas la confiance.
- Aucune machine ne connaît le chemin complet. Chaque routeur ne connaît que le
  saut suivant.

---

## À faire maintenant

Observez votre propre réseau. Aucune de ces commandes ne modifie quoi que ce soit.

**Votre configuration :**

```sh
ifconfig            # macOS
ip addr             # Linux
```

Quelle est votre adresse IP ? Commence-t-elle par `192.168` ou `10.` ? Si oui,
vous êtes derrière un NAT. Repérez aussi votre adresse MAC.

**Votre passerelle :**

```sh
netstat -nr | grep default     # macOS
ip route                       # Linux
```

C'est l'adresse de votre box — le premier saut de tous vos paquets sortants.

**Votre adresse publique :**

```sh
curl ifconfig.me
```

Elle diffère de votre adresse locale. C'est celle que voient les sites que vous
visitez, et elle est partagée par toutes les machines de votre foyer.

**Tester la connectivité :**

```sh
ping -c 4 wikipedia.org
```

Notez le temps d'aller-retour. Comparez avec un serveur lointain :

```sh
ping -c 4 www.u-tokyo.ac.jp
```

L'écart correspond à la distance physique. La lumière met environ 45
millisecondes à faire Paris–Tokyo dans une fibre — vous ne pouvez pas descendre
en dessous. **La géographie est une contrainte du réseau**, et c'est pourquoi les
grands services répliquent leurs serveurs sur tous les continents.

**Suivre le chemin :**

```sh
traceroute wikipedia.org
```

Combien de routeurs traversés ? À quel saut sortez-vous du réseau de votre
opérateur ?

**Résoudre un nom :**

```sh
dig wikipedia.org
```

Relancez immédiatement la commande : la réponse arrive plus vite. C'est le cache
DNS.

**Voir vos connexions en cours :**

```sh
netstat -an | grep ESTABLISHED | head -20
```

Chaque ligne est une conversation TCP ouverte par un programme de votre machine.
Vous en aurez sans doute plus que prévu.

**Voir une requête HTTP en entier :**

```sh
curl -v http://example.com
```

Les lignes préfixées par `>` sont votre requête, celles préfixées par `<` la
réponse. Vous lisez le protocole tel qu'il circule.
