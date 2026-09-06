# Fichiers et appels système

La dernière leçon du module. Elle franchit la frontière entre ton processus et
le noyau — et ouvre directement `4.Learn_dev_OS`.

Suite de [09_structures.md](09_structures.md).

---

## 1. Deux couches

Il existe deux façons de lire un fichier en C, et comprendre leur différence est
tout l'objet de cette leçon.

```
ton code
   │
   ├─► fopen / fread / fprintf     ← couche C standard, BUFFÉRISÉE
   │        │
   │        ▼
   └─► open / read / write         ← couche système, APPELS SYSTÈME
            │
            ▼
         noyau
```

La couche haute est portable et pratique. La couche basse est ce qui se passe
réellement. On commence par la première, on descend ensuite.

---

## 2. `fopen` et `fclose`

```c
#include <stdio.h>

FILE *f = fopen("donnees.txt", "r");
if (f == NULL) {
    fprintf(stderr, "Impossible d'ouvrir le fichier\n");
    return 1;
}

// ... utilisation ...

fclose(f);
```

`fopen` renvoie `NULL` en cas d'échec — **toujours tester**, comme pour `malloc`.

### Les modes

| Mode | Effet | Si le fichier n'existe pas |
|---|---|---|
| `"r"` | lecture | échec |
| `"w"` | écriture — **vide le fichier** | le crée |
| `"a"` | ajout en fin | le crée |
| `"r+"` | lecture + écriture | échec |
| `"w+"` | lecture + écriture, vide | le crée |
| `"rb"` `"wb"` | idem, mode binaire | |

**Attention à `"w"`.** Il tronque le fichier à zéro immédiatement, avant même
que tu écrives quoi que ce soit. Une faute de frappe dans le nom peut effacer
un fichier existant.

Sur macOS et Linux, le `b` de `"rb"` ne change rien (les fichiers texte et
binaires sont identiques). Sous Windows il compte. Mets-le pour du binaire,
par habitude portable.

---

## 3. Lire un fichier texte

### Ligne par ligne — la méthode standard

```c
char ligne[256];

while (fgets(ligne, sizeof(ligne), f) != NULL) {
    printf("%s", ligne);        // pas de \n : fgets le garde déjà
}
```

`fgets` renvoie `NULL` en fin de fichier. Il garde le `\n` final, et borne
toujours la lecture à la taille donnée — c'est pourquoi on le préfère à
`scanf`.

Pour enlever le `\n` :

```c
ligne[strcspn(ligne, "\n")] = '\0';
```

### Lire des valeurs formatées

```c
int a, b;
while (fscanf(f, "%d %d", &a, &b) == 2) {     // tester le NOMBRE lu
    printf("%d + %d = %d\n", a, b, a + b);
}
```

`fscanf` renvoie le nombre de conversions réussies. **Toujours comparer à ce
que tu attends** — c'est la seule façon de détecter une ligne mal formée.

### Caractère par caractère

```c
int c;                              // int, pas char : EOF ne tient pas dans un char
while ((c = fgetc(f)) != EOF) {
    putchar(c);
}
```

**Pourquoi `int`.** `EOF` vaut `-1`. Si tu déclares `char c`, la valeur `255`
(un octet légitime) devient `-1` après conversion, et ta boucle s'arrête au
milieu d'un fichier binaire. Bug classique.

---

## 4. Écrire

```c
FILE *f = fopen("sortie.txt", "w");
if (f == NULL) { ... }

fprintf(f, "Nom : %s, age : %d\n", nom, age);   // comme printf, avec le FILE*
fputs("une ligne\n", f);
fputc('X', f);

fclose(f);
```

`fprintf` est identique à `printf`, avec le fichier en premier argument.

**`fclose` n'est pas optionnel.** Il vide le tampon (section 8), rend le
descripteur, et libère la structure `FILE`. Sans lui, tes données peuvent ne
jamais atteindre le disque.

---

## 5. Les trois flux standard

Ils sont ouverts automatiquement au démarrage de tout programme :

| Flux | Numéro | Rôle | Par défaut |
|---|---|---|---|
| `stdin` | 0 | entrée | le clavier |
| `stdout` | 1 | sortie normale | le terminal |
| `stderr` | 2 | messages d'erreur | le terminal |

```c
printf("bonjour\n");                    // équivaut à fprintf(stdout, ...)
fprintf(stderr, "Erreur : ...\n");      // les erreurs vont sur stderr
fgets(ligne, sizeof(ligne), stdin);     // lire l'entrée standard
```

**Pourquoi séparer `stdout` et `stderr` ?** Parce que le shell les redirige
indépendamment :

