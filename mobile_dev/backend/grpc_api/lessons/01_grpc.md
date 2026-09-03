# gRPC — appeler une fonction qui vit sur une autre machine

Quand tu écris `printf("Salut\n");` en C, tu appelles une fonction : un nom, des arguments, une valeur de retour. Tout se passe dans ton programme, sur ton Mac, dans la même mémoire.

Cette leçon parle du cas où la fonction n'est **pas** dans ton programme. Elle est sur un serveur, à Paris ou en Virginie, écrite peut-être dans un autre langage. Tu voudrais quand même l'appeler comme si elle était locale : `envoyerMessage("bonjour")`, et hop. C'est le problème que gRPC résout.

> **Prérequis :** cette leçon suppose un « hello world » en C et les bases du terminal (`cd`, `ls`, chemins, `chmod`). Elle ne suppose **pas** HTTP, JSON, ni un langage de haut niveau. Quand une section a besoin de ce que tu n'as pas encore vu, c'est signalé.

---

## 1. Le problème : deux machines qui ne partagent rien

En local, un appel de fonction bénéficie de choses gratuites, parce que tout est au même endroit :

```
   ton programme (un processus, une mémoire)
   ┌──────────────────────────────────┐
   │  main()                          │
   │    │ appel : addition(3, 4)      │
   │    ▼                             │
   │  addition(int a, int b)          │
   │    └── retourne 7 ──────┐        │
   │                         ▼        │
   │  main() continue avec 7          │
   └──────────────────────────────────┘
```

Mets `addition` sur une autre machine, et tout ce qui était gratuit devient un problème :

| Gratuit en local | Le problème en distant |
|---|---|
| Les arguments sont en mémoire | Il faut les transformer en octets transmissibles |
| Le compilateur vérifie les types | Les deux machines compilent séparément |
| L'appel réussit toujours | Le réseau coupe, le serveur s'éteint |
| L'appel est instantané | On compte en millisecondes, parfois en centaines |
| Même langage des deux côtés | Le serveur est en Go, ton app en Swift |

Transformer une valeur en mémoire vers une suite d'octets s'appelle la **sérialisation** ; l'inverse, la **désérialisation**. Tout le reste en découle.

---

## 2. RPC : une vieille idée

**RPC** = *Remote Procedure Call*, appel de procédure à distance (« procédure » est un vieux mot pour « fonction »). L'idée date des années 1970-80 et n'a pas bougé : *faire en sorte que l'appel distant ressemble à un appel local*.

Pour créer cette illusion, RPC introduit le **stub** (ou « souche »). C'est une fonction locale, générée automatiquement, qui a la signature de la fonction distante mais dont le corps ne calcule pas — il voyage :

```
   CLIENT                          RÉSEAU           SERVEUR
   ──────                          ──────           ───────
   ton code
     │ addition(3, 4)
     ▼
   ┌────────┐  sérialise
   │  STUB  │ ─────────────►  octets  ────►  ┌────────┐ désérialise
   └────────┘                                │  STUB  │
       ▲                                     └────────┘
       │                                          │
       │                                          ▼
       │                                   addition(3, 4)
       │                                    vraie fonction
       │                                          │
       │ désérialise ◄── octets ◄───── sérialise 7
       ▼
   ton code reçoit 7
```

Les deux stubs sont **générés** à partir d'une description commune. Tu ne les écris jamais à la main : tu écris la description, un outil fabrique le code des deux côtés.

Il y a eu Sun RPC dans les années 80, CORBA dans les années 90, XML-RPC puis SOAP vers 2000 — tous jugés trop lourds à un moment. **gRPC** est la proposition de Google (2015), celle qui domine aujourd'hui. Le « g » est officiellement récursif et change de sens à chaque version ; c'est une blague d'ingénieurs, ne cherche pas.

---

## 3. Protocol Buffers : la description et le format binaire

gRPC repose sur **Protocol Buffers** (*protobuf*), qui est deux choses à distinguer : un **langage de description** (le fichier `.proto`, ton contrat) et un **format binaire** (la façon dont les données voyagent).

### 3.1 Le fichier .proto

```proto
syntax = "proto3";

message Utilisateur {
  int32  id    = 1;
  string nom   = 2;
  bool   actif = 3;
}
```

Un `message` regroupe des champs, exactement comme un `struct` en C :

```c
struct Utilisateur {
    int   id;
    char *nom;
    int   actif;   /* pas de bool natif en C89 */
};
```

