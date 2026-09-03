# Graphes : représentation et parcours

## La structure la plus générale

Une liste chaînée : chaque nœud a un suivant. Un arbre : chaque nœud a deux
enfants, et pas de cycle. Un **graphe** : chaque nœud peut être relié à
n'importe quel autre, y compris en formant des cycles.

C'est la structure la plus générale — les précédentes en sont des cas
particuliers — et c'est celle qui modélise le plus de choses réelles :

- un réseau social (personnes, amitiés) ;
- un plan de métro (stations, lignes) ;
- les dépendances d'un projet (modules, `#include`) ;
- le web (pages, liens) ;
- un labyrinthe (cases, passages).

## Le vocabulaire

**Sommet** (*vertex*) : un nœud. **Arête** (*edge*) : un lien entre deux sommets.

**Non orienté** : l'arête va dans les deux sens (une amitié).
**Orienté** : elle a un sens (un lien hypertexte, une dépendance).

**Pondéré** : chaque arête porte un coût (une distance, un temps).

**Degré** d'un sommet : son nombre d'arêtes. En orienté, on distingue degré
entrant et sortant.

**Chemin** : une suite de sommets reliés. **Cycle** : un chemin qui revient à son
point de départ.

**Connexe** : il existe un chemin entre toute paire de sommets. Sinon, le graphe a
plusieurs **composantes connexes**.

## Les deux représentations

Le choix de représentation détermine la complexité de tous les algorithmes qui
suivront. Ce n'est pas un détail d'implémentation.

Notations : `V` = nombre de sommets, `E` = nombre d'arêtes.

### Matrice d'adjacence

Un tableau à deux dimensions. `mat[i][j] = 1` s'il y a une arête de `i` vers `j`.

```c
#define MAX_SOMMETS 100

typedef struct {
    int mat[MAX_SOMMETS][MAX_SOMMETS];
    int nb_sommets;
} GrapheMatrice;

void ajouter_arete(GrapheMatrice *g, int a, int b) {
    g->mat[a][b] = 1;
    g->mat[b][a] = 1;        // à retirer si le graphe est orienté
}
```

Pour le graphe `0—1, 0—2, 1—2, 2—3` :

```
     0  1  2  3
  0 [0][1][1][0]
  1 [1][0][1][0]
  2 [1][1][0][1]
  3 [0][0][1][0]
```

La matrice d'un graphe non orienté est **symétrique**. C'est le test de cohérence
le plus simple.

**Mémoire** : O(V²) — toujours, même sans aucune arête.
**Tester l'existence d'une arête** : **O(1)**.
**Lister les voisins d'un sommet** : O(V), il faut balayer toute la ligne.

### Listes d'adjacence

Pour chaque sommet, la liste de ses voisins.

```c
typedef struct Voisin {
    int sommet;
    struct Voisin *suivant;
} Voisin;

typedef struct {
    Voisin *listes[MAX_SOMMETS];
    int nb_sommets;
} Graphe;

bool ajouter_arete(Graphe *g, int a, int b) {
    Voisin *v1 = malloc(sizeof(Voisin));
    if (v1 == NULL) return false;
    v1->sommet = b;
    v1->suivant = g->listes[a];
    g->listes[a] = v1;

    Voisin *v2 = malloc(sizeof(Voisin));      // à retirer si orienté
    if (v2 == NULL) return false;
    v2->sommet = a;
    v2->suivant = g->listes[b];
    g->listes[b] = v2;
    return true;
}
```

```
0 → 2 → 1 → NULL
1 → 2 → 0 → NULL
2 → 3 → 1 → 0 → NULL
3 → 2 → NULL
```

**Mémoire** : O(V + E).
**Tester une arête** : O(degré), donc O(V) au pire.
**Lister les voisins** : O(degré) — optimal.

### Comment choisir

| | Matrice | Listes |
|---|---|---|
| Mémoire | O(V²) | **O(V + E)** |
| Arête existe ? | **O(1)** | O(degré) |
| Voisins de `v` | O(V) | **O(degré)** |
| Parcours complet | O(V²) | **O(V + E)** |

La question est la **densité**. Un graphe est dense si `E ≈ V²`, creux si
`E ≈ V`.

Un réseau social de 1 milliard d'utilisateurs ayant 200 amis chacun : la matrice
demanderait 10¹⁸ cases, soit un exaoctet. Les listes demandent 2×10¹¹ entrées.
Le choix ne se discute pas.

