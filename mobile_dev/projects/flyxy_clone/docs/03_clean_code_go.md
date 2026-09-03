# 3. Le clean code en Go

L'architecture range les fichiers. Le clean code décide de ce qu'il y a dedans.

---

## 1. Ce que « propre » ne veut pas dire

Le clean code n'est pas une affaire de goût, d'indentation ou de nombre de lignes
par fonction. Ces règles-là sont des conséquences, pas des principes.

Le vrai critère est unique :

> **Du code propre est du code qu'on peut modifier sans le comprendre en entier.**

Tout ce qui suit découle de cette phrase. Une fonction courte est utile parce
qu'elle se comprend seule. Un bon nom est utile parce qu'il évite d'aller lire
l'implémentation. Une dépendance injectée est utile parce qu'elle rend le
comportement local et prévisible.

Et une précision qui compte, parce que Go a une culture propre : **le Go idiomatique
n'est pas le Java propre**. Beaucoup de « bonnes pratiques » venues d'ailleurs
produisent du mauvais Go. On va voir lesquelles.

## 2. Nommer

### Les noms courts sont bons — quand la portée est courte

C'est ce qui déroute le plus en arrivant d'un autre langage. En Go, la longueur
d'un nom doit être **proportionnelle à la taille de sa portée**.

```go
for i, s := range stops { ... }        // ✅ i et s vivent 3 lignes
func DistanceMeters(a, b Position)     // ✅ a et b : la symétrie dit tout
```

```go
for indexOfCurrentStop, currentStopBeingProcessed := range stops   // ❌ bruit
```

Mais un champ de structure ou une fonction exportée vit longtemps et se lit
loin de sa définition : là, le nom doit être complet.

```go
type NearbyStop struct {
    Stop           Stop
    DistanceMeters int         // ✅ l'unité est dans le nom
    Departures     []Departure
}
```

### L'unité dans le nom

`DistanceMeters`, pas `Distance`. `ttl time.Duration`, pas `timeout int`.

Une variable numérique sans unité est une invitation au bug — c'est le genre de
confusion qui a fait s'écraser une sonde martienne. Quand le type ne porte pas
l'unité, le nom doit le faire.

### Ne pas bégayer

Le nom du paquet fait déjà partie du nom à l'appel.

```go
// paquet domain
type DomainStop struct{}   // ❌ s'appelle domain.DomainStop
type Stop struct{}         // ✅ s'appelle domain.Stop
```

```go
// paquet prim
func NewPrimClient()       // ❌ prim.NewPrimClient()
func New()                 // ✅ prim.New()
```

### Les interfaces disent ce qu'elles font

Une interface décrit une **capacité**. En Go, la convention pour une interface à
une méthode est le suffixe `-er` :

```go
type RealtimeSource interface { ... }   // ✅ un rôle
type StopRegistry interface { ... }     // ✅ un rôle
type IRealtimeService interface { ... } // ❌ le préfixe I n'est pas du Go
```

## 3. Les interfaces : la règle la plus contre-intuitive

Voici la différence la plus profonde entre Go et les langages à interfaces
explicites, et elle change la façon de concevoir.

> **En Go, l'interface est définie par celui qui l'utilise, pas par celui qui
> l'implémente.**

Dans la plupart des langages, on écrit une classe puis on lui fait « implémenter »
une interface, déclarée à côté. En Go, l'implémentation est **implicite** : un
type satisfait une interface simplement en ayant les bonnes méthodes. Il n'y a
rien à déclarer.

Conséquence : **le consommateur peut définir exactement ce dont il a besoin, et
rien de plus.**

C'est précisément ce que fait notre architecture. `ports.RealtimeSource` est
déclaré **du côté du domaine**, qui en a besoin. Le paquet `prim` ne mentionne
même pas cette interface — il expose juste un type avec la bonne méthode, et ça
suffit.

### Corollaire : des interfaces minuscules

