# Exercices — Listes chaînées

Support : [06_listes_chainees.md](../lessons/06_listes_chainees.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

**Règle supplémentaire pour cette leçon :** tout programme doit passer sans fuite.

```sh
leaks --atExit -- ./codes/liste                 # macOS
valgrind --leak-check=full ./codes/liste        # Linux
```

Un programme qui fuit n'est pas terminé, même s'il affiche le bon résultat.

---

# Niveau 1 — Les bases

## 1.1 — Construire à la main

Sans aucune fonction : trois `malloc`, trois affectations de valeur, deux
chaînages, puis un parcours qui affiche.

```
12 -> 5 -> 38 -> NULL
```

Puis trois `free`. Vérifie l'absence de fuite.

C'est le seul exercice où tu écris tout en dur. Il sert à voir la structure avant
de l'abstraire.

## 1.2 — Insertion en tête

`Noeud *inserer_tete(Noeud *tete, int valeur)`.

Construis une liste de 5 éléments en insérant `1, 2, 3, 4, 5` dans cet ordre.

Dans quel ordre s'affichent-ils ? Explique.

## 1.3 — Afficher

`void afficher(const Noeud *tete)` au format `12 -> 5 -> 38 -> NULL`.

Le `const` doit compiler. S'il ne compile pas, ta fonction modifie quelque
chose — trouve quoi.

## 1.4 — Longueur

Itérative, puis récursive.

Sur une liste de 500 000 nœuds, la récursive doit s'écraser. Confirme, et
explique.

## 1.5 — Insertion en queue

Version O(n) qui parcourt. Puis version O(1) avec une structure qui garde les
deux pointeurs :

```c
typedef struct {
    Noeud *tete;
    Noeud *queue;
    int taille;
} Liste;
```

Chronomètre 100 000 insertions en queue avec chacune. Note le rapport.

## 1.6 — Libérer

`void liberer_liste(Noeud *tete)`.

Écris d'abord la version **fausse** :

```c
while (tete != NULL) {
    free(tete);
    tete = tete->suivant;      // lecture après libération
}
```

Compile avec `-fsanitize=address` et lance. Lis le rapport en entier.

Puis corrige.

---

# Niveau 2 — Manipulations

## 2.1 — Rechercher

`Noeud *rechercher(Noeud *tete, int valeur)` — renvoie le nœud ou `NULL`.

## 2.2 — Supprimer la première occurrence

Gère les trois cas : la tête, un nœud du milieu, une valeur absente.

## 2.3 — Supprimer toutes les occurrences

En un seul parcours.

## 2.4 — Insérer à une position

`inserer_position(tete, valeur, pos)`. Position `0` = en tête.

Que fais-tu si `pos` dépasse la longueur ? Décide, documente en commentaire, et
sois cohérent.

## 2.5 — Insérer en gardant le tri

La liste est triée croissante ; insère la valeur à sa place.

*Piège :* il faut le nœud **précédent** pour recâbler. Trois cas : avant la tête,
au milieu, après la queue.

## 2.6 — Le pointeur double

Réécris `supprimer` avec `Noeud **`.

Compare les deux versions : combien de cas particuliers dans chacune ?

Si l'indirection double ne te paraît pas claire, dessine sur papier ce que
contient `courant`, ce que contient `*courant`, et ce que contient
`(*courant)->suivant`.

---

# Niveau 3 — Les classiques

Ce sont les exercices de liste chaînée qu'on rencontre partout. Chacun a une
idée à trouver.

## 3.1 — Inverser la liste

En place, en O(n), sans allouer.

*Indice :* trois pointeurs — `precedent`, `courant`, `suivant`. À chaque tour, on
retourne un lien.

Écris la version itérative. Puis la récursive.

## 3.2 — Le milieu en un seul passage

Trouve le nœud du milieu **sans compter d'abord la longueur**.

*Indice :* deux curseurs, l'un avançant deux fois plus vite que l'autre. Quand le
rapide atteint la fin, où est le lent ?

C'est la technique du « lièvre et de la tortue ». Elle resservira.

## 3.3 — Le k-ième depuis la fin

En un seul passage, sans connaître la longueur.

*Indice :* deux curseurs, espacés de `k`.

## 3.4 — Détecter un cycle

Crée volontairement un cycle : fais pointer le dernier nœud sur le troisième.

Écris `bool a_un_cycle(const Noeud *tete)`.

Naïvement, il faudrait mémoriser les nœuds visités — O(n) en espace. L'algorithme
de Floyd le fait en **O(1)** : lièvre et tortue, et s'ils se rencontrent, il y a
un cycle.

Écris-le. Puis explique en une phrase pourquoi ils se rencontrent forcément si un
cycle existe.

*Attention :* n'appelle pas `afficher` ni `liberer_liste` sur cette liste.

## 3.5 — L'entrée du cycle

Une fois le cycle détecté, trouve le nœud où il commence.

*Indice :* remets un curseur à la tête, avance les deux d'un pas à la fois. Ils
se rencontrent à l'entrée du cycle.

Le résultat paraît magique. Il se démontre en écrivant les distances parcourues.
Essaie, ou admets-le.

## 3.6 — Fusionner deux listes triées

Deux listes triées → une seule liste triée. **Sans allouer de nouveaux nœuds** :
on recâble les existants.

C'est la fusion du tri fusion, sur pointeurs.

## 3.7 — Trier une liste chaînée

Tri fusion sur liste : couper au milieu (exercice 3.2), trier récursivement,
fusionner (exercice 3.6).

Complexité en temps ? En espace ?

Compare à ce que coûterait un tri fusion sur tableau. C'est ici que la liste
chaînée gagne : pas de tampon.

## 3.8 — Supprimer les doublons

Sur une liste **triée**, en un passage.

Puis sur une liste **non triée**. Quelle complexité ? Peux-tu faire mieux qu'en
O(n²) ? (Repense à la leçon 9 — tu pourras y revenir plus tard.)

---

# Niveau 4 — Variantes de structure

## 4.1 — Liste doublement chaînée

Ajoute `precedent`. Réécris : insertion en tête, en queue, suppression,
affichage avant et arrière.

*Piège :* à chaque opération, **deux** liens à mettre à jour dans chaque sens.
Une seule omission et la liste devient incohérente dans un sens seulement — un
bug qui ne se voit qu'en parcourant à l'envers. Teste toujours les deux sens.

## 4.2 — Suppression en O(1)

Avec une liste doublement chaînée, écris `supprimer_noeud(Liste *l, NoeudD *n)`
qui prend directement le nœud.

Complexité ? Compare à la simple chaînée.

## 4.3 — Liste circulaire

Le dernier nœud pointe sur le premier.

Réécris l'affichage : la condition d'arrêt n'est plus `NULL`.

## 4.4 — Le problème de Josèphe

`n` personnes en cercle. On élimine une personne sur `k`, en tournant. Qui reste ?

Une liste circulaire rend la solution presque immédiate.

Teste : `n = 7, k = 3` → la personne 4 survit.

## 4.5 — Une liste générique

```c
typedef struct Noeud {
    void *donnee;
    struct Noeud *suivant;
} Noeud;
```

Adapte insertion, recherche (avec une fonction de comparaison passée en
paramètre) et libération (avec une fonction de destruction).

Teste avec des `int`, puis avec des `char *`.

Question à trancher : la liste possède-t-elle les données, ou seulement les
pointeurs ? Décide, documente, et applique-le à `liberer_liste`.

---

# Niveau 5 — Confrontation

## 5.1 — Insertion en tête

Chronomètre 100 000 insertions en tête dans un tableau (avec décalage) et dans
une liste chaînée.

Le rapport doit être spectaculaire. Explique.

## 5.2 — Accès indexé

Chronomètre 100 000 accès à un indice aléatoire dans un tableau et dans une liste
de 100 000 éléments.

Rapport inverse. Explique.

## 5.3 — Le parcours, et le cache

Somme des éléments d'un tableau de 10 000 000 d'entiers, puis d'une liste de
10 000 000 de nœuds.

Les deux sont O(n). Note le rapport de temps.

Puis refais le test après avoir alloué les nœuds de la liste **dans l'ordre**
(un seul gros `malloc` découpé en nœuds contigus). Le rapport change-t-il ?

C'est la démonstration que le cache, absent de Big-O, décide en pratique.

## 5.4 — La mémoire

Pour 1 000 000 d'entiers, calcule :

- la mémoire d'un tableau
- la mémoire d'une liste simplement chaînée
- la mémoire d'une liste doublement chaînée

*Indice :* `sizeof(Noeud)` n'est pas `sizeof(int) + sizeof(void*)`. Affiche-le et
explique l'écart — c'est l'alignement.

---

# Auto-évaluation

- [ ] Pourquoi `struct Noeud *suivant` et non `Noeud *suivant` dans la définition ?
- [ ] Pourquoi l'insertion en tête est-elle O(1) et l'accès indexé O(n) ?
- [ ] Pourquoi ne peut-on pas faire de recherche dichotomique sur une liste triée ?
- [ ] Que se passe-t-il si on lit `noeud->suivant` après `free(noeud)` ?
- [ ] Pourquoi l'ordre des conditions du `&&` est-il significatif ?
- [ ] À quoi sert le pointeur double dans `supprimer` ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Dessiner les pointeurs sur papier**, avant et après l'opération. Sur les
   listes chaînées, c'est le seul débogage qui fonctionne vraiment.
2. **`afficher` après chaque opération**, systématiquement.
3. **`-fsanitize=address`** dès qu'un comportement est bizarre. Il transforme un
   plantage mystérieux en un message précis avec numéro de ligne.
