# Listes chaînées

## Ce que le tableau ne sait pas faire

Un tableau C a deux propriétés, et elles sont indissociables : ses éléments sont
**contigus** en mémoire, et sa taille est **fixée** à la création.

La contiguïté est un cadeau : `tab[i]` se calcule en une addition
(`adresse_base + i × sizeof(élément)`), donc l'accès est O(1), et le cache du
processeur charge les éléments voisins gratuitement.

La taille fixe est le prix. Insérer un élément au début d'un tableau de `n`
éléments oblige à décaler les `n` autres : O(n). L'agrandir oblige à réallouer et
recopier.

La liste chaînée fait le choix inverse. Elle abandonne la contiguïté, et gagne
l'insertion en O(1). Comprendre ce troc, c'est comprendre pourquoi les deux
structures coexistent.

## La structure

Chaque élément — un **nœud** — contient sa valeur et l'adresse du suivant.

```c
typedef struct Noeud {
    int valeur;
    struct Noeud *suivant;
} Noeud;
```

Notez `struct Noeud *suivant` et non `Noeud *suivant` : au moment où le
compilateur lit cette ligne, le `typedef` n'est pas encore terminé. Le nom
`Noeud` n'existe pas. Le nom de structure `struct Noeud`, lui, est déjà déclaré
par la ligne d'ouverture. C'est la raison d'être du nom répété.

En mémoire, rien n'est contigu :

```
tete
 │
 ▼
┌────┬───┐    ┌────┬───┐    ┌────┬──────┐
│ 12 │ ●─┼───►│ 5  │ ●─┼───►│ 38 │ NULL │
└────┴───┘    └────┴───┘    └────┴──────┘
 0x7f2a         0x91c4        0x3e08
```

Les adresses sont quelconques. Le seul lien entre les nœuds est le pointeur
`suivant`. Le `NULL` du dernier marque la fin — exactement comme le `'\0'` d'une
chaîne C.

**Perdre le pointeur de tête, c'est perdre toute la liste.** Aucun moyen de la
retrouver, et la mémoire reste allouée : c'est une fuite.

## Les opérations

### Insertion en tête — O(1)

```c
Noeud *inserer_tete(Noeud *tete, int valeur) {
    Noeud *nouveau = malloc(sizeof(Noeud));
    if (nouveau == NULL) {
        return tete;                 // échec : la liste reste intacte
    }
    nouveau->valeur = valeur;
    nouveau->suivant = tete;
    return nouveau;                  // la nouvelle tête
}
```

**L'ordre des deux affectations n'est pas négociable.** Si on écrivait
`tete = nouveau` avant `nouveau->suivant = tete`, le nouveau nœud pointerait sur
lui-même et le reste de la liste serait perdu.

La fonction **renvoie** la nouvelle tête, et l'appelant doit la récupérer :

```c
tete = inserer_tete(tete, 42);
```

C'est la convention la plus simple. L'alternative — passer `Noeud **tete` et
modifier sur place — évite l'oubli mais ajoute un niveau d'indirection. Choisissez
une convention et tenez-la dans tout le fichier.

C'est le O(1) qui justifie la structure : quelle que soit la taille de la liste,
insérer en tête coûte trois opérations.

### Insertion en queue — O(n)

```c
Noeud *inserer_queue(Noeud *tete, int valeur) {
    Noeud *nouveau = malloc(sizeof(Noeud));
    if (nouveau == NULL) return tete;
    nouveau->valeur = valeur;
    nouveau->suivant = NULL;

    if (tete == NULL) {
        return nouveau;              // liste vide : le nouveau est la tête
    }

    Noeud *courant = tete;
    while (courant->suivant != NULL) {
        courant = courant->suivant;
    }
    courant->suivant = nouveau;
    return tete;
}
```

Il faut parcourir jusqu'au bout : O(n). Notez la condition
`courant->suivant != NULL` et non `courant != NULL` — on veut s'arrêter **sur**
le dernier nœud, pas après lui.

Si on insère souvent en queue, on garde un pointeur `queue` en plus du pointeur
`tete`, et l'opération redevient O(1).

