# 2. L'architecture du backend Go

Domaine, ports et adaptateurs — et pourquoi cette découpe précise.

---

## 1. La question centrale

Tu vas écrire quelques milliers de lignes de Go. La question n'est pas « quel
framework », c'est : **comment les ranger pour qu'on puisse encore y toucher
dans six mois ?**

Il y a deux façons de ranger du code. Elles se ressemblent sur le papier et n'ont
rien à voir en pratique.

**Par nature technique :**
```
controllers/    services/    models/    utils/
```

**Par ce que fait le code :**
```
domain/    (les règles)
adapters/  (le monde extérieur)
```

Le premier découpage semble ordonné. Il a un défaut fatal, et il vaut la peine
de le comprendre avant d'aller plus loin.

## 2. Pourquoi `controllers/ services/ models/` échoue

Prends un projet rangé ainsi. Maintenant réponds : **où est la règle « on
n'affiche pas les passages déjà partis » ?**

Tu ne peux pas savoir. Elle est peut-être dans le contrôleur, peut-être dans le
service, peut-être dans une méthode du modèle, peut-être dupliquée aux trois
endroits — c'est le cas le plus fréquent.

Le découpage technique range le code par **ce qu'il est** (« c'est un
contrôleur »), pas par **ce dont il parle** (« ça parle de passages »). Or quand
tu modifies un logiciel, tu ne penses jamais « je dois changer un contrôleur ».
Tu penses « je dois changer la règle des passages ».

> **Une structure de dossiers est bonne quand la question « où est cette
> règle ? » a une réponse évidente.**

Il y a un second défaut, plus grave. Dans cette structure, `models/` finit par
importer la bibliothèque de base de données, `services/` importe le client HTTP
de PRIM, et le métier se retrouve **soudé** à des outils. Tu ne peux plus tester
une règle sans réseau. Tu ne peux plus changer de fournisseur sans tout rouvrir.

## 3. L'idée : le monde extérieur est un détail

L'architecture hexagonale (aussi appelée *ports & adaptateurs*) part d'une
observation simple.

Dans notre projet, qu'est-ce qui est **stable** et qu'est-ce qui est
**remplaçable** ?

| Concept | Durée de vie |
|---|---|
| Un arrêt a un nom et une position | Stable — vrai depuis toujours |
| Un passage a une heure et une destination | Stable |
| « On cache les passages déjà partis » | Stable — c'est notre produit |
| PRIM répond du SIRI Lite en JSON | **Remplaçable** — peut changer demain |
| On expose du HTTP avec Gin | **Remplaçable** — gRPC, CLI, tests |
| Le référentiel est un fichier CSV | **Remplaçable** — pourrait être une base |

La colonne de droite, c'est de l'**infrastructure**. Elle change pour des raisons
qui n'ont rien à voir avec ton produit : un fournisseur qui migre, un framework
qui sort, une contrainte d'hébergement.

D'où la règle, qui est **la seule chose à retenir de ce document** :

> **Le domaine ne dépend de rien. Tout dépend du domaine.**

Les flèches d'importation pointent **vers l'intérieur**, jamais vers l'extérieur.

```
        ┌─────────────────────────────────┐
        │                                 │
        │   ADAPTATEURS (le monde)        │
        │                                 │
        │   HTTP Gin · Client PRIM ·      │
        │   Fichier CSV · Cache           │
        │                                 │
        │      ┌───────────────────┐      │
        │      │   PORTS           │      │
        │      │   (interfaces)    │      │
        │      │                   │      │
        │      │  ┌─────────────┐  │      │
        │      │  │   DOMAINE   │  │      │
        │      │  │             │  │      │
        │      │  │  Types      │  │      │
        │      │  │  Règles     │  │      │
        │      │  │             │  │      │
        │      │  │ 0 dépendance│  │      │
        │      │  └─────────────┘  │      │
        │      └───────────────────┘      │
        └─────────────────────────────────┘

           les flèches vont vers le centre
```

## 4. Le test qui prouve que c'est bien fait

Il existe un critère unique et impitoyable :

> **Peux-tu tester toutes tes règles métier sans réseau, sans fichier, sans
> serveur HTTP, en moins d'une seconde ?**

Si oui, l'architecture est correcte. Si non, quelque chose du monde extérieur a
fuité dans le domaine.

Ce test n'est pas académique. Un projet où il faut lancer un serveur et attendre
une API externe pour vérifier une règle est un projet où **plus personne n'écrit
de tests** — et donc où plus personne n'ose refactorer.

## 5. La structure de dossiers

