# gRPC — appeler une fonction qui vit sur une autre machine

Tu viens d'écrire tes premiers programmes en C. Quand tu écris `printf("Salut\n");`,
tu appelles une fonction : tu donnes un nom, des arguments, et tu récupères
éventuellement une valeur de retour. Tout se passe dans ton programme, sur ton Mac,
dans la même mémoire.

Cette leçon parle de ce qui se passe quand la fonction que tu veux appeler n'est
**pas** dans ton programme. Elle est sur un serveur, à Paris ou en Virginie, écrite
peut-être dans un autre langage que le tien. Tu voudrais quand même l'appeler comme
si elle était locale : `envoyerMessage("bonjour")` et hop, ça marche.

C'est exactement le problème que gRPC résout.

> **Prérequis :** cette leçon suppose que tu as fait un « hello world » en C et que tu
> sais te déplacer dans un terminal (`cd`, `ls`, chemins relatifs et absolus, `chmod`).
> Elle ne suppose **pas** que tu connaisses HTTP, JSON, ni un langage de haut niveau.
> Quand une section a besoin de quelque chose que tu n'as pas encore vu, c'est signalé
> explicitement.

---

## 1. Le problème : deux machines qui ne partagent rien

Reprends ton programme C. Quand tu appelles une fonction, plusieurs choses se
passent automatiquement, et elles sont *gratuites* parce que tout est au même endroit :

```
   ton programme (un seul processus, une seule mémoire)
   ┌────────────────────────────────────────────┐
   │  main()                                    │
   │     │                                      │
   │     │  appel : addition(3, 4)              │
   │     ▼                                      │
   │  addition(int a, int b)                    │
   │     └── retourne 7 ────────────┐           │
   │                                ▼           │
   │  main() continue avec 7                    │
   └────────────────────────────────────────────┘
```

Les arguments `3` et `4` sont déjà en mémoire, dans un format que le processeur
comprend. Le compilateur connaît le type de `addition` : il sait que ça prend deux
`int` et rend un `int`. Si tu te trompes, la compilation échoue. Et l'appel est
quasi instantané.

Maintenant, mets `addition` sur une autre machine. Tout ce qui était gratuit devient
un problème :

| Ce qui était gratuit en local | Le problème quand c'est distant |
|---|---|
| Les arguments sont en mémoire | Il faut les transformer en une suite d'octets transmissible |
| Le compilateur vérifie les types | Les deux machines compilent séparément, personne ne vérifie |
| L'appel réussit toujours | Le réseau peut couper, le serveur peut être éteint |
| Le temps d'appel est négligeable | Il faut compter en millisecondes, parfois en centaines |
| Même langage des deux côtés | Le serveur est peut-être en Go, ton app en Swift |

Transformer une valeur en mémoire vers une suite d'octets s'appelle la
**sérialisation**. L'opération inverse, la **désérialisation**. C'est le cœur du
sujet : tout le reste en découle.

---

## 2. RPC : une vieille idée

**RPC** veut dire *Remote Procedure Call* — appel de procédure à distance. Le mot
« procédure » est un vieux mot pour « fonction ».

L'idée date des années 1970-1980, et elle est restée la même depuis : *faire en sorte
que l'appel distant ressemble le plus possible à un appel local*. Le programmeur écrit
`client.Addition(3, 4)` et il ne veut pas savoir qu'un paquet réseau part, traverse
l'Atlantique et revient.

Pour créer cette illusion, RPC introduit une pièce essentielle : le **stub** (parfois
appelé « souche » ou « proxy »). C'est une fonction locale, générée automatiquement,
qui a la même signature que la fonction distante, mais dont le corps ne fait pas le
calcul — il fait le voyage :

