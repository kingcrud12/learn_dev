# ADR-002 — Adopter une architecture hexagonale côté Go

**Statut :** Accepté
**Date :** 2026-09-03

## Contexte

Le backend combine trois natures de code très différentes :

| Nature | Change | Exemple |
|---|---|---|
| Règles métier | Quand le produit évolue | Cacher les passages déjà partis |
| Sources externes | Pour des raisons qui nous échappent | La forme du JSON de PRIM |
| Exposition | Rarement | Le serveur HTTP |

Mélanger ces trois-là rend impossible de tester une règle sans réseau, et de
changer de fournisseur sans tout rouvrir.

Par ailleurs, PRIM est un fournisseur unique dont on ne contrôle ni le format, ni
la disponibilité, ni les quotas.

## Décision

Découpage en **domaine / ports / adaptateurs** :

- `internal/domain/` — types et règles, **aucune dépendance externe**
- `internal/ports/` — interfaces définies **par** le domaine, dans son vocabulaire
- `internal/adapters/` — PRIM, référentiel CSV, cache, HTTP Gin
- `cmd/api/main.go` — le seul endroit qui connaît les implémentations concrètes

Critère de validation : **tester toutes les règles métier sans réseau ni serveur,
en moins d'une seconde.**

## Alternatives écartées

**Découpage `controllers/ services/ models/`.**
Range le code par nature technique plutôt que par sujet. La question « où est
cette règle ? » n'a alors pas de réponse évidente, et les règles finissent
dupliquées entre les trois dossiers. Pire, `models/` et `services/` finissent par
importer les bibliothèques d'infrastructure : le métier se soude aux outils.

**Tout dans un seul paquet.**
Défendable pour un prototype de 300 lignes. Ici, les frontières
« métier / PRIM / HTTP » sont réelles et connues d'avance : les matérialiser
coûte peu maintenant et beaucoup plus tard.

**Microservices.**
Un seul développeur, un seul domaine cohérent. Le coût opérationnel serait sans
rapport avec le bénéfice. Un binaire bien découpé pourra se scinder le jour où ce
sera justifié — un plat de spaghettis, non.

## Conséquences

**Positif**
- Le métier est testable sans réseau, en quelques millisecondes.
- Changer de fournisseur temps réel ne touche qu'un dossier.
- Le cache s'ajoute en enveloppant un port, **sans modifier le domaine**.
- Les frontières sont vérifiables mécaniquement (`grep` sur les imports).

**Négatif**
- Plus de fichiers et de dossiers qu'une approche directe.
- Il faut écrire des conversions entre types du domaine et types externes.
- L'indirection des interfaces demande un temps d'adaptation à la lecture.

Le coût est réel et assumé : il est payé une fois, à l'écriture, et remboursé à
chaque modification ultérieure.
