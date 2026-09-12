# Ce que je sais déjà — le bilan par le hello world

Point d'étape. Tout ce qui suit part du premier programme du parcours, et montre
ce que chacune de ses six lignes contient — maintenant que tu sais les lire.

---

## Le programme de départ

```c
#include <stdio.h>

int main() {
  printf("Hello, World!");
  return 0;
}
```

Six lignes. Quand tu l'as tapé, c'était une formule magique. Voici ce que tu y
vois aujourd'hui.

---

## Ligne 1 — `#include <stdio.h>`

**Ce que tu sais maintenant :**

Le `#` marque une directive de **préprocesseur**, traitée avant la compilation.
Elle colle le contenu de `stdio.h` à la place de la ligne.

Ce qui arrive alors, c'est la **déclaration** de `printf` :

```c
int printf(const char *format, ...);
```

Une annonce, pas du code. Le compilateur lit de haut en bas et doit connaître
`printf` avant de rencontrer ton appel. Le code réel arrive plus tard, à
l'édition de liens.

Tu sais aussi maintenant lire ce prototype :
- `const char *format` — un **pointeur** vers des caractères, en lecture seule
- `...` — un nombre variable d'arguments, d'où `printf("%d %d", a, b)`

**Tu l'as pratiqué :** `#include <math.h>` dans `calculator.c`, pour `NAN`.

**La distinction acquise :** déclaration (`;`, dans le `.h`) contre définition
(`{ }`, dans le `.c`). Deux erreurs différentes en découlent —
`implicit declaration` à la compilation, `undefined symbol` à l'édition de liens.

---

## Ligne 3 — `int main()`

**Ce que tu sais maintenant :**

C'est le point d'entrée. Le système appelle cette fonction quand tu lances
`./programme`.

**Le `int`** est le type de retour : un entier rendu au système d'exploitation.

**Les parenthèses vides** signifient « je ne précise pas les paramètres », pas
« aucun paramètre ». La forme rigoureuse est `int main(void)` — tu l'as appliquée
dans `scan.c`.

Tu sais aussi qu'il existe une seconde forme, pour les arguments de ligne de
commande :

```c
int main(int argc, char *argv[])
```

`argv` est un **tableau de pointeurs vers des chaînes**. Trois notions dans une
déclaration, et tu peux maintenant la lire.

---

## Ligne 4 — `printf("Hello, World!")`

**Ce que tu sais maintenant :**

L'appel est accepté parce que la déclaration venue de `stdio.h` est passée
au-dessus.

**La chaîne en mémoire :**

```
'H' 'e' 'l' 'l' 'o' ',' ' ' 'W' 'o' 'r' 'l' 'd' '!' '\0'
```

Ce `'\0'` final marque la fin. Le C ne stocke aucune longueur à côté : `printf`
avance caractère par caractère et s'arrête sur le zéro.

Tu sais désormais que ce littéral vit dans une zone **en lecture seule** de
l'exécutable, et que ceci segfaulterait :

```c
char *s = "Hello";
s[0] = 'J';        // écriture dans une page en lecture seule → SIGSEGV
```

Alors que ceci fonctionne, parce que la chaîne est **copiée** dans un tableau à
toi :

```c
char s[] = "Hello";
s[0] = 'J';        // OK
```

**Tu l'as pratiqué :** `char name[] = "Yann";` dans `presenting_myself.c`.

**La bufférisation.** Sans `\n`, la chaîne part dans un tampon en mémoire et
n'atteint pas l'écran tout de suite. La libc regroupe les écritures pour
minimiser les **appels système**, qui coûtent cher. C'est pourquoi ce programme
laisse le prompt du shell collé à la sortie.

**Les marqueurs de format** que tu utilises maintenant :

| Marqueur | Type | Où tu l'as écrit |
|---|---|---|
| `%d` | `int` | tous tes programmes |
| `%c` | `char` | `display_var.c` |
| `%s` | chaîne | `presenting_myself.c` |
| `%.2f` | `double`, 2 décimales | `maths.c`, `calculator.c` |
| `%zu` | `size_t` (pour `sizeof`) | `display_var.c` |
| `%p` | une adresse | à venir |

---

## Ligne 5 — `return 0;`

**Ce que tu sais maintenant :**

C'est le **code de sortie**, récupéré par le shell. Convention Unix : `0` =
succès, non nul = erreur.

Ce n'est pas décoratif :

```sh
./programme && echo "OK"     # le echo ne s'exécute que si le retour est 0
echo $?                       # affiche le code de sortie
```

C'est aussi ce dont se servent `make`, les scripts CI et les tests automatisés.

Tu sais également qu'en C99+, l'omettre est légal **pour `main` uniquement** — le
compilateur sous-entend `return 0;`. Tes fichiers `display_var.c` et
`presenting_myself.c` en profitent sans le savoir.

---

# Ce que tes cinq programmes ont ajouté

## `display_var.c` — les types ont une taille

```c
int age = 30;
char name = 'Y';
double height = 1.83;
printf("sizeof(age) = %lu\n", sizeof(age));
```

**Ce que tu y as appris :** un type n'est pas une catégorie abstraite, c'est un
nombre d'octets réservés. `int` en fait 4, `char` 1, `double` 8.

**Le détail à corriger :** `sizeof` renvoie un `size_t`, dont le marqueur est
`%zu`, pas `%lu`. Ça fonctionne sur ton Mac par coïncidence (les deux types y
font 8 octets) mais casserait ailleurs. `-Wall` ne le signale pas toujours.

## `presenting_myself.c` — un tableau de caractères

```c
char name[] = "Yann";
printf("Hi, I'm %s...", name);
```

