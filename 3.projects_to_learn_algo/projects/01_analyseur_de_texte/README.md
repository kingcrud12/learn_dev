# Projet 1 — Analyseur de texte

**Prérequis :** leçons 1 à 4, 9 (et 8 pour la comparaison finale).

## Le problème

On veut interroger un corpus de texte. Concrètement, répondre à des questions
comme :

- combien de mots distincts ce livre contient-il ?
- quels sont les 20 mots les plus fréquents ?
- le mot « mémoire » y apparaît-il, et combien de fois ?
- quels mots commencent par « algo » ?
- quels mots n'apparaissent qu'une seule fois ?

Chacune de ces questions favorise une structure de données différente. C'est
tout l'intérêt du projet : il n'existe pas de choix qui les serve toutes.

## Ce qu'il faut décider avant d'écrire une ligne

Prends ces trois décisions **par écrit**, dans `DECISIONS.md`, avant de coder.

### 1. Qu'est-ce qu'un mot ?

`"L'algorithme, c'est-à-dire..."` contient combien de mots ?

À trancher :

- les apostrophes coupent-elles ? (`l'algorithme` → un mot ou deux ?)
- les traits d'union ? (`c'est-à-dire`)
- la casse ? (`Le` et `le` sont-ils le même mot ?)
- les accents ? (`memoire` et `mémoire` ?)
- les chiffres ?

**Il n'y a pas de bonne réponse.** Il y a une réponse que tu choisis, que tu
documentes, et qui reste cohérente dans tout le programme. C'est la première
leçon du projet : les décisions de domaine précèdent les décisions techniques,
et elles ne se déduisent pas du code.

*Note sur les accents :* un fichier UTF-8 encode `é` sur deux octets. Un
`char` ne suffit pas. Décide si tu traites l'UTF-8 correctement ou si tu te
limites à l'ASCII — et écris-le. C'est une limite acceptable si elle est assumée.

### 2. Quelle structure pour l'index ?

Les trois candidates, avec ce qu'elles coûtent :

| | Tableau trié | ABR | Table de hachage |
|---|---|---|---|
| Insertion | O(n) | O(log n)* | O(1) |
| Recherche | O(log n) | O(log n)* | O(1) |
| Ordre alphabétique | gratuit | gratuit | tri O(n log n) |
| Préfixe « algo » | O(log n + k) | O(log n + k) | **O(n)** |
| Mémoire | la plus faible | +2 pointeurs/nœud | +listes, +cases vides |

\* si équilibré — et un ABR alimenté par un texte trié dégénère.

Le profil d'usage décide. Mesure-le : sur un livre, combien d'insertions pour
combien de recherches ?

### 3. Le compteur d'occurrences

Une structure `{mot, compteur}`, ou un index qui compte les insertions ?

Et le point important : quand tu insères un mot déjà présent, tu **n'alloues
pas** une nouvelle entrée — tu incrémentes. Cela change la signature de ta
fonction d'insertion.

## Architecture proposée

```
01_analyseur_de_texte/
├── Makefile
├── DECISIONS.md
├── src/
│   ├── main.c            # ligne de commande, boucle interactive
│   ├── lexeur.c/.h       # fichier → suite de mots
│   ├── index.c/.h        # la structure d'index (interface abstraite)
│   ├── table.c/.h        # implémentation par table de hachage
│   ├── arbre.c/.h        # implémentation par ABR
│   └── stats.c/.h        # mesures et rapports
└── data/
    └── corpus.txt
```

**Le point d'architecture qui compte** : `index.h` définit une interface, et
`table.c` et `arbre.c` en sont deux implémentations interchangeables.

```c
// index.h
typedef struct Index Index;

Index *index_creer(void);
void   index_detruire(Index *idx);
bool   index_ajouter(Index *idx, const char *mot);
int    index_compter(const Index *idx, const char *mot);
int    index_nb_distincts(const Index *idx);
void   index_parcourir(const Index *idx,
                       void (*action)(const char *, int, void *),
                       void *contexte);
