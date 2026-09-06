# Tableaux et chaînes de caractères

Un bloc contigu d'octets, et une convention pour marquer la fin. C'est tout ce
qu'il y a — et c'est pour ça qu'il faut être prudent.

Suite de [05_representation_binaire.md](05_representation_binaire.md).

---

## 1. Un tableau est un bloc contigu

```c
int notes[5] = {12, 15, 8, 17, 10};
```

En mémoire, 5 × 4 = **20 octets côte à côte**, sans interruption :

```
adresse:  0x1000    0x1004    0x1008    0x100C    0x1010
          [  12  ]  [  15  ]  [   8  ]  [  17  ]  [  10  ]
index:       0         1         2         3         4
```

L'indice commence à **0**. Ce n'est pas une convention arbitraire : c'est un
**décalage** depuis le début. `notes[0]` est à « début + 0 case », `notes[3]`
à « début + 3 cases ».

D'où la formule que le processeur applique réellement :

```
adresse de notes[i]  =  adresse de notes  +  i × sizeof(int)
```

Une multiplication et une addition. **C'est pour ça que l'accès à un tableau
est instantané, quel que soit l'indice** — contrairement à une liste chaînée,
que tu verras dans `3.projects_to_learn_algo`.

---

## 2. Déclarer et initialiser

```c
int a[5];                        // 5 int NON initialisés — contiennent n'importe quoi
int b[5] = {1, 2, 3, 4, 5};      // explicite
int c[5] = {1, 2};               // {1, 2, 0, 0, 0} — le reste est mis à zéro
int d[5] = {0};                  // tout à zéro (idiome courant)
int e[]  = {1, 2, 3};            // taille déduite : 3

const int TAILLE = 5;
int f[TAILLE];                   // valide en C99+ (VLA), mais à éviter
```

**Le piège du tableau non initialisé.** `int a[5];` ne met rien à zéro. Il te
donne 20 octets tels qu'ils étaient — restes de calculs précédents. Lire
`a[0]` avant d'y écrire donne une valeur imprévisible.

**Réflexe :** `int a[5] = {0};` par défaut.

---

## 3. La taille : `sizeof` et son piège

```c
int notes[5] = {12, 15, 8, 17, 10};

sizeof(notes)              // 20  — octets totaux
sizeof(notes) / sizeof(notes[0])   // 5  — nombre d'éléments
```

L'idiome `sizeof(tableau) / sizeof(tableau[0])` est le seul moyen de connaître
la longueur d'un tableau en C. **Et il cesse de fonctionner dès qu'on passe le
tableau à une fonction** — on verra pourquoi en section 6.

---

## 4. Aucune vérification de borne

C'est le point le plus important de cette leçon.

```c
int notes[5] = {0};
notes[10] = 42;      // compile sans erreur. S'exécute sans erreur.
```

Le C ne vérifie **jamais** que ton indice est valide. Il calcule
`adresse + 10 × 4` et écrit là. Cette adresse appartient à autre chose : une
autre variable, un pointeur de retour, n'importe quoi.

Trois issues possibles, par ordre de gravité croissante :

1. **Segfault** — tu as touché une page mémoire non mappée. Le meilleur cas :
   le problème est visible immédiatement.
2. **Corruption silencieuse** — tu as écrasé une autre de tes variables. Le
   programme continue avec des données fausses. Le bug apparaîtra ailleurs,
   plus tard, sans lien apparent.
3. **Faille de sécurité** — si l'indice vient d'une entrée utilisateur, un
   attaquant contrôle où écrire. C'est le **débordement de tampon**, la
   catégorie de faille la plus exploitée de l'histoire de l'informatique.

### Le démontrer

```c
#include <stdio.h>

int main(void) {
    int avant  = 111;
    int tab[3] = {0, 0, 0};
    int apres  = 999;

    tab[3] = 42;      // hors borne : une case trop loin

    printf("avant = %d, apres = %d\n", avant, apres);
    // l'un des deux a probablement changé
    return 0;
}
```

Lance-le. Puis relance-le avec le sanitizer :

