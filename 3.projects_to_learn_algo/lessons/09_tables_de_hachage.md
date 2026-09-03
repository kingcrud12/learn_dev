# Tables de hachage

## L'idée qui semble trop belle

Chercher dans un tableau non trié : O(n). Trié : O(log n). Dans un arbre
équilibré : O(log n).

La table de hachage fait de la recherche en **O(1) en moyenne**. Le temps ne
dépend pas du nombre d'éléments. Un million d'entrées se cherche aussi vite que
dix.

Il y a une contrepartie, elle est importante, et la moitié de cette leçon
consiste à l'exposer honnêtement.

## Le point de départ : l'accès direct

Vous voulez associer un âge à un numéro d'employé, entre 0 et 999.

```c
int ages[1000];
ages[347] = 42;                 // écriture O(1)
printf("%d\n", ages[347]);      // lecture O(1)
```

C'est parfait, et ce n'est pas une table de hachage : c'est un tableau. L'accès
est O(1) parce que l'indice **est** la clé.

Le problème surgit dès que la clé n'est plus un petit entier.

- Clé = un numéro de sécurité sociale (15 chiffres) → il faudrait un tableau de
  10¹⁵ cases.
- Clé = une chaîne de caractères → un tableau ne s'indexe pas par `"martin"`.

Il faut donc une fonction qui transforme n'importe quelle clé en un indice de
tableau raisonnable. C'est la **fonction de hachage**.

```
clé quelconque → fonction de hachage → indice dans [0, taille[
```

## La fonction de hachage

### Ses exigences

**Déterministe.** La même clé donne toujours le même indice. Sans cela, on ne
retrouve jamais ce qu'on a rangé.

**Rapide.** Elle est appelée à chaque opération. Si elle coûte O(n), le O(1) est
perdu.

**Uniforme.** Elle doit disperser les clés sur tout le tableau. Une fonction qui
renvoie toujours `0` est déterministe et rapide, et parfaitement inutile.

### Une mauvaise fonction

```c
unsigned int hachage_mauvais(const char *cle, int taille) {
    unsigned int somme = 0;
    for (int i = 0; cle[i] != '\0'; i++) {
        somme += (unsigned char)cle[i];
    }
    return somme % taille;
}
```

Deux défauts, tous deux graves.

**Elle ignore l'ordre.** `"abc"`, `"cba"` et `"bac"` donnent le même indice. Sur
un jeu de clés qui sont des anagrammes ou des permutations, c'est un désastre.

**Sa plage est étroite.** Pour des mots de 8 lettres ASCII, la somme est
comprise entre ~500 et ~1000. Avec un tableau de 10 000 cases, 90 % restent
vides et tout s'entasse dans une petite zone.

### Une bonne fonction : djb2

```c
unsigned long hachage(const char *cle) {
    unsigned long h = 5381;
    int c;
    while ((c = (unsigned char)*cle++) != 0) {
        h = ((h << 5) + h) + c;      // h * 33 + c
    }
    return h;
}
```

Le `(h << 5) + h` vaut `h * 33`, écrit avec un décalage parce que c'était plus
rapide sur les machines de l'époque. Aujourd'hui le compilateur fait la
conversion tout seul ; l'écriture est restée.

Trois raisons pour lesquelles elle fonctionne :

- La multiplication à chaque caractère fait que **la position compte** :
  `"abc"` et `"cba"` divergent.
- Multiplier par un nombre impair et premier disperse bien les bits.
- Le débordement de `unsigned long` est **défini** en C (arithmétique modulo
  2⁶⁴) et joue ici le rôle d'un brassage supplémentaire. C'est le seul endroit
  du cours où un débordement est voulu — et il n'est légal que parce que le type
  est non signé. Sur un `long` signé, ce serait un comportement indéfini.

*Note :* djb2 est une fonction de hachage **non cryptographique**. Elle est faite
pour disperser, pas pour résister à un attaquant. Sur un service exposé où les
clés viennent de l'extérieur, un attaquant peut fabriquer des milliers de clés
qui collisionnent et faire retomber la table à O(n) — c'est une attaque par déni
de service bien réelle. Les langages modernes utilisent pour cela SipHash avec
une graine aléatoire par processus.

## Les collisions

Voici la contrepartie annoncée.

Le tableau a `m` cases, mais l'ensemble des clés possibles est immense. Le
pigeonnier est formel : **deux clés différentes finiront par donner le même
indice**. C'est une **collision**, et ce n'est pas un cas rare à traiter en
marge — c'est le cœur de l'implémentation.

