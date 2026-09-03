# Découvrez l'ordinateur

## Une machine qui ne sait faire qu'une chose

Un ordinateur ne comprend ni le français, ni les images, ni la musique. Il ne
sait faire qu'une seule chose : **manipuler des nombres, très vite**.

Tout le reste — cette page, une vidéo, un jeu, un appel téléphonique — n'est
qu'une conséquence de cette capacité unique. Comprendre comment on passe de
« manipuler des nombres » à « regarder un film », c'est comprendre l'informatique.

C'est l'objet de cette leçon. Elle ne demande aucune connaissance préalable, et
elle ne demande pas d'écrire de code : elle pose le vocabulaire et les
mécanismes sur lesquels tout le reste du parcours s'appuiera.

## Pourquoi des 0 et des 1

On dit toujours que l'ordinateur « fonctionne en binaire ». C'est vrai, mais
l'explication habituelle s'arrête trop tôt. La vraie raison est **physique**.

Un ordinateur est un circuit électrique. Dans un fil, on peut mesurer une
tension. Le problème, c'est que cette tension n'est jamais parfaitement stable :
elle fluctue, elle subit des parasites, elle varie avec la température.

Imaginons qu'on veuille représenter les chiffres de 0 à 9 avec des tensions
entre 0 et 5 volts. Chaque chiffre occuperait une tranche de 0,5 volt. Une
perturbation de quelques dixièmes de volt suffirait à transformer un 3 en 4.
Le calcul serait faux, sans que rien ne le signale.

Avec seulement **deux** états, la marge devient énorme :

- entre 0 et 1 volt → c'est un **0**
- entre 4 et 5 volts → c'est un **1**

Il faudrait une perturbation de plusieurs volts pour provoquer une confusion.
Le système devient **fiable**.

Voilà pourquoi le binaire s'est imposé : non pas parce qu'il est élégant, mais
parce que c'est le seul codage qui résiste au bruit électrique. Toute
l'informatique repose sur ce compromis : on renonce à la richesse de
représentation pour gagner la fiabilité.

### Le bit et l'octet

Un **bit** (*binary digit*) est un 0 ou un 1. C'est la plus petite information
possible : une réponse à une question fermée.

Un seul bit ne dit pas grand-chose. Mais on les groupe :

| Bits | Combinaisons possibles | Exemple |
|---|---|---|
| 1 | 2 | oui / non |
| 2 | 4 | 00, 01, 10, 11 |
| 3 | 8 | |
| 8 | **256** | un **octet** |
| 16 | 65 536 | |
| 32 | ~4,3 milliards | |
| 64 | ~18 milliards de milliards | |

Chaque bit ajouté **double** le nombre de combinaisons. C'est une croissance
exponentielle, et elle explique pourquoi 64 bits suffisent à adresser bien plus
de mémoire qu'aucune machine n'en possédera jamais.

L'**octet** (8 bits, *byte* en anglais) est l'unité de base. C'est la plus petite
quantité de mémoire qu'un ordinateur manipule couramment.

### Une remarque sur les unités

Un « kilo-octet » vaut-il 1 000 ou 1 024 octets ?

Les deux, selon qui parle. Historiquement, l'informatique comptait par
puissances de deux : 1 Ko = 2¹⁰ = 1 024 octets. Les fabricants de disques durs,
eux, comptent en puissances de dix : 1 Ko = 1 000 octets — ce qui fait paraître
leurs disques plus gros.

La norme actuelle tranche ainsi :

- **ko** (kilo-octet) = 1 000 octets
- **Kio** (kibi-octet) = 1 024 octets

En pratique, tout le monde continue de dire « kilo » pour les deux. C'est
pourquoi un disque vendu pour 1 To affiche 931 Go dans votre système : ce sont
les mêmes octets, comptés avec deux règles différentes.

## Tout est nombre

Puisque la machine ne manipule que des nombres, il faut **coder** toute
information sous forme numérique. Ce codage est une convention : rien dans les
bits eux-mêmes n'indique ce qu'ils représentent.

### Les nombres entiers

Le binaire est un système de numération comme le décimal, mais en base 2.

En décimal, `237` signifie `2×100 + 3×10 + 7×1`.
En binaire, `1101` signifie `1×8 + 1×4 + 0×2 + 1×1` = **13**.

| Binaire | Calcul | Décimal |
|---|---|---|
| `0000` | | 0 |
| `0001` | 1 | 1 |
| `0101` | 4+1 | 5 |
| `1010` | 8+2 | 10 |
| `1111` | 8+4+2+1 | 15 |

