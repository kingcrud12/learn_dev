# Tris efficaces : fusion et rapide

## Diviser pour régner

Les tris de la leçon précédente traitent le tableau comme un tout : une boucle
qui balaie, une boucle imbriquée qui compare. D'où le O(n²).

Les deux tris de cette leçon appliquent une stratégie différente, qui porte un
nom : **diviser pour régner**. Elle tient en trois temps.

1. **Diviser** — couper le problème en sous-problèmes plus petits.
2. **Régner** — les résoudre récursivement.
3. **Combiner** — assembler les solutions.

Le gain vient d'une arithmétique simple : diviser par deux à chaque étape donne
`log n` niveaux. Si chaque niveau coûte O(n), le total est O(n log n).

Le tri fusion et le tri rapide sont tous deux issus de cette stratégie. Ils
diffèrent sur **l'étape où le travail se fait** — et cette différence explique
tout le reste.

## Tri fusion

### L'idée

Diviser le tableau en deux moitiés, trier chacune récursivement, puis **fusionner**
les deux moitiés triées.

```
                [38, 27, 43, 3, 9, 82, 10]
                 /                      \
        [38, 27, 43]                [3, 9, 82, 10]
         /        \                  /          \
     [38]      [27, 43]          [3, 9]      [82, 10]
                /     \           /   \       /     \
             [27]    [43]      [3]   [9]   [82]    [10]

  ... puis on remonte en fusionnant ...

             [27, 43]                [3, 9]      [10, 82]
        [27, 38, 43]                  [3, 9, 10, 82]
                [3, 9, 10, 27, 38, 43, 82]
```

Tout le travail est dans la **remontée**. La descente ne fait que couper en deux
au milieu — aucune décision.

### La fusion

C'est le cœur de l'algorithme. Deux tableaux triés, un pointeur dans chacun : on
prend le plus petit des deux têtes, on avance.

```c
static void fusionner(int tab[], int gauche, int milieu, int droite, int tampon[]) {
    int i = gauche;         // curseur dans la moitié gauche
    int j = milieu + 1;     // curseur dans la moitié droite
    int k = gauche;         // curseur d'écriture dans le tampon

    while (i <= milieu && j <= droite) {
        if (tab[i] <= tab[j]) {
            tampon[k++] = tab[i++];
        } else {
            tampon[k++] = tab[j++];
        }
    }

    while (i <= milieu)  tampon[k++] = tab[i++];    // reste de gauche
    while (j <= droite)  tampon[k++] = tab[j++];    // reste de droite

    for (int m = gauche; m <= droite; m++) {
        tab[m] = tampon[m];
    }
}
```

**Le `<=` de la comparaison décide de la stabilité.** À égalité, on prend
l'élément de **gauche** — celui qui venait en premier. Écrivez `<` et le tri
devient instable. Une seule touche, encore.

Les deux `while` de fin ne peuvent pas s'exécuter tous les deux : quand on sort
de la boucle principale, un des deux côtés est épuisé.

### Le tri

```c
static void tri_fusion_rec(int tab[], int gauche, int droite, int tampon[]) {
    if (gauche >= droite) {
        return;                  // 0 ou 1 élément : déjà trié
    }
    int milieu = gauche + (droite - gauche) / 2;

    tri_fusion_rec(tab, gauche, milieu, tampon);
    tri_fusion_rec(tab, milieu + 1, droite, tampon);
    fusionner(tab, gauche, milieu, droite, tampon);
}

int tri_fusion(int tab[], int n) {
    if (n <= 1) return 0;

    int *tampon = malloc(n * sizeof(int));
    if (tampon == NULL) {
        return -1;               // l'appelant doit savoir que ça a échoué
    }
    tri_fusion_rec(tab, 0, n - 1, tampon);
    free(tampon);
    return 0;
}
```

**Le tampon est alloué une seule fois**, dans la fonction publique, et passé aux
appels récursifs. L'erreur classique est de faire un `malloc` dans `fusionner` :
cela fait `n` allocations au lieu d'une, et le coût des appels système écrase le
gain algorithmique.

Notez aussi le traitement du `malloc` qui échoue. En C, ignorer une allocation
ratée n'est pas une simplification : c'est un déréférencement de `NULL` en
attente.

### Analyse

**Temps** : O(n log n) **dans tous les cas**. Pire, moyen, meilleur : identiques.
La découpe se fait toujours exactement au milieu, indépendamment des données.

Pourquoi O(n log n) ? Il y a `log₂ n` niveaux de découpe. À chaque niveau, la
fusion touche chacun des `n` éléments exactement une fois. Donc `n × log n`.

**Espace** : **O(n)**. C'est son défaut, et il est structurel : la fusion ne peut
pas se faire en place sans faire exploser le temps.

