# Passerelle — l'algorithmique en apprenant le C au passage

Ce document existe parce que le module algo suppose des notions C que tu n'as
pas encore vues. Plutôt que d'attendre, chaque sujet introduit ici **la notion C
juste avant d'en avoir besoin**, puis l'algorithme qui s'en sert.

C'est une passerelle, pas un remplacement. Les leçons complètes sont dans
[lessons/](lessons/) et dans [1.Learn_C/lessons/](../1.Learn_C/lessons/) — ce
document te dit quoi lire, quand, et pourquoi.

**Prérequis :** leçons C 01 à 03 (types, boucles, fonctions).

---

## Comment lire ce document

Chaque partie suit le même schéma :

```
┌─ LA NOTION C ─────────┐   ┌─ L'ALGORITHME ────────┐
│ ce dont tu as besoin  │ → │ ce que ça débloque    │
│ pour la suite         │   │ et pourquoi ça compte │
└───────────────────────┘   └───────────────────────┘
```

Compile tout avec :

```sh
gcc -Wall -Wextra -g -fsanitize=address,undefined prog.c -o prog
```

---

# Partie 1 — Mesurer avant de coder

*Aucune notion C nouvelle. Tu peux lire ceci aujourd'hui.*

## Le problème

Une fonction qui cherche un nom parmi 50 contacts : instantanée. La même sur
10 millions : huit secondes. Le code n'a pas changé — **n** a changé.

L'algorithmique, c'est prédire ce comportement **avant** d'avoir les données.

## Compter les opérations, pas les secondes

Chronométrer dépend de ta machine. On compte plutôt les **opérations
élémentaires** en fonction de `n`.

```c
for (int i = 0; i < n; i++) {       // n tours
    somme += tab[i];                 // 1 opération par tour
}
```

→ `n` opérations. On note **O(n)**.

La notation Big-O ne garde que le terme dominant, sans les constantes :

| Vraie formule | Big-O | Pourquoi |
|---|---|---|
| `3n + 7` | O(n) | la constante 3 et le +7 disparaissent |
| `n² + 100n` | O(n²) | pour n grand, n² écrase 100n |
| `5` | O(1) | ne dépend pas de n |

## Les classes à connaître

| Complexité | Nom | n = 1 000 000 |
|---|---|---|
| O(1) | constant | 1 opération |
| O(log n) | logarithmique | ~20 |
| O(n) | linéaire | 1 000 000 |
| O(n log n) | quasi-linéaire | ~20 000 000 |
| O(n²) | quadratique | 1 000 000 000 000 |

**L'écart entre O(n log n) et O(n²) est ce qui sépare un tri utilisable d'un tri
inutilisable.** Sur un million d'éléments : quelques secondes contre plusieurs
jours.

## Reconnaître une complexité

```c
for (int i = 0; i < n; i++)              // O(n)

for (int i = 0; i < n; i++)              // O(n²) : boucles imbriquées
    for (int j = 0; j < n; j++)

while (n > 1) { n = n / 2; }             // O(log n) : on divise à chaque tour
```

**La règle :** des boucles imbriquées multiplient, des boucles successives
s'additionnent (donc la plus coûteuse gagne).

## À faire maintenant

Écris ces trois fonctions et donne leur complexité :

1. Somme des entiers de 1 à n, par une boucle
2. La même par la formule `n(n+1)/2`
3. Compte combien de fois on peut diviser n par 2 avant d'atteindre 1

La 1 est O(n), la 2 est O(1), la 3 est O(log n). **Tu viens de rencontrer les
trois classes fondamentales.**

→ Leçon complète : [01_complexite_et_big_o.md](lessons/01_complexite_et_big_o.md)

---

# Partie 2 — Les tableaux, et la recherche

## ┌─ LA NOTION C : les tableaux ─┐

Un tableau est un **bloc contigu** d'éléments de même type.

```c
int notes[5] = {12, 15, 8, 17, 10};
```

En mémoire, 5 × 4 = 20 octets côte à côte :