**En pratique, la quasi-totalité des graphes réels sont creux.** Les listes
d'adjacence sont le choix par défaut. La matrice se justifie sur des petits
graphes denses, ou quand on teste sans cesse l'existence d'arêtes.

## Parcours en profondeur (DFS)

On s'enfonce le plus loin possible dans une direction, puis on revient sur ses
pas pour explorer les autres.

### Version récursive

```c
void dfs(const Graphe *g, int sommet, bool visite[]) {
    visite[sommet] = true;
    printf("%d ", sommet);

    for (Voisin *v = g->listes[sommet]; v != NULL; v = v->suivant) {
        if (!visite[v->sommet]) {
            dfs(g, v->sommet, visite);
        }
    }
}
```

**Le tableau `visite` n'est pas une optimisation, c'est une condition de
terminaison.** Sur un arbre, on pouvait s'en passer : pas de cycle, donc pas de
retour en arrière possible. Sur un graphe, `0 → 1 → 0 → 1 → ...` boucle
indéfiniment sans lui.

C'est la différence essentielle entre parcourir un arbre et parcourir un graphe.

### Version itérative, avec pile explicite

```c
void dfs_iteratif(const Graphe *g, int depart) {
    bool visite[MAX_SOMMETS] = {false};
    int pile[MAX_SOMMETS];
    int sommet_pile = 0;

    pile[sommet_pile++] = depart;

    while (sommet_pile > 0) {
        int courant = pile[--sommet_pile];

        if (visite[courant]) continue;
        visite[courant] = true;
        printf("%d ", courant);

        for (Voisin *v = g->listes[courant]; v != NULL; v = v->suivant) {
            if (!visite[v->sommet]) {
                pile[sommet_pile++] = v->sommet;
            }
        }
    }
}
```

La version récursive utilisait la pile d'appels ; celle-ci utilise une pile
explicite. **C'est le même algorithme.** La récursion n'est qu'une façon d'obtenir
une pile gratuitement.

Le `if (visite[courant]) continue;` est nécessaire ici : un sommet peut être
empilé plusieurs fois avant d'être traité.

Cette version est préférable sur les grands graphes : elle ne risque pas le
débordement de pile d'appels.

**Complexité** : O(V + E). Chaque sommet est visité une fois, chaque arête
examinée une fois (deux fois en non orienté).
**Espace** : O(V).

### Ce que le DFS résout

- Détecter un **cycle**.
- Trouver les **composantes connexes** : on lance un DFS par sommet non encore
  visité, et on compte les lancements.
- Le **tri topologique** : ordonner des tâches selon leurs dépendances. C'est ce
  que fait `make` pour décider de l'ordre de compilation.
- Le **retour sur trace** : labyrinthes, sudoku, n-reines.

## Parcours en largeur (BFS)

On visite tous les sommets à distance 1, puis tous ceux à distance 2, et ainsi de
suite. Comme une onde qui se propage.

```c
void bfs(const Graphe *g, int depart) {
    bool visite[MAX_SOMMETS] = {false};
    int file[MAX_SOMMETS];
    int tete = 0, queue = 0;

    visite[depart] = true;
    file[queue++] = depart;

    while (tete < queue) {
        int courant = file[tete++];
        printf("%d ", courant);

        for (Voisin *v = g->listes[courant]; v != NULL; v = v->suivant) {
            if (!visite[v->sommet]) {
                visite[v->sommet] = true;      // marquer À L'ENFILEMENT
                file[queue++] = v->sommet;
            }
        }
    }
}
```

**Le code est celui du DFS itératif, avec une file au lieu d'une pile.** C'est
tout. La structure de données choisie détermine l'ordre de parcours — et c'est
l'illustration la plus nette de la leçon 7.

**Une différence importante** : on marque `visite` **au moment d'enfiler**, pas
au moment de défiler. Sinon un même sommet peut être enfilé plusieurs fois par
différents voisins avant d'être traité, et la file gonfle inutilement.

**Complexité** : O(V + E). **Espace** : O(V).

### La propriété qui fait tout l'intérêt du BFS

> Le BFS visite les sommets **par distance croissante** depuis le départ.

Conséquence : **il trouve le plus court chemin** dans un graphe non pondéré. Le
DFS, non.