```sh
./prog > resultat.txt              # stdout dans le fichier, erreurs à l'écran
./prog 2> erreurs.txt              # l'inverse
./prog > resultat.txt 2>&1         # les deux dans le fichier
./prog | grep "toto"               # stdout dans le tube, erreurs visibles
```

**Règle :** les résultats sur `stdout`, tout le reste sur `stderr`. C'est ce qui
rend un programme utilisable dans un pipeline — et c'est la convention de tous
les outils Unix.

Le fait que les flux standard portent les numéros **0, 1, 2** n'est pas un
détail : ce sont de vrais descripteurs de fichier, comme on va le voir.

---

## 6. Arguments de la ligne de commande

```c
int main(int argc, char *argv[]) {
    printf("Nom du programme : %s\n", argv[0]);

    for (int i = 1; i < argc; i++) {
        printf("Argument %d : %s\n", i, argv[i]);
    }
    return 0;
}
```

- `argc` — nombre d'arguments, **`argv[0]` compris**
- `argv` — tableau de chaînes ; `argv[0]` est le nom du programme

```sh
./prog fichier.txt -v
# argc = 3
# argv[0] = "./prog"   argv[1] = "fichier.txt"   argv[2] = "-v"
```

Usage typique :

```c
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <fichier>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "%s : impossible d'ouvrir %s\n", argv[0], argv[1]);
        return 1;
    }
    ...
}
```

C'est le squelette de tous les outils en ligne de commande que tu utilises.

---

## 7. `errno` — savoir *pourquoi* ça a échoué

```c
#include <errno.h>
#include <string.h>

FILE *f = fopen("absent.txt", "r");
if (f == NULL) {
    fprintf(stderr, "Erreur : %s\n", strerror(errno));
    // "No such file or directory"

    perror("fopen");        // plus court : "fopen: No such file or directory"
    return 1;
}
```

`errno` est une variable globale que les fonctions système renseignent en cas
d'échec. Deux règles :

1. **Ne la lire qu'après un échec avéré.** Elle n'est pas remise à zéro en cas
   de succès — une valeur ancienne peut traîner.
2. **La lire immédiatement.** Tout appel de fonction intermédiaire peut l'écraser.

---

## 8. La bufférisation

Voici le point qui explique le mieux la différence entre les deux couches.

```c
printf("bonjour");     // n'atteint PAS l'écran tout de suite
```

La libc accumule les écritures dans un tampon en mémoire, et ne le vide que
lorsqu'il est plein, à `\n` sur un terminal, ou à `fclose`/fin du programme.

**Pourquoi.** Chaque écriture réelle est un appel système, et un appel système
coûte cher — il faut basculer en mode noyau. Écrire 1000 caractères un par un
ferait 1000 bascules. Le tampon les regroupe en une seule.

Trois modes :

| Mode | Vidé quand | Utilisé pour |
|---|---|---|
| par ligne | à chaque `\n` | terminal |
| par bloc | tampon plein (4 ko) | fichiers, tubes |
| aucun | immédiatement | `stderr` |

**`stderr` n'est pas bufférisé** — pour que les messages d'erreur sortent avant
un crash éventuel. C'est une raison de plus d'y mettre tes diagnostics.

### Le démontrer

```c
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("sans saut de ligne");
    sleep(3);                        // rien à l'écran pendant 3 secondes
    printf("\n");                    // ← c'est ici que tout sort
    return 0;
}
```

Pour forcer :

```c
fflush(stdout);
```

**Le piège pratique :** si ton programme crashe, les `printf` en attente dans le
tampon sont perdus. Tu déboguais avec des `printf` et le dernier ne s'affiche
pas ? Il était dans le tampon. Utilise `fprintf(stderr, ...)` pour déboguer —
ou `lldb`, qui est fait pour ça.

---

## 9. La couche système : `open`, `read`, `write`

On descend d'un étage. Ici, plus de `FILE *`, plus de tampon : chaque appel
franchit la frontière vers le noyau.

```c
#include <fcntl.h>
#include <unistd.h>

int fd = open("donnees.txt", O_RDONLY);
if (fd == -1) {
    perror("open");
    return 1;
}

char tampon[4096];
ssize_t n;

while ((n = read(fd, tampon, sizeof(tampon))) > 0) {
    write(STDOUT_FILENO, tampon, n);    // STDOUT_FILENO vaut 1
}

if (n == -1) {
    perror("read");
}

close(fd);
```

Tu viens d'écrire `cat`.

### Les différences avec la couche haute

