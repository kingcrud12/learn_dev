# Exercices — Découvrez l'ordinateur

Support : [decouvrez_lordinateur.md](../lessons/decouvrez_lordinateur.md)

Ces exercices ne demandent **aucune programmation**. Ils se font avec du papier,
une calculatrice, et un terminal.

**Ouvrir un terminal :** `Cmd + Espace`, taper « Terminal » (macOS) ; `Ctrl +
Alt + T` (Linux).

Aucune commande de ces exercices ne modifie votre machine. Vous ne pouvez rien
casser.

---

# Niveau 1 — Compter en binaire

Sur papier d'abord. La calculatrice sert à vérifier, pas à résoudre.

## 1.1 — Du binaire au décimal

Convertissez :

- `0011`
- `1000`
- `1010`
- `1111`
- `10000000`
- `11111111`

*Méthode :* chaque position vaut une puissance de 2, en partant de la droite :
1, 2, 4, 8, 16, 32, 64, 128.

**Vérification :**

```sh
echo "ibase=2; 1010" | bc
```

## 1.2 — Du décimal au binaire

Convertissez en binaire sur 8 bits : `5`, `12`, `64`, `100`, `200`, `255`.

*Méthode :* cherchez la plus grande puissance de 2 qui tient dans le nombre,
soustrayez, recommencez.

**Vérification :**

```sh
echo "obase=2; 100" | bc
```

## 1.3 — Combien de combinaisons

Sans calculatrice, complétez :

| Bits | Combinaisons |
|---|---|
| 1 | 2 |
| 2 | |
| 4 | |
| 8 | |
| 10 | |
| 16 | |

Quelle opération fait passer d'une ligne à la suivante ? Formulez la règle en une
phrase.

## 1.4 — Le débordement

Un compteur sur 8 bits contient `11111111` (255). On lui ajoute 1.

Posez l'addition en binaire, bit par bit, en propageant les retenues. Que
devient le neuvième bit ?

Quel est le résultat affiché ? Expliquez en une phrase ce qui s'est passé.

Ce phénomène a une conséquence historique : cherchez ce qu'était le « bug de
l'an 2000 ». C'est le même mécanisme, sur un autre nombre de chiffres.

## 1.5 — Combien de bits faut-il

Combien de bits au minimum pour représenter :

- les 7 jours de la semaine
- les 26 lettres de l'alphabet
- les 12 mois
- un nombre entre 0 et 1000
- la population mondiale (~8 milliards)

*Méthode :* cherchez le plus petit `n` tel que `2ⁿ` dépasse la valeur voulue.

## 1.6 — Kilo, kibi

Un disque est vendu pour **500 Go**. Votre système affiche **465 Go**.

Le vendeur a-t-il menti ? Calculez `500 × 10⁹ / 2³⁰` et concluez.

**Vérification :**

```sh
python3 -c "print(500 * 10**9 / 2**30)"
```

---

# Niveau 2 — Le codage de l'information

## 2.1 — Lire des octets bruts

```sh
echo "Hello" | xxd
```

Vous obtenez `4865 6c6c 6f0a`.

Ces nombres sont en **hexadécimal** (base 16, chiffres 0-9 puis a-f). Convertissez
chacun en décimal et identifiez le caractère correspondant.

*Aide :* `A` vaut 65, et les lettres se suivent.

Que représente le dernier octet, `0a` (soit 10) ? Pourquoi est-il là alors que
vous n'avez pas tapé de sixième caractère ?

## 2.2 — Vérifier l'écart majuscules / minuscules

```sh
printf 'A' | xxd -p
printf 'a' | xxd -p
```

Convertissez les deux en décimal. Quel est l'écart ?

Testez avec deux autres lettres (`M`/`m`, `Z`/`z`). L'écart est-il constant ?

Quelle opération arithmétique transforme une minuscule en majuscule ?

## 2.3 — Le piège du caractère chiffre

```sh
printf '7' | xxd -p
```

Le caractère `7` ne vaut pas 7. Combien vaut-il ?

Quelle soustraction permet de passer du **caractère** `7` au **nombre** 7 ?

Vérifiez que cette même soustraction fonctionne pour `0`, `3` et `9`.

