# Compiler, exécuter, déboguer

Ce qui se passe entre le fichier `.c` que tu écris et le programme qui tourne.

Suite de [01_pourquoi_le_c.md](01_pourquoi_le_c.md).

---

## 1. Compiler

```sh
gcc -Wall -Wextra -g fichier.c -o programme
```

| Morceau | Rôle |
|---|---|
| `gcc` | Le compilateur : traduit ton C en code machine |
| `-Wall` | *Warnings all* — active les avertissements courants |
| `-Wextra` | En active d'autres, que `-Wall` laisse passer |
| `-g` | Garde les infos de débogage (indispensable pour lldb) |
| `fichier.c` | Le fichier source, ton texte |
| `-o programme` | *Output* — le nom de l'exécutable à produire |

Sans `-o`, gcc produit un fichier nommé `a.out`, par tradition.

**gcc ne parle que s'il a quelque chose à redire.** Le silence est le succès.

### Les quatre étapes cachées

```
fichier.c
     ↓  préprocesseur    →  colle stdio.h à la place du #include
     ↓  compilation      →  traduit le C en assembleur
     ↓  assemblage       →  traduit l'assembleur en code machine (.o)
     ↓  édition de liens →  raccroche le code réel de printf
programme                →  exécutable
```

C'est à la dernière étape que la promesse du header est tenue.

Pour voir le résultat du préprocesseur seul :

```sh
gcc -E fichier.c | wc -l
```

Six lignes deviennent des centaines : c'est `stdio.h` déversé à la place de la directive.

---

## 2. Exécuter

```sh
./programme
```

### Pourquoi le `./`

Le shell distingue deux cas selon la **présence d'un `/`** dans ce que tu tapes :

| Ce que tu tapes | Interprétation |
|---|---|
| `programme` | Un **nom de commande** → cherche dans `PATH` |
| `./programme` | Un **chemin** → va directement à cet endroit |
| `temp/programme` | Un **chemin** → va directement à cet endroit |

Le déclencheur n'est pas le `./` en particulier, mais le `/` quelque part dans la chaîne.
`.` désigne le répertoire courant : `./programme` sert juste à **fabriquer un `/`**.
Dans `temp/programme`, le `/` est déjà là — le `./` serait redondant.

**Pourquoi cette règle :** si le shell cherchait d'abord dans le répertoire courant,
un fichier malveillant nommé `ls` déposé dans un dossier s'exécuterait à ta place.
Un nom nu ne peut venir que de `PATH`, une liste de répertoires système contrôlés.

```sh
echo $PATH      # la liste — le répertoire courant n'y est jamais
which gcc       # /usr/bin/gcc — d'où vient une commande trouvée
```

### Chemin absolu ou relatif

| Écriture | Signification |
|---|---|
| `/temp/bug.c` | Depuis la **racine du disque** — chemin absolu |
| `temp/bug.c` | Depuis le **répertoire courant** — chemin relatif |

Un `/` en première position veut dire « pars du tout début du système de fichiers ».
`pwd` te dit où tu es.

---

## 3. Enchaîner avec `&&`

```sh
gcc -Wall -Wextra -g fichier.c -o programme && ./programme
```

`&&` signifie : **n'exécute la seconde commande que si la première a renvoyé `0`.**

C'est le `return 0;` de ton programme, vu de l'autre côté. gcc renvoie `0` s'il a
compilé sans erreur, autre chose sinon.

L'intérêt est concret : si la compilation échoue, le programme ne se lance pas.
Sans le `&&`, tu exécuterais l'**ancien** binaire — celui d'avant ta modification —
et tu chercherais longtemps pourquoi ton correctif ne change rien.

```sh
echo $?     # le code de sortie de la dernière commande
```

---

## 4. Comment l'ordinateur exécute réellement le programme

Ce qui se passe entre « Entrée » et l'affichage.

```
Tu tapes « temp/bug » + Entrée
        ↓
[1] Le shell analyse la ligne
        ↓
[2] Le shell se dédouble (fork)
        ↓
[3] Le double se remplace par ton programme (exec)
        ↓
[4] Le noyau lit le fichier et le charge en mémoire
        ↓
[5] Le programme démarre → main() → printf → return 0
        ↓
[6] Le shell récupère le code de sortie et te rend la main
```

### [1] Analyse