```
adresse:  0x1000    0x1004    0x1008    0x100C    0x1010
          [  12  ]  [  15  ]  [   8  ]  [  17  ]  [  10  ]
index:       0         1         2         3         4
```

L'indice est un **décalage** depuis le début — d'où le départ à 0. Le processeur
calcule :

```
adresse de notes[i] = adresse de notes + i × sizeof(int)
```

Une multiplication, une addition. **C'est pourquoi l'accès est O(1)**, quel que
soit l'indice. Retiens ça, c'est le fondement de toute la suite.

### Les trois règles

```c
int tab[5] = {0};                        // 1. TOUJOURS initialiser
int n = sizeof(tab) / sizeof(tab[0]);    // 2. la longueur, à calculer soi-même
for (int i = 0; i < n; i++)              // 3. i < n, JAMAIS i <= n
```

### Aucune borne n'est vérifiée

```c
int tab[5] = {0};
tab[10] = 42;      // compile, s'exécute, corrompt la mémoire
```

Le C calcule l'adresse et écrit. Pas d'`IndexError`. C'est **le** piège du
langage — compile toujours avec `-fsanitize=address`, il le détecte.

### Passer un tableau à une fonction

```c
void afficher(const int tab[], int n) {    // la taille en paramètre, obligatoire
    for (int i = 0; i < n; i++) {
        printf("%d ", tab[i]);
    }
}
```

**Pourquoi `n` en plus ?** Parce que le tableau n'est pas copié : seule
l'adresse du premier élément est transmise. L'information de longueur est
perdue. C'est la convention du C, sans exception — regarde `memcpy(dst, src, n)`.

Le `const` dit « je ne modifie pas ». Le compilateur vérifie.

**Conséquence :** une fonction sans `const` modifie ton tableau original.

```c
void doubler(int tab[], int n) {
    for (int i = 0; i < n; i++) tab[i] *= 2;
}
// après l'appel, ton tableau A CHANGÉ
```

→ Leçon complète : [06_tableaux_et_chaines.md](../1.Learn_C/lessons/06_tableaux_et_chaines.md)

## └─ L'ALGORITHME : recherche ─┘

### Recherche linéaire — O(n)

```c
int recherche_lineaire(const int tab[], int n, int cible) {
    for (int i = 0; i < n; i++) {
        if (tab[i] == cible) {
            return i;            // trouvé : on renvoie l'indice
        }
    }
    return -1;                   // convention : -1 = absent
}
```

Au pire, n comparaisons. Fonctionne sur **n'importe quel** tableau.

### Recherche dichotomique — O(log n)

**Exige un tableau trié.** L'idée : couper en deux à chaque tour.

```c
int recherche_dicho(const int tab[], int n, int cible) {
    int gauche = 0;
    int droite = n - 1;

    while (gauche <= droite) {
        int milieu = gauche + (droite - gauche) / 2;

        if (tab[milieu] == cible) {
            return milieu;
        }
        if (tab[milieu] < cible) {
            gauche = milieu + 1;      // la cible est à droite
        } else {
            droite = milieu - 1;      // la cible est à gauche
        }
    }
    return -1;
}
```

**Le détail qui compte :** `gauche + (droite - gauche) / 2` plutôt que
`(gauche + droite) / 2`. Sur de très grands tableaux, la seconde forme peut
**déborder** l'`int` — un bug qui est resté vingt ans dans la bibliothèque
standard de Java.

### Pourquoi O(log n)

À chaque tour, l'espace de recherche est divisé par 2 :

```
1 000 000 → 500 000 → 250 000 → ... → 1
```

Combien de divisions ? `log₂(1 000 000) ≈ 20`.

**Vingt comparaisons au lieu d'un million.** C'est le premier vrai gain
algorithmique que tu rencontres.

### Le compromis

| | Linéaire | Dichotomique |
|---|---|---|
| Coût | O(n) | O(log n) |
| Tableau trié requis | non | **oui** |
| Coût du tri préalable | — | O(n log n) |

