# Les pointeurs

La leçon charnière du parcours. Tout ce qui précède y converge, tout ce qui
suit en dépend.

Suite de [06_tableaux_et_chaines.md](06_tableaux_et_chaines.md).

---

## 1. L'idée, en une phrase

> **La mémoire est un immense tableau d'octets numérotés. Un pointeur est un
> numéro de case.**

C'est tout. Il n'y a rien de plus à comprendre sur ce qu'est un pointeur. La
difficulté du sujet ne vient pas du concept — elle vient de la syntaxe, qui
réutilise le même symbole `*` pour deux choses différentes.

Reprends le modèle de la leçon 05 :

```
adresse:  0x1000  0x1004  0x1008  0x100C
contenu:  [  42  ][  17  ][  99  ][ ...  ]
nom:         n       m       k
```

Si `n` est à l'adresse `0x1000`, alors :
- **la valeur de `n`** est `42`
- **l'adresse de `n`** est `0x1000`

Un pointeur, c'est une variable qui stocke `0x1000`.

---

## 2. Les deux opérateurs

```c
int n = 42;
int *p = &n;      // p contient l'ADRESSE de n
```

| Opérateur | Nom | Lit | Fait |
|---|---|---|---|
| `&n` | *adresse de* | « adresse de n » | donne où est `n` |
| `*p` | *déréférencement* | « ce qui est à p » | va chercher la valeur |

Les deux sont **inverses l'un de l'autre** :

```c
int n = 42;
int *p = &n;

printf("%d\n",  n);    // 42     — la valeur
printf("%p\n", &n);    // 0x16f... — l'adresse
printf("%p\n",  p);    // la même adresse : p contient &n
printf("%d\n", *p);    // 42     — on suit le pointeur
```

Schéma :

```
     n                        p
  ┌──────┐                 ┌────────┐
  │  42  │  ← ─────────────│ 0x1000 │
  └──────┘      *p         └────────┘
   0x1000                    0x2000
      ↑                          
      └────── &n ────────────────┘
```

---

## 3. Le piège de la syntaxe

Le `*` a **deux sens différents** selon l'endroit. C'est la seule vraie
difficulté du sujet.

```c
int *p = &n;    // ICI : déclaration. "p est un pointeur vers int"
*p = 100;       // ICI : déréférencement. "écris 100 là où p pointe"
```

Aide mémoire : lis la déclaration `int *p` comme **« `*p` est un `int` »**.
Autrement dit : ce qu'on obtient en déréférençant `p`, c'est un `int`.

Autre conséquence, sur les déclarations multiples :

```c
int *a, b;      // a est un pointeur, b est un int ORDINAIRE
int *a, *b;     // les deux sont des pointeurs
```

Le `*` s'attache à la variable, pas au type. C'est pourquoi beaucoup écrivent
`int *p` plutôt que `int* p` — la seconde forme suggère à tort que le `*` fait
partie du type.

---

## 4. Le type du pointeur compte

```c
int    *pi;    // pointe vers 4 octets, à lire comme un int
char   *pc;    // pointe vers 1 octet
double *pd;    // pointe vers 8 octets, à lire comme un IEEE 754
```

Tous font **8 octets** sur ton Mac (ils contiennent tous une adresse 64 bits) :

```c
printf("%zu %zu %zu\n", sizeof(pi), sizeof(pc), sizeof(pd));   // 8 8 8
```

Alors à quoi sert le type ? À deux choses :

1. **Savoir combien d'octets lire** au déréférencement
2. **L'arithmétique de pointeurs** (section suivante)

C'est exactement l'idée de la leçon 05, appliquée aux pointeurs : le type est
une **convention de lecture** posée sur des octets.

D'où le cast que tu as déjà utilisé pour le boutisme :

```c
int n = 0x12345678;
unsigned char *p = (unsigned char *)&n;   // même adresse, lunette différente
```

---

## 5. Arithmétique de pointeurs

```c
int tab[5] = {10, 20, 30, 40, 50};
int *p = tab;      // pas de & : un tableau est déjà une adresse

printf("%d\n", *p);        // 10
printf("%d\n", *(p + 1));  // 20
printf("%d\n", *(p + 3));  // 40
```

**`p + 1` n'ajoute pas 1 à l'adresse.** Il ajoute `sizeof(int)`, soit 4 octets.
Le compilateur multiplie automatiquement par la taille du type pointé.

```
p       →  0x1000    *p       = 10
p + 1   →  0x1004    *(p+1)   = 20     (+4, pas +1)
p + 2   →  0x1008    *(p+2)   = 30
```

Avec un `char *`, `p + 1` avancerait de 1 octet. Avec un `double *`, de 8.

