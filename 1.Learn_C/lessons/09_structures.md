# Structures, unions, énumérations

Regrouper des données qui vont ensemble — et découvrir que la disposition en
mémoire n'est pas celle qu'on croit.

Suite de [08_memoire_dynamique.md](08_memoire_dynamique.md).

---

## 1. Le problème

Représenter un étudiant demande trois données liées :

```c
char nom[50];
int  age;
double moyenne;
```

Trois variables séparées, qu'il faut passer ensemble à chaque fonction, et
dupliquer pour chaque étudiant. Ça ne tient pas.

```c
struct Etudiant {
    char   nom[50];
    int    age;
    double moyenne;
};
```

Un `struct` est un **bloc de mémoire contenant plusieurs champs**, manipulable
comme une seule entité.

---

## 2. Déclarer et utiliser

```c
struct Etudiant e;                  // noter le mot-clé struct obligatoire

strcpy(e.nom, "Yann");
e.age = 30;
e.moyenne = 15.5;

printf("%s, %d ans, %.2f\n", e.nom, e.age, e.moyenne);
```

Initialisation directe :

```c
struct Etudiant e = {"Yann", 30, 15.5};              // dans l'ordre des champs

struct Etudiant e = {                                 // désigné (C99) — préférable
    .nom = "Yann",
    .age = 30,
    .moyenne = 15.5
};
```

**Préfère la forme désignée.** Elle est lisible, et elle survit à un
réordonnancement des champs.

---

## 3. `typedef` — supprimer le mot `struct`

```c
typedef struct {
    char   nom[50];
    int    age;
    double moyenne;
} Etudiant;

Etudiant e;              // plus besoin d'écrire struct
```

La forme avec nom, nécessaire dès qu'une structure se référence elle-même
(listes chaînées, arbres) :

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *suivant;    // ICI, le typedef n'existe pas encore
} Noeud;
```

C'est la brique de base des structures de données du module algo.

---

## 4. Structures et fonctions

Une structure est passée **par copie**, comme un `int` :

```c
void afficher(Etudiant e) {     // copie les ~64 octets à chaque appel
    printf("%s\n", e.nom);
}

void vieillir(Etudiant e) {
    e.age++;                     // modifie la COPIE — sans effet
}
```

D'où l'usage systématique du **pointeur vers structure**, avec l'opérateur `->` :

```c
void afficher(const Etudiant *e) {   // const : lecture seule
    printf("%s, %d ans\n", e->nom, e->age);
}

void vieillir(Etudiant *e) {
    e->age++;                         // modifie l'original
}

Etudiant e = {.nom = "Yann", .age = 30};
vieillir(&e);
```

### `.` et `->`

```c
e.age       // e est une structure
p->age      // p est un POINTEUR vers structure
```

`p->age` est exactement `(*p).age`. Les parenthèses sont obligatoires dans la
forme longue (`.` est plus prioritaire que `*`), et assez pénibles pour que la
flèche ait été créée.

**Réflexe :** passe toujours les structures par pointeur, avec `const` si tu ne
modifies pas. Ça évite la copie et rend l'intention explicite.

---

## 5. Le bourrage (*padding*)

Voici ce qui surprend, et qui est le vrai enseignement machine de cette leçon.

```c
typedef struct {
    char   a;      // 1 octet
    int    b;      // 4 octets
    char   c;      // 1 octet
} Exemple;

printf("%zu\n", sizeof(Exemple));    // 12, pas 6
```

**Pourquoi 12 ?** Le processeur lit la mémoire par blocs alignés. Un `int` de
4 octets doit commencer à une adresse multiple de 4 — sinon l'accès est plus
lent, voire interdit selon l'architecture.

Le compilateur insère donc des **octets de bourrage** :

```
offset:  0    1  2  3    4  5  6  7    8    9 10 11
        [a] [░░░░░░░]  [   b       ]  [c]  [░░░░░░░]
         1B   3B pad        4B         1B    3B pad
                                             └─ pour que sizeof soit
                                                multiple de l'alignement
