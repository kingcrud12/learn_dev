# Projet 3 — Banc de mesure des tris

**Prérequis :** leçons 1, 3, 4. Les leçons 5 et 7 pour les extensions.

## Le problème

Toutes les leçons de ce cours affirment des complexités. Ce projet consiste à
les **vérifier** — et à mesurer tout ce que la notation Big-O ne dit pas.

L'objectif est un outil en ligne de commande qui produit un rapport comparatif
exploitable :

```sh
./banc --tris tous --tailles 1000,10000,100000 --donnees toutes --repetitions 5
```

Ce n'est pas un exercice de tri : tu les as déjà écrits. C'est un exercice de
**méthode expérimentale**, et c'est une compétence à part entière.

## Pourquoi ce projet est plus difficile qu'il n'y paraît

Mesurer correctement est étonnamment délicat. Voici les pièges, tous réels, et
tous rencontrés par ceux qui font ce genre de mesure pour la première fois.

**Le compilateur supprime le code inutile.** Si le résultat du tri n'est jamais
lu, `-O2` peut éliminer l'appel entier. Ta mesure donnera 0 nanoseconde.
*Remède :* utilise le résultat — affiche une somme de contrôle, ou passe le
tableau à une fonction que le compilateur ne peut pas analyser.

**Le premier appel est plus lent.** Caches froids, pages mémoire non encore
allouées par le système. *Remède :* un tour de chauffe non mesuré.

**Un seul essai ne vaut rien.** Une interruption système, un autre processus, et
la mesure double. *Remède :* plusieurs répétitions, et on retient la **médiane**
plutôt que la moyenne — elle résiste aux valeurs aberrantes.

**`clock()` mesure le temps CPU, pas le temps écoulé.** Ce n'est pas la même
chose, et sur une machine chargée l'écart est important. Pour un banc de tri,
c'est `clock()` qu'on veut : il ignore le temps où le processus ne tourne pas.
Mais il faut le savoir et le dire.

**La résolution du chronomètre est limitée.** Pour `n = 100`, un tri dure moins
d'une microseconde — sous le seuil de mesure. *Remède :* répéter l'opération
1 000 fois et diviser.

**Les données doivent être identiques entre les algorithmes.** Comparer un tri
sur un jeu et un autre sur un jeu différent ne compare rien. *Remède :* générer
une fois, copier avant chaque tri, `srand(graine)` fixe pour la reproductibilité.

**Trier un tableau déjà trié.** Si tu tries en place et relances sans régénérer,
la deuxième mesure porte sur des données triées. C'est l'erreur la plus fréquente
et elle produit des résultats spectaculairement faux.

Ces sept points sont le vrai contenu du projet. Note-les dans `DECISIONS.md`
avec ce que tu as mis en place pour chacun.

## Ce qu'il faut mesurer

Le temps ne suffit pas. Il dépend de la machine, et il ne dit pas pourquoi.

| Mesure | Ce qu'elle révèle |
|---|---|
| Temps écoulé | la performance réelle |
| Comparaisons | le comportement algorithmique, indépendant de la machine |
| Écritures | ce qui distingue insertion et bulles à complexité égale |
| Appels récursifs | le coût caché de la récursion |
| Profondeur max | le risque de débordement de pile |
| Mémoire de pointe | le troc temps/espace |

Les comparaisons et les écritures sont **reproductibles** — elles ne dépendent
d'aucune machine. C'est ce qui rend un résultat vérifiable par quelqu'un d'autre.

## Les algorithmes à intégrer

Au minimum huit :

1. Tri par sélection
2. Tri par insertion
3. Tri à bulles (avec drapeau)
4. Tri fusion
5. Tri rapide, pivot en dernière position
6. Tri rapide, médian de trois
7. Tri hybride (rapide + insertion sous un seuil)
8. `qsort` de la bibliothèque standard, comme référence

Les deux variantes du tri rapide ne sont pas une redondance : leur écart sur les
données triées est l'un des résultats les plus parlants du banc.

## Les jeux de données

