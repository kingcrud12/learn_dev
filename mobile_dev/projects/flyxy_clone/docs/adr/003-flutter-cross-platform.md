# ADR-003 — Utiliser Flutter plutôt que du développement natif

**Statut :** Accepté
**Date :** 2026-09-03

## Contexte

L'application doit tourner sur Android et iOS. Elle a besoin de la géolocalisation
et d'appels HTTP — aucune fonctionnalité matérielle exotique.

Le projet a un seul développeur, en apprentissage.

## Décision

**Flutter**, une base de code unique pour les deux plateformes.

## Alternatives écartées

**Natif Kotlin + Swift.**
Meilleure intégration à chaque plateforme et accès immédiat aux nouveautés des
OS. Mais cela signifie **deux applications à écrire et à maintenir**, dans deux
langages et deux écosystèmes. Pour un développeur seul, c'est le double du
travail — et surtout deux fois le temps d'apprentissage, pour une application
dont aucun besoin ne justifie le natif.

**React Native.**
Choix légitime, avec un large écosystème. Écarté parce que Flutter dessine ses
propres composants plutôt que de piloter ceux du système : le rendu est plus
prévisible entre plateformes, et le pont JavaScript disparaît. Dart, typé
statiquement, offre aussi de meilleures garanties à la compilation — ce qui
compte directement pour le type d'état scellé du document 4.

**Une application web (PWA).**
Zéro publication sur les stores. Mais la géolocalisation y est moins fiable,
l'accès au capteur plus contraint, et l'objectif du projet est explicitement
**d'apprendre le développement mobile**.

## Conséquences

**Positif**
- Une seule base de code, un seul langage.
- Rendu identique sur les deux plateformes.
- Rechargement à chaud : boucle de développement très courte.
- Dart typé statiquement : le compilateur attrape les états non traités.

**Négatif**
- Taille de l'application supérieure au natif (le moteur est embarqué).
- Les nouveautés des OS arrivent avec un décalage.
- L'aspect ne sera jamais parfaitement identique à une application native.
- Les fonctionnalités système passent par des plugins, de qualité inégale.

Aucun de ces inconvénients ne touche les besoins de ce projet : afficher une
liste, lire une position GPS, appeler une API.