La différence importante : `= 1`, `= 2`, `= 3` ne sont **pas des valeurs par défaut**. Ce sont des **numéros de champ**, le mécanisme central de protobuf.

### 3.2 Pourquoi des numéros

Protobuf n'écrit **pas** le nom des champs sur le réseau, seulement leur numéro. Compare avec un format texte (JSON, que tu verras en leçon REST) :

```
{"id":42,"nom":"Zoe","actif":true}        →  34 octets
```

Le même en protobuf (en hexadécimal, une paire = un octet) :

```
08 2A 12 03 5A 6F 65 18 01                →  9 octets
│  │  │  │  └──────┘  │  └── valeur : vrai
│  │  │  │     │      └───── champ 3
│  │  │  │     └──────────── "Zoe" en ASCII
│  │  │  └────────────────── longueur : 3
│  │  └───────────────────── champ 2, longueur préfixée
│  └──────────────────────── valeur : 42 (0x2A)
└─────────────────────────── champ 1
```

Presque quatre fois moins. Les noms n'apparaissent nulle part : le récepteur sait que le champ 1 s'appelle `id` parce qu'il a **le même `.proto`**. Le contrat est partagé à la compilation, pas transmis à chaque message.

D'où une règle absolue : **on ne change jamais le numéro d'un champ existant**. Renommer `nom` en `pseudo` ne casse rien (le nom ne voyage pas) ; changer son numéro de 2 à 5 fait lire n'importe quoi à tous les clients déployés.

### 3.3 Les types de base

| Type proto | Ce que c'est | Équivalent C approximatif |
|---|---|---|
| `int32`, `int64` | entier signé | `int32_t`, `int64_t` |
| `uint32`, `uint64` | entier non signé | `uint32_t`, `uint64_t` |
| `bool` | vrai ou faux | `int` valant 0 ou 1 |
| `string` | texte, toujours UTF-8 | `char *` avec longueur connue |
| `bytes` | données brutes | `unsigned char *` + taille |
| `double`, `float` | virgule flottante | `double`, `float` |
| `repeated X` | liste de X, taille variable | tableau dynamique |

`repeated` remplace ce que tu ferais en C avec un pointeur et un compteur : `repeated string tags = 4;` te donne une liste utilisable directement.

---

## 4. Définir un service

Un `message` décrit des **données** ; `service` et `rpc` décrivent des **fonctions** :

```proto
service Calculatrice {
  rpc Addition(RequeteAddition) returns (ReponseAddition);
}
```

Contrainte forte : **une méthode prend exactement un message et rend exactement un message**. C'est volontaire — ça permet d'ajouter un champ plus tard sans casser les appelants. Deux paramètres ? Mets-les dans le message :

```proto
message RequeteAddition {
  int32 a = 1;
  int32 b = 2;
}

message ReponseAddition {
  int32 resultat = 1;
}
```

Envelopper les paramètres dans un message dédié, même pour un seul champ, est une bonne pratique reconnue.

---

## 5. REST vs gRPC : la comparaison honnête

REST est l'autre grande façon de faire communiquer deux machines, et de loin la plus répandue ; tu l'étudieras dans `../../rest_api/lessons/`. Les vraies différences, sans militantisme :

| Critère | REST (avec JSON) | gRPC (avec protobuf) |
|---|---|---|
| Format réseau | Texte, lisible à l'œil | Binaire, illisible sans outil |
| Contrat | Informel : doc, convention | Formel : le `.proto` fait foi |
| Vérification des types | À l'exécution, souvent jamais | À la génération, avant l'exécution |
| Taille des messages | Référence | 3 à 10× plus petit |
| (Dé)sérialisation | Analyse de texte, coûteuse | Lecture d'octets, très rapide |
| Débogage à la main | `curl` suffit | Il faut `grpcurl` et le `.proto` |
| Support navigateur | Natif et total | Partiel, via grpc-web (§10) |
| Streaming | Pas prévu | Natif, dans les deux sens |
| Apprentissage | Doux | Plus raide, outillage à installer |
| Écosystème | Immense | Solide mais plus restreint |

La ligne décisive est celle du **contrat**. En REST, si le serveur renomme un champ, ton app mobile le découvre quand un utilisateur ouvre l'écran, en production. En gRPC, tu régénères et ton code ne compile plus : l'erreur remonte à la compilation au lieu de remonter chez l'utilisateur.