Trier pour une seule recherche n'a aucun sens. Trier une fois pour un million de
recherches, oui. **La bonne question n'est jamais « quel algorithme est le
meilleur » mais « combien de fois vais-je m'en servir ».**

### À faire

1. Écris les deux fonctions. Ajoute un compteur de comparaisons dans chacune.
2. Compare sur un tableau trié de 1000 éléments. Tu devrais voir ~500 contre ~10.
3. Que se passe-t-il si tu lances la dichotomique sur un tableau non trié ?

→ Leçon complète : [02_recherche.md](lessons/02_recherche.md)

---

# Partie 3 — Échanger, et trier

## ┌─ LA NOTION C : le pointeur, minimum vital ─┐

Pour trier, il faut **échanger** deux éléments. Et pour écrire une fonction qui
échange, il faut des pointeurs.

### L'idée en une phrase

> La mémoire est un tableau d'octets numérotés. Un pointeur est un numéro de case.

```c
int nombre = 42;
int *p = &nombre;      // p contient l'ADRESSE de nombre

*p = 100;              // écrit 100 À CETTE ADRESSE
printf("%d\n", nombre); // 100 — nombre a changé
```

| Écriture | Sens |
|---|---|
| `&nombre` | l'**adresse de** nombre |
| `*p` | **ce qui est à** l'adresse p (lecture ou écriture) |

### Pourquoi c'est nécessaire

En C, les paramètres sont **copiés**. Sans pointeur, impossible de modifier une
variable de l'appelant :

```c
void echanger_faux(int a, int b) {
    int tmp = a; a = b; b = tmp;      // échange les COPIES, sans effet
}

void echanger(int *a, int *b) {       // avec pointeurs
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int x = 3, y = 7;
echanger(&x, &y);        // x vaut 7, y vaut 3
```

**Écris cette fonction avant de continuer.** C'est la brique de tous les tris.

→ Leçon complète : [07_pointeurs.md](../1.Learn_C/lessons/07_pointeurs.md)

## └─ L'ALGORITHME : les tris ─┘

### Tri par sélection — O(n²)

*Chercher le minimum, le mettre devant, recommencer.*

```c
void tri_selection(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[min]) {
                min = j;
            }
        }
        if (min != i) {
            echanger(&tab[i], &tab[min]);
        }
    }
}
```

Deux boucles imbriquées → **O(n²)**, toujours, même sur un tableau déjà trié.

### Tri par insertion — O(n²), mais O(n) si presque trié

*Comme trier des cartes en main : prendre la suivante, l'insérer au bon endroit.*

```c
void tri_insertion(int tab[], int n) {
    for (int i = 1; i < n; i++) {
        int cle = tab[i];
        int j = i - 1;

        while (j >= 0 && tab[j] > cle) {
            tab[j + 1] = tab[j];       // décaler à droite
            j--;
        }
        tab[j + 1] = cle;
    }
}
```

**Sa qualité :** sur un tableau déjà presque trié, la boucle interne ne tourne
presque pas → O(n). C'est pourquoi les vraies bibliothèques l'utilisent sur les
petits sous-tableaux, à l'intérieur d'algorithmes plus gros.

### Tri fusion — O(n log n)

*Diviser en deux, trier chaque moitié, fusionner.*

```c
void fusionner(int tab[], int gauche, int milieu, int droite, int tmp[]) {
    int i = gauche, j = milieu + 1, k = gauche;

    while (i <= milieu && j <= droite) {
        tmp[k++] = (tab[i] <= tab[j]) ? tab[i++] : tab[j++];
    }
    while (i <= milieu)  tmp[k++] = tab[i++];
    while (j <= droite)  tmp[k++] = tab[j++];

    for (int m = gauche; m <= droite; m++) {
        tab[m] = tmp[m];
    }
}

void tri_fusion(int tab[], int gauche, int droite, int tmp[]) {
    if (gauche >= droite) {
        return;                                    // cas de base
    }
    int milieu = gauche + (droite - gauche) / 2;

    tri_fusion(tab, gauche, milieu, tmp);          // trier la moitié gauche
    tri_fusion(tab, milieu + 1, droite, tmp);      // trier la moitié droite
    fusionner(tab, gauche, milieu, droite, tmp);   // recoller
}
```