### L'équivalence fondamentale

```c
tab[i]   ≡   *(tab + i)
```

**Les crochets ne sont que du sucre syntaxique.** Le compilateur traduit l'un en
l'autre. C'est ce qui explique la décroissance vue en leçon 06 : `tab` est déjà
une adresse, donc `tab[i]` fonctionne aussi bien sur un pointeur.

Curiosité qui découle de l'équivalence : `tab[2]` et `2[tab]` sont tous deux
valides et identiques, puisque `*(tab+2) == *(2+tab)`. À ne jamais écrire, mais
ça prouve que l'indexation est bien de l'addition.

---

## 6. Le vrai usage : modifier une variable de l'appelant

Rappelle-toi ta question sur `divide` : comment renvoyer à la fois un résultat
et un statut d'erreur ? Voici la réponse.

En C, les paramètres sont **copiés**. Une fonction ne peut pas modifier une
variable de l'appelant :

```c
void incrementer(int x) {
    x = x + 1;         // modifie la COPIE locale
}

int n = 5;
incrementer(n);
printf("%d\n", n);     // 5 — inchangé
```

Avec un pointeur, on transmet l'adresse — et la fonction va écrire à la source :

```c
void incrementer(int *x) {
    *x = *x + 1;       // écrit LÀ OÙ x pointe
}

int n = 5;
incrementer(&n);       // on passe l'adresse
printf("%d\n", n);     // 6 — modifié
```

**C'est pour ça que `scanf` exige un `&`** : `scanf("%d", &age)` lui donne
l'adresse où écrire. Sans le `&`, il recevrait la valeur de `age` et la
prendrait pour une adresse — segfault.

### Appliqué à ta calculatrice

```c
// 1 = succès, 0 = échec ; le résultat sort par le pointeur
int divide(double x, double y, double *resultat) {
    if (y == 0.0) {
        return 0;
    }
    *resultat = x / y;
    return 1;
}
```

Usage :

```c
double q;
if (divide(10.0, 5.0, &q)) {
    printf("Quotient : %.2f\n", q);
} else {
    fprintf(stderr, "Erreur : division par zero\n");
}
```

Plus d'ambiguïté : le statut et la valeur voyagent par deux canaux distincts.
**C'est le pattern standard du C**, celui que tu retrouveras dans toute la
bibliothèque système.

### Renvoyer plusieurs valeurs

Même mécanisme :

```c
void min_max(const int tab[], int n, int *min, int *max) {
    *min = *max = tab[0];
    for (int i = 1; i < n; i++) {
        if (tab[i] < *min) *min = tab[i];
        if (tab[i] > *max) *max = tab[i];
    }
}

int mn, mx;
min_max(notes, 5, &mn, &mx);
```

---

## 7. `NULL`

`NULL` est l'adresse « nulle part ». C'est la valeur qu'on donne à un pointeur
qui ne pointe sur rien de valide.

```c
int *p = NULL;

if (p != NULL) {
    printf("%d\n", *p);     // sûr : on a vérifié
}

printf("%d\n", *p);          // SEGFAULT : déréférencer NULL
```

**Les deux règles :**

1. Initialise tout pointeur — à une adresse valide ou à `NULL`
2. Vérifie avant de déréférencer, quand la valeur peut être `NULL`

C'est aussi la convention de retour d'erreur des fonctions qui renvoient un
pointeur : `malloc`, `fopen`, `strchr` renvoient `NULL` en cas d'échec.

---

## 8. Les pointeurs dangereux

Trois façons d'avoir un pointeur qui compile mais casse.

### Le pointeur non initialisé

```c
int *p;         // contient une adresse aléatoire
*p = 42;        // écrit n'importe où — UB
```

**Toujours** `int *p = NULL;`

### Le pointeur pendant (*dangling*)

Le plus vicieux, parce qu'il semble marcher :

```c
int *fonction_dangereuse(void) {
    int local = 42;
    return &local;      // l'adresse d'une variable qui va disparaître
}

int *p = fonction_dangereuse();
printf("%d\n", *p);      // UB : la case a été rendue
```

`local` vit dans le **cadre de pile** de la fonction. Au `return`, ce cadre est
dépilé et la mémoire redevient disponible. Le pointeur reste valide
syntaxiquement, mais désigne une case qui sera réutilisée au prochain appel.

Souvent, ça « marche » en test et casse en production. `-Wall` te prévient
(`function returns address of local variable`) — lis tes avertissements.

### Le pointeur hors borne

```c
int tab[5];
int *p = &tab[10];    // adresse calculée hors du tableau
*p = 42;              // corruption
```