```c
void bfs_chemin(const Graphe *g, int depart, int distance[], int parent[]) {
    bool visite[MAX_SOMMETS] = {false};
    int file[MAX_SOMMETS];
    int tete = 0, queue = 0;

    for (int i = 0; i < g->nb_sommets; i++) {
        distance[i] = -1;
        parent[i] = -1;
    }

    visite[depart] = true;
    distance[depart] = 0;
    file[queue++] = depart;

    while (tete < queue) {
        int courant = file[tete++];

        for (Voisin *v = g->listes[courant]; v != NULL; v = v->suivant) {
            if (!visite[v->sommet]) {
                visite[v->sommet] = true;
                distance[v->sommet] = distance[courant] + 1;
                parent[v->sommet] = courant;
                file[queue++] = v->sommet;
            }
        }
    }
}
```

Le tableau `parent` permet de **reconstruire** le chemin, en remontant de
l'arrivée jusqu'au départ :

```c
void afficher_chemin(const int parent[], int arrivee) {
    if (parent[arrivee] != -1) {
        afficher_chemin(parent, parent[arrivee]);
    }
    printf("%d ", arrivee);
}
```

La récursion inverse naturellement l'ordre : on affiche le parent avant l'enfant,
donc le chemin sort du départ vers l'arrivée.

### Ce que le BFS résout

- Le **plus court chemin** dans un graphe non pondéré.
- La **distance minimale** entre deux sommets — les « degrés de séparation ».
- Le plus court chemin dans un **labyrinthe** : chaque case est un sommet.
- La **diffusion** dans un réseau, niveau par niveau.

## DFS ou BFS ?

| | DFS | BFS |
|---|---|---|
| Structure | **pile** (ou récursion) | **file** |
| Ordre | en profondeur d'abord | par distance croissante |
| Plus court chemin | non | **oui** (non pondéré) |
| Espace | O(hauteur) | O(largeur maximale) |
| Cycles, connexité | **oui** | oui |
| Tri topologique | **oui** | non (naturellement) |
| Retour sur trace | **oui** | non |

Sur un graphe très large et peu profond, le DFS consomme moins de mémoire. Sur un
graphe très profond, c'est le BFS. Les deux sont O(V + E) en temps.

**La règle simple** : plus court chemin → BFS. Tout le reste → DFS, plus simple à
écrire en récursif.

## Ce qui vient après

Ces deux parcours sont la base. Les algorithmes qu'on construit dessus :

**Dijkstra** — plus court chemin dans un graphe **pondéré**. C'est un BFS où la
file est remplacée par une **file de priorité** : on explore toujours le sommet
le plus proche non encore traité. O((V + E) log V) avec un tas.

**A\*** — Dijkstra guidé par une heuristique. L'algorithme des GPS et des jeux
vidéo.

**Bellman-Ford** — accepte les poids négatifs, que Dijkstra refuse. O(V × E).

**Kruskal, Prim** — arbre couvrant de poids minimal : relier tous les sommets au
moindre coût. Câblage réseau, routes.

**Tri topologique** — ordonner des tâches sous contraintes de dépendances.
`make`, les gestionnaires de paquets, les ordonnanceurs de build.

Tous partent du même principe : parcourir en marquant les sommets visités.

## Ce qu'il faut retenir

- Sommets + arêtes. Orienté ou non, pondéré ou non.
- **Listes d'adjacence par défaut** (O(V + E) en mémoire) ; matrice seulement
  pour les petits graphes denses.
- Les deux parcours sont **O(V + E)**.
- **Le tableau `visite` est obligatoire** : sans lui, un cycle boucle
  indéfiniment. C'est ce qui distingue un graphe d'un arbre.
- DFS = pile, BFS = file. **Même code, structure différente.**
- **Le BFS trouve le plus court chemin** en non pondéré ; le DFS non.
- Le tableau `parent` reconstruit le chemin.
- Dijkstra est un BFS à file de priorité.

---

## À faire maintenant

**1.** Construis ce graphe en listes d'adjacence :

```
0 —— 1 —— 3
|    |    |
2 —— 4 —— 5        et un sommet 6 isolé
```

Lance un DFS et un BFS depuis `0`. Les ordres doivent différer, et `6`
n'apparaître dans aucun des deux.

**2.** Écris `compter_composantes()` : boucle sur tous les sommets, lance un DFS
sur chaque sommet non visité, compte les lancements. Tu dois obtenir **2**.

**3.** Écris `plus_court_chemin(0, 5)` avec le tableau `parent`. Vérifie le
résultat à la main sur le dessin.

**4.** Lance aussi un DFS depuis `0` et essaie de reconstruire un chemin vers `5`
avec son propre tableau `parent`. Compare les longueurs.

Le chemin du DFS sera valide mais plus long. C'est la démonstration que « trouver
un chemin » et « trouver le plus court chemin » sont deux problèmes différents.
