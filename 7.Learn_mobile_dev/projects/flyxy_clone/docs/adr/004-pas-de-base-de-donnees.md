# ADR-004 — Ne pas utiliser de base de données en v1

**Statut :** Accepté
**Date :** 2026-09-03

## Contexte

Le backend manipule deux jeux de données :

| Donnée | Volume | Change | Durée de vie utile |
|---|---|---|---|
| Référentiel des arrêts | ~60 000 entrées | Quelques fois par an | Permanente |
| Passages temps réel | Quelques dizaines par requête | En permanence | ~30 secondes |

La v1 n'a ni comptes utilisateurs, ni favoris côté serveur, ni historique.

## Décision

**Aucune base de données.**

- Le référentiel est chargé depuis un fichier au démarrage et gardé **en
  mémoire**.
- Les passages temps réel ne sont **jamais persistés** ; un cache en mémoire de
  30 secondes suffit.

## Alternatives écartées

**PostgreSQL avec PostGIS pour la recherche géographique.**
La solution correcte pour de la géo à grande échelle. Ici, un balayage linéaire
de 60 000 arrêts prend moins d'une milliseconde : la base résoudrait un problème
qu'on n'a pas, en ajoutant un service à administrer, sauvegarder et déployer.

**Redis pour le cache.**
Justifié à partir de plusieurs instances du serveur, qui doivent partager leur
cache. Avec une seule instance, une `map` protégée par un mutex fait le même
travail sans dépendance opérationnelle. À reconsidérer lors d'une mise à
l'échelle horizontale — et le port du document 2 fait que ce changement ne
touchera pas le domaine.

**SQLite embarqué.**
Éviterait le service séparé, mais n'apporte rien : il n'y a rien à persister
entre deux démarrages, et le référentiel se recharge en quelques secondes.

## Conséquences

**Positif**
- Déploiement trivial : un binaire et un fichier.
- Recherche en mémoire, donc extrêmement rapide.
- Aucun schéma à migrer, aucune sauvegarde à organiser.
- Rien à administrer.

**Négatif**
- Consommation mémoire fixe (quelques dizaines de Mo pour le référentiel).
- Démarrage un peu plus lent — acceptable pour un service qui tourne en continu.
- Le cache est perdu à chaque redémarrage.
- Plusieurs instances auraient chacune leur cache, donc plus d'appels à PRIM.

Cet ADR devra être revu si l'application acquiert des comptes utilisateurs, un
historique, ou plusieurs instances de serveur. **Ces trois évolutions ne
toucheront pas le domaine** : elles ajouteront des adaptateurs derrière des
ports existants.