**Pourquoi O(n log n) :** on divise en deux `log n` fois, et chaque niveau de
fusion coûte O(n).

```
        [8 éléments]              ← 1 niveau, n opérations
       /            \
   [4]              [4]           ← 2 niveaux, n opérations
   / \              / \
 [2] [2]          [2] [2]         ← log n niveaux au total
```

Son défaut : il lui faut un tableau temporaire → **O(n) d'espace en plus**.

### Le tableau récapitulatif

| Tri | Temps moyen | Pire cas | Espace | Stable |
|---|---|---|---|---|
| Sélection | O(n²) | O(n²) | O(1) | non |
| Insertion | O(n²) | O(n²) | O(1) | oui |
| Fusion | O(n log n) | **O(n log n)** | O(n) | oui |
| Rapide | O(n log n) | O(n²) | O(log n) | non |

**Stable** = deux éléments égaux gardent leur ordre relatif. Ça compte quand tu
tries par nom puis par âge : un tri stable préserve le premier tri.

### La mesure qui frappe

Mesuré sur un Mac ARM64, avec `-O2`, sur des entiers aléatoires :

| n | insertion O(n²) | fusion O(n log n) | rapport |
|---|---|---|---|
| 1 000 | 0,0001 s | 0,0000 s | — |
| 2 000 | 0,0004 s | 0,0001 s | 4× |
| 4 000 | 0,0015 s | 0,0001 s | 15× |
| 20 000 | 0,0428 s | 0,0008 s | **53×** |

**Regarde la colonne insertion : quand n double, le temps quadruple.**
0,0001 → 0,0004 → 0,0015. C'est la signature visible du O(n²).

La fusion, elle, suit à peine la croissance de n. Et l'écart continue de se
creuser : à 200 000 éléments, l'insertion demanderait plusieurs secondes quand
la fusion resterait sous le centième.

**C'est ça, la différence entre O(n²) et O(n log n)** — pas une subtilité
théorique, un facteur qui grandit sans limite.

### À faire

1. Écris `echanger`, puis le tri par sélection.
2. Écris le tri par insertion.
3. Mesure les deux avec `clock()` sur 1000, 10 000, 100 000 éléments aléatoires.
   Note les temps : tu dois voir le temps **quadrupler** quand n double.
4. Écris le tri fusion et refais la mesure.

→ Leçons complètes : [03_tris_elementaires.md](lessons/03_tris_elementaires.md) ·
[04_tris_efficaces.md](lessons/04_tris_efficaces.md)

---

# Partie 4 — La récursivité

*Aucune notion C nouvelle. Accessible dès maintenant.*

## L'idée

Une fonction qui s'appelle elle-même, sur un problème plus petit.

```c
long factorielle(int n) {
    if (n <= 1) {
        return 1;              // CAS DE BASE — sans lui, boucle infinie
    }
    return n * factorielle(n - 1);   // CAS RÉCURSIF
}
```

**Deux règles absolues :**

1. Toujours un **cas de base** qui s'arrête
2. Chaque appel doit **s'en rapprocher**

## Ce qui se passe en mémoire

Chaque appel empile un **cadre** contenant ses variables locales :

```
factorielle(4)
  └─ factorielle(3)
       └─ factorielle(2)
            └─ factorielle(1) → 1        ← cas de base atteint
       ← 2 × 1 = 2
  ← 3 × 2 = 6
← 4 × 6 = 24
```

C'est la **pile d'appels**. Elle est physique : un registre du processeur suit
son sommet, chaque appel le décale.

**Conséquence :** elle a une taille limitée (~8 Mo). Une récursion trop profonde
la déborde → *stack overflow*.