| | `fopen` / `fread` | `open` / `read` |
|---|---|---|
| Type | `FILE *` (une structure) | `int` (un descripteur) |
| Tampon | oui, dans ton processus | non |
| Appels système | regroupés | un par appel |
| Échec | `NULL` | `-1` |
| Portabilité | norme C, partout | POSIX (Unix, macOS, Linux) |

### Le descripteur de fichier

**Un descripteur est un simple entier** : un index dans une table que le noyau
tient pour ton processus.

```
Table des descripteurs du processus 4217
┌────┬──────────────────────────┐
│ 0  │ → terminal (stdin)       │
│ 1  │ → terminal (stdout)      │
│ 2  │ → terminal (stderr)      │
│ 3  │ → donnees.txt            │  ← ce que open() vient de renvoyer
└────┴──────────────────────────┘
```

C'est pourquoi le premier `open` d'un programme renvoie **3** : 0, 1 et 2 sont
déjà pris.

Vérifie-le :

```c
int fd = open("donnees.txt", O_RDONLY);
printf("fd = %d\n", fd);      // 3
```

### « Tout est fichier »

Le principe fondateur d'Unix. La même table, la même interface `read`/`write`
servent pour :

- un fichier sur disque
- le terminal
- un **tube** entre deux processus (`|` du shell)
- une **socket** réseau
- un périphérique (`/dev/random`, un port série)

Une socket réseau se lit avec `read` exactement comme un fichier. **C'est pour
ça qu'un serveur web en C ressemble à un programme de manipulation de
fichiers** — et c'est ce qui rend le shell si composable.

---

## 10. Ce que ça t'apprend de la machine

### Un appel système, c'est quoi exactement

C'est le franchissement de la frontière entre ton programme et le noyau.

```
MODE UTILISATEUR                          MODE NOYAU
────────────────                          ──────────
ton programme
   │  read(fd, buf, 4096)
   │
   ├──► instruction spéciale (svc sur ARM64, syscall sur x86)
   │         │
   │         └──► le CPU BASCULE en mode privilégié
   │                     │
   │                     ├─ le noyau vérifie tes droits sur fd
   │                     ├─ il lit le disque (ou son cache)
   │                     ├─ il copie les octets dans ton buf
   │                     │
   │         ┌───────────┘
   │         ▼
   ◄──── retour en mode utilisateur
```

**Pourquoi cette barrière existe.** Ton processus n'a pas le droit de parler au
disque directement. S'il le pouvait, n'importe quel programme lirait les
fichiers de n'importe qui. Le processeur possède deux modes d'exécution, et
seul le mode noyau peut exécuter les instructions matérielles privilégiées.

**L'appel système est la seule porte.** Toute interaction avec le monde
extérieur — fichier, réseau, écran, clavier, autre processus — passe par là.
C'est la définition même d'un système d'exploitation : le gardien de cette
frontière.

**Et ça coûte cher.** Une bascule de mode, une vérification de droits, un
changement de contexte : quelques centaines à quelques milliers de cycles,
contre quelques cycles pour un appel de fonction ordinaire. D'où le tampon de
la section 8 — toute la couche `FILE *` existe pour **minimiser le nombre de
franchissements**.

### Le voir en vrai

```sh
# Linux (dans une VM)
strace ./prog

# macOS — nécessite sudo, et le SIP peut le bloquer
sudo dtruss ./prog
```

Tu verras la liste exacte : `openat`, `read`, `write`, `close`, `mmap`,
`exit_group`. C'est la conversation complète entre ton programme et le noyau.

Une expérience à faire : compare la trace de la version `fopen`/`fgetc` de ton
`cat` avec la version `open`/`read`. La première fait quelques `read` de 4096
octets ; la seconde en fait autant que tu as écrit d'appels. **Le tampon devient
visible.**

C'est le point de départ du module `4.Learn_dev_OS`.

---

## 11. Un programme complet : `wc` simplifié

```c
#include <stdio.h>
#include <ctype.h>

typedef struct {
    long lignes;
    long mots;
    long caracteres;
} Compteur;

static Compteur compter(FILE *f) {
    Compteur c = {0, 0, 0};
    int dans_un_mot = 0;
    int ch;

    while ((ch = fgetc(f)) != EOF) {
        c.caracteres++;

        if (ch == '\n') {
            c.lignes++;
        }

        if (isspace(ch)) {
            dans_un_mot = 0;
        } else if (!dans_un_mot) {
            dans_un_mot = 1;
            c.mots++;
        }
    }
    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <fichier>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        perror(argv[1]);
        return 1;
    }

    Compteur c = compter(f);
    fclose(f);

    printf("%ld %ld %ld %s\n", c.lignes, c.mots, c.caracteres, argv[1]);
    return 0;
}
```

Vérifie contre le vrai :

