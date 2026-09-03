# Variables et types

Un programme sans variable ne peut rien retenir. C'est ce qui manque à
`hello_world.c` pour devenir un vrai programme.

Suite de [02_compiler_executer_debugger.md](02_compiler_executer_debugger.md).

---

## 1. Une variable, c'est quoi

**Un emplacement en mémoire, désigné par un nom.**

```c
int age = 25;
```

Cette ligne demande trois choses au compilateur :

1. **Réserve** de la place en mémoire — 4 octets, parce que c'est un `int`
2. **Retiens** que cet emplacement s'appelle `age`
3. **Écris** la valeur 25 dedans

C'est déjà la différence majeure avec Python. En Python, `age = 25` crée un objet
et lui colle une étiquette. En C, tu réserves un espace physique d'une taille
décidée à l'avance, et tu y ranges un nombre.

Cette place a une **adresse**, que tu peux voir avec `&` :

```c
printf("valeur : %d\n", age);
printf("adresse : %p\n", (void *)&age);
```

Retiens ce `&` — c'est la porte d'entrée des pointeurs, plus tard.

---

## 2. Déclarer

```c
int age;           // déclaration seule : la place est réservée, le contenu est indéfini
age = 25;          // affectation

int taille = 180;  // déclaration + initialisation, en une fois
```

**Toujours initialiser.** Une variable déclarée sans valeur ne contient pas `0` :
elle contient ce qui traînait à cet endroit de la mémoire. Le programme compilera
et affichera n'importe quoi.

```c
int x;
printf("%d\n", x);   // affiche une valeur imprévisible
```

C'est exactement ce que `-Wall -Wextra` te signale. **Un avertissement est un bug
qui n'a pas encore eu lieu.**

### Règles de nommage

- Lettres, chiffres et `_` ; jamais de chiffre en première position
- Sensible à la casse : `age` et `Age` sont deux variables différentes
- Pas de mot réservé (`int`, `return`, `if`…)
- Par convention en C : `minuscules_avec_underscores`

Un nom doit dire ce que la variable contient. `nb_etudiants` vaut mieux que `n`.

---

## 3. Les types de base

Mesurés **sur cette machine** (Mac Apple Silicon, arm64) :

| Type | Taille | Intervalle | Usage |
|---|---|---|---|
| `char` | 1 octet | −128 à 127 | Un caractère, ou un très petit entier |
| `short` | 2 octets | −32 768 à 32 767 | Petit entier |
| `int` | 4 octets | −2 147 483 648 à 2 147 483 647 | **L'entier par défaut** |
| `long` | 8 octets | ±9,2 × 10¹⁸ | Grand entier |
| `float` | 4 octets | ~6 chiffres significatifs | Nombre à virgule, précision faible |
| `double` | 8 octets | ~15 chiffres significatifs | **Le flottant par défaut** |

**Ces tailles ne sont pas garanties par la norme.** Le C promet seulement des
minimums (`int` ≥ 2 octets). Sur un microcontrôleur — ce qui t'attend dans
`dev_Embeded/` — un `int` peut faire 2 octets. D'où l'intérêt de ne jamais les
supposer, mais de les mesurer :

```c
printf("int fait %zu octets\n", sizeof(int));
```

`sizeof` donne la taille en octets. Le marqueur `%zu` lui est réservé.

### Signé ou non

`unsigned` supprime les valeurs négatives et double la portée positive :

```c
unsigned int u = 4000000000;   // 0 à 4 294 967 295
```

Utile pour ce qui ne peut pas être négatif (une taille, un compteur). Mais source
de pièges classiques : `unsigned` moins grand que zéro repasse au maximum.

### Il n'y a pas de booléen (à l'origine)

Le C utilise des entiers : **`0` est faux, tout le reste est vrai.**

```c
if (3) { /* s'exécute */ }
if (0) { /* jamais */ }
```

`<stdbool.h>` ajoute `bool`, `true` et `false` — plus lisible, mais ce ne sont
que des entiers déguisés.

---

## 4. Afficher avec `printf`

Chaque type a son marqueur :