La ligne gênante est le **débogage**. Avec REST tu tapes une commande et tu lis la réponse ; avec gRPC les octets ne veulent rien dire sans le `.proto`. C'est un vrai coût quotidien, autant l'assumer.

---

## 6. Les quatre types d'appels

REST propose un modèle : une requête, une réponse. gRPC en propose quatre, parce qu'il repose sur HTTP/2 (§7) qui permet plusieurs messages sur une connexion ouverte.

### 6.1 Unaire — une requête, une réponse

L'équivalent exact d'un appel de fonction classique. 90 % de ce que tu écriras.

```
CLIENT                      SERVEUR
  │ ──── Requete ─────────►   │
  │                           │ calcule
  │ ◄──── Reponse ─────────   │
  ✕ terminé
```

```proto
rpc Addition(RequeteAddition) returns (ReponseAddition);
```

### 6.2 Server streaming — une requête, N réponses

Le client demande une fois, le serveur répond en plusieurs morceaux jusqu'à annoncer la fin.

```
CLIENT                      SERVEUR
  │ ──── Requete ─────────►   │
  │ ◄──── Message #1 ──────   │
  │ ◄──── Message #2 ──────   │
  │ ◄──── Message #3 ──────   │
  │ ◄──── (fin du flux) ───   │
  ✕
```

Pour : parcourir une longue liste sans tout charger en mémoire, recevoir des notifications, suivre une tâche longue.

```proto
rpc Historique(HistoriqueRequete) returns (stream Message);
```

### 6.3 Client streaming — N requêtes, une réponse

L'inverse : le client envoie plusieurs messages, annonce la fin, le serveur répond une fois.

```
CLIENT                      SERVEUR
  │ ──── Morceau #1 ──────►   │
  │ ──── Morceau #2 ──────►   │
  │ ──── Morceau #3 ──────►   │
  │ ──── (fin du flux) ───►   │
  │                           │ traite l'ensemble
  │ ◄──── Confirmation ────   │
  ✕
```

Pour : envoyer un fichier par morceaux, remonter un lot de mesures, téléverser une photo depuis un mobile.

```proto
rpc Televerser(stream Morceau) returns (Confirmation);
```

### 6.4 Bidirectionnel — N et N, dans n'importe quel ordre

Les deux flux sont indépendants, personne n'attend l'autre.

```
CLIENT                      SERVEUR
  │ ──── Message A ───────►   │
  │ ◄──── Message 1 ───────   │
  │ ──── Message B ───────►   │
  │ ──── Message C ───────►   │
  │ ◄──── Message 2 ───────   │
  │            ...            │
```

Pour : un chat, un jeu multijoueur, une session collaborative. C'est là que gRPC concurrence les WebSockets (`../../web_socket_api/lessons/`).

> **Prérequis :** les trois modes streaming impliquent de la **programmation asynchrone** — ton code n'attend pas, il réagit quand un message arrive. C'est un vrai changement par rapport au C séquentiel. Tu peux comprendre les schémas maintenant ; les **implémenter** demandera d'avoir vu l'asynchrone. Reste sur l'unaire pour tes premiers essais.

---

## 7. HTTP/2, le transport

gRPC ne réinvente pas le transport : il utilise **HTTP/2** (2015).

> **Prérequis :** HTTP en profondeur (verbes, en-têtes, codes de statut) est traité dans la leçon REST. Retiens ici seulement ce que HTTP/2 apporte.

Le point décisif est le **multiplexage**. En HTTP/1.1, une connexion TCP traite une requête à la fois :

```
HTTP/1.1 — une connexion, en file d'attente
  ├── requête A ───►  ◄─── réponse A
  ├── requête B ───►  ◄─── réponse B     B attend A
  └── requête C ───►  ◄─── réponse C     C attend B
```

En HTTP/2, une connexion porte plusieurs **flux** simultanés, numérotés, dont les octets sont entrelacés puis réassemblés :

```
HTTP/2 — une connexion, flux entrelacés
  ┌── flux 1 : requête A ──►  ◄── réponse A ──┐
  │── flux 3 : requête B ──►  ◄── réponse B ──│   en parallèle
  └── flux 5 : requête C ──►  ◄── réponse C ──┘
```

