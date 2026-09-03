# Découvrir l'informatique

Module d'entrée du parcours. Il précède [1.Learn_C](../1.Learn_C/) et ne suppose
**aucune connaissance préalable**.

## Pourquoi ce module existe

Le reste du parcours part du principe que certains mots sont acquis : mémoire,
adresse, processus, octet, protocole, port. Ces mots ne sont jamais définis dans
les cours de programmation — ils sont supposés connus.

Ce module les pose. Il ne fait pas écrire une ligne de code : il installe le
vocabulaire et les mécanismes sur lesquels tout le reste s'appuie.

Deux questions le structurent :

1. **Comment une machine qui ne sait que compter fait-elle tourner un
   navigateur ?**
2. **Comment deux machines se parlent-elles à travers un système dont aucune
   partie n'est fiable ?**

## Contenu

| Leçon | Exercices | Sujet |
|---|---|---|
| [Découvrez l'ordinateur](lessons/decouvrez_lordinateur.md) | [→](exercices/decouvrez_lordinateur.md) | Binaire, codage, processeur, mémoire, système d'exploitation |
| [Découvrez les réseaux informatiques](lessons/decouvrez_les_reseaux_informatiques.md) | [→](exercices/decouvrez_les_reseaux_informatiques.md) | Couches, IP, TCP/UDP, DNS, HTTP |

Lisez-les dans cet ordre : la leçon réseau part de la machine isolée décrite dans
la première.

## Comment travailler

**Pas de code, mais un terminal ouvert.** Chaque leçon se termine par des
observations à faire sur votre propre machine, et les exercices en sont
majoritairement composés. Voir sa propre adresse IP vaut mieux que lire ce
qu'est une adresse IP.

**Ouvrir un terminal :**

- macOS — `Cmd + Espace`, taper « Terminal »
- Linux — `Ctrl + Alt + T`

**Toutes les commandes de ce module sont en lecture seule.** Elles observent,
elles ne modifient rien. Vous ne pouvez rien casser.

**Une règle pour les exercices réseau :** ne les exécutez que sur votre propre
réseau et vers des serveurs publics prévus pour (`example.com`,
`wikipedia.org`, `httpbin.org`). Sonder un réseau qui ne vous appartient pas est
illégal dans la plupart des pays, même sans intention de nuire.

**Répondre par écrit.** Beaucoup d'exercices sont des questions de raisonnement
sans réponse unique. Les formuler par écrit est l'exercice lui-même — une notion
qu'on ne sait pas expliquer n'est pas acquise.

## Ce que ce module prépare

| Notion vue ici | Où elle resservira |
|---|---|
| Adresse mémoire | Les **pointeurs** du C — le point charnière du parcours |
| Octet, débordement | Les types et leurs limites en C |
| ASCII, UTF-8 | Les chaînes de caractères, et leurs pièges |
| Hiérarchie mémoire, cache | Pourquoi un tableau bat une liste chaînée en algo |
| Processus, ordonnancement | Le cours Linux, puis la spécialisation systèmes |
| « Tout est fichier » | Le cours Linux |
| Couches réseau, HTTP | La spécialisation web, et les API du mobile |

## Progression

- [ ] Découvrez l'ordinateur — leçon
- [ ] Découvrez l'ordinateur — exercices
- [ ] Découvrez les réseaux informatiques — leçon
- [ ] Découvrez les réseaux informatiques — exercices
- [ ] Passage à [1.Learn_C](../1.Learn_C/)

## Ce que ce module ne couvre pas

Dit franchement :

- **Le fonctionnement interne d'un transistor** ou d'une porte logique. On
  s'arrête au niveau où le binaire est une donnée du problème.
- **Le détail des protocoles.** On explique ce que fait TCP, pas le format exact
  de ses en-têtes.
- **La sécurité**, au-delà du principe de HTTPS.
- **Le Wi-Fi, la fibre, la 5G** dans leur fonctionnement physique.

Ce qui est ici est ce qu'il faut pour aborder la suite sans buter sur du
vocabulaire.
