# Projet 2 — Moteur d'itinéraires

**Prérequis :** leçons 1, 2, 7, 10. La leçon 9 aide pour l'index des noms.

## Le problème

Un réseau de transport : des stations, des liaisons entre elles, des temps de
trajet. On veut répondre à :

- comment aller de Châtelet à La Défense ?
- quel est l'itinéraire **le plus rapide** ?
- celui avec le **moins de correspondances** ?
- que se passe-t-il si la ligne 1 est interrompue ?
- quelles stations sont accessibles en moins de 20 minutes ?

Ces questions ne se répondent pas avec le même algorithme, et c'est le sujet du
projet.

## Le piège à identifier d'emblée

« Le plus court chemin » est ambigu, et l'ambiguïté n'est pas technique — elle
est dans le problème.

**Le moins de correspondances** ≠ **le plus rapide**. Un trajet direct de 40
minutes contre deux trajets de 8 minutes avec une correspondance : lequel est
« le plus court » ?

- Moins d'arrêts → graphe **non pondéré** → **BFS**.
- Le plus rapide → graphe **pondéré** → **Dijkstra**.

Ce sont deux problèmes différents, deux algorithmes différents, deux réponses
différentes. Un moteur d'itinéraires sérieux propose les deux, et laisse
l'utilisateur choisir.

Écris cette distinction dans `DECISIONS.md` avant de coder. C'est la décision de
domaine du projet.

## Les décisions de modélisation

### 1. Qu'est-ce qu'un sommet ?

Deux modélisations, et elles ne sont pas équivalentes.

**Une station = un sommet.** Simple. Mais une correspondance devient invisible :
passer de la ligne 1 à la ligne 4 à Châtelet ne coûte rien, alors qu'en réalité
c'est 5 minutes de marche.

**Un couple (station, ligne) = un sommet.** Châtelet-ligne-1 et Châtelet-ligne-4
sont deux sommets distincts, reliés par une arête de correspondance pesant
5 minutes.

La seconde modélisation est plus fidèle et multiplie le nombre de sommets par le
nombre moyen de lignes par station. Elle rend le problème **correct**.

C'est le cœur du projet : le graphe ne préexiste pas au problème. On le construit
en fonction de ce qu'on veut calculer. Mal le modéliser produit des réponses
fausses qu'aucun débogage d'algorithme ne rattrapera.

### 2. Quelle représentation ?

Un réseau de transport est très **creux** : quelques centaines de stations, deux
à quatre voisins chacune.

Calcule les deux mémoires pour `V = 400`. La conclusion sera nette.

### 3. Comment nommer les sommets ?

Les algorithmes travaillent sur des indices `0..V-1`. L'utilisateur tape
« Châtelet ».

Il faut donc une correspondance nom ↔ indice, dans les deux sens :

- nom → indice : une **table de hachage** (leçon 9), ou un tableau trié avec
  dichotomie (leçon 2) ;
- indice → nom : un simple tableau.

Ce n'est pas un détail d'implémentation. C'est un composant à part entière, et il
mérite son module.

## Le format de données

Un fichier texte, à écrire à la main pour commencer.

```
# stations
0 Chatelet
1 Louvre
2 Concorde
3 Etoile
4 La_Defense

# liaisons : depuis vers minutes ligne
0 1 2 1
1 2 3 1
2 3 4 1
3 4 6 1
0 2 5 14
```

Commence avec 5 stations que tu peux vérifier à la main. **Ne débogue jamais un
algorithme de graphe sur un réseau de 300 stations.**

Une fois que tout marche, passe à un réseau réel : les données du métro parisien
sont disponibles en ouvert, et le passage à l'échelle révélera les problèmes de
performance que les 5 stations cachaient.

## Architecture proposée

```
02_moteur_itineraires/
├── Makefile
├── DECISIONS.md
├── src/
│   ├── main.c            # boucle interactive
│   ├── graphe.c/.h       # listes d'adjacence pondérées
│   ├── noms.c/.h         # correspondance nom ↔ indice
│   ├── chargeur.c/.h     # lecture du fichier réseau
│   ├── bfs.c/.h          # moins de correspondances
│   ├── dijkstra.c/.h     # le plus rapide
│   └── tas.c/.h          # file de priorité
└── data/
    ├── mini.txt          # 5 stations, vérifiable à la main
    └── reseau.txt        # le réseau complet
```

Les interfaces à figer avant d'écrire les `.c` :

```c
// resultat d'une recherche d'itineraire
typedef struct {
    int  *chemin;        // suite d'indices de sommets
    int   longueur;      // nombre de sommets du chemin
    int   cout;          // minutes, ou nombre d'arêtes selon l'algorithme
    bool  trouve;
} Itineraire;

Itineraire itineraire_bfs(const Graphe *g, int depart, int arrivee);
Itineraire itineraire_dijkstra(const Graphe *g, int depart, int arrivee);
void       itineraire_liberer(Itineraire *it);
```

Une **même structure de résultat** pour les deux algorithmes : c'est ce qui
permet à `main.c` d'afficher les deux réponses côte à côte sans code
spécifique.

