# Projets

Les leçons enseignent des structures isolées. Les projets les mettent en
concurrence : c'est là qu'on découvre que le choix d'une structure de données
**est** la décision d'architecture, et que Big-O ne tranche pas tout seul.

| Projet | Sujet | Structures en jeu |
|---|---|---|
| [01_analyseur_de_texte/](01_analyseur_de_texte/) | Compter, classer et interroger les mots d'un corpus | Table de hachage, ABR, tri |
| [02_moteur_itineraires/](02_moteur_itineraires/) | Plus court chemin dans un réseau de transport | Graphe, BFS, Dijkstra, tas |
| [03_banc_de_tri/](03_banc_de_tri/) | Banc de mesure comparant huit algorithmes de tri | Tous les tris, instrumentation |

## Comment aborder un projet

Un projet n'est pas un exercice. Un exercice a une solution ; un projet a des
**arbitrages**.

L'ordre de travail est toujours le même, et l'inverse de celui qu'on adopte
spontanément :

1. **Comprendre le problème** — quelles opérations, à quelle fréquence, sur
   quelle volumétrie. Avant toute structure de données.
2. **Choisir les structures** — et écrire pourquoi. Un choix non justifié est un
   choix non fait.
3. **Écrire les interfaces** — les fichiers `.h`, avant les `.c`. Le contrat
   d'abord.
4. **Construire en tranches verticales** — une chaîne complète et étroite, qu'on
   épaissit ensuite.

Le réflexe naturel est de commencer par le code. Le résultat est un projet dont
la structure est un accident de ses premières heures.

## Trois règles communes à tous les projets

**Compilation séparée.** Un fichier `.c` par module, un `.h` par interface, un
`Makefile`. Ces projets sont aussi l'occasion de pratiquer ce que la leçon C sur
la compilation séparée a introduit.

```sh
gcc -Wall -Wextra -g -c module.c -o module.o
```

Aucun avertissement toléré.

**Mesurer, pas supposer.** Chaque projet a une section « Mesures attendues ». Un
projet dont on n'a pas mesuré le comportement n'est pas terminé — on ne saurait
pas dire si les choix étaient bons.

**Aucune fuite.**

```sh
leaks --atExit -- ./programme            # macOS
valgrind --leak-check=full ./programme   # Linux
```

## Un carnet de décisions

Pour chaque projet, tiens un fichier `DECISIONS.md` avec une entrée par choix
structurant :

```markdown
## Structure choisie pour l'index des mots

**Options envisagées :** tableau trié, ABR, table de hachage.

**Retenu :** table de hachage avec chaînage, taille initiale 4096.

**Pourquoi :** les mesures montrent 95 % de recherches pour 5 % d'insertions.
L'ABR donnait 18 comparaisons par recherche contre 1,4 pour la table.

**Ce qu'on abandonne :** le parcours dans l'ordre alphabétique. On le récupère
par un tri final en O(n log n), payé une seule fois à l'affichage.
```

Trois de ces entrées par projet suffisent. C'est le livrable le plus utile des
trois : dans six mois, c'est ce qui expliquera le code.
