# Récursivité

## Une fonction qui s'appelle elle-même

Vous l'avez déjà utilisée : le tri fusion et le tri rapide sont récursifs. Cette
leçon revient dessus pour l'outiller — parce que la récursivité n'est pas une
astuce d'écriture, c'est une façon de décomposer un problème, et elle a un coût
en mémoire qu'il faut savoir estimer.

```c
int factorielle(int n) {
    if (n <= 1) {
        return 1;                        // cas de base
    }
    return n * factorielle(n - 1);       // cas récursif
}
```

Deux parties, toujours :

- **Le cas de base** : la situation qu'on sait résoudre sans récursion. Il
  **arrête** la descente.
- **Le cas récursif** : on se rappelle sur un problème **strictement plus petit**,
  et on combine.

Oubliez le cas de base, ou appelez-vous sur un problème qui ne rétrécit pas, et
vous obtenez une récursion infinie. Qui ne tourne pas indéfiniment : elle
s'écrase, et on va voir précisément pourquoi.

## Ce qui se passe dans la mémoire

C'est le point que les cours passent souvent sous silence, et c'est le seul qui
compte vraiment.

Chaque appel de fonction empile un **cadre de pile** (*stack frame*) : ses
paramètres, ses variables locales, et l'adresse de retour. Ce cadre vit jusqu'au
`return`.

Pour `factorielle(4)` :

```
                              pile
appel factorielle(4)     │ n=4 │                        ← empile
appel factorielle(3)     │ n=4 │ n=3 │
appel factorielle(2)     │ n=4 │ n=3 │ n=2 │
appel factorielle(1)     │ n=4 │ n=3 │ n=2 │ n=1 │      ← cas de base
retourne 1               │ n=4 │ n=3 │ n=2 │            ← dépile
retourne 2*1 = 2         │ n=4 │ n=3 │
retourne 3*2 = 6         │ n=4 │
retourne 4*6 = 24        │                              ← pile vide
```

Deux conséquences pratiques.

**La pile est une ressource limitée.** Typiquement 8 Mo sur Linux, 1 Mo sur
Windows, souvent quelques Ko sur un microcontrôleur. À ~50 octets par cadre, un
million d'appels imbriqués suffit à la remplir. Le programme reçoit alors un
`SIGSEGV` — le fameux *stack overflow*.

**La profondeur est la complexité en espace.** Une récursion de profondeur `d`
coûte O(d) en mémoire, même si la fonction ne déclare aucune variable. C'est ce
qui rend le tri rapide dangereux sur un tableau trié : profondeur `n`, donc O(n)
de pile.

Vérifiez-le vous-même :

```c
int profondeur(int n) {
    printf("%d\n", n);
    return profondeur(n + 1);       // aucun cas de base
}
```

Lancez-le. Il affichera quelques centaines de milliers de lignes, puis
`Segmentation fault`. Le dernier nombre affiché est le nombre de cadres que
votre pile a pu contenir. C'est une mesure concrète, pas une abstraction.

## Récursivité et itération

Toute récursion peut s'écrire en boucle, et réciproquement. Le choix est
d'expressivité et de coût, pas de possibilité.

```c
// Récursif : lit comme la définition mathématique
int factorielle_rec(int n) {
    if (n <= 1) return 1;
    return n * factorielle_rec(n - 1);
}

// Itératif : O(1) en espace, pas de risque de débordement
int factorielle_iter(int n) {
    int resultat = 1;
    for (int i = 2; i <= n; i++) {
        resultat *= i;
    }
    return resultat;
}
```

Ici, l'itératif gagne : même lisibilité, meilleur coût. **Quand la version
itérative est aussi claire, préférez-la.**

La récursivité s'impose quand la structure du problème est elle-même récursive :
les arbres, les graphes, le retour sur trace. Écrire un parcours d'arbre en
itératif oblige à gérer une pile explicite — on réécrit à la main ce que le
langage faisait gratuitement.

## Récursion terminale

Une récursion est **terminale** quand l'appel récursif est la **dernière**
opération de la fonction — rien à faire après lui.