```sh
./mon_wc fichier.txt
wc fichier.txt
```

Ce programme rassemble presque tout le module : `argc`/`argv`, `FILE *`,
gestion d'erreur avec `perror`, une structure comme valeur de retour, `static`
pour une fonction interne, et une machine à états sur deux lignes.

---

## 12. Exercices

**Couche standard**

1. Écris un programme qui affiche un fichier ligne par ligne, avec le numéro de
   ligne devant. Le nom du fichier vient de `argv[1]`.

2. Écris un programme qui compte les lignes d'un fichier. Compare avec
   `wc -l`.

3. Tape le `wc` de la section 11 et vérifie-le contre le vrai `wc`.

4. Écris un programme qui copie un fichier vers un autre
   (`./copie source destination`), avec toutes les vérifications d'erreur.

5. Écris un programme qui lit un fichier CSV simple `nom,age,moyenne` et
   remplit un tableau dynamique de structures `Etudiant` (leçons 08 et 09).

**Diagnostic et flux**

6. Ouvre un fichier inexistant et affiche l'erreur avec `perror`, puis avec
   `strerror(errno)`.

7. Reproduis l'expérience de bufférisation de la section 8. Puis remplace le
   `printf` par un `fprintf(stderr, ...)` et constate la différence.

8. Écris un programme qui affiche un message sur `stdout` et un autre sur
   `stderr`. Teste les quatre redirections de la section 5.

**Couche système**

9. Écris `cat` avec `open`/`read`/`write` (section 9). Affiche la valeur du
   descripteur et vérifie qu'elle vaut 3.

10. Écris deux versions du même programme de copie — une avec `fread`/`fwrite`,
    une avec `read`/`write` — et compare leur temps sur un gros fichier avec
    `time`. Explique l'écart.

11. Si tu as accès à une VM Linux : lance `strace` sur tes deux versions et
    compte les appels système de chacune.

---

## Mémo

```c
#include <stdio.h>

FILE *f = fopen("nom.txt", "r");        // "r" "w" "a" — attention, "w" TRONQUE
if (f == NULL) { perror("fopen"); return 1; }

fgets(ligne, sizeof(ligne), f)          // lecture bornée, garde le \n
fscanf(f, "%d", &n) == 1                // tester le nombre de conversions
int c = fgetc(f);                       // int, pas char (à cause de EOF)

fprintf(f, "...", ...);                 // écrire
fclose(f);                              // obligatoire : vide le tampon

fprintf(stderr, "...")                  // erreurs : jamais sur stdout
fflush(stdout);                         // forcer l'écriture

int main(int argc, char *argv[])        // argv[0] = nom du programme

#include <errno.h>
perror("contexte");                     // affiche "contexte: message"
```

```c
// couche système
#include <fcntl.h>
#include <unistd.h>

int fd = open("f.txt", O_RDONLY);       // -1 en cas d'échec
ssize_t n = read(fd, buf, sizeof(buf)); // 0 = fin, -1 = erreur
write(STDOUT_FILENO, buf, n);
close(fd);
```

```sh
./prog > out.txt 2> err.txt      # séparer les flux
strace ./prog                    # voir les appels système (Linux)
```

**Les quatre réflexes :**

1. Tester `fopen`, comme on teste `malloc`
2. Résultats sur `stdout`, diagnostics sur `stderr`
3. `fclose` toujours — sinon les données restent dans le tampon
4. `int c` pour `fgetc`, jamais `char`

---

## Fin du module

Tu as parcouru la chaîne complète : du fichier texte que tu écris jusqu'à
l'appel système qui parle au noyau.

Ce que tu sais maintenant, et qui ne s'oublie pas :

- un exécutable est **assemblé** à partir d'unités compilées séparément
- un type est une **taille** et une **convention de lecture** posées sur des octets
- la mémoire est un tableau d'octets numérotés, et un pointeur en est un numéro
- les adresses que tu vois sont **virtuelles** — la MMU et le noyau font l'illusion
- la pile est automatique et limitée, le tas est manuel et vaste
- un descripteur de fichier est un **entier** dans une table du noyau
- l'appel système est **la seule porte** vers le monde extérieur

**La suite :**

- `2.Learn_linux` — le shell, les processus, les permissions. Tu y retrouveras
  les descripteurs, les redirections et les drapeaux de bits, vus depuis
  l'autre côté.
- `3.projects_to_learn_algo` — listes chaînées, arbres, tables de hachage.
  Tout s'écrit avec les pointeurs et `malloc` de ce module.
- `4.Learn_dev_OS` — processus, mémoire virtuelle, ordonnancement. Il reprend
  exactement là où cette leçon s'arrête.
