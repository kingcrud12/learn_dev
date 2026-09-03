# 4. L'architecture Flutter

Les couches côté mobile, et la vraie question : où vit l'état.

---

## 1. Le problème propre au mobile

Le backend a un travail simple : une requête entre, une réponse sort, on oublie
tout. Il est **sans mémoire** entre deux appels.

Une application mobile est l'inverse. Elle est **allumée en permanence** et
possède un état qui vit : un chargement en cours, une erreur affichée, une
permission GPS refusée, une liste déjà chargée qu'on est en train de rafraîchir.

> **La difficulté du développement mobile n'est pas de dessiner des écrans. C'est
> de savoir qui détient l'état, qui a le droit de le changer, et comment
> l'affichage en est informé.**

Tout ce qui suit répond à cette question.

## 2. Le réflexe à ne pas avoir

Voici ce qu'on écrit naturellement quand on débute en Flutter :

```dart
class _NearbyPageState extends State<NearbyPage> {
  List<Stop> stops = [];
  bool loading = false;

  Future<void> load() async {
    setState(() => loading = true);
    final pos = await Geolocator.getCurrentPosition();
    final res = await http.get(Uri.parse('https://api.../departures?lat=${pos.latitude}...'));
    setState(() {
      stops = (jsonDecode(res.body) as List).map(...).toList();
      loading = false;
    });
  }
}
```

Ça marche. C'est aussi une impasse, pour cinq raisons qui vont toutes apparaître
en même temps, vers le troisième écran :

1. **L'URL de l'API est dans un widget.** Elle sera copiée-collée ailleurs, et un
   jour il faudra la changer à sept endroits.
2. **Rien n'est testable.** Tester cette logique demande de lancer l'interface et
   d'avoir un vrai GPS.
3. **Les erreurs n'existent pas.** Pas de `catch` : la requête échoue, l'écran
   reste bloqué en chargement pour toujours.
4. **`loading` et `stops` peuvent se contredire.** Rien n'empêche
   `loading == true` avec une erreur affichée et une ancienne liste. Ce sont des
   **états impossibles**, et ils finiront par arriver.
5. **Le JSON du serveur est décodé dans l'interface.** Un champ renommé côté Go
   casse un widget.

Remarque que les points 1, 2 et 5 sont exactement les problèmes réglés côté Go
au document 2. **C'est le même principe des deux côtés** : le cœur ne doit pas
connaître le monde extérieur.

## 3. Les couches

```
┌────────────────────────────────────────┐
│  PRESENTATION                          │
│  Widgets. Dessinent, ne décident pas.  │
└──────────────────┬─────────────────────┘
                   │ écoute un état
                   ▼
┌────────────────────────────────────────┐
│  ÉTAT (state management)               │
│  Détient l'état, gère les transitions  │
└──────────────────┬─────────────────────┘
                   │ appelle
                   ▼
┌────────────────────────────────────────┐
│  DOMAINE                               │
│  Modèles purs. Aucun Flutter, aucun    │
│  JSON, aucun HTTP.                     │
└──────────────────┬─────────────────────┘
                   │ via des interfaces
                   ▼
┌────────────────────────────────────────┐
│  DATA                                  │
│  Client API, GPS, décodage JSON        │
└────────────────────────────────────────┘
```

C'est la **même architecture qu'en Go**, avec les mêmes flèches vers le centre.
Ce n'est pas une coïncidence : le principe ne dépend pas du langage.

### La structure

```
lib/
├── main.dart                      assemble et lance
│
├── core/
│   ├── result.dart                le type Result<T>
│   └── config.dart                l'URL de base, une seule fois
│
├── domain/                        ← le cœur, aucun import Flutter
│   ├── models/
│   │   ├── stop.dart
│   │   ├── departure.dart
│   │   └── position.dart
│   └── repositories/
│       ├── departures_repository.dart   (interface)
│       └── location_repository.dart     (interface)
│
├── data/                          ← le monde extérieur
│   ├── api/
│   │   ├── api_client.dart
│   │   └── dto/                   miroirs du JSON + conversion
│   └── repositories/
│       ├── departures_repository_impl.dart
│       └── location_repository_impl.dart
│
└── features/
    └── nearby/
        ├── nearby_state.dart      les états possibles
        ├── nearby_controller.dart les transitions
        └── widgets/               l'affichage
```

### Découpage par fonctionnalité

Note `features/nearby/`. Quand on ajoutera les favoris, ce sera
`features/favorites/` — et **tout** ce qui concerne les favoris sera dans ce
dossier.

C'est le même raisonnement qu'au document 2 : ranger par **sujet**, pas par
nature technique. Un dossier `widgets/` global, un `controllers/` global, et tu
retrouves le problème des `controllers/ services/ models/`.

## 4. Le domaine Flutter

Des modèles purs. Aucun `import 'package:flutter/...'`, aucun tag JSON.

