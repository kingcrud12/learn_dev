# Naviguer et manipuler des fichiers

Les commandes que tu utiliseras tous les jours.

Suite de [01_pourquoi_linux.md](01_pourquoi_linux.md).

---

## 1. L'arborescence

Un système Unix range tout dans **un seul arbre**, dont la racine s'écrit `/`.
Pas de `C:` ni de `D:` : un disque externe apparaît comme un dossier de plus.

```
/                      la racine
├── Users/             les utilisateurs (macOS ; /home sur Linux)
│   └── Hugotestas/    ton dossier personnel  =  ~
│       └── learn_dev/
│           ├── learn_C/
│           └── learn_linux/
├── bin/  usr/bin/     les programmes (ls, gcc, grep...)
├── etc/               la configuration du système
└── tmp/               les fichiers temporaires, vidés au redémarrage
```

Le `/tmp` que tu as croisé en compilant `bug.c` est celui de cet arbre — d'où
l'erreur quand tu avais écrit `/temp/bug.c` : un dossier `temp` à la racine, qui
n'existe pas.

### Les raccourcis de chemin

| Écriture | Sens |
|---|---|
| `/` | La racine du disque |
| `~` | Ton dossier personnel (`/Users/Hugotestas`) |
| `.` | Le répertoire courant |
| `..` | Le répertoire parent |
| `-` | Le répertoire précédent (avec `cd`) |

### Absolu ou relatif

**Absolu** — commence par `/`, part de la racine. Fonctionne d'où que tu sois :

```sh
/Users/Hugotestas/learn_dev/learn_C/codes/hello_world.c
```

**Relatif** — part du répertoire courant :

```sh
codes/hello_world.c        # depuis learn_C/
../learn_linux/lessons     # remonte d'un cran, puis redescend
```

C'est la distinction qui explique le `./` de `./programme` : un chemin, même
minimal, plutôt qu'un nom de commande cherché dans `PATH`.

---

## 2. Se repérer et se déplacer

### `pwd` — où suis-je

```sh
pwd
# /Users/Hugotestas/learn_dev
```

*Print working directory.* Le premier réflexe quand quelque chose ne marche pas.

### `ls` — que contient ce dossier

```sh
ls                  # les fichiers visibles
ls -l               # format long : droits, taille, date
ls -a               # tout, y compris les fichiers cachés (commençant par .)
ls -la              # les deux combinés
ls -lh              # tailles lisibles (4.0K, 2.1M) au lieu d'octets
ls -lt              # trié par date, le plus récent en premier
ls learn_C/         # le contenu d'un autre dossier
```

Les options se combinent : `-l -a -h` s'écrit `-lah`.

Décoder une ligne de `ls -l` :

```
-rw-r--r--  1 Hugotestas  staff   350 Sep  3 17:34 variables_types.md
│└┬┘└┬┘└┬┘     │          │       │   │            │
│ │  │  │      │          │       │   │            └─ nom
│ │  │  │      │          │       │   └─ date de modification
│ │  │  │      │          │       └─ taille en octets
│ │  │  │      │          └─ groupe
│ │  │  │      └─ propriétaire
│ │  │  └─ droits des autres
│ │  └─ droits du groupe
│ └─ droits du propriétaire
└─ type : - fichier, d dossier, l lien
```

### `cd` — changer de dossier

```sh
cd learn_C          # descendre
cd ..               # remonter d'un cran
cd ../..            # remonter de deux
cd ~                # aller au dossier personnel
cd                  # idem, plus court
cd -                # revenir au dossier précédent
cd /usr/bin         # chemin absolu
```

**Astuce :** Tab complète les noms. Tape `cd lea` puis Tab.

### `tree` — voir l'arborescence

Pas installé par défaut sur macOS. L'équivalent immédiat :

```sh
find . -type d              # tous les dossiers
find . -not -path '*/.*'    # tout, en ignorant les fichiers cachés
```

---

