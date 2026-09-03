# Recherche : linéaire et dichotomique

## Deux façons de chercher un mot dans un dictionnaire

Vous cherchez « algorithme » dans un dictionnaire papier.

**Méthode 1** : première page, vous lisez chaque mot jusqu'à tomber dessus.
Ça marche. Ça prend la journée.

**Méthode 2** : vous ouvrez au milieu. « Marmotte » — trop loin. Vous ouvrez au
milieu de la première moitié. « Ébène » — trop loin encore. Vous continuez.
En une dizaine d'ouvertures, vous y êtes.

Personne n'utilise la méthode 1. Mais tout le monde l'écrit en code, parce
qu'elle est plus simple — et parce que la méthode 2 exige une condition qu'on
oublie de vérifier : **le dictionnaire est trié**.

Toute la leçon tient dans cette tension.

## Recherche linéaire

On parcourt, on compare, on s'arrête.

```c
int recherche_lineaire(const int tab[], int n, int cible) {
    for (int i = 0; i < n; i++) {
        if (tab[i] == cible) {
            return i;
        }
    }
    return -1;
}
```

**Complexité** : O(n) au pire, O(1) au mieux, O(n) en moyenne.
**Espace** : O(1).

Quelques remarques sur le code lui-même, parce qu'elles reviendront partout.

Le `const` sur `tab` annonce que la fonction ne modifie pas le tableau. Le
compilateur le vérifie. C'est de la documentation exécutable — prenez-en
l'habitude dès maintenant.

Le retour `-1` pour « absent » est une convention, pas une règle. Elle marche
parce qu'aucun indice valide n'est négatif. Une autre approche consiste à
renvoyer un booléen et à passer l'indice par pointeur :

```c
bool recherche_lineaire(const int tab[], int n, int cible, int *indice);
```

Plus verbeux, mais impossible à mal interpréter. Les deux se défendent.

### Quand elle est le bon choix

On la présente souvent comme l'algorithme naïf. Elle reste pourtant le bon choix
dans trois situations :

