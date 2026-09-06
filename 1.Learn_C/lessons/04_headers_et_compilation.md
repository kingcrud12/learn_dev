# Headers et compilation séparée

Ce que `#include` fait réellement, et comment un programme se construit à partir
de plusieurs fichiers.

Suite de [03_variables_et_types.md](03_variables_et_types.md).

---

## 1. Le problème que résout un header

Le compilateur lit ton fichier **de haut en bas, une seule fois**. Quand il
rencontre un appel, il doit déjà connaître la fonction.

```c
int main(void) {
    printf("bonjour\n");   // ERREUR : printf inconnu ici
    return 0;
}
```

Il te faut donc **annoncer** la fonction avant de l'appeler. Cette annonce
s'appelle un **prototype** :

```c
int printf(const char *format, ...);   // l'annonce

int main(void) {
    printf("bonjour\n");               // maintenant accepté
    return 0;
}
```

Écrire ça à la main pour chaque fonction de la bibliothèque standard serait
absurde. D'où les headers : `<stdio.h>` est un fichier qui contient déjà tous
ces prototypes.

```c
#include <stdio.h>   // colle ici le contenu de stdio.h, prototypes compris
```

**Un header ne contient pas de code. Il contient des promesses.**

---

## 2. Déclaration et définition

C'est la distinction centrale du C. Deux mots proches, deux choses opposées.

| | Déclaration | Définition |
|---|---|---|
| Dit quoi | « cette fonction existe, voilà sa forme » | « voilà ce qu'elle fait » |
| Contient du code | Non | Oui |
| Se termine par | `;` | `}` |
| Combien de fois | Autant qu'on veut | **Une seule** dans tout le programme |
| Vit dans | le `.h` | le `.c` |

```c
double aire(double l, double h);          // déclaration — un prototype

double aire(double l, double h) {         // définition — il y a un corps
    return l * h;
}
```

Le compilateur a besoin de la **déclaration** pour vérifier tes appels.
L'éditeur de liens a besoin de la **définition** pour produire l'exécutable.

Deux étapes différentes, deux besoins différents. Retiens ça : la plupart des
messages d'erreur incompréhensibles en C viennent d'une confusion entre les deux.

---

## 3. Les deux formes de `#include`

```c
#include <stdio.h>     // chevrons : cherche dans les répertoires système
#include "geometrie.h" // guillemets : cherche d'abord à côté de ton fichier
```

Règle simple : **chevrons pour la bibliothèque standard, guillemets pour tes
propres fichiers.**

---

## 4. Découper un programme en plusieurs fichiers

Prenons ta calculatrice. Aujourd'hui tout est dans un fichier. Voici la
découpe standard.

### `calcul.h` — l'interface

```c
#ifndef CALCUL_H
#define CALCUL_H

double add(double x, double y);
double subtract(double x, double y);
double multiply(double x, double y);
double divide(double x, double y);

#endif
```

### `calcul.c` — l'implémentation

```c
#include "calcul.h"     // le .c inclut son propre .h
#include <math.h>

double add(double x, double y)      { return x + y; }
double subtract(double x, double y) { return x - y; }
double multiply(double x, double y) { return x * y; }

double divide(double x, double y) {
    if (y == 0.0) {
        return NAN;
    }
    return x / y;
}
```

### `main.c` — l'utilisation

```c
#include <stdio.h>
#include <math.h>
#include "calcul.h"     // il ne connaît que l'interface

int main(void) {
    double q = divide(10.0, 5.0);

    if (isnan(q)) {
        fprintf(stderr, "Erreur : division par zero\n");
        return 1;
    }
    printf("Quotient : %.2f\n", q);
    return 0;
}
```

### Compiler l'ensemble

```sh
gcc -Wall -Wextra -g main.c calcul.c -o calculator
```

Note bien : on liste **les `.c`, jamais les `.h`**. Le header est déjà entré
dans les `.c` par le préprocesseur.

---

## 5. Les gardes d'inclusion

Ces trois lignes dans le `.h` :

```c
#ifndef CALCUL_H     // si CALCUL_H n'est pas défini
#define CALCUL_H     // alors définis-le
...
#endif
```