| Marqueur | Type | Exemple |
|---|---|---|
| `%d` | `int` | `printf("%d", 42)` |
| `%u` | `unsigned int` | `printf("%u", 42u)` |
| `%ld` | `long` | `printf("%ld", 42L)` |
| `%f` | `float` / `double` | `printf("%f", 3.14)` |
| `%.2f` | idem, 2 décimales | `3.14` |
| `%c` | `char` (le caractère) | `printf("%c", 'A')` → `A` |
| `%s` | chaîne | `printf("%s", "salut")` |
| `%zu` | résultat de `sizeof` | `printf("%zu", sizeof(int))` |
| `%p` | adresse | `printf("%p", (void *)&x)` |
| `%%` | un `%` littéral | |

**Le marqueur doit correspondre au type.** `printf("%d", 3.14)` compile mais
affiche n'importe quoi : `printf` fait confiance au marqueur, pas à la valeur.
`-Wall` te prévient — encore une raison de ne jamais l'omettre.

```c
int a = 5;
double b = 2.5;
printf("a=%d b=%.1f somme=%.1f\n", a, b, a + b);
```

---

## 5. Trois pièges à connaître maintenant

Les trois surprennent tout le monde. Mieux vaut les rencontrer ici que dans un
programme de 200 lignes.

### La division entière

```c
7 / 2       →  3        (pas 3.5)
7 % 2       →  1        (le reste)
7.0 / 2     →  3.500000
(float)7/2  →  3.500000
```

**Entier ÷ entier = entier.** La partie décimale est jetée, pas arrondie.

C'est exactement la famille du bug de `moyenne` dans la leçon précédente : le
programme compile, ne plante pas, et donne un résultat faux.

Pour obtenir une division réelle, il faut qu'au moins un opérande soit flottant —
soit en l'écrivant `7.0`, soit par un **cast** : `(float)7`.

`%` (modulo) donne le reste. Très utile : `n % 2 == 0` teste la parité.

### Les flottants sont imprécis

```c
printf("%.20f\n", 0.1 + 0.2);      // 0.30000000000000004441
printf("%d\n", 0.1 + 0.2 == 0.3);  // 0  → faux !
```

Ce n'est pas un bug du C : `0.1` n'a pas de représentation exacte en binaire, comme
⅓ n'en a pas en décimal. Tous les langages ont ce comportement — le C te le montre
simplement en face.

**Conséquence pratique : ne jamais comparer deux flottants avec `==`.** On compare
leur écart à une petite tolérance :

```c
#include <math.h>
if (fabs(a - b) < 0.0001) { /* considérés égaux */ }
```

*(`math.h` demande `-lm` à l'édition de liens — le cas dont on a parlé où le header
promet une fonction dont le code est ailleurs.)*

### `char` est un nombre

```c
char c = 'A';
printf("%d\n", c);        // 65   ← le code ASCII
printf("%c\n", c + 1);    // B
```

Un `char` est un entier sur 1 octet. `'A'` vaut 65, `'a'` vaut 97, `'0'` vaut 48.
`%d` montre le nombre, `%c` montre le caractère : même donnée, deux lectures.

C'est ce qui permet des astuces comme `c - '0'` pour convertir le caractère `'7'`
en l'entier 7.

**Attention aux guillemets :**

| Écriture | Sens |
|---|---|
| `'A'` | Un `char`, 1 octet |
| `"A"` | Une chaîne, 2 octets (`'A'` et `'\0'`) |

Les simples pour un caractère, les doubles pour une chaîne. Ce n'est pas
interchangeable, contrairement à Python.

---

## 6. Conversions

### Automatiques

Le C convertit tout seul quand des types se mélangent, du plus petit vers le plus
grand :

```c
int a = 5;
double b = 2.5;
double r = a + b;      // a est converti en double → 7.5
```

Dans l'autre sens, il y a **perte** :

```c
double pi = 3.99;
int n = pi;            // n vaut 3 — tronqué, pas arrondi
```

### Explicites (cast)

```c
int a = 7, b = 2;
double r = (double)a / b;    // 3.5
```

Le cast `(double)` force la conversion avant le calcul. Sans lui, `a / b` est une
division entière qui donne `3`, et le `3` est ensuite converti en `3.0` — trop tard.

---

## 7. Constantes

Deux façons de nommer une valeur qui ne doit pas changer :