```
backend/
├── cmd/
│   └── api/
│       └── main.go              ← assemble tout, et rien d'autre
│
├── internal/
│   ├── domain/                  ← LE CŒUR — aucun import externe
│   │   ├── stop.go              Arrêt, Position, distance
│   │   ├── departure.go         Passage, Ligne, Mode
│   │   ├── nearby.go            LA règle : trouver, grouper, trier
│   │   └── errors.go            Les erreurs du métier
│   │
│   ├── ports/                   ← LES CONTRATS (interfaces)
│   │   ├── stop_registry.go     « je sais trouver les arrêts proches »
│   │   └── realtime.go          « je sais donner les passages d'un quai »
│   │
│   ├── adapters/                ← LE MONDE EXTÉRIEUR
│   │   ├── prim/                client HTTP PRIM  (implémente realtime)
│   │   ├── registry/            référentiel CSV   (implémente stop_registry)
│   │   ├── cache/               enveloppe realtime avec un cache
│   │   └── http/                serveur Gin, handlers, DTO
│   │
│   └── config/
│       └── config.go            variables d'environnement
│
├── data/
│   └── arrets.csv               le référentiel (non commité si volumineux)
│
├── go.mod
└── .env.example                 la clé PRIM ne vit QUE dans .env
```

### Pourquoi `internal/` ?

Ce n'est pas une convention esthétique : c'est **imposé par le compilateur Go**.
Un paquet sous `internal/` ne peut être importé que par le module qui le
contient. Personne d'autre ne peut se brancher sur tes détails internes.

### Pourquoi `cmd/api/` ?

C'est le seul endroit qui a le droit de tout connaître. Son rôle unique est de
**créer les adaptateurs et de les brancher** dans le domaine. Une trentaine de
lignes, aucune logique.

## 6. Le domaine : ce qu'il contient

Le domaine contient deux choses : des **types** qui nomment le métier, et des
**fonctions** qui appliquent les règles.

Aucun tag JSON. Aucune mention de HTTP. Aucun `net/http`, aucun `gin`. Seulement
la bibliothèque standard la plus neutre (`time`, `math`, `errors`).

```go
// internal/domain/stop.go — illustration
package domain

type Position struct {
    Lat float64
    Lon float64
}

type Mode string

const (
    ModeBus   Mode = "bus"
    ModeMetro Mode = "metro"
    ModeTram  Mode = "tram"
    ModeRER   Mode = "rer"
    ModeTrain Mode = "train"
)

// Stop est une zone d'arrêt : ce que l'utilisateur appelle « un arrêt ».
type Stop struct {
    ID       StopID
    Name     string
    Position Position
    Quays    []QuayID   // les quais physiques à interroger
}
```

Note la présence de `Quays` : la tension « zone / quai » du document 0 est
**modélisée explicitement**. On ne la découvrira pas au milieu d'un handler
HTTP — elle est écrite dans le type.

### Les règles sont des fonctions pures

Une **fonction pure** ne fait que transformer ses entrées en sortie : pas de
réseau, pas d'horloge cachée, pas d'écriture. Même entrée, même sortie,
toujours.

```go
// internal/domain/nearby.go — illustration

// KeepUpcoming retire les passages déjà partis et garde les N premiers.
// L'instant présent est un PARAMÈTRE, pas un appel à time.Now().
func KeepUpcoming(deps []Departure, now time.Time, limit int) []Departure

// SortByDistance ordonne les arrêts du plus proche au plus lointain.
func SortByDistance(stops []NearbyStop)

// DistanceMeters calcule la distance entre deux points (haversine).
func DistanceMeters(a, b Position) float64
```

Regarde bien `now time.Time` en paramètre. C'est un détail qui change tout.

Si la fonction appelait `time.Now()` elle-même, elle serait **impossible à
tester** : le résultat dépendrait de l'heure à laquelle tourne le test. En
passant l'instant en argument, on peut écrire :

```go
now := time.Date(2026, 9, 3, 18, 0, 0, 0, time.UTC)
got := domain.KeepUpcoming(passages, now, 3)
// résultat déterministe, testable, éternel
```

> **L'horloge est une dépendance extérieure comme une autre.** Le réseau, les
> fichiers, le hasard et le temps : les quatre grandes sources d'imprévisibilité.
> Sors-les toutes du domaine, en les passant en paramètre.

## 7. Les ports : le domaine dicte ses besoins

Le domaine a besoin de choses qu'il ne sait pas faire : trouver des arrêts,
appeler PRIM. Il exprime ces besoins par des **interfaces** — les *ports*.

Point crucial : **c'est le domaine qui définit l'interface**, pas
l'implémentation. L'interface est écrite du point de vue du besoin métier, avec
les mots du métier.

```go
// internal/ports/realtime.go — illustration
package ports

// RealtimeSource fournit les passages temps réel d'un quai.
// Le nom ne mentionne ni PRIM, ni HTTP, ni SIRI : ce sont des détails
// de l'implémentation, pas du besoin.
type RealtimeSource interface {
    DeparturesAt(ctx context.Context, quay domain.QuayID) ([]domain.Departure, error)
}
```