Cette opération reviendra dans le cours de C. C'est l'une des toutes premières
sources de confusion en programmation.

## 2.4 — UTF-8 en action

```sh
echo -n "cafe" | xxd
echo -n "café" | xxd
```

*(Le `-n` supprime le saut de ligne, pour ne compter que les caractères.)*

Combien d'octets dans chaque cas ? Combien de caractères ?

Sur combien d'octets le `é` est-il codé ? Lesquels ?

Essayez maintenant :

```sh
echo -n "日本" | xxd
echo -n "🙂" | xxd
```

Complétez le tableau :

| Texte | Caractères | Octets |
|---|---|---|
| `cafe` | 4 | |
| `café` | 4 | |
| `日本` | 2 | |
| `🙂` | 1 | |

**La question qui compte :** pourquoi un programme qui suppose « un caractère =
un octet » fonctionne-t-il parfaitement en anglais et échoue-t-il en français ?

## 2.5 — Le poids d'une image

Une image de 1920 × 1080 pixels, 3 octets par pixel.

- Combien de pixels ?
- Combien d'octets bruts ?
- Combien de Mo ?

Une image de ce format sur votre disque fait typiquement 300 Ko. Quel est le
rapport de compression ? Comment est-ce possible ?

## 2.6 — Le poids d'un son

Qualité CD : 44 100 échantillons par seconde, 16 bits par échantillon, 2 canaux.

- Combien d'octets par seconde ?
- Combien pour une chanson de 4 minutes ?
- Un CD contient 700 Mo. Combien de minutes de musique ?

Comparez à la durée réelle d'un CD audio (environ 74 minutes). Votre calcul
tombe-t-il juste ?

## 2.7 — Le même octet, plusieurs sens

L'octet `01000001` vaut 65 en décimal.

Donnez **quatre** interprétations possibles de cet octet, selon le contexte
d'utilisation.

Puis répondez : qu'est-ce qui, dans le fichier, indique laquelle est la bonne ?

## 2.8 — Ouvrir une image comme du texte

Trouvez une petite image sur votre machine, puis :

```sh
xxd votre_image.png | head -5
```

Les premiers octets sont-ils lisibles ? Cherchez la suite `PNG` dans la sortie.

Ces premiers octets s'appellent un **nombre magique** : ils identifient le format
du fichier, indépendamment de son extension. Renommer `photo.png` en `photo.txt`
ne change rien à son contenu.

Testez-le : copiez l'image, renommez la copie en `.txt`, ouvrez-la dans un
éditeur de texte. Que voyez-vous ? Pourquoi ?

---

# Niveau 3 — Explorer votre machine

## 3.1 — Votre processeur

```sh
sysctl -n machdep.cpu.brand_string     # macOS
lscpu | head -20                        # Linux
```

Notez : le modèle, le nombre de cœurs, la fréquence.

```sh
sysctl -n hw.ncpu                       # macOS
nproc                                   # Linux
```

À 3 GHz, combien de cycles par seconde ? Par minute ?

## 3.2 — Le temps d'un cycle

Combien de temps dure **un** cycle sur un processeur à 3 GHz ? Exprimez-le en
nanosecondes.

La lumière parcourt 30 cm par nanoseconde. Quelle distance pendant un cycle ?

En quoi cela explique-t-il que les composants d'un processeur soient si petits et
si rapprochés ?

## 3.3 — Votre mémoire

```sh
top -l 1 | head -10                     # macOS
free -h                                 # Linux
```

Combien de RAM au total ? Combien de « libre » ?

Le chiffre de mémoire libre est probablement bas. Cherchez la ligne mentionnant
un cache ou du « inactive ». Pourquoi le système préfère-t-il remplir la mémoire
plutôt que de la laisser vide ?

## 3.4 — Vos programmes en cours

```sh
ps aux | wc -l
```

Combien de processus ? Combien de cœurs avez-vous ?

Comment des centaines de programmes tournent-ils sur si peu de cœurs ? Comment
s'appelle le mécanisme ?

Les dix plus gourmands en processeur :

```sh
ps aux | sort -k3 -rn | head -10
```

## 3.5 — Votre stockage

