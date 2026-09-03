# Piles et files

## Deux structures définies par ce qu'elles interdisent

Un tableau et une liste chaînée permettent tout : lire n'importe où, insérer
n'importe où. La pile et la file font l'inverse — elles **restreignent**
volontairement les accès.

C'est contre-intuitif : pourquoi choisir une structure qui en fait moins ?

Parce que la restriction est le service. Une pile garantit que le dernier entré
sortira en premier ; on peut donc s'appuyer sur cette garantie pour construire
un algorithme. Et parce que ces opérations restreintes sont toutes **O(1)** — il
n'y a pas de cas coûteux caché.

Ce sont les structures les plus utilisées en informatique système, précisément
pour ces deux raisons.

## Pile — LIFO

*Last In, First Out.* Le dernier arrivé est le premier servi. Une pile
d'assiettes : on pose dessus, on prend dessus.

Trois opérations :

- `push(x)` — empiler
- `pop()` — dépiler et renvoyer le sommet
- `peek()` — lire le sommet sans le retirer

Toutes en **O(1)**.

### Implémentation sur tableau

```c
#define CAPACITE 100

typedef struct {
    int donnees[CAPACITE];
    int sommet;              // indice du sommet ; -1 = pile vide
} Pile;

void pile_init(Pile *p) {
    p->sommet = -1;
}

bool pile_est_vide(const Pile *p) {
    return p->sommet == -1;
}

bool pile_est_pleine(const Pile *p) {
    return p->sommet == CAPACITE - 1;
}

bool pile_push(Pile *p, int valeur) {
    if (pile_est_pleine(p)) {
        return false;                  // débordement : l'appelant décide
    }
    p->donnees[++p->sommet] = valeur;
    return true;
}

bool pile_pop(Pile *p, int *sortie) {
    if (pile_est_vide(p)) {
        return false;
    }
    *sortie = p->donnees[p->sommet--];
    return true;
}
```

Deux choix de conception à relever.

**`sommet = -1` pour la pile vide**, plutôt que `0`. Cela permet d'écrire
`donnees[++sommet]` : on incrémente puis on écrit. Avec la convention
`sommet = 0` (« prochaine case libre »), on écrit `donnees[sommet++]`. Les deux
marchent ; les mélanger produit un décalage d'une case, le bug le plus fréquent
sur cette structure.

**Le retour booléen plutôt qu'une valeur sentinelle.** `pop` ne peut pas renvoyer
`-1` pour « pile vide », puisque `-1` est une valeur stockable. On renvoie donc
un succès et la valeur par pointeur. Cette contrainte revient dès qu'une
structure peut contenir toutes les valeurs du type.

### Implémentation sur liste chaînée

```c
typedef struct NoeudPile {
    int valeur;
    struct NoeudPile *suivant;
} NoeudPile;

bool pile_push(NoeudPile **sommet, int valeur) {
    NoeudPile *nouveau = malloc(sizeof(NoeudPile));
    if (nouveau == NULL) return false;
    nouveau->valeur = valeur;
    nouveau->suivant = *sommet;
    *sommet = nouveau;
    return true;
}
```

C'est exactement l'insertion en tête de la leçon précédente. **Une pile est une
liste chaînée où l'on s'interdit tout sauf la tête** — et l'insertion en tête est
justement la seule opération O(1) d'une liste chaînée. Les deux structures sont
faites l'une pour l'autre.

| | Tableau | Liste chaînée |
|---|---|---|
| Taille | fixe | dynamique |
| Vitesse | meilleure (cache) | `malloc` à chaque push |
| Mémoire | allouée d'emblée | 8 octets de plus par élément |
| Débordement | possible | seulement si la mémoire est pleine |

Sur tableau par défaut. Sur liste chaînée si la taille maximale est inconnue.

### Où l'on rencontre des piles

**La pile d'appels.** C'est celle de la leçon sur la récursivité. Chaque appel
`push` un cadre, chaque `return` le `pop`. Le débordement de pile est
littéralement un `push` sur une pile pleine.