```sh
gcc -fsanitize=address -g prog.c -o prog && ./prog
```

Le message est explicite : `stack-buffer-overflow`, avec l'endroit exact.
**Prends l'habitude de compiler avec `-fsanitize=address` pendant tout le reste
du module.**

---

## 5. Parcourir un tableau

```c
int notes[5] = {12, 15, 8, 17, 10};
int n = sizeof(notes) / sizeof(notes[0]);

for (int i = 0; i < n; i++) {         // i < n, jamais i <= n
    printf("%d ", notes[i]);
}
```

**L'erreur classique** (*off-by-one*) : `i <= n` accède à `notes[5]`, qui
n'existe pas. Le dernier indice valide est toujours `n - 1`.

Somme et moyenne :

```c
int somme = 0;
for (int i = 0; i < n; i++) {
    somme += notes[i];
}
double moyenne = (double)somme / n;    // le cast, comme dans ta calculatrice
```

---

## 6. Passer un tableau à une fonction

Voici ce qui surprend tout le monde.

```c
void afficher(int tab[]) {
    int n = sizeof(tab) / sizeof(tab[0]);   // FAUX : donne 8/4 = 2
    ...
}
```

**Ce qui se passe.** Quand on passe un tableau à une fonction, il n'est pas
copié. Ce qui est transmis, c'est **l'adresse de son premier élément**. Le
paramètre `int tab[]` est en réalité un `int *` — un pointeur — déguisé.

On appelle ça la **décroissance** (*array decay*) : le tableau « se dégrade » en
pointeur au passage.

Donc `sizeof(tab)` dans la fonction donne la taille d'un **pointeur** (8 octets),
pas celle du tableau. L'information de longueur est perdue.

### La solution : passer la taille

```c
void afficher(int tab[], int n) {       // ou int *tab, c'est identique
    for (int i = 0; i < n; i++) {
        printf("%d ", tab[i]);
    }
}

int main(void) {
    int notes[5] = {12, 15, 8, 17, 10};
    afficher(notes, 5);                  // on transmet la taille à la main
    return 0;
}
```

**C'est la convention en C, sans exception.** Toute fonction qui reçoit un
tableau reçoit aussi sa taille. Regarde n'importe quelle fonction de la
bibliothèque standard : `memcpy(dst, src, n)`, `fread(ptr, taille, n, f)`.

### Conséquence : la fonction peut modifier ton tableau

Puisque c'est l'adresse qui est passée, la fonction travaille sur **l'original**,
pas sur une copie.

```c
void doubler(int tab[], int n) {
    for (int i = 0; i < n; i++) {
        tab[i] *= 2;
    }
}

int notes[3] = {1, 2, 3};
doubler(notes, 3);
// notes vaut maintenant {2, 4, 6} — modifié pour de bon
```

C'est la première fois que tu vois une fonction modifier une variable de
l'appelant. Rappelle-toi la leçon sur les fonctions : les paramètres sont des
**copies**. Ici, ce qui est copié c'est l'adresse — et une copie d'adresse
pointe toujours vers le même endroit.

Pour interdire la modification :

```c
void afficher(const int tab[], int n) { ... }   // le compilateur refusera tab[i] = ...
```

**Mets `const` dès que la fonction ne doit que lire.** C'est une documentation
vérifiée par le compilateur.

---

## 7. Les chaînes de caractères

Une chaîne en C est un **tableau de `char` terminé par `'\0'`**. Il n'existe
pas de type `string`.

```c
char nom[] = "Yann";
```

En mémoire, 5 octets :

```
index:    0     1     2     3     4
        ['Y'] ['a'] ['n'] ['n'] ['\0']
         89    97   110   110    0
```

Le `'\0'` (octet de valeur zéro) marque la fin. **Aucune longueur n'est stockée
ailleurs.** Toutes les fonctions de chaînes avancent jusqu'à rencontrer ce zéro.

D'où :

```c
char nom[] = "Yann";
sizeof(nom)    // 5  — la place occupée, '\0' compris
strlen(nom)    // 4  — les lettres utiles, sans le '\0'
```