```sh
df -h
```

Quelle capacité ? Quel espace libre ?

Testez la vitesse d'écriture (crée un fichier de 100 Mo, puis le supprime) :

```sh
dd if=/dev/zero of=/tmp/test bs=1m count=100 && rm /tmp/test
```

Notez le débit affiché. Un SSD dépasse généralement 500 Mo/s, un disque dur
tourne autour de 100 Mo/s. Lequel avez-vous ?

## 3.6 — L'échelle des temps d'accès

En supposant qu'un accès RAM prenne **1 seconde**, complétez le tableau. Le
rapport réel est indiqué.

| Opération | Temps réel | À l'échelle 1 s = RAM |
|---|---|---|
| Accès registre | 0,3 ns | |
| Accès cache L1 | 1 ns | |
| Accès RAM | 50 ns | 1 seconde |
| Accès SSD | 50 µs | |
| Accès disque dur | 5 ms | |
| Paris → New York (réseau) | 80 ms | |

*Méthode :* divisez chaque temps par 50 ns.

Que vous inspire la dernière ligne ? Vaut-il mieux lire une donnée sur un disque
dur local ou dans la RAM d'un serveur à New York ?

## 3.7 — Votre système

```sh
uname -a
sw_vers                                 # macOS
cat /etc/os-release                     # Linux
```

Quel système ? Quelle version ? Quelle architecture de processeur (`arm64`,
`x86_64`) ?

Cherchez la différence entre ces deux architectures. Pourquoi un programme
compilé pour l'une ne fonctionne-t-il pas sur l'autre ?

---

# Niveau 4 — Comprendre l'architecture

Des questions de raisonnement. Répondez par écrit, en quelques phrases.

## 4.1 — Pourquoi pas la base 10

On a vu que le binaire s'impose pour une raison physique.

Expliquez avec vos mots pourquoi un ordinateur qui utiliserait dix niveaux de
tension serait moins fiable. Que se passerait-il en cas de petite perturbation
électrique ?

## 4.2 — RAM contre disque

Pourquoi un document non enregistré disparaît-il en cas de coupure de courant,
alors qu'un document enregistré survit ?

Puis : pourquoi ne pas se passer de RAM et tout faire sur le disque, puisqu'il
est persistant ?

Et l'inverse : pourquoi ne pas tout mettre en RAM, puisqu'elle est cent mille
fois plus rapide ?

Vos deux réponses décrivent un **arbitrage**. Formulez-le en une phrase.

## 4.3 — Les trois étapes

Le processeur répète : chercher, décoder, exécuter.

Il n'existe aucune instruction « afficher une image ». Comment une image
s'affiche-t-elle alors ?

## 4.4 — L'ordonnancement

Votre machine a 8 cœurs et 300 processus.

Comment le système donne-t-il l'illusion que tout tourne en même temps ?

Que se passe-t-il quand un programme monopolise le processeur ? Pourquoi la
machine devient-elle lente **partout**, et pas seulement dans ce programme ?

## 4.5 — La mémoire virtuelle

Chaque programme croit disposer de toute la mémoire, seul.

Quel problème cela résout-il ? Que se passerait-il si tous les programmes
partageaient directement la même mémoire physique ?

Reliez votre réponse à la notion de sécurité.

## 4.6 — Compilé ou interprété

Un même calcul écrit en C et en Python. Le C sera dix à cent fois plus rapide.

Pourquoi ? Qu'est-ce que Python fait à l'exécution que le C a déjà fait avant ?

Puis : pourquoi Python existe-t-il quand même, et pourquoi l'utilise-t-on
massivement ?

## 4.7 — La chaîne de démarrage

Remettez ces étapes dans l'ordre, et justifiez :

- le noyau charge les pilotes
- le processeur exécute le firmware
- le bootloader copie le noyau en RAM
- l'interface graphique se lance
- le firmware teste la mémoire
- le firmware cherche un disque de démarrage

**La question clé :** pourquoi le noyau doit-il être copié **en RAM** avant de
s'exécuter ? Pourquoi le processeur ne peut-il pas l'exécuter directement depuis
le disque ?

## 4.8 — Les couches d'abstraction