## 3. Lire des fichiers

```sh
cat fichier.c              # affiche tout d'un coup
cat -n fichier.c           # avec les numéros de ligne
head fichier.c             # les 10 premières lignes
head -20 fichier.c         # les 20 premières
tail fichier.c             # les 10 dernières
tail -f journal.log        # suit le fichier en direct (Ctrl+C pour sortir)
less fichier.c             # navigation page par page (q pour sortir)
wc -l fichier.c            # compte les lignes
```

**Lequel choisir :** `cat` pour un petit fichier, `less` pour un gros, `head`/`tail`
pour jeter un œil, `tail -f` pour surveiller un journal qui grossit.

Dans `less` : flèches pour naviguer, `/mot` pour chercher, `n` pour l'occurrence
suivante, `q` pour sortir. Ce sont les mêmes touches que `man` — c'est le même
programme derrière.

---

## 4. Créer, copier, déplacer, supprimer

### Créer

```sh
mkdir mon_dossier              # un dossier
mkdir -p a/b/c                 # toute une arborescence d'un coup
touch fichier.txt              # un fichier vide
```

### Copier

```sh
cp source.c destination.c      # un fichier
cp source.c dossier/           # dans un dossier
cp -r dossier1 dossier2        # un dossier entier (-r = récursif)
```

### Déplacer et renommer

```sh
mv ancien.c nouveau.c          # renommer
mv fichier.c dossier/          # déplacer
mv dossier1 dossier2           # renommer un dossier
```

**`mv` fait les deux** : renommer, c'est déplacer vers un nom différent au même
endroit. C'est ce qui corrigerait `prensenting_myself.c` :

```sh
mv codes/prensenting_myself.c codes/presenting_myself.c
```

### Supprimer

```sh
rm fichier.txt                 # un fichier
rm -r dossier                  # un dossier et son contenu
rm -i fichier.txt              # demande confirmation
```

**Il n'y a pas de corbeille.** `rm` détruit immédiatement et définitivement.

Trois précautions qui valent d'être prises maintenant :

1. **`ls` avant `rm`** — vérifie ce que ton motif attrape avant de le détruire
2. **Jamais `rm -rf` par réflexe** — le `-f` supprime les garde-fous
3. **`rm -rf /`** efface le système entier. Ne le tape jamais, même pour rire.

Un espace mal placé transforme `rm -rf ./dossier` en `rm -rf / dossier`. La
différence est totale.

---

## 5. Les jokers

Le shell développe certains caractères **avant** de lancer la commande.

| Motif | Sens | Exemple |
|---|---|---|
| `*` | N'importe quelle suite de caractères | `*.c` → tous les fichiers C |
| `?` | Un seul caractère | `fichier?.c` → `fichier1.c`, `fichier2.c` |
| `[abc]` | Un caractère parmi | `[abc]*.c` |
| `[0-9]` | Un chiffre | `exo[0-9].c` |

```sh
ls *.c                    # tous les .c
ls codes/*.c              # tous les .c de codes/
rm *.o                    # tous les fichiers objets
cp *.c sauvegarde/        # copier tous les .c
```

**Vérifier avant de détruire** — remplace `rm` par `ls` et regarde :

```sh
ls *.o        # ce que rm *.o va supprimer
```

C'est le shell qui fait ce travail, pas la commande : `ls *.c` devient
`ls a.c b.c c.c` avant même que `ls` ne démarre.

---

## 6. Les permissions

Chaque fichier porte des droits pour trois catégories : le **propriétaire**, le
**groupe**, les **autres**.

```
-rwxr-xr-x
 └┬┘└┬┘└┬┘
  │  │  └── autres    : r-x  lire, exécuter
  │  └───── groupe    : r-x  lire, exécuter
  └──────── proprio   : rwx  lire, écrire, exécuter
```

| Lettre | Sur un fichier | Sur un dossier |
|---|---|---|
| `r` | Lire le contenu | Lister le contenu |
| `w` | Modifier | Créer/supprimer dedans |
| `x` | **Exécuter** | Entrer dedans |