```dart
// lib/domain/models/departure.dart — illustration
class Departure {
  final String lineName;
  final TransportMode mode;
  final String destination;
  final DateTime expectedTime;   // heure ABSOLUE, jamais "dans 3 min"

  const Departure({ ... });

  // Une règle métier d'affichage, calculée à la demande.
  Duration waitFrom(DateTime now) => expectedTime.difference(now);
}
```

Voilà le document 1, section 4, appliqué : le serveur a envoyé un **fait**
(`expectedTime`), et le client dérive le **rendu** quand il en a besoin. Comme
`waitFrom` prend `now` en paramètre — encore l'injection du temps — la valeur
peut être recalculée chaque seconde sans aucun appel réseau.

### Les interfaces de dépôt

Exactement les *ports* du document 2, côté Dart :

```dart
// lib/domain/repositories/departures_repository.dart
abstract interface class DeparturesRepository {
  Future<Result<List<NearbyStop>>> nearby(Position position, int radiusMeters);
}
```

Le domaine dit **ce dont il a besoin**. Le dossier `data/` fournit. Le nom ne
mentionne ni HTTP, ni JSON, ni l'URL du serveur.

## 5. Les DTO : la membrane, côté client aussi

Le JSON du serveur ne doit pas entrer dans le domaine. On refait la membrane du
paquet `prim/`, dans l'autre sens.

```dart
// lib/data/api/dto/departure_dto.dart — illustration
class DepartureDto {
  final String line;
  final String mode;
  final String destination;
  final String expectedTime;   // une chaîne : c'est ce que dit le JSON

  factory DepartureDto.fromJson(Map<String, dynamic> json) => ...;

  // La traduction, en un seul endroit du projet.
  Departure toDomain() => Departure(
        lineName: line,
        mode: TransportMode.parse(mode),
        destination: destination,
        expectedTime: DateTime.parse(expectedTime).toLocal(),
      );
}
```

Le gain : si le serveur renomme un champ, **un seul fichier change**. Aucun
widget n'est touché.

Note aussi `DateTime.parse(...).toLocal()`. La conversion de fuseau horaire se
fait **une fois, à la frontière**. Ensuite, tout le reste du code manipule des
`DateTime` locaux sans jamais y repenser. C'est le principe général : **normalise
aux frontières, pour ne plus jamais avoir à te méfier à l'intérieur.**

## 6. L'état : le vrai sujet

On arrive au cœur du développement mobile.

### Rendre les états impossibles… impossibles

Rappelle-toi le défaut n° 4 de la section 2 : `loading` et `stops` pouvaient se
contredire. La cause est structurelle — deux variables indépendantes autorisent
quatre combinaisons, dont certaines n'ont aucun sens.

La solution est de **modéliser l'état comme un seul type à plusieurs formes**,
mutuellement exclusives :

```dart
// lib/features/nearby/nearby_state.dart — illustration
sealed class NearbyState {
  const NearbyState();
}

class NearbyInitial     extends NearbyState { }
class NearbyLoading     extends NearbyState { }
class NearbyPermissionDenied extends NearbyState { }
class NearbyEmpty       extends NearbyState { }   // aucun arrêt à proximité
class NearbyError       extends NearbyState { final String message; ... }
class NearbyLoaded      extends NearbyState {
  final List<NearbyStop> stops;
  final DateTime fetchedAt;
  final bool isRefreshing;   // rechargement en fond, liste déjà visible
}
```

`sealed` garantit que la liste est fermée : le compilateur connaît **tous** les
cas possibles. On ne peut plus être « en chargement avec une erreur ».

Note `NearbyEmpty` **distinct** de `NearbyError`. « Aucun bus autour de toi à
3 h du matin » n'est pas une panne — c'est une réponse valide, qui mérite son
propre écran, calme et explicatif.

Note aussi `isRefreshing` **à l'intérieur** de `NearbyLoaded` : un
rafraîchissement ne doit pas effacer la liste pour remettre une roue qui tourne.
L'utilisateur garde ses données sous les yeux. Ce détail d'état est une décision
de **qualité perçue**.

### Le compilateur devient un allié

```dart
Widget build(BuildContext context) {
  return switch (state) {
    NearbyInitial()          => const StartView(),
    NearbyLoading()          => const LoadingView(),
    NearbyPermissionDenied() => const PermissionView(),
    NearbyEmpty()            => const EmptyView(),
    NearbyError(:final message) => ErrorView(message: message),
    NearbyLoaded(:final stops, :final isRefreshing)
                             => StopList(stops: stops, refreshing: isRefreshing),
  };
}
```

Le jour où tu ajoutes un état — un mode hors ligne, par exemple — **le
compilateur refuse de compiler** tant que tu n'as pas décidé de son affichage.
L'oubli devient impossible.

C'est l'exemple parfait d'un principe qui vaut partout : **choisis les
représentations qui font travailler le compilateur à ta place.**

### Le contrôleur

Il détient l'état et gère les transitions. Il ne connaît **que des interfaces**.

