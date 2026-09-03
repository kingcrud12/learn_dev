# Exercices — Complexité et Big-O

Support : [01_complexite_et_big_o.md](../lessons/01_complexite_et_big_o.md)

Beaucoup de ces exercices se répondent sur papier avant de toucher au clavier.
C'est voulu : l'analyse de complexité est un raisonnement, pas une mesure.

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

**Règle du parcours :** aucun avertissement toléré.

---

# Niveau 1 — Lire une complexité

Réponds sur papier. Justifie en une phrase à chaque fois.

## 1.1 — Six fragments

Donne la complexité en temps de chacun.

```c
// (a)
for (int i = 0; i < n; i++)
    printf("%d\n", i);

// (b)
for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        printf("%d\n", i * j);

// (c)
for (int i = 0; i < n; i++)
    for (int j = 0; j < 100; j++)
        printf("%d\n", i);

// (d)
for (int i = 1; i < n; i = i * 2)
    printf("%d\n", i);

// (e)
for (int i = 0; i < n; i++)
    for (int j = i; j < n; j++)
        printf("%d\n", j);

// (f)
for (int i = 0; i < n; i++)
    for (int j = 1; j < n; j = j * 2)
        printf("%d\n", j);
```

*Piège :* (c) et (e) sont ceux où l'intuition se trompe le plus souvent.

## 1.2 — Simplifier

Réduis à sa classe Big-O :

- `5n + 3`
- `n² + 1000n + 50000`
- `3n³ + n² log n`
- `2ⁿ + n¹⁰⁰`
- `log n + 42`
- `n log n + n`

Pour l'avant-dernière, justifie : `n¹⁰⁰` est un nombre gigantesque, pourquoi
disparaît-il ?

## 1.3 — Ordonner

Classe par ordre croissant de vitesse de croissance :

`O(n²)`, `O(1)`, `O(n log n)`, `O(2ⁿ)`, `O(log n)`, `O(n)`, `O(n³)`, `O(√n)`

`O(√n)` n'était pas dans la leçon. Situe-le en raisonnant : est-il plus proche de
`log n` ou de `n` ?

## 1.4 — Deux boucles successives

```c
for (int i = 0; i < n; i++)      { /* O(1) */ }
for (int i = 0; i < n * n; i++)  { /* O(1) */ }
```

Complexité totale ? Pourquoi n'écrit-on pas `O(n + n²)` ?

## 1.5 — La question du temps

Un algorithme O(n²) traite 1 000 éléments en 1 seconde.

Combien de temps pour 10 000 ? Pour 100 000 ?

Même question pour un algorithme O(n log n) qui traite 1 000 éléments en 1 seconde.

---

# Niveau 2 — Mesurer

On passe au clavier. L'objectif est de **vérifier** les réponses du niveau 1.

## 2.1 — Compteur d'opérations

Écris une fonction `somme_paires` qui, pour un tableau de taille `n`, additionne
toutes les paires `tab[i] + tab[j]` avec `i < j`.

Ajoute un compteur d'additions. Lance avec `n = 100, 200, 400, 800`.

Le compteur est-il multiplié par 2 ou par 4 quand `n` double ? Qu'en conclus-tu ?

## 2.2 — Chronométrer

Avec `<time.h>` :

```c
clock_t debut = clock();
/* ... */
double secondes = (double)(clock() - debut) / CLOCKS_PER_SEC;
```

Chronomètre une boucle O(n²) pour `n = 1000, 2000, 4000, 8000`.

Les temps suivent-ils le facteur 4 attendu ? Note les écarts et propose une
explication.

## 2.3 — La constante cachée

Écris deux fonctions :

- `lente_lineaire(n)` : boucle O(n) faisant 1 000 opérations par tour
- `rapide_quadratique(n)` : boucle O(n²) faisant 1 opération par tour

Trouve expérimentalement le `n` à partir duquel la linéaire devient plus rapide.

Que dit ce résultat sur l'usage de Big-O pour choisir un algorithme ?

## 2.4 — Croissance logarithmique

Écris une boucle `for (int i = n; i > 0; i /= 2)` avec un compteur.

Lance avec `n = 1000, 1000000, 1000000000`. Trois valeurs, trois compteurs.

Le compteur triple-t-il ? Compare-le à `log₂(n)`.

## 2.5 — Espace mémoire

Écris deux fonctions qui inversent un tableau : l'une en place, l'autre en
allouant un second tableau.

Pour `n = 10 000 000` d'entiers, calcule la mémoire supplémentaire de chacune.

---

# Niveau 3 — Analyser du vrai code

## 3.1 — Doublons, version naïve