Trois conséquences pour gRPC. Une seule connexion suffit pour toute l'application, et sur mobile ouvrir une connexion coûte cher (poignée de main TCP puis négociation TLS, plusieurs allers-retours) — en économiser, c'est économiser de la batterie. Les en-têtes sont compressés (HPACK) au lieu d'être renvoyés en texte à chaque appel. Enfin les flux sont bidirectionnels par nature : sans HTTP/2, gRPC n'aurait que l'unaire.

---

## 8. Un exemple complet et commenté

Un service de messagerie avec les quatre types d'appels. Lis-le en entier — les exercices s'y rapportent.

```proto
// Version de la syntaxe. Toujours "proto3" : proto2 est ancien et se
// comporte différemment sur les valeurs par défaut.
syntax = "proto3";

// Espace de noms, pour éviter la collision entre ton message "Message"
// et celui d'une autre bibliothèque. Par convention en minuscules.
package messagerie.v1;

// ---------- Les données ----------

message Utilisateur {
  string id      = 1;   // identifiant unique, choisi par le serveur
  string pseudo  = 2;
  bool   enligne = 3;
}

message Message {
  string id            = 1;
  string auteur_id     = 2;  // qui a écrit
  string salon_id      = 3;  // dans quel salon
  string texte         = 4;
  int64  horodatage_ms = 5;  // millisecondes depuis 1970 (temps Unix)
}

// ---------- Requêtes et réponses ----------
// Un message dédié par méthode, même à un seul champ : on pourra en
// ajouter un plus tard sans casser les clients déjà déployés.

message EnvoyerMessageRequete {
  string salon_id = 1;
  string texte    = 2;
}

message EnvoyerMessageReponse {
  Message message = 1;  // le message tel qu'enregistré, avec son id
                        // et son horodatage définitifs
}

message HistoriqueRequete {
  string salon_id = 1;
  int32  limite   = 2;  // combien de messages au maximum
}

message TeleversementMorceau {
  bytes donnees = 1;    // un fragment du fichier
  int32 index   = 2;    // son rang, pour reconstituer l'ordre
}

message TeleversementReponse {
  string url           = 1;
  int64  taille_octets = 2;
}

message EvenementSalon {
  // "oneof" : un seul de ces champs est rempli à la fois. C'est une
  // union en C, mais avec l'info de quel membre est actif.
  oneof evenement {
    Message     nouveau_message = 1;
    Utilisateur arrivee         = 2;
    Utilisateur depart          = 3;
  }
}

// ---------- Le service ----------

service Messagerie {

  // UNAIRE : j'envoie un message, je reçois la confirmation.
  rpc EnvoyerMessage(EnvoyerMessageRequete) returns (EnvoyerMessageReponse);

  // SERVER STREAMING : je demande l'historique une fois, le serveur me
  // renvoie les messages un par un, sans tout charger en mémoire.
  rpc Historique(HistoriqueRequete) returns (stream Message);

  // CLIENT STREAMING : j'envoie un fichier par morceaux, le serveur ne
  // répond qu'une fois qu'il a tout reçu.
  rpc TeleverserFichier(stream TeleversementMorceau) returns (TeleversementReponse);

  // BIDIRECTIONNEL : je reste connecté au salon. J'envoie quand je veux,
  // je reçois quand ça bouge, les deux sens sont indépendants.
  rpc Salon(stream EnvoyerMessageRequete) returns (stream EvenementSalon);
}
```

Trois détails méritent l'attention.

Le `package messagerie.v1` porte un numéro de version. Convention répandue : le jour d'un changement incompatible, tu créeras `messagerie.v2` à côté et les deux tourneront en parallèle, le temps que les vieilles applications installées chez tes utilisateurs soient mises à jour. Tu ne contrôles pas quand un utilisateur met à jour son app — contrainte propre au mobile.

Le `oneof` est une `union` C, plus l'information de savoir quel membre est rempli ; le code généré t'oblige à traiter chaque cas.

`horodatage_ms` est un `int64`, pas un `string`. Une date en texte (« 2026-09-03T14:32:00Z ») fait une vingtaine d'octets, se compare mal et dépend du fuseau. Un entier de millisecondes en fait huit et se compare avec `<`.

---

## 9. Le cycle : du .proto au code qui tourne

