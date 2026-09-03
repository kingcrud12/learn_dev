# Exercices — Tris efficaces

Support : [04_tris_efficaces.md](../lessons/04_tris_efficaces.md)

Reprends l'outillage du niveau 0 des exercices précédents (`afficher`,
`est_trie`, les générateurs, les compteurs).

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

---

# Niveau 1 — Tri fusion

## 1.1 — La fusion seule

Avant le tri, écris uniquement la fusion.

Prends deux tableaux **déjà triés** et fusionne-les dans un troisième.

```
[1, 5, 9, 12]  +  [2, 3, 8, 20]  →  [1, 2, 3, 5, 8, 9, 12, 20]
```

Teste les cas limites : un des deux vide, tailles très inégales, tous les
éléments de l'un plus petits que ceux de l'autre.

## 1.2 — Le tri complet

Écris `tri_fusion` avec le tampon alloué **une seule fois** dans la fonction
publique.

Traite l'échec du `malloc` : renvoie un code d'erreur, ne l'ignore pas.

## 1.3 — L'erreur du malloc interne

Écris une seconde version où `fusionner` fait son propre `malloc`/`free`.

Chronomètre les deux sur 1 000 000 d'éléments. Note le rapport.

Combien d'allocations fait la mauvaise version ? (Ajoute un compteur global.)

## 1.4 — Visualiser la récursion

Ajoute un paramètre `profondeur` et affiche l'indentation à chaque appel :

```
fusion [0..7]
  fusion [0..3]
    fusion [0..1]
      fusion [0..0]
      fusion [1..1]
    fusionner [0..1]
  ...
```

Sur 8 éléments. Compte les niveaux : tu dois en trouver `log₂(8) = 3`.

## 1.5 — Les trois cas

Lance le tri fusion sur aléatoire, trié, inversé, avec `n = 100000`.

Les trois nombres de comparaisons doivent être quasi identiques. Pourquoi ?

## 1.6 — La stabilité

Reprends le test des élèves (exercice 3.1 de la leçon précédente). Le tri fusion
est-il stable ?

Puis remplace `tab[i] <= tab[j]` par `tab[i] < tab[j]` et refais le test.

---

# Niveau 2 — Tri rapide

## 2.1 — La partition seule

Écris `partitionner` avec le pivot en dernière position.

Teste-la isolément : affiche le tableau avant et après, et l'indice renvoyé.

Vérifie à la main que tout ce qui est à gauche de l'indice est ≤ pivot.

## 2.2 — Le tri complet

Écris `tri_rapide`. Vérifie avec `est_trie` sur 100 000 éléments aléatoires.

## 2.3 — Provoquer le pire cas

Lance le tri rapide (pivot en dernière position) sur un tableau **déjà trié** de
100 000 éléments.

Attends-toi à un `Segmentation fault`.

Diminue la taille jusqu'à ce que ça passe : `50000`, `20000`, `10000`. Trouve la
limite sur ta machine.

Puis compare `ulimit -s` (taille de pile en Ko) à cette limite. Déduis la taille
d'un cadre de pile.

## 2.4 — Mesurer la dégradation

Sur une taille qui passe (`5000` par exemple), compare le nombre de comparaisons
entre le cas aléatoire et le cas trié.

Le rapport doit être de l'ordre de `n / log n`. Vérifie.

## 2.5 — Le médian de trois

Implémente `choisir_pivot` avec la médiane de trois. N'oublie pas d'échanger la
médiane en dernière position avant d'appeler `partitionner`.

Relance sur le tableau trié de 100 000 éléments. Plus de crash, et le temps doit
redevenir comparable au cas aléatoire.

## 2.6 — Le pivot aléatoire

Autre version : `rand() % (droite - gauche + 1) + gauche`, échangé en dernière
position.

Compare les deux stratégies sur : aléatoire, trié, inversé, et un tableau où
toutes les valeurs sont identiques.

Le dernier cas est intéressant. Que se passe-t-il ? Pourquoi ?

## 2.7 — La récursion sur le petit côté

Implémente la version qui récurse sur la plus petite moitié et boucle sur la
grande.

Ajoute un compteur de profondeur maximale de récursion.

Sur un tableau trié avec pivot en dernière position (le pire cas), compare la
profondeur maximale entre cette version et la version naïve.

Cette version ne crashe plus, même si le temps reste O(n²). Explique la
différence entre les deux problèmes.

---

# Niveau 3 — Le duel

## 3.1 — Fusion contre rapide

Sur 1 000 000 d'entiers aléatoires, chronomètre :

- tri fusion
- tri rapide avec médian de trois
- `qsort` de la bibliothèque standard

Classe-les. Le résultat te surprendra peut-être.

## 3.2 — Pourquoi qsort gagne (ou perd)

`qsort` prend une fonction de comparaison par pointeur. Chaque comparaison est
donc un **appel de fonction indirect**, que le compilateur ne peut pas mettre en
ligne.

Cela devrait le pénaliser. Est-ce le cas dans ta mesure ? Si non, pourquoi ?

