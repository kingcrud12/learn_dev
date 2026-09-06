# Ce qu'il y a vraiment dans un type

Un type n'est pas une catégorie abstraite : c'est une taille en octets et une
convention de lecture. Cette leçon ouvre la boîte.

Suite de [04_headers_et_compilation.md](04_headers_et_compilation.md).

---

## 1. La mémoire est une suite d'octets numérotés

Rien de plus. Pas de types, pas de noms, pas de structure : une immense rangée
de cases, chacune contenant 8 bits, chacune ayant un numéro appelé **adresse**.

```
adresse:  0x1000  0x1001  0x1002  0x1003  0x1004 ...
contenu:  [0x2A]  [0x00]  [0x00]  [0x00]  [0x41] ...
```

Quand tu écris `int n = 42;`, il se passe exactement ceci :

1. Le compilateur réserve **4 cases consécutives**
2. Il y écrit la représentation binaire de 42
3. Il retient que le nom `n` désigne la première de ces cases
4. Il retient que ces 4 octets doivent être lus **comme un entier signé**

Les points 3 et 4 n'existent qu'à la compilation. **À l'exécution, il ne reste
que des octets.** Le nom `n` a disparu, le type aussi.

C'est pourquoi le C peut faire des choses que d'autres langages interdisent :
il te laisse relire les mêmes octets sous un autre type.

---

## 2. Tailles

```c
#include <stdio.h>

int main(void) {
    printf("char        %zu\n", sizeof(char));
    printf("short       %zu\n", sizeof(short));
    printf("int         %zu\n", sizeof(int));
    printf("long        %zu\n", sizeof(long));
    printf("float       %zu\n", sizeof(float));
    printf("double      %zu\n", sizeof(double));
    printf("pointeur    %zu\n", sizeof(void *));
    return 0;
}
```

Sur ton Mac (ARM64, 64 bits) :

| Type | Octets | Bits | Peut représenter |
|---|---|---|---|
| `char` | 1 | 8 | −128 à 127 |
| `short` | 2 | 16 | −32 768 à 32 767 |
| `int` | 4 | 32 | ±2,1 milliards |
| `long` | 8 | 64 | ±9,2 × 10¹⁸ |
| `double` | 8 | 64 | ~15 chiffres significatifs |
| `void *` | 8 | 64 | une adresse |

**Ces tailles ne sont pas garanties par la norme.** Elle impose seulement des
minimums et l'ordre `char ≤ short ≤ int ≤ long`. Un `int` fait 2 octets sur
certains microcontrôleurs — tu le rencontreras en embarqué.

D'où `<stdint.h>`, quand la taille compte vraiment :

```c
#include <stdint.h>

int32_t  compteur;    // exactement 32 bits, partout
uint8_t  octet;       // exactement 8 bits, non signé
uint64_t horodatage;
```

**Réflexe :** `int` pour du calcul ordinaire, `int32_t`/`uint8_t` dès que tu
manipules un format binaire, un protocole, ou un registre matériel.

---

## 3. Les entiers négatifs : le complément à deux

Comment stocker `-1` avec des bits, qui ne connaissent que 0 et 1 ?

La solution retenue par tous les processeurs modernes s'appelle le **complément
à deux**. La règle : pour obtenir `-n`, on inverse tous les bits de `n` puis on
ajoute 1.

Sur 8 bits :

```
  1   =  0000 0001
inverse=  1111 1110
    +1 =  1111 1111   →  -1
```

Vérifions par le calcul : `1111 1111` + `0000 0001` = `1 0000 0000`. Le neuvième
bit déborde et disparaît, il reste `0000 0000` = 0. Donc `255 + 1 = 0`, donc
`1111 1111` se comporte bien comme `-1`. Le mécanisme est cohérent.

**Ce que ça explique.** Le bit de poids fort est le signe : 0 = positif,
1 = négatif. Et l'addition fonctionne à l'identique pour les positifs et les
négatifs — c'est tout l'intérêt : le processeur n'a besoin que d'**un seul
circuit d'addition**.

### Le voir toi-même

```c
#include <stdio.h>

int main(void) {
    int n = -1;
    printf("%d en hexa : 0x%X\n", n, (unsigned int)n);   // 0xFFFFFFFF
    return 0;
}
```

---

## 4. Signé et non signé : un piège réel

```c
#include <stdio.h>

int main(void) {
    unsigned int u = 0;
    u = u - 1;
    printf("%u\n", u);           // 4294967295, pas -1

    int          a = -1;
    unsigned int b = 1;
    if (a < b) {
        printf("normal\n");
    } else {
        printf("surprise\n");    // c'est CE message qui s'affiche
    }
    return 0;
}
```