Avec 8 bits, on va de `00000000` (0) à `11111111` (255). D'où les 256
combinaisons de l'octet.

**Cette limite a des conséquences réelles.** Un compteur sur 8 bits qui atteint
255 et qu'on incrémente encore ne passe pas à 256 : il repart à 0. C'est un
**débordement**, et c'est une source classique de bugs — y compris dans des
logiciels critiques.

### Les caractères

Pour écrire du texte, on décide qu'un nombre représente une lettre. C'est une
table de correspondance, et la plus ancienne s'appelle **ASCII** :

| Caractère | Code |
|---|---|
| `A` | 65 |
| `B` | 66 |
| `a` | 97 |
| `0` (le chiffre) | 48 |
| espace | 32 |

Remarquez que le **caractère** `0` vaut 48, pas 0. Le caractère est un symbole
imprimé, le nombre est une quantité. Ce sont deux choses différentes, et les
confondre est l'une des premières erreurs qu'on commet en programmation.

Remarquez aussi que `a` (97) et `A` (65) diffèrent de 32 exactement. Cet écart
est le même pour toutes les lettres : passer en majuscules revient à soustraire
32. Ce genre de régularité n'est pas un hasard — la table a été conçue ainsi.

ASCII n'utilise que 7 bits, soit 128 caractères. Assez pour l'anglais, pas pour
le français (`é`, `à`, `ç`), encore moins pour le grec, l'arabe ou le japonais.

D'où **Unicode**, qui attribue un numéro à chaque caractère de chaque écriture
humaine — plus de 150 000 aujourd'hui, emoji compris. Son codage le plus répandu,
**UTF-8**, utilise un nombre variable d'octets : un seul pour les caractères
ASCII (par compatibilité), deux pour `é`, jusqu'à quatre pour certains
idéogrammes.

Conséquence concrète : dans un fichier UTF-8, **le nombre de caractères n'est pas
le nombre d'octets**. Le mot « café » occupe 5 octets pour 4 caractères. C'est
une source de bugs si fréquente qu'elle mérite d'être sue dès maintenant.

### Les images

Une image est une grille de points, les **pixels**. Chaque pixel a une couleur,
codée en général sur 3 octets : un pour le rouge, un pour le vert, un pour le
bleu, chacun de 0 à 255.

- `(255, 0, 0)` → rouge pur
- `(0, 0, 0)` → noir
- `(255, 255, 255)` → blanc

Avec 256 valeurs par canal, on obtient 256³ ≈ **16,7 millions** de couleurs.

Le calcul de taille est instructif. Une photo de 4000 × 3000 pixels :

```
4000 × 3000 × 3 octets = 36 000 000 octets ≈ 36 Mo
```

Or une photo de cet appareil fait typiquement 4 Mo. La différence est la
**compression** : les images réelles contiennent beaucoup de redondance (des
zones de ciel presque uniformes, par exemple), et les formats comme JPEG
l'exploitent — en acceptant de perdre un peu de qualité.

### Les sons

Un son est une onde continue. Un ordinateur ne sait pas stocker du continu : il
**échantillonne**, c'est-à-dire qu'il mesure l'amplitude à intervalles réguliers.

La qualité CD utilise 44 100 mesures par seconde, sur 16 bits, en stéréo :

```
44100 × 2 octets × 2 canaux = 176 400 octets par seconde
soit environ 10 Mo par minute
```

Là encore, MP3 et consorts compressent, en supprimant ce que l'oreille humaine
ne perçoit pas.

### Le point à retenir

Un même octet valant `01000001` peut être :

- le nombre **65**
- la lettre **A**
- une nuance de rouge sombre
- un échantillon sonore
- une instruction pour le processeur

**Rien dans l'octet ne dit lequel.** C'est le programme qui décide de
l'interprétation, en fonction du contexte. Ouvrir une image avec un éditeur de
texte affiche du charabia : les octets sont les mêmes, la convention de lecture
ne l'est pas.

C'est l'idée la plus importante de cette leçon.

## Les organes de la machine

Un ordinateur, quel qu'il soit — serveur, portable, téléphone, machine à laver —
suit la même organisation, dite **architecture de von Neumann**, formulée en
1945 et toujours d'actualité.