```
   CLIENT                              RÉSEAU              SERVEUR
   ──────                              ──────              ───────
   ton code
     │
     │ addition(3, 4)
     ▼
   ┌──────────┐
   │  STUB    │  sérialise 3 et 4
   │ (généré) │  ──────────────────►  octets  ──────►  ┌──────────┐
   └──────────┘                                        │  STUB    │ désérialise
        ▲                                              │ (généré) │
        │                                              └──────────┘
        │                                                   │
        │                                                   ▼
        │                                            addition(3, 4)
        │                                              vraie fonction
        │                                                   │
        │                                                   ▼
   désérialise  ◄──────  octets  ◄─────────────────  sérialise 7
        │
        ▼
   ton code reçoit 7
```

Les deux stubs sont **générés automatiquement** à partir d'une description commune.
C'est ça la clé : tu ne les écris jamais à la main. Tu écris la description, un outil
fabrique le code des deux côtés.

Il y a eu beaucoup de systèmes RPC : Sun RPC dans les années 80, CORBA dans les années
90, XML-RPC puis SOAP autour de 2000. Ils ont tous été jugés trop lourds ou trop
complexes à un moment. **gRPC** est la proposition de Google, sortie en 2015, et c'est
celle qui domine aujourd'hui dans les architectures modernes. Le « g » est
officiellement récursif et change de sens à chaque version — ne cherche pas, c'est une
blague d'ingénieurs.

---

## 3. Protocol Buffers : la description et le format binaire

gRPC repose sur **Protocol Buffers** (souvent abrégé *protobuf*). C'est deux choses
en une, et il faut bien les distinguer :

1. **Un langage de description** : tu écris un fichier `.proto` qui décrit tes données
   et tes fonctions. C'est le contrat.
2. **Un format binaire** : la façon dont ces données voyagent sur le réseau, sous forme
   d'octets compacts.

### 3.1 Le fichier .proto

Voici le plus petit exemple utile :

```proto
syntax = "proto3";

message Utilisateur {
  int32 id = 1;
  string nom = 2;
  bool actif = 3;
}
```

Un `message` est un **regroupement de champs**, exactement comme un `struct` en C.
Si tu avais écrit ça en C, tu aurais fait :

```c
struct Utilisateur {
    int   id;
    char *nom;
    int   actif;   /* pas de bool natif en C89 */
};
```

La différence importante, c'est le `= 1`, `= 2`, `= 3`. **Ce ne sont pas des valeurs
par défaut.** Ce sont des **numéros de champ**, et c'est le mécanisme central de
protobuf.

### 3.2 Pourquoi des numéros de champ

Quand protobuf sérialise ton `Utilisateur`, il n'écrit **pas** le nom des champs. Il
écrit le numéro. Compare :

Du texte lisible (ce que fait un autre format, JSON, que tu verras dans la leçon REST) :

```
{"id":42,"nom":"Zoe","actif":true}      →  34 octets
```

Du protobuf (représenté ici en hexadécimal, chaque paire = 1 octet) :

```
08 2A 12 03 5A 6F 65 18 01               →  9 octets
│  │  │  │  └──────┘  │  └── valeur : 1 (vrai)
│  │  │  │     │      └───── champ 3, type variable
│  │  │  │     └──────────── "Zoe" en ASCII
│  │  │  └────────────────── longueur : 3 octets
│  │  └───────────────────── champ 2, type "longueur préfixée"
│  └──────────────────────── valeur : 42 (0x2A)
└─────────────────────────── champ 1, type variable
```

Presque quatre fois moins d'octets. Les noms `id`, `nom`, `actif` n'apparaissent nulle
part : le récepteur sait que le champ 1 s'appelle `id` parce qu'il a **le même fichier
.proto**. Le contrat est partagé à la compilation, pas transmis à chaque message.

C'est aussi pour ça qu'une règle est absolue en protobuf : **on ne change jamais le
numéro d'un champ existant**. Tu peux renommer `nom` en `pseudo` sans rien casser (le
nom ne voyage pas), mais si tu changes son numéro de 2 à 5, tous les clients déjà
déployés liront n'importe quoi.