Vous cliquez sur « enregistrer » dans un éditeur de texte.

Décrivez ce qui se passe en traversant les couches, du programme jusqu'au disque.
Cinq à six phrases suffisent.

Quelle couche décide de l'endroit physique où les octets seront écrits sur le
disque ? Est-ce le programme ?

---

# Niveau 5 — Aller plus loin

## 5.1 — L'hexadécimal

L'hexadécimal (base 16) est partout en informatique : `xxd`, les couleurs web,
les adresses mémoire.

Pourquoi ? Convertissez `11111111` en binaire vers l'hexadécimal, puis
`10101010`.

Combien de bits représente **un** chiffre hexadécimal ? Combien pour un octet ?

Formulez pourquoi l'hexadécimal est plus commode que le décimal pour représenter
des octets.

## 5.2 — Les couleurs web

En HTML, une couleur s'écrit `#FF0000` pour le rouge.

Décomposez : quelles sont les valeurs de rouge, vert, bleu ? Que valent-elles en
décimal ?

Que donnent `#000000`, `#FFFFFF`, `#808080` ?

Écrivez le code d'un orange (beaucoup de rouge, un peu de vert, pas de bleu) et
vérifiez dans un éditeur ou un navigateur.

## 5.3 — Estimer avant de mesurer

Estimez, **avant** de vérifier :

- combien de photos de 4 Mo tiennent sur un disque de 500 Go ?
- combien de temps pour copier 100 Go à 500 Mo/s ?
- combien de caractères dans un livre de 300 pages ? Combien d'octets en ASCII ?
  Et le fichier tiendrait-il sur une disquette de 1,44 Mo ?

Vérifiez ensuite. L'estimation d'ordre de grandeur est une compétence
d'ingénieur : elle évite de partir sur une mauvaise idée.

## 5.4 — Le cache en action

Créez un gros fichier, puis lisez-le deux fois :

```sh
dd if=/dev/zero of=/tmp/gros bs=1m count=500
time cat /tmp/gros > /dev/null
time cat /tmp/gros > /dev/null
rm /tmp/gros
```

La seconde lecture est plus rapide. Pourquoi ?

D'où viennent les données la seconde fois ?

## 5.5 — Une machine sans système

Imaginez un ordinateur sans système d'exploitation, où un seul programme tourne
et fait tout.

Citez trois choses que ce programme devrait gérer lui-même et qu'il n'a
normalement pas à faire.

Cette situation existe réellement : cherchez ce qu'est un système
« bare metal », et dans quel domaine on le pratique. *(Indice : c'est l'une des
spécialisations de ce parcours.)*

---

# Auto-évaluation

Vous pouvez passer à la suite si vous répondez sans hésiter :

- [ ] Pourquoi les ordinateurs utilisent-ils le binaire ? (la raison physique)
- [ ] Combien de valeurs différentes tiennent dans un octet ?
- [ ] Qu'est-ce qui distingue le caractère `7` du nombre 7 ?
- [ ] Pourquoi « café » occupe-t-il 5 octets pour 4 caractères ?
- [ ] Quelle est la différence entre RAM et disque ? Citez deux critères.
- [ ] Qu'est-ce qu'une adresse mémoire ?
- [ ] Quelles sont les trois étapes que le processeur répète indéfiniment ?
- [ ] Citez trois rôles du système d'exploitation.
- [ ] Quelle est la différence entre un langage compilé et interprété ?
- [ ] Pourquoi le noyau doit-il être chargé en RAM pour s'exécuter ?

---

# Solutions

Aucune n'est donnée — volontairement.

Trois recours dans l'ordre :

1. **Relire la leçon.** Chaque exercice correspond à une section précise.
2. **Vérifier par la commande.** La plupart des réponses se constatent dans un
   terminal. `bc`, `xxd` et `python3 -c` répondent plus vite qu'une recherche.
3. **Expliquer à voix haute.** Si vous ne pouvez pas expliquer une réponse
   simplement, c'est qu'elle n'est pas acquise.

Sur cette leçon en particulier : les questions du niveau 4 n'ont pas de réponse
unique. Ce qui compte est le raisonnement, pas la formule exacte.