Le shell découpe la ligne, voit un `/`, en déduit un chemin, et le combine avec le
répertoire courant pour obtenir le chemin absolu. Puis il vérifie que le fichier
existe et que tu as le droit de l'exécuter :

```sh
ls -l temp/bug      # -rwxr-xr-x — les x sont le droit d'exécution
```

Sans ces `x` : `Permission denied`. gcc les pose en créant le fichier.

### [2] `fork` — le shell se clone

Le shell ne lance pas ton programme directement : il **se clone lui-même** via
l'appel système `fork()`. Le noyau crée une copie quasi identique du processus.

Il y a maintenant deux processus :

- **le parent** — ton shell, qui va attendre
- **l'enfant** — le clone, qui va se transformer

**Pourquoi ce détour :** le shell doit survivre. S'il se transformait lui-même en
ton programme, il disparaîtrait à la fin de celui-ci et tu perdrais ton terminal.
Le clone est sacrifiable.

### [3] `exec` — le clone se remplace

L'enfant appelle `execve()` avec le chemin de ton programme. Le mot juste est
**remplacement**, pas lancement : le noyau vide le processus de tout son contenu
(le code du shell, ses variables, sa mémoire) et met ton programme à la place.

Le processus garde son identifiant (PID), mais son contenu est intégralement
substitué. La même personne, qui change entièrement de vêtements et de mission
sans changer de nom.

### [4] Chargement par le noyau

Le noyau ouvre le fichier et lit son **en-tête**, qui identifie le format.
Sur Mac Apple Silicon : Mach-O, arm64. Sous Linux : ELF.

```sh
file temp/bug       # affiche le format
```

Il y trouve le plan du programme et met en place sa mémoire en zones distinctes :

| Zone | Contenu |
|---|---|
| **Code** (*text*) | Les instructions machine, en lecture seule |
| **Données** | Les variables globales, les chaînes littérales |
| **Pile** (*stack*) | Les variables locales, les appels de fonctions |
| **Tas** (*heap*) | La mémoire allouée dynamiquement — vide au départ |

Cette carte mémoire est le cœur de ce que le C rend visible. À revoir avec les
pointeurs, puis en profondeur dans `dev_OS/`.

Le noyau charge aussi les bibliothèques dont le programme dépend — dont la
bibliothèque C, qui contient le vrai code de `printf` :

```sh
otool -L temp/bug   # /usr/lib/libSystem.B.dylib — d'où vient printf
```

C'est la promesse de `#include <stdio.h>` tenue au tout dernier moment.

### [5] Exécution

Le noyau place le pointeur d'instruction du processeur au point d'entrée et laisse courir.

**`main` n'est pas le premier code exécuté.** Une routine d'initialisation de la
bibliothèque C tourne d'abord : elle prépare les entrées/sorties, met en place les
arguments, initialise l'environnement. *Ensuite* elle appelle `main`.

`printf` écrit sur la **sortie standard**, un canal que le shell avait branché sur
ton terminal avant le `fork`.

### [6] Retour

Le `0` de `return 0` remonte à la routine d'initialisation, qui appelle `exit(0)`.
Le noyau termine le processus et **conserve ce code de sortie**.

Pendant ce temps, le shell parent était bloqué dans `wait()` — « préviens-moi quand
mon enfant se termine ». Le noyau le réveille, lui transmet le `0`, que le shell
range dans `$?` avant de réafficher le prompt.

### Le schéma complet

```
   SHELL (zsh)
      │
      │ tu tapes « temp/bug »
      │ résolution du chemin, vérification des droits
      │
      ├─── fork() ──────────────► CLONE du shell
      │                              │
      │ wait()                       │ execve("temp/bug")
      │ (bloqué,                     │
      │  en attente)                 ▼
      │                          Le noyau vide le clone,
      │                          lit l'en-tête Mach-O,
      │                          met en place code/données/pile/tas,
      │                          charge libSystem (printf)
      │                              │
      │                              ▼
      │                          init de la bibliothèque C
      │                              │
      │                              ▼
      │                          main() → moyenne() → printf()
      │                              │
      │                          return 0 → exit(0)
      │                              │
      ◄──── code de sortie 0 ────────┘
      │
      │ $? = 0
      ▼
   prompt réaffiché
```

### À retenir

**Le shell n'exécute rien lui-même.** Il sait faire trois choses : se dédoubler,
demander au noyau de remplacer le double, attendre le résultat. Tout le travail
réel est fait par le noyau.

