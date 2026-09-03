# Pourquoi commencer par le C

## Le C ne cache rien

La plupart des langages modernes sont conçus pour vous **épargner** la machine.
Python, JavaScript, Java gèrent la mémoire à votre place, devinent les types,
nettoient derrière vous. C'est confortable, et c'est le but.

Le C fait l'inverse. Il vous met face à ce qui se passe réellement : où sont vos
données, combien d'octets elles occupent, qui les libère. Rien n'est automatique,
donc rien n'est invisible.

C'est inconfortable au début. C'est aussi exactement pourquoi on commence par lui :
**on ne peut pas comprendre ce qu'un langage fait pour vous si on n'a jamais eu à
le faire soi-même.**

## Il est sous tout le reste

Le C n'est pas un langage parmi d'autres, c'est le socle sur lequel le reste est bâti :

- **Linux, Windows, macOS** — leurs noyaux sont en C
- **Python** — l'interpréteur CPython est écrit en C
- **Les bases de données** — PostgreSQL, SQLite, Redis : en C
- **Les navigateurs** — leurs moteurs, en C et C++
- **L'embarqué** — de la machine à laver au satellite

Quand vous écrivez `print("bonjour")` en Python, l'interpréteur finit par appeler
du C. Apprendre le C, c'est descendre d'un étage et voir sur quoi repose le plancher.

## Ce qu'il vous apprend, et qui ne s'oublie pas

Trois choses que le C enseigne mieux que n'importe quel autre langage :

**La mémoire est un lieu.** Vos variables sont quelque part, à une adresse. Les
pointeurs — la grande difficulté du C — ne sont que la manipulation explicite de
ces adresses. Une fois compris, vous comprenez aussi pourquoi Python est plus lent,
pourquoi Rust existe, ce qu'est une fuite mémoire.

**Un programme, ça se construit.** Le C vous fait traverser toutes les étapes :
préprocesseur, compilation, assemblage, édition de liens. Là où Python vous dit
juste « lance le fichier », le C vous montre la chaîne complète.

**Les types ont un coût.** Un `int` fait 4 octets, un `char` en fait 1. Ce n'est
pas une abstraction, c'est de la place réelle en mémoire.

## Il ouvre les quatre portes de ce projet

Le choix n'est pas arbitraire par rapport à vos objectifs :

| Spécialisation | Rapport au C |
|---|---|
| **Embarqué** | C'est le langage du métier, directement |
| **Systèmes d'exploitation** | Noyaux, drivers, appels système : en C |
| **Web** | Comprendre mémoire et réseau sépare celui qui code de celui qui fait tourner |
| **Mobile** | Kotlin et Swift reprennent des concepts C ; les couches basses sont en C |

Les deux premières branches **exigent** le C. Les deux autres en tirent une
compréhension que peu de développeurs ont.

## Une objection honnête

Le C est un mauvais premier langage pour **produire vite**. Afficher une page web,
tracer un graphique, appeler une API : en Python c'est cinq lignes, en C c'est
une journée.

Mais l'objectif ici n'est pas de produire vite. C'est de **comprendre**. Et cette
compréhension se transporte : après le C, apprendre Python ou JavaScript prend
quelques semaines. L'inverse est faux — on peut faire du Python pendant des années
sans jamais comprendre ce qu'est une adresse mémoire.

## Une remarque d'usage

Le C moderne s'écrit avec des garde-fous. Deux options à prendre comme réflexe
dès maintenant :

```sh
gcc -Wall -Wextra -g fichier.c -o programme
```

`-Wall -Wextra` activent les avertissements du compilateur. Il vous signalera des
erreurs que le langage, lui, laisserait passer silencieusement. Le C ne vous
protège pas — le compilateur, un peu, si vous le lui demandez.

---

# Décortiquons

Voici le programme. Six lignes, et pourtant tout le mécanisme du C y est déjà.

```c
#include <stdio.h>

int main() {
  printf("Goodbye, World!");
  return 0;
}
```

## Vue d'ensemble

Trois choses s'y passent :

1. On **annonce** au compilateur qu'une fonction d'affichage existe
2. On **définit** le point d'entrée du programme
3. On **affiche** un texte, puis on **rend la main** au système