Compare avec ce qu'on écrit spontanément :

| À éviter | Pourquoi | Préférer |
|---|---|---|
| `PrimClient` | Nomme le fournisseur | `RealtimeSource` |
| `FetchSiriStopMonitoring()` | Nomme le protocole | `DeparturesAt()` |
| retourne `SiriResponse` | Type du fournisseur | retourne `[]domain.Departure` |

Si demain PRIM disparaît et qu'on branche une autre source, **seul le dossier
`adapters/prim/` change**. Le domaine ne s'en aperçoit pas. C'est très exactement
ce qu'on achète.

> C'est la même idée que les headers en C, vue dans la leçon REST : `stdio.h`
> déclare `printf` sans dire comment il écrit à l'écran.

### L'inversion de dépendance, concrètement

Sans ports, le sens naturel serait : `domaine → client PRIM`. Le métier dépend
de l'outil.

Avec un port, la flèche s'inverse :

```
domaine  ──définit──▶  interface RealtimeSource
                            ▲
                            │ implémente
                    adapters/prim.Client
```

Le domaine ne connaît que l'interface **qu'il a lui-même écrite**. L'adaptateur
se plie au domaine, et jamais l'inverse.

## 8. Les adaptateurs : la traduction

Un adaptateur a **un seul travail** : traduire entre le monde extérieur et le
vocabulaire du domaine.

### `adapters/prim/` — vers PRIM

Ses responsabilités :

1. Construire l'URL et poser l'en-tête d'authentification
2. Décoder le JSON SIRI Lite, avec ses noms à rallonge
3. **Traduire** ce JSON en `[]domain.Departure`
4. Traduire les erreurs HTTP en erreurs du domaine

Le point 3 est le plus important. La réponse de PRIM ressemble à ceci :

```
Siri.ServiceDelivery.StopMonitoringDelivery[0]
    .MonitoredStopVisit[]
        .MonitoredVehicleJourney
            .MonitoredCall.ExpectedDepartureTime
            .DestinationName
            .LineRef
```

**Cette structure ne doit jamais sortir du dossier `prim/`.** Elle est décodée
dans des types privés, puis convertie en types du domaine :

```go
// interne au paquet prim, invisible ailleurs
type siriResponse struct { /* la structure biscornue */ }

func toDomain(v monitoredStopVisit) (domain.Departure, error) {
    // la traduction, en un seul endroit
}
```

> **Le paquet `prim` est une membrane.** Le JSON de PRIM entre ; des types du
> domaine sortent. Rien de la forme du fournisseur ne traverse. Si tu vois
> `MonitoredVehicleJourney` ailleurs que dans ce dossier, l'architecture fuit.

### `adapters/registry/` — le référentiel géographique

Charge le fichier des arrêts au démarrage, le garde en mémoire, et répond aux
recherches de proximité.

Pour la v1, un balayage linéaire des 60 000 arrêts suffit : c'est de l'ordre de
la milliseconde, et **une optimisation prématurée coûte plus qu'elle ne
rapporte**. Le point d'architecture est ailleurs : comme c'est derrière un port,
remplacer le balayage par une grille spatiale ou un R-tree ne touchera **aucune
autre ligne du projet**.

### `adapters/cache/` — l'enveloppe

Voici le bénéfice le plus élégant de cette architecture. Le cache **implémente le
même port** que le client PRIM, et en enveloppe un :

```go
// implémente ports.RealtimeSource, et en contient un autre
type Cached struct {
    inner ports.RealtimeSource   // le vrai client PRIM
    ttl   time.Duration
}

func (c *Cached) DeparturesAt(ctx, quay) ([]domain.Departure, error) {
    // si en cache et frais → renvoyer
    // sinon → c.inner.DeparturesAt(...), stocker, renvoyer
}
```

Puis, dans `main.go`, on empile :

```go
var realtime ports.RealtimeSource = prim.New(apiKey)
realtime = cache.Wrap(realtime, 30*time.Second)
```

**Le domaine n'a pas changé d'une ligne.** Il ne sait même pas qu'un cache
existe. On peut l'ajouter, le retirer, changer sa durée de vie, sans jamais
ouvrir un fichier du métier.

C'est ça, une bonne architecture : **les nouvelles fonctionnalités s'ajoutent
sur les bords, pas en ouvrant le centre.**

### `adapters/http/` — Gin

Gin vit **ici, et nulle part ailleurs**. Aucun autre dossier ne l'importe.

Le handler fait quatre choses, dans cet ordre :

1. Lire et valider les paramètres (`lat`, `lon`, `radius`)
2. Appeler le service du domaine
3. Convertir le résultat du domaine en **DTO** de sortie
4. Traduire l'erreur éventuelle en code HTTP

Rien d'autre. Un handler qui contient une règle métier est un bug d'architecture.