**`fork` puis `exec` est le mécanisme universel sous Unix.** Chaque commande passe
par là. C'est aussi ainsi que ton navigateur ouvre un onglet.

**Le code de sortie boucle la chaîne.** Ton `return 0` traverse tout ce circuit
pour finir dans `$?`.

---

## 5. Déboguer

**Un bug** = un écart entre ce que ton programme fait et ce que tu croyais qu'il
faisait. **Déboguer** = trouver où et pourquoi.

Le mot vient d'un vrai insecte, un papillon coincé dans un relais du calculateur
Harvard Mark II en 1947, scotché au carnet de bord avec la mention
*« First actual case of bug being found »*.

### Trois familles de bugs

**Erreurs de compilation** — le programme ne se construit pas. gcc te dit où.
Les plus faciles. *Corrige la première erreur et recompile* : une seule erreur en
cascade en provoque souvent dix-neuf autres qui disparaîtront d'elles-mêmes.

**Erreurs de logique** — ça compile, mais c'est faux. Le compilateur ne peut rien :
il vérifie la grammaire, pas ton intention.

**Erreurs de mémoire** — `Segmentation fault`. Tu as touché une zone qui n'est pas
à toi. Le bug emblématique du C, celui qu'aucun autre langage grand public ne laisse
commettre. À découvrir avec les pointeurs.

### Outil 1 — `-Wall -Wextra`

Ta première défense. Ces options ne débusquent pas les bugs après coup,
**elles les empêchent d'exister**.

```c
int x;
printf("%d\n", x);   // x n'a jamais reçu de valeur
```

Sans les options, gcc compile sans broncher et le programme affiche n'importe quoi.
Avec, il te prévient.

**Un avertissement est un bug qui n'a pas encore eu lieu.** Ne jamais les ignorer.

### Outil 2 — `printf`

Le débogage du pauvre, et il marche. La technique la plus utilisée au monde,
professionnels compris.

```c
printf("DEBUG: a=%d b=%d\n", a, b);
```