```c
// PAS terminale : il reste la multiplication après l'appel
int fact(int n) {
    return n * fact(n - 1);
}

// Terminale : l'appel est la dernière chose
int fact_term(int n, int accumulateur) {
    if (n <= 1) return accumulateur;
    return fact_term(n - 1, n * accumulateur);
}
```

L'astuce est l'**accumulateur** : on transporte le résultat partiel en paramètre
au lieu de l'attendre au retour.

L'intérêt : le cadre courant n'a plus besoin d'exister pendant l'appel. Un
compilateur peut le réutiliser au lieu d'en empiler un nouveau — la récursion
devient une boucle, et l'espace retombe à O(1).

```sh
gcc -O2 -S fichier.c        # regarder l'assembleur généré
```

Cherchez un `jmp` là où vous attendiez un `call`. Si vous le trouvez,
l'optimisation a eu lieu.

**Réserve importante** : en C, cette optimisation n'est **pas garantie** par la
norme. GCC et Clang la font en `-O2`, pas en `-O0`. Ne construisez jamais un
programme C dont la correction dépend d'elle. En Scheme ou en Haskell, elle est
garantie par le langage ; en C, c'est une faveur du compilateur.

## Trois schémas classiques

### Linéaire — un seul appel

Parcourir une liste, calculer une factorielle. Profondeur `n`, complexité O(n).

```c
int somme_tableau(const int tab[], int n) {
    if (n == 0) return 0;
    return tab[n - 1] + somme_tableau(tab, n - 1);
}
```

### Binaire — deux appels

Diviser pour régner. Si la découpe est équilibrée, profondeur `log n`.

```c
int maximum(const int tab[], int gauche, int droite) {
    if (gauche == droite) return tab[gauche];

    int milieu = gauche + (droite - gauche) / 2;
    int max_g = maximum(tab, gauche, milieu);
    int max_d = maximum(tab, milieu + 1, droite);

    return (max_g > max_d) ? max_g : max_d;
}
```

Attention : cette version est O(n) en temps comme une boucle simple, mais O(log n)
en espace au lieu de O(1). Ce n'est pas un progrès — c'est une illustration.

### Multiple — retour sur trace

Plusieurs appels, avec annulation quand une piste échoue. C'est le schéma des
puzzles : sudoku, n-reines, labyrinthes.

```
pour chaque choix possible :
    appliquer le choix
    si résoudre(reste) réussit : renvoyer succès
    annuler le choix          ← le « backtrack »
renvoyer échec
```

Le `annuler le choix` est ce qui distingue le retour sur trace d'une simple
récursion. On explore une branche, et si elle ne mène nulle part, on remet
l'état comme on l'a trouvé avant d'essayer la suivante.

## Le piège de Fibonacci

L'exemple canonique de récursion — et l'exemple canonique de la mauvaise
récursion.

```c
long fibo(int n) {
    if (n <= 1) return n;
    return fibo(n - 1) + fibo(n - 2);
}
```

Élégant, conforme à la définition mathématique, et catastrophique.

```
                    fibo(5)
              /                \
         fibo(4)              fibo(3)
        /      \              /     \
   fibo(3)   fibo(2)     fibo(2)  fibo(1)
   /    \     /   \       /   \
fibo(2) f(1) f(1) f(0)  f(1) f(0)
 /   \
f(1) f(0)
```

`fibo(3)` est calculé deux fois. `fibo(2)`, trois fois. Le nombre d'appels croît
comme `1,618ⁿ` — la complexité est **O(2ⁿ)**.

En chiffres : `fibo(40)` ≈ 300 millions d'appels, une seconde. `fibo(50)` ≈ 40
milliards, deux minutes. `fibo(100)` ne terminerait pas avant la fin de
l'univers.

### Le remède : la mémoïsation

On stocke les résultats déjà calculés.

```c
long fibo_memo(int n, long cache[]) {
    if (n <= 1) return n;
    if (cache[n] != -1) return cache[n];        // déjà connu

    cache[n] = fibo_memo(n - 1, cache) + fibo_memo(n - 2, cache);
    return cache[n];
}
```

Chaque valeur est calculée une seule fois. La complexité passe de **O(2ⁿ) à O(n)**.

