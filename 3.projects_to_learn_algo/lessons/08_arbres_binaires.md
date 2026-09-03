# Arbres binaires de recherche

## Le compromis qu'on n'avait pas encore

Récapitulons les deux structures vues jusqu'ici, sur les trois opérations qui
comptent.

| | Tableau trié | Liste chaînée |
|---|---|---|
| Recherche | **O(log n)** | O(n) |
| Insertion | O(n) | **O(1)** |
| Suppression | O(n) | **O(1)**\* |

\* si on a déjà l'adresse du nœud.

Chacune est excellente sur un axe et mauvaise sur l'autre. Le tableau trié
cherche vite mais insère lentement ; la liste insère vite mais cherche lentement.

L'arbre binaire de recherche offre **O(log n) sur les trois** — à une condition
qu'on examinera, et qui n'est pas un détail.

## La structure

Chaque nœud a une valeur et **deux** enfants au lieu d'un suivant.

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *gauche;
    struct Noeud *droite;
} Noeud;
```

Le vocabulaire, une fois pour toutes :

- **Racine** : le nœud du sommet, sans parent.
- **Feuille** : un nœud sans enfant.
- **Hauteur** : le nombre d'arêtes du plus long chemin racine → feuille.
- **Sous-arbre** : n'importe quel nœud avec toute sa descendance — et c'est le
  point : **un sous-arbre est un arbre**. D'où la récursivité de tout ce qui suit.

## La propriété d'ordre

Un arbre binaire devient un **arbre binaire de recherche** (ABR) s'il respecte,
pour **chaque** nœud :

> tout ce qui est dans le sous-arbre gauche est **plus petit** que le nœud,
> tout ce qui est dans le sous-arbre droit est **plus grand**.

```
                50
              /    \
            30      70
           /  \    /  \
         20   40  60   80
```

Vérifiez sur le nœud `30` : à gauche `20` (plus petit), à droite `40` (plus
grand). Sur `50` : tout le sous-arbre gauche `{20, 30, 40}` est inférieur, tout
le droit `{60, 70, 80}` est supérieur.

**La propriété est récursive et globale.** L'erreur classique est de ne la
vérifier qu'entre un nœud et ses enfants directs. Cet arbre-là est faux :

```
                50
              /    \
            30      70
           /  \
         20   60          ← 60 > 50, il ne peut pas être à gauche de 50