```

Total : 12 octets, dont 6 de vide.

### Réordonner change tout

```c
typedef struct {
    int    b;      // 4
    char   a;      // 1
    char   c;      // 1
} Optimise;

printf("%zu\n", sizeof(Optimise));    // 8, pas 12
```

```
offset:  0  1  2  3    4    5    6  7
        [   b      ]  [a]  [c]  [░░░]
```

Un seul bourrage de 2 octets à la fin.

**Règle pratique :** déclare les champs **du plus grand au plus petit**. Sur une
structure isolée c'est anecdotique ; sur un tableau d'un million d'éléments,
c'est 4 Mo d'écart et un impact réel sur le cache.

### Le voir avec `offsetof`

```c
#include <stddef.h>

printf("a : %zu\n", offsetof(Exemple, a));    // 0
printf("b : %zu\n", offsetof(Exemple, b));    // 4  ← le trou est visible
printf("c : %zu\n", offsetof(Exemple, c));    // 8
```

**Conséquence importante :** ne suppose jamais que `sizeof(struct)` égale la
somme de ses champs. C'est le piège n°1 quand on écrit une structure dans un
fichier binaire ou qu'on l'envoie sur le réseau — le bourrage part avec, et
diffère d'une machine à l'autre.

---

## 6. Structures imbriquées et tableaux

```c
typedef struct {
    int jour, mois, annee;
} Date;

typedef struct {
    char nom[50];
    Date naissance;        // une structure dans une structure
} Personne;

Personne p;
p.naissance.annee = 1995;
```

Tableau de structures :

```c
Etudiant promo[30];
promo[0].age = 20;

for (int i = 0; i < 30; i++) {
    printf("%s\n", promo[i].nom);
}
```

Alloué dynamiquement :

```c
Etudiant *promo = malloc(n * sizeof(Etudiant));
if (promo == NULL) { ... }
promo[0].age = 20;
free(promo);
```

---

## 7. `enum`

Nommer un ensemble fini de valeurs.

```c
typedef enum {
    LUNDI,       // 0
    MARDI,       // 1
    MERCREDI     // 2
} Jour;

Jour j = MARDI;
```

Les valeurs sont des entiers, numérotés à partir de 0 par défaut. On peut les
fixer :

```c
typedef enum {
    OK              = 0,
    ERR_FICHIER     = 1,
    ERR_MEMOIRE     = 2,
    ERR_ARGUMENT    = 3
} Statut;
```

**L'usage qui t'intéresse tout de suite :** remplacer les codes de retour
anonymes de ta calculatrice.

```c
typedef enum { CALC_OK, CALC_DIV_ZERO, CALC_OP_INCONNU } CalcStatut;

CalcStatut divide(double x, double y, double *resultat) {
    if (y == 0.0) {
        return CALC_DIV_ZERO;
    }
    *resultat = x / y;
    return CALC_OK;
}
```

L'appelant lit `if (st == CALC_DIV_ZERO)` au lieu de `if (st == 2)`. Le
compilateur t'avertit aussi si un `switch` sur un `enum` oublie un cas — avec
`-Wall`, un vrai filet de sécurité.

---

## 8. `union`

Toutes les alternatives partagent **le même emplacement mémoire**.

```c
union Valeur {
    int    i;
    float  f;
    char   c[4];
};

printf("%zu\n", sizeof(union Valeur));   // 4 — la taille du plus grand champ
```

Écrire dans un champ écrase les autres :

```c
union Valeur v;
v.i = 1065353216;
printf("%f\n", v.f);      // 1.000000 — les MÊMES octets, lus en float
```

**C'est la démonstration la plus pure de la leçon 05 :** un type n'est qu'une
convention de lecture posée sur des octets. L'union rend cette idée visible
dans le langage.

Usage sérieux — l'union étiquetée, pour représenter « une valeur d'un type
parmi plusieurs » :

```c
typedef enum { T_ENTIER, T_REEL, T_TEXTE } TypeValeur;

