# ADR-005 — Transmettre des heures absolues, jamais des durées d'attente

**Statut :** Accepté
**Date :** 2026-09-03

## Contexte

L'utilisateur veut lire « dans 3 min ». La question est de savoir **qui calcule
cette phrase** : le serveur ou l'application.

Deux faits :

- une réponse HTTP met de 100 à 800 ms à arriver sur un réseau mobile ;
- l'utilisateur garde l'écran ouvert plusieurs dizaines de secondes.

## Décision

Le serveur transmet **l'heure absolue de passage**, au format ISO 8601 avec
fuseau horaire :

```json
"expected_time": "2026-09-03T18:44:00+02:00"
```

L'application calcule l'écart avec l'instant courant et produit l'affichage.

## Alternatives écartées

**Envoyer `"wait_minutes": 3`.**
Cette valeur est fausse dès la seconde suivante. Après 400 ms de réseau et une
minute d'écran ouvert, l'application affiche « 3 min » pour un bus déjà passé.
Le seul remède serait de rappeler le serveur chaque seconde — absurde.

**Envoyer `"display": "dans 3 min"`.**
Même défaut, plus un second : la langue, le format et les cas particuliers
(« à l'approche », « à quai ») seraient figés côté serveur. Changer un libellé
demanderait un déploiement serveur pour un problème d'affichage.

**Envoyer un horodatage Unix nu.**
Fonctionnellement équivalent, mais illisible en débogage et sans indication de
fuseau. ISO 8601 se lit à l'œil dans un `curl`, et transporte le décalage horaire
explicitement — ce qui compte lors des changements d'heure.

## Conséquences

**Positif**
- L'affichage reste exact indéfiniment, sans rappeler le serveur.
- Le compte à rebours peut se rafraîchir chaque seconde, localement.
- Langue et formulation appartiennent au client, où elles doivent être.
- Le fuseau explicite élimine toute ambiguïté aux changements d'heure.

**Négatif**
- Le client doit analyser une date et gérer les fuseaux.
- Un décalage d'horloge sur le téléphone fausse l'affichage. Le champ
  `generated_at` de la réponse permettra, si besoin, de corriger cette dérive.

## Portée

Cet ADR est un cas particulier d'une règle générale du projet :

> **Une API transmet des faits, pas des rendus.** Une date, pas une chaîne
> formatée. Un code de mode, pas une icône. Un booléen, pas une couleur.

Le fait survit au temps, à la langue et à la taille de l'écran. Le rendu, non.