### 3.3 Les types de base

| Type proto | Ce que c'est | Équivalent C approximatif |
|---|---|---|
| `int32`, `int64` | entier signé | `int32_t`, `int64_t` |
| `uint32`, `uint64` | entier non signé | `uint32_t`, `uint64_t` |
| `bool` | vrai ou faux | `int` valant 0 ou 1 |
| `string` | texte, toujours en UTF-8 | `char *` (mais avec la longueur connue) |
| `bytes` | données brutes, non interprétées | `unsigned char *` + taille |
| `double`, `float` | nombre à virgule | `double`, `float` |
| `repeated X` | une liste de X, taille variable | tableau dynamique |

`repeated` remplace ce que tu ferais en C avec un pointeur et un compteur de taille.
En proto, tu écris juste `repeated string tags = 4;` et le code généré te donne une
liste utilisable directement.

---

## 4. Définir un service

Un `message` décrit des **données**. Pour décrire des **fonctions**, on utilise
`service` et `rpc` :

```proto
service Calculatrice {
  rpc Addition(RequeteAddition) returns (ReponseAddition);
}
```

Ça se lit : « le service `Calculatrice` expose une fonction `Addition` qui prend un
`RequeteAddition` et rend un `ReponseAddition` ».

Note une contrainte forte : **une méthode gRPC prend exactement un message et rend
exactement un message**. Pas deux arguments, pas zéro. C'est volontaire — ça permet
d'ajouter un champ plus tard sans casser les appelants existants. Si tu as besoin de
deux paramètres, tu les mets dans le message :

```proto
message RequeteAddition {
  int32 a = 1;
  int32 b = 2;
}

message ReponseAddition {
  int32 resultat = 1;
}
```

Cette habitude de toujours envelopper les paramètres dans un message dédié est une
bonne pratique reconnue, même quand il n'y a qu'un seul champ.

---

## 5. REST vs gRPC : la comparaison honnête

REST est l'autre grande façon de faire communiquer deux machines, et c'est de loin la
plus répandue. Tu l'étudieras dans `../../rest_api/lessons/`. Voici les vraies
différences, sans militantisme :

| Critère | REST (avec JSON) | gRPC (avec protobuf) |
|---|---|---|
| Format sur le réseau | Texte JSON, lisible à l'œil nu | Binaire, illisible sans outil |
| Contrat | Informel : documentation, convention | Formel : le fichier `.proto` fait foi |
| Vérification des types | À l'exécution, souvent jamais | À la génération de code, avant l'exécution |
| Taille des messages | Référence | Typiquement 3 à 10× plus petit |
| Vitesse de (dé)sérialisation | Analyse de texte, coûteuse | Lecture d'octets, très rapide |
| Débogage à la main | `curl` suffit, tu lis la réponse | Il faut `grpcurl` et le `.proto` |
| Support navigateur | Natif et total | Partiel, via grpc-web (voir §10) |
| Streaming | Pas prévu par le modèle | Natif, dans les deux sens |
| Courbe d'apprentissage | Douce | Plus raide : outillage à installer |
| Écosystème, tutoriels | Immense | Solide mais plus restreint |

La ligne la plus importante est celle du **contrat**. En REST, si le serveur renomme
un champ, ton app mobile ne s'en rend compte qu'au moment où un utilisateur ouvre
l'écran concerné, en production. En gRPC, le `.proto` change, tu régénères, et ton
code ne compile plus. L'erreur remonte à la compilation au lieu de remonter chez
l'utilisateur.

La ligne la plus gênante est celle du **débogage**. Avec REST, tu tapes une commande
dans ton terminal et tu lis la réponse. Avec gRPC, les octets ne veulent rien dire
sans le `.proto` sous la main. C'est un vrai coût quotidien, il faut l'assumer.

---

## 6. Les quatre types d'appels