typedef struct {
    TypeValeur type;         // l'étiquette dit quel champ est valide
    union {
        int    entier;
        double reel;
        char  *texte;
    } donnee;
} Valeur;

void afficher(const Valeur *v) {
    switch (v->type) {
        case T_ENTIER: printf("%d\n",  v->donnee.entier); break;
        case T_REEL:   printf("%f\n",  v->donnee.reel);   break;
        case T_TEXTE:  printf("%s\n",  v->donnee.texte);  break;
    }
}
```

C'est ainsi qu'un interpréteur représente ses valeurs — CPython fait
exactement ça pour les objets Python.

**Danger :** rien ne vérifie que tu lis le bon champ. Lire `v.f` après avoir
écrit `v.i` est légal mais donne du n'importe quoi si l'étiquette n'est pas
respectée. L'étiquette est une discipline, pas une garantie.

---

## 9. Un exemple complet

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char   nom[50];
    int    age;
    double moyenne;
} Etudiant;

typedef struct {
    Etudiant *etudiants;
    size_t    taille;
    size_t    capacite;
} Promo;

/* Renvoie NULL en cas d'échec. L'appelant doit appeler promo_liberer(). */
Promo *promo_creer(void) {
    Promo *p = malloc(sizeof(Promo));
    if (p == NULL) {
        return NULL;
    }
    p->capacite  = 4;
    p->taille    = 0;
    p->etudiants = malloc(p->capacite * sizeof(Etudiant));
    if (p->etudiants == NULL) {
        free(p);
        return NULL;
    }
    return p;
}

/* 1 si ajouté, 0 si échec d'allocation. */
int promo_ajouter(Promo *p, const char *nom, int age, double moyenne) {
    if (p->taille == p->capacite) {
        size_t nouvelle = p->capacite * 2;
        Etudiant *tmp = realloc(p->etudiants, nouvelle * sizeof(Etudiant));
        if (tmp == NULL) {
            return 0;
        }
        p->etudiants = tmp;
        p->capacite  = nouvelle;
    }

    Etudiant *e = &p->etudiants[p->taille];
    snprintf(e->nom, sizeof(e->nom), "%s", nom);
    e->age     = age;
    e->moyenne = moyenne;
    p->taille++;
    return 1;
}

double promo_moyenne(const Promo *p) {
    if (p->taille == 0) {
        return 0.0;
    }
    double somme = 0.0;
    for (size_t i = 0; i < p->taille; i++) {
        somme += p->etudiants[i].moyenne;
    }
    return somme / p->taille;
}

void promo_liberer(Promo *p) {
    if (p == NULL) {
        return;
    }
    free(p->etudiants);      // le tableau d'abord
    free(p);                 // la structure ensuite
}

int main(void) {
    Promo *p = promo_creer();
    if (p == NULL) {
        fprintf(stderr, "Allocation impossible\n");
        return 1;
    }

    promo_ajouter(p, "Yann",  30, 15.5);
    promo_ajouter(p, "Alex",  22, 12.0);
    promo_ajouter(p, "Sam",   25, 17.5);

    for (size_t i = 0; i < p->taille; i++) {
        printf("%-10s %3d ans  %5.2f\n",
               p->etudiants[i].nom,
               p->etudiants[i].age,
               p->etudiants[i].moyenne);
    }
    printf("Moyenne de la promo : %.2f\n", promo_moyenne(p));

    promo_liberer(p);
    return 0;
}
```

Remarque l'ordre des `free` dans `promo_liberer` : **de l'intérieur vers
l'extérieur**. Libérer `p` d'abord rendrait `p->etudiants` inaccessible — fuite
garantie. C'est une règle générale pour toute structure imbriquée.

Remarque aussi que ce programme combine tout le module : structures, pointeurs,
allocation dynamique, tableau qui double, `const`, propriété documentée.

---

## 10. Ce que ça t'apprend de la machine