Six profils. Les deux derniers sont ceux qui révèlent le plus.

| Nom | Description | Ce qu'il révèle |
|---|---|---|
| `aleatoire` | uniforme | le cas moyen |
| `trie` | croissant | l'adaptativité, le pire cas du tri rapide naïf |
| `inverse` | décroissant | le pire cas de l'insertion |
| `presque_trie` | trié avec 1 % d'échanges | le cas le plus fréquent en pratique |
| `doublons` | 10 valeurs distinctes | la faiblesse de la partition à deux voies |
| `organ_pipe` | monte puis descend | un cas défavorable au médian de trois |

Le dernier mérite une explication : `[1,2,3,...,n/2,...,3,2,1]`. Le premier
élément, le dernier et celui du milieu sont mal placés pour la médiane de trois,
qui choisit alors systématiquement un mauvais pivot. C'est un cas construit
exprès — et il montre qu'aucune heuristique de pivot n'est à l'abri.

## Architecture proposée

```
03_banc_de_tri/
├── Makefile
├── DECISIONS.md
├── RESULTATS.md
├── src/
│   ├── main.c            # arguments de ligne de commande
│   ├── tris.c/.h         # les huit algorithmes
│   ├── stats.c/.h        # compteurs
│   ├── donnees.c/.h      # les six générateurs
│   ├── banc.c/.h         # protocole de mesure
│   └── rapport.c/.h      # sortie texte, CSV, Markdown
```

L'interface qui rend le banc extensible :

```c
typedef struct {
    long comparaisons;
    long ecritures;
    long appels_recursifs;
    int  profondeur_max;
} Stats;

typedef struct {
    const char *nom;
    void (*trier)(int *tab, int n, Stats *s);
    bool stable;
    bool en_place;
    const char *complexite_pire;
} Algorithme;

static const Algorithme ALGOS[] = {
    {"selection",  tri_selection,  false, true,  "O(n^2)"},
    {"insertion",  tri_insertion,  true,  true,  "O(n^2)"},
    /* ... */
};
```

Un tableau de descripteurs, et la boucle de mesure ne connaît aucun algorithme en
particulier. **Ajouter un neuvième tri ne doit demander qu'une ligne** dans ce
tableau.

C'est le point d'architecture du projet : la boucle de mesure et les algorithmes
mesurés ne se connaissent pas.

## Le protocole de mesure

Écris-le comme une procédure, et respecte-la :

```
pour chaque algorithme :
  pour chaque taille :
    pour chaque profil de données :
      generer(profil, taille, graine_fixe)   -> reference
      copier(reference) -> travail
      trier(travail)                          # chauffe, non mesuré

      pour i de 1 a repetitions :
        copier(reference) -> travail          # REGENERER a chaque fois
        remettre_stats_a_zero()
        t0 = clock()
        trier(travail)
        t1 = clock()
        verifier(est_trie(travail))           # sinon le resultat ne vaut rien
        enregistrer(t1 - t0, stats)

      retenir la mediane des temps
```

Le `verifier` n'est pas optionnel. Un tri rapide et faux n'est pas un résultat.

## Sécurité du banc

Deux protections nécessaires, parce que certaines combinaisons sont
catastrophiques.

**Une limite de temps.** Un tri O(n²) sur 1 000 000 d'éléments prend des heures.
Prévois un plafond, et affiche `> 60 s (interrompu)` plutôt que de bloquer.

**Un garde-fou sur les tailles.** Refuse les tris quadratiques au-delà d'un seuil
configurable, sauf si l'utilisateur passe `--force`.

Le tri rapide naïf sur données triées **plantera** au-delà de quelques dizaines
de milliers d'éléments. Ce n'est pas un bug de ton banc : c'est le résultat.
Attrape-le et rapporte-le comme tel — `SIGSEGV (debordement de pile)` est une
ligne de résultat parfaitement valide, et c'est même la plus instructive du
rapport.

## Le rapport

Trois formats :

**Texte**, pour lire immédiatement.