```c
const double PI = 3.14159;      // une variable en lecture seule
#define TAILLE_MAX 100          // une substitution de texte, faite par le préprocesseur
```

`const` est vérifié par le compilateur : toute tentative de modification est une
erreur. `#define` est un simple remplacement avant compilation — le même mécanisme
que `#include`.

**Préfère `const`** : il a un type, donc le compilateur peut le contrôler.

---

## 8. Lire une entrée avec `scanf`

L'inverse de `printf` : lire ce que l'utilisateur tape.

```c
#include <stdio.h>

int main(void) {
    int age;
    printf("Ton age ? ");
    scanf("%d", &age);              // ← le & est obligatoire
    printf("Tu as %d ans.\n", age);
    return 0;
}
```

**Le `&` devant la variable est obligatoire.** `printf` reçoit une *valeur* ;
`scanf` a besoin de savoir *où écrire*, donc on lui donne l'**adresse**. C'est le
même `&` que celui vu au début — et l'oublier est l'erreur n°1 des débutants en C.
Le programme compile et plante à l'exécution.

*(Pourquoi cette différence sera limpide avec les pointeurs. Pour l'instant : `&`
devant chaque variable de `scanf`.)*

`scanf` est fragile face à une entrée inattendue (taper `abc` quand on attend un
nombre). Suffisant pour apprendre, à remplacer plus tard par `fgets`.

---

## 9. Exercices

À écrire dans `codes/`, compiler avec :

```sh
gcc -Wall -Wextra -g codes/fichier.c -o codes/fichier && ./codes/fichier
```

**1 — Tailles.** Affiche la taille de chaque type avec `sizeof` et `%zu`. Compare
avec le tableau ci-dessus.

**2 — Fiche.** Déclare un `char` (initiale), un `int` (âge), un `double` (taille en
mètres) et affiche une phrase les combinant, avec 2 décimales pour la taille.

**3 — Division.** Affiche `7 / 2`, `7 % 2`, `7.0 / 2` et `(double)7 / 2`. Explique
à voix haute pourquoi les résultats diffèrent.

**4 — Moyenne.** Demande deux entiers avec `scanf` et affiche leur moyenne exacte
(avec la décimale). C'est le bug de la leçon 2, corrigé — attention aux parenthèses
*et* au type.

**5 — Débordement.** Donne à un `int` sa valeur maximale (2147483647), ajoute 1,
affiche. Le résultat te dira ce qui se passe quand un type déborde de sa capacité.

**6 — ASCII.** Affiche le code numérique de `'A'`, `'a'` et `'0'` avec `%d`, puis
convertis le caractère `'7'` en l'entier 7.

---

## 10. Un programme complet, décortiqué

Le premier programme écrit de bout en bout — `codes/prensenting_myself.c` :

```c
#include <stdio.h>

int main(){
    char name[] = "Yann";
    int age = 30;
    printf("Hello my name is %s and I am %d years old!\n", name, age);
    return 0;
}
```

Il compile sans le moindre avertissement avec `-Wall -Wextra`, et affiche :

```
Hello my name is Yann and I am 30 years old!
```

### `char name[] = "Yann";`

Les crochets `[]` changent tout : ce n'est plus **une** variable, mais un
**tableau** — une suite de cases contiguës en mémoire.

`name` n'est donc pas un `char` mais un tableau de `char`, c'est-à-dire une chaîne
de caractères. Le C n'a pas de type `string` : une chaîne **est** un tableau de
caractères, rien de plus.

Voici ce que la mémoire contient réellement :

```
indice :     0     1     2     3     4
          ┌─────┬─────┬─────┬─────┬─────┐
name   =  │ 'Y' │ 'a' │ 'n' │ 'n' │'\0' │
          └─────┴─────┴─────┴─────┴─────┘
valeur :    89    97   110   110     0
```

**Cinq octets pour quatre lettres.** Le compilateur ajoute automatiquement le
`'\0'` final — le caractère de valeur zéro qui marque la fin. Sans lui, `printf`
ne saurait pas où s'arrêter et continuerait à lire la mémoire suivante.

Vérifiable :

```c
printf("%zu\n", sizeof(name));   // 5, pas 4
```