```

`60` est bien supérieur à `30`, son parent. Mais il se trouve dans le sous-arbre
gauche de `50`, ce qui viole la propriété. Une recherche de `60` partirait à
droite depuis la racine et ne le trouverait jamais.

## Recherche

La propriété d'ordre donne directement l'algorithme : à chaque nœud, on sait de
quel côté aller.

```c
Noeud *rechercher(Noeud *racine, int valeur) {
    if (racine == NULL || racine->valeur == valeur) {
        return racine;
    }
    if (valeur < racine->valeur) {
        return rechercher(racine->gauche, valeur);
    }
    return rechercher(racine->droite, valeur);
}
```

C'est la recherche dichotomique, appliquée à une structure de pointeurs. Chaque
descente **élimine un sous-arbre entier**.

**Complexité : O(h)**, où `h` est la hauteur. Retenez bien `O(h)` et non
`O(log n)` — la suite de la leçon explique pourquoi la distinction est vitale.

La version itérative, sans coût de pile :

```c
Noeud *rechercher_iter(Noeud *racine, int valeur) {
    while (racine != NULL && racine->valeur != valeur) {
        racine = (valeur < racine->valeur) ? racine->gauche : racine->droite;
    }
    return racine;
}
```

## Insertion

On descend comme pour une recherche, jusqu'à trouver une place vide.

```c
Noeud *inserer(Noeud *racine, int valeur) {
    if (racine == NULL) {
        Noeud *nouveau = malloc(sizeof(Noeud));
        if (nouveau == NULL) return NULL;
        nouveau->valeur = valeur;
        nouveau->gauche = NULL;
        nouveau->droite = NULL;
        return nouveau;
    }

    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = inserer(racine->droite, valeur);
    }
    // valeur égale : on ne fait rien (pas de doublons)
    return racine;
}
```

Le schéma `racine->gauche = inserer(racine->gauche, ...)` est **le motif
fondamental** des arbres en C. La fonction renvoie la racine du sous-arbre —
inchangée dans le cas général, nouvelle si le sous-arbre était vide. Le parent
réaffecte systématiquement. Cela évite d'avoir à distinguer « je crée » de « je
descends ».

Les nouveaux nœuds arrivent toujours **en feuille**. Aucun nœud existant n'est
déplacé.

## Les parcours

Quatre façons de visiter tous les nœuds. Les trois premières diffèrent
uniquement par **la position de la ligne de traitement**.

### Infixe (gauche → nœud → droite)

```c
void parcours_infixe(const Noeud *racine) {
    if (racine == NULL) return;
    parcours_infixe(racine->gauche);
    printf("%d ", racine->valeur);        // au milieu
    parcours_infixe(racine->droite);
}
```

Sur l'arbre exemple : `20 30 40 50 60 70 80`.

**Le parcours infixe d'un ABR donne les valeurs triées.** C'est la propriété la
plus utile de la structure, et la façon la plus sûre de vérifier qu'un arbre est
un ABR valide.

### Préfixe (nœud → gauche → droite)

`50 30 20 40 70 60 80`. On voit le nœud avant sa descendance : c'est le parcours
qui sert à **copier** ou **sérialiser** un arbre. Réinsérer les valeurs dans cet
ordre reconstruit exactement le même arbre.

### Suffixe (gauche → droite → nœud)

`20 40 30 60 80 70 50`. On voit les enfants avant le parent : c'est
**obligatoirement** celui à utiliser pour libérer un arbre.

```c
void liberer_arbre(Noeud *racine) {
    if (racine == NULL) return;
    liberer_arbre(racine->gauche);
    liberer_arbre(racine->droite);
    free(racine);                         // en dernier
}
```

Libérer la racine en premier rendrait ses pointeurs inaccessibles, et tout le
reste de l'arbre serait perdu — une fuite de tous les nœuds.

### En largeur (niveau par niveau)

`50 30 70 20 40 60 80`. Celui-ci n'est pas récursif : il utilise une **file**.

```c
void parcours_largeur(Noeud *racine) {
    if (racine == NULL) return;

    Noeud *file[100];
    int tete = 0, queue = 0;
    file[queue++] = racine;

    while (tete < queue) {
        Noeud *courant = file[tete++];
        printf("%d ", courant->valeur);

        if (courant->gauche)  file[queue++] = courant->gauche;
        if (courant->droite)  file[queue++] = courant->droite;
    }
}
```

C'est déjà l'algorithme du BFS de la leçon suivante. Un arbre est un cas
particulier de graphe, et ses parcours sont ceux des graphes.

## Suppression — les trois cas

L'opération la plus délicate de toute la leçon, parce qu'il faut retirer un nœud
**sans casser la propriété d'ordre**.

### Cas 1 : une feuille

On la supprime, on renvoie `NULL`.

### Cas 2 : un seul enfant

L'enfant prend la place du nœud supprimé.

```
      30                30
     /       →         /
   20                15
     \
      15
```

### Cas 3 : deux enfants

Ici, ni l'un ni l'autre enfant ne peut simplement remonter — la propriété serait
violée. La solution : remplacer la valeur du nœud par son **successeur infixe**,
c'est-à-dire la plus petite valeur du sous-arbre droit. Puis supprimer ce
successeur, qui est forcément un cas 1 ou 2.

Pourquoi ce nœud précis ? Parce que c'est **la seule valeur** qui est à la fois
supérieure à tout le sous-arbre gauche et inférieure à tout le reste du
sous-arbre droit. C'est exactement le rôle que tenait le nœud supprimé.

```c
static Noeud *minimum(Noeud *racine) {
    while (racine->gauche != NULL) {
        racine = racine->gauche;
    }
    return racine;
}

Noeud *supprimer(Noeud *racine, int valeur) {
    if (racine == NULL) return NULL;

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    } else if (valeur > racine->valeur) {
        racine->droite = supprimer(racine->droite, valeur);
    } else {
        // trouvé
        if (racine->gauche == NULL) {              // cas 1 et 2
            Noeud *enfant = racine->droite;
            free(racine);
            return enfant;
        }
        if (racine->droite == NULL) {
            Noeud *enfant = racine->gauche;
            free(racine);
            return enfant;
        }
        // cas 3
        Noeud *succ = minimum(racine->droite);
        racine->valeur = succ->valeur;
        racine->droite = supprimer(racine->droite, succ->valeur);
    }
    return racine;
}
```

Le cas 3 ne libère pas `racine` : il **écrase sa valeur** puis se rappelle pour
supprimer le successeur, dont la suppression relève forcément du cas 1 ou 2 (il
n'a pas d'enfant gauche, sinon il ne serait pas le minimum). La récursion ne peut
donc pas s'enfoncer indéfiniment.

## Le problème : la dégénérescence

Toutes les opérations sont O(h). Reste à savoir ce que vaut `h`.

**Arbre équilibré** : `h ≈ log₂ n`. Pour un million de nœuds, `h ≈ 20`.

**Arbre dégénéré** : `h = n - 1`.

Et voici comment on l'obtient, sans le faire exprès :

```c
inserer(racine, 10);
inserer(racine, 20);
inserer(racine, 30);
inserer(racine, 40);
```

```
   10
     \
      20
        \
         30
           \
            40