```

`struct Index` n'est **pas** défini dans le header — seulement déclaré. C'est un
type opaque : les appelants ne peuvent pas accéder aux champs, et `main.c` ne
sait pas s'il parle à une table ou à un arbre.

Cela permet de comparer les deux implémentations **sans toucher au reste du
code**, en changeant une ligne du `Makefile`. C'est exactement le genre de
frontière qu'on cherche à poser.

Le `void *contexte` de `index_parcourir` est le motif standard en C pour passer
un état à une fonction de rappel. Sans lui, la fonction d'action devrait utiliser
une variable globale.

## Les tranches de construction

Une tranche verticale, c'est une chaîne complète qui marche, même minuscule.

**Tranche 1 — la chaîne minimale.** Lire un fichier, découper en mots, afficher
le nombre total. Pas d'index, pas de structure. Vérifie ton lexeur : c'est là
que sont la moitié des bugs, et ils sont invisibles ensuite.

**Tranche 2 — l'index par table de hachage.** Nombre de mots distincts, et
recherche d'un mot. C'est le cœur.

**Tranche 3 — le classement.** Les 20 mots les plus fréquents. Extrais les
paires dans un tableau, trie, affiche. Quel tri choisis-tu ? Faut-il vraiment
trier les 50 000 mots pour n'en afficher que 20 ?

**Tranche 4 — l'ABR.** La seconde implémentation, derrière la même interface.
C'est ici que le travail d'interface de la tranche 2 est payé.

**Tranche 5 — les requêtes.** Ordre alphabétique, préfixe, mots uniques. Compare
le coût de chacune sur les deux implémentations.

**Tranche 6 — le rapport.** Toutes les mesures dans un tableau.

## Mesures attendues

Le projet n'est pas fini sans ces chiffres. Prends un corpus d'au moins
100 000 mots (un roman du domaine public, ou plusieurs concaténés).

| Mesure | Table de hachage | ABR |
|---|---|---|
| Temps d'indexation | | |
| Mémoire de pointe | | |
| Comparaisons/recherche (moyenne) | | |
| Temps des 20 plus fréquents | | |
| Temps du parcours alphabétique | | |
| Temps de la recherche par préfixe | | |
| Facteur de charge / hauteur | | |

**Les questions auxquelles ces chiffres doivent répondre :**

1. Quel est le rapport de temps d'indexation ? Correspond-il à l'écart théorique
   entre O(1) et O(log n) ?
2. Le parcours alphabétique de la table est-il vraiment plus lent, une fois le
   tri compté ? Sur un seul appel, le tri O(n log n) peut coûter moins cher que
   des milliers de recherches économisées.
3. Quelle est la hauteur de ton ABR ? Le texte est-il assez désordonné pour
   l'équilibrer naturellement ?

Cette troisième question est piégeuse. Un texte naturel présente des mots dans un
ordre quasi aléatoire — bien meilleur pour un ABR qu'un dictionnaire trié. Mesure
avant de conclure.

## Extensions

Si le projet t'intéresse au-delà :

**Un trie (arbre préfixe).** La structure faite pour la recherche par préfixe :
`O(longueur du préfixe)`, indépendamment du nombre de mots. C'est la structure
derrière l'autocomplétion.

**Des n-grammes.** Compter les paires de mots consécutifs. La clé devient
`"le chat"`. Cela suffit à faire un générateur de texte markovien — et cela
double la taille de l'index, ce qui rend les questions de mémoire concrètes.

**Un index inversé multi-fichiers.** Pour chaque mot, la liste des fichiers où il
apparaît. La valeur associée à la clé n'est plus un entier mais une liste : la
structure de l'index change.

**Le TF-IDF.** Pondérer chaque mot par sa rareté dans le corpus. C'est le
classement de base d'un moteur de recherche.

## Ce que ce projet doit t'apprendre

- Le choix d'une structure de données **est** une décision d'architecture, et
  elle se prend sur un profil d'usage mesuré.
- Une interface opaque permet de changer d'implémentation sans changer les
  appelants. C'est ce qui rend la comparaison possible.
- Le O(1) de la table de hachage a un prix : aucun ordre. Le récupérer coûte un
  tri.
- Les décisions de domaine (« qu'est-ce qu'un mot ? ») précèdent les décisions
  techniques, et ne se déduisent d'aucun raisonnement algorithmique.
