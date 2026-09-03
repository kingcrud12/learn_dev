# Algorithmique et structures de données

Troisième et dernier module du socle, après [1.Learn_C](../1.Learn_C/) et
[2.Learn_linux](../2.Learn_linux/).

## Pourquoi ce module

Le C apprend à écrire un programme qui marche. L'algorithmique apprend à écrire
un programme qui **tient la charge**.

C'est une différence de nature, pas de degré. Un tri par sélection et un tri
fusion produisent exactement le même résultat. Sur 100 éléments, aucun écart
perceptible. Sur un million, l'un met dix-sept minutes et l'autre deux
centièmes de seconde.

Le code ne change pas. C'est `n` qui change — et ce module enseigne à prévoir ce
qui se passe alors, **avant** d'avoir les données.

## Ce que ce module apporte aux quatre spécialisations

| Spécialisation | Ce qui en resservira directement |
|---|---|
| **Embarqué** | La complexité en espace. Sur 2 Ko de RAM, O(n) de mémoire supplémentaire décide de ce qui est implémentable. |
| **Systèmes d'exploitation** | Files (ordonnanceurs), tables de hachage (tables de pages), arbres rouge-noir (le CFS de Linux), tampons circulaires (E/S). |
| **Web** | Index, caches LRU, files de tâches. La différence entre une requête à 20 ms et une à 2 s est presque toujours un choix de structure. |
| **Mobile** | Les listes défilantes, la recherche locale, le cache d'images. Une structure mal choisie se voit à l'écran. |

## Structure du module

```
4.projects_to_learn_algo/
├── lessons/       # 10 leçons, dans l'ordre
├── exercices/     # un fichier par leçon, en niveaux progressifs
├── codes/         # tes implémentations
└── projects/      # 3 projets, à faire après les leçons correspondantes
```

## Le parcours

Les leçons se suivent : chacune s'appuie sur les précédentes.

| # | Leçon | Exercices | Ce qu'on y gagne |
|---|---|---|---|
| 1 | [Complexité et Big-O](lessons/01_complexite_et_big_o.md) | [→](exercices/01_complexite.md) | L'outil d'analyse commun à tout le reste |
| 2 | [Recherche](lessons/02_recherche.md) | [→](exercices/02_recherche.md) | Le premier O(log n), et la dichotomie sur une réponse |
| 3 | [Tris élémentaires](lessons/03_tris_elementaires.md) | [→](exercices/03_tris_elementaires.md) | Stabilité, adaptativité, invariants |
| 4 | [Tris efficaces](lessons/04_tris_efficaces.md) | [→](exercices/04_tris_efficaces.md) | Diviser pour régner, et la borne O(n log n) |
| 5 | [Récursivité](lessons/05_recursivite.md) | [→](exercices/05_recursivite.md) | La pile, la mémoïsation, les récurrences |
| 6 | [Listes chaînées](lessons/06_listes_chainees.md) | [→](exercices/06_listes_chainees.md) | Le troc contiguïté ↔ insertion |
| 7 | [Piles et files](lessons/07_piles_et_files.md) | [→](exercices/07_piles_et_files.md) | LIFO, FIFO, le tableau circulaire |
| 8 | [Arbres binaires de recherche](lessons/08_arbres_binaires.md) | [→](exercices/08_arbres_binaires.md) | O(log n) sur les trois opérations, et l'équilibrage |
| 9 | [Tables de hachage](lessons/09_tables_de_hachage.md) | [→](exercices/09_tables_de_hachage.md) | O(1), et ce qu'il coûte |
| 10 | [Graphes](lessons/10_graphes.md) | [→](exercices/10_graphes.md) | DFS, BFS, et la modélisation |

### Les projets

À faire **après** les leçons indiquées, pas à la fin de tout.

| Projet | Après la leçon | Sujet |
|---|---|---|
| [Analyseur de texte](projects/01_analyseur_de_texte/) | 9 | Table de hachage contre ABR, sur un vrai corpus |
| [Moteur d'itinéraires](projects/02_moteur_itineraires/) | 10 | BFS, Dijkstra, tas binaire |
| [Banc de mesure des tris](projects/03_banc_de_tri/) | 4 | Vérifier expérimentalement tout ce qui a été affirmé |

Le troisième peut se commencer dès la leçon 4 et s'enrichir ensuite.

## Comment travailler

**Tout s'écrit en C.** C'est un choix, et il a une raison : implémenter une liste
chaînée en Python cache exactement ce qu'on cherche à comprendre. En C, chaque
`malloc` est visible, chaque pointeur est explicite, chaque octet se compte.

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

Aucun avertissement toléré.

**Vérifier la mémoire, systématiquement.** À partir de la leçon 6, tous les
programmes allouent.

```sh
leaks --atExit -- ./codes/nom                 # macOS
valgrind --leak-check=full ./codes/nom        # Linux
gcc -fsanitize=address -g codes/nom.c -o ...  # les deux
```

Un programme qui fuit n'est pas terminé, même si son résultat est juste.

**Mesurer, pas supposer.** Chaque leçon se termine par une mesure à faire. Ce
n'est pas décoratif : l'écart entre l'analyse théorique et le comportement réel
est précisément le sujet de ce module. Une analyse qui contredit une mesure a
presque toujours tort.

**Écrire l'invariant.** Devant un algorithme qu'on ne comprend pas, la question
qui débloque est : *qu'est-ce qui est vrai à chaque tour de boucle ?* C'est ce
qui transforme un code qu'on récite en un code qu'on sait.

## Progression

- [ ] 1 — Complexité et Big-O
- [ ] 2 — Recherche
- [ ] 3 — Tris élémentaires
- [ ] 4 — Tris efficaces
- [ ] Projet — Banc de mesure des tris
- [ ] 5 — Récursivité
- [ ] 6 — Listes chaînées
- [ ] 7 — Piles et files
- [ ] 8 — Arbres binaires de recherche
- [ ] 9 — Tables de hachage
- [ ] Projet — Analyseur de texte
- [ ] 10 — Graphes
- [ ] Projet — Moteur d'itinéraires

## Ce que le module ne couvre pas

Dit franchement, pour que les manques soient connus :

- **Les arbres équilibrés** (AVL, rouge-noir) sont expliqués, pas implémentés.
  Leur code est long et technique ; on en donne le principe et les rotations.
- **La programmation dynamique** n'est qu'effleurée, via la mémoïsation de
  Fibonacci. C'est un sujet à part entière.
- **Les tries, les tas de Fibonacci, les structures union-find** apparaissent en
  extensions de projets, pas en leçons.
- **Les algorithmes de graphes avancés** — Bellman-Ford, Kruskal, Prim, les
  composantes fortement connexes — sont cités et situés, pas traités.

Ce qui est ici est ce qui sert partout. Le reste s'apprend ensuite, et s'apprend
vite quand ces bases-là sont solides.
