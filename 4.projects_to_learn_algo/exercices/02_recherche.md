# Exercices — Recherche

Support : [02_recherche.md](../lessons/02_recherche.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

**Règle du parcours :** aucun avertissement toléré.

---

# Niveau 1 — Recherche linéaire

## 1.1 — La base

Écris `int recherche_lineaire(const int tab[], int n, int cible)` qui renvoie
l'indice de la cible, ou `-1`.

Teste : élément présent au début, au milieu, à la fin, absent, tableau vide.

Les cinq cas, pas trois. Le tableau vide est celui qu'on oublie.

## 1.2 — Compter les occurrences

Combien de fois la cible apparaît-elle ? Renvoie le compte.

## 1.3 — Dernière occurrence

Renvoie l'indice de la **dernière** occurrence. Deux façons : parcourir en
mémorisant, ou parcourir à l'envers. Écris les deux.

Laquelle est plus rapide en moyenne ? Pourquoi ?

## 1.4 — Chercher un minimum et un maximum en un seul passage

Renvoie les deux par pointeur. Combien de comparaisons fais-tu ?

*Bonus :* on peut descendre à `3n/2` comparaisons en traitant les éléments par
paires. Trouve comment.

## 1.5 — Recherche dans un tableau de chaînes

```c
const char *noms[] = {"alice", "bob", "chloe", "david"};
```

Écris `int chercher_nom(const char *noms[], int n, const char *cible)`.

*Piège :* `==` compare des adresses, pas des contenus. Utilise `strcmp`.

---

# Niveau 2 — Recherche dichotomique

## 2.1 — La version itérative

Écris-la de mémoire, sans relire la leçon.

Puis compare ton code au sien sur trois points : le calcul du milieu, la
condition du `while`, les `+1`/`-1`.

## 2.2 — Compter les tours

Ajoute un compteur de tours de boucle. Sur un tableau de 1 000 000 d'éléments,
cherche une valeur absente.

Combien de tours ? Compare à `log₂(1000000) ≈ 20`.

## 2.3 — Le bug du `<`

Remplace `while (gauche <= droite)` par `while (gauche < droite)`.

Trouve une valeur que le code ne trouve plus. Explique précisément pourquoi.

Remets le `<=`.

## 2.4 — Le bug du milieu

Écris `int milieu = (gauche + droite) / 2;`.

Sur un tableau normal, aucun effet. Pour provoquer le bug sans allouer 8 Go :
force `gauche = 2000000000` et `droite = 2000000001`, affiche le milieu calculé
des deux façons.

L'une donne un nombre négatif. Que se passerait-il ensuite dans le code ?

## 2.5 — La version récursive

Écris `dicho_rec`. Ajoute un compteur de profondeur.

Sur 1 000 000 d'éléments, quelle profondeur maximale ? Compare à la version
itérative en espace mémoire.

## 2.6 — Dichotomie sur un tableau non trié

Prends un tableau non trié et lance la dichotomie sur une valeur qui **est**
présente.

Note ce qui se passe. Le programme plante-t-il ? Que renvoie-t-il ?

C'est le cas le plus dangereux de tout ce cours : un résultat faux sans aucun
signal d'erreur. Écris en une phrase pourquoi c'est pire qu'un crash.

---

# Niveau 3 — Les variantes

## 3.1 — Première occurrence

Sur `[1, 3, 3, 3, 5, 7]`, la dichotomie de base peut renvoyer l'indice 1, 2 ou 3
pour la cible `3`.

Écris `premiere_occurrence` qui renvoie toujours `1`.

## 3.2 — Dernière occurrence

Même chose, mais elle renvoie `3`.

## 3.3 — Compter en O(log n)

Combien de fois `3` apparaît-il ? Déduis-le des deux fonctions précédentes,
**sans parcourir** le tableau.

Vérifie sur un tableau de 1 000 000 d'éléments contenant 500 000 fois la même
valeur. Compte les comparaisons : tu dois être autour de 40, pas de 500 000.

## 3.4 — Point d'insertion

Écris `point_insertion(tab, n, valeur)` avec la convention d'intervalle
semi-ouvert.

Teste avec une valeur plus petite que tout (doit renvoyer `0`), plus grande que
tout (doit renvoyer `n`), et déjà présente.

## 3.5 — Tri par insertion dichotomique

Utilise `point_insertion` pour écrire un tri par insertion qui trouve la position
par dichotomie au lieu de la chercher linéairement.

Compte les **comparaisons** et les **décalages** séparément.

Question importante : le nombre de comparaisons chute. Le tri est-il pour autant
plus rapide ? Mesure, et explique le résultat.

## 3.6 — Élément qui vaut son indice

Dans un tableau trié **sans doublons**, trouve un `i` tel que `tab[i] == i`.

Solution en O(log n). *Indice :* si `tab[i] > i`, de quel côté peut se trouver la
réponse ?

---

# Niveau 4 — Dichotomie sur une réponse

C'est le niveau qui compte le plus. Aucun de ces exercices ne cherche dans un
tableau.

## 4.1 — Racine carrée entière

`racine_entiere(n)` renvoie le plus grand `x` tel que `x² ≤ n`.

Teste avec `16` (→ 4), `17` (→ 4), `2` (→ 1), `1000000` (→ 1000).

*Piège :* `milieu * milieu` déborde pour de grands `n`. Utilise `long`.

## 4.2 — Racine cubique entière

Même principe.

## 4.3 — Distribuer des bonbons

`n` paquets contenant `bonbons[i]` bonbons chacun, `k` enfants. Chaque enfant
reçoit le même nombre de bonbons, tous issus d'un seul paquet (on peut couper un
paquet, on ne peut pas fusionner deux paquets).