Même problème qu'en leçon 06, exprimé avec des pointeurs.

---

## 9. `const` et les pointeurs

Trois positions possibles, trois sens différents :

```c
const int *p;          // le POINTÉ est constant : *p = 5 interdit, p = &x autorisé
int *const p = &n;     // le POINTEUR est constant : p = &x interdit, *p = 5 autorisé
const int *const p = &n;  // les deux
```

**Lis de droite à gauche** en partant du nom : `int *const p` → « p est un
`const` pointeur vers `int` ».

En pratique, la première forme est de loin la plus fréquente :

```c
size_t ma_strlen(const char *s);      // je promets de ne pas modifier s
void afficher(const int *tab, int n); // idem
```

**Réflexe :** `const` sur tout paramètre pointeur que la fonction ne modifie pas.
Le compilateur vérifie ta promesse, et le lecteur sait immédiatement ce que la
fonction fait ou ne fait pas.

---

## 10. Pointeurs et chaînes

Tu peux maintenant relire la leçon 06 avec les bons yeux :

```c
char  a[] = "Yann";     // TABLEAU : 5 octets copiés dans TON espace
const char *b = "Yann"; // POINTEUR vers le littéral, en lecture seule
```

Réimplémentons `strlen` en style pointeur :

```c
size_t ma_strlen(const char *s) {
    const char *debut = s;
    while (*s != '\0') {    // tant que le caractère pointé n'est pas le zéro
        s++;                // avance d'un char
    }
    return s - debut;       // différence de deux pointeurs = nombre d'éléments
}
```

Note la dernière ligne : **soustraire deux pointeurs donne le nombre
d'éléments** entre eux, pas le nombre d'octets. Le compilateur divise par
`sizeof` du type pointé.

---

## 11. Pointeurs de fonction

Une fonction est du code, et le code est en mémoire. Donc une fonction a une
adresse.

```c
double add(double x, double y)      { return x + y; }
double multiply(double x, double y) { return x * y; }

double (*operation)(double, double);   // un pointeur vers une telle fonction

operation = add;
printf("%.2f\n", operation(3, 4));      // 7.00

operation = multiply;
printf("%.2f\n", operation(3, 4));      // 12.00
```

Lecture de `double (*operation)(double, double)` : « `operation` est un pointeur
vers une fonction prenant deux `double` et renvoyant un `double` ». Les
parenthèses autour de `*operation` sont obligatoires.

### Appliqué à ta calculatrice

Une table remplace une longue chaîne de `if` :

```c
typedef struct {
    char symbole;
    double (*fn)(double, double);
} Operation;

Operation table[] = {
    {'+', add},
    {'-', subtract},
    {'*', multiply},
    {'/', divide},
};

for (size_t i = 0; i < sizeof(table)/sizeof(table[0]); i++) {
    if (table[i].symbole == op) {
        resultat = table[i].fn(a, b);
        break;
    }
}
```

C'est ainsi que `qsort` accepte n'importe quel critère de tri, et c'est le
mécanisme qui, dans un noyau, permet à un pilote de s'enregistrer auprès du
système.

---

## 12. Ce que ça t'apprend de la machine

C'est la section la plus importante du module.

### Les adresses que tu vois sont virtuelles

Lance ce programme **deux fois** :

```c
#include <stdio.h>

int main(void) {
    int n = 42;
    printf("%p\n", (void *)&n);
    return 0;
}
```

L'adresse change à chaque exécution. Deux enseignements :

**1. Chaque processus a son propre espace d'adressage.** Ton `0x16f4b3a2c`
n'est pas l'adresse physique en RAM. Le processeur possède une unité dédiée, la
**MMU** (*Memory Management Unit*), qui traduit chaque adresse virtuelle en
adresse physique, via des tables tenues par le noyau.

Conséquence : deux programmes peuvent utiliser la même adresse virtuelle sans se
marcher dessus — elles pointent vers des pages physiques différentes. **C'est
l'isolation des processus, et c'est la fonction centrale d'un système
d'exploitation.**

**2. L'adresse change à chaque lancement à cause de l'ASLR** (*Address Space
Layout Randomization*). Le noyau place volontairement la pile et le tas à des
adresses aléatoires, pour qu'un attaquant exploitant un débordement ne puisse
pas prédire où sauter. C'est une défense directe contre ce que tu as vu en
leçon 06.

### Un segfault, c'est quoi exactement

Une adresse virtuelle n'a de sens que si elle est **mappée** vers une page
physique. Quand tu déréférences un pointeur invalide :