- **Le tableau n'est pas trié**, et le trier coûterait plus cher que la recherche.
- **Le tableau est petit** (quelques dizaines d'éléments) : les constantes
  dominent, et une boucle simple bat souvent une dichotomie.
- **On cherche une fois.** Trier coûte O(n log n) ; si c'est pour une seule
  recherche, on a perdu.

La bascule se fait dès qu'on cherche **plusieurs fois** dans les mêmes données :
`O(n log n)` de tri, puis `O(log n)` par recherche, l'emporte vite sur `k × O(n)`.

## Recherche dichotomique

Le tableau doit être **trié**. Sans cette garantie, l'algorithme ne renvoie pas
un résultat lent : il renvoie un résultat **faux**.

### Le principe

On maintient un intervalle de recherche `[gauche, droite]`. À chaque tour :

1. On regarde l'élément du milieu.
2. S'il vaut la cible, terminé.
3. S'il est trop grand, la cible est forcément à gauche → on jette la moitié droite.
4. S'il est trop petit, la cible est à droite → on jette la moitié gauche.

Chaque tour **élimine la moitié des candidats**. C'est la définition même du
logarithme.

### Le code

```c
int recherche_dichotomique(const int tab[], int n, int cible) {
    int gauche = 0;
    int droite = n - 1;

    while (gauche <= droite) {
        int milieu = gauche + (droite - gauche) / 2;

        if (tab[milieu] == cible) {
            return milieu;
        } else if (tab[milieu] < cible) {
            gauche = milieu + 1;
        } else {
            droite = milieu - 1;
        }
    }
    return -1;
}
```

**Complexité** : O(log n). **Espace** : O(1).

### Trois détails qui font la différence

Ce code fait dix lignes et contient trois pièges. C'est l'algorithme le plus
célèbre pour ça : une étude classique a montré que la majorité des programmeurs
professionnels l'écrivent faux du premier coup.

**Le calcul du milieu.** Pourquoi `gauche + (droite - gauche) / 2` et pas
`(gauche + droite) / 2` ?

Les deux donnent le même résultat... jusqu'à ce que `gauche + droite` dépasse
`INT_MAX`. Sur un tableau de plus de 2 milliards d'éléments, l'addition déborde
et le résultat devient négatif. C'est le bug qui a dormi neuf ans dans la
bibliothèque standard de Java. La forme `gauche + (droite - gauche) / 2` ne
déborde jamais, puisque `droite - gauche` est au plus `n`.

Vous avez rencontré ce dépassement silencieux dans les exercices du cours de C.
Le voici en conditions réelles.

**Le `<=` de la condition.** Avec `<`, l'intervalle d'un seul élément
(`gauche == droite`) n'est jamais examiné. La recherche échoue sur le dernier
candidat. Essayez : c'est le bug le plus fréquent de la dichotomie.

**Le `+1` et le `-1`.** Sans eux, quand `tab[milieu] != cible`, l'intervalle ne
rétrécit pas et la boucle tourne indéfiniment. Le `milieu` vient d'être testé :
on doit l'exclure.

### La version récursive

```c
int dicho_rec(const int tab[], int gauche, int droite, int cible) {
    if (gauche > droite) {
        return -1;
    }
    int milieu = gauche + (droite - gauche) / 2;

    if (tab[milieu] == cible)   return milieu;
    if (tab[milieu] < cible)    return dicho_rec(tab, milieu + 1, droite, cible);
    return dicho_rec(tab, gauche, milieu - 1, cible);
}
```

Même complexité en temps. En espace, elle passe de O(1) à O(log n) : chaque
appel empile un contexte. Pour n = 1 milliard, cela fait 30 cadres de pile —
négligeable. On y reviendra dans la leçon sur la récursivité.

*Note :* cette récursion est **terminale** (l'appel récursif est la dernière
opération). Un compilateur avec `-O2` la transforme souvent en boucle, ramenant
l'espace à O(1). Vérifiable au désassembleur, mais ne comptez pas dessus.

## Dérouler un exemple

`tab = [2, 5, 8, 12, 16, 23, 38, 56, 72, 91]`, `n = 10`, cible = `23`.

| Tour | gauche | droite | milieu | tab[milieu] | Décision |
|---|---|---|---|---|---|
| 1 | 0 | 9 | 4 | 16 | 16 < 23 → gauche = 5 |
| 2 | 5 | 9 | 7 | 56 | 56 > 23 → droite = 6 |
| 3 | 5 | 6 | 5 | 23 | trouvé, indice 5 |

Trois tours pour dix éléments. La recherche linéaire en aurait fait six.

L'écart paraît modeste. Refaites l'exercice avec un million d'éléments : 20 tours
contre 500 000 en moyenne.

## Les variantes utiles

Trouver l'élément n'est pas toujours la question. Trois variantes reviennent
sans cesse en pratique.

### Première occurrence dans un tableau avec doublons

Le code de base renvoie **une** position, pas forcément la première. Pour la
première, on ne s'arrête pas sur une égalité : on mémorise et on continue à
gauche.

```c
int premiere_occurrence(const int tab[], int n, int cible) {
    int gauche = 0, droite = n - 1, resultat = -1;

    while (gauche <= droite) {
        int milieu = gauche + (droite - gauche) / 2;
        if (tab[milieu] == cible) {
            resultat = milieu;
            droite = milieu - 1;     // on continue à chercher plus à gauche
        } else if (tab[milieu] < cible) {
            gauche = milieu + 1;
        } else {
            droite = milieu - 1;
        }
    }
    return resultat;
}
```

### Point d'insertion

Où faudrait-il insérer la valeur pour conserver le tri ? C'est la brique d'un
tri par insertion dichotomique, et la base des `lower_bound` des bibliothèques
standard.

```c
int point_insertion(const int tab[], int n, int valeur) {
    int gauche = 0, droite = n;          // noter : n, pas n - 1

    while (gauche < droite) {            // noter : <, pas <=
        int milieu = gauche + (droite - gauche) / 2;
        if (tab[milieu] < valeur) {
            gauche = milieu + 1;
        } else {
            droite = milieu;             // noter : milieu, pas milieu - 1
        }
    }
    return gauche;
}
```

Trois différences avec le code de base, et aucune n'est arbitraire. L'intervalle
est ici **semi-ouvert** `[gauche, droite[` : `droite` désigne une position
possible d'insertion, pas un élément existant. C'est pour cela que `n` est une
valeur légitime — insérer après le dernier élément.

Choisir sa convention d'intervalle et **s'y tenir dans tout le corps de la
fonction** : voilà le vrai remède aux erreurs de dichotomie. Les bugs viennent
presque toujours d'un mélange des deux conventions dans la même boucle.

### La dichotomie sur une réponse

La dichotomie ne sert pas qu'à chercher dans un tableau. Elle s'applique dès
qu'une réponse est **monotone** : si `x` convient, tout ce qui est plus grand
convient aussi.

Exemple : trouver la racine carrée entière de `n` sans `math.h`.

```c
int racine_entiere(int n) {
    int gauche = 0, droite = n, resultat = 0;

    while (gauche <= droite) {
        long milieu = gauche + (droite - gauche) / 2;
        if (milieu * milieu <= n) {
            resultat = (int)milieu;      // candidat valide, on tente plus grand
            gauche = (int)milieu + 1;
        } else {
            droite = (int)milieu - 1;
        }
    }
    return resultat;
}
```

Aucun tableau. On cherche dans l'espace des réponses possibles. Le `long` sur
`milieu` évite que `milieu * milieu` déborde — le même réflexe que pour le calcul
du milieu.

C'est l'usage le plus puissant de la dichotomie, et le moins enseigné.

## Le tableau de comparaison

| | Linéaire | Dichotomique |
|---|---|---|
| Temps (pire) | O(n) | O(log n) |
| Temps (meilleur) | O(1) | O(1) |
| Espace | O(1) | O(1) itératif, O(log n) récursif |
| Tableau trié requis | non | **oui** |
| n = 1 000 | ~1 000 ops | ~10 ops |
| n = 1 000 000 | ~1 000 000 ops | ~20 ops |

## Ce qu'il faut retenir

- La recherche linéaire est O(n) et ne suppose rien. Elle reste le bon choix sur
  des données non triées ou petites, ou pour une recherche unique.
- La dichotomie est O(log n) et **exige un tableau trié**. Sur des données non
  triées, elle ne ralentit pas : elle ment.
- `gauche + (droite - gauche) / 2`, jamais `(gauche + droite) / 2`.
- `while (gauche <= droite)` avec des bornes fermées ; `<` avec un intervalle
  semi-ouvert. Une convention, tenue jusqu'au bout.
- La dichotomie s'applique à tout espace de réponses monotone, pas seulement aux
  tableaux.

---

## À faire maintenant

Instrumente les deux recherches pour compter leurs comparaisons, sur un tableau
trié de 1 000 000 d'éléments, en cherchant une valeur absente (pire cas des deux).

Compare les compteurs. Puis recommence avec 2 000 000 : le compteur linéaire
double, le compteur dichotomique augmente de **1**.

C'est toute la différence entre O(n) et O(log n), constatée.
