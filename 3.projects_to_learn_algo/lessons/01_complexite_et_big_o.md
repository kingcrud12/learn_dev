# Complexité et notation Big-O

## Le problème que personne ne voit venir

Vous écrivez une fonction qui cherche un nom dans une liste de contacts. Vous la
testez avec vos 50 contacts : instantané. Vous la mettez en production sur une
base de 10 millions d'utilisateurs : elle met huit secondes.

Rien n'a changé dans le code. Ce qui a changé, c'est **n**.

L'algorithmique n'est pas l'art d'écrire du code qui marche — vous savez déjà
faire. C'est l'art de prédire **comment il se comporte quand les données
grossissent**, avant d'avoir les données.

## Mesurer, mais mesurer quoi

Premier réflexe : chronométrer.

```c
clock_t debut = clock();
recherche(tableau, 1000000, cible);
clock_t fin = clock();
printf("%f s\n", (double)(fin - debut) / CLOCKS_PER_SEC);
```

C'est utile, mais ça ne dit presque rien. Le résultat dépend de votre processeur,
de la charge de la machine, du compilateur, des options d'optimisation. Le même
code donne 0,3 s sur votre portable et 0,05 s sur un serveur.

Ce qu'on veut, c'est une mesure qui **ne dépende pas de la machine**. Alors on
ne compte pas des secondes : on compte des **opérations**, en fonction de `n`.

## Compter les opérations

Prenons la recherche linéaire :

```c
int recherche(int tab[], int n, int cible) {
    for (int i = 0; i < n; i++) {
        if (tab[i] == cible) {
            return i;
        }
    }
    return -1;
}
```

Combien de comparaisons `tab[i] == cible` ?

- **Meilleur cas** : la cible est en première position → 1 comparaison
- **Pire cas** : la cible est absente → `n` comparaisons
- **Cas moyen** : elle est quelque part au milieu → `n/2` comparaisons

Trois réponses. Par convention, **on retient le pire cas**. Pas par pessimisme :
parce que c'est la seule garantie qu'on peut donner. « Ça prendra au plus `n`
comparaisons » est une promesse tenable ; « ça prendra en moyenne `n/2` » ne dit
rien sur la journée où tout va mal.

## Pourquoi on jette les constantes

La recherche linéaire fait `n` comparaisons. Mais aussi `n` incréments de `i`,
`n` tests `i < n`, et quelques initialisations. Un décompte honnête donnerait
quelque chose comme `3n + 2`.

Comparons deux algorithmes sur un tableau qui grandit :

| n | `3n + 2` | `n² / 2` |
|---|---|---|
| 10 | 32 | 50 |
| 100 | 302 | 5 000 |
| 1 000 | 3 002 | 500 000 |
| 1 000 000 | 3 000 002 | 500 000 000 000 |

Au début, le premier est à peine meilleur. À un million, il est 166 000 fois plus
rapide. Le facteur `3` et le `+2` n'ont jamais compté : ce qui compte, c'est que
l'un grandit **comme n** et l'autre **comme n²**.

D'où la règle : on garde uniquement le terme qui domine, et on jette son
coefficient.

- `3n + 2` → **O(n)**
- `n²/2 + 10n + 7` → **O(n²)**
- `5` → **O(1)**

On lit « grand O de n ». Cela signifie : *le temps croît au plus proportionnellement
à n, à une constante près.*

## Les classes qu'il faut reconnaître à vue

### O(1) — temps constant

Le temps ne dépend pas de `n`. Accéder à `tab[i]`, empiler sur une pile, faire
une addition.

```c
int premier(int tab[], int n) {
    return tab[0];      // 1 opération, que n vaille 10 ou 10 milliards
}
```

### O(log n) — logarithmique

À chaque étape, on **divise le problème par deux**. C'est la signature de la
recherche dichotomique.

Le point à retenir : `log₂(1 000 000) ≈ 20`. Un million d'éléments, vingt étapes.
Un milliard, trente. C'est presque aussi bon que constant.

### O(n) — linéaire

On parcourt les données une fois. Un `for` simple sur le tableau.

### O(n log n) — quasi-linéaire

On parcourt les données `log n` fois, ou on divise en deux en traitant chaque
niveau entièrement. C'est la complexité des bons tris (fusion, rapide) — et,
on le démontrera plus tard, **on ne peut pas faire mieux** pour un tri par
comparaisons.

### O(n²) — quadratique

Deux boucles imbriquées sur les mêmes données. Chaque élément est comparé à
chaque autre.

```c
for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        // n × n itérations
    }
}
```

### O(2ⁿ) — exponentiel

Chaque élément ajouté **double** le travail. Au-delà de n ≈ 40, c'est
inutilisable, quelle que soit la machine.

## L'échelle, en ordres de grandeur

Pour une machine faisant 10⁹ opérations par seconde :

| n | O(log n) | O(n) | O(n log n) | O(n²) | O(2ⁿ) |
|---|---|---|---|---|---|
| 10 | 3 | 10 | 33 | 100 | 1 024 |
| 100 | 7 | 100 | 664 | 10 000 | 10³⁰ |
| 1 000 | 10 | 1 000 | 9 966 | 10⁶ | — |
| 1 000 000 | 20 | 10⁶ | 2×10⁷ | 10¹² (~17 min) | — |

Deux lectures s'imposent.

