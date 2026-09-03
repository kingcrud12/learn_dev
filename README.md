# learn_dev — De zéro à la spécialisation

Parcours d'apprentissage du développement informatique, en partant de zéro
jusqu'au niveau permettant de choisir et d'attaquer une spécialisation :
**embarqué**, **systèmes d'exploitation**, **web** ou **mobile**.

## Philosophie

- **Comprendre avant d'utiliser.** On apprend ce qu'il se passe sous le capot
  (mémoire, pointeurs, système de fichiers, processus) avant de s'appuyer sur
  des frameworks qui masquent ces mécanismes.
- **Écrire du code tous les jours.** Chaque notion est validée par un
  programme qui compile et qui tourne, pas par une lecture.
- **Un socle commun, puis une branche.** Le C, Linux et l'algorithmique
  servent les quatre spécialisations. On les construit d'abord.

## Structure du dépôt

```
learn_dev/
├── 0.Discover_IT/           # Socle : ordinateur et réseaux, sans prérequis
├── learn_C/                 # Socle : langage C, mémoire, pointeurs
├── learn_linux/             # Socle : shell, système de fichiers, outils
├── projets_to_learn_algo/   # Socle : algorithmique et structures de données
│
├── dev_Embeded/             # Spécialisation : systèmes embarqués
├── dev_OS/                  # Spécialisation : systèmes d'exploitation
├── dev_web/                 # Spécialisation : développement web
└── mobile_dev/              # Spécialisation : développement mobile
```

## Le parcours

### Phase 1 — Le socle (à faire en premier, dans l'ordre)

#### 0. [`0.Discover_IT/`](0.Discover_IT/) — Découvrir l'informatique

Point d'entrée, sans aucun prérequis. Ce module ne fait pas écrire de code : il
pose le vocabulaire que tous les cours suivants supposent acquis — mémoire,
adresse, octet, processus, protocole, port.

- Comment une machine qui ne sait que compter fait tourner un navigateur
- Binaire, codage de l'information, ASCII et UTF-8
- Processeur, mémoire vive, stockage, hiérarchie mémoire
- Rôle du système d'exploitation
- Réseaux : couches, IP, TCP/UDP, DNS, HTTP/HTTPS

À faire avant `1.Learn_C` si les mots ci-dessus ne sont pas familiers.

#### 1. `learn_C/` — Le langage C

Le C est choisi comme premier langage parce qu'il ne cache rien : la mémoire,
les types et le cycle de compilation sont explicites. Ce qu'on y apprend
resservira partout.

- Compilation, exécution, cycle `.c` → `.o` → binaire
- Types, variables, opérateurs
- Conditions, boucles, fonctions
- Tableaux et chaînes de caractères
- **Pointeurs** (le point charnière du parcours)
- Allocation dynamique : `malloc`, `free`, fuites mémoire
- Structures, `typedef`, énumérations
- Fichiers multiples, headers, compilation séparée, `make`
- Lecture et écriture de fichiers

#### 2. `learn_linux/` — L'environnement

Toutes les spécialisations visées se pratiquent sous Linux (ou proche).
Savoir naviguer dans le système est un prérequis, pas un bonus.

- Shell : navigation, manipulation de fichiers, permissions
- Redirections, tubes (`|`), chaînage de commandes
- `grep`, `find`, `sed`, `awk`
- Processus, signaux, tâches de fond
- Éditeur en terminal (`vim` ou `nano`)
- **Git** : commits, branches, remotes, résolution de conflits
- Bases du scripting shell

#### 3. [`3.projects_to_learn_algo/`](3.projects_to_learn_algo/) — Algorithmique

Ce qui sépare un programme qui marche d'un programme qui tient la charge.

Dix leçons, un fichier d'exercices par leçon, et trois projets. Tout s'écrit en C :
implémenter une liste chaînée dans un langage à ramasse-miettes cache exactement
ce qu'on cherche à comprendre.

- Complexité, notation Big-O
- Recherche : linéaire, dichotomique
- Tris : sélection, insertion, bulles, fusion, rapide
- Récursivité, pile d'appels, mémoïsation
- Structures : listes chaînées, piles, files, arbres, tables de hachage
- Graphes : parcours en largeur et en profondeur

Les trois projets — [analyseur de texte](3.projects_to_learn_algo/projects/01_analyseur_de_texte/),
[moteur d'itinéraires](3.projects_to_learn_algo/projects/02_moteur_itineraires/),
[banc de mesure des tris](3.projects_to_learn_algo/projects/03_banc_de_tri/) — mettent
les structures en concurrence sur un cas réel.

### Phase 2 — Choisir une spécialisation

Le socle terminé, chaque dossier ci-dessous est une porte d'entrée. Le choix
n'est pas définitif — mais mieux vaut en approfondir une avant d'en ouvrir
une autre.

#### `dev_Embeded/` — Systèmes embarqués
Programmer du matériel avec peu de mémoire et pas de système d'exploitation.
Microcontrôleurs, registres, GPIO, interruptions, protocoles (UART, I2C, SPI),
temps réel. *Prolonge directement le C.*

#### `dev_OS/` — Systèmes d'exploitation
Comprendre et construire ce sur quoi tourne le reste. Appels système,
gestion des processus et de la mémoire, ordonnancement, systèmes de fichiers,
noyau, concurrence. *La branche la plus exigeante en C.*

#### `dev_web/` — Développement web
Front-end (HTML, CSS, JavaScript, un framework), back-end (API, bases de
données, authentification), réseau (HTTP, DNS), déploiement.
*La branche avec le plus grand écart entre le socle et la pratique du métier.*

#### `mobile_dev/` — Développement mobile
Applications Android (Kotlin) ou iOS (Swift), cycle de vie d'une application,
interface, stockage local, appels réseau, publication sur les stores.
Le cross-platform (Flutter) et les quatre façons de brancher un backend y sont
également traités, et mis en pratique dans
[7.Learn_mobile_dev/projects/](7.Learn_mobile_dev/projects/).

## Progression

- [x] Environnement de travail en place
- [ ] `0.Discover_IT` — ordinateur et réseaux
- [ ] `learn_C` — bases du langage
- [ ] `learn_C` — pointeurs et mémoire
- [ ] `learn_linux` — shell et Git
- [ ] `3.projects_to_learn_algo` — structures de données
- [ ] Choix de la spécialisation

## Comment travailler dans ce dépôt

Compiler et exécuter un programme C :

```sh
gcc -Wall -Wextra -g fichier.c -o programme
./programme
```

Les options `-Wall -Wextra` affichent les avertissements du compilateur.
**Un programme qui compile sans avertissement est un objectif, pas un détail.**

## Règles que je m'impose

1. Ne pas copier-coller du code que je ne saurais pas réécrire.
2. Chaque programme doit compiler sans avertissement.
3. Commiter chaque exercice terminé, avec un message qui dit ce qui a été appris.
4. Quand une notion résiste : la réexpliquer à voix haute avant de chercher la réponse.