1. Le CPU demande la traduction à la MMU
2. La MMU ne trouve pas de mapping valide (ou trouve une page interdite en écriture)
3. Le CPU déclenche une **exception matérielle** (*page fault*)
4. Le noyau prend la main, constate que l'accès est illégitime
5. Le noyau envoie le signal **SIGSEGV** à ton processus
6. Le comportement par défaut de SIGSEGV est de tuer le processus

« Segmentation fault » n'est donc pas un message du C : c'est le nom d'un
signal Unix, déclenché par une exception matérielle. **Ton programme a été tué
par le noyau, pas par le langage.**

C'est pourquoi `b[0] = 'J'` sur un littéral segfaulte : la page du segment
`text` est marquée en lecture seule dans la table des pages. Le matériel
refuse l'écriture.

### La pile est une vraie pile

Chaque appel de fonction **pousse** un cadre contenant les variables locales,
les paramètres, et l'adresse de retour. Chaque `return` le **dépile**.

C'est un registre du processeur (le *stack pointer*) qui suit le sommet. Pousser
un cadre, c'est simplement le décrémenter. D'où :

- l'allocation d'une variable locale est **quasi gratuite**
- le pointeur pendant de la section 8 : la case est réellement rendue
- le **débordement de pile** (*stack overflow*) d'une récursion infinie : la
  pile a une taille fixe, ~8 Mo par défaut

### Le code est en mémoire comme les données

Le pointeur de fonction le prouve. Cette unité entre code et données est
l'**architecture de von Neumann**, le principe fondateur de tous les ordinateurs
courants.

C'est aussi ce qui rend possible les attaques par injection de code — et
pourquoi les systèmes modernes marquent la pile comme non-exécutable (bit NX).

---

## 13. Exercices

**Bases**

1. Déclare `int n = 42;` et `int *p = &n;`. Affiche `n`, `&n`, `p`, `*p`, `&p`.
   Explique par écrit ce que chaque ligne montre.

2. Écris `void echanger(int *a, int *b)` qui échange deux entiers. Teste-la.

3. Écris `void incrementer(int *n)`. Vérifie que la variable de `main` change.

**Tableaux et pointeurs**

4. Parcours un tableau de deux façons — `tab[i]` puis `*(tab + i)` — et vérifie
   que les sorties sont identiques.

5. Affiche `p`, `p+1`, `p+2` pour un `int *`, puis pour un `char *`. Constate
   l'écart de 4 contre 1.

6. Réimplémente `strlen` en style pointeur (section 10), puis `strcmp`.

**Le pattern statut + sortie**

7. Réécris ta fonction `divide` avec la signature
   `int divide(double x, double y, double *resultat)`. Adapte `main`.

8. Écris `void min_max(const int tab[], int n, int *min, int *max)`.

**Diagnostic**

9. Écris la fonction qui renvoie l'adresse d'une variable locale. Compile avec
   `-Wall` et lis l'avertissement. Puis exécute et observe.

10. Déréférence `NULL` volontairement. Lance sous `lldb` :
    ```sh
    lldb ./prog
    (lldb) run
    (lldb) bt
    ```
    Lis le message et la pile d'appels.

11. Lance dix fois le programme qui affiche `&n`. Note les adresses. Explique
    l'ASLR à voix haute.

**Aller plus loin**

12. Réécris la sélection d'opération de ta calculatrice avec une table de
    pointeurs de fonction (section 11).

---

## Mémo

```c
int n = 42;
int *p = &n;        // & : adresse de
*p = 100;           // * : ce qui est à cette adresse

int *p = NULL;      // toujours initialiser
if (p != NULL) *p;  // toujours vérifier avant de déréférencer

// équivalences
tab[i]  ≡  *(tab + i)
tab     ≡  &tab[0]

// arithmétique : avance de sizeof(type)
p + 1               // int* : +4 octets  |  char* : +1 octet
fin - debut         // nombre d'ÉLÉMENTS, pas d'octets

// modifier l'appelant
void f(int *x) { *x = 5; }
f(&n);

// statut + résultat
int f(double a, double b, double *out);

// const
const int *p        // pointé constant  ← le cas courant
int *const p        // pointeur constant

// pointeur de fonction
double (*op)(double, double) = add;
op(3, 4);

printf("%p", (void *)p);    // afficher une adresse
```

```sh
gcc -Wall -Wextra -g -fsanitize=address,undefined prog.c -o prog
lldb ./prog     →  run  |  bt  |  frame variable  |  p *p
```

**Les quatre réflexes :**

1. Un pointeur est un numéro de case mémoire — rien de plus
2. `int *p` se lit « `*p` est un int »
3. Initialise à `NULL`, vérifie avant de déréférencer
4. Ne jamais renvoyer l'adresse d'une variable locale