## Ligne 1 — `#include <stdio.h>`

**On inclut le header `stdio.h`, qui déclare `printf`.**

Le `#` marque une directive de préprocesseur, traitée *avant* la compilation.
Elle colle le contenu de `stdio.h` à la place de la ligne.

Ce qui arrive alors en haut du fichier, c'est la déclaration de `printf` — son
nom, son type de retour, ses arguments :

```c
int printf(const char *format, ...);
```

Une annonce, pas du code. Le compilateur lit de haut en bas et doit connaître
`printf` **avant** de rencontrer votre appel plus bas. Le code réel arrive plus
tard, à l'édition de liens.

C'est déjà le C dans son principe : **rien n'est connu d'avance**. Le langage
lui-même est minuscule — une trentaine de mots-clés, aucune fonction. Tout le
reste vient de bibliothèques qu'il faut présenter explicitement.

## Ligne 3 — `int main() {`

Le point d'entrée. Quand vous lancez `./programme`, le système appelle cette
fonction. Un programme en a exactement une.

- `int` — le type de retour : un entier rendu au système
- `main` — le nom, imposé par la norme
- `()` — pas d'arguments
- `{` — ouvre le corps de la fonction

Notez le contraste avec la ligne 1 : `stdio.h` **déclarait** `printf` (une annonce,
sans code). Ici vous **définissez** `main` — il y a des accolades et du contenu.
Déclaration et définition, les deux faces du C, dans le même fichier.

*Petite réserve :* `()` signifie « je ne précise pas les paramètres », pas « aucun
paramètre ». La forme rigoureuse est `int main(void)`.

## Ligne 4 — `printf("Goodbye, World!");`

L'appel. Il est accepté parce que la déclaration venue de `stdio.h` est passée
juste au-dessus.

Les guillemets délimitent un littéral de chaîne. En mémoire :

```
'G' 'o' 'o' 'd' 'b' 'y' 'e' ',' ' ' 'W' 'o' 'r' 'l' 'd' '!' '\0'
```

Ce `'\0'` final marque la fin. Le C ne stocke aucune longueur à côté : `printf`
avance caractère par caractère et s'arrête sur le zéro. Voilà exactement le genre
de détail que les autres langages vous cachent — et la source d'un grand classique
du bug en C, quand ce zéro manque.

Le `;` termine l'instruction : obligatoire.

`printf` = *print formatted*. Ici la chaîne sort telle quelle, mais elle peut
porter des marqueurs : `printf("j'ai %d ans\n", 25)`.

## Ligne 5 — `return 0;`

Termine `main` et renvoie `0`, cohérent avec le `int` annoncé ligne 3.

C'est le **code de sortie**, récupéré par le système. Convention Unix : `0` =
succès, non nul = erreur.

Ce n'est pas décoratif. C'est ce qui permet d'enchaîner des commandes :

```sh
gcc prog.c -o prog && ./prog    # ./prog ne tourne que si gcc a renvoyé 0
```

```sh
./programme
echo $?     # affiche 0
```

## Ligne 6 — `}`

Ferme le bloc. Fin du programme.

## Le fil, de bout en bout

```
#include <stdio.h>   →  le header déclare printf          (une promesse)
                             ↓
printf(...)          →  le compilateur accepte l'appel
                             ↓
édition de liens     →  le code réel de printf est lié    (la promesse tenue)
                             ↓
./programme          →  le système appelle main
                             ↓
return 0             →  main rend 0 : tout s'est bien passé
```

Une même idée revient deux fois : **annoncer avant de fournir**. `printf` est
déclaré bien avant que son code n'arrive. `main` est défini bien avant que le
système ne l'appelle.

C'est la logique du C en entier, tenue dans six lignes.

---

## Un détail à corriger

La chaîne ne se termine pas par `\n`. Sans ce saut de ligne, le prompt du shell
se colle à la sortie :

```
Goodbye, World!Hugotestas@macbook learn_dev %
```

## À faire maintenant

```sh
gcc -Wall -Wextra -g learn_C/hello_world.c -o learn_C/hello_world
./learn_C/hello_world
echo $?
```