**L'annulation (Ctrl+Z).** Chaque action est empilée ; annuler, c'est dépiler.
L'ordre LIFO est exactement la sémantique attendue.

**L'évaluation d'expressions.** Vérifier que des parenthèses sont équilibrées,
convertir en notation postfixée, évaluer. C'est l'application la plus classique.

**Le parcours en profondeur d'un graphe.** La version itérative du DFS utilise
une pile explicite, là où la version récursive utilise la pile d'appels. Ce sont
deux formulations de la même chose — on le verra en leçon 9.

### L'exemple canonique : les parenthèses équilibrées

```c
bool parentheses_equilibrees(const char *expr) {
    Pile p;
    pile_init(&p);

    for (int i = 0; expr[i] != '\0'; i++) {
        char c = expr[i];

        if (c == '(' || c == '[' || c == '{') {
            if (!pile_push(&p, c)) return false;
        }
        else if (c == ')' || c == ']' || c == '}') {
            int ouvrante;
            if (!pile_pop(&p, &ouvrante)) {
                return false;                       // fermante sans ouvrante
            }
            if ((c == ')' && ouvrante != '(') ||
                (c == ']' && ouvrante != '[') ||
                (c == '}' && ouvrante != '{')) {
                return false;                       // mauvais appariement
            }
        }
    }
    return pile_est_vide(&p);                       // reste-t-il des ouvrantes ?
}
```

Le `return pile_est_vide(&p)` final est essentiel : sans lui, `"((("` serait
accepté.

Pourquoi une pile est-elle la bonne structure ici ? Parce que les parenthèses
s'imbriquent : la prochaine fermante doit correspondre à la **dernière** ouvrante
non encore fermée. « La dernière ouverte » est la définition même du LIFO. Quand
la structure du problème correspond à celle de l'outil, le code s'écrit tout seul.

## File — FIFO

*First In, First Out.* Le premier arrivé est le premier servi. Une file d'attente.

- `enqueue(x)` — enfiler, à l'arrière
- `dequeue()` — défiler, à l'avant

Toutes deux en **O(1)** — si l'implémentation est correcte.

### La fausse bonne idée

Un tableau, avec `dequeue` qui décale tout d'un cran vers la gauche.

```c
// À NE PAS FAIRE
int dequeue_naif(Queue *q) {
    int valeur = q->donnees[0];
    for (int i = 0; i < q->taille - 1; i++) {
        q->donnees[i] = q->donnees[i + 1];      // O(n) à chaque défilement
    }
    q->taille--;
    return valeur;
}
```

O(n) par opération. Sur une file de 10 000 éléments entièrement vidée, cela fait
50 millions de déplacements — pour une structure censée être O(1).

### La bonne solution : le tableau circulaire

On ne décale rien. On fait avancer deux indices, qui reviennent à zéro en
arrivant au bout.

```c
#define CAPACITE 100

typedef struct {
    int donnees[CAPACITE];
    int tete;        // indice du prochain à sortir
    int queue;       // indice de la prochaine case libre
    int taille;      // nombre d'éléments
} File;

void file_init(File *f) {
    f->tete = 0;
    f->queue = 0;
    f->taille = 0;
}

bool file_enqueue(File *f, int valeur) {
    if (f->taille == CAPACITE) return false;

    f->donnees[f->queue] = valeur;
    f->queue = (f->queue + 1) % CAPACITE;      // le modulo fait le tour
    f->taille++;
    return true;
}

bool file_dequeue(File *f, int *sortie) {
    if (f->taille == 0) return false;

    *sortie = f->donnees[f->tete];
    f->tete = (f->tete + 1) % CAPACITE;
    f->taille--;
    return true;
}
```

Le `% CAPACITE` est tout l'algorithme. Arrivé à la fin du tableau, l'indice
repart à 0 et réutilise les cases libérées à l'avant.