**Stable** : oui, avec le `<=`.

**Adaptatif** : non dans cette version. Il existe des variantes qui le sont
(TimSort, le tri de Python et de Java, détecte les segments déjà triés).

### Quand le choisir

- Quand la **stabilité** est requise.
- Quand il faut une **garantie** O(n log n), sans pire cas dégradé.
- Pour trier des **listes chaînées** : la fusion n'y a pas besoin de tampon, on
  recâble les pointeurs. Le tri fusion y devient O(1) en espace, et c'est le
  meilleur tri possible sur cette structure.
- Pour le **tri externe**, quand les données ne tiennent pas en RAM : on trie des
  blocs, on les écrit sur disque, on les fusionne. C'est sa raison d'être
  historique.

## Tri rapide

### L'idée

On choisit un élément, le **pivot**. On réorganise le tableau pour que tout ce
qui est plus petit soit à gauche, tout ce qui est plus grand à droite. Le pivot
est alors à sa place définitive. On recommence sur les deux côtés.

La symétrie avec le tri fusion est exacte, et inversée :

| | Tri fusion | Tri rapide |
|---|---|---|
| Découpe | triviale (au milieu) | coûteuse (partition) |
| Combinaison | coûteuse (fusion) | triviale (rien à faire) |

Le tri fusion travaille en remontant, le tri rapide en descendant. Quand les
deux moitiés du tri rapide sont triées, il n'y a rien à combiner : elles sont
déjà aux bonnes positions.

### La partition de Lomuto

Le schéma le plus simple à écrire et à comprendre. Le pivot est le dernier
élément.

```c
static int partitionner(int tab[], int gauche, int droite) {
    int pivot = tab[droite];
    int i = gauche - 1;              // frontière des « plus petits »

    for (int j = gauche; j < droite; j++) {
        if (tab[j] <= pivot) {
            i++;
            int tmp = tab[i]; tab[i] = tab[j]; tab[j] = tmp;
        }
    }

    int tmp = tab[i + 1]; tab[i + 1] = tab[droite]; tab[droite] = tmp;
    return i + 1;                    // position finale du pivot
}
```

**L'invariant** : à tout moment, `tab[gauche..i]` contient les éléments ≤ pivot,
et `tab[i+1..j-1]` ceux qui sont > pivot. Le dernier échange place le pivot juste
après la zone des petits.

### Le tri

```c
void tri_rapide(int tab[], int gauche, int droite) {
    if (gauche >= droite) {
        return;
    }
    int p = partitionner(tab, gauche, droite);
    tri_rapide(tab, gauche, p - 1);
    tri_rapide(tab, p + 1, droite);
}
```

Le pivot en `p` est exclu des deux appels : il est définitivement à sa place.

### Le problème du pivot

C'est ici que tout se joue.

**Si le pivot tombe près du milieu**, les deux moitiés sont équilibrées, il y a
`log n` niveaux, et le total est O(n log n).

**Si le pivot est le minimum ou le maximum**, une partition est vide et l'autre
contient `n-1` éléments. Il y a `n` niveaux, et le total est **O(n²)**.

Or avec le pivot en dernière position, quel est le pire cas ? Un tableau **déjà
trié**. Le dernier élément est alors toujours le maximum.

C'est le paradoxe du tri rapide naïf : il s'effondre précisément sur le cas le
plus courant en pratique. Et pas seulement en temps — la récursion atteint une
profondeur `n`, ce qui provoque un **débordement de pile** sur un grand tableau.

### Les remèdes

**Le pivot médian de trois.** On regarde le premier, le milieu et le dernier, on
prend la valeur médiane des trois.

```c
static int choisir_pivot(int tab[], int gauche, int droite) {
    int milieu = gauche + (droite - gauche) / 2;

    if (tab[gauche] > tab[milieu])  { /* échanger gauche et milieu */ }
    if (tab[gauche] > tab[droite])  { /* échanger gauche et droite */ }
    if (tab[milieu] > tab[droite])  { /* échanger milieu et droite */ }
    // tab[milieu] est maintenant la médiane des trois
    return milieu;
}
```

Cela élimine le pire cas sur les données triées ou inversées — les deux
distributions les plus fréquentes dans la vraie vie. Un adversaire qui connaît
l'algorithme peut encore construire un pire cas ; un utilisateur normal, non.

**Le pivot aléatoire.** `rand() % (droite - gauche + 1)`. Le pire cas devient
improbable au sens statistique, quelles que soient les données. C'est le choix
quand l'entrée peut être hostile.

**La récursion sur la plus petite moitié.** On récurse sur le petit côté, on
boucle sur le grand. La profondeur de pile est alors garantie O(log n), même si
le temps reste O(n²) au pire.

