# La mémoire dynamique

Jusqu'ici, toutes tes tailles étaient connues à la compilation. Cette leçon
lève cette contrainte — et te rend responsable de la mémoire.

Suite de [07_pointeurs.md](07_pointeurs.md).

---

## 1. Le problème

```c
int n;
printf("Combien de notes ? ");
scanf("%d", &n);

int notes[n];    // taille connue seulement à l'exécution
```

Cette forme (VLA, *variable-length array*) existe en C99, mais elle a trois
défauts qui la rendent déconseillée : la taille reste limitée par la pile
(~8 Mo), un `n` trop grand provoque un crash sans message, et le tableau meurt
à la fin du bloc.

Il te faut donc de la mémoire :
- de taille décidée à l'exécution
- qui survit à la fonction qui la crée
- de grande capacité

C'est le **tas** (*heap*), et on y accède avec `malloc`.

---

## 2. Pile et tas

Reprends la carte mémoire de la fiche de parcours :

```
  adresses hautes
  ┌────────────────────────────┐
  │  PILE (stack)              │  variables locales, cadres d'appel
  │      ↓ croît vers le bas   │  automatique · limitée (~8 Mo)
  │                            │
  │            ...             │
  │                            │
  │      ↑ croît vers le haut  │
  │  TAS (heap)                │  malloc / free
  ├────────────────────────────┤  manuel · vaste
  │  BSS / data / text         │
  └────────────────────────────┘
  adresses basses
```