```c
long f(int n) { return f(n - 1); }   // pas de cas de base → crash
```

## Le piège : Fibonacci naïf

```c
long fibo(int n) {
    if (n <= 1) return n;
    return fibo(n - 1) + fibo(n - 2);
}
```

Élégant, et **catastrophique** : O(2ⁿ). `fibo(50)` prendrait des années.

Pourquoi ? Les mêmes valeurs sont recalculées des milliards de fois :

```
        fibo(5)
       /        \
   fibo(4)      fibo(3)      ← fibo(3) calculé deux fois
   /     \      /     \
fibo(3) fibo(2) ...          ← et encore
```

**La solution — mémoïsation :** retenir ce qu'on a déjà calculé.

```c
long fibo_memo(int n, long cache[]) {
    if (n <= 1)         return n;
    if (cache[n] != -1) return cache[n];      // déjà calculé

    cache[n] = fibo_memo(n - 1, cache) + fibo_memo(n - 2, cache);
    return cache[n];
}
```

O(2ⁿ) devient **O(n)**. Même algorithme, un tableau de plus.

Ou simplement une boucle, en O(n) et O(1) d'espace :

```c
long fibo_iter(int n) {
    long a = 0, b = 1;
    for (int i = 0; i < n; i++) {
        long tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}
```

**La leçon :** la récursivité est un outil d'expression, pas de performance.
Elle est indispensable pour les arbres et les graphes — où la structure elle-même
est récursive.

## À faire

1. Factorielle récursive, puis itérative.
2. `fibo` naïf : chronomètre `fibo(30)`, `fibo(35)`, `fibo(40)`. Constate l'explosion.
3. Ajoute la mémoïsation. Refais la mesure.
4. Écris une fonction récursive qui inverse une chaîne.
5. Les tours de Hanoï — l'exercice classique, trois lignes en récursif.

→ Leçon complète : [05_recursivite.md](lessons/05_recursivite.md)

---

# Partie 5 — Structures et mémoire dynamique

*C'est la porte d'entrée de tout ce qui suit. Deux notions C, puis quatre
structures de données.*

## ┌─ LA NOTION C (1/2) : les structures ─┐

Regrouper des données qui vont ensemble :

```c
typedef struct {
    char   nom[50];
    int    age;
} Etudiant;

Etudiant e = {.nom = "Yann", .age = 30};
printf("%s\n", e.nom);
```

Passée à une fonction, on utilise un **pointeur** et la flèche `->` :

```c
void afficher(const Etudiant *e) {
    printf("%s, %d ans\n", e->nom, e->age);   // e->nom  ≡  (*e).nom
}
afficher(&e);
```

### La structure qui se référence elle-même

C'est **la** brique des structures chaînées :

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *suivant;     // note bien : struct Noeud, pas Noeud
} Noeud;
```

Pourquoi `struct Noeud` à l'intérieur ? Parce qu'à cette ligne, le `typedef`
n'est pas terminé — le nom court `Noeud` n'existe pas encore. Le nom
`struct Noeud`, lui, est déclaré dès la première ligne.

→ Leçon complète : [09_structures.md](../1.Learn_C/lessons/09_structures.md)

## ┌─ LA NOTION C (2/2) : malloc et free ─┐

Jusqu'ici, toutes tes tailles étaient fixées à la compilation. `malloc` lève
cette contrainte.

```c
#include <stdlib.h>