C'est le point de bascule le plus spectaculaire de tout le cours : deux lignes de
code, et un algorithme infaisable devient instantané.

### Et la version itérative

```c
long fibo_iter(int n) {
    if (n <= 1) return n;
    long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long c = a + b;
        a = b;
        b = c;
    }
    return b;
}
```

O(n) en temps, **O(1) en espace** — la mémoïsation coûtait O(n) de tableau plus
O(n) de pile.

La leçon complète tient dans ces trois versions : la récursion naïve est
exponentielle, la mémoïsation la ramène à linéaire, l'itération élimine en plus
le coût mémoire. Reconnaître les **sous-problèmes qui se recouvrent** est le
signal qu'il faut sortir de la récursion naïve. C'est la porte d'entrée de la
programmation dynamique.

## Comment analyser une récursion

On écrit une **relation de récurrence** : le coût `T(n)` en fonction du coût des
sous-problèmes.

| Récurrence | Solution | Exemple |
|---|---|---|
| `T(n) = T(n-1) + O(1)` | O(n) | somme d'un tableau |
| `T(n) = T(n-1) + O(n)` | O(n²) | tri par sélection récursif |
| `T(n) = T(n/2) + O(1)` | O(log n) | recherche dichotomique |
| `T(n) = 2T(n/2) + O(n)` | **O(n log n)** | tri fusion |
| `T(n) = 2T(n/2) + O(1)` | O(n) | maximum récursif |
| `T(n) = 2T(n-1) + O(1)` | O(2ⁿ) | tours de Hanoï |

La ligne à retenir est la quatrième. `2T(n/2)` : deux sous-problèmes de taille
moitié. `+ O(n)` : un travail linéaire pour combiner. C'est exactement la
signature du tri fusion, et la raison de son O(n log n).

## Les règles à suivre

**Un cas de base, toujours, écrit en premier.** C'est la première ligne de la
fonction.

**Le problème doit rétrécir strictement.** `f(n-1)` rétrécit. `f(n)` ne rétrécit
pas. `f(n/2)` rétrécit — sauf si `n` vaut 1 et que la division entière renvoie 1.

**Estimer la profondeur maximale.** Si elle peut atteindre 100 000, la pile
lâchera. Passez à l'itératif ou à une pile explicite.

**Faire confiance à l'appel récursif.** L'erreur du débutant est de vouloir
dérouler mentalement tous les niveaux. Supposez que `f(n-1)` renvoie le bon
résultat, et occupez-vous uniquement de passer de `n-1` à `n`. C'est le même
raisonnement qu'une preuve par récurrence, et c'est la seule façon d'écrire du
code récursif sans s'y perdre.

## Ce qu'il faut retenir

- Cas de base + cas récursif sur un problème **strictement plus petit**.
- Chaque appel coûte un cadre de pile : profondeur `d` → **O(d) en espace**.
- La pile est finie (~8 Mo) : au-delà de ~100 000 niveaux, débordement.
- La **récursion terminale** peut devenir une boucle, mais ce n'est pas garanti
  en C.
- Des **sous-problèmes qui se recouvrent** signalent qu'il faut mémoïser :
  O(2ⁿ) → O(n).
- On analyse par une **relation de récurrence** ; `T(n) = 2T(n/2) + O(n)` donne
  O(n log n).
- Récursif quand la structure du problème l'est (arbres, graphes) ; itératif
  quand c'est aussi clair.

---

## À faire maintenant

Trois mesures, dans cet ordre.

**1.** Chronomètre `fibo(30)`, `fibo(35)`, `fibo(40)` en version naïve. Le temps
est multiplié par ~11 à chaque saut de 5. Extrapole `fibo(60)`.

**2.** Ajoute la mémoïsation. Relance `fibo(60)`. Instantané.

**3.** Écris la fonction sans cas de base et lance-la. Note le dernier nombre
affiché avant le crash : c'est la capacité de ta pile, mesurée.

```sh
ulimit -s        # taille de pile en Ko
```

Divise cette valeur par le nombre d'appels atteint : tu obtiens la taille
approximative d'un cadre de pile sur ta machine.