Deux questions différentes, deux réponses différentes.

### Les deux formes, et leur différence capitale

```c
char  a[] = "Yann";     // un TABLEAU : 5 octets copiés, modifiables
char *b   = "Yann";     // un POINTEUR vers un littéral en lecture seule
```

```c
a[0] = 'J';    // OK
b[0] = 'J';    // SEGFAULT à l'exécution
```

**Pourquoi.** Le littéral `"Yann"` est stocké dans le segment `text` de
l'exécutable, en lecture seule (revois la carte mémoire). Avec `a[]`, il est
**copié** dans un tableau à toi. Avec `*b`, tu ne fais que pointer dessus.

**Réflexe :** si tu déclares un littéral que tu ne modifieras pas, écris
`const char *b = "Yann";` — le compilateur t'arrêtera à la compilation plutôt
qu'au segfault.

---

## 8. Les fonctions de `<string.h>`

```c
#include <string.h>

strlen(s)              // longueur, sans le '\0'
strcmp(a, b)           // 0 si égales, <0 si a<b, >0 si a>b
strcpy(dst, src)       // copie src dans dst  — DANGEREUX
strcat(dst, src)       // concatène           — DANGEREUX
strchr(s, 'a')         // pointeur vers la 1re occurrence, ou NULL
```

### Comparer : jamais avec `==`

```c
char a[] = "chat";
char b[] = "chat";

if (a == b)              { ... }   // FAUX : compare deux ADRESSES
if (strcmp(a, b) == 0)   { ... }   // correct : compare le contenu
```

`a` et `b` sont deux tableaux distincts, à deux adresses différentes.
`a == b` est toujours faux. C'est une erreur si fréquente qu'elle vaut d'être
retenue à part.

Note aussi que `strcmp` renvoie **0 pour l'égalité** — l'inverse de l'intuition.

### Pourquoi `strcpy` est dangereux

```c
char petit[5];
strcpy(petit, "une chaine beaucoup trop longue");   // écrit hors du tableau
```

`strcpy` copie jusqu'au `'\0'` de la source, **sans jamais regarder la taille de
la destination**. Si la source est plus longue, elle déborde.

C'est le débordement de tampon de la section 4, en version bibliothèque
standard. Les variantes bornées :

```c
snprintf(dst, sizeof(dst), "%s", src);   // le plus sûr et portable
strncpy(dst, src, sizeof(dst) - 1);      // attention : peut ne pas terminer par '\0'
dst[sizeof(dst) - 1] = '\0';             // donc on le force
```

**Préfère `snprintf`.** Il borne toujours et termine toujours par `'\0'`.

---

## 9. Lire une chaîne au clavier

```c
char nom[50];

scanf("%s", nom);            // MAUVAIS : aucune borne, et s'arrête à l'espace
scanf("%49s", nom);          // mieux : borné
fgets(nom, sizeof(nom), stdin);   // le bon réflexe
```

`fgets` lit une ligne entière, bornée par la taille donnée. Un détail : il
**garde le `\n`** final. Pour l'enlever :

```c
nom[strcspn(nom, "\n")] = '\0';
```

Note l'absence de `&` devant `nom` avec `scanf("%s", nom)` — alors que tu écris
`scanf("%d", &age)`. Raison : un tableau se dégrade déjà en adresse. Pas besoin
de prendre l'adresse d'une adresse.

---

## 10. Tableaux à deux dimensions

```c
int grille[3][4] = {
    {1,  2,  3,  4},
    {5,  6,  7,  8},
    {9, 10, 11, 12}
};

grille[1][2]     // 7
```

**Ils sont contigus, pas imbriqués.** En mémoire, les 12 entiers se suivent
ligne après ligne :

```
1 2 3 4 5 6 7 8 9 10 11 12
└─ligne 0─┘ └─ligne 1─┘ └─ligne 2─┘
```

L'adresse de `grille[i][j]` vaut `début + (i × 4 + j) × sizeof(int)`.