```
        ┌─────────────────────────────────┐
        │         PROCESSEUR (CPU)        │
        │   ┌──────────┐   ┌───────────┐  │
        │   │  unité   │   │ registres │  │
        │   │ de calcul│   │           │  │
        │   └──────────┘   └───────────┘  │
        └────────────┬────────────────────┘
                     │  bus
        ┌────────────┴────────────────────┐
        │      MÉMOIRE VIVE (RAM)         │
        │  programmes + données en cours  │
        └────────────┬────────────────────┘
                     │
        ┌────────────┴────────────────────┐
        │   STOCKAGE (disque, SSD)        │
        │   ce qui survit à l'extinction  │
        └─────────────────────────────────┘
                     │
        ┌────────────┴────────────────────┐
        │  ENTRÉES / SORTIES              │
        │  clavier, écran, réseau...      │
        └─────────────────────────────────┘
```

### Le processeur

C'est l'organe qui calcule. Son fonctionnement est d'une simplicité déroutante :
il répète indéfiniment trois étapes.

1. **Chercher** l'instruction suivante en mémoire
2. **Décoder** ce qu'elle demande
3. **Exécuter**

Et il recommence. Des milliards de fois par seconde.

Les instructions qu'il comprend sont élémentaires — bien plus que ce qu'on
imagine :

- charger un nombre depuis la mémoire
- additionner deux nombres
- comparer deux nombres
- si le résultat est zéro, sauter à une autre instruction
- écrire un nombre en mémoire

C'est à peu près tout. Il n'existe aucune instruction « afficher une image » ou
« envoyer un mail ». Ces opérations sont des **assemblages** de millions
d'instructions élémentaires.

Cette idée mérite qu'on s'y arrête : toute la richesse du logiciel provient de la
combinaison d'opérations triviales, exécutées à une vitesse qui échappe à
l'intuition.

**La fréquence** se mesure en hertz. 3 GHz = 3 milliards de cycles par seconde.
Une instruction simple prend un ou quelques cycles.

Pour donner une échelle : la lumière parcourt 10 centimètres pendant un cycle
d'un processeur à 3 GHz. C'est pourquoi les composants sont si petits et si
proches — au-delà, le temps de trajet du signal devient un facteur limitant.

**Les cœurs.** Un processeur moderne en contient plusieurs, chacun capable
d'exécuter un programme indépendamment. Huit cœurs permettent huit calculs
simultanés — à condition que le programme soit écrit pour en profiter, ce qui est
loin d'être automatique.

### La mémoire vive (RAM)

C'est l'espace de travail. Tout ce que le processeur manipule à un instant donné
s'y trouve : le système, les programmes ouverts, les documents en cours.

Deux propriétés la définissent.

**Elle est rapide** — quelques dizaines de nanosecondes pour un accès.

**Elle est volatile** — son contenu disparaît à l'extinction. C'est pourquoi un
document non enregistré est perdu en cas de coupure : il n'existait qu'en RAM.

On peut se la représenter comme une immense série de cases numérotées, chacune
contenant un octet :

```
adresse    contenu
0x0000     01001000
0x0001     01100101
0x0002     01101100
...
```

Ce numéro de case est l'**adresse**. Un programme qui veut lire ou écrire une
donnée doit connaître son adresse.

Cette notion est centrale. Le cours de C y consacrera une place importante sous
le nom de **pointeurs** — et ce sera le point le plus exigeant du parcours. Vous
pouvez d'ores et déjà retenir l'essentiel : *une donnée est quelque part, et ce
« quelque part » est un nombre*.

### Le stockage

Disque dur ou SSD : ce qui **survit** à l'extinction.

| | RAM | SSD | Disque dur |
|---|---|---|---|
| Vitesse d'accès | ~50 nanosecondes | ~50 microsecondes | ~5 millisecondes |
| Persistant | non | oui | oui |
| Prix par Go | élevé | moyen | faible |
| Capacité typique | 8–32 Go | 256 Go – 2 To | 1–20 To |

Les écarts de vitesse sont difficiles à saisir tels quels. Ramenons-les à
l'échelle humaine, en supposant qu'un accès RAM prenne **1 seconde** :

| Opération | Temps réel | À l'échelle humaine |
|---|---|---|
| Accès RAM | 50 ns | 1 seconde |
| Accès SSD | 50 µs | 17 minutes |
| Accès disque dur | 5 ms | **1 jour** |

Un disque dur est cent mille fois plus lent que la RAM. C'est la raison pour
laquelle le remplacement d'un disque par un SSD transforme une machine bien plus
qu'un processeur plus rapide : ce n'est presque jamais le calcul qui est le
goulot d'étranglement, c'est l'attente des données.