**Le problème qu'elles résolvent.** Imagine :

```
main.c inclut calcul.h
main.c inclut affichage.h  →  qui inclut aussi calcul.h
```

Sans garde, le contenu de `calcul.h` arriverait **deux fois** dans `main.c`.
Pour des prototypes ce n'est pas grave, mais pour un `struct` ou un `typedef`,
c'est une erreur de redéfinition.

Avec la garde : au deuxième passage, `CALCUL_H` est déjà défini, donc tout ce
qui est entre `#ifndef` et `#endif` est sauté.

**Mets-en une dans chacun de tes `.h`, systématiquement.** Le nom par
convention : le nom du fichier en majuscules, avec `_H`.

---

## 6. La compilation, étape par étape

Jusqu'ici tu as tapé une commande et obtenu un exécutable. Voici ce qui se
passe en réalité — les quatre étapes déjà vues en leçon 02, appliquées à
plusieurs fichiers.

```
main.c ─┐
        ├─→ [préprocesseur] ─→ [compilation] ─→ [assemblage] ─→ main.o  ─┐
calcul.h┘                                                                ├─→ [édition de liens] ─→ calculator
                                                                         │
calcul.c ─┐                                                              │
          ├─→ [préprocesseur] ─→ [compilation] ─→ [assemblage] ─→ calcul.o ┘
calcul.h ─┘
```

Point clé : **chaque `.c` est compilé séparément, sans rien savoir des autres.**
On appelle ça une *unité de compilation*. `main.c` ne sait pas comment `divide`
est écrite — il sait seulement qu'elle existe, grâce au prototype.

C'est l'éditeur de liens qui, à la fin, raccorde l'appel dans `main.o` à la
définition dans `calcul.o`.

### Le faire à la main

```sh
gcc -c main.c      # produit main.o   (-c = compile seulement, ne lie pas)
gcc -c calcul.c    # produit calcul.o
gcc main.o calcul.o -o calculator    # édition de liens
```

L'intérêt : si tu modifies seulement `calcul.c`, tu ne recompiles que lui.
Sur un projet de 500 fichiers, la différence est énorme. C'est exactement ce
que `make` automatisera.

---

## 7. Lire les erreurs

Les deux messages que tu rencontreras le plus, et ce qu'ils signifient.

### « implicit declaration of function »

```
warning: implicit declaration of function 'divide'
```

**Traduction :** tu appelles `divide` mais aucun prototype n'est passé avant.
→ Il manque un `#include`, ou le prototype.

### « undefined symbol » / « undefined reference »

```
Undefined symbols for architecture arm64:
  "_divide", referenced from: _main in main.o
```

**Traduction :** le compilateur t'a cru sur parole (le prototype était là),
mais l'éditeur de liens ne trouve **aucune définition**.

Causes habituelles :
- tu as oublié `calcul.c` dans la commande gcc
- la fonction est déclarée mais jamais écrite
- une faute de frappe dans le nom de la définition

La distinction est nette et utile :

| Erreur | Étape fautive | Ce qui manque |
|---|---|---|
| implicit declaration | compilation | la **déclaration** |
| undefined symbol | édition de liens | la **définition** |

---

## 8. `static` — limiter la portée

Une fonction ou une variable globale est visible depuis les autres fichiers par
défaut. `static` l'enferme dans son fichier.

```c
static double arrondir(double x) {   // usage interne à calcul.c uniquement
    return (int)(x * 100 + 0.5) / 100.0;
}
```

Utilité : ne pas exposer tes fonctions internes, et éviter les collisions de
noms entre fichiers. **Ce qui n'est pas dans le `.h` mérite souvent d'être
`static`.**

---

## 9. Automatiser avec `make`

Retaper la commande gcc devient vite pénible. `make` lit un fichier nommé
`Makefile` :

```make
CC = gcc
CFLAGS = -Wall -Wextra -g

calculator: main.o calcul.o
	$(CC) main.o calcul.o -o calculator

main.o: main.c calcul.h
	$(CC) $(CFLAGS) -c main.c

calcul.o: calcul.c calcul.h
	$(CC) $(CFLAGS) -c calcul.c

clean:
	rm -f *.o calculator
```