### Le paradoxe des anniversaires

Combien faut-il insérer de clés dans un tableau de 365 cases pour avoir 50 % de
chances de collision ?

Pas 183. **Vingt-trois.**

C'est le paradoxe des anniversaires. Les collisions arrivent bien plus tôt que
l'intuition ne le suggère. Une table de hachage doit donc les gérer dès la
première ligne de code, jamais comme une amélioration ultérieure.

### Résolution par chaînage

Chaque case du tableau contient une **liste chaînée** de toutes les paires dont
la clé tombe sur cet indice.

```c
typedef struct Entree {
    char *cle;
    int valeur;
    struct Entree *suivant;
} Entree;

typedef struct {
    Entree **cases;        // tableau de pointeurs de listes
    int taille;
    int nb_elements;
} Table;
```

```
cases[0] → NULL
cases[1] → ["martin", 42] → ["durand", 31] → NULL     ← collision
cases[2] → NULL
cases[3] → ["dupont", 28] → NULL
```

**Insertion :**

```c
bool table_inserer(Table *t, const char *cle, int valeur) {
    unsigned long h = hachage(cle) % t->taille;

    for (Entree *e = t->cases[h]; e != NULL; e = e->suivant) {
        if (strcmp(e->cle, cle) == 0) {
            e->valeur = valeur;              // clé existante : mise à jour
            return true;
        }
    }

    Entree *nouvelle = malloc(sizeof(Entree));
    if (nouvelle == NULL) return false;

    nouvelle->cle = strdup(cle);             // copie : on ne fait pas confiance
    if (nouvelle->cle == NULL) {
        free(nouvelle);
        return false;
    }
    nouvelle->valeur = valeur;
    nouvelle->suivant = t->cases[h];         // insertion en tête, O(1)
    t->cases[h] = nouvelle;
    t->nb_elements++;
    return true;
}
```

**Recherche :**

```c
bool table_chercher(const Table *t, const char *cle, int *sortie) {
    unsigned long h = hachage(cle) % t->taille;

    for (Entree *e = t->cases[h]; e != NULL; e = e->suivant) {
        if (strcmp(e->cle, cle) == 0) {
            *sortie = e->valeur;
            return true;
        }
    }
    return false;
}
```

Deux points essentiels dans ce code.

**Le `strcmp` est obligatoire.** Le hachage identique ne prouve rien : deux clés
différentes peuvent tomber sur la même case. Il faut comparer les clés
elles-mêmes. Sauter ce test est le bug le plus grave possible sur cette
structure — la table renverrait la valeur d'une autre clé, silencieusement.

**Le `strdup` aussi.** Si on stockait le pointeur `cle` fourni par l'appelant, la
table dépendrait d'une mémoire qu'elle ne contrôle pas. L'appelant peut la
libérer, la réutiliser, ou la modifier. La table possède ses clés ; elle les
copie, et son `free` devra les libérer.

### Résolution par adressage ouvert

L'autre approche : pas de listes. En cas de collision, on cherche une autre case
libre dans le tableau lui-même.

```c
unsigned long h = hachage(cle) % taille;
while (cases[h].occupee && strcmp(cases[h].cle, cle) != 0) {
    h = (h + 1) % taille;              // sondage linéaire
}
```

**Avantage** : aucun `malloc` par élément, tout est contigu, excellent
comportement du cache. C'est plus rapide en pratique quand la table est peu
chargée.

**Inconvénient** : la suppression devient délicate. Vider une case briserait la
chaîne de sondage — les éléments placés après elle deviendraient introuvables. Il
faut marquer les cases comme « supprimées » plutôt que « libres », et ces marques
s'accumulent.

Le chaînage est plus simple à écrire correctement ; commencez par lui.

## Le facteur de charge

C'est le rapport `α = nb_elements / taille`. Il gouverne toute la performance.

Avec le chaînage, la longueur moyenne d'une liste est `α`. Une recherche coûte
donc `1 + α` en moyenne :

- `α = 0,5` → ~1,5 comparaison
- `α = 1,0` → ~2 comparaisons
- `α = 10` → ~11 comparaisons — on a une table de listes chaînées, plus une table
  de hachage

**Le O(1) n'est vrai que si `α` reste borné.** C'est la condition cachée derrière
l'annonce.

### Le redimensionnement