| | Pile | Tas |
|---|---|---|
| Allocation | automatique, à la déclaration | explicite, `malloc` |
| Libération | automatique, en fin de bloc | explicite, `free` |
| Taille | fixe, ~8 Mo | limitée par la RAM |
| Vitesse | très rapide (un registre décalé) | plus lent (recherche d'un bloc) |
| Durée de vie | le bloc | jusqu'au `free` |
| Taille connue | à la compilation | à l'exécution |

Le tas résout exactement les trois limites de la pile. Le prix : **tu deviens
responsable de la libération.**

---

## 3. `malloc`

```c
#include <stdlib.h>

int *tab = malloc(n * sizeof(int));
```

Décomposons :

| Morceau | Rôle |
|---|---|
| `malloc(...)` | demande un bloc d'octets, renvoie son adresse |
| `n * sizeof(int)` | la taille en **octets** — jamais en nombre d'éléments |
| `int *tab` | on récupère l'adresse dans un pointeur |

`malloc` renvoie un `void *` — un pointeur sans type. En C, il est converti
automatiquement, pas besoin de cast.

### Toujours vérifier le retour

```c
int *tab = malloc(n * sizeof(int));
if (tab == NULL) {
    fprintf(stderr, "Erreur : allocation impossible\n");
    return 1;
}
```

`malloc` renvoie `NULL` s'il échoue. C'est rare sur un Mac avec de la RAM, mais
courant en embarqué — et le déréférencement d'un `NULL` non testé est un crash.

**Ne jamais oublier ce test.** C'est un réflexe, pas une option.

### Le contenu n'est pas initialisé

```c
int *tab = malloc(5 * sizeof(int));
printf("%d\n", tab[0]);      // n'importe quoi
```

Le bloc contient ce qu'il y avait avant. Pour l'obtenir à zéro :

```c
int *tab = calloc(n, sizeof(int));   // n éléments de sizeof(int), tous à 0
```

`calloc` prend **deux** arguments (nombre, taille unitaire) et met tout à zéro.
Légèrement plus lent, plus sûr par défaut.

---

## 4. `free`

```c
free(tab);
tab = NULL;      // bon réflexe : évite le pointeur pendant
```

`free` rend le bloc à l'allocateur. Deux points essentiels :

**1. `free` ne change pas le pointeur.** Après `free(tab)`, `tab` contient
toujours l'ancienne adresse — qui ne t'appartient plus. C'est un **pointeur
pendant**. D'où le `tab = NULL;` juste après : déréférencer `NULL` crashe
franchement, alors qu'un pointeur pendant corrompt silencieusement.

**2. On ne libère que ce que `malloc` a donné.** Jamais une variable locale,
jamais un pointeur au milieu d'un bloc :

```c
int *p = malloc(10 * sizeof(int));
p++;              // p ne pointe plus au début
free(p);          // ERREUR : adresse invalide
```

---

## 5. Le cycle complet

```c
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    printf("Combien de notes ? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Entree invalide\n");
        return 1;
    }

    int *notes = malloc(n * sizeof(int));      // 1. allouer
    if (notes == NULL) {                       // 2. vérifier
        fprintf(stderr, "Allocation impossible\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {              // 3. utiliser
        printf("Note %d : ", i + 1);
        scanf("%d", &notes[i]);
    }

    int somme = 0;
    for (int i = 0; i < n; i++) {
        somme += notes[i];
    }
    printf("Moyenne : %.2f\n", (double)somme / n);

    free(notes);                               // 4. libérer
    notes = NULL;

    return 0;
}
```

**Ces quatre étapes sont indissociables.** Écris le `free` en même temps que le
`malloc`, avant même de remplir le corps. C'est l'habitude qui évite le plus de
bugs.

Note au passage : le bloc obtenu s'utilise exactement comme un tableau —
`notes[i]` fonctionne, puisque `tab[i]` n'est que `*(tab + i)`.

---

## 6. `realloc`

Pour redimensionner un bloc existant :

```c
int *nouveau = realloc(tab, nouvelle_taille * sizeof(int));
if (nouveau == NULL) {
    free(tab);           // l'ancien est toujours valide, il faut le libérer
    return 1;
}
tab = nouveau;
```

**Le piège classique :**

```c
tab = realloc(tab, taille);    // DANGEREUX
```

Si `realloc` échoue, il renvoie `NULL` **sans libérer l'ancien bloc**. En
écrasant `tab`, tu perds la seule référence vers cette mémoire : fuite garantie,
et données perdues.

**Toujours passer par une variable temporaire.**

### Le tableau qui grandit

Le motif de base de tout tableau dynamique :

```c
size_t capacite = 4;
size_t taille   = 0;
int *tab = malloc(capacite * sizeof(int));

// ajouter un élément
if (taille == capacite) {
    capacite *= 2;                       // on DOUBLE, on n'ajoute pas 1
    int *tmp = realloc(tab, capacite * sizeof(int));
    if (tmp == NULL) { free(tab); return 1; }
    tab = tmp;
}
tab[taille++] = valeur;
```

**Pourquoi doubler ?** Agrandir de 1 à chaque ajout coûterait une recopie à
chaque fois : O(n²) pour n insertions. En doublant, le coût moyen par insertion
devient constant. C'est exactement ce que font `std::vector` en C++, `list` en
Python et `ArrayList` en Java, sous le capot.

---

## 7. Les quatre bugs de mémoire

Ils forment une famille, et ils ont tous le même remède : le sanitizer.

### 1. Fuite mémoire (*memory leak*)

```c
int *tab = malloc(100 * sizeof(int));
return 0;                    // jamais libéré
```

Le bloc reste réservé. Sans conséquence dans un petit programme (le système
récupère tout à la fin), mais fatal dans un serveur ou une boucle : la mémoire
grimpe jusqu'à l'épuisement.

### 2. Double libération

```c
free(tab);
free(tab);      // UB — corruption des structures de l'allocateur
```

Le `tab = NULL;` après chaque `free` immunise contre ça : `free(NULL)` est
légal et ne fait rien.

### 3. Utilisation après libération (*use-after-free*)

```c
free(tab);
tab[0] = 42;    // UB — le bloc peut déjà être réattribué ailleurs
```

Le plus dangereux, parce qu'il « marche » souvent en test. C'est une des
catégories de failles les plus exploitées dans les navigateurs.

### 4. Débordement de tas

```c
int *tab = malloc(5 * sizeof(int));
tab[10] = 42;   // hors du bloc
```

Même chose qu'en leçon 06, mais sur le tas.

---

## 8. Les outils de détection

**Compile toujours ainsi pendant cette leçon :**

```sh
gcc -Wall -Wextra -g -fsanitize=address,undefined prog.c -o prog
./prog
```

AddressSanitizer instrumente ton code et détecte les quatre bugs ci-dessus,
avec l'emplacement exact.

Exemple de sortie sur un use-after-free :

```
==12345==ERROR: AddressSanitizer: heap-use-after-free on address 0x602000000010
WRITE of size 4 at 0x602000000010 thread T0
    #0 0x... in main prog.c:12
freed by thread T0 here:
    #0 0x... in free
    #1 0x... in main prog.c:11
```

Il te donne la ligne du bug **et** la ligne du `free` fautif. C'est infiniment
plus utile qu'un segfault nu.

Pour les fuites spécifiquement, sur macOS :

```sh
leaks --atExit -- ./prog
```

(`valgrind`, l'outil de référence, fonctionne mal sur ARM64 — utilise le
sanitizer sur ton Mac, tu retrouveras valgrind dans une VM Linux.)

---

## 9. Qui possède quoi

Le C n'a aucune notion de propriété. C'est à toi de l'établir par convention,
et de la **documenter**.

```c
// L'appelant devient propriétaire du bloc renvoyé : il doit le free().
char *dupliquer(const char *src) {
    size_t n = strlen(src) + 1;      // +1 pour le '\0'
    char *copie = malloc(n);
    if (copie == NULL) {
        return NULL;
    }
    memcpy(copie, src, n);
    return copie;
}
```

Usage :

```c
char *c = dupliquer("bonjour");
if (c != NULL) {
    printf("%s\n", c);
    free(c);         // c'est l'appelant qui libère
}
```

**Les trois règles de propriété :**

1. Un bloc a **exactement un** propriétaire à la fois
2. Le propriétaire, et lui seul, appelle `free`
3. Toute fonction qui renvoie de la mémoire allouée **doit le dire en commentaire**

C'est ce raisonnement — qui possède, jusqu'à quand — que Rust a transformé en
règles vérifiées par le compilateur. En C, il vit uniquement dans ta discipline
et dans tes commentaires.

---

## 10. Ce que ça t'apprend de la machine

### `malloc` n'est pas un appel système

C'est une **fonction de bibliothèque**, dans la libc. Elle gère un grand bloc
que le processus a déjà obtenu du noyau.

```
ton code
   │  malloc(40)
   ▼
allocateur (libc)  ──► a-t-il déjà un bloc libre assez grand ?
   │                       oui → le renvoie, aucun appel système
   │  non
   ▼
noyau  ──►  mmap() / brk()  : agrandit l'espace du processus
```

Conséquence mesurable : **mille `malloc` ne font pas mille appels système.**
L'allocateur découpe un grand bloc qu'il a déjà. Un appel système coûte cher —
il faut basculer en mode noyau — donc l'éviter est tout l'intérêt de cette
couche intermédiaire.

### La fragmentation

L'allocateur doit trouver un trou assez grand. Après beaucoup de `malloc`/`free`
de tailles variées, la mémoire ressemble à ceci :

```
[utilisé][libre][utilisé][libre][utilisé][libre]
          20o             30o             25o
```

75 octets libres au total, mais impossible d'allouer 60 octets d'un coup : aucun
trou n'est assez grand. C'est la **fragmentation externe**.

C'est pourquoi les allocateurs sont un sujet de recherche à part entière
(ptmalloc, jemalloc, tcmalloc), et pourquoi l'embarqué critique interdit souvent
`malloc` après l'initialisation : on ne peut pas garantir qu'une allocation
réussira.

### Ce que fait un ramasse-miettes, et à quel prix

Python, Java et Go font ce travail à ta place. Ils suivent les références, et
libèrent ce qui n'est plus atteignable.

Ce que ça coûte :

- **de la mémoire** — chaque objet porte des métadonnées (compteur, marques)
- **du temps, à des moments imprévisibles** — les pauses GC
- **du contrôle** — tu ne sais pas quand la libération a lieu

Ce que ça t'évite : les quatre bugs de la section 7, entièrement.

**Le marché est maintenant clair.** Le C te donne le contrôle total et la
performance prévisible, contre la responsabilité. Un GC prend la responsabilité,
contre du contrôle et de la prévisibilité. Rust cherche une troisième voie : la
vérification à la compilation, sans coût à l'exécution.

Tu ne peux comprendre ce compromis qu'après avoir géré la mémoire à la main.
C'est pourquoi cette leçon est au cœur du parcours.

### Voir la mémoire grimper

```c
#include <stdlib.h>

int main(void) {
    while (1) {
        malloc(1024 * 1024);     // 1 Mo, jamais libéré
    }
    return 0;
}
```

Lance-le et ouvre le Moniteur d'activité (ou `top`). Regarde la colonne mémoire
du processus grimper d'1 Mo par tour. **Tue-le avec Ctrl-C avant de saturer ta
machine.**

Puis note ceci : quand le processus meurt, le système récupère **toute** sa
mémoire d'un coup. Le noyau tient la liste des pages de chaque processus et les
rend au moment de la terminaison. Une fuite n'est donc jamais définitive — elle
est bornée par la durée de vie du processus. C'est ce qui rend les fuites
tolérables dans un outil en ligne de commande, et inacceptables dans un serveur.

---

## 11. Exercices

**Bases**

1. Reprends le programme de moyenne (section 5). Fais-le tourner, puis
   supprime le `free` et relance avec `-fsanitize=address`. Lis le rapport de fuite.

2. Écris `int *creer_tableau(int n)` qui alloue et remplit un tableau des
   entiers de 0 à n−1. Documente en commentaire qui doit le libérer.

3. Réimplémente `strdup` (la fonction `dupliquer` de la section 9), sans
   regarder, puis compare.

**Les quatre bugs**

4. Provoque volontairement chacun des quatre bugs de la section 7, un par un,
   avec le sanitizer actif. Note pour chacun le nom exact que donne
   AddressSanitizer (`memory leak`, `double-free`, `heap-use-after-free`,
   `heap-buffer-overflow`).

5. Écris le piège `tab = realloc(tab, ...)` et explique par écrit pourquoi il
   fuit en cas d'échec.

**Construire**

6. Implémente un tableau dynamique complet :
   ```c
   typedef struct {
       int   *donnees;
       size_t taille;
       size_t capacite;
   } Tableau;

   Tableau *tab_creer(void);
   int      tab_ajouter(Tableau *t, int valeur);   // 1 si OK
   void     tab_liberer(Tableau *t);
   ```
   C'est le cœur de `3.projects_to_learn_algo`.

7. Écris une fonction qui lit un fichier texte ligne par ligne dans un tableau
   dynamique de `char *`. Libère tout correctement à la fin — chaque ligne
   **et** le tableau.

**Observer la machine**

8. Lance le programme de fuite de la section 10 et observe la mémoire dans
   le Moniteur d'activité.

9. Alloue 3 blocs, libère celui du milieu, alloue-en un plus gros. Affiche les
   adresses (`%p`) à chaque étape et déduis ce que l'allocateur a fait.

---

## Mémo

```c
#include <stdlib.h>

int *tab = malloc(n * sizeof(int));      // non initialisé
int *tab = calloc(n, sizeof(int));       // mis à zéro
if (tab == NULL) { ... }                 // TOUJOURS vérifier

int *tmp = realloc(tab, n2 * sizeof(int));   // jamais tab = realloc(tab,...)
if (tmp == NULL) { free(tab); ... }
tab = tmp;

free(tab);
tab = NULL;                              // évite le pointeur pendant

// tableau dynamique : doubler, pas +1
if (taille == capacite) { capacite *= 2; ... }
```

```sh
gcc -Wall -Wextra -g -fsanitize=address,undefined prog.c -o prog
leaks --atExit -- ./prog        # macOS
```

**Les quatre réflexes :**

1. Écris le `free` en même temps que le `malloc`
2. Vérifie le retour de `malloc`, sans exception
3. `tab = NULL;` après chaque `free`
4. Documente qui possède la mémoire renvoyée par une fonction