int *tab = malloc(n * sizeof(int));    // n décidé à l'exécution
if (tab == NULL) {                     // TOUJOURS vérifier
    return 1;
}
// ... utiliser ...
free(tab);                             // TOUJOURS libérer
tab = NULL;
```

### Pile et tas

| | Pile | Tas (`malloc`) |
|---|---|---|
| Libération | automatique | **à toi** |
| Taille | fixe, ~8 Mo | limitée par la RAM |
| Durée de vie | le bloc courant | jusqu'au `free` |
| Taille connue | à la compilation | à l'exécution |

**Le tas est ce qui permet une structure qui grandit.** Une liste chaînée alloue
un nœud à chaque insertion — impossible sur la pile.

### Les quatre règles

```c
1. malloc → vérifier NULL
2. écrire le free EN MÊME TEMPS que le malloc
3. tab = NULL après free
4. compiler avec -fsanitize=address
```

Les quatre bugs qu'elles évitent : fuite, double `free`, usage après libération,
débordement de tas.

→ Leçon complète : [08_memoire_dynamique.md](../1.Learn_C/lessons/08_memoire_dynamique.md)

## └─ L'ALGORITHME (1/4) : listes chaînées ─┘

Des nœuds éparpillés en mémoire, reliés par des pointeurs :

```
tête → [12|•] → [7|•] → [23|•] → NULL
```

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *suivant;
} Noeud;

/* Renvoie la nouvelle tête. NULL si l'allocation échoue. */
Noeud *inserer_tete(Noeud *tete, int valeur) {
    Noeud *n = malloc(sizeof(Noeud));
    if (n == NULL) {
        return tete;                   // échec : liste inchangée
    }
    n->valeur  = valeur;
    n->suivant = tete;                 // l'ancien premier devient le second
    return n;                          // le nouveau nœud est la tête
}

void liberer(Noeud *tete) {
    while (tete != NULL) {
        Noeud *suivant = tete->suivant;   // sauvegarder AVANT de libérer
        free(tete);
        tete = suivant;
    }
}
```

**Le piège de `liberer` :** si tu fais `free(tete)` puis `tete = tete->suivant`,
tu lis une zone déjà libérée. Il faut sauvegarder le pointeur suivant d'abord.

### Tableau contre liste chaînée

| Opération | Tableau | Liste chaînée |
|---|---|---|
| Accès à l'élément i | **O(1)** | O(n) |
| Insertion en tête | O(n) | **O(1)** |
| Insertion en fin | O(1)* | O(n) |
| Mémoire | contiguë | éparpillée |

*Le vrai départage n'est pas dans ce tableau.* Le tableau est **contigu**, donc
le processeur charge les éléments voisins d'avance dans son cache. La liste
saute d'une adresse à l'autre, et chaque saut peut coûter un accès à la RAM —
cent fois plus lent.

**En pratique, le tableau gagne bien plus souvent que la théorie ne le suggère.**

→ Leçon complète : [06_listes_chainees.md](lessons/06_listes_chainees.md)

## └─ L'ALGORITHME (2/4) : piles et files ─┘

Deux structures qui ne diffèrent que par **où l'on retire**.

### Pile (LIFO — dernier entré, premier sorti)

```
push(1) push(2) push(3)  →  [1][2][3]
pop() → 3
```

```c
typedef struct { int donnees[100]; int sommet; } Pile;

void pile_init(Pile *p)        { p->sommet = -1; }
int  pile_vide(const Pile *p)  { return p->sommet == -1; }

int pile_empiler(Pile *p, int v) {
    if (p->sommet >= 99) return 0;        // pleine
    p->donnees[++p->sommet] = v;
    return 1;
}

int pile_depiler(Pile *p, int *out) {
    if (pile_vide(p)) return 0;
    *out = p->donnees[p->sommet--];
    return 1;
}
```

Toutes les opérations en **O(1)**.

**Où tu l'as déjà rencontrée :** la pile d'appels de la partie 4. Chaque appel
empile, chaque `return` dépile. Ce n'est pas une métaphore — c'est la même
structure.

Ses usages : annuler/refaire, vérifier des parenthèses, parcours en profondeur,
évaluation d'expressions.

### File (FIFO — premier entré, premier sorti)

Une file d'attente. On ajoute à la fin, on retire au début.

Ses usages : ordonnancement des processus par le noyau, tampons réseau,
**parcours en largeur** d'un graphe.

→ Leçon complète : [07_piles_et_files.md](lessons/07_piles_et_files.md)