```go
// ❌ une interface fourre-tout
type TransportService interface {
    GetDepartures(...)
    GetStops(...)
    GetLines(...)
    GetDisruptions(...)
    RefreshCache(...)
}
```

Un test qui n'a besoin que de `GetDepartures` doit quand même implémenter les
cinq méthodes. Personne ne le fait ; les tests sont abandonnés.

```go
// ✅ une capacité, une interface
type RealtimeSource interface {
    DeparturesAt(ctx context.Context, quay domain.QuayID) ([]domain.Departure, error)
}
```

> **Plus une interface est petite, plus elle est utile.** L'adage de la
> communauté Go : *the bigger the interface, the weaker the abstraction*.

### Ne pas créer d'interface « au cas où »

Erreur fréquente chez qui découvre l'architecture hexagonale : mettre une
interface devant chaque type.

Une interface se justifie quand il y a **une vraie frontière** : une source de
données remplaçable, une chose à simuler dans un test. `DistanceMeters` est une
fonction pure — elle n'a rien à faire derrière une interface.

Dans ce projet, il y a **deux** ports. Pas douze.

## 4. Les erreurs

En Go, une erreur est une valeur ordinaire qu'on retourne et qu'on inspecte.

### Envelopper avec du contexte

```go
// ❌ on perd l'origine
if err != nil {
    return nil, err
}

// ❌ on perd l'erreur d'origine (impossible à inspecter ensuite)
if err != nil {
    return nil, fmt.Errorf("échec de l'appel")
}

// ✅ contexte ajouté, cause conservée grâce à %w
if err != nil {
    return nil, fmt.Errorf("interrogation du quai %s: %w", quay, err)
}
```

Le verbe `%w` conserve la chaîne : `errors.Is` et `errors.As` pourront remonter
jusqu'à la cause d'origine, même à travers cinq couches.

### Traduire aux frontières

C'est le prolongement direct de la section 11 du document précédent. Chaque
couche traduit dans son propre vocabulaire :

```
PRIM → 429 Too Many Requests
  ↓  adapters/prim traduit
domain.ErrRealtimeUnavailable
  ↓  adapters/http traduit
HTTP 503 + {"error": "service temporairement indisponible"}
```

Le domaine n'a jamais entendu parler du code 429. Le handler n'a jamais entendu
parler de PRIM.

### Ne jamais avaler une erreur

```go
result, _ := doSomething()   // ❌
```

Le `_` sur une erreur est presque toujours un bug futur. Si l'erreur est
vraiment sans importance, **un commentaire doit dire pourquoi** — sinon le
prochain lecteur (toi, dans trois mois) ne pourra pas savoir si c'était un choix
ou un oubli.

## 5. Le `context`, correctement

`context.Context` est le mécanisme d'annulation et d'échéance de Go. Dans notre
projet, il porte une chose vitale : **si l'utilisateur ferme l'application
pendant que 25 requêtes PRIM sont en vol, elles doivent s'arrêter.**

Trois règles :

1. **Premier paramètre**, toujours nommé `ctx`.
2. **On le fait passer**, on ne le stocke pas dans une structure.
3. **Toute fonction qui fait du réseau le prend.**

```go
func (c *Client) DeparturesAt(ctx context.Context, quay domain.QuayID) (...)  // ✅

type Client struct { ctx context.Context }   // ❌ jamais
```

Note que les fonctions **pures** du domaine ne prennent **pas** de contexte :
`DistanceMeters` ne peut pas être annulée. La présence d'un `ctx` dans une
signature est un signal — elle annonce « cette fonction touche au monde
extérieur ».

## 6. Les dépendances : injectées, jamais fabriquées

C'est la traduction en code de tout le document 2.

```go
// ❌ le service fabrique lui-même ses dépendances
func NewNearbyService() *NearbyService {
    return &NearbyService{
        realtime: prim.New(os.Getenv("PRIM_API_KEY")),
    }
}
```