**L'alignement est une contrainte matérielle, pas une convention.** Le
processeur accède à la mémoire par mots alignés. Un `int` à une adresse non
multiple de 4 demande deux accès au lieu d'un — et sur certaines architectures
ARM, provoque une exception. Le bourrage est le compilateur qui te protège de
ça, silencieusement.

**Une structure n'est qu'une disposition d'octets.** Il n'y a ni méthode, ni
héritage, ni vtable — juste des champs à des décalages fixes. `e->age` se
compile en « adresse de e + 52 ». Un objet C++ ou Java ajoute des couches
au-dessus de cette même idée.

**C'est ainsi qu'on parle au matériel.** En embarqué, un registre de
périphérique se déclare comme une structure placée à une adresse fixe :

```c
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)0x40020000)

GPIOA->ODR |= (1 << 5);      // allume une LED
```

Les décalages des champs **doivent** correspondre exactement à la fiche
technique du composant. C'est le module `6.Learn_dev_Embeded` en entier, et
c'est pourquoi le bourrage y est un sujet sérieux.

**Le bourrage voyage avec les données.** Écrire une structure telle quelle dans
un fichier ou sur une socket transmet aussi les octets de bourrage — non
initialisés, donc potentiellement des restes d'autres données. C'est une fuite
d'information réelle, qui a donné lieu à des CVE dans le noyau Linux. Pour la
sérialisation, on écrit champ par champ, explicitement.

---

## 11. Exercices

1. Écris `struct Exemple` de la section 5, affiche son `sizeof` et les
   `offsetof` de chaque champ. Retrouve les trous à la main.

2. Réordonne les champs pour minimiser la taille. Vérifie le gain.

3. Définis `Point` et écris `double distance(const Point *a, const Point *b)`
   (avec `sqrt` de `<math.h>`).

4. Définis `Rectangle` à partir de deux `Point`, et écris les fonctions
   `aire` et `perimetre` — la boucle est bouclée avec ta première leçon.

5. Reprends ta calculatrice : remplace les codes de retour par un `enum
   CalcStatut`. Écris un `switch` complet sur cet enum et vérifie que `-Wall`
   te prévient si tu enlèves un cas.

6. Écris l'union étiquetée de la section 8 et sa fonction d'affichage.

7. Reproduis la démonstration `v.i = 1065353216; printf("%f", v.f);`.
   Puis retrouve à la main, avec la structure IEEE 754 de la leçon 05,
   pourquoi ces octets valent exactement `1.0`.

8. Tape le programme complet de la section 9. Fais-le tourner sous
   `-fsanitize=address` et vérifie qu'il n'y a aucune fuite.

9. Ajoute à ce programme une fonction
   `Etudiant *promo_chercher(Promo *p, const char *nom)` qui renvoie un
   pointeur vers l'étudiant, ou `NULL`.

10. Découpe le programme de la section 9 en `promo.h` / `promo.c` / `main.c`,
    avec gardes d'inclusion et `Makefile`. C'est la synthèse des leçons 04 à 09.

---

## Mémo

```c
typedef struct {
    double grand;      // du plus grand au plus petit
    int    moyen;      // pour limiter le bourrage
    char   petit;
} Truc;

Truc t = {.grand = 1.0, .moyen = 2};    // initialisation désignée

t.champ         // sur une structure
p->champ        // sur un pointeur — équivaut à (*p).champ

void f(const Truc *t)      // toujours par pointeur, const si lecture seule

sizeof(Truc)               // != somme des champs (bourrage)
offsetof(Truc, champ)      // <stddef.h> — voir la disposition réelle

typedef enum { OK, ERR_A, ERR_B } Statut;   // codes de retour lisibles

union { int i; float f; };   // même mémoire, deux lectures

// libérer de l'intérieur vers l'extérieur
free(p->tableau);
free(p);
```

**Les quatre réflexes :**

1. Structures par pointeur, `const` si lecture seule
2. Champs du plus grand au plus petit
3. `sizeof(struct)` n'est pas la somme des champs
4. Libérer les champs alloués avant la structure elle-même