REST propose essentiellement un modèle : une requête, une réponse. gRPC en propose
quatre, parce qu'il est construit sur HTTP/2 (voir §7) qui permet d'envoyer plusieurs
messages sur une même connexion ouverte.

### 6.1 Unaire — une requête, une réponse

C'est l'équivalent exact d'un appel de fonction classique. 90 % de ce que tu écriras.

```
CLIENT                          SERVEUR
  │                                │
  │ ──── RequeteAddition ────────► │
  │                                │  calcule
  │ ◄──── ReponseAddition ──────── │
  │                                │
  ✕ appel terminé
```

```proto
rpc Addition(RequeteAddition) returns (ReponseAddition);
```

### 6.2 Server streaming — une requête, N réponses

Le client demande une fois, le serveur répond en plusieurs morceaux, sur la même
connexion, jusqu'à ce qu'il annonce la fin.

```
CLIENT                          SERVEUR
  │                                │
  │ ──── RequeteHistorique ──────► │
  │                                │
  │ ◄──── Message #1 ───────────── │
  │ ◄──── Message #2 ───────────── │
  │ ◄──── Message #3 ───────────── │
  │ ◄──── (fin du flux) ────────── │
  ✕
```

Utile pour : télécharger une longue liste sans tout charger en mémoire, recevoir des
notifications, suivre l'avancement d'une tâche longue.

```proto
rpc Historique(RequeteHistorique) returns (stream Message);
```

### 6.3 Client streaming — N requêtes, une réponse

L'inverse. Le client envoie plusieurs messages, puis annonce qu'il a fini, et le
serveur répond une seule fois.

```
CLIENT                          SERVEUR
  │                                │
  │ ──── Morceau #1 ─────────────► │
  │ ──── Morceau #2 ─────────────► │
  │ ──── Morceau #3 ─────────────► │
  │ ──── (fin du flux) ──────────► │
  │                                │  traite l'ensemble
  │ ◄──── Confirmation ─────────── │
  ✕
```

Utile pour : envoyer un fichier par morceaux, remonter un lot de mesures de capteurs,
téléverser une photo depuis un mobile.

```proto
rpc Televerser(stream Morceau) returns (Confirmation);
```

### 6.4 Bidirectionnel — N requêtes, N réponses, dans n'importe quel ordre

Les deux flux sont indépendants. Personne n'attend l'autre.

```
CLIENT                          SERVEUR
  │                                │
  │ ──── Message A ──────────────► │
  │ ◄──── Message 1 ────────────── │
  │ ──── Message B ──────────────► │
  │ ──── Message C ──────────────► │
  │ ◄──── Message 2 ────────────── │
  │ ◄──── Message 3 ────────────── │
  │              ...               │
```

Utile pour : un chat, un jeu multijoueur, une session collaborative. C'est le cas où
gRPC concurrence directement les WebSockets (voir `../../web_socket_api/lessons/`).

> **Prérequis :** les trois modes en streaming impliquent de la **programmation
> asynchrone** — ton code ne s'arrête pas à attendre, il réagit quand un message
> arrive. C'est un vrai changement de mentalité par rapport au C séquentiel que tu
> écris aujourd'hui. Tu peux tout à fait comprendre les schémas ci-dessus maintenant,
> mais les **implémenter** demandera d'avoir vu l'asynchrone dans un langage de haut
> niveau. Reste sur l'unaire pour tes premiers essais.

---

## 7. HTTP/2, le transport

gRPC ne réinvente pas le transport : il utilise **HTTP/2**, la version 2 du protocole
du web, sortie en 2015.

> **Prérequis :** HTTP en profondeur (verbes, en-têtes, codes de statut) fait l'objet
> de la leçon REST. Ici, retiens seulement ce que HTTP/2 apporte à gRPC.

Le point décisif est le **multiplexage**. En HTTP/1.1, une connexion TCP ne traite
qu'une requête à la fois. Si tu en lances trois, la deuxième attend la première :