Quand `α` dépasse un seuil (0,75 est la valeur usuelle), on double la taille du
tableau et on **réinsère tout**.

```c
static bool table_agrandir(Table *t) {
    int ancienne_taille = t->taille;
    Entree **anciennes = t->cases;

    t->taille = ancienne_taille * 2;
    t->cases = calloc(t->taille, sizeof(Entree *));
    if (t->cases == NULL) {
        t->cases = anciennes;              // restaurer et signaler l'échec
        t->taille = ancienne_taille;
        return false;
    }
    t->nb_elements = 0;

    for (int i = 0; i < ancienne_taille; i++) {
        Entree *e = anciennes[i];
        while (e != NULL) {
            Entree *suivant = e->suivant;
            table_inserer(t, e->cle, e->valeur);
            free(e->cle);
            free(e);
            e = suivant;
        }
    }
    free(anciennes);
    return true;
}
```

**Il faut réellement re-hacher.** L'indice dépend de `% taille` ; changer la
taille change tous les indices. Recopier les listes telles quelles produirait une
table où rien n'est retrouvable.

Cette opération est O(n). Elle est rare — la taille double, donc les
redimensionnements s'espacent exponentiellement — et le coût **amorti** par
insertion reste O(1). C'est le même raisonnement que pour un tableau dynamique.

Conséquence pratique : une insertion est O(1) *en moyenne amortie*, mais une
insertion **particulière** peut coûter O(n). Dans un système temps réel, cette
latence imprévisible est parfois rédhibitoire — et c'est une raison de préférer
un arbre équilibré, dont le O(log n) est garanti à chaque opération.

## Récapitulatif des complexités

| Opération | Moyen | Pire |
|---|---|---|
| Recherche | **O(1)** | O(n) |
| Insertion | **O(1)** amorti | O(n) |
| Suppression | **O(1)** | O(n) |
| Parcours trié | — | **impossible** |

Le pire cas O(n) survient quand toutes les clés collisionnent. Improbable avec
une bonne fonction de hachage sur des données ordinaires ; **provoquable** par un
attaquant qui connaît la fonction.

## Table de hachage ou arbre équilibré ?

| | Table de hachage | ABR équilibré |
|---|---|---|
| Recherche | **O(1)** moyen | O(log n) garanti |
| Pire cas | O(n) | **O(log n)** |
| Ordre trié | non | **oui, gratuit** |
| Intervalle `[a, b]` | non | **oui** |
| Min / max | O(n) | **O(log n)** |
| Latence | irrégulière (resize) | **régulière** |
| Vitesse brute | **meilleure** | bonne |

**Table de hachage** quand on ne fait que des recherches par clé exacte : un
cache, un index, un dictionnaire, un comptage d'occurrences.

**Arbre** quand l'ordre compte, quand il faut des intervalles, ou quand la
latence doit être prévisible.

## Ce qu'il faut retenir

- Une fonction de hachage transforme une clé quelconque en indice de tableau :
  déterministe, rapide, uniforme.
- Les **collisions sont inévitables** (paradoxe des anniversaires) — le chaînage
  ou l'adressage ouvert les traite.
- **Toujours comparer les clés avec `strcmp`** après avoir trouvé la case.
- La table **possède ses clés** : `strdup` à l'insertion, `free` à la destruction.
- Le **facteur de charge** doit rester sous ~0,75, sinon le O(1) s'évanouit.
- Le redimensionnement **re-hache tout** ; O(n) ponctuel, O(1) amorti.
- Aucun ordre : pas de parcours trié, pas d'intervalle, pas de minimum.

---

## À faire maintenant

**1.** Implémente la table par chaînage : `inserer`, `chercher`, `supprimer`,
`detruire`. Vérifie l'absence de fuite avec `leaks --atExit`.

**2.** Écris `statistiques()` qui affiche, pour une table donnée : le facteur de
charge, le nombre de cases vides, la longueur de la plus longue chaîne.

Insère 1 000 mots avec `taille = 100`, puis avec `taille = 1000`. Compare la
plus longue chaîne dans les deux cas.

**3.** Remplace `hachage` par `return 0;` — une fonction déterministe, rapide,
et catastrophique. Réinsère les 1 000 mots et mesure une recherche.

Tu viens de transformer une table de hachage en liste chaînée, en changeant une
ligne. C'est la démonstration que le O(1) n'est pas dans la structure : il est
dans la fonction de hachage.
