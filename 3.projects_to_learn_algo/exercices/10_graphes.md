# Exercices — Graphes

Support : [10_graphes.md](../lessons/10_graphes.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

---

# Niveau 0 — Le graphe de référence

Tous les exercices des niveaux 1 à 3 utilisent ce graphe non orienté :

```
0 —— 1 —— 3
|    |    |
2 —— 4 —— 5        6 (isolé)
```

Arêtes : `0-1, 0-2, 1-3, 1-4, 2-4, 3-5, 4-5`. Sept sommets, sept arêtes, deux
composantes connexes.

Écris-le une fois, réutilise-le partout.

---

# Niveau 1 — Représenter

## 1.1 — Matrice d'adjacence

Implémente `GrapheMatrice`, `ajouter_arete`, `afficher_matrice`.

Affiche la matrice du graphe de référence. Vérifie qu'elle est **symétrique** —
c'est le test de cohérence d'un graphe non orienté.

## 1.2 — Listes d'adjacence

Implémente `Graphe`, `ajouter_arete`, `afficher_listes`, `liberer_graphe`.

Aucune fuite.

## 1.3 — Compter les arêtes

Dans chaque représentation.

*Piège :* en non orienté, chaque arête apparaît deux fois. Ne renvoie pas 14.

## 1.4 — Degré d'un sommet

Dans les deux représentations. Complexité de chacune ?

Vérifie : le sommet `4` a un degré de 3, le sommet `6` de 0.

## 1.5 — La somme des degrés

Calcule-la. Elle vaut exactement `2 × E`. Vérifie, et explique pourquoi.

## 1.6 — Convertir

Écris `matrice_vers_listes` et `listes_vers_matrice`.

Complexité de chacune ?

## 1.7 — La mémoire

Pour `V = 10000` et `E = 20000` (un graphe creux typique), calcule la mémoire des
deux représentations.

Puis pour `V = 1000` et `E = 400000` (dense).

Dans quel cas chaque représentation gagne-t-elle ?

## 1.8 — Un graphe orienté

Retire la symétrie dans `ajouter_arete`.

Construis : `0→1, 1→2, 2→0, 2→3`.

Écris `degre_entrant` et `degre_sortant`. En listes d'adjacence, lequel est
coûteux ? Pourquoi ?

---

# Niveau 2 — Les parcours

## 2.1 — DFS récursif

Depuis le sommet `0`. Affiche l'ordre de visite.

Le sommet `6` ne doit pas apparaître.

## 2.2 — DFS itératif

Avec une pile explicite.

L'ordre est-il identique à celui du récursif ? Si non, pourquoi ? (Regarde
l'ordre dans lequel tu empiles les voisins.)

## 2.3 — Sans le tableau visite

Retire-le du DFS récursif. Lance.

Note ce qui se passe. Explique en une phrase pourquoi un arbre n'en avait pas
besoin et pourquoi un graphe si.

## 2.4 — BFS

Avec une file. Depuis `0`.

Compare l'ordre au DFS. Lequel visite `5` en premier ?

## 2.5 — Marquer au bon moment

Dans le BFS, déplace `visite[v] = true` du moment de l'enfilement à celui du
défilement.

Ajoute un compteur d'enfilements. De combien augmente-t-il ?

Le résultat reste-t-il correct ? Qu'est-ce qui se dégrade ?

## 2.6 — Compter les visites

Ajoute un compteur d'arêtes examinées dans les deux parcours.

Il doit valoir `2E` en non orienté. Vérifie, et rapproche-le du O(V + E) annoncé.

## 2.7 — Ordre de découverte et de fin

Dans le DFS récursif, note pour chaque sommet le moment où on l'atteint et le
moment où on le quitte.

```
sommet 0 : decouverte=1, fin=14
sommet 1 : decouverte=2, fin=11
...
```

Ces deux nombres sont la base de plusieurs algorithmes avancés (tri topologique,
composantes fortement connexes).

---

# Niveau 3 — Ce qu'on résout avec les parcours

## 3.1 — Composantes connexes

Boucle sur tous les sommets, lance un DFS sur chaque sommet non visité, compte
les lancements.

Tu dois obtenir **2**.

Affiche aussi la composition de chaque composante.

## 3.2 — Un chemin existe-t-il ?

`bool chemin_existe(g, a, b)`.

Teste `(0, 5)` → vrai, `(0, 6)` → faux.

## 3.3 — Le plus court chemin

BFS avec les tableaux `distance` et `parent`.

Affiche la distance de `0` à tous les autres sommets. Le sommet `6` doit avoir
une distance de `-1`.

## 3.4 — Reconstruire le chemin

`afficher_chemin(parent, arrivee)`, récursif.

Affiche le chemin de `0` à `5`. Vérifie sa longueur sur le dessin.

## 3.5 — DFS contre BFS pour un chemin

Fais aussi un DFS avec son propre tableau `parent`, et reconstruis un chemin de
`0` à `5`.

Le chemin est-il valide ? Est-il le plus court ?

Compare les longueurs. C'est la démonstration que « trouver un chemin » et
« trouver le plus court » sont deux problèmes distincts.

## 3.6 — Détecter un cycle (non orienté)

Pendant le DFS, si on trouve un voisin déjà visité **qui n'est pas le parent
immédiat**, il y a un cycle.

Le graphe de référence en contient. Trouve-le à la main d'abord, puis vérifie.

Puis teste sur un arbre (un graphe connexe sans cycle) : ta fonction doit
renvoyer faux.

## 3.7 — Détecter un cycle (orienté)

La règle du parent ne fonctionne plus. Il faut trois états : `BLANC` (non
visité), `GRIS` (en cours d'exploration), `NOIR` (terminé).

Un cycle existe si on atteint un sommet `GRIS`.

Teste sur `0→1, 1→2, 2→0` (cycle) et `0→1, 0→2, 1→3, 2→3` (pas de cycle, malgré
les deux chemins vers 3).

Ce second cas est important : il montre pourquoi la version non orientée ne
marche pas ici.

## 3.8 — Biparti ?

Un graphe est biparti si on peut colorier ses sommets en deux couleurs sans que
deux voisins partagent la même.

BFS en coloriant alternativement. Si on trouve un voisin de même couleur, ce
n'est pas biparti.

Le graphe de référence l'est-il ? Vérifie à la main.

## 3.9 — Tri topologique

Sur un graphe **orienté sans cycle**, ordonne les sommets de sorte que chaque
arête aille de gauche à droite.

Exemple : dépendances de compilation.

```
main.c dépend de utils.h et config.h
utils.c dépend de utils.h
utils.h dépend de config.h
```

Méthode : DFS, et empiler chaque sommet **après** avoir exploré tous ses
successeurs. L'ordre topologique est l'inverse de l'ordre de fin.

Ajoute une détection de cycle : un graphe cyclique n'a pas d'ordre topologique.
(C'est ce que `make` signale par « dépendance circulaire ».)

---

# Niveau 4 — Grilles

Une grille est un graphe implicite : chaque case est un sommet, chaque case
voisine une arête. On ne construit pas la structure — on calcule les voisins.

## 4.1 — Chemin dans un labyrinthe

```c
char laby[6][8] = {
    "S......#",
    ".####..#",
    ".#...#.#",
    ".#.#...#",
    "...#.##.",
    "####...E"
};
```

DFS pour trouver **un** chemin de `S` à `E`.

Affiche le labyrinthe avec le chemin marqué.

## 4.2 — Le plus court chemin dans le labyrinthe

BFS. Compare la longueur au chemin du DFS.

Affiche les deux chemins côte à côte.

## 4.3 — Compter les îles

Une grille de `0` et de `1`. Une « île » est un groupe de `1` connectés
horizontalement ou verticalement.

```
1 1 0 0 0
1 1 0 0 0
0 0 1 0 0
0 0 0 1 1
```

Réponse : 3.

Un DFS par case non visitée valant `1`.

## 4.4 — La plus grande île

Même grille, renvoie la taille de la plus grande.

## 4.5 — Remplissage par diffusion

L'outil « pot de peinture » : depuis une case, remplace la couleur de toutes les
cases connectées de même couleur.

DFS ou BFS, au choix. Explique lequel tu choisis et pourquoi.

## 4.6 — Les oranges pourries

Une grille où `2` est une orange pourrie, `1` une orange saine, `0` une case
vide. À chaque minute, toute orange saine adjacente à une pourrie pourrit.

Combien de minutes pour que tout pourrisse ? `-1` si c'est impossible.

*Indice :* BFS **multi-sources** — on enfile toutes les oranges pourries au
départ. Le niveau du BFS **est** le temps.

C'est l'exercice qui montre le mieux la propriété « par distance croissante » du
BFS.

## 4.7 — Le cavalier

Sur un échiquier 8×8, combien de coups minimum pour aller de `a1` à `h8` ?

Les huit déplacements du cavalier remplacent les quatre directions.

BFS, forcément — on veut le minimum.

---

# Niveau 5 — Aller plus loin

## 5.1 — Graphe pondéré

Ajoute un poids aux arêtes dans les listes d'adjacence.

Vérifie que le BFS ne donne **plus** le plus court chemin. Construis un
contre-exemple : un chemin de 1 arête pesant 10, contre un chemin de 3 arêtes
pesant 1 chacune.

Le BFS choisit le premier. C'est faux en pondéré.

## 5.2 — Dijkstra, version simple

Sans file de priorité : à chaque tour, cherche linéairement le sommet non traité
de plus petite distance.

O(V²). Suffisant pour un petit graphe, et bien plus simple à écrire.

Teste sur le contre-exemple du 5.1.

## 5.3 — Dijkstra avec tas

Remplace la recherche linéaire par une file de priorité (un tas binaire).

O((V + E) log V).

Compare les deux sur un graphe généré de 10 000 sommets et 50 000 arêtes.

## 5.4 — Les poids négatifs

Ajoute une arête de poids négatif à ton graphe et lance Dijkstra.

Le résultat est faux. Explique pourquoi : à quel moment Dijkstra suppose-t-il
qu'une distance est définitive ?

*Pour information :* Bellman-Ford traite ce cas, en O(V × E).

## 5.5 — Générer un graphe aléatoire

`V` sommets, chaque paire reliée avec une probabilité `p`.

Fais varier `p` de 0 à 1 par pas de 0,01 et compte les composantes connexes.

Tu observeras une **transition de phase** : autour de `p ≈ ln(V)/V`, le graphe
passe brutalement de fragmenté à connexe.

C'est un résultat classique de la théorie des graphes aléatoires, et il se voit
très bien à la mesure.

## 5.6 — Les six degrés de séparation

Construis un graphe social : 1 000 personnes, chacune avec ~50 relations
aléatoires.

Calcule la distance moyenne entre deux personnes prises au hasard (BFS depuis
plusieurs sources).

Tu devrais obtenir un nombre étonnamment petit — c'est le phénomène du « petit
monde ».

---

# Auto-évaluation

- [ ] Quand choisir une matrice, quand choisir des listes d'adjacence ?
- [ ] Pourquoi le tableau `visite` est-il obligatoire sur un graphe ?
- [ ] Quelle structure pour un DFS ? Pour un BFS ?
- [ ] Pourquoi le BFS trouve-t-il le plus court chemin, et pas le DFS ?
- [ ] Pourquoi le BFS échoue-t-il sur un graphe pondéré ?
- [ ] Pourquoi détecter un cycle diffère-t-il entre orienté et non orienté ?
- [ ] Qu'est-ce que Dijkstra ajoute au BFS ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Dessiner le graphe** et dérouler le parcours à la main, en notant l'état de
   la pile ou de la file à chaque étape. C'est indispensable sur les six premiers
   exercices.
2. **Afficher l'ordre de visite** systématiquement, et le comparer au dessin.
3. **Commencer petit** : quatre sommets suffisent à révéler un bug. Ne débogue
   jamais sur un graphe généré aléatoirement.