Ce code paraît pratique. Il est en réalité **impossible à tester** : impossible
de lui donner une fausse source, il ira toujours appeler le vrai PRIM. Et le
domaine importe maintenant `prim` et `os` — la flèche pointe dans le mauvais sens.

```go
// ✅ les dépendances arrivent de l'extérieur, sous forme d'interfaces
func NewNearbyService(
    stops    ports.StopRegistry,
    realtime ports.RealtimeSource,
    clock    func() time.Time,
) *NearbyService
```

> **Une structure ne doit pas savoir d'où viennent ses dépendances.** Un seul
> endroit du programme le sait : `main.go`.

### Pas de variables globales, pas de `init()`

Un état global est une dépendance invisible : elle n'apparaît dans aucune
signature, et deux tests qui tournent en parallèle se marchent dessus. Toute
dépendance doit être **visible dans la signature**.

## 7. `main.go` : le seul endroit qui sait tout

Le point d'assemblage. Il est volontairement bête et linéaire — c'est sa qualité.

```go
func main() {
    cfg := config.Load()                              // 1. lire l'environnement

    registry := registry.LoadFromCSV(cfg.StopsFile)   // 2. construire les adaptateurs
    var realtime ports.RealtimeSource = prim.New(cfg.PrimAPIKey)
    realtime = cache.Wrap(realtime, 30*time.Second)   //    (le cache enveloppe)

    svc := domain.NewNearbyService(registry, realtime, time.Now)   // 3. brancher

    srv := httpadapter.New(svc)                       // 4. exposer
    srv.Run(cfg.Addr)
}
```

Lis les lignes 3 et 4. Le service reçoit `registry` et `realtime` **en tant
qu'interfaces**. Il ne saura jamais qu'il parle à un CSV et à PRIM à travers un
cache.

C'est là que se vérifie tout le travail d'architecture : **si tu peux changer une
ligne de `main.go` pour remplacer une source de données, sans toucher à rien
d'autre, c'est réussi.**

## 8. Les tests

### Ce qu'on teste, et à quelle intensité

| Quoi | Effort | Pourquoi |
|---|---|---|
| Fonctions pures du domaine | **Beaucoup** | Rapides, déterministes, c'est là qu'est la valeur |
| Service applicatif, avec des faux | **Beaucoup** | Vérifie les scénarios et les cas dégradés |
| Traduction dans `prim/` | **Un peu**, sur un JSON figé | La traduction est un lieu classique de bugs |
| Handlers HTTP | **Peu** | Codes de statut et validation d'entrée |
| Gin, le CSV, le réseau | **Rien** | Ce n'est pas ton code |

### Les tests en table

L'idiome Go pour tester un comportement sur plusieurs cas :

```go
func TestKeepUpcoming(t *testing.T) {
    now := time.Date(2026, 9, 3, 18, 0, 0, 0, time.UTC)

    tests := []struct {
        name  string
        input []domain.Departure
        want  int
    }{
        {"retire les passages écoulés", ..., 2},
        {"liste vide", nil, 0},
        {"tous dans le passé", ..., 0},
        {"respecte la limite", ..., 3},
    }

    for _, tc := range tests {
        t.Run(tc.name, func(t *testing.T) { ... })
    }
}
```

Ajouter un cas coûte **une ligne**. Quand ajouter un test est facile, les tests
existent.

### Les faux sont triviaux — et c'est la preuve

Grâce aux interfaces minuscules :

```go
type fakeRealtime struct {
    byQuay map[domain.QuayID][]domain.Departure
    err    error
}

func (f fakeRealtime) DeparturesAt(_ context.Context, q domain.QuayID) ([]domain.Departure, error) {
    return f.byQuay[q], f.err
}
```

Dix lignes, aucune bibliothèque de mock. **Si écrire un faux est pénible, c'est
que l'interface est trop grosse** — le test te renvoie un diagnostic
d'architecture.

