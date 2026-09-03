# Tris élémentaires : sélection, insertion, bulles

## Pourquoi étudier des algorithmes qu'on n'utilisera pas

Les trois tris de cette leçon sont tous O(n²). Aucune bibliothèque sérieuse ne
les emploie pour trier un gros tableau. Alors pourquoi les apprendre ?

Trois raisons, et la troisième est la vraie.

**Ils sont la référence.** On ne comprend pas pourquoi le tri fusion est bon
tant qu'on n'a pas senti à quel point le tri par sélection est lent.

**Ils gagnent sur les petits tableaux.** En dessous de ~16 éléments, le tri par
insertion bat le tri rapide. Les implémentations professionnelles de `qsort`
basculent dessus. Ce n'est pas une curiosité : c'est le premier cas concret où
Big-O ne suffit pas à décider.

**Ils introduisent le vocabulaire.** Stabilité, tri en place, adaptativité,
invariant de boucle. Ces notions se comprennent mieux sur du code simple, et
elles resserviront à chaque algorithme.

## Le vocabulaire, d'abord

**Tri en place** : l'algorithme n'utilise qu'une mémoire supplémentaire O(1). Il
réarrange le tableau existant au lieu d'en construire un autre.

**Tri stable** : deux éléments de même clé conservent leur ordre relatif d'origine.

Cela paraît théorique. Ça ne l'est pas. Imaginez une liste d'employés déjà triée
par prénom, que vous retriez par service :

```
Avant (trié par prénom)      Après un tri stable par service
Alice   — Compta             Alice   — Compta
Bernard — RH                 David   — Compta
Chloé   — RH                 Bernard — RH
David   — Compta             Chloé   — RH
```

Avec un tri stable, à l'intérieur de chaque service les prénoms restent triés.
Avec un tri instable, cet ordre est perdu. C'est ce qui permet de trier sur
plusieurs critères en enchaînant des tris simples.

**Tri adaptatif** : l'algorithme est plus rapide si les données sont déjà
partiellement triées.

## Tri par sélection

### L'idée

À chaque tour, on cherche le plus petit élément de la partie non triée et on le
met à sa place définitive.

```
[64, 25, 12, 22, 11]   min = 11, échange avec position 0
[11, 25, 12, 22, 64]   min du reste = 12, échange avec position 1
[11, 12, 25, 22, 64]   min du reste = 22, échange avec position 2
[11, 12, 22, 25, 64]   min du reste = 25, déjà en place
[11, 12, 22, 25, 64]   trié
```

### Le code

```c
void tri_selection(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int indice_min = i;

        for (int j = i + 1; j < n; j++) {
            if (tab[j] < tab[indice_min]) {
                indice_min = j;
            }
        }

        if (indice_min != i) {
            int tmp = tab[i];
            tab[i] = tab[indice_min];
            tab[indice_min] = tmp;
        }
    }
}
```

**L'invariant** : à la fin du tour `i`, les cases `0..i` contiennent les `i+1`
plus petits éléments, dans l'ordre, à leur place définitive.

Savoir énoncer l'invariant d'une boucle, c'est savoir prouver que l'algorithme
est correct. Prenez l'habitude de le chercher — c'est la question à se poser
devant tout algorithme qu'on ne comprend pas.

### Analyse

**Temps** : O(n²) **dans tous les cas**. La boucle interne parcourt toujours
tout le reste du tableau, que les données soient triées ou non. Exactement
`n(n-1)/2` comparaisons, toujours.

**Espace** : O(1), en place.

**Stable** : **non**. L'échange à distance peut faire passer un élément par-dessus
un autre de même clé. Avec `[2a, 2b, 1]` : on échange le `1` avec le `2a`, ce
qui donne `[1, 2b, 2a]`. L'ordre relatif des deux `2` est inversé.

**Adaptatif** : non.

**Son seul avantage** : il fait au plus `n-1` **échanges**, le minimum possible.
Si écrire en mémoire coûte très cher (mémoire flash à écritures limitées, par
exemple), c'est un argument réel. C'est la seule situation où on le choisit.

## Tri par insertion

### L'idée

C'est ainsi qu'on trie des cartes à jouer. On prend la carte suivante et on la
glisse à sa place dans la main déjà triée, en décalant les autres.

```
[64, 25, 12, 22, 11]
 |--| trié            insère 25 : glisse avant 64
[25, 64, 12, 22, 11]
 |----| trié          insère 12 : glisse tout au début
[12, 25, 64, 22, 11]
 |--------| trié      insère 22 : glisse entre 12 et 25
[12, 22, 25, 64, 11]
 |------------| trié  insère 11 : glisse tout au début
[11, 12, 22, 25, 64]
```

### Le code

```c
void tri_insertion(int tab[], int n) {
    for (int i = 1; i < n; i++) {
        int cle = tab[i];
        int j = i - 1;

        while (j >= 0 && tab[j] > cle) {
            tab[j + 1] = tab[j];
            j--;
        }
        tab[j + 1] = cle;
    }
}
```

Trois points à comprendre précisément.

**On sauvegarde `cle` avant de décaler.** Sans cela, le premier décalage
`tab[j+1] = tab[j]` écraserait la valeur qu'on est en train de placer.

**L'ordre du `&&` n'est pas interchangeable.** `j >= 0 && tab[j] > cle` : si `j`
devient `-1`, C évalue en court-circuit et n'exécute jamais `tab[-1]`. Écrivez
les deux conditions dans l'autre sens et vous lisez hors du tableau. Le programme
semblera fonctionner — c'est justement le problème.

