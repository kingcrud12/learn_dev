# Exercices — Variables et types

Support : [03_variables_et_types.md](../lessons/03_variables_et_types.md)

Vingt exercices, du plus simple au plus exigeant. Chaque niveau suppose le
précédent acquis.

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

**Règle du parcours :** aucun avertissement toléré. Un programme qui compile avec
un `warning` n'est pas terminé.

---

# Niveau 1 — Déclarer et afficher

Objectif : réserver de la place, y mettre une valeur, la faire sortir.

## 1.1 — Trois variables

Déclare et affiche, chacune sur sa ligne :

- un `int` valant ton âge
- un `char` valant ton initiale
- un `double` valant ta taille en mètres

Attendu (avec tes valeurs) :

```
Age : 30
Initiale : Y
Taille : 1.800000
```

*Points d'attention :* un marqueur par type (`%d`, `%c`, `%f`), et un `\n` à chaque
ligne.

## 1.2 — Deux décimales

Reprends la taille de l'exercice précédent et affiche-la avec exactement deux
décimales : `1.80` au lieu de `1.800000`.

*Indice :* le marqueur accepte une précision.

## 1.3 — Mesurer les types

Affiche la taille en octets de `char`, `short`, `int`, `long`, `float` et `double`.

```
char   : 1 octet(s)
short  : 2 octet(s)
int    : 4 octet(s)
...
```

*Points d'attention :* `sizeof` s'affiche avec `%zu`, pas `%d`.

## 1.4 — Une phrase composée

Refais `prensenting_myself.c` de mémoire, sans le relire : un tableau de `char`
pour le prénom, un `int` pour l'âge, une seule ligne `printf` qui combine les deux.

## 1.5 — Périmètre et aire

Déclare `largeur = 7.5` et `hauteur = 3.2` en `double`. Affiche le périmètre et
l'aire du rectangle, avec deux décimales.

---

# Niveau 2 — Calculer

Objectif : les opérateurs, et les pièges de la division.

## 2.1 — Les quatre opérations

Avec `a = 17` et `b = 5` (deux `int`), affiche successivement `a+b`, `a-b`, `a*b`,
`a/b` et `a%b`.

Résultat attendu :

```
17 + 5 = 22
17 - 5 = 12
17 * 5 = 85
17 / 5 = 3
17 % 5 = 2
```

**Arrête-toi sur `17 / 5 = 3`.** Explique à voix haute pourquoi ce n'est pas 3.4.

## 2.2 — Forcer la division réelle

Toujours avec `a = 17` et `b = 5` en `int`, obtiens `3.400000`.

Trouve **deux** façons différentes d'y parvenir sans changer le type des variables.

## 2.3 — Moyenne de trois notes

Trois notes entières (`14`, `17`, `12`). Affiche la moyenne exacte avec une décimale.

*Piège :* c'est le bug de `moyenne` de la leçon 2, doublé de celui de 2.1. Deux
erreurs possibles à la fois — les parenthèses **et** le type.

Attendu : `14.3`

## 2.4 — Celsius vers Fahrenheit

Convertis `37.0 °C` en Fahrenheit. Formule : `F = C × 9/5 + 32`.

Attendu : `98.6`

*Piège :* si tu écris `9/5` avec des entiers, tu obtiens `1` et tout le calcul est faux.

## 2.5 — Secondes en h/m/s

Convertis `7385` secondes en heures, minutes et secondes.

Attendu :

```
7385 secondes = 2h 3m 5s
```

*Indice :* `/` donne le quotient, `%` donne le reste. Les deux te suffisent.

---

# Niveau 3 — Conversions et limites

Objectif : voir ce que le C fait quand un type ne suffit plus.

## 3.1 — Troncature

Déclare `double x = 3.7;` puis `int n = x;`. Affiche les deux.

`n` vaut-il 3 ou 4 ? Note la réponse **avant** de compiler, puis vérifie.

## 3.2 — Arrondir à la main

Sans utiliser `math.h`, transforme `3.7` en `4` et `3.2` en `3`.

*Indice :* que se passe-t-il si tu ajoutes `0.5` avant de tronquer ?

## 3.3 — Les limites d'un `int`

Avec `#include <limits.h>`, affiche `INT_MAX` et `INT_MIN`.

Puis stocke `INT_MAX` dans une variable, ajoute 1, et affiche le résultat.

**Le résultat va te surprendre.** Explique-le : 4 octets, 32 bits, combien de
valeurs distinctes ?

## 3.4 — Un dépassement silencieux

```c
int pop = 2000000000;
printf("%d\n", pop + pop);
```

Deux milliards plus deux milliards. Compile, exécute, constate.

Puis corrige le programme pour qu'il affiche le bon résultat — sans changer les
valeurs.

## 3.5 — L'avertissement du compilateur

Écris `int trop_grand = 5000000000;` et compile avec `-Wall -Wextra`.

Lis le message en entier. Que dit-il exactement sur la valeur ? Le programme
est-il quand même construit ? Pourquoi est-ce plus grave qu'une erreur ?

## 3.6 — Comparer des flottants

```c
double a = 0.1 + 0.2;
double b = 0.3;
```

Affiche `a` avec vingt décimales (`%.20f`), puis teste `a == b`.

Écris ensuite une comparaison qui fonctionne, en passant par un écart toléré.

---

# Niveau 4 — Les caractères sont des nombres

