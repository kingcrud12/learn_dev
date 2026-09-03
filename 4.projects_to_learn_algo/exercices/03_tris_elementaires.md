# Exercices — Tris élémentaires

Support : [03_tris_elementaires.md](../lessons/03_tris_elementaires.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

**Règle du parcours :** aucun avertissement toléré.

---

# Niveau 0 — L'outillage

Ces trois fonctions serviront dans tous les exercices suivants. Écris-les
d'abord, dans un fichier que tu réutiliseras.

## 0.1 — Afficher

`void afficher(const int tab[], int n)` — sur une ligne, séparé par des espaces.

## 0.2 — Vérifier

`bool est_trie(const int tab[], int n)` — un tri qu'on ne vérifie pas n'est pas
testé.

## 0.3 — Générer

Trois générateurs, tous de taille `n` :

- `remplir_aleatoire(tab, n)`
- `remplir_trie(tab, n)` — croissant
- `remplir_inverse(tab, n)` — décroissant

Pour l'aléatoire, `srand(42)` en début de programme : la graine fixe rend tes
mesures **reproductibles**. C'est ce qui permet de comparer deux exécutions.

---

# Niveau 1 — Écrire les trois tris

## 1.1 — Tri par sélection

Écris-le. Affiche le tableau **après chaque tour** de la boucle externe, sur un
tableau de 8 éléments.

Vérifie visuellement l'invariant : la partie gauche grandit d'une case à chaque
ligne, et elle est triée.

## 1.2 — Tri par insertion

Même chose. Affiche après chaque insertion.

Observe la différence de comportement : en sélection, la partie triée est
**définitive** ; en insertion, elle est triée mais ses éléments peuvent encore
se décaler.

## 1.3 — Tri à bulles

Avec le drapeau `echange`. Affiche le nombre de passes réellement effectuées.

Lance-le sur un tableau déjà trié : il doit faire **une seule** passe.

## 1.4 — Sans le drapeau

Retire le drapeau. Relance sur le tableau trié.

Combien de passes maintenant ? Quelle complexité pour le meilleur cas dans chaque
version ?

## 1.5 — Le bug de l'ordre du `&&`

Dans le tri par insertion, écris `while (tab[j] > cle && j >= 0)`.

Compile, lance avec un tableau dont le minimum est en dernière position.

Le programme plante-t-il ? Si non, lance-le avec :

```sh
gcc -Wall -Wextra -g -fsanitize=address codes/tri.c -o codes/tri && ./codes/tri
```

Lis le rapport d'AddressSanitizer. Il te dira exactement ce que le programme
faisait sans le dire.

---

# Niveau 2 — Instrumenter

## 2.1 — Deux compteurs

Ajoute à chaque tri un compteur de **comparaisons** et un compteur
d'**écritures** (toute affectation dans le tableau).

Passe-les par pointeur, ou utilise une structure `Stats`.

## 2.2 — La matrice 3 × 3

Lance les trois tris sur les trois types de données, avec `n = 2000`.

Remplis ce tableau :

```
                 Aléatoire        Trié           Inversé
                 comp / écr    comp / écr      comp / écr
Sélection        ____ / ____   ____ / ____     ____ / ____
Insertion        ____ / ____   ____ / ____     ____ / ____
Bulles           ____ / ____   ____ / ____     ____ / ____
```

## 2.3 — Trois lectures

Réponds à partir de ta matrice :

- Pourquoi la ligne « Sélection » est-elle presque identique dans les trois colonnes ?
- Pourquoi l'insertion sur données triées fait-elle ~2 000 comparaisons et non
  ~2 000 000 ?
- Pourquoi les bulles font-elles plus d'écritures que l'insertion, à comparaisons
  comparables ?

## 2.4 — Vérifier la formule

Le tri par sélection fait exactement `n(n-1)/2` comparaisons.

Pour `n = 2000` : `2000 × 1999 / 2 = 1 999 000`.

Ton compteur donne-t-il exactement cette valeur ? S'il diffère, ton compteur est
mal placé — trouve où.

## 2.5 — Chronométrer

Ajoute un chronomètre. Lance les trois tris sur des données aléatoires avec
`n = 1000, 2000, 4000, 8000`.

Le temps est-il multiplié par 4 quand `n` double ? Pour les trois ?

---

# Niveau 3 — La stabilité

## 3.1 — Voir l'instabilité

Trie un tableau de structures :

```c
typedef struct {
    int note;
    char nom[20];
} Eleve;
```

Six élèves, avec des notes qui se répètent :

```
{15,"alice"} {12,"bob"} {15,"chloe"} {10,"david"} {12,"emma"} {15,"felix"}
```

Trie par note avec chacun des trois tris. Affiche les résultats.

Chez les élèves à 15, l'ordre `alice, chloe, felix` est-il préservé ? Réponds
tri par tri.

## 3.2 — Casser la stabilité de l'insertion

Dans le tri par insertion, remplace `while (j >= 0 && tab[j] > cle)` par `>=`.

Relance le test 3.1. Le tri est-il encore stable ?

Une seule touche de clavier. Explique le mécanisme.

## 3.3 — Tri à deux critères

Trie les élèves par note **décroissante**, et à note égale, par nom
**alphabétique**.

Fais-le en deux passes, en exploitant la stabilité : d'abord par nom, puis par
note.

Si tu utilises un tri instable pour la seconde passe, le résultat sera faux.
Vérifie-le en essayant avec la sélection.

C'est l'utilité concrète de la stabilité, et la raison pour laquelle on s'en
préoccupe.

---

# Niveau 4 — Variantes et adaptations

## 4.1 — Tri décroissant

Modifie les trois tris pour trier en ordre décroissant. Une seule comparaison
change dans chacun.

## 4.2 — Trier des chaînes

Adapte le tri par insertion à un `char *tab[]`, avec `strcmp`.

*Piège :* tu déplaces des pointeurs, pas des chaînes. Ne fais aucun `strcpy`.

## 4.3 — Tri générique

Écris un tri par insertion qui prend une fonction de comparaison :

```c
void tri_insertion_generique(void *base, size_t n, size_t taille,
                             int (*compare)(const void *, const void *));
```

C'est la signature de `qsort`. Tu manipuleras des `char *` et `memcpy` pour
déplacer des éléments de taille inconnue.

Difficile. Prends-le comme un exercice de pointeurs autant que d'algorithmique.

## 4.4 — Tri cocktail

Variante des bulles : on alterne les passes gauche→droite et droite→gauche.

Écris-le. Compte les passes sur un tableau où le plus petit élément est en
dernière position.

Compare aux bulles classiques sur ce cas précis. Le gain a un nom : ce cas
s'appelle une « tortue ».

## 4.5 — Tri gnome

```
i = 0
tant que i < n :
    si i == 0 ou tab[i-1] <= tab[i] : i++
    sinon : échanger tab[i] et tab[i-1] ; i--
```

Écris-le. Six lignes.

Compare son nombre d'opérations à celui du tri par insertion sur les mêmes
données. Que constates-tu ? Que peux-tu en déduire sur la parenté des deux
algorithmes ?

---

# Niveau 5 — Le seuil

## 5.1 — Trouver le point de bascule

Utilise `qsort` de la bibliothèque standard comme référence « tri rapide ».

Pour `n = 4, 8, 16, 32, 64, 128`, chronomètre 100 000 tris de tableaux
aléatoires, avec `qsort` et avec ton tri par insertion.

*Note :* pour de si petits `n`, un seul tri est trop rapide pour être mesurable.
D'où les 100 000 répétitions.

## 5.2 — La conclusion

À partir de quel `n` `qsort` devient-il plus rapide ?

Explique le résultat : `qsort` est O(n log n), l'insertion O(n²). Pourquoi
l'insertion gagne-t-elle en dessous du seuil ?

Cite au moins deux causes.

## 5.3 — Presque trié

Génère un tableau trié, puis échange 1 % des éléments au hasard.

Chronomètre l'insertion dessus, puis sur un tableau totalement aléatoire de même
taille.

Le rapport que tu obtiens est la valeur de l'adaptativité, mesurée.

---

# Auto-évaluation

- [ ] Pourquoi le tri par sélection a-t-il la même complexité dans tous les cas ?
- [ ] Pourquoi le tri par insertion est-il O(n) sur des données déjà triées ?
- [ ] Qu'est-ce qu'un tri stable, et quand cela change-t-il quelque chose ?
- [ ] Pourquoi l'insertion bat-elle les bulles alors que les deux sont O(n²) ?
- [ ] Pourquoi une implémentation de `qsort` appelle-t-elle un tri par insertion ?
- [ ] Quel est l'invariant de boucle du tri par sélection ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Afficher le tableau à chaque tour.** Sur un tri, c'est le débogage le plus
   efficace : l'erreur se voit.
2. **Relire la leçon** — chaque exercice pointe une section.
3. **`lldb`** avec un point d'arrêt dans la boucle interne.

Un tri qui « marche presque » a presque toujours un problème de bornes de boucle.
Vérifie `n - 1` contre `n`, et `i + 1` contre `i`.