Le `x` est celui qui te concerne directement : c'est lui qui permet `./programme`.
gcc le pose automatiquement sur les exécutables qu'il produit.

```sh
ls -l codes/hello_world      # -rwxr-xr-x  ← les x sont là
chmod +x script.sh           # ajouter le droit d'exécution
chmod -x fichier             # le retirer
```

Sans `x` : `Permission denied`.

### La notation numérique

Chaque droit vaut un nombre : `r`=4, `w`=2, `x`=1. On les additionne par catégorie.

```sh
chmod 755 fichier    # rwx r-x r-x  → un exécutable ou un dossier
chmod 644 fichier    # rw- r-- r--  → un fichier de données
chmod 700 fichier    # rwx --- ---  → privé
```

`755` se lit : propriétaire 7 (4+2+1 = tout), groupe 5 (4+1), autres 5 (4+1).

---

## 7. Historique et raccourcis

Le terminal garde tout ce que tu tapes.

```sh
history              # tout l'historique
!!                   # relance la dernière commande
!gcc                 # relance la dernière commande commençant par gcc
```

| Raccourci | Effet |
|---|---|
| **↑ / ↓** | Naviguer dans l'historique |
| **Ctrl+R** | Chercher dans l'historique (tape un morceau) |
| **Tab** | Compléter un nom |
| **Ctrl+A / Ctrl+E** | Début / fin de ligne |
| **Ctrl+U** | Effacer la ligne |
| **Ctrl+L** | Nettoyer l'écran (`clear`) |
| **Ctrl+C** | Interrompre le programme en cours |
| **Ctrl+D** | Fin de saisie / quitter |

**Ctrl+R est le raccourci le plus rentable du terminal.** Tape `Ctrl+R` puis `gcc` :
il retrouve ta dernière compilation, quel que soit le temps écoulé.

---

## 8. Trouver de l'aide

```sh
man ls               # le manuel complet (q pour sortir)
ls --help            # aide courte (souvent absent sur macOS)
which gcc            # où se trouve un programme
type cd              # commande externe ou intégrée au shell ?
```

Dans `man` : `/mot` pour chercher, `n` pour l'occurrence suivante, `q` pour sortir.

Les manuels sont arides mais exacts. La section **EXAMPLES**, souvent en bas, est
généralement la plus utile.

---

## 9. macOS ou Linux : ce qui change

Ton Mac utilise les outils BSD, la plupart des Linux utilisent GNU.

| Cas | macOS (BSD) | Linux (GNU) |
|---|---|---|
| Édition sur place | `sed -i '' 's/a/b/' f` | `sed -i 's/a/b/' f` |
| Version d'un outil | souvent indisponible | `ls --version` |
| Couleurs de `ls` | `ls -G` | `ls --color` |
| Regex Perl | non supporté | `grep -P` |

Ces écarts ne concernent qu'une poignée de commandes. Tout ce qui précède
fonctionne à l'identique des deux côtés.

---

## Mémo

```sh
pwd                      # où suis-je
ls -la                   # tout voir, format long
cd dossier / cd .. / cd -   # naviguer

cat / head / tail / less # lire
wc -l fichier            # compter les lignes

mkdir -p a/b/c           # créer une arborescence
touch f.txt              # créer un fichier vide
cp -r src dst            # copier
mv ancien nouveau        # déplacer ou renommer
rm -r dossier            # supprimer (definitif !)

*.c  ?  [0-9]            # jokers — vérifier avec ls avant rm
chmod +x prog            # rendre exécutable
man commande             # documentation
Ctrl+R                   # chercher dans l'historique
```

**Les trois réflexes :** `pwd` en cas de doute, Tab pour compléter, `ls` avant `rm`.

Exercices : [../exercises/naviguer_et_manipuler.md](../exercises/naviguer_et_manipuler.md)