Deux précautions : **toujours mettre `\n`** (sinon la sortie peut rester en mémoire
tampon et ne jamais s'afficher si le programme plante juste après), et **retirer les
témoins** une fois le bug corrigé.

### Outil 3 — `lldb`

C'est ici que le `-g` prend son sens. Sans lui, l'exécutable ne contient que du code
machine, sans lien avec ton source. Avec, tu peux **arrêter le programme en pleine
exécution** et inspecter son état.

`lldb` sur macOS ; `gdb` est l'équivalent GNU, plus courant sous Linux.

---

## 6. Séance lldb — un cas réel

Le programme d'exercice :

```c
#include <stdio.h>

int moyenne(int a, int b) {
    return a + b / 2;        // ← le bug est ici
}

int main(void) {
    int x = 10;
    int y = 20;
    printf("Moyenne : %d\n", moyenne(x, y));
    return 0;
}
```

Il affiche `20` au lieu de `15`. **Il compile sans le moindre avertissement** — c'est
du C parfaitement valide. Ni plantage, ni message d'erreur : le programme fait
exactement ce qu'on lui a demandé. C'est ce qu'on lui a demandé qui est faux.

### La séance

```sh
lldb temp/bug
```

```
(lldb) b bug.c:4              # point d'arrêt sur la ligne du calcul
(lldb) r                      # lance — le programme se fige ligne 4
(lldb) p a                    # (int) $0 = 10
(lldb) p b                    # (int) $1 = 20
(lldb) p a + b / 2            # 20  ← l'expression écrite
(lldb) p (a + b) / 2          # 15  ← l'expression voulue
(lldb) p b / 2                # 10  ← la preuve : b est divisé seul
(lldb) c                      # laisse le programme finir
(lldb) q                      # quitter (y pour confirmer)
```

Ce que lldb répond au point d'arrêt :

```
Breakpoint 1: where = bug`moyenne + 12 at bug.c:4:12, address = 0x000000010000046c
```

- `bug\`moyenne + 12` — dans l'exécutable `bug`, fonction `moyenne`, 12 octets après son début
- `at bug.c:4:12` — fichier, ligne, colonne : **c'est le `-g` qui permet cette correspondance**
- `address = ...` — l'adresse mémoire de l'instruction

**La cause :** `/` est prioritaire sur `+`. La machine calcule `10 + (20/2)` et non
`(10+20)/2`. La parenthèse manque.

```c
return (a + b) / 2;     // corrigé → 15
```

### Ce que la séance a démontré

1. **Un résultat suspect** — `20`, alors qu'on attendait `15`
2. **Une hypothèse** — soit les entrées sont fausses, soit le calcul l'est
3. **Un test** — `p a`, `p b` : les entrées sont bonnes, hypothèse éliminée
4. **Un second test** — les deux expressions côte à côte : le calcul est en cause
5. **La cause** — priorité des opérateurs

L'intérêt de lldb sur les `printf` : tu **poses des questions au programme pendant
qu'il tourne**, sans recompiler à chaque fois.

### Commandes essentielles

| Commande | Abrégé | Effet |
|---|---|---|
| `breakpoint set -f fichier.c -l 12` | `b fichier.c:12` | Pose un point d'arrêt |
| `run` | `r` | Lance le programme |
| `next` | `n` | Ligne suivante (sans entrer dans les fonctions) |
| `step` | `s` | Ligne suivante (en entrant dans les fonctions) |
| `print var` | `p var` | Affiche une variable ou évalue une expression |
| `continue` | `c` | Reprend jusqu'au prochain arrêt |
| `bt` | | Pile d'appels — qui a appelé qui |
| `quit` | `q` | Sortir (`y` pour confirmer) |

`bt` après un `Segmentation fault` est la commande qui économise le plus de temps :
elle dit à quelle ligne le programme est mort, et par quel chemin il y est arrivé.

**Attention à l'espace :** `p a`, pas `pa`. `p` est la commande, `a` son argument.

**Sortir :** `q` puis `y`. Ou `Ctrl+D`. `Ctrl+C` n'interrompt pas lldb mais le
*programme* en cours — utile face à une boucle infinie, suivi de `bt`.

### Dans VSCode

Avec l'extension **C/C++** de Microsoft : clic dans la marge pour poser un point
d'arrêt, F5 pour lancer, les variables s'affichent dans un panneau latéral.
Même mécanisme, interface plus confortable. Le `-g` reste indispensable.

---

## 7. La méthode, qui compte plus que les outils

**Reproduire.** Un bug qu'on ne sait pas déclencher à volonté ne se corrige pas.

**Réduire.** Enlève tout ce qui n'est pas nécessaire pour le reproduire. Souvent,
le bug devient évident pendant cette réduction.

**Localiser par dichotomie.** Le bug est entre le début et la fin. Vérifie au milieu :
si l'état est bon, il est dans la seconde moitié ; sinon dans la première. Recommence.
Même logique qu'une recherche dichotomique — cf. `projets_to_learn_algo/`.

**Formuler une hypothèse et la tester.** « Je crois que `b` vaut 20 ici. » Puis vérifie.
Ne modifie pas le code au hasard en espérant que ça passe.

**Vérifier ses certitudes.** Le bug est presque toujours dans ce dont tu étais sûr.
Si tu ne le trouves pas, c'est qu'une de tes convictions est fausse.

**Expliquer à voix haute.** Le *rubber duck debugging* : raconter son code ligne par
ligne à un canard en plastique. En verbalisant, on entend soi-même l'incohérence.

---

## Erreurs rencontrées

| Message | Cause | Remède |
|---|---|---|
| `no such file or directory: '/temp/bug.c'` | Le `/` initial = racine du disque | Enlever le `/` : `temp/bug.c` |
| `command not found` | Nom sans `/` → cherché dans `PATH` | Ajouter `./` |
| `'pa' is not a valid command` | Espace oublié dans lldb | `p a`, pas `pa` |
| `gcc: command not found` | Outils non installés (macOS) | `xcode-select --install` |
| `Permission denied` | Pas de droit d'exécution | `chmod +x fichier` |
| `undefined reference to '...'` | Déclaration vue, code introuvable | Lier la bibliothèque (`-lm` pour `math.h`) |

---

## Mémo

```sh
pwd                                              # où suis-je
gcc -Wall -Wextra -g src.c -o prog && ./prog     # compiler et lancer
echo $?                                          # code de sortie
lldb prog                                        # déboguer
file prog                                        # format de l'exécutable
otool -L prog                                    # bibliothèques liées
ls -l prog                                       # permissions
```