```
HTTP/1.1 — une connexion, en file d'attente
  ├── requête A ────────► ◄──── réponse A
  ├────────────────── attente ──────────────┐
  ├── requête B ────────► ◄──── réponse B   │  B est bloqué par A
  └── requête C ────────► ◄──── réponse C   │  C est bloqué par B
```

En HTTP/2, une seule connexion porte plusieurs **flux** (*streams*) simultanés,
identifiés par un numéro. Les octets des différents flux sont entrelacés et
réassemblés à l'arrivée :

```
HTTP/2 — une connexion, flux entrelacés
  ┌── flux 1 : requête A ──►  ◄── réponse A ──┐
  │── flux 3 : requête B ──►  ◄── réponse B ──│   tout circule en parallèle
  └── flux 5 : requête C ──►  ◄── réponse C ──┘
```

Trois conséquences directes pour gRPC :

Une seule connexion suffit pour tous les appels d'une application. Sur mobile, ouvrir
une connexion coûte cher (poignée de main TCP, puis négociation TLS, soit plusieurs
allers-retours) — en économiser, c'est économiser de la batterie.

Les en-têtes sont compressés (algorithme HPACK) au lieu d'être renvoyés en texte à
chaque requête, ce qui économise de la bande passante sur des appels répétitifs.

Les flux sont bidirectionnels par nature, ce qui rend possibles les quatre modes de la
section 6. Sans HTTP/2, gRPC n'aurait que l'unaire.

---

## 8. Un exemple complet et commenté

Voici un service de messagerie simple, avec les quatre types d'appels, entièrement
commenté. Lis-le en entier avant de continuer — c'est le fichier de référence auquel
les exercices se rapportent.

```proto
// La version de la syntaxe. Écris toujours "proto3" : proto2 est ancien
// et se comporte différemment sur les valeurs par défaut.
syntax = "proto3";

// Un espace de noms, pour éviter que ton message "Message" entre en
// collision avec celui d'une autre bibliothèque. Par convention, en
// minuscules avec des points.
package messagerie.v1;

// ---------- Les données ----------

message Utilisateur {
  string id     = 1;   // identifiant unique, choisi par le serveur
  string pseudo = 2;
  bool   enligne = 3;
}

message Message {
  string id             = 1;
  string auteur_id      = 2;  // qui a écrit
  string salon_id       = 3;  // dans quel salon
  string texte          = 4;
  int64  horodatage_ms  = 5;  // millisecondes depuis 1970 (temps Unix)
}

// ---------- Les requêtes et réponses ----------
// Un message dédié par méthode, même quand il n'a qu'un champ :
// ça permet d'en ajouter un plus tard sans casser les clients.

message EnvoyerMessageRequete {
  string salon_id = 1;
  string texte    = 2;
}

message EnvoyerMessageReponse {
  Message message = 1;  // le message tel que le serveur l'a enregistré,
                        // avec son id et son horodatage définitifs
}

message HistoriqueRequete {
  string salon_id = 1;
  int32  limite   = 2;  // combien de messages au maximum
}

message TelversementMorceau {
  bytes donnees = 1;    // un fragment du fichier
  int32 index   = 2;    // son rang, pour reconstituer dans l'ordre
}

message TelversementReponse {
  string url            = 1;
  int64  taille_octets  = 2;
}

message EvenementSalon {
  // "oneof" veut dire : un seul de ces champs est rempli à la fois.
  // C'est l'équivalent d'une union en C, mais avec l'information
  // de quel membre est actif.
  oneof evenement {
    Message      nouveau_message = 1;
    Utilisateur  arrivee         = 2;
    Utilisateur  depart          = 3;
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
  rpc TeleverserFichier(stream TelversementMorceau) returns (TelversementReponse);

  // BIDIRECTIONNEL : je reste connecté au salon. J'envoie quand je veux,
  // je reçois quand ça bouge, les deux sens sont indépendants.
  rpc Salon(stream EnvoyerMessageRequete) returns (stream EvenementSalon);
}
```