## └─ L'ALGORITHME (3/4) : arbres binaires de recherche ─┘

Chaque nœud a deux enfants, avec un **invariant** :

> tout ce qui est à gauche est plus petit, tout ce qui est à droite est plus grand

```
          8
        /   \
       3     10
      / \      \
     1   6      14
```

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *gauche;
    struct Noeud *droite;
} Noeud;

Noeud *inserer(Noeud *racine, int v) {
    if (racine == NULL) {                    // place trouvée
        Noeud *n = malloc(sizeof(Noeud));
        if (n == NULL) return NULL;
        n->valeur = v;
        n->gauche = n->droite = NULL;
        return n;
    }
    if (v < racine->valeur) {
        racine->gauche = inserer(racine->gauche, v);
    } else if (v > racine->valeur) {
        racine->droite = inserer(racine->droite, v);
    }
    return racine;
}

void parcours_infixe(const Noeud *n) {
    if (n == NULL) return;
    parcours_infixe(n->gauche);
    printf("%d ", n->valeur);              // entre les deux = ordre CROISSANT
    parcours_infixe(n->droite);
}
```

**Le parcours infixe sort les valeurs triées.** C'est une conséquence directe de
l'invariant.

Note comme la récursivité devient naturelle ici : la structure est elle-même
récursive (un arbre contient des arbres), donc l'algorithme l'est aussi.

### La condition de performance

| Cas | Forme | Coût |
|---|---|---|
| Équilibré | touffu | O(log n) |
| Dégénéré | une ligne | O(n) |

Insérer `1, 2, 3, 4, 5` dans l'ordre produit une liste chaînée déguisée. D'où
les arbres **auto-équilibrants** (AVL, rouge-noir) qui se réorganisent tout
seuls — c'est ce qu'utilise `std::map` en C++.

→ Leçon complète : [08_arbres_binaires.md](lessons/08_arbres_binaires.md)

## └─ L'ALGORITHME (4/4) : tables de hachage ─┘

**La structure la plus utilisée en informatique** — c'est le `dict` de Python,
l'`Object` de JavaScript, la `HashMap` de Java.

L'idée : une fonction transforme la clé en indice de tableau.

```c
unsigned long hacher(const char *cle) {
    unsigned long h = 5381;
    while (*cle) {
        h = h * 33 + (unsigned char)*cle++;    // djb2
    }
    return h;
}

int indice = hacher("Yann") % TAILLE;      // → une case du tableau
```

Accès en **O(1) en moyenne** : on calcule, on va directement à la case. Pas de
parcours.

### Les collisions

Deux clés différentes peuvent donner le même indice. Solution la plus courante —
le **chaînage** : chaque case contient une liste chaînée.

```
case 0 → NULL
case 1 → ["Yann"|42] → ["Alex"|17] → NULL     ← collision
case 2 → ["Sam"|8]  → NULL
```

Tu vois pourquoi les listes chaînées viennent avant : elles sont un **composant**
de la table de hachage.

| | Moyenne | Pire cas |
|---|---|---|
| Recherche | O(1) | O(n) |
| Insertion | O(1) | O(n) |

Le pire cas survient si toutes les clés collisionnent — d'où l'importance d'une
bonne fonction de hachage.

**Le compromis :** O(1) contre l'ordre. Une table de hachage ne connaît aucun
ordre entre ses clés. Si tu as besoin de parcourir dans l'ordre, il te faut un
arbre.

→ Leçon complète : [09_tables_de_hachage.md](lessons/09_tables_de_hachage.md)

---

# Partie 6 — Les graphes

*Rien de nouveau en C. Tout ce qui précède converge ici.*

## Ce que c'est

Des **sommets** reliés par des **arêtes**. Un réseau routier, un réseau social,
des dépendances entre paquets, le web lui-même.

Deux représentations :

```
Matrice d'adjacence          Listes d'adjacence
    A B C                    A → [B, C]
A [ 0 1 1 ]                  B → [A]
B [ 1 0 0 ]                  C → [A]
C [ 1 0 0 ]

