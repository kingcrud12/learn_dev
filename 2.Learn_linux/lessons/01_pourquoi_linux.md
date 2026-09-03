# Pourquoi Linux et la ligne de commande

Pendant du `01_pourquoi_le_c.md` de `learn_C/`. Même logique : comprendre l'outil
avant de s'en servir.

---

## Le terminal n'est pas une nostalgie

L'objection est connue : pourquoi taper des commandes quand une interface
graphique existe ?

Parce qu'une interface graphique montre **ce que son auteur a prévu**. La ligne de
commande donne accès à ce que la machine **peut faire**. La différence apparaît dès
qu'on sort du cas prévu : renommer trois cents fichiers selon une règle, trouver
tous les `.c` modifiés cette semaine qui contiennent `malloc`, relancer une
compilation à chaque sauvegarde. Aucun menu ne propose ça. Une ligne suffit.

Deuxième raison, décisive : **une commande s'écrit, donc se répète, s'archive et se
partage**. Un clic ne se documente pas. C'est tout le principe de l'automatisation,
et c'est ce qui sépare un développeur d'un utilisateur.

## Tu y es déjà

Sans le terminal, ce parcours serait bloqué depuis la première leçon :

```sh
gcc -Wall -Wextra -g codes/prog.c -o codes/prog && ./codes/prog
lldb codes/prog
```

Le `&&`, le `./`, les chemins relatifs, le code de sortie dans `$?` — tout ça est
du shell, pas du C. Tu utilises déjà Linux sans l'avoir appris. Cette partie du
parcours consiste à remplacer les réflexes par une compréhension.

## Ce que Linux fait tourner

- **Tous les serveurs web** ou presque — ton futur `dev_web/` s'y déploiera
- **Android** — un noyau Linux sous l'interface, pour `mobile_dev/`
- **Le matériel embarqué** — routeurs, box, voitures, satellites : `dev_Embeded/`
- **Les supercalculateurs** — les 500 plus puissants du monde, sans exception
- **macOS** — un cousin Unix, d'où le fait que ton Mac parle presque la même langue

Écrire du C sans savoir naviguer dans un système Unix, c'est savoir fabriquer une
pièce sans savoir où elle s'installe.

## macOS, Linux, Unix : où tu te situes

Ton Mac n'est pas Linux, mais un descendant d'**Unix** — comme Linux. Les deux
partagent la même philosophie et l'essentiel des commandes.

Les différences apparaissent sur les détails : les outils de macOS viennent de BSD,
ceux de Linux du projet GNU. Concrètement, quelques options changent.

| Cas | macOS (BSD) | Linux (GNU) |
|---|---|---|
| Modifier un fichier sur place | `sed -i '' 's/a/b/' f` | `sed -i 's/a/b/' f` |
| Version d'un outil | souvent absent | `date --version` |
| Expressions Perl | `grep -E` | `grep -P` |

**Ce qui compte pour toi :** 95 % de ce que tu apprendras ici fonctionne à
l'identique des deux côtés. Les écarts seront signalés dans les leçons, avec la
mention *(macOS)* ou *(Linux)*.

Si tu veux le vrai Linux plus tard, une machine virtuelle ou un Raspberry Pi
suffira — c'est d'ailleurs le point de passage naturel vers `dev_OS/`.

## La philosophie Unix

Cinq principes énoncés dans les années 1970, et qui expliquent tout le reste :

**Un programme fait une seule chose, et la fait bien.** `ls` liste, `grep` filtre,
`wc` compte. Aucun ne fait le travail d'un autre.

**Les programmes se combinent.** La sortie de l'un devient l'entrée du suivant.
C'est le tube `|`, et c'est là que la puissance apparaît :

```sh
ls *.c | wc -l          # combien de fichiers .c ?
```

**Tout est fichier.** Un document, un disque, un clavier, une connexion réseau :
le système les présente tous de la même façon. Un seul jeu d'outils suffit.

**Le texte est le format universel.** Pas de format binaire propriétaire entre les
outils — du texte, que n'importe quel programme peut lire et produire.

**Le silence vaut succès.** Une commande qui réussit n'affiche rien. Tu l'as déjà
vécu avec `gcc` : quand il ne dit rien, c'est que tout va bien.

## Ce que ça t'apporte pour la suite

| Spécialisation | Ce que le shell y devient |
|---|---|
| **Embarqué** | Compilation croisée, communication série, flash de cartes |
| **OS** | Le terrain d'étude lui-même : processus, appels système, noyau |
| **Web** | Administration de serveurs, déploiement, journaux, conteneurs |
| **Mobile** | `adb`, chaînes de compilation, automatisation des builds |

Et transversalement : **Git**, qui s'utilise en ligne de commande et sans lequel
aucun projet sérieux ne se mène.

---

## Le vocabulaire minimum

| Terme | Sens |
|---|---|
| **Terminal** | La fenêtre — un affichage, rien de plus |
| **Shell** | Le programme qui interprète tes commandes (`zsh` chez toi) |
| **Prompt** | Le texte affiché quand le shell t'attend (`%` ou `$`) |
| **Commande** | Un programme lancé depuis le shell |
| **Argument** | Ce que tu passes à la commande (`ls mon_dossier`) |
| **Option** | Un réglage, précédé de `-` ou `--` (`ls -l`) |

Ton shell est `zsh` — le défaut sur macOS depuis 2019. `bash` est le défaut sur la
plupart des Linux. Les différences sont mineures à ce stade.

```sh
echo $SHELL     # /bin/zsh
```

## Les trois réflexes à prendre tout de suite

**`pwd` avant tout.** *Print working directory* — où suis-je ? La moitié des erreurs
de débutant viennent d'un mauvais répertoire courant. Tu l'as déjà rencontré avec
`/temp/bug.c` contre `temp/bug.c`.

**La touche Tab complète.** Tape les premières lettres, appuie sur Tab : le shell
finit le mot. Deux fois : il propose les possibilités. C'est plus rapide, et surtout
ça évite les fautes de frappe dans les chemins.

**`man commande` pour la documentation.** Chaque outil embarque son manuel. Navigation
avec les flèches, `/mot` pour chercher, `q` pour sortir. C'est austère, mais c'est la
source de vérité.

```sh
man ls
```

---

## À retenir

Le shell n'est pas une interface de secours pour experts, c'est **un langage** :
des commandes qu'on combine, qu'on enchaîne, qu'on archive dans des scripts.

Tu en connais déjà des morceaux — `&&`, `./`, `$?`, les chemins relatifs. La suite
consiste à en faire un outil conscient plutôt qu'une suite de formules recopiées.

Leçon suivante : [02_naviguer_et_manipuler.md](02_naviguer_et_manipuler.md)