Trois détails qui méritent l'attention.

Le `package messagerie.v1` contient un numéro de version. C'est une convention très
répandue : quand tu devras faire un changement incompatible, tu créeras un
`messagerie.v2` à côté, et les deux tourneront en parallèle le temps que les vieilles
applications mobiles installées chez tes utilisateurs soient mises à jour. Tu ne
contrôles pas quand un utilisateur met à jour son app — c'est une contrainte propre au
mobile.

Le `oneof` est l'équivalent d'une `union` en C, avec en plus l'information de savoir
quel membre est actuellement rempli. Le code généré t'oblige à traiter chaque cas.

Le champ `horodatage_ms` est un `int64` et pas un `string`. Une date en texte
(« 2026-09-03T14:32:00Z ») fait une vingtaine d'octets, se compare mal et dépend du
fuseau. Un entier de millisecondes en fait huit et se compare avec `<`.

---

## 9. Le cycle complet : du .proto au code qui tourne

Voici l'enchaînement, dans l'ordre :

```
   messagerie.proto            ← 1. tu l'écris à la main
         │
         │  protoc --plugin=...  ← 2. tu lances le compilateur
         ▼
   ┌──────────────────────────────────────────┐
   │  code généré (NE JAMAIS MODIFIER)        │
   │                                          │
   │  • les structures/classes des messages   │
   │  • le stub client (fonctions à appeler)  │
   │  • la base du serveur (à compléter)      │
   └──────────────────────────────────────────┘
         │                          │
         │ 3a.                      │ 3b.
         ▼                          ▼
   ton code CLIENT            ton code SERVEUR
   appelle les stubs          remplit les méthodes vides
         │                          │
         └──────── réseau ──────────┘
                    4. ça tourne
```

`protoc` est le **compilateur protobuf**. Le parallèle avec `gcc` est direct : `gcc`
prend un `.c` et produit un exécutable ; `protoc` prend un `.proto` et produit du code
source dans le langage de ton choix.

Une commande typique ressemble à ça :

```sh
protoc --proto_path=. \
       --python_out=./genere \
       --grpc_python_out=./genere \
       messagerie.proto
```

Décomposons, chaque option compte :

| Option | Rôle |
|---|---|
| `--proto_path=.` | où chercher les `.proto`, y compris ceux importés |
| `--python_out=DOSSIER` | générer les **messages** en Python dans ce dossier |
| `--grpc_python_out=DOSSIER` | générer les **stubs de service** (plugin séparé) |
| `messagerie.proto` | le fichier d'entrée |

Il faut deux options de sortie parce que protobuf (les messages) et gRPC (les
services) sont deux projets distincts : `protoc` sait faire le premier tout seul, et
délègue le second à un plugin.

La règle absolue : **le code généré ne se modifie jamais à la main**. Il sera écrasé à
la prochaine génération. On le régénère à chaque changement du `.proto`, et beaucoup
d'équipes ne le versionnent même pas dans git — c'est un artefact de compilation, comme
un fichier `.o` en C.

> **Prérequis :** pour écrire le code client et serveur (étapes 3a et 3b), il te faut
> un langage de haut niveau. `protoc` ne génère **pas** de C — il génère du C++, du
> Python, du Go, du Java, du Kotlin, du Swift, du JavaScript et quelques autres. Tu
> peux dès maintenant écrire des `.proto` et lancer `protoc` pour **lire** le code
> généré : c'est instructif et parfaitement à ta portée. Écrire le serveur viendra
> après ton premier langage de haut niveau.

---

## 10. Choisir : gRPC, REST ou WebSocket

Les trois savent faire communiquer deux machines. Voici comment trancher.

**Choisis REST quand :**
- une API publique doit être consommée par des gens que tu ne connais pas (REST est le
  standard de fait, personne ne se plaindra) ;