### La hiérarchie mémoire

Cette différence de vitesse impose une organisation en étages. Plus c'est
rapide, plus c'est cher, donc plus c'est petit.

```
   registres      quelques octets      1 cycle          dans le processeur
   cache L1       ~64 Ko               ~4 cycles
   cache L2       ~1 Mo                ~12 cycles
   cache L3       ~32 Mo               ~40 cycles
   RAM            8–32 Go              ~200 cycles
   SSD            500 Go               ~200 000 cycles
   disque dur     2 To                 ~20 000 000 cycles
```

Le principe : les données fréquemment utilisées remontent automatiquement vers
les niveaux rapides. Ce mécanisme est invisible pour le programmeur — mais ses
effets ne le sont pas. Deux programmes calculant la même chose peuvent différer
d'un facteur dix selon la façon dont ils parcourent la mémoire.

Le cours d'algorithmique reviendra sur ce point : c'est ce qui explique qu'un
tableau soit souvent bien plus rapide qu'une liste chaînée, alors que la théorie
les donne équivalents.

### Les entrées et sorties

Tout le reste : clavier, souris, écran, carte réseau, imprimante.

Le principe est uniforme. Chaque périphérique est vu par le système comme une
zone de mémoire à lire ou à écrire. Envoyer une image à l'écran, c'est écrire des
octets à un certain endroit ; lire une touche, c'est lire des octets ailleurs.

Sous Linux, cette abstraction est poussée jusqu'au bout : **tout est fichier**.
Le clavier est un fichier qu'on lit, l'écran un fichier qu'on écrit, une carte
réseau presque pareil. C'est une idée d'une puissance remarquable, et le cours
sur Linux la reprendra en détail.

## Le logiciel : des couches d'abstraction

Le matériel est une chose. Ce qui tourne dessus en est une autre, et s'organise
en couches, chacune masquant la complexité de celle du dessous.

```
┌────────────────────────────────────────┐
│  Vos programmes (navigateur, éditeur)  │
├────────────────────────────────────────┤
│  Bibliothèques (fonctions réutilisables)│
├────────────────────────────────────────┤
│  Système d'exploitation (noyau)        │
├────────────────────────────────────────┤
│  Pilotes (drivers)                     │
├────────────────────────────────────────┤
│  Matériel                              │
└────────────────────────────────────────┘
```

### Le système d'exploitation

Linux, Windows, macOS, Android, iOS. Son rôle est celui d'un arbitre.

**Il partage le processeur.** Vous avez peut-être 200 programmes en cours pour
8 cœurs. Le système donne à chacun quelques millisecondes puis passe au suivant,
si vite que tout paraît simultané. C'est l'**ordonnancement**.

**Il partage la mémoire.** Chaque programme croit disposer de toute la mémoire,
seul. En réalité le système lui présente une **mémoire virtuelle**, et traduit
ses adresses vers la mémoire réelle. Cela évite qu'un programme lise ou détruise
les données d'un autre — accidentellement ou volontairement.

**Il gère les fichiers.** Un disque ne contient pas de « dossiers » : ce sont des
blocs numérotés. L'arborescence est une construction du système.

**Il contrôle les accès.** Un programme ne parle jamais directement au matériel.
Il demande au système, qui vérifie et exécute. C'est un **appel système**, et
c'est la frontière de sécurité fondamentale d'un ordinateur moderne.

### Les langages de programmation

Le processeur ne comprend que des nombres. Une instruction ressemble à :

```
10110000 01100001
```

Écrire un programme ainsi est possible — cela s'est fait — mais impraticable. On
a donc inventé des langages lisibles par des humains, traduits ensuite en binaire.

**L'assembleur** est la transcription directe des instructions machine :

```asm
mov al, 97
```

Un mot par instruction. Lisible, mais il faut des dizaines de lignes pour la
moindre opération, et le code ne fonctionne que sur un type de processeur.

**Les langages compilés** (C, C++, Rust, Go) sont traduits une fois pour toutes
en binaire par un **compilateur**. Le résultat est un fichier exécutable, rapide,
mais spécifique à une machine et à un système.

```c
printf("bonjour\n");
```

**Les langages interprétés** (Python, JavaScript, Ruby) sont lus et exécutés à la
volée par un **interpréteur**. Plus souples, plus lents, et il faut que
l'interpréteur soit installé.