O(V²) mémoire                O(V + E) mémoire
test d'arête O(1)            test d'arête O(degré)
bon si dense                 bon si creux ← le cas courant
```

## Les deux parcours

Ils ne diffèrent que par **la structure d'attente** — et c'est tout le sujet.

### Parcours en profondeur (DFS) — avec une pile

```c
void dfs(int graphe[][MAX], int n, int sommet, int visite[]) {
    visite[sommet] = 1;
    printf("%d ", sommet);

    for (int i = 0; i < n; i++) {
        if (graphe[sommet][i] && !visite[i]) {
            dfs(graphe, n, i, visite);      // la récursion EST la pile
        }
    }
}
```

Va au plus profond avant de revenir. Usages : détection de cycles, tri
topologique, résolution de labyrinthes.

### Parcours en largeur (BFS) — avec une file

Explore niveau par niveau. **Sur un graphe non pondéré, il trouve le plus court
chemin** — c'est sa propriété centrale.

Usages : plus court chemin, degrés de séparation dans un réseau social,
diffusion.

### La différence tient à une ligne

```
DFS → pile → "je vais au plus loin"
BFS → file → "j'explore tout autour d'abord"
```

Même code, structure d'attente différente. **C'est là que piles et files
révèlent leur raison d'être.**

## Le tableau de convergence

| Ce que tu as appris | Où ça sert dans les graphes |
|---|---|
| Tableaux | matrice d'adjacence, tableau `visite[]` |
| Listes chaînées | listes d'adjacence |
| Pile | DFS |
| File | BFS |
| Récursivité | DFS récursif |
| `malloc` | graphes de taille variable |
| Complexité | O(V + E) contre O(V²) |

**Rien de nouveau — tout est réutilisé.** C'est pour ça que les graphes sont en
dernier.

→ Leçon complète : [10_graphes.md](lessons/10_graphes.md)

---

# Ce qu'il te reste à faire

## L'ordre concret

```
AUJOURD'HUI          partie 1 (complexité) + partie 4 (récursivité)
                     ↓ ne demandent rien de plus que ce que tu sais

leçon C 06           tableaux
   └──►              partie 2 (recherche) + partie 3 (tris)

leçon C 07           pointeurs
leçon C 08           malloc
leçon C 09           structures
   └──►              partie 5 (listes, piles, arbres, hachage)
                     partie 6 (graphes)
```

**Quatre leçons C débloquent tout le module algo.**

## Les trois idées à retenir de ce document

**1. La complexité prime sur le code.** Un O(n log n) mal écrit bat un O(n²)
optimisé, dès que n grandit. Choisis l'algorithme avant d'optimiser les lignes.

**2. Chaque structure est un compromis.** Tableau : accès rapide, insertion
lente. Liste : l'inverse. Hachage : O(1) mais aucun ordre. Arbre : O(log n) et
l'ordre conservé. **Il n'y a pas de meilleure structure, seulement une meilleure
pour ton usage.**

**3. En C, tu vois le compromis.** Un `dict` Python cache sa table de hachage.
Ici, tu l'écris — collisions comprises. C'est plus long, et c'est exactement
pourquoi ton parcours passe par le C.

## Pour commencer maintenant

Ces cinq programmes ne demandent que tes leçons 01 à 03 :

1. Somme de 1 à n : par boucle O(n), puis par formule O(1)
2. Compte les divisions par 2 jusqu'à 1 — tu obtiens O(log n)
3. Factorielle récursive et itérative
4. `fibo` naïf chronométré sur 30, 35, 40 — vois l'explosion exponentielle
5. Devine le nombre : le programme tire entre 1 et 1000, tu proposes, il répond
   « plus grand / plus petit ». **Tu implémentes une dichotomie sans tableau.**

Le cinquième est le plus formateur. Quand tu arriveras à la partie 2, tu
reconnaîtras le mécanisme — tu l'auras déjà écrit.