**On décale, on n'échange pas.** Un échange fait trois affectations, un décalage
une seule. Sur des données aléatoires, cela rend ce tri environ deux fois plus
rapide que le tri à bulles, à complexité identique.

### Analyse

**Temps** :
- Pire cas (tableau trié à l'envers) : O(n²), `n(n-1)/2` comparaisons.
- Meilleur cas (tableau déjà trié) : **O(n)**. Le `while` ne s'exécute jamais,
  une seule comparaison par élément.
- Moyen : O(n²).

**Espace** : O(1), en place.

**Stable** : **oui**. La condition est `tab[j] > cle`, strictement. Un élément
égal n'est pas décalé, donc le nouvel élément se pose après lui : l'ordre
d'origine est préservé. Remplacez `>` par `>=` et le tri devient instable — une
seule touche de clavier.

**Adaptatif** : **oui**, et c'est sa qualité maîtresse. Sur un tableau presque
trié, il approche O(n). Plus précisément, sa complexité est O(n + d), où `d` est
le nombre d'inversions dans les données.

### Pourquoi c'est celui qui compte

C'est le seul tri élémentaire réellement utilisé en production, pour deux raisons.

Il est **adaptatif** : les données du monde réel sont rarement dans un ordre
totalement aléatoire.

Il a **d'excellentes constantes** : boucle simple, accès mémoire contigus, très
bon comportement du cache. En dessous de ~16 éléments, il bat le tri rapide malgré
sa complexité asymptotique inférieure.

C'est pour cela que le tri rapide de la bibliothèque standard, arrivé sur un
sous-tableau de moins de 16 cases, arrête de récurser et appelle un tri par
insertion. Le tri « lent » est la dernière étape du tri « rapide ».

## Tri à bulles

### L'idée

On parcourt le tableau en comparant les éléments deux à deux, et on échange ceux
qui sont dans le désordre. À chaque passe, le plus grand élément restant
« remonte » jusqu'à sa place, comme une bulle.

### Le code, avec sa seule optimisation qui vaille

```c
void tri_bulles(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        bool echange = false;

        for (int j = 0; j < n - 1 - i; j++) {
            if (tab[j] > tab[j + 1]) {
                int tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
                echange = true;
            }
        }

        if (!echange) {
            break;          // aucun échange : le tableau est trié
        }
    }
}
```

Le `- i` de la boucle interne exploite le fait que les `i` derniers éléments sont
déjà à leur place définitive. Sans lui, on refait un travail inutile.

Le drapeau `echange` rend le tri adaptatif : sur un tableau déjà trié, une seule
passe suffit à le détecter, et on sort. Cela le fait passer de O(n²) à O(n) dans
le meilleur cas.

### Analyse

**Temps** : O(n²) au pire et en moyenne, O(n) au mieux avec le drapeau.
**Espace** : O(1). **Stable** : oui. **Adaptatif** : oui, avec le drapeau.

Sur le papier, il ressemble donc au tri par insertion. En pratique il est
nettement plus lent : il fait des **échanges** (trois écritures) là où l'insertion
fait des **décalages** (une écriture), et sur des données aléatoires il en fait
environ trois fois plus.

**Il n'a aucun cas d'usage.** Le tri par insertion est meilleur sur tous les
critères. On l'enseigne parce qu'il est le plus simple à écrire — et c'est bien
la seule raison de le connaître.

## Comparaison

| | Sélection | Insertion | Bulles |
|---|---|---|---|
| Pire | O(n²) | O(n²) | O(n²) |
| Moyen | O(n²) | O(n²) | O(n²) |
| Meilleur | O(n²) | **O(n)** | O(n) |
| Espace | O(1) | O(1) | O(1) |
| Stable | non | **oui** | oui |
| Adaptatif | non | **oui** | oui |
| Échanges | **O(n)** | O(n²) | O(n²) |
| À utiliser | jamais* | petits tableaux | jamais |

\* sauf si le coût d'écriture domine tout le reste.

## Le point qui compte

Ces trois algorithmes ont la même complexité et des comportements réels très
différents. C'est la démonstration la plus nette de la limite de Big-O : il classe
les algorithmes en catégories, il ne les départage pas à l'intérieur d'une
catégorie.

Pour départager, il faut regarder ce que la notation ignore : le nombre
d'écritures, l'adaptativité, la stabilité, le comportement du cache.

## Ce qu'il faut retenir

- **Sélection** : minimum d'échanges, jamais adaptatif, instable. Cas d'usage
  quasi nul.
- **Insertion** : stable, adaptatif, excellent sur les petits tableaux et les
  données presque triées. **Le seul qui serve vraiment.**
- **Bulles** : dominé par l'insertion sur tous les plans. Valeur pédagogique
  uniquement.
- Un **invariant de boucle** est la façon de prouver qu'un tri est correct.
- La **stabilité** tient parfois à un `>` contre un `>=`.

---

## À faire maintenant

Écris les trois tris dans un même fichier, avec un compteur de comparaisons **et**
un compteur d'écritures pour chacun.

Lance-les sur trois jeux de 2 000 éléments : aléatoire, déjà trié, trié à
l'envers.

Neuf résultats. Trois constats à vérifier :

1. La sélection donne le même nombre de comparaisons dans les trois cas.
2. L'insertion sur un tableau trié fait environ 2 000 comparaisons — pas 2 000 000.
3. Les bulles font environ trois fois plus d'écritures que l'insertion sur les
   données aléatoires.

Le troisième constat n'apparaît nulle part dans la notation Big-O. C'est
exactement pourquoi on mesure.