```
Capacité 5, après 3 enqueue puis 2 dequeue puis 3 enqueue :

indice :   0    1    2    3    4
        [  F ][  G ][  . ][  D ][  E ]
              ▲          ▲
            queue       tete

La file contient D, E, F, G — dans cet ordre, en "faisant le tour".
```

**Pourquoi le champ `taille` ?** Sans lui, `tete == queue` est ambigu : file vide
ou file pleine ? Les deux donnent la même configuration d'indices. Deux remèdes
existent : garder un compteur (ci-dessus, le plus clair), ou sacrifier une case
en déclarant la file pleine quand `(queue + 1) % CAPACITE == tete`. Le compteur
est préférable — plus lisible, et pas de capacité perdue.

### Où l'on rencontre des files

**L'ordonnanceur du système.** Les processus prêts attendent leur tour dans une
file. L'ordre d'arrivée est l'équité de base.

**Les tampons d'entrée/sortie.** Le clavier, une socket réseau, un flux de
fichier : les octets arrivent dans un ordre et doivent être consommés dans le
même. Un tampon circulaire, exactement le code ci-dessus.

**Le parcours en largeur d'un graphe.** La file garantit qu'on visite tous les
voisins à distance 1 avant ceux à distance 2. C'est ce qui rend le BFS capable de
trouver le plus court chemin — leçon 9.

**Les files de messages.** RabbitMQ, Kafka, une file de tâches : c'est la
structure, à l'échelle du réseau.

## Variantes utiles

**Deque** (*double-ended queue*) : insertion et suppression aux deux bouts. Une
liste doublement chaînée, ou un tableau circulaire avec `tete` et `queue` qui
peuvent tous deux reculer. C'est la structure derrière `std::deque` et
`collections.deque`.

**File de priorité** : on ne sort pas le plus ancien, mais le plus prioritaire.
Implémentée sur un **tas binaire**, elle donne `enqueue` et `dequeue` en
O(log n). C'est la structure au cœur de l'algorithme de Dijkstra et des
ordonnanceurs modernes.

## Récapitulatif

| | Pile (LIFO) | File (FIFO) |
|---|---|---|
| Ajout | `push` — au sommet | `enqueue` — à l'arrière |
| Retrait | `pop` — au sommet | `dequeue` — à l'avant |
| Coût | O(1) | O(1) |
| Sur tableau | indice `sommet` | **circulaire, avec modulo** |
| Sur liste | insertion/suppression en tête | tête + pointeur de queue |
| Parcours de graphe | profondeur (DFS) | largeur (BFS) |
| Usage type | annulation, appels, expressions | ordonnancement, tampons |

## Ce qu'il faut retenir

- Ces structures valent par ce qu'elles **interdisent** : la restriction est la
  garantie.
- Toutes leurs opérations sont O(1). Une file dont le `dequeue` est O(n) est mal
  implémentée.
- Le **tableau circulaire** et son modulo sont la bonne implémentation d'une file.
- Une pile est une liste chaînée réduite à sa tête.
- Pile → DFS, File → BFS. Cette correspondance est la clé de la leçon sur les
  graphes.
- Toujours prévoir le cas « vide » et le cas « plein », et les signaler à
  l'appelant.

---

## À faire maintenant

**1.** Implémente la pile sur tableau et écris `parentheses_equilibrees`. Teste
avec `"({[]})"` (vrai), `"([)]"` (faux), `"((("` (faux), `""` (vrai).

**2.** Implémente la file circulaire avec une capacité de 5. Enfile 5 éléments,
défile 3, enfile 3 de plus. Affiche les indices `tete` et `queue` à chaque
opération et **observe le passage par zéro**.

**3.** Écris la version naïve du `dequeue` avec décalage. Chronomètre 100 000
défilements avec chacune des deux versions.

Le rapport que tu obtiendras est la différence entre O(1) et O(n), mesurée sur
une structure de dix lignes.