### Pourquoi des DTO séparés des types du domaine ?

C'est une question légitime : pourquoi ne pas mettre des tags JSON directement
sur `domain.Departure` et l'envoyer tel quel ? Ce serait moins de code.

Trois raisons :

1. **Les tags JSON sont une dépendance au monde extérieur.** Ton contrat d'API
   se retrouverait décidé par la forme de tes types internes.
2. **Renommer un champ du domaine casserait l'application mobile.** Avec un DTO,
   le renommage s'arrête à la couche de conversion.
3. **Le domaine contient des choses privées.** Un identifiant de quai interne n'a
   aucune raison d'être exposé.

> **Le domaine est ce que tu penses. Le DTO est ce que tu dis.** Ce ne sont pas
> les mêmes choses, et les confondre te fera changer ta pensée chaque fois que tu
> voudras changer tes mots.

## 9. Le service : là où le scénario s'écrit

Il reste une pièce : quelque chose doit **orchestrer** les trois étapes du
document 0. Ce n'est ni un handler HTTP (ce serait du métier dans l'infra), ni
une fonction pure (ça appelle le réseau).

C'est un **service applicatif**. Il vit dans le domaine et parle **uniquement**
aux ports :

```go
// internal/domain/nearby_service.go — illustration
type NearbyService struct {
    stops    ports.StopRegistry   // interfaces, jamais d'implémentations
    realtime ports.RealtimeSource
    clock    func() time.Time     // le temps, injecté lui aussi
}

func (s *NearbyService) Nearby(ctx, pos Position, radius int) ([]NearbyStop, error) {
    // 1. trouver les arrêts proches          → port StopRegistry
    // 2. interroger leurs quais en parallèle → port RealtimeSource
    // 3. grouper, filtrer, trier             → fonctions pures
}
```

Note ce qu'il ne fait **pas** : il ne construit aucune URL, ne décode aucun JSON,
ne connaît ni Gin ni PRIM. Il enchaîne des appels à des interfaces et des
fonctions pures.

Testable **entièrement** avec deux implémentations bidon en mémoire, sans
réseau : c'est le test de la section 4, et il passe.

### L'étape 2 mérite un mot

« Interroger N quais » est le passage où le Go va briller : `errgroup` permet de
lancer les appels en parallèle avec une limite de concurrence, en propageant
proprement l'annulation via le `context`.

Et une règle métier à décider **maintenant, pas dans un `catch`** : si 8 quais
sur 10 répondent, que fait-on ? Pour ce produit, on affiche les 8 — un écran
partiel vaut mieux qu'une erreur. Cette décision appartient au domaine, et elle
sera écrite là.

## 10. Le sens des flèches, vérifié

Voici le contrôle mécanique de l'architecture. Qui a le droit d'importer quoi :

| Paquet | Peut importer | Ne doit jamais importer |
|---|---|---|
| `domain/` | stdlib, `ports/` | `adapters/`, gin, net/http |
| `ports/` | stdlib, `domain/` | `adapters/`, gin |
| `adapters/*` | tout | un autre `adapters/*` |
| `cmd/api/` | tout | — |

Un `grep -r "gin-gonic" internal/domain/` qui remonte quelque chose est le signal
d'alarme. Zéro résultat : l'architecture tient.

## 11. Les erreurs : un vocabulaire du domaine

Dernier point structurant, souvent négligé.

Le domaine définit **ses** erreurs, dans ses termes :

```go
var (
    ErrNoStopNearby     = errors.New("aucun arrêt à proximité")
    ErrPositionInvalid  = errors.New("position hors zone couverte")
    ErrRealtimeUnavailable = errors.New("source temps réel indisponible")
)
```

Chaque couche **traduit** :

```
PRIM renvoie 429  →  prim/  traduit en  ErrRealtimeUnavailable
                  →  http/  traduit en  503 + un message clair
```

Le domaine ne connaît pas le code 429. Le handler ne connaît pas PRIM. Chacun ne
parle que son propre langage, et traduit à sa frontière.

## 12. Résumé

1. Ranger le code **par sujet**, jamais par nature technique.
2. **Le domaine ne dépend de rien ; tout dépend du domaine.**
3. Les **ports** sont des interfaces définies *par* le domaine, avec les mots du
   métier — jamais ceux du fournisseur.
4. Les **adaptateurs** traduisent et forment une membrane : la forme du monde
   extérieur ne traverse pas.
5. Le **temps est une dépendance** : on l'injecte, on ne l'appelle pas.
6. Les **DTO** séparent ce qu'on pense de ce qu'on dit.
7. Le critère de réussite : **tester tout le métier sans réseau, en moins d'une
   seconde**.

---

**Suite :** [03_clean_code_go.md](03_clean_code_go.md) — l'écriture du code, une
fois la structure posée.