```
   messagerie.proto           ← 1. tu l'écris à la main
         │
         │  protoc            ← 2. tu lances le compilateur
         ▼
   ┌────────────────────────────────────┐
   │  code généré (NE PAS MODIFIER)     │
   │   • les structures des messages    │
   │   • le stub client (à appeler)     │
   │   • la base du serveur (à remplir) │
   └────────────────────────────────────┘
         │                      │
         │ 3a.                  │ 3b.
         ▼                      ▼
   ton code CLIENT        ton code SERVEUR
   appelle les stubs      remplit les méthodes
         │                      │
         └────── réseau ────────┘
                4. ça tourne
```

`protoc` est le **compilateur protobuf**, et le parallèle avec `gcc` est direct : `gcc` prend un `.c` et produit un exécutable, `protoc` prend un `.proto` et produit du code source dans le langage de ton choix.

```sh
protoc --proto_path=. \
       --python_out=./genere \
       --grpc_python_out=./genere \
       messagerie.proto
```

| Option | Rôle |
|---|---|
| `--proto_path=.` | où chercher les `.proto`, y compris les importés |
| `--python_out=DOSSIER` | générer les **messages** dans ce dossier |
| `--grpc_python_out=DOSSIER` | générer les **stubs de service** (plugin séparé) |
| `messagerie.proto` | le fichier d'entrée |

Deux options de sortie, parce que protobuf (les messages) et gRPC (les services) sont deux projets distincts : `protoc` fait le premier seul et délègue le second à un plugin.

Règle absolue : **le code généré ne se modifie jamais à la main**, il sera écrasé à la génération suivante. On le régénère à chaque changement du `.proto`, et beaucoup d'équipes ne le versionnent même pas dans git — c'est un artefact de compilation, comme un `.o` en C.

> **Prérequis :** pour écrire le client et le serveur (3a et 3b), il te faut un langage de haut niveau. `protoc` ne génère **pas** de C : il génère du C++, Python, Go, Java, Kotlin, Swift, JavaScript et quelques autres. Dès maintenant tu peux écrire des `.proto`, lancer `protoc` et **lire** le code généré — c'est instructif et à ta portée. Écrire le serveur viendra après ton premier langage de haut niveau.

---

## 10. Choisir : gRPC, REST ou WebSocket