Les crochets vides `[]` disent au compilateur : *« compte toi-même »*. Il regarde
le littéral `"Yann"`, y trouve 4 caractères, ajoute le `'\0'`, et réserve 5 octets.

C'est le même `'\0'` que celui rencontré dans `"Goodbye, World!"` à la leçon 1 —
sauf qu'ici il est visible, parce que la chaîne porte un nom et occupe une place
que l'on peut mesurer.

### `int age = 30;`

Une variable simple, cette fois. Pas de crochets : **une seule** case de 4 octets,
contenant la valeur 30.

Le contraste entre les deux lignes résume la section 3 :

| | `name` | `age` |
|---|---|---|
| Nature | Tableau de `char` | Un seul `int` |
| Taille | 5 octets | 4 octets |
| Contenu | `'Y'`,`'a'`,`'n'`,`'n'`,`'\0'` | `30` |
| Marqueur | `%s` | `%d` |

### `printf("... %s ... %d ...\n", name, age);`

C'est ici que `printf` mérite enfin son nom — *print **formatted***. Jusqu'à
présent la chaîne sortait telle quelle ; elle porte maintenant deux **marqueurs**,
remplacés dans l'ordre par les arguments qui suivent :

```
printf("Hello my name is %s and I am %d years old!\n", name, age);
                          │              │             │     │
                          └──────────────┼─────────────┘     │
                                         └───────────────────┘
```

`%s` prend `name`, `%d` prend `age`. **L'ordre compte** : le premier marqueur va
avec le premier argument.

Chaque marqueur correspond au type de sa variable — `%s` pour une chaîne, `%d`
pour un entier. C'est la règle de la section 4, et c'est ce qui fait que le
programme passe `-Wall -Wextra` sans broncher.

Un détail sur `%s` : `printf` ne reçoit pas les 5 octets, mais l'adresse du
premier. Il lit ensuite caractère par caractère jusqu'à tomber sur le `'\0'`.
D'où l'importance de ce zéro final — c'est le seul signal d'arrêt.

Et le `\n` termine par un saut de ligne, pour que le prompt du shell ne se colle
pas à la sortie.

### `return 0;`

Le code de sortie, récupéré par le shell dans `$?`. Vérifiable :

```sh
./codes/prensenting_myself
echo $?      # 0
```

### Ce que ce programme démontre

Trois notions de cette leçon, réunies en six lignes :

1. **Une variable est une place en mémoire** dont la taille dépend du type —
   4 octets pour l'`int`, 5 pour le tableau
2. **Une chaîne est un tableau de `char` terminé par `'\0'`** — le C n'a pas
   d'autre mécanisme
3. **Chaque marqueur `printf` correspond à un type** — `%s` et `%d` ne sont pas
   interchangeables

### Pour aller plus loin

Le tableau se parcourt case par case, avec les crochets :

```c
printf("%c\n", name[0]);      // Y
printf("%d\n", name[0]);      // 89  — le meme octet, lu comme un nombre
printf("%zu\n", sizeof(name)); // 5
```

L'indice commence à **0**, pas à 1. `name[0]` est la première lettre, `name[4]`
le `'\0'`.

Et `strlen` (dans `<string.h>`) donne la longueur *utile*, sans compter le zéro :

```c
#include <string.h>
printf("%zu\n", strlen(name));   // 4  — alors que sizeof donne 5
```

Deux nombres, deux questions différentes : `sizeof` demande *« combien de place
occupée ? »*, `strlen` demande *« combien de lettres ? »*. La distinction reviendra
avec les tableaux et les pointeurs.

---

## Mémo

```c
int    n = 42;              // %d    4 octets
long   l = 42L;             // %ld   8 octets
char   c = 'A';             // %c ou %d   1 octet
double d = 3.14;            // %f, %.2f   8 octets
unsigned int u = 42u;       // %u

const double PI = 3.14159;  // constante typée

sizeof(int)                 // %zu — taille en octets
&variable                   // %p — adresse

(double)a / b               // cast : force la division réelle
scanf("%d", &age);          // le & est obligatoire
```

**Les trois réflexes :**

1. Toujours initialiser une variable
2. Le marqueur `printf` doit correspondre au type
3. `entier / entier` reste un entier