**Attention :** les lignes de commande doivent commencer par une **tabulation**,
pas des espaces. C'est l'erreur n°1 des débutants avec make.

Lecture d'une règle :

```
cible: dépendances
	commande à exécuter
```

Puis :

```sh
make          # construit la première cible
make clean    # supprime les fichiers produits
```

**Ce que fait make, exactement :** il compare les **dates de modification**.
Si `calcul.c` est plus récent que `calcul.o`, il recompile. Sinon il saute.
C'est tout — un graphe de dépendances sur des dates de fichiers.

---

## 10. Ce que ça t'apprend de la machine

**Un exécutable est assemblé, pas écrit.** Ton programme final est un collage
de morceaux compilés séparément, plus des morceaux venus de la bibliothèque
standard. L'éditeur de liens est le monteur.

**Un symbole est un nom dans une table.** `divide`, `main`, `printf` existent
dans les `.o` sous forme d'entrées de table. « undefined symbol » veut dire :
une entrée référencée que personne ne fournit.

**Les bibliothèques sont ce même mécanisme, en plus gros.** `libc` est un sac
d'objets pré-compilés. Quand tu ajoutes `-lm`, tu dis à l'éditeur de liens
d'aller aussi chercher dedans. C'est le même geste que d'ajouter `calcul.o`.

Deux formes existent :
- **statique** (`.a`) — le code est copié dans ton exécutable
- **dynamique** (`.so` sous Linux, `.dylib` sous macOS) — le code reste dehors
  et est chargé au lancement

Sur ta machine :

```sh
otool -L ./calculator     # macOS : liste les bibliothèques dynamiques liées
```

Tu verras `libSystem.dylib` : la libc de macOS, que ton programme ne contient
pas mais dont il dépend au démarrage. **C'est aussi la raison d'être de Docker** —
garantir que ces bibliothèques externes sont bien là, et dans la bonne version.

---

## 11. Exercices

1. Découpe ta `calculator.c` actuelle en `calcul.h` + `calcul.c` + `main.c`.
   Compile avec la commande à trois fichiers.

2. Supprime volontairement `calcul.c` de la commande gcc. Lis le message
   d'erreur en entier. Retrouve dans le texte : le nom du symbole manquant,
   et le fichier qui le réclamait.

3. Enlève les gardes d'inclusion de `calcul.h`, puis dans `main.c` écris
   `#include "calcul.h"` **deux fois**. Compile. Puis ajoute un
   `typedef struct { double x; } Point;` dans le header et recompile —
   maintenant l'erreur apparaît. Explique pourquoi elle n'apparaissait pas avant.

4. Écris le `Makefile` ci-dessus. Lance `make` deux fois de suite : la seconde
   fois, il doit dire « up to date ». Touche un fichier avec
   `touch calcul.c` et relance : observe ce qui est recompilé.

5. Rends `arrondir` `static` dans `calcul.c` et essaie de l'appeler depuis
   `main.c`. Quelle erreur, et à quelle étape ?

---

## Mémo

```c
#include <stdio.h>      // bibliothèque système
#include "calcul.h"     // fichier local

#ifndef CALCUL_H        // garde d'inclusion, dans tout .h
#define CALCUL_H
double aire(double l, double h);   // déclaration : ;
#endif

double aire(double l, double h) { ... }   // définition : { }

static void interne(void);   // visible dans ce fichier seulement
```

```sh
gcc -Wall -Wextra -g main.c calcul.c -o prog   # tout d'un coup
gcc -c calcul.c                                # un .o seulement
gcc main.o calcul.o -o prog                    # édition de liens
gcc -E main.c | wc -l                          # voir le préprocesseur
otool -L ./prog                                # bibliothèques dynamiques (macOS)
```

**Les trois réflexes :**

1. Le `.h` déclare, le `.c` définit
2. On compile les `.c`, jamais les `.h`
3. « implicit declaration » = compilation ; « undefined symbol » = édition de liens