## Les tranches de construction

**Tranche 1 — charger et afficher.** Lire `mini.txt`, construire le graphe,
afficher les listes d'adjacence. Vérifie chaque arête à la main.

**Tranche 2 — le BFS.** Le plus court en nombre d'arêtes, avec reconstruction du
chemin. Vérifie sur `mini.txt`, où tu connais la réponse.

**Tranche 3 — l'interface utilisateur.** Taper deux noms, obtenir un itinéraire
lisible :

```
Chatelet -> Louvre -> Concorde -> Etoile -> La_Defense
4 stations, 15 minutes
```

C'est ici que la correspondance nom ↔ indice devient nécessaire.

**Tranche 4 — Dijkstra en O(V²).** Sans tas : une recherche linéaire du minimum.
C'est plus simple, c'est suffisant pour 400 sommets, et cela permet de valider
l'algorithme avant d'ajouter une structure.

Construis un cas où BFS et Dijkstra donnent des réponses **différentes**. C'est
le test qui prouve que tu as compris la distinction.

**Tranche 5 — le tas binaire.** Remplace la recherche linéaire. Mesure le gain,
et sois honnête : sur 400 sommets, il sera peut-être négligeable. Note-le.

**Tranche 6 — les extras.** Interruption de ligne, isochrones, correspondances
pondérées.

## Le tas binaire

C'est la seule structure nouvelle du projet, et elle mérite un module à part.

Un tas binaire min est un arbre binaire **complet** stocké dans un tableau, où
chaque nœud est plus petit que ses enfants.

```
indices :  0   1   2   3   4   5
valeurs : [2] [5] [3] [9] [7] [4]

              2
            /   \
           5     3
          / \   /
         9   7 4
```

Les relations sont arithmétiques, sans aucun pointeur :

- enfants de `i` : `2i + 1` et `2i + 2`
- parent de `i` : `(i - 1) / 2`

Deux opérations :

- `inserer` : placer à la fin, puis **remonter** tant qu'on est plus petit que
  son parent. O(log n).
- `extraire_min` : prendre la racine, y mettre le dernier élément, puis
  **descendre** en échangeant avec le plus petit enfant. O(log n).

Écris-le et teste-le **isolément** avant de l'intégrer à Dijkstra. Insère
10 000 valeurs aléatoires, extrais-les toutes : elles doivent sortir triées.

*Au passage :* tu viens d'écrire un tri par tas, en O(n log n), en place. C'est
le troisième tri optimal, après fusion et rapide.

## Mesures attendues

| Mesure | Valeur |
|---|---|
| Sommets, arêtes du réseau | |
| Mémoire : listes d'adjacence | |
| Mémoire : matrice (calculée, pas implémentée) | |
| BFS : temps moyen sur 1 000 requêtes | |
| Dijkstra O(V²) : temps moyen | |
| Dijkstra avec tas : temps moyen | |
| Sommets visités : BFS vs Dijkstra | |
| Requêtes où BFS et Dijkstra diffèrent (%) | |

**Les questions auxquelles répondre :**

1. Sur quelle proportion des trajets les deux algorithmes donnent-ils des
   itinéraires différents ? C'est la mesure qui justifie d'avoir écrit les deux.
2. Le tas apporte-t-il un gain mesurable à cette taille ? Si non, à partir de
   combien de sommets deviendrait-il nécessaire ? Extrapole.
3. Dijkstra visite-t-il plus ou moins de sommets que le BFS ? Pourquoi ?

## Extensions

**A\*.** Dijkstra guidé par une estimation de la distance restante (à vol
d'oiseau, si tu as des coordonnées). Il visite beaucoup moins de sommets.
Compare-les — c'est la différence entre un GPS des années 90 et un GPS moderne.

**Horaires réels.** Les arêtes ne sont plus des durées fixes mais des horaires de
départ. Le temps d'attente dépend de l'heure d'arrivée à la station. Le graphe
devient **dépendant du temps** — et Dijkstra continue de fonctionner, à condition
que le coût soit monotone. Réfléchis à pourquoi.

**Trajets multi-critères.** Le plus rapide **et** le moins cher : il n'existe
généralement pas de solution optimale pour les deux. On calcule alors un
**front de Pareto** — l'ensemble des solutions qu'aucune autre ne domine sur les
deux critères.

**Robustesse.** Supprime une ligne au hasard et mesure l'allongement moyen des
trajets. Quelle ligne est la plus critique pour le réseau ?

## Ce que ce projet doit t'apprendre

- **La modélisation précède l'algorithme.** Le même réseau modélisé de deux
  façons donne des réponses différentes, et l'algorithme n'y peut rien.
- « Le plus court chemin » est une question ambiguë. Le rôle de l'ingénieur est
  de la désambiguïser avant de la résoudre.
- BFS et Dijkstra sont le même algorithme, à la structure de file près.
- Un tas binaire n'a besoin d'aucun pointeur : la structure d'arbre tient dans
  l'arithmétique des indices.
- Une optimisation asymptotique n'est pas toujours mesurable à l'échelle où l'on
  travaille. Le dire est aussi important que de l'implémenter.