- un navigateur web appelle directement ton serveur ;
- tu veux pouvoir déboguer avec `curl` et lire les réponses à l'œil nu ;
- le projet est petit et tu ne veux pas installer d'outillage.

**Choisis gRPC quand :**
- tes services internes se parlent entre eux (c'est le cas d'usage roi) ;
- une application mobile parle à ton propre backend ;
- le contrat strict et la génération de code te font gagner du temps ;
- le volume d'appels est élevé et les octets comptent ;
- tu as besoin de streaming structuré dans un sens ou dans les deux.

**Choisis WebSocket quand :**
- il faut du temps réel bidirectionnel **et** que le client est un navigateur ;
- les messages sont libres, sans schéma fixe fort ;
- tu veux le support navigateur natif sans couche de traduction.

En résumé, sous forme de tableau de décision :

| Ta situation | Le bon choix |
|---|---|
| API publique, clients inconnus | REST |
| Navigateur → ton serveur | REST ou WebSocket |
| Microservice → microservice | gRPC |
| App mobile → ton backend | gRPC (ou REST si l'équipe est déjà dessus) |
| Chat temps réel dans un navigateur | WebSocket |
| Chat temps réel dans une app mobile | gRPC bidirectionnel |
| Téléversement de gros fichiers | gRPC client streaming, ou HTTP classique |

Et un rappel de réalisme : dans la vraie vie, beaucoup d'architectures font **les
deux**. Une passerelle expose du REST vers l'extérieur, et parle gRPC aux services
internes derrière. Ce n'est pas un choix idéologique, c'est une question de contexte.

---

## 11. gRPC en mobile

Ton objectif final étant le développement mobile, cette section est celle qui te
concernera le plus.

Un téléphone n'est pas un serveur. Il a trois contraintes que gRPC adresse bien.

**La batterie.** Sur un mobile, ce qui consomme, c'est la **radio** — l'antenne
cellulaire. La réveiller coûte de l'énergie, et elle reste allumée un moment après
chaque transmission. Moins d'octets et moins de connexions, c'est directement moins de
temps d'antenne allumée. Une connexion HTTP/2 unique et persistante bat largement
plusieurs connexions HTTP/1.1 rouvertes en boucle.

**La bande passante.** En 4G bridée, dans le métro ou avec un forfait limité, un
facteur trois ou quatre sur la taille des messages se ressent. Sur un écran qui charge
cinquante éléments, la différence entre 200 Ko et 60 Ko est perceptible à l'usage.

**Le type strict.** Une application mobile déployée sur l'App Store ou le Play Store
ne se met pas à jour instantanément : des utilisateurs gardent une vieille version
pendant des mois. Un contrat `.proto` versionné et vérifié à la compilation limite
sérieusement les mauvaises surprises comparé à du JSON dont on découvre les
incompatibilités en production.

**La limite : le navigateur.** C'est le vrai point faible, et il faut le dire
clairement. gRPC a besoin d'un contrôle fin sur les trames HTTP/2 que le JavaScript
d'une page web n'a **pas** — l'API `fetch` du navigateur ne l'expose pas. Un navigateur
ne peut donc pas parler gRPC nativement.

La réponse s'appelle **grpc-web**, et c'est un compromis :

```
   Navigateur                Proxy                  Serveur
   ┌────────┐            ┌──────────┐            ┌──────────┐
   │ JS     │ grpc-web   │  Envoy   │   gRPC     │  ton     │
   │ client │ ─────────► │    ou    │ ─────────► │  service │
   │        │  (HTTP/1.1 │  autre   │  (HTTP/2   │          │
   │        │   possible)│          │   complet) │          │
   └────────┘            └──────────┘            └──────────┘
```

Il faut donc déployer et maintenir un proxy en plus. Et grpc-web ne supporte **pas** le
client streaming ni le bidirectionnel — seulement l'unaire et le server streaming.

Bonne nouvelle pour toi : **cette limite ne concerne pas les applications mobiles
natives**. Une app iOS en Swift ou une app Android en Kotlin contrôle sa pile réseau et
parle gRPC directement, sans proxy, avec les quatre modes. C'est précisément le terrain
où gRPC est le plus à l'aise.

---

## 12. Installation sur macOS

Tu es sur Apple Silicon avec zsh. Si tu n'as pas Homebrew :

```sh
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Puis :

```sh
brew install protobuf
brew install grpc
```

Vérifie que ça a marché :

```sh
protoc --version
# doit afficher quelque chose comme : libprotoc 29.x
```

Note un piège spécifique à Apple Silicon : Homebrew installe dans `/opt/homebrew`
(alors que sur les Mac Intel c'était `/usr/local`). Si `protoc --version` répond
`command not found`, c'est que `/opt/homebrew/bin` n'est pas dans ton `PATH`. Vérifie :

```sh
echo $PATH | tr ':' '\n' | grep homebrew
```

Si la commande ne renvoie rien, ajoute la ligne à ton `~/.zshrc` :

```sh
echo 'eval "$(/opt/homebrew/bin/brew shellenv)"' >> ~/.zshrc
source ~/.zshrc
```

Un dernier outil, très utile pour tester un service gRPC depuis le terminal — c'est
l'équivalent de `curl` pour gRPC :

```sh
brew install grpcurl
```

Enfin, un détail macOS que tu croiseras : les outils en ligne de commande de macOS sont
des **BSD**, pas des **GNU** comme sur Linux. `sed -i` et `grep` n'ont pas exactement
les mêmes options. Beaucoup de tutoriels gRPC sont écrits pour Linux ; si une commande
de préparation de fichiers échoue chez toi avec une erreur d'option, c'est souvent ça.
`brew install gnu-sed grep` te donne les versions GNU si besoin.

---

## Mémo

**gRPC** = appeler une fonction sur une autre machine comme si elle était locale.

**RPC** = *Remote Procedure Call*, l'idée générale ; les **stubs** générés créent
l'illusion de l'appel local.

**Protocol Buffers** = le langage de description (`.proto`) + le format binaire compact.

**Le fichier .proto** contient :
- `syntax = "proto3";` toujours en première ligne
- `package nom.v1;` pour l'espace de noms et la version
- `message` = un regroupement de champs, comme un `struct` en C
- `service` + `rpc` = les fonctions exposées
- les numéros de champ (`= 1`, `= 2`) **ne changent jamais** ; les noms, eux, peuvent
  changer sans rien casser

**Une méthode rpc** prend exactement un message et rend exactement un message.

**Les quatre modes :**

| Mode | Signature | Exemple |
|---|---|---|
| Unaire | `(A) returns (B)` | envoyer un message |
| Server streaming | `(A) returns (stream B)` | recevoir un historique |
| Client streaming | `(stream A) returns (B)` | téléverser un fichier |
| Bidirectionnel | `(stream A) returns (stream B)` | un salon de chat |

**Le cycle :** `.proto` → `protoc` → code généré (jamais modifié à la main) →
implémentation client et serveur.

**HTTP/2** apporte le multiplexage (plusieurs flux sur une connexion), la compression
des en-têtes et les flux bidirectionnels.

**Quand l'utiliser :** services internes, app mobile native vers ton backend, streaming
structuré. **Quand l'éviter :** API publique, appel direct depuis un navigateur (il
faut un proxy grpc-web, sans client streaming ni bidirectionnel).

**Installation macOS :** `brew install protobuf grpc grpcurl`, puis vérifie
`protoc --version`.

**Ce que tu peux faire dès maintenant :** écrire des `.proto`, lancer `protoc`, lire le
code généré. **Ce qui attendra :** implémenter client et serveur, qui demande un
langage de haut niveau et, pour le streaming, la programmation asynchrone.

À faire ensuite : `../exercices/grpc.md`.