```dart
class NearbyController extends ChangeNotifier {
  final DeparturesRepository _departures;   // interfaces, jamais
  final LocationRepository _location;       // d'implémentations

  NearbyController(this._departures, this._location);

  NearbyState _state = const NearbyInitial();
  NearbyState get state => _state;

  Future<void> load({bool refresh = false}) async {
    // 1. passer en Loading, ou marquer isRefreshing si déjà chargé
    // 2. demander la position     → LocationRepository
    // 3. demander les passages    → DeparturesRepository
    // 4. produire l'état résultant : Loaded, Empty, Error ou PermissionDenied
  }
}
```

Testable sans interface, sans GPS, sans réseau : deux fausses implémentations
suffisent. **Le même critère de réussite qu'au document 2**, appliqué au mobile.

> `ChangeNotifier` est le mécanisme intégré à Flutter, sans dépendance. Riverpod
> ou Bloc apportent plus d'outillage ; ils changent la **plomberie**, pas
> l'architecture. Commence par le plus simple : tu comprendras ce que les autres
> t'apportent le jour où tu en auras besoin.

## 7. Les widgets

La règle est stricte, et se vérifie d'un coup d'œil :

> **Un widget lit un état et dessine. Il ne calcule rien, n'appelle rien, ne
> décide rien.**

| Interdit dans un widget | Où ça va |
|---|---|
| `http.get(...)` | `data/api/` |
| `jsonDecode(...)` | `data/api/dto/` |
| `Geolocator.getCurrentPosition()` | `data/repositories/` |
| Trier ou filtrer une liste | Le serveur, ou le domaine |
| Une URL en dur | `core/config.dart` |

Un widget peut en revanche **formater** — `Duration` → `"3 min"` — parce que
c'est de la présentation pure, dépendante de la langue et de l'écran.

### Petits widgets, composés

```
NearbyPage                  choisit la vue selon l'état
 └── StopList               la liste
      └── StopCard          un arrêt
           └── DepartureRow une ligne de passage
```

`DepartureRow` reçoit un `Departure` et rend une ligne. Elle ne sait rien du
reste : on peut la réutiliser dans les favoris, l'afficher dans une galerie de
composants, la tester isolément.

## 8. Les erreurs, côté client

Un rappel du document 0 : **le réseau mobile est mauvais**. Ce n'est pas un cas
exceptionnel à traiter à la fin, c'est un mode de fonctionnement normal.

Plutôt que des exceptions qui traversent les couches, un type de résultat
explicite oblige à traiter le cas :

```dart
// lib/core/result.dart — illustration
sealed class Result<T> {}
class Ok<T>  extends Result<T> { final T value; }
class Err<T> extends Result<T> { final AppError error; }
```

L'appelant **ne peut pas oublier** le cas d'échec : le `switch` ne compile pas
sans lui. Une exception, elle, s'oublie silencieusement — jusqu'à la production.

### Chaque erreur mérite son message

| Cause | Ce qu'on affiche | Action proposée |
|---|---|---|
| Permission GPS refusée | « L'application a besoin de ta position » | Ouvrir les réglages |
| Pas de réseau | « Connexion indisponible » | Réessayer |
| Serveur en panne (5xx) | « Service temporairement indisponible » | Réessayer |
| Aucun arrêt à proximité | « Aucun arrêt dans un rayon de 400 m » | Élargir la recherche |

Un message générique « Une erreur est survenue » est un aveu : l'application sait
ce qui s'est passé et refuse de le dire. **Le traitement des erreurs est une
fonctionnalité, pas de la plomberie.**

## 9. Les décisions propres au mobile

Trois choses qui n'ont pas d'équivalent côté serveur, et qu'il faut décider
maintenant.

**La permission GPS.** Elle peut être refusée, refusée définitivement, ou
révoquée pendant l'utilisation. Ce n'est pas une erreur technique : c'est un
**état de l'application** — d'où `NearbyPermissionDenied` dans le type d'état.

**Le cycle de vie.** L'application passe en arrière-plan, revient dix minutes
plus tard. Les horaires affichés sont périmés. Il faut détecter le retour au
premier plan et rafraîchir — sinon l'utilisateur lit des données mortes en les
croyant vives.

**La batterie.** Ne jamais suivre la position en continu : une seule mesure au
chargement suffit. Et arrêter tout rafraîchissement automatique quand
l'application n'est pas visible.

## 10. Résumé

1. La question centrale du mobile est **où vit l'état**.
2. Mêmes couches qu'en Go : **domaine au centre, monde extérieur en périphérie**.
3. Les **DTO** forment la membrane ; le JSON n'entre pas dans le domaine.
4. Normaliser aux frontières (fuseaux horaires, formats) pour ne plus s'en méfier
   ensuite.
5. Un **type d'état scellé** rend les états contradictoires impossibles et fait
   travailler le compilateur.
6. Un widget **dessine** ; il ne décide de rien.
7. Les erreurs sont **explicites, typées, et chacune a son message**.
8. GPS, cycle de vie et batterie sont des **contraintes d'architecture** propres
   au mobile.

---

**Suite :** [05_contrat_api.md](05_contrat_api.md) — le contrat qui relie les deux
côtés.