**CSV**, pour tracer des courbes ailleurs. C'est le format qui permet de vérifier
visuellement qu'une courbe est bien quadratique.

**Markdown**, pour `RESULTATS.md` :

```markdown
## n = 100 000, données aléatoires

| Algorithme | Temps (ms) | Comparaisons | Écritures |
|---|---|---|---|
| qsort         |    9,8 |     1 708 234 |          — |
| rapide_hybride|   10,4 |     1 743 891 |    621 004 |
| rapide_med3   |   11,7 |     1 812 445 |    698 332 |
| fusion        |   14,2 |     1 536 118 |  1 700 000 |
| insertion     | 2451,0 | 2 498 213 445 | 2 498 213 445 |
```

## Les résultats à obtenir

Le rapport doit permettre de répondre à ces sept questions. Écris les réponses
dans `RESULTATS.md`.

1. **Le temps des tris O(n²) est-il multiplié par 4 quand `n` double ?**
   Vérifie-le sur trois tailles successives.

2. **Celui des O(n log n) est-il multiplié par un peu plus de 2 ?**
   Le « un peu plus » est le `log n`. Peux-tu l'isoler ?

3. **À partir de quel `n` le tri par insertion perd-il contre le tri fusion ?**
   Le seuil est plus haut que l'intuition ne le suggère.

4. **Quel est l'écart du tri rapide naïf entre données aléatoires et triées ?**
   Sur les tailles où il ne plante pas.

5. **Le médian de trois élimine-t-il complètement le problème ?**
   Regarde le profil `organ_pipe` avant de répondre oui.

6. **Le tri fusion fait moins de comparaisons que le tri rapide. Pourquoi
   est-il plus lent ?**
   La réponse n'est pas dans le nombre d'opérations. Elle est dans leur nature :
   le tri fusion écrit dans un tampon distant, le tri rapide travaille en place
   avec des accès séquentiels. Vérifie en regardant la colonne « écritures ».

7. **`qsort` bat-il ton tri rapide ? Devrait-il ?**
   Il paie un appel de fonction indirect par comparaison, ce que tes tris ne font
   pas. S'il gagne quand même, cherche ce que fait réellement son implémentation.

## Extensions

**Tracer les courbes.** Exporte le CSV et trace temps en fonction de `n` en
échelle log-log. Une droite de pente 2 est un O(n²), une droite de pente 1 un
O(n). La pente **est** l'exposant : c'est la façon la plus directe de lire une
complexité expérimentalement.

**Tri par tas.** Le troisième tri O(n log n) : en place comme le tri rapide,
garanti au pire cas comme le tri fusion. Il est pourtant plus lent que les deux.
Mesure-le et cherche pourquoi (indice : ses accès mémoire sautent d'un facteur 2
à chaque niveau).

**Tri par comptage et tri radix.** Ils battent la borne O(n log n) sur des
entiers de plage bornée. Ajoute-les avec un profil de données adapté.

**Introsort.** Ce que fait réellement une bibliothèque standard moderne : tri
rapide, avec repli sur un tri par tas quand la profondeur de récursion dépasse
`2 log n`, et insertion sous un seuil. Il n'a **aucun** pire cas quadratique.
Implémente-le et vérifie qu'il tient sur `organ_pipe`.

**Mesurer les défauts de cache.**

```sh
perf stat -e cache-misses ./banc     # Linux
```

C'est la mesure qui explique tous les écarts que Big-O ne prédit pas.

## Ce que ce projet doit t'apprendre

- **Mesurer correctement est une compétence**, distincte de savoir programmer.
  Les sept pièges du début ne sont pas des détails.
- Un banc de mesure se conçoit pour être **étendu** : la boucle de mesure ne doit
  connaître aucun algorithme.
- Les compteurs de comparaisons et d'écritures sont **reproductibles** là où le
  temps ne l'est pas.
- Big-O prédit la **forme** de la courbe, jamais sa position. Les constantes
  décident sur les tailles réelles.
- Un crash est un résultat. Un tri qui déborde la pile sur des données triées est
  la ligne la plus instructive d'un rapport.