Écris `contient_doublon(tab, n)` qui renvoie `true` si une valeur apparaît deux
fois. Deux boucles imbriquées.

Complexité ? Nombre exact de comparaisons pour `n = 1000` dans le pire cas ?

## 3.2 — Doublons, version triée

Réécris la fonction en triant d'abord (utilise `qsort` de `<stdlib.h>`) puis en
comparant les voisins.

Complexité ? À partir de quelle taille bat-elle la version naïve ? Mesure.

## 3.3 — Trois sommes

Étant donné un tableau et une cible, trouve trois éléments dont la somme vaut la
cible. Version à trois boucles imbriquées.

Complexité ? Pour `n = 1000`, combien de triplets examinés ? Estime le temps.

## 3.4 — Sous-tableau de somme maximale

Trouve le sous-tableau contigu de somme maximale.

Écris d'abord la version à trois boucles (tous les débuts, toutes les fins, somme
de chaque). Complexité ?

Puis la version à deux boucles, en accumulant la somme au lieu de la recalculer.
Complexité ?

*Bonus difficile :* il existe une solution O(n), l'algorithme de Kadane. Cherche
la seule idée qui manque, ne cherche pas le code.

## 3.5 — La fonction mystère

```c
int mystere(int n) {
    int compteur = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j += i) {
            compteur++;
        }
    }
    return compteur;
}
```

Le pas de la boucle interne dépend de `i`. Combien de tours pour `i = 1` ? Pour
`i = 2` ? Pour `i = n` ?

Écris la somme totale. Elle vaut `n × (1 + 1/2 + 1/3 + ... + 1/n)`.

Cette somme entre parenthèses est la série harmonique, qui vaut environ `ln(n)`.
Conclusion sur la complexité ?

Vérifie par la mesure.

---

# Niveau 4 — Raisonner sur les cas

## 4.1 — Trois cas

Pour chacun, donne meilleur, pire et cas moyen :

- recherche linéaire dans un tableau non trié
- vérifier si un tableau est trié
- trouver le maximum d'un tableau
- recherche dichotomique

Lequel a le même coût dans les trois cas ? Pourquoi est-ce une propriété
intéressante ?

## 4.2 — Fabriquer le pire cas

Écris une fonction `est_trie(tab, n)` qui s'arrête dès qu'elle trouve une
inversion.

Construis deux tableaux de 1 000 000 d'éléments : l'un déclenchant le meilleur
cas, l'autre le pire. Mesure les deux.

## 4.3 — Le coût amorti

Écris un tableau dynamique qui double sa capacité quand il est plein.

Insère 1 000 000 d'éléments en comptant le **nombre total de copies** effectuées
lors des réallocations.

Divise par 1 000 000. Le résultat doit être petit — autour de 2. C'est ce que
signifie « O(1) amorti » : quelques opérations coûteuses, noyées dans la masse.

## 4.4 — Complexité en espace

Donne la complexité en espace :

```c
// (a)
int somme(int tab[], int n) {
    int s = 0;
    for (int i = 0; i < n; i++) s += tab[i];
    return s;
}

// (b)
int somme_rec(int tab[], int n) {
    if (n == 0) return 0;
    return tab[n-1] + somme_rec(tab, n-1);
}

// (c)
int *doubler(int tab[], int n) {
    int *sortie = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) sortie[i] = tab[i] * 2;
    return sortie;
}
```

(a) et (b) calculent la même chose. Pourquoi leurs espaces diffèrent-ils ?

---

# Auto-évaluation

- [ ] Pourquoi jette-t-on les constantes dans Big-O ?
- [ ] Quelle est la différence entre O(n log n) et O(n²) pour n = 1 000 000 ?
- [ ] Pourquoi analyse-t-on par défaut le pire cas ?
- [ ] Une boucle `for (i = 0; i < n; i++) for (j = i; j < n; j++)` est-elle
      O(n²) ou O(n²/2) ?
- [ ] Un algorithme O(n) est-il toujours plus rapide qu'un O(n²) ?
- [ ] Qu'est-ce que la complexité en espace, et pourquoi la récursion en
      consomme-t-elle ?

---

# Solutions

Aucune n'est donnée.

Trois recours dans l'ordre :

1. **Relire la leçon** — chaque exercice pointe une section précise.
2. **Mesurer** — un compteur d'opérations tranche un désaccord d'analyse.
3. **Expliquer à voix haute**, boucle par boucle.

Sur cette leçon en particulier : quand une analyse et une mesure se contredisent,
c'est presque toujours l'analyse qui a tort. Refais-la avant d'accuser la machine.