*Piste :* regarde ce qu'est réellement l'implémentation de `qsort` sur ta
plateforme — ce n'est pas un tri rapide pur.

## 3.3 — La mémoire

Mesure la mémoire de pointe des deux tris sur 10 000 000 d'entiers.

```sh
/usr/bin/time -l ./codes/tri     # macOS
/usr/bin/time -v ./codes/tri     # Linux
```

Le tri fusion doit consommer ~40 Mo de plus. Vérifie que cela correspond bien à
`n × sizeof(int)`.

## 3.4 — Le tri hybride

Ajoute un seuil au tri rapide : en dessous de `SEUIL` éléments, appelle un tri
par insertion.

Teste `SEUIL = 1, 4, 8, 16, 32, 64` sur 1 000 000 d'éléments. Trace la courbe des
temps.

Où est le minimum ? Compare-le au seuil que tu avais trouvé à l'exercice 5.1 de
la leçon précédente.

## 3.5 — Le gain total

Compare ton tri rapide final (médian de trois + seuil d'insertion + récursion sur
le petit côté) à ta première version naïve.

Note le gain sur données aléatoires, et sur données triées.

---

# Niveau 4 — Aller plus loin

## 4.1 — Tri fusion itératif

Version ascendante, sans récursion : on fusionne d'abord des blocs de 1, puis de
2, puis de 4...

```
pour taille = 1, 2, 4, 8, ... tant que taille < n :
    pour chaque paire de blocs consécutifs de cette taille :
        fusionner
```

Même complexité, aucune pile. Vérifie que le résultat est identique.

## 4.2 — Partition de Hoare

L'autre schéma de partition, avec deux curseurs qui se rapprochent.

```
i = gauche - 1 ; j = droite + 1
boucle :
    faire i++ tant que tab[i] < pivot
    faire j-- tant que tab[j] > pivot
    si i >= j : renvoyer j
    échanger tab[i] et tab[j]
```

Attention : cette version renvoie un point de séparation, **pas** la position
finale du pivot. Les appels récursifs deviennent `(gauche, j)` et `(j+1, droite)`.

Compare le nombre d'échanges avec Lomuto sur les mêmes données. Hoare en fait
environ trois fois moins.

## 4.3 — Le tri à trois voies

Sur un tableau contenant beaucoup de doublons, la partition classique est
inefficace : les éléments égaux au pivot sont retraités à chaque niveau.

Implémente une partition en **trois** zones : `< pivot`, `== pivot`, `> pivot`.
Seules les zones extrêmes sont récursées.

Teste sur un tableau de 1 000 000 d'éléments ne contenant que 10 valeurs
distinctes. Compare à la version classique.

## 4.4 — Tri par comptage

Pour des entiers dans une plage connue et petite (`0..k`) :

```
compter les occurrences de chaque valeur
reconstruire le tableau à partir des comptes
```

O(n + k). Il **bat la borne O(n log n)** — sans la contredire, puisqu'il ne
compare aucun élément entre eux.

Trie 10 000 000 d'entiers entre 0 et 100. Compare à `qsort`.

## 4.5 — Sélection rapide

Trouver le `k`-ième plus petit élément **sans trier tout le tableau**.

C'est le tri rapide où l'on ne récurse que du côté qui contient le rang cherché.

Complexité moyenne : **O(n)**, pas O(n log n).

Compare, pour trouver la médiane de 1 000 000 d'éléments : ta sélection rapide
contre un tri complet suivi d'un accès à l'indice `n/2`.

## 4.6 — Compter les inversions

Une inversion est une paire `(i, j)` avec `i < j` et `tab[i] > tab[j]`. C'est une
mesure du désordre d'un tableau.

La version naïve est O(n²). Le tri fusion permet de le faire en O(n log n) :
pendant la fusion, quand on prend un élément de la moitié **droite**, il est plus
petit que tous les éléments restants de la moitié gauche. Compte-les.

Vérifie ta fonction : un tableau trié a 0 inversion, un tableau inversé en a
`n(n-1)/2`.

Rapproche ce résultat de l'adaptativité du tri par insertion, qui est O(n + d)
où `d` est justement ce nombre.

---

# Auto-évaluation

- [ ] Pourquoi le tri fusion est-il O(n log n) dans tous les cas ?
- [ ] Pourquoi le tri rapide est-il O(n²) sur un tableau trié avec pivot en fin ?
- [ ] Pourquoi le tri rapide est-il quand même le plus utilisé ?
- [ ] Pourquoi un tri par comparaisons ne peut-il pas faire mieux que O(n log n) ?
- [ ] Pourquoi le tri par comptage échappe-t-il à cette borne ?
- [ ] Quel tri choisirais-tu pour une liste chaînée ? Pourquoi ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Afficher les sous-tableaux à chaque appel récursif**, avec indentation.
   Sur un tri récursif, c'est le seul débogage qui fonctionne vraiment.
2. **Tester la partition et la fusion isolément**, avant de les intégrer.
   La quasi-totalité des bugs sont là.
3. **`est_trie` après chaque tri**, systématiquement. Un tri qu'on ne vérifie
   pas n'est pas testé.