**REST** quand une API publique est consommée par des gens que tu ne connais pas (c'est le standard de fait), quand un navigateur appelle directement ton serveur, quand tu veux déboguer avec `curl`, ou quand le projet est petit et que tu ne veux pas installer d'outillage.

**gRPC** quand tes services internes se parlent entre eux (le cas d'usage roi), quand une app mobile parle à ton propre backend, quand le contrat strict et la génération de code te font gagner du temps, quand le volume d'appels est élevé, ou quand tu as besoin de streaming structuré.

**WebSocket** quand il faut du temps réel bidirectionnel **et** que le client est un navigateur, quand les messages sont libres sans schéma fixe, ou quand tu veux le support navigateur natif sans couche de traduction.

| Ta situation | Le bon choix |
|---|---|
| API publique, clients inconnus | REST |
| Navigateur → ton serveur | REST ou WebSocket |
| Microservice → microservice | gRPC |
| App mobile → ton backend | gRPC (ou REST si l'équipe y est déjà) |
| Chat temps réel dans un navigateur | WebSocket |
| Chat temps réel dans une app mobile | gRPC bidirectionnel |
| Téléversement de gros fichiers | gRPC client streaming, ou HTTP classique |

Rappel de réalisme : beaucoup d'architectures font **les deux**. Une passerelle expose du REST vers l'extérieur et parle gRPC aux services internes derrière. Ce n'est pas idéologique, c'est contextuel.

---

## 11. gRPC en mobile

Ton objectif final étant le mobile, voici la section qui te concernera le plus. Un téléphone a trois contraintes que gRPC adresse bien.

**La batterie.** Ce qui consomme sur un mobile, c'est la **radio**. La réveiller coûte de l'énergie et elle reste allumée un moment après chaque transmission. Moins d'octets et moins de connexions, c'est directement moins de temps d'antenne. Une connexion HTTP/2 unique et persistante bat largement plusieurs connexions HTTP/1.1 rouvertes en boucle.

**La bande passante.** En 4G bridée, dans le métro ou avec un forfait limité, un facteur trois ou quatre sur la taille se ressent. Sur un écran qui charge cinquante éléments, 200 Ko contre 60 Ko est perceptible à l'usage.

**Le type strict.** Une app déployée sur l'App Store ou le Play Store ne se met pas à jour instantanément : des utilisateurs gardent une vieille version des mois durant. Un contrat `.proto` versionné et vérifié à la compilation limite sérieusement les mauvaises surprises face à du JSON dont on découvre les incompatibilités en production.

**La limite : le navigateur.** C'est le vrai point faible. gRPC a besoin d'un contrôle fin sur les trames HTTP/2 que le JavaScript d'une page web n'a **pas** — l'API `fetch` ne l'expose pas. Un navigateur ne peut donc pas parler gRPC nativement. La réponse est **grpc-web**, un compromis :

```
   Navigateur              Proxy                Serveur
   ┌────────┐          ┌──────────┐          ┌──────────┐
   │   JS   │ grpc-web │  Envoy   │   gRPC   │   ton    │
   │ client │ ───────► │    ou    │ ───────► │ service  │
   │        │          │  autre   │ (HTTP/2  │          │
   └────────┘          └──────────┘  complet)└──────────┘
```

Il faut déployer et maintenir un proxy en plus, et grpc-web ne supporte **ni** le client streaming **ni** le bidirectionnel — seulement l'unaire et le server streaming.

Bonne nouvelle : **cette limite ne concerne pas les apps mobiles natives**. Une app iOS en Swift ou Android en Kotlin contrôle sa pile réseau, parle gRPC directement, sans proxy, avec les quatre modes. C'est le terrain où gRPC est le plus à l'aise.

---

## 12. Installation sur macOS

Sur Apple Silicon avec zsh. Si tu n'as pas Homebrew :

```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Puis les outils, et la vérification :

```sh
brew install protobuf grpc grpcurl
protoc --version        # doit afficher : libprotoc 29.x ou proche
```

`grpcurl` est l'équivalent de `curl` pour gRPC : c'est lui qui te permettra de tester un service depuis le terminal.

Piège spécifique à Apple Silicon : Homebrew installe dans `/opt/homebrew` (et non `/usr/local` comme sur les Mac Intel). Si `protoc --version` répond `command not found`, c'est que `/opt/homebrew/bin` n'est pas dans ton `PATH` :

```sh
echo $PATH | tr ':' '\n' | grep homebrew
```

Si rien ne sort, ajoute la ligne à ton `~/.zshrc` :

```sh
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zshrc
source ~/.zshrc
```

Dernier détail macOS : les outils en ligne de commande sont des **BSD**, pas des **GNU** comme sur Linux. `sed -i` et `grep` n'ont pas les mêmes options. Beaucoup de tutoriels gRPC sont écrits pour Linux ; si une commande échoue chez toi sur une option inconnue, c'est souvent ça. `brew install gnu-sed grep` te donne les versions GNU au besoin.

---

## Mémo

**gRPC** = appeler une fonction sur une autre machine comme si elle était locale.

**RPC** = *Remote Procedure Call* ; les **stubs** générés créent l'illusion de l'appel local.

**Protocol Buffers** = le langage de description (`.proto`) + le format binaire compact.

**Le `.proto` contient :**
- `syntax = "proto3";` en première ligne, toujours
- `package nom.v1;` pour l'espace de noms et la version
- `message` = un regroupement de champs, comme un `struct` en C
- `service` + `rpc` = les fonctions exposées
- les numéros de champ (`= 1`, `= 2`) **ne changent jamais** ; les noms, si

**Une méthode rpc** prend exactement un message et rend exactement un message.

| Mode | Signature | Exemple |
|---|---|---|
| Unaire | `(A) returns (B)` | envoyer un message |
| Server streaming | `(A) returns (stream B)` | recevoir un historique |
| Client streaming | `(stream A) returns (B)` | téléverser un fichier |
| Bidirectionnel | `(stream A) returns (stream B)` | un salon de chat |

**Le cycle :** `.proto` → `protoc` → code généré (jamais modifié à la main) → client et serveur.

**HTTP/2** apporte le multiplexage, la compression des en-têtes et les flux bidirectionnels.

**L'utiliser :** services internes, app mobile native vers ton backend, streaming structuré. **L'éviter :** API publique, appel direct depuis un navigateur (proxy grpc-web obligatoire, sans client streaming ni bidirectionnel).

**Installation :** `brew install protobuf grpc grpcurl`, puis `protoc --version`.

**Praticable dès maintenant :** écrire des `.proto`, lancer `protoc`, lire le code généré. **Ce qui attendra :** implémenter client et serveur (langage de haut niveau), et le streaming (asynchrone).

À faire ensuite : `../exercices/grpc.md`.