**Pourquoi.** Dans la comparaison `a < b`, les deux opérandes doivent avoir le
même type. Les règles de conversion du C convertissent le `int` signé en
`unsigned int`. `-1` devient alors `4294967295`, qui est bien supérieur à `1`.

C'est exactement le genre de bug qui passe les tests et casse en production.
`-Wextra` te prévient (`comparison of integer expressions of different
signedness`) — encore une raison de ne jamais l'enlever.

### Un piège classique dérivé

```c
for (unsigned int i = taille - 1; i >= 0; i--) {   // BOUCLE INFINIE
    ...
}
```

Un `unsigned` n'est **jamais** négatif. La condition `i >= 0` est toujours
vraie. Quand `i` vaut 0 et qu'on décrémente, il repasse à 4294967295.

---

## 5. Le débordement signé est indéfini

```c
int n = 2147483647;   // INT_MAX
n = n + 1;            // comportement INDÉFINI
```

Attention à la nuance, elle est importante :

| | Débordement |
|---|---|
| `unsigned` | **Défini** : boucle modulo 2ⁿ. Garanti par la norme. |
| signé | **Indéfini** : le compilateur peut faire n'importe quoi. |

« Indéfini » (*undefined behavior*, UB) ne veut pas dire « valeur imprévisible ».
Ça veut dire que le compilateur a le droit de supposer que ça n'arrive jamais,
et d'optimiser en conséquence — y compris en supprimant du code.

Exemple réel :

```c
if (n + 1 < n) { ... }    // gcc peut supprimer ce test entièrement
```

Puisque déborder est UB, le compilateur en déduit que `n + 1 > n` toujours, donc
que la condition est toujours fausse. Le test disparaît à l'optimisation. C'est
légal, et c'est une source de failles de sécurité bien réelles.

**Retiens :** en C, UB n'est pas un bug de comportement — c'est l'absence de
contrat. Le sanitizer les détecte :

```sh
gcc -fsanitize=undefined -g prog.c -o prog
```

---

## 6. Les flottants : IEEE 754

Tu as déjà vu que `0.1 + 0.2 != 0.3`. Voici pourquoi.

Un `double` répartit ses 64 bits ainsi :

```
[signe : 1 bit][exposant : 11 bits][mantisse : 52 bits]

valeur = (-1)^signe × 1,mantisse × 2^(exposant-1023)
```

C'est de la **notation scientifique en base 2**. Et c'est là qu'est le problème :
en base 2, `0.1` s'écrit `0.0001100110011001100...` — infini périodique, comme
`1/3 = 0,333...` en base 10.

Avec 52 bits de mantisse, on tronque. La valeur stockée n'est pas `0.1` mais
la plus proche représentable.

```c
printf("%.20f\n", 0.1);    // 0.10000000000000000555
```

### Les conséquences pratiques

**1. Ne jamais tester l'égalité de deux flottants.**

```c
if (a == b) { ... }                    // FRAGILE

#include <math.h>
if (fabs(a - b) < 1e-9) { ... }        // correct
```

**2. L'argent ne se stocke pas en `double`.** On stocke des centimes en entier.
Les erreurs d'arrondi s'accumulent.

**3. L'addition n'est pas associative.**

```c
(0.1 + 0.2) + 0.3   !=   0.1 + (0.2 + 0.3)
```

Un fait qui a des conséquences énormes en calcul scientifique et en
parallélisation.

### Voir la représentation brute

Le format `%a` affiche un flottant en hexadécimal, mantisse et exposant visibles :

```c
printf("%a\n", 0.1);    // 0x1.999999999999ap-4
```

Lecture : `1.999999999999a` en hexa (la mantisse), fois 2 puissance −4. Le `a`
final est le dernier chiffre tronqué — la trace exacte de l'imprécision.

---

## 7. Les opérateurs bit-à-bit

Ils agissent sur chaque bit individuellement.

| Opérateur | Nom | Effet |
|---|---|---|
| `&` | ET | 1 si les deux bits sont à 1 |
| `\|` | OU | 1 si au moins un bit est à 1 |
| `^` | OU exclusif | 1 si les bits diffèrent |
| `~` | NON | inverse tous les bits |
| `<<` | décalage gauche | multiplie par 2 à chaque cran |
| `>>` | décalage droite | divise par 2 à chaque cran |