```python
print("bonjour")
```

Ces trois lignes font la même chose. Elles diffèrent par ce qu'elles **cachent** :
l'assembleur ne cache rien, le C cache un peu, Python cache presque tout.

### Pourquoi ce parcours commence par le C

Vous savez maintenant assez pour comprendre ce choix.

Python gère la mémoire à votre place, devine les types, nettoie derrière vous.
C'est confortable — et c'est précisément ce qui empêche de comprendre ce qui se
passe.

Le C, lui, vous met face à la machine telle qu'elle est : les adresses, les
octets, l'allocation, la libération. Ce que cette leçon a décrit en mots, le C
vous le fera manipuler directement.

C'est plus inconfortable. C'est aussi la seule façon de ne pas rester à la
surface.

## Ce qui se passe quand vous allumez la machine

Pour rassembler tout ce qui précède, suivons le déroulement complet.

**1. Mise sous tension.** Le processeur démarre à une adresse fixe, câblée dans
le matériel. À cette adresse se trouve le **firmware** (BIOS ou UEFI), stocké sur
une puce de la carte mère.

**2. Test du matériel.** Le firmware vérifie que la RAM répond, que les
périphériques essentiels sont là.

**3. Recherche du système.** Il cherche, sur les disques, un programme de
démarrage — le *bootloader*.

**4. Chargement du noyau.** Le bootloader lit le noyau du système sur le disque
et le copie **en RAM**. Rappelez-vous : le processeur ne peut exécuter que ce qui
est en mémoire vive.

**5. Le noyau prend la main.** Il initialise ses structures, charge les pilotes,
monte le système de fichiers.

**6. Lancement des services**, puis de l'interface graphique.

**7. Vous ouvrez un programme.** Le système lit le fichier exécutable sur le
disque, le charge en RAM, lui attribue un espace mémoire virtuel, et demande au
processeur de sauter à sa première instruction.

**8. Le programme tourne.** Chercher, décoder, exécuter. Des milliards de fois
par seconde. Quand il a besoin du disque, du réseau ou de l'écran, il fait un
appel système.

Tout ce que vous verrez ensuite — un fichier qui s'ouvre, une page qui se charge,
un programme qui plante — est une variation sur cette séquence.

## Ce qu'il faut retenir

- Le binaire s'impose pour une raison **physique** : deux états résistent au
  bruit électrique, dix non.
- Un octet = 8 bits = 256 valeurs possibles.
- **Tout est codé en nombres** : texte, images, sons, instructions. Rien dans les
  octets n'indique leur signification — c'est le programme qui décide.
- Le processeur ne sait faire que des opérations élémentaires, mais des milliards
  par seconde.
- La **RAM** est rapide et volatile ; le **stockage** est lent et persistant.
  L'écart de vitesse est d'un facteur 100 000.
- Une **adresse** est le numéro d'une case mémoire. C'est la base des pointeurs
  du C.
- Le **système d'exploitation** arbitre l'accès au processeur, à la mémoire et
  aux périphériques.
- Le logiciel s'organise en **couches d'abstraction**, chacune masquant la
  précédente. Apprendre l'informatique, c'est descendre ces couches.

---

## À faire maintenant

Aucun code — seulement des observations sur votre propre machine. Ouvrez un
terminal.

**Voir votre processeur :**

```sh
sysctl -n machdep.cpu.brand_string     # macOS
lscpu                                   # Linux
```

Combien de cœurs ? Quelle fréquence ? Combien de cycles par seconde cela
représente-t-il ?

**Voir votre mémoire :**

```sh
top -l 1 | head -10                     # macOS
free -h                                 # Linux
```

Combien de RAM au total ? Combien est réellement libre ? Le chiffre vous
surprendra peut-être — le système utilise la mémoire inoccupée comme cache
plutôt que de la laisser vide.

**Compter les programmes en cours :**

```sh
ps aux | wc -l
```

Vous en avez probablement plusieurs centaines pour une poignée de cœurs. C'est
l'ordonnancement à l'œuvre.

**Voir des octets bruts :**

```sh
echo "Hello" | xxd
```

Vous verrez apparaître `48 65 6c 6c 6f 0a`. Ce sont les codes des caractères, en
hexadécimal. `48` en hexadécimal vaut 72 en décimal — le code de `H`.

Essayez maintenant :

```sh
echo "café" | xxd
```

Comptez les octets. Il y en a plus que de caractères. Vous venez de voir UTF-8
en action.