**Conséquence pratique :** parcourir ligne par ligne est plus rapide que
colonne par colonne, parce que tu suis l'ordre physique de la mémoire et que
le cache du processeur charge les octets voisins d'avance. Sur de grosses
matrices, l'écart atteint un facteur 10. Tu le mesureras dans le module algo.

---

## 11. Ce que ça t'apprend de la machine

**L'indexation est une arithmétique d'adresse.** `tab[i]` n'est pas une
opération magique : c'est `*(tab + i)`. Les crochets sont du sucre syntaxique.
Cette équivalence est le pont vers la leçon suivante.

**Aucune sécurité n'est offerte.** Python lève `IndexError`, Java lève
`ArrayIndexOutOfBoundsException` — ils stockent la longueur et la vérifient à
chaque accès. Ça coûte de la mémoire et du temps. Le C ne paie pas ce coût, et
te laisse la responsabilité. **Voilà le marché du C, énoncé clairement.**

**La contiguïté est ce qui rend rapide.** Le processeur ne lit pas la RAM octet
par octet : il charge des **lignes de cache** de 64 octets. Un tableau parcouru
dans l'ordre exploite ça parfaitement. C'est pourquoi, à complexité algorithmique
égale, un tableau bat souvent une structure chaînée.

**Le `'\0'` est une convention, pas une garantie.** Rien dans le matériel
n'impose qu'une chaîne se termine. Si le zéro manque, `strlen` continue de lire
la mémoire suivante jusqu'à en trouver un — d'où des longueurs aberrantes et des
segfaults. Les langages modernes stockent la longueur explicitement, précisément
pour éliminer cette classe de bugs.

---

## 12. Exercices

1. Écris `int somme(const int tab[], int n)` et `double moyenne(const int tab[], int n)`.
   Teste sur `{12, 15, 8, 17, 10}`.

2. Écris `int maximum(const int tab[], int n)` qui renvoie la plus grande valeur.
   Puis une variante qui renvoie **l'indice** du maximum.

3. Écris `void inverser(int tab[], int n)` qui inverse le tableau sur place
   (sans tableau auxiliaire). Deux indices qui se rapprochent.

4. Reproduis le débordement de la section 4. Lance-le d'abord sans, puis avec
   `-fsanitize=address`. Compare les deux sorties.

5. Réimplémente `strlen` :
   `size_t ma_strlen(const char *s)`. Une boucle jusqu'au `'\0'`.

6. Réimplémente `strcmp` : parcours les deux chaînes en parallèle, renvoie la
   différence au premier caractère qui diffère.

7. Écris un programme qui lit une ligne avec `fgets`, enlève le `\n`, et affiche
   la chaîne à l'envers.

8. Écris `int compter_voyelles(const char *s)`.

9. Reprends ta `calculator.c` : ajoute une lecture de l'opération au clavier
   (`fgets` + un `switch` sur le caractère).

---

## Mémo

```c
int tab[5] = {0};                       // toujours initialiser
int n = sizeof(tab) / sizeof(tab[0]);   // ne marche PAS dans une fonction

for (int i = 0; i < n; i++)             // i < n, jamais i <= n

void f(const int tab[], int n)          // toujours passer la taille
                                        // const si lecture seule

char  a[] = "texte";        // tableau, modifiable, 6 octets
const char *b = "texte";    // pointeur vers littéral, lecture seule

sizeof(a)    // 6 — avec le '\0'
strlen(a)    // 5 — sans

strcmp(a, b) == 0           // égalité de chaînes — JAMAIS a == b
snprintf(dst, sizeof(dst), "%s", src)   // copie bornée
fgets(buf, sizeof(buf), stdin)          // lecture bornée
buf[strcspn(buf, "\n")] = '\0';         // enlever le \n
```

```sh
gcc -Wall -Wextra -g -fsanitize=address prog.c -o prog
```

**Les trois réflexes :**

1. Une fonction qui reçoit un tableau reçoit aussi sa taille
2. `strcmp`, jamais `==`, pour comparer des chaînes
3. Aucune borne n'est vérifiée — c'est à toi de le faire
