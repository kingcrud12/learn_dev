# Architecture Decision Records (ADR)

Un ADR est une note courte qui garde la trace d'**une** décision d'architecture :
ce qu'on a choisi, et surtout **pourquoi**.

## Pourquoi en écrire

Dans six mois, tu retrouveras un choix étrange dans ton propre code et tu te
demanderas s'il y avait une raison ou si c'était un accident. Sans trace, tu ne
pourras pas savoir — et tu n'oseras ni le garder ni le changer.

Un ADR répond à cette question. Il documente surtout ce qui a été **écarté** :
c'est l'information qui disparaît le plus vite, et celle qui manque le plus.

> **Le code dit ce qu'on fait. L'ADR dit pourquoi on ne fait pas autrement.**

## Le format

Court — une page maximum. Cinq sections :

```markdown
# ADR-00X — Titre à l'infinitif

**Statut :** Accepté | Remplacé par ADR-00Y
**Date :** AAAA-MM-JJ

## Contexte
La situation, les contraintes. Les faits, pas encore la décision.

## Décision
Ce qu'on fait. Une phrase claire.

## Alternatives écartées
Ce qu'on a envisagé, et la raison de l'écarter. **La section la plus utile.**

## Conséquences
Ce que ça implique — le bon comme le mauvais. Un ADR honnête liste ses coûts.
```

## Une règle importante

**Un ADR ne se modifie pas.** Il enregistre ce qu'on savait à une date donnée.

Quand une décision change, on écrit un **nouvel** ADR qui remplace l'ancien, et
on marque l'ancien comme remplacé. L'historique des décisions a autant de valeur
que la décision actuelle : il montre comment la compréhension du problème a
évolué.

## Les décisions de ce projet

| # | Décision | Statut |
|---|---|---|
| [001](001-backend-intermediaire.md) | Un backend Go entre le mobile et PRIM | Accepté |
| [002](002-architecture-hexagonale.md) | Architecture hexagonale côté Go | Accepté |
| [003](003-flutter-cross-platform.md) | Flutter plutôt que du natif | Accepté |
| [004](004-pas-de-base-de-donnees.md) | Aucune base de données en v1 | Accepté |
| [005](005-heures-absolues.md) | Transmettre des heures absolues | Accepté |