**Ce que tu y as appris :** une chaîne est un tableau de `char` terminé par
`'\0'`. `name` occupe 5 octets, pas 4.

Tu sais maintenant que `%s` reçoit **l'adresse du premier caractère**, et que
`printf` avance jusqu'au zéro terminal. C'est pourquoi il n'y a pas de `&`
devant `name` — un tableau est déjà une adresse.

## `maths.c` — les fonctions

```c
double perimeter(double length, double width){
    return 2 * (length + width);
}
```

**Ce que tu y as appris :** définir une fonction au-dessus de `main`, choisir un
type de retour, nommer des paramètres.

Tu sais aussi maintenant que ces paramètres sont des **copies locales** —
`length` dans la fonction n'a aucun lien avec `length` dans `main`, même nom
compris.

## `calculator.c` — la division, et les erreurs

```c
double divide(int x, int y) {
    if (y == 0) {
        return NAN;
    }
    return (double)x / y;
}
```

**Trois choses acquises ici, et ce sont les plus importantes.**

**1. La division entière.** `x / y` entre deux `int` tronque. Le cast `(double)x`
convertit un opérande, et la **promotion arithmétique** convertit l'autre
automatiquement. Tu as évité le piège `(double)(x / y)`, qui divise d'abord.

**2. Signaler une erreur sans exception.** Ton premier `return 0` était ambigu :
impossible de distinguer « division par zéro » de « le résultat vaut zéro ».
`NAN` lève l'ambiguïté — l'appelant teste `isnan()`.

**3. La séparation des responsabilités.** Tu as sorti le `printf` de la fonction
de calcul. Une fonction qui calcule ne devrait pas décider comment l'erreur
s'affiche.

## `scan.c` — le premier pointeur

```c
int nombre;
scanf("%d", &nombre);
```

**Ce que tu y as appris :** le `&` donne l'**adresse** de `nombre`. `scanf` en a
besoin parce qu'il doit **écrire** dans ta variable — et qu'en C les paramètres
sont copiés.

Le mécanisme complet, que tu peux maintenant réciter :

```
&nombre  →  l'adresse (0x1000) est passée à scanf
scanf    →  lit "22" au clavier, convertit en entier 22
*ptr = 22 →  écrit à l'adresse reçue
```

**Le `*` est bien là** — dans `scanf`, pas dans ton code. C'est la répartition
standard : `&` chez l'appelant, `*` dans la fonction appelée.

---

# La carte de ce que tu maîtrises

## Acquis

| Domaine | Ce que tu sais faire |
|---|---|
| **Compilation** | `gcc -Wall -Wextra -g`, lire les avertissements, un `Makefile` avec règle générique |
| **Types** | `int`, `char`, `double`, `sizeof`, les casts, la promotion arithmétique |
| **Affichage** | `printf` et ses marqueurs, `%.2f`, la bufférisation |
| **Saisie** | `scanf` et le `&`, tester son retour |
| **Fonctions** | définir, appeler, prototype, passage par valeur |
| **Contrôle** | `if`/`else`, boucles |
| **Chaînes** | tableau de `char`, `'\0'`, `char s[]` contre `char *s` |
| **Erreurs** | `NAN` + `isnan`, `stderr`, code de sortie |
| **Vocabulaire** | adresse, pointeur, déclaration vs définition, pile |

## Le modèle mental en place

Trois idées que tu peux maintenant énoncer :

**1. Un type est une taille et une convention de lecture.** Les mêmes octets lus
autrement donnent autre chose. C'est ce que fait ton cast `(double)x`.

**2. La mémoire est un tableau d'octets numérotés.** Un pointeur est un numéro
de case. `&` donne le numéro, `*` va le chercher.

**3. Un programme se construit.** Préprocesseur, compilation, assemblage,
édition de liens. Chaque étape a ses erreurs propres.

## Ce qui vient ensuite

| Leçon | Ce que ça débloque |
|---|---|
| [04 Headers](04_headers_et_compilation.md) | découper `calculator.c` en trois fichiers |
| [05 Représentation](05_representation_binaire.md) | pourquoi `0.1 + 0.2 != 0.3` |
| [06 Tableaux](06_tableaux_et_chaines.md) | manipuler des séries de données |
| [07 **Pointeurs**](07_pointeurs.md) | la charnière — mémoire virtuelle, segfault |
| [08 `malloc`](08_memoire_dynamique.md) | des tailles décidées à l'exécution |
| [09 Structures](09_structures.md) | regrouper des données liées |
| [10 Fichiers](10_fichiers_et_appels_systeme.md) | lire et écrire, appels système |

---

# Le hello world réécrit avec ce que tu sais

```c
#include <stdio.h>

int main(void) {                          // void, pas ()
    printf("Hello, World!\n");            // \n : vide le tampon, libère le prompt
    return 0;                             // explicite, même si sous-entendu
}
```

Trois changements minuscules, et chacun repose sur quelque chose que tu ne
savais pas au départ : la différence entre `()` et `(void)`, la bufférisation de
la sortie, et le rôle du code de retour auprès du shell.

**C'est ça, la progression.** Le programme n'a pas changé de taille — c'est ce
que tu y vois qui a changé.

---

## Le prochain geste

Écris cette fonction. Elle ne demande rien que tu n'aies déjà, et c'est le
premier `*` que tu écriras toi-même :

```c
void echanger(int *a, int *b);

int x = 3, y = 7;
echanger(&x, &y);
printf("%d %d\n", x, y);    // 7 3
```

Quand elle marche, la notion de pointeur est acquise — et la leçon 07 devient
une formalité.