**En bas à droite, l'infaisable.** `2¹⁰⁰` dépasse le nombre d'atomes dans
l'univers observable. Aucun matériel ne rattrape un mauvais algorithme.

**Le saut entre O(n log n) et O(n²).** À un million d'éléments : 20 millions
d'opérations contre mille milliards. Deux centièmes de seconde contre dix-sept
minutes. C'est exactement l'écart entre un tri fusion et un tri par sélection —
et c'est pourquoi la moitié de ce cours porte sur les tris.

## Comment lire la complexité d'un code

Trois règles suffisent presque toujours.

**1. Instructions successives : on additionne, puis on garde le max.**

```c
for (int i = 0; i < n; i++) { ... }     // O(n)
for (int j = 0; j < n * n; j++) { ... } // O(n²)
// total : O(n + n²) = O(n²)
```

**2. Boucles imbriquées : on multiplie.**

```c
for (int i = 0; i < n; i++) {       // n fois
    for (int j = 0; j < m; j++) {   // × m fois
        ...                          // = O(n × m)
    }
}
```

**3. Une boucle dont l'indice est divisé (ou multiplié) : O(log n).**

```c
for (int i = n; i > 0; i = i / 2) { ... }   // O(log n)
```

### Le piège classique

Cette boucle **n'est pas** O(n²) :

```c
for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
        ...
    }
}
```

La boucle interne fait `n-1` tours, puis `n-2`, puis `n-3`... Le total est
`n(n-1)/2`, soit `n²/2 - n/2`. On jette la constante et le terme dominé :
**c'est bien O(n²)**.

Le piège est inverse : on croit souvent que « seulement la moitié du tableau »
change la classe. Non. Diviser par deux ne change jamais une classe de
complexité — c'est une constante.

## Complexité en espace

Le temps n'est pas la seule ressource. La **complexité en espace** mesure la
mémoire supplémentaire utilisée, hors données d'entrée.

```c
// O(1) en espace : trois variables, quel que soit n
void inverser(int tab[], int n) {
    for (int i = 0; i < n / 2; i++) {
        int tmp = tab[i];
        tab[i] = tab[n - 1 - i];
        tab[n - 1 - i] = tmp;
    }
}

// O(n) en espace : on alloue un second tableau
int *copie_inversee(int tab[], int n) {
    int *sortie = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        sortie[i] = tab[n - 1 - i];
    }
    return sortie;
}
```

C'est un vrai arbitrage, et il revient constamment. Le tri fusion est plus
rapide que le tri par insertion, mais consomme O(n) de mémoire là où l'autre
n'en consomme aucune. Sur un microcontrôleur avec 2 Ko de RAM, ce détail décide
du choix.

## Ce que Big-O ne dit pas

Trois réserves honnêtes, parce que la notation est souvent mal employée.

**Les constantes existent en vrai.** O(n) avec un facteur 1000 est plus lent que
O(n²) tant que `n < 1000`. C'est pour cette raison que les implémentations
sérieuses de tri rapide basculent sur un tri par insertion en dessous de ~16
éléments : asymptotiquement moins bon, concrètement plus rapide sur les petits
tableaux.

**La mémoire a une géographie.** Parcourir un tableau et parcourir une liste
chaînée sont tous deux O(n). En pratique le tableau est souvent 10 fois plus
rapide, parce que ses éléments sont contigus et que le cache du processeur les
charge par blocs. Big-O ignore complètement cela.

**Le pire cas n'est pas toujours le bon critère.** Le tri rapide est O(n²) au
pire, O(n log n) en moyenne — et c'est le tri le plus utilisé au monde, parce
que son pire cas est rarissime et ses constantes excellentes.

Big-O est un outil de **cadrage**, pas un verdict. Il vous dit quel algorithme
ne tiendra jamais la charge. Il ne vous dit pas lequel des deux survivants sera
le plus rapide sur vos données : ça, il faut le mesurer.

## Ce qu'il faut retenir

- On compte des **opérations en fonction de n**, pas des secondes.
- On garde le **terme dominant**, sans son coefficient.
- Par défaut on parle du **pire cas**.
- La hiérarchie : O(1) < O(log n) < O(n) < O(n log n) < O(n²) < O(2ⁿ).
- Le saut O(n log n) → O(n²) est celui qui décide si un programme tient la charge.
- L'espace compte autant que le temps quand la mémoire est contrainte.

---

## À faire maintenant

Écris un programme qui compte réellement les opérations, au lieu de les estimer :

```c
#include <stdio.h>

int recherche(int tab[], int n, int cible, long *compteur) {
    for (int i = 0; i < n; i++) {
        (*compteur)++;
        if (tab[i] == cible) return i;
    }
    return -1;
}

int main(void) {
    int tab[1000];
    for (int i = 0; i < 1000; i++) tab[i] = i;

    long ops = 0;
    recherche(tab, 1000, 999, &ops);      // pire cas : cible en fin
    printf("Recherche de 999 : %ld operations\n", ops);

    ops = 0;
    recherche(tab, 1000, 0, &ops);        // meilleur cas
    printf("Recherche de 0   : %ld operations\n", ops);

    return 0;
}
```

```sh
gcc -Wall -Wextra -g codes/compter_ops.c -o codes/compter_ops && ./codes/compter_ops
```

Fais varier la taille : 1000, 2000, 4000. Le nombre d'opérations du pire cas
double-t-il à chaque fois ? C'est la signature de O(n), constatée et non admise.
