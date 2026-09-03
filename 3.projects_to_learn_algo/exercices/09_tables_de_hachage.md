# Exercices — Tables de hachage

Support : [09_tables_de_hachage.md](../lessons/09_tables_de_hachage.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

Vérifie l'absence de fuite : cette structure alloue les clés **et** les entrées.

---

# Niveau 1 — La fonction de hachage

## 1.1 — La mauvaise, et ses défauts

Implémente la somme des caractères modulo la taille.

Hache `"abc"`, `"cba"`, `"bac"`. Trois indices identiques.

Puis hache 1 000 mots de 5 à 8 lettres avec une taille de 1 000. Affiche la
plage des indices obtenus : de quel minimum à quel maximum ?

## 1.2 — djb2

Implémente-la. Refais les deux tests du 1.1.

`"abc"` et `"cba"` donnent-ils maintenant des indices différents ?

## 1.3 — Mesurer la dispersion

Écris `analyser_dispersion(fonction, mots, nb_mots, taille)` qui affiche :

- le nombre de cases vides
- le nombre de cases avec exactement 1 élément
- la longueur de la plus longue chaîne
- l'écart-type des longueurs

Compare la mauvaise fonction et djb2 sur 10 000 mots avec `taille = 10000`.

## 1.4 — Un histogramme

Affiche la distribution des longueurs de chaîne :

```
longueur 0 : ████████ (2103 cases)
longueur 1 : ████████████████ (4210 cases)
longueur 2 : ████████ (2087 cases)
...
```

Sur une bonne fonction de hachage avec `α = 1`, la distribution suit une loi de
Poisson : environ 37 % de cases vides, 37 % avec un élément, 18 % avec deux.

Vérifie sur tes chiffres.

## 1.5 — Le paradoxe des anniversaires

Table de 365 cases. Insère des clés aléatoires une par une jusqu'à la première
collision. Note le nombre d'insertions.

Répète 10 000 fois et fais la moyenne.

Tu dois obtenir environ **24**. Pas 183.

## 1.6 — Trois fonctions

Implémente aussi FNV-1a et une multiplication par un nombre premier simple.
Compare les trois avec `analyser_dispersion`.

Compare aussi leur vitesse : hache 10 000 000 de chaînes avec chacune.

Dispersion et vitesse s'opposent-elles ? Que conclus-tu ?

---

# Niveau 2 — La table par chaînage

## 2.1 — La structure

`Table`, `Entree`, `table_creer(taille)`, `table_detruire(t)`.

Utilise `calloc` pour le tableau de cases : il initialise à zéro, donc tous les
pointeurs à `NULL`.

## 2.2 — Insérer

`table_inserer(t, cle, valeur)`.

Trois comportements à décider et à documenter :

- clé déjà présente → mise à jour ou refus ?
- `malloc` échoué → que renvoie-t-on ?
- clé `NULL` → refus.

## 2.3 — Chercher

`table_chercher(t, cle, &sortie)`.

**Le `strcmp` est obligatoire.** Pour t'en convaincre, retire-le et cherche une
clé qui collisionne avec une autre. La table renverra la mauvaise valeur.

Fabrique volontairement ce cas : trouve deux mots qui collisionnent avec ta
fonction sur une petite table.

## 2.4 — Supprimer

Attention au cas où l'entrée est en tête de sa liste.

N'oublie pas de libérer la clé **et** l'entrée.

## 2.5 — Détruire

Libère toutes les entrées de toutes les listes, puis le tableau, puis la table.

Vérifie avec `leaks --atExit`. Trois niveaux d'allocation, trois niveaux de
libération.

## 2.6 — Le strdup

Écris une version qui **ne copie pas** la clé, et stocke le pointeur reçu.

Puis fais ceci :

```c
char cle[20];
strcpy(cle, "martin");
table_inserer(t, cle, 42);
strcpy(cle, "durand");            // on réutilise le tampon
table_chercher(t, "martin", &v);  // que se passe-t-il ?
```

Explique le résultat. C'est la raison du `strdup`.

---

# Niveau 3 — Le facteur de charge

## 3.1 — Mesurer l'effet

Insère 1 000 mots dans des tables de taille `100`, `500`, `1000`, `2000`, `5000`.

Pour chacune, affiche le facteur de charge et le nombre moyen de comparaisons
d'une recherche (mesure sur 10 000 recherches).

Trace la relation. Est-elle bien de la forme `1 + α/2` ?

## 3.2 — Le cas dégénéré

Remplace ta fonction de hachage par `return 0;`.

Insère 10 000 mots. Chronomètre une recherche.

Compare à la version normale. Tu viens de transformer O(1) en O(n) en changeant
une ligne.

## 3.3 — Le redimensionnement

Ajoute le doublement automatique quand `α > 0,75`.

Insère 1 000 000 d'éléments en comptant :

- le nombre de redimensionnements
- le nombre total de ré-insertions effectuées

Divise le second par 1 000 000. Le résultat doit être petit — c'est le coût
amorti.

## 3.4 — La latence

Chronomètre **chaque** insertion individuellement pendant les 1 000 000
d'insertions. Affiche les 20 plus lentes.

Correspondent-elles aux redimensionnements ? De quel facteur la plus lente
dépasse-t-elle la médiane ?

C'est le problème du O(1) amorti : la moyenne est excellente, le pire cas ne
l'est pas. Explique en quoi c'est un problème dans un système temps réel.

## 3.5 — Oublier de re-hacher

Dans `table_agrandir`, recopie les listes telles quelles au lieu de réinsérer.

Insère 100 mots, force un redimensionnement, puis cherche les 100 mots.

Combien en retrouves-tu ? Explique.

---

# Niveau 4 — L'adressage ouvert

## 4.1 — Sondage linéaire

Réimplémente la table sans listes : en cas de collision, `h = (h + 1) % taille`.

Un tableau de structures, avec un champ d'état : `LIBRE`, `OCCUPEE`, `SUPPRIMEE`.

## 4.2 — Le problème de la suppression

Implémente d'abord la suppression en remettant simplement la case à `LIBRE`.

Puis : insère trois clés qui collisionnent, supprime la première, cherche la
troisième.

Elle est introuvable. Explique précisément pourquoi.

## 4.3 — Les pierres tombales

Corrige avec l'état `SUPPRIMEE` : la recherche continue à travers, l'insertion
peut réutiliser la case.

Refais le test du 4.2.

## 4.4 — L'accumulation

Fais 100 000 cycles d'insertion/suppression sur une table de 1 000 cases.

Compte les cases `SUPPRIMEE`. Que devient la performance de la recherche ?

Comment y remédier ? (Réponse : reconstruire périodiquement. Implémente-le.)

## 4.5 — Le duel

Compare chaînage et adressage ouvert sur 1 000 000 d'insertions puis
1 000 000 de recherches, avec `α = 0,5` et `α = 0,9`.

Lequel gagne, et pourquoi ? Le résultat change-t-il selon `α` ?

*Piste d'explication :* comptez les `malloc` de chaque version, et pensez au cache.

## 4.6 — Sondage quadratique

`h = (h_initial + i²) % taille`.

Cela évite le **regroupement primaire** : avec le sondage linéaire, les zones
occupées s'agglomèrent et s'allongent mutuellement.

Compare la longueur moyenne des sondages entre linéaire et quadratique à
`α = 0,8`.

---

# Niveau 5 — Applications

## 5.1 — Compter les mots

Lis un fichier texte, compte les occurrences de chaque mot.

Compare à la version par ABR (exercice 5.1 de la leçon 8) : temps d'exécution, et
**capacité à afficher les mots dans l'ordre alphabétique**.

Le second point est décisif. Formule la conclusion.

## 5.2 — Détecter des doublons

Détermine si un tableau de 1 000 000 d'entiers contient un doublon.

Trois versions : double boucle O(n²), tri puis parcours O(n log n), table de
hachage O(n).

Chronomètre les trois. La première ne terminera peut-être pas — estime son temps.

## 5.3 — Deux sommes

Étant donné un tableau et une cible, trouve deux éléments dont la somme vaut la
cible.

Version naïve O(n²). Version table de hachage O(n) : pour chaque `x`, cherche
`cible - x` dans la table.

C'est le cas d'école du troc temps/espace.

## 5.4 — Grouper les anagrammes

Groupe les mots qui sont anagrammes entre eux.

*Indice :* la clé est le mot avec ses lettres triées. `"chien"` et `"niche"`
donnent tous deux `"cehin"`.

Teste sur un dictionnaire.

## 5.5 — Un cache LRU

Table de hachage + liste doublement chaînée. Capacité fixe ; quand elle est
atteinte, on évince l'élément le moins récemment utilisé.

- `get(cle)` : O(1), et déplace l'élément en tête de la liste.
- `put(cle, valeur)` : O(1), évince la queue si nécessaire.

C'est la structure la plus utilisée en pratique de tout ce cours — dans les
processeurs, les bases de données, les navigateurs.

C'est aussi le projet de cette leçon. Prends le temps.

## 5.6 — Un index inversé

Pour chaque mot d'un ensemble de fichiers, la liste des fichiers où il apparaît.

Puis une recherche : donne un mot, obtiens les fichiers.

C'est le principe d'un moteur de recherche, en très réduit.

---

# Auto-évaluation

- [ ] Quelles sont les trois exigences d'une fonction de hachage ?
- [ ] Pourquoi les collisions sont-elles inévitables ?
- [ ] Pourquoi le `strcmp` après le hachage est-il obligatoire ?
- [ ] Pourquoi la table doit-elle copier ses clés ?
- [ ] Qu'est-ce que le facteur de charge, et quel seuil vise-t-on ?
- [ ] Pourquoi faut-il re-hacher lors d'un redimensionnement ?
- [ ] Quand préférer un arbre à une table de hachage ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **La fonction `statistiques()`** — écris-la tôt, appelle-la partout. Sur une
   table de hachage, on ne débogue pas sans mesurer la distribution.
2. **Tester avec une petite table** (10 cases) et peu de clés : les collisions
   deviennent fréquentes et observables à l'œil.
3. **`-fsanitize=address`** — cette structure fait beaucoup d'allocations
   imbriquées, et c'est là que les erreurs de libération se logent.
