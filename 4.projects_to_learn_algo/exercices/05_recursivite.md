# Exercices — Récursivité

Support : [05_recursivite.md](../lessons/05_recursivite.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

---

# Niveau 1 — Les classiques

Pour chacun : écris la version récursive **et** la version itérative, puis dis
laquelle tu préfères et pourquoi.

## 1.1 — Factorielle

`n! = n × (n-1)!`, avec `0! = 1`.

À partir de quelle valeur de `n` le résultat déborde-t-il un `int` ? Un
`long long` ? Vérifie.

## 1.2 — Somme de 1 à n

Récursive, itérative — et la formule `n(n+1)/2`, qui est O(1).

Trois complexités pour un même résultat. Note-les.

## 1.3 — Puissance

`x^n`. Version naïve en O(n).

Puis l'**exponentiation rapide** en O(log n) :

```
x^n = (x^(n/2))²           si n est pair
x^n = x × (x^(n-1))        si n est impair
```

Compte les multiplications de chaque version pour `n = 1000`. Environ 1 000
contre environ 10.

## 1.4 — Inverser une chaîne

Récursivement, en place. *Indice :* échange le premier et le dernier, puis
récurse sur l'intérieur.

## 1.5 — Palindrome

`est_palindrome(chaine)` récursif. Même schéma que 1.4.

Teste avec `"radar"`, `"abc"`, `"a"`, `""`.

## 1.6 — PGCD d'Euclide

```
pgcd(a, b) = a              si b == 0
pgcd(a, b) = pgcd(b, a % b) sinon
```

Deux lignes. Compte les appels pour `pgcd(1071, 462)`.

Cette récursion est-elle terminale ? Justifie.

## 1.7 — Compter les chiffres

Combien de chiffres dans un entier ? Récursivement.

*Piège :* `0` a un chiffre. Ton cas de base le gère-t-il ?

## 1.8 — Somme des chiffres

`1234 → 1+2+3+4 = 10`. Récursivement.

---

# Niveau 2 — La pile

## 2.1 — Mesurer sa profondeur

```c
int profondeur(int n) {
    printf("%d\n", n);
    return profondeur(n + 1);
}
```

Lance-le, redirige la sortie : `./codes/pile > /tmp/out.txt`.

Le dernier nombre du fichier est le nombre de cadres que ta pile a contenus.

## 2.2 — Calculer la taille d'un cadre

```sh
ulimit -s
```

Divise cette valeur (en octets) par le nombre trouvé en 2.1.

Combien d'octets par cadre ? Est-ce cohérent avec ce que contient la fonction ?

## 2.3 — Faire grossir le cadre

Ajoute une variable locale `char tampon[1000];` dans la fonction (et utilise-la,
sinon le compilateur l'élimine).

Relance. La profondeur atteinte doit chuter d'un facteur ~20. Vérifie.

## 2.4 — Changer la limite

```sh
ulimit -s 16384        # 16 Mo, dans le shell courant
```

Relance 2.1. La profondeur double-t-elle ?

## 2.5 — Récursion terminale, en vrai

Écris `factorielle_terminale(n, accumulateur)`.

Compile en `-O0` puis en `-O2`, et regarde l'assembleur :

```sh
gcc -O0 -S codes/fact.c -o /tmp/o0.s
gcc -O2 -S codes/fact.c -o /tmp/o2.s
grep -c "call" /tmp/o0.s /tmp/o2.s
```

L'optimisation a-t-elle eu lieu ? Vérifie ensuite en pratique : la version `-O2`
survit-elle à `factorielle_terminale(10000000, 1)` ?

---

# Niveau 3 — Fibonacci et la mémoïsation

## 3.1 — La version naïve

Écris `fibo(n)`. Vérifie : `fibo(10) = 55`.

## 3.2 — Compter les appels

Ajoute un compteur global. Combien d'appels pour `fibo(10)` ? `fibo(20)` ?
`fibo(30)` ?

Le rapport entre deux valeurs consécutives approche `1,618` — le nombre d'or.
Vérifie-le.

## 3.3 — Chronométrer

Mesure `fibo(30)`, `fibo(35)`, `fibo(40)`.

Extrapole `fibo(50)` et `fibo(60)`. Ne les lance pas.

## 3.4 — Mémoïser

Ajoute un cache. Initialise-le à `-1`.

Relance `fibo(60)`, `fibo(80)`, `fibo(90)`. Instantané.

*Piège :* `fibo(93)` déborde un `unsigned long long`. Trouve la valeur exacte à
partir de laquelle ça arrive.

## 3.5 — La version itérative

Deux variables, une boucle. O(n) en temps, O(1) en espace.

Compare les trois versions sur `fibo(90)` : temps et mémoire.

## 3.6 — Les tours de Hanoï

Déplacer `n` disques d'une tour A à une tour C, avec une tour B intermédiaire.
Jamais un grand disque sur un petit.

```
hanoi(n, depart, arrivee, intermediaire) :
    si n == 0 : retour
    hanoi(n-1, depart, intermediaire, arrivee)
    afficher "déplacer disque n de depart vers arrivee"
    hanoi(n-1, intermediaire, arrivee, depart)
```

Affiche les mouvements pour `n = 3`. Compte-les pour `n = 1..20`.

Le nombre de mouvements est `2ⁿ - 1`. Vérifie.

Contrairement à Fibonacci, cette complexité exponentielle est **irréductible** —
aucune mémoïsation ne peut aider. Explique la différence entre les deux cas.

---

# Niveau 4 — Récursion sur les structures

## 4.1 — Somme d'un tableau

`somme(tab, n) = tab[n-1] + somme(tab, n-1)`.

Quelle profondeur pour `n = 1000000` ? Teste. Que se passe-t-il ?

## 4.2 — Maximum par diviser pour régner

Divise le tableau en deux, prends le max des deux max.

Complexité en temps ? En espace ? Compare à la boucle simple.

Cette version est-elle un progrès ? Justifie.

## 4.3 — Recherche dichotomique récursive

Réécris-la (elle était en exercice 2.5). Ajoute un compteur de profondeur.

## 4.4 — Générer les permutations

Affiche toutes les permutations de `"abc"`. Il y en a 6.

```
pour chaque position i à partir de debut :
    échanger tab[debut] et tab[i]
    permuter(debut + 1)
    échanger tab[debut] et tab[i]      ← annuler
```

Le second échange est le **retour sur trace**. Retire-le et observe le résultat :
tu comprendras à quoi il sert.

Combien de permutations pour `n = 8` ? Pour `n = 12` ? Complexité ?

## 4.5 — Sous-ensembles

Affiche tous les sous-ensembles de `{1, 2, 3}`. Il y en a 8.

À chaque élément, deux choix : le prendre ou non. Complexité O(2ⁿ), et c'est
normal — il y a bien `2ⁿ` sous-ensembles à produire.

## 4.6 — Le labyrinthe

```c
char laby[5][5] = {
    {'.','.','#','.','.'},
    {'#','.','#','.','#'},
    {'.','.','.','.','.'},
    {'.','#','#','#','.'},
    {'.','.','.','#','S'}
};
```

Depuis `(0,0)`, trouve un chemin jusqu'à `S`. Déplacements en haut, bas, gauche,
droite.

```
resoudre(x, y) :
    si hors limites ou mur ou déjà visité : faux
    si arrivée : vrai
    marquer visité
    si resoudre(voisin) pour un des 4 : vrai
    démarquer                          ← le backtrack
    faux
```

*Question :* faut-il vraiment démarquer ici ? Essaie avec et sans, et explique la
différence. (Ce n'est pas le même cas que pour les permutations.)

## 4.7 — Les n reines

Placer `n` reines sur un échiquier `n × n` sans qu'aucune ne se menace.

Compte le nombre de solutions pour `n = 4` (2), `n = 6` (4), `n = 8` (92).

Chronomètre `n = 8`, `n = 10`, `n = 12`. La croissance est brutale — c'est le
retour sur trace dans toute sa nature.

---

# Niveau 5 — Analyser

## 5.1 — Écrire les récurrences

Donne la relation de récurrence `T(n)` et sa solution :

- factorielle récursive
- recherche dichotomique récursive
- tri fusion
- maximum par diviser pour régner
- Fibonacci naïf
- tours de Hanoï

## 5.2 — Trois fonctions à analyser

```c
// (a)
int f(int n) {
    if (n <= 1) return 1;
    return f(n - 1) + f(n - 1);
}

// (b)
int g(int n) {
    if (n <= 1) return 1;
    return g(n / 2) + g(n / 2);
}

// (c)
int h(int n) {
    if (n <= 1) return 1;
    return 2 * h(n / 2);
}
```

(a) et (b) ont la même forme, des complexités différentes. Pourquoi ?
(b) et (c) calculent la même chose, avec des complexités différentes. Pourquoi ?

Vérifie les trois avec un compteur d'appels.

## 5.3 — Convertir en itératif

Prends une de tes fonctions récursives non terminales (le parcours de
permutations, par exemple) et réécris-la avec une **pile explicite**.

C'est fastidieux. C'est exactement le travail que la récursion faisait pour toi —
et la raison pour laquelle on la garde quand la structure du problème est
récursive.

---

# Auto-évaluation

- [ ] Quels sont les deux éléments obligatoires d'une fonction récursive ?
- [ ] Pourquoi une récursion profonde provoque-t-elle un crash ?
- [ ] Qu'est-ce qu'une récursion terminale, et le C garantit-il son optimisation ?
- [ ] Pourquoi Fibonacci naïf est-il O(2ⁿ) alors que factorielle est O(n) ?
- [ ] Qu'est-ce qui signale qu'il faut mémoïser ?
- [ ] Quand la récursivité est-elle préférable à l'itération ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **Afficher avec indentation** proportionnelle à la profondeur. Sur une
   récursion, c'est le seul débogage lisible.
2. **`lldb`** puis `bt` (*backtrace*) à un point d'arrêt : la pile d'appels
   s'affiche en entier. C'est la leçon rendue visible.
3. **Faire confiance à l'appel récursif.** Si tu essaies de dérouler tous les
   niveaux dans ta tête, tu t'y perdras. Suppose que `f(n-1)` est correct, et
   occupe-toi uniquement du passage de `n-1` à `n`.