## 9. Les commentaires

Un commentaire qui répète le code est du bruit qui se périmera :

```go
// incrémente i de 1
i++          // ❌
```

Un commentaire utile explique **pourquoi**, ou une contrainte extérieure :

```go
// PRIM plafonne à 20 requêtes/seconde ; on limite la concurrence
// à 10 pour garder de la marge lors des pics.
const maxConcurrentCalls = 10        // ✅
```

Le premier type d'information est dans le code. Le second n'y est **nulle part
ailleurs** — sans le commentaire, il disparaît, et quelqu'un montera la valeur à
50 en croyant optimiser.

Les commentaires de documentation, eux, commencent par le nom de l'élément :

```go
// DeparturesAt retourne les prochains passages du quai donné,
// triés par heure croissante. Retourne une liste vide si le quai
// n'est pas desservi actuellement — ce n'est pas une erreur.
```

Note la dernière phrase : elle documente un **cas limite** qu'aucune signature
ne peut exprimer. C'est exactement ce qu'un commentaire doit faire.

## 10. Ce qui est du mauvais Go venu d'ailleurs

| Réflexe importé | Pourquoi ça ne marche pas en Go |
|---|---|
| `IStopRepository` | Le préfixe `I` n'existe pas en Go |
| Une interface pour chaque struct | Les interfaces se définissent chez le consommateur, au besoin |
| Getters/setters partout | Les champs exportés sont normaux en Go |
| Hiérarchies d'héritage | Il n'y a pas d'héritage ; on compose |
| Un framework d'injection | `main.go` suffit — l'assemblage explicite est une qualité |
| `utils/`, `helpers/`, `common/` | Des noms qui ne veulent rien dire deviennent des décharges |
| Exceptions et panics pour le flux normal | Les erreurs sont des valeurs qu'on retourne |

Le dernier point de la liste des dossiers mérite un mot : **`utils/` est le
symptôme d'un domaine mal nommé.** Quand tu veux y mettre quelque chose,
demande-toi de quoi ça parle. `DistanceMeters` parle de géographie : elle va dans
`domain/`, pas dans `utils/`.

## 11. La checklist de relecture

Avant de considérer un fichier comme fini :

- [ ] `gofmt` est passé (non négociable, pas de débat de style en Go)
- [ ] `go vet` ne dit rien
- [ ] Aucun `_` sur une erreur sans commentaire justificatif
- [ ] Toutes les erreurs enveloppées avec `%w` et un contexte
- [ ] `ctx` en premier paramètre partout où il y a du réseau
- [ ] Aucun `time.Now()` dans le domaine
- [ ] Aucune dépendance construite ailleurs que dans `main.go`
- [ ] Aucune variable globale mutable
- [ ] `grep -r "gin-gonic\|net/http" internal/domain/` ne remonte rien
- [ ] Les noms exportés sont documentés, en commençant par leur nom

Les deux lignes en gras méritent d'être des vérifications automatiques plus tard
(un `make lint`), mais commence par les faire à la main : c'est comme ça qu'elles
deviennent des réflexes.

## 12. Résumé

1. Propre = **modifiable sans tout comprendre**.
2. Longueur du nom **proportionnelle à la portée** ; l'unité dans le nom.
3. Les interfaces se définissent **chez le consommateur**, et sont **minuscules**.
4. Les erreurs s'**enveloppent** avec `%w` et se **traduisent** aux frontières.
5. Les dépendances sont **injectées** ; seul `main.go` connaît le monde réel.
6. Le **temps** et le **réseau** sont des dépendances comme les autres.
7. Un faux difficile à écrire est le **symptôme** d'une interface trop grosse.
8. Les commentaires disent **pourquoi**, jamais quoi.

---

**Suite :** [04_flutter_architecture.md](04_flutter_architecture.md) — de l'autre
côté de la frontière.