Quel est le nombre maximal de bonbons par enfant ?

*Méthode :* est-il possible de donner `x` bonbons à chacun ? Écris cette fonction
de test, puis dichotomise sur `x`.

C'est le schéma général : **une fonction de faisabilité monotone, puis une
dichotomie dessus**.

## 4.4 — Le plus petit diviseur

Étant donné un tableau et une limite `L`, trouve le plus petit entier `d` tel que
la somme des `ceil(tab[i] / d)` soit ≤ `L`.

Même méthode.

## 4.5 — Chercher dans un tableau trié et pivoté

`[15, 18, 2, 3, 6, 12]` — un tableau trié, coupé et recollé.

Trouve une valeur en O(log n).

*Indice :* à chaque étape, **au moins une des deux moitiés est triée**. Détermine
laquelle, puis regarde si la cible y est.

C'est l'exercice de dichotomie le plus demandé en entretien. Prends le temps.

---

# Niveau 5 — Mise en situation

## 5.1 — Le duel

Sur un tableau trié de 10 000 000 d'entiers, lance 1 000 recherches de valeurs
aléatoires avec chaque méthode. Chronomètre les deux totaux.

Note le rapport.

## 5.2 — Trier d'abord, ça vaut le coup ?

Tu as un tableau **non trié** de 1 000 000 d'éléments et `k` recherches à faire.

- Option A : `k` recherches linéaires.
- Option B : un `qsort`, puis `k` dichotomies.

Trouve expérimentalement le `k` de bascule.

Puis retrouve-le par le calcul : résous `k × n = n log n + k × log n`.

Les deux valeurs concordent-elles ? Si non, quelle hypothèse du calcul est
fausse ?

## 5.3 — Un dictionnaire

Charge un fichier de mots, un par ligne (`/usr/share/dict/words` sur macOS et
Linux), dans un tableau. Trie-le. Écris une boucle interactive qui demande un mot
et dit s'il existe.

Affiche le nombre de comparaisons de chaque recherche. Sur ~235 000 mots, tu
devrais tourner autour de 18.

---

# Auto-évaluation

- [ ] Pourquoi la dichotomie exige-t-elle un tableau trié ?
- [ ] Que se passe-t-il si le tableau ne l'est pas ?
- [ ] Pourquoi `gauche + (droite - gauche) / 2` plutôt que `(gauche + droite) / 2` ?
- [ ] Pourquoi `<=` et non `<` dans la boucle ?
- [ ] Quand la recherche linéaire est-elle le meilleur choix ?
- [ ] Qu'est-ce qui rend un problème « dichotomisable » sans tableau ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Relire la leçon** — chaque exercice pointe une section.
2. **Dérouler à la main** sur un tableau de 6 éléments, en écrivant `gauche`,
   `droite` et `milieu` à chaque tour. Pour la dichotomie, c'est la méthode la
   plus efficace : les bugs sautent aux yeux sur papier.
3. **`lldb`** avec un point d'arrêt dans la boucle, et `p gauche`, `p droite`.

Un exercice qui résiste plus de trente minutes mérite une question.