```c
void tri_rapide(int tab[], int gauche, int droite) {
    while (gauche < droite) {
        int p = partitionner(tab, gauche, droite);

        if (p - gauche < droite - p) {
            tri_rapide(tab, gauche, p - 1);   // récursion sur le petit côté
            gauche = p + 1;                   // boucle sur le grand
        } else {
            tri_rapide(tab, p + 1, droite);
            droite = p - 1;
        }
    }
}
```

**Le seuil d'insertion.** En dessous de ~16 éléments, on arrête de récurser et on
appelle un tri par insertion. Gain typique de 10 à 20 % — c'est là qu'on retrouve
la leçon précédente.

### Analyse

**Temps** : O(n log n) en moyenne, **O(n²) au pire**.
**Espace** : O(log n) de pile en moyenne, O(n) au pire (O(log n) garanti avec la
récursion sur le petit côté).
**Stable** : **non**. Les échanges à distance de la partition détruisent l'ordre
relatif des égaux.
**Adaptatif** : non.

### Pourquoi il domine malgré son pire cas

Le tri rapide est le tri le plus utilisé au monde, alors qu'il est O(n²) au pire
et instable. Trois raisons.

**Ses constantes sont excellentes.** La partition est une boucle simple avec des
accès séquentiels — le cas idéal pour le cache du processeur. En pratique il est
2 à 3 fois plus rapide que le tri fusion sur un tableau, à complexité affichée
égale.

**Il est en place.** Pas de tampon de `n` entiers. Sur des données volumineuses,
cela change la faisabilité, pas juste la vitesse.

**Son pire cas est évitable.** Médian de trois, ou pivot aléatoire, et il ne se
présente plus.

C'est le cas d'école de la distance entre l'analyse asymptotique et la
performance réelle. Un algorithme au pire cas moins bon, choisi en connaissance
de cause, parce que le pire cas ne se produit pas.

## La borne inférieure

Peut-on faire mieux que O(n log n) ?

**Non — pas avec un tri par comparaisons.** La démonstration est courte.

Un tri qui ne procède que par comparaisons est un arbre de décision : chaque
comparaison est un nœud à deux branches, chaque feuille est une permutation
possible du tableau.

Il y a `n!` permutations possibles, donc au moins `n!` feuilles. Un arbre binaire
de hauteur `h` a au plus `2^h` feuilles. Il faut donc `2^h ≥ n!`, soit
`h ≥ log₂(n!)`.

Et par l'approximation de Stirling, `log₂(n!) ≈ n log₂ n`.

Le nombre de comparaisons dans le pire cas est la hauteur de l'arbre. Donc tout
tri par comparaisons fait au moins `n log n` comparaisons au pire.

Le tri fusion atteint cette borne. Il est **optimal** au sens asymptotique.

*Réserve importante :* la démonstration ne vaut que pour les tris **par
comparaisons**. Des tris qui exploitent la structure des clés — tri par
comptage, tri radix — descendent à O(n) ou O(n + k). Ils ne contredisent pas le
théorème : ils ne comparent pas les éléments entre eux.

## Récapitulatif

| | Fusion | Rapide | Insertion |
|---|---|---|---|
| Pire | O(n log n) | O(n²) | O(n²) |
| Moyen | O(n log n) | O(n log n) | O(n²) |
| Meilleur | O(n log n) | O(n log n) | O(n) |
| Espace | O(n) | O(log n) | O(1) |
| Stable | oui | non | oui |
| En place | non | oui | oui |
| Vitesse réelle | bonne | **la meilleure** | petits n |

## Ce qu'il faut retenir

- **Diviser pour régner** : diviser en deux donne `log n` niveaux ; un travail
  O(n) par niveau donne O(n log n).
- **Fusion** : travail à la remontée, stable, garanti O(n log n), coûte O(n) en
  mémoire. Le bon choix pour les listes chaînées et le tri externe.
- **Rapide** : travail à la descente, en place, le plus rapide en pratique,
  O(n²) au pire si le pivot est mal choisi. Médian de trois ou pivot aléatoire
  règlent le problème.
- **O(n log n) est une borne infranchissable** pour un tri par comparaisons.
- Les meilleures implémentations réelles sont hybrides : rapide + insertion sous
  un seuil, avec repli sur un tri par tas si la profondeur dérape (introsort).

---

## À faire maintenant

Implémente le tri fusion et le tri rapide, puis mets-les à l'épreuve.

Trie 100 000 entiers aléatoires avec les deux, en chronométrant. Puis trie
100 000 entiers **déjà triés** avec le tri rapide à pivot en dernière position.

Attends-toi à un crash — débordement de pile. C'est le résultat attendu :
constate-le avant de le corriger.

Corrige ensuite avec le médian de trois, et relance. Le temps doit redevenir
comparable à celui du cas aléatoire.