### Recherche — O(n)

```c
Noeud *rechercher(Noeud *tete, int valeur) {
    for (Noeud *courant = tete; courant != NULL; courant = courant->suivant) {
        if (courant->valeur == valeur) {
            return courant;
        }
    }
    return NULL;
}
```

**Aucune recherche dichotomique n'est possible sur une liste chaînée**, même
triée. Il n'y a pas d'élément « du milieu » accessible : pour l'atteindre, il
faudrait déjà parcourir la moitié de la liste. La dichotomie a besoin de l'accès
indexé, donc de la contiguïté.

C'est la contrepartie la plus lourde du choix de structure.

### Suppression — O(n)

L'opération la plus délicate. Il faut le nœud **précédent** pour recâbler.

```c
Noeud *supprimer(Noeud *tete, int valeur) {
    if (tete == NULL) return NULL;

    if (tete->valeur == valeur) {          // cas particulier : la tête
        Noeud *nouvelle_tete = tete->suivant;
        free(tete);
        return nouvelle_tete;
    }

    Noeud *courant = tete;
    while (courant->suivant != NULL && courant->suivant->valeur != valeur) {
        courant = courant->suivant;
    }

    if (courant->suivant != NULL) {
        Noeud *a_supprimer = courant->suivant;
        courant->suivant = a_supprimer->suivant;    // on court-circuite
        free(a_supprimer);
    }
    return tete;
}
```

Trois pièges, tous classiques :

**Récupérer `suivant` avant de libérer.** Après `free(a_supprimer)`, lire
`a_supprimer->suivant` est un accès à de la mémoire libérée. Le programme
fonctionnera souvent — la mémoire n'est pas effacée — puis échouera un jour, dans
des conditions impossibles à reproduire.

**L'ordre du `&&`.** `courant->suivant != NULL` doit venir **avant**
`courant->suivant->valeur`. Le court-circuit du C garantit qu'on ne déréférence
jamais `NULL`.

**Le cas de la tête est particulier.** Elle n'a pas de précédent.

### Libérer la liste

```c
void liberer_liste(Noeud *tete) {
    while (tete != NULL) {
        Noeud *suivant = tete->suivant;   // sauvegarder AVANT de libérer
        free(tete);
        tete = suivant;
    }
}
```

Le même piège, en boucle. Chaque `malloc` a son `free` : c'est la règle du C, et
une liste chaînée en fait un par nœud.

```sh
# macOS
leaks --atExit -- ./codes/liste
# Linux
valgrind --leak-check=full ./codes/liste
```

Un programme qui manipule des listes doit être vérifié ainsi. Une fuite ne se
voit pas à l'exécution.

## Le pointeur double, ou comment éliminer les cas particuliers

Tout le code ci-dessus traite la tête à part. C'est répétitif et c'est là que
les bugs se logent. Il existe une technique pour l'éviter : manipuler un
**pointeur vers pointeur**.

```c
void supprimer_v2(Noeud **tete, int valeur) {
    Noeud **courant = tete;

    while (*courant != NULL) {
        if ((*courant)->valeur == valeur) {
            Noeud *a_supprimer = *courant;
            *courant = a_supprimer->suivant;
            free(a_supprimer);
            return;
        }
        courant = &(*courant)->suivant;
    }
}
```

`courant` ne pointe plus sur un nœud, mais sur **le pointeur qui mène au nœud** —
soit `tete` lui-même au premier tour, soit le champ `suivant` d'un nœud ensuite.
Écrire dans `*courant` modifie donc le bon pointeur dans les deux cas, sans
distinguer.

Le cas particulier de la tête disparaît. C'est plus dense à lire, et c'est la
forme qu'on trouve dans le noyau Linux. Si elle ne vous paraît pas évidente,
dessinez les deux niveaux d'indirection sur papier — c'est la seule façon de
l'ancrer.

## Liste doublement chaînée

On ajoute un pointeur vers le précédent.

```c
typedef struct NoeudD {
    int valeur;
    struct NoeudD *suivant;
    struct NoeudD *precedent;
} NoeudD;
```