```

**Insérer des données triées produit une liste chaînée.** Toutes les opérations
retombent à O(n), et la structure ne sert plus à rien — elle est même pire qu'une
liste, avec son pointeur inutilisé par nœud.

C'est la même leçon que le pivot du tri rapide : les données triées sont le pire
cas, et les données réelles sont souvent triées.

## Les arbres équilibrés

La solution consiste à **rééquilibrer** après chaque insertion et suppression,
par des **rotations** — des recâblages locaux de trois pointeurs qui préservent
la propriété d'ordre tout en réduisant la hauteur.

```
   Rotation droite sur 30 :

        30                  20
       /  \                /  \
     20    D      →      10    30
    /  \                      /  \
  10    C                    C    D
```

Vérifiez : l'ordre infixe est `10 C 20 D 30` avant, `10 C 20 D 30`... pardon,
`10, C, 20, D, 30` dans les deux cas. La rotation ne change pas le tri, elle
change la forme.

Deux familles principales :

**AVL** — la différence de hauteur entre les deux sous-arbres de chaque nœud ne
dépasse jamais 1. Très strictement équilibré, donc recherche très rapide, mais
plus de rotations à l'insertion.

**Rouge-noir** — équilibre plus lâche (la hauteur peut aller jusqu'à `2 log n`),
moins de rotations. C'est le meilleur compromis en pratique, et c'est ce qu'on
trouve dans `std::map` en C++, `TreeMap` en Java, et dans l'ordonnanceur CFS du
noyau Linux.

Les deux garantissent `h = O(log n)` **quel que soit l'ordre d'insertion**.

Leur implémentation dépasse le cadre de ce cours. Ce qu'il faut en retenir : un
ABR non équilibré est une structure d'apprentissage, pas une structure de
production. En production, on utilise un arbre équilibré ou une table de hachage.

## Arbre contre table de hachage

| | ABR équilibré | Table de hachage |
|---|---|---|
| Recherche | O(log n) | **O(1)** moyen |
| Insertion | O(log n) | **O(1)** moyen |
| Pire cas | O(log n) **garanti** | O(n) |
| Parcours **trié** | **O(n), gratuit** | impossible sans trier |
| Recherche par intervalle | **oui** | non |
| Minimum / maximum | O(log n) | O(n) |
| Mémoire | 2 pointeurs par nœud | dépend du facteur de charge |

La table de hachage gagne sur la vitesse brute. L'arbre gagne sur **l'ordre** :
« tous les éléments entre 10 et 50 », « le plus petit élément supérieur à x »,
« parcourir dans l'ordre » — autant de questions qu'une table de hachage ne sait
pas traiter.

C'est le critère de choix, et il est net : **avez-vous besoin de l'ordre ?**

## Ce qu'il faut retenir

- Un ABR : gauche < nœud < droite, **récursivement et globalement**.
- Recherche, insertion, suppression : **O(h)**.
- `h ≈ log n` si équilibré, `h = n` si dégénéré. **Insérer trié dégénère.**
- Le parcours **infixe donne les valeurs triées** — le test de validité.
- Le parcours **suffixe est le seul correct pour libérer** l'arbre.
- La suppression à deux enfants passe par le **successeur infixe** (minimum du
  sous-arbre droit).
- En production : arbre équilibré (AVL, rouge-noir) ou table de hachage.

---

## À faire maintenant

**1.** Construis un ABR avec `50, 30, 70, 20, 40, 60, 80`. Affiche les quatre
parcours. Vérifie que l'infixe est trié.

**2.** Écris `hauteur(Noeud *)` récursive :
`1 + max(hauteur(gauche), hauteur(droite))`, avec `-1` pour l'arbre vide.

**3.** Insère `1..100` dans l'ordre croissant. Affiche la hauteur : tu dois
obtenir **99**. Puis insère les mêmes valeurs mélangées aléatoirement : la
hauteur doit tomber autour de **10 à 12**.

Le même contenu, les mêmes opérations, un facteur 8 sur toutes les recherches.
C'est le problème de l'équilibrage, mesuré.