Objectif : exploiter le fait qu'un `char` est un entier sur 1 octet.

## 4.1 — Code ASCII

Affiche la valeur numérique de `'A'`, `'a'`, `'0'` et `' '` avec `%d`.

Attendu : `65`, `97`, `48`, `32`.

## 4.2 — Le chiffre caché

Le caractère `'7'` ne vaut pas 7 mais 55. Convertis-le en l'entier `7`, puis
multiplie-le par 2 pour prouver que c'est bien devenu un nombre.

*Indice :* que vaut `'7' - '0'` ?

## 4.3 — Minuscule vers majuscule

Transforme `'g'` en `'G'` par un simple calcul, sans `toupper` ni condition.

*Indice :* calcule d'abord `'a' - 'A'`. Cet écart est le même pour toutes les lettres.

## 4.4 — Lettre suivante

Affiche la lettre qui suit `'m'` dans l'alphabet. Puis celle qui la précède.

## 4.5 — Chaîne, taille et longueur

Avec `char nom[] = "Yann";`, affiche :

- `sizeof(nom)`
- `strlen(nom)` (nécessite `<string.h>`)
- chaque caractère un par un, avec son code numérique

Les deux premiers nombres diffèrent. **Explique pourquoi.**

---

# Niveau 5 — Interaction

Objectif : lire une entrée, et se méfier de ce que l'utilisateur tape.

## 5.1 — Premier `scanf`

Demande son âge à l'utilisateur, puis affiche-le dans une phrase.

*Piège :* le `&` devant la variable est obligatoire. Essaie **volontairement** sans,
compile, exécute. Note ce qui se passe.

## 5.2 — Calculatrice

Demande deux entiers, affiche leur somme, différence, produit, quotient entier,
reste, et quotient réel avec deux décimales.

## 5.3 — Ta propre fiche

Demande un prénom (`%s`), un âge (`%d`) et une taille en mètres (`%lf` pour un
`double`), puis affiche une phrase récapitulative.

*Points d'attention :* le prénom se lit dans un tableau de taille fixe
(`char nom[50];`). Le `&` ne se met **pas** devant un tableau — trouve pourquoi
en essayant.

---

# Niveau 6 — Aller plus loin

Objectif : combiner ce qui précède, avec des astuces qui font réfléchir.

## 6.1 — Décomposer un nombre

Demande un entier à quatre chiffres (par exemple `4729`) et affiche séparément
ses unités, dizaines, centaines et milliers, puis leur somme.

Attendu pour `4729` :

```
unites   : 9
dizaines : 2
centaines: 7
milliers : 4
somme    : 22
```

*Indice :* `/` et `%` combinés suffisent. Aucune boucle n'est nécessaire.

## 6.2 — Intervertir sans variable temporaire

Deux entiers `a = 5` et `b = 12`. Échange leurs valeurs **sans déclarer de
troisième variable**.

*Indice :* trois lignes, uniquement des additions et des soustractions.

Vérification : `a = 12`, `b = 5`.

## 6.3 — Parité sans condition

Détermine si un nombre est pair, et affiche `0` pour impair, `1` pour pair —
sans utiliser `if`.

*Indice :* que vaut `n % 2` ? Comment le transformer ?

## 6.4 — Prix TTC

Demande un prix hors taxes et un taux de TVA en pourcentage (`20` pour 20 %).
Affiche le montant de TVA et le prix TTC, chacun avec deux décimales.

*Piège :* le taux est un entier, le prix un `double`. Attention à l'ordre des
opérations.

## 6.5 — Le programme qui ment

Ce programme est syntaxiquement correct, compile sans avertissement, et donne un
résultat faux :

```c
#include <stdio.h>

int main(void) {
    int total = 10;
    int nombre = 4;
    double moyenne = total / nombre;
    printf("Moyenne : %.2f\n", moyenne);
    return 0;
}
```

Il affiche `2.00` au lieu de `2.50`.

Trouve la cause **avant** de corriger. Puis vérifie ton hypothèse avec `lldb` :
point d'arrêt sur la ligne du calcul, `p total / nombre`, `p (double)total / nombre`.

C'est exactement la démarche de la leçon 2, appliquée à un bug de type.

---

# Auto-évaluation

Tu peux passer à la suite si tu réponds sans hésiter :

- [ ] Pourquoi `7 / 2` donne `3` et non `3.5` ?
- [ ] Que vaut `sizeof(name)` pour `char name[] = "Yann"` — et pourquoi pas 4 ?
- [ ] Que se passe-t-il quand on ajoute 1 à `INT_MAX` ? Le compilateur prévient-il ?
- [ ] Pourquoi `0.1 + 0.2 == 0.3` est faux ?
- [ ] Pourquoi `scanf` exige un `&` alors que `printf` non ?
- [ ] Quelle est la différence entre `'A'` et `"A"` ?

---

# Solutions

Aucune n'est donnée ici — volontairement.

Trois recours dans l'ordre :

1. **Relire la leçon.** Chaque exercice correspond à une section précise.
2. **Interroger le programme** : `printf` de contrôle, ou `lldb` avec un point
   d'arrêt et `p expression`.
3. **Expliquer le code à voix haute**, ligne par ligne. C'est la règle n°4 du
   `README.md`, et elle résout plus de bugs qu'on ne le croit.

Un exercice qui résiste plus de trente minutes mérite une question, pas un
acharnement.