**Ce qu'on gagne** : parcours dans les deux sens, et surtout suppression d'un
nœud en **O(1)** quand on a déjà son adresse — plus besoin de chercher le
précédent.

**Ce qu'on paie** : un pointeur de plus par nœud (8 octets sur une machine
64 bits), et deux fois plus de liens à maintenir cohérents à chaque opération.

Sur des `int` de 4 octets, un nœud simplement chaîné en occupe déjà 16 (4 de
valeur, 8 de pointeur, 4 de remplissage d'alignement). Doublement chaîné, 24.
Six fois la taille de la donnée utile. C'est un coût réel, à peser.

## Tableau contre liste chaînée

| Opération | Tableau | Liste chaînée |
|---|---|---|
| Accès à l'indice `i` | **O(1)** | O(n) |
| Recherche (non trié) | O(n) | O(n) |
| Recherche (trié) | **O(log n)** | O(n) |
| Insertion en tête | O(n) | **O(1)** |
| Insertion en queue | O(1) amorti | O(n) ou O(1) avec pointeur de queue |
| Suppression en tête | O(n) | **O(1)** |
| Mémoire par élément | la donnée | la donnée **+ 8 octets** |
| Cache processeur | **excellent** | mauvais |
| Taille | fixe ou réallouée | dynamique |

### La ligne « cache » mérite un développement

Les deux structures parcourues entièrement sont O(n). En pratique, le tableau est
souvent **5 à 10 fois plus rapide**.

Le processeur ne lit pas la mémoire octet par octet : il charge des **lignes de
cache** de 64 octets. Sur un tableau d'entiers, une lecture charge les 16 éléments
suivants — les 15 accès d'après sont gratuits.

Sur une liste chaînée, les nœuds sont dispersés. Chaque `courant->suivant` peut
tomber n'importe où en mémoire, et provoquer un défaut de cache. Pire, le
processeur ne peut pas anticiper : il doit **lire** le nœud courant pour savoir
où aller ensuite. C'est une dépendance en chaîne, impossible à paralléliser.

Big-O ne voit rien de tout cela. C'est la raison pour laquelle, en pratique
moderne, on utilise beaucoup moins de listes chaînées que ne le suggèrent les
cours d'algorithmique.

## Quand choisir la liste chaînée

**Oui** quand :

- on insère et supprime beaucoup **en tête** ou à une position déjà connue ;
- la taille varie énormément et imprévisiblement ;
- on ne peut pas se permettre la recopie d'une réallocation (temps réel) ;
- on construit une **pile** ou une **file** — les deux prochaines leçons ;
- les éléments sont gros : le surcoût de 8 octets devient négligeable, et on
  évite de recopier de grosses structures.

**Non** quand :

- on accède par indice ;
- on parcourt souvent tout le contenu (le cache décide) ;
- les éléments sont petits et nombreux ;
- **par défaut** : un tableau dynamique est presque toujours le bon premier choix.

## Ce qu'il faut retenir

- Un nœud = une valeur + l'adresse du suivant. Rien n'est contigu.
- Insertion et suppression en tête : **O(1)**. C'est la seule vraie raison de
  choisir cette structure.
- Accès indexé et recherche dichotomique : **impossibles** en mieux que O(n).
- Toujours sauvegarder `suivant` **avant** de `free`.
- Le court-circuit du `&&` protège les déréférencements ; l'ordre des conditions
  est significatif.
- Le **pointeur double** élimine les cas particuliers de tête.
- Le coût réel du cache ne figure pas dans Big-O, et il est souvent décisif.

---

## À faire maintenant

Construis une liste de 10 entiers et écris `afficher` pour la visualiser :

```
12 -> 5 -> 38 -> NULL
```

Puis, dans l'ordre :

1. Insère en tête, en queue, supprime au milieu, supprime la tête.
2. Écris `longueur` — récursive et itérative. Compare-les sur une liste de
   500 000 nœuds : la récursive doit s'écraser. Tu sais maintenant pourquoi.
3. Vérifie l'absence de fuite avec `leaks --atExit -- ./codes/liste`.

Un programme sans fuite, ou il n'est pas terminé.
