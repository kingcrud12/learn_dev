# ADR-001 — Placer un backend Go entre l'application mobile et PRIM

**Statut :** Accepté
**Date :** 2026-09-03

## Contexte

L'application doit afficher les prochains passages autour d'une position GPS, à
partir de l'API PRIM d'Île-de-France Mobilités.

Trois faits contraignent la décision :

1. PRIM exige une **clé API** dans chaque requête.
2. L'API temps réel répond **un quai à la fois** : un écran demande 20 à 25
   appels.
3. Trouver les arrêts proches d'une coordonnée exige le **référentiel complet**
   des arrêts d'Île-de-France (~60 000 entrées), que PRIM ne fournit pas en
   temps réel.

Flutter sait faire des requêtes HTTP : appeler PRIM directement est
techniquement possible.

## Décision

L'application mobile ne parle **qu'à notre backend Go**. Elle ne connaît ni
l'existence de PRIM, ni la clé API.

## Alternatives écartées

**Appeler PRIM directement depuis Flutter.**
Écarté pour une raison rédhibitoire : une application mobile est téléchargeable
et décompilable. Une clé embarquée est publique, quelle que soit la façon dont on
la dissimule — elle doit être en clair au moment de l'envoi. La clé finirait
exposée et le compte suspendu.
S'y ajoutent 25 requêtes sur un réseau mobile médiocre, et l'impossibilité
d'embarquer un référentiel qui vieillit sans publier une mise à jour sur les
stores.

**Une fonction serverless comme simple relais.**
Réglerait le secret de la clé, mais pas l'agrégation ni le référentiel : charger
60 000 arrêts à chaque démarrage à froid est incompatible avec ce modèle. Le
référentiel a besoin d'un processus qui vit.

**Un backend-as-a-service (Firebase).**
Conviendrait pour du stockage de données utilisateur. Ici le besoin est d'appeler
une API externe et d'agréger — c'est du calcul serveur, pas du stockage.

## Conséquences

**Positif**
- La clé API reste secrète.
- Un écran = une requête pour le téléphone.
- Le référentiel se met à jour sans publier de version mobile.
- Un cache partagé entre tous les utilisateurs devient possible (facteur ~100
  sur le quota PRIM).
- Les règles métier se corrigent en minutes, sans validation d'App Store.

**Négatif**
- Un composant de plus à écrire, héberger et surveiller.
- Le backend devient un point de défaillance unique.
- Un coût d'hébergement, même faible.
- Sans lui, l'application est totalement inutilisable.

Ces coûts sont acceptés : le premier point des « négatifs » est le prix des cinq
premiers points positifs, et le premier argument est de toute façon éliminatoire.
