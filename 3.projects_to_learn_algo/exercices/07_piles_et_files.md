# Exercices — Piles et files

Support : [07_piles_et_files.md](../lessons/07_piles_et_files.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

Vérifie l'absence de fuite sur toutes les versions à base de liste chaînée.

---

# Niveau 1 — La pile

## 1.1 — Sur tableau

Implémente `Pile` avec `init`, `est_vide`, `est_pleine`, `push`, `pop`, `peek`.

Teste : `push` sur pile pleine, `pop` sur pile vide. Les deux doivent renvoyer
`false`, pas planter.

## 1.2 — Le décalage d'une case

Change la convention : `sommet = 0` signifie « pile vide », et `sommet` désigne
la prochaine case libre.

Réécris `push` et `pop` en conséquence.

Puis, volontairement, mélange les deux conventions : garde `sommet = -1` à
l'init mais écris `donnees[sommet++]` dans `push`.

Que se passe-t-il ? C'est le bug le plus courant sur cette structure — il vaut
la peine de l'avoir vu une fois.

## 1.3 — Sur liste chaînée

Même interface, implémentation par liste.

Compare : quelles opérations changent de complexité ? Aucune. Qu'est-ce qui
change alors ?

## 1.4 — Pile dynamique

Sur tableau, mais qui **double sa capacité** au lieu de refuser quand elle est
pleine.

Compte le nombre total de réallocations pour 1 000 000 de `push`. Environ 20.
Pourquoi ce nombre ?

## 1.5 — Inverser avec une pile

Inverse une chaîne de caractères en utilisant uniquement une pile.

Puis inverse un tableau. Puis une liste chaînée.

## 1.6 — Pile générique

`void *` et une taille d'élément. Teste avec des `int`, des `double`, et une
`struct`.

---

# Niveau 2 — Les applications de la pile

## 2.1 — Parenthèses équilibrées

Trois types : `()`, `[]`, `{}`.

Teste : `"({[]})"` vrai, `"([)]"` faux, `"((("` faux, `")"` faux, `""` vrai,
`"a(b)c[d]"` vrai.

Le dernier cas vérifie que tu ignores bien les autres caractères.

## 2.2 — Évaluer une expression postfixée

`"3 4 + 2 *"` vaut 14.

```
pour chaque jeton :
    si nombre : empiler
    si opérateur : dépiler deux fois, calculer, empiler le résultat
résultat = sommet de pile
```

Teste avec `"5 1 2 + 4 * + 3 -"` → 14.

Gère les erreurs : pile trop courte, division par zéro, pile non vide à la fin.

## 2.3 — Infixe vers postfixe

`"3 + 4 * 2"` → `"3 4 2 * +"`.

C'est l'algorithme du **train de wagons** (*shunting-yard*) de Dijkstra. Il
demande une table de priorité des opérateurs.

Gère les parenthèses.

## 2.4 — Une calculatrice complète

Combine 2.3 et 2.2 : lis une expression infixe au clavier, convertis, évalue,
affiche.

C'est le petit projet de cette leçon. Prends le temps de le finir.

## 2.5 — Annuler / rétablir

Deux piles. Un éditeur de texte minimal : ajouter un caractère, `undo`, `redo`.

*Question clé :* que doit-il arriver à la pile `redo` quand l'utilisateur fait
une **nouvelle** action après un `undo` ? Réponds avant de coder, puis vérifie
le comportement de ton éditeur habituel.

## 2.6 — Vérifier du HTML

`"<a><b></b></a>"` est valide, `"<a><b></a></b>"` non.

Même principe que les parenthèses, avec des balises nommées.

---

# Niveau 3 — La file

## 3.1 — La version naïve

Implémente la file avec décalage à chaque `dequeue`. Elle est fausse par
principe, mais elle sert de référence.

## 3.2 — La file circulaire

Implémente-la correctement, avec le champ `taille`.

## 3.3 — Observer le tour

Capacité 5. Affiche `tete`, `queue` et `taille` après chaque opération.

Séquence : 5 `enqueue`, 3 `dequeue`, 3 `enqueue`.

Vérifie que `queue` repasse bien par 0. Dessine l'état du tableau à la fin.

## 3.4 — Le duel

Chronomètre 100 000 `enqueue` suivis de 100 000 `dequeue`, avec la version naïve
puis la circulaire.

Note le rapport. C'est O(n) contre O(1), mesuré.

## 3.5 — Sans le champ taille

Réimplémente en sacrifiant une case : la file est pleine quand
`(queue + 1) % CAPACITE == tete`.

Vérifie que tu ne peux stocker que `CAPACITE - 1` éléments.

Laquelle des deux versions préfères-tu, et pourquoi ?

## 3.6 — File sur liste chaînée

Avec un pointeur de tête **et** un pointeur de queue, pour que les deux
opérations soient O(1).

Attention au cas où la file devient vide : les deux pointeurs doivent redevenir
`NULL`. C'est le cas qu'on oublie.

---

# Niveau 4 — Les combinaisons

## 4.1 — Une file avec deux piles

C'est un classique. Une pile « entrée », une pile « sortie ».

- `enqueue` : `push` sur l'entrée.
- `dequeue` : si la sortie est vide, transvaser toute l'entrée dedans (ce qui
  inverse l'ordre), puis `pop` sur la sortie.

Chaque élément est transvasé au plus une fois. La complexité **amortie** de
`dequeue` est donc O(1), même si une opération particulière peut être O(n).

Vérifie-le : compte le nombre total de transferts pour 100 000 opérations, et
divise.

## 4.2 — Une pile avec deux files

L'inverse, et il est moins élégant. L'une des deux opérations sera forcément
O(n).

Laquelle choisis-tu de dégrader, et pourquoi ?

## 4.3 — Une pile avec minimum en O(1)

`push`, `pop`, et `minimum()` — toutes les trois en O(1).

*Indice :* une seconde pile qui suit les minimums successifs.

Teste : `push(5), push(3), push(7), min()` → 3, `pop(), pop(), min()` → 5.

## 4.4 — Deque

Insertion et suppression aux deux extrémités, toutes en O(1).

Sur tableau circulaire : `tete` doit pouvoir **reculer**. Attention au modulo sur
un nombre négatif en C — `-1 % 5` vaut `-1`, pas `4`.

Trouve la bonne formule.

## 4.5 — File de priorité, version simple

`enqueue(valeur, priorite)`, et `dequeue` renvoie l'élément de plus haute
priorité.

Version naïve : liste triée à l'insertion. `enqueue` en O(n), `dequeue` en O(1).

Autre version : liste non triée. `enqueue` en O(1), `dequeue` en O(n).

Laquelle choisir ? Cela dépend du rapport entre le nombre d'insertions et de
retraits. Explique.

*Pour info :* un tas binaire donne O(log n) pour les deux. C'est la structure
utilisée en pratique.

## 4.6 — Le maximum glissant

Étant donné un tableau et une fenêtre de taille `k`, affiche le maximum de chaque
fenêtre.

La version naïve est O(n × k). Avec un **deque** qui maintient les indices des
candidats, on descend à **O(n)**.

C'est l'exercice le plus difficile de cette leçon. L'idée : le deque contient les
indices des éléments par valeur décroissante, et on élimine par les deux bouts.

---

# Niveau 5 — En situation

## 5.1 — Un tampon circulaire

Simule un producteur et un consommateur : le producteur ajoute des octets, le
consommateur en retire, à des rythmes différents.

Affiche le taux de remplissage. Que se passe-t-il quand le producteur est plus
rapide ? Quand c'est le consommateur ?

C'est exactement le mécanisme d'un tampon d'entrée/sortie.

## 5.2 — Un ordonnanceur

Trois processus, chacun avec un temps d'exécution. Un ordonnancement en tourniquet
(*round-robin*) : chacun s'exécute pendant un quantum, puis retourne en fin de file.

Affiche l'ordre d'exécution et le temps d'attente moyen.

Fais varier le quantum. Que se passe-t-il quand il est très grand ? Très petit ?

## 5.3 — Parcours d'un arbre en largeur

Reprends un arbre binaire (leçon 8, ou construis-en un à la main) et affiche-le
niveau par niveau avec une file.

Puis affiche chaque niveau **sur sa propre ligne**.

*Indice :* avant chaque niveau, note la taille courante de la file.

---

# Auto-évaluation

- [ ] Quelle est la différence entre LIFO et FIFO, en une phrase ?
- [ ] Pourquoi un `dequeue` par décalage est-il une erreur d'implémentation ?
- [ ] À quoi sert le modulo dans une file circulaire ?
- [ ] Pourquoi faut-il un champ `taille` (ou sacrifier une case) ?
- [ ] Pourquoi une pile est-elle naturellement une liste chaînée réduite à sa tête ?
- [ ] Quelle structure pour un DFS ? Pour un BFS ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Afficher l'état complet** (indices, taille, contenu) après chaque opération.
   Sur une file circulaire, c'est indispensable.
2. **Tester d'abord les cas limites** : vide, plein, un seul élément. C'est là
   que sont les bugs, pas dans le cas général.
3. **Dessiner le tableau circulaire** sur papier, avec les deux indices.