```c
unsigned char a = 0b1100;    // 12
unsigned char b = 0b1010;    // 10

a & b    // 0b1000 = 8
a | b    // 0b1110 = 14
a ^ b    // 0b0110 = 6
a << 1   // 0b11000 = 24
a >> 2   // 0b0011 = 3
```

**Ne les confonds pas avec `&&` et `||`**, qui sont logiques et travaillent sur
des conditions entières, pas bit par bit.

### Les drapeaux : leur usage principal

Plutôt que huit variables booléennes, un seul octet portant huit informations.

```c
#define LECTURE   (1 << 0)    // 0b00000001
#define ECRITURE  (1 << 1)    // 0b00000010
#define EXECUTION (1 << 2)    // 0b00000100

unsigned int perms = LECTURE | ECRITURE;   // activer

if (perms & ECRITURE) { ... }              // tester
perms &= ~ECRITURE;                        // désactiver
perms ^= LECTURE;                          // basculer
```

C'est **exactement** le mécanisme des permissions Unix que tu verras dans
`2.Learn_linux` : le `755` de `chmod` est un jeu de bits. Et c'est aussi ainsi
qu'on pilote un registre matériel en embarqué.

---

## 8. Le boutisme (endianness)

Un `int` occupe 4 octets. Dans quel **ordre** sont-ils rangés ?

Pour la valeur `0x12345678` :

```
petit-boutiste (little-endian)  →  78 56 34 12   ← x86, ARM, ton Mac
gros-boutiste  (big-endian)     →  12 34 56 78   ← réseau, quelques CPU
```

Ton Mac est petit-boutiste : l'octet de **poids faible** est stocké en premier.

### Le vérifier

```c
#include <stdio.h>

int main(void) {
    int n = 0x12345678;
    unsigned char *p = (unsigned char *)&n;   // relire les mêmes octets un par un

    for (int i = 0; i < 4; i++) {
        printf("%02X ", p[i]);
    }
    printf("\n");    // 78 56 34 12 sur ton Mac
    return 0;
}
```

Ce petit programme est la démonstration la plus directe de l'idée de cette
leçon : **les mêmes octets, lus sous un autre type, deviennent autre chose.**
Le cast en `unsigned char *` ne convertit rien — il change juste la lunette.

**Quand ça compte :** dès que des octets sortent de ta machine. Les protocoles
réseau imposent le gros-boutiste (« network byte order »), d'où les fonctions
`htons`, `htonl`. Un fichier binaire écrit sur un Mac et relu sur un autre CPU
peut donner n'importe quoi si tu n'y prends pas garde.

---

## 9. Exercices

1. Écris le programme de la section 2 et note les tailles sur ta machine.

2. Affiche `-1`, `-42` et `INT_MIN` en hexadécimal (`%X`). Retrouve à la main
   la règle du complément à deux sur `-42` en 8 bits.

3. Écris une fonction `void afficher_bits(unsigned char c)` qui affiche les
   8 bits d'un octet. Utilise `c >> i & 1` dans une boucle.
   Teste-la sur `'A'`, `42`, `255`.

4. Reproduis le piège signé/non-signé de la section 4. Recompile avec
   `-Wextra` seul et lis l'avertissement exact.

5. Vérifie le boutisme de ta machine avec le programme de la section 8.
   Puis fais la même chose avec un `double` de valeur `1.0` — 8 octets à
   afficher. Retrouve la structure IEEE 754 dedans.

6. Implémente les permissions Unix par drapeaux : une fonction qui prend un
   `unsigned int` et affiche `rwx` ou `-` pour chaque bit.

---

## Mémo

```c
sizeof(int)              // 4 sur ton Mac, pas garanti ailleurs
#include <stdint.h>
int32_t / uint8_t        // taille garantie, pour formats binaires

// complément à deux
-1  ==  0xFFFFFFFF       // en int 32 bits

// signé vs non signé
unsigned u = 0; u - 1;   // 4294967295, défini
int n = INT_MAX; n + 1;  // UNDEFINED BEHAVIOR

// flottants
fabs(a - b) < 1e-9       // jamais a == b
printf("%a", x)          // voir la représentation brute

// bits
x | MASQUE               // activer
x & MASQUE               // tester
x &= ~MASQUE             // désactiver
1 << n                   // le n-ième bit

// relire les octets bruts
unsigned char *p = (unsigned char *)&valeur;
```

```sh
gcc -fsanitize=undefined,address -g prog.c -o prog   # détecte les UB
```

**Les trois réflexes :**

1. Un type = une taille + une convention de lecture
2. Jamais `==` entre deux flottants
3. Le débordement signé est indéfini, pas « ça boucle »
