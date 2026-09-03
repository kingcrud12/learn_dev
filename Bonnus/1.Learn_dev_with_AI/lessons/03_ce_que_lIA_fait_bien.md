# Ce que l'IA fait bien, ce qu'elle fait mal

## Une carte, pas une opinion

La leçon [01](01_ce_quest_un_LLM.md) a donné le mécanisme. Cette leçon en tire la
conséquence pratique : **où déléguer, où vérifier, où ne pas déléguer du tout.**

Le critère n'est pas la difficulté de la tâche. Un modèle explique un arbre
rouge-noir mieux que la plupart des tutoriels, et se trompe sur le nom d'un
paramètre de `fopen`. Ce n'est pas incohérent, et le critère qui l'explique est
le suivant :

> **L'information nécessaire est-elle dans le contexte que je fournis, ou
> massivement présente dans son entraînement ?**

Si oui, la fiabilité est haute. Si l'information est rare, récente, ou propre à
votre machine — la fiabilité s'effondre, sans que le ton ne change.

Tout ce qui suit découle de cette question.

## Ce qu'elle fait mieux qu'un tutoriel

### Expliquer à votre niveau exact

C'est son avantage décisif sur toute documentation. Un tutoriel est écrit pour un
lecteur moyen qui n'existe pas. Un modèle s'adapte à vous.

```
Explique-moi les pointeurs comme si je savais déjà ce qu'est une
variable et une adresse mémoire, mais que je n'avais jamais vu
d'indirection.
```

Et surtout, il répond à la question de suivi — celle qu'aucun tutoriel ne prévoit :

```
D'accord, mais alors pourquoi *p et p[0] donnent la même chose ?
```

Cette capacité de relance est ce qui manque le plus dans l'apprentissage en
autonomie. C'est le tutorat, disponible en permanence.

### Décoder un message d'erreur

Excellent, et sous-estimé. Les erreurs de compilation C sont notoirement
obscures :

```
error: assignment to 'char *' from incompatible pointer type 'char **'
```

Le modèle traduit, explique la cause probable, et pointe la ligne suspecte.
C'est un motif massivement présent dans son entraînement — donc dans sa zone de
haute fiabilité. Le message d'erreur est de plus **dans le contexte**, ce qui est
la seconde condition.

Le bon réflexe : coller **le message complet et le code**, pas une paraphrase.

### Comparer des approches

```
Pour un cache de 10 000 entrées avec beaucoup de lectures, table de
hachage ou arbre binaire de recherche ? Donne-moi le critère de
décision, pas juste la réponse.
```

Il expose les arbitrages. Utilisez-le pour **construire votre grille de
décision**, pas pour décider à votre place — la compétence visée est de savoir
choisir.

### Relire votre code

Le plus rentable de tous les usages en apprentissage. Vous avez fait l'effort,
vous recevez un retour immédiat sur ce qu'un développeur expérimenté verrait :

- une fuite mémoire
- un cas limite non traité (tableau vide, `n = 0`, entrée `NULL`)
- une convention non respectée
- un nommage confus

C'est l'équivalent d'une revue de code, sans attendre un relecteur.

### Générer des cas de test

Tâche ingrate que l'IA fait bien, et qui vous apprend à penser aux cas limites :

```
Voici ma fonction de recherche dichotomique : [code]
Liste les cas limites qui devraient la casser. Ne donne pas les tests
écrits, juste la liste des cas — je veux vérifier lesquels j'avais vus.
```

Le « je veux vérifier lesquels j'avais vus » est ce qui en fait un exercice
plutôt qu'un service.

### Traduire entre langages

Vous savez le faire en C, vous devez le faire en Kotlin ? La traduction
commentée est un excellent accélérateur — **à condition de connaître déjà le
concept**. Si vous ne le connaissez pas, vous obtenez du code qui marche et rien
de plus.

## Ce qu'elle fait mal

### Compter, calculer, manipuler des caractères

Vu en leçon 01 : les tokens. Le modèle ne voit pas les lettres et ne calcule
pas — il prédit.

| Demande | Résultat |
|---|---|
| Compter les `r` dans un mot | Souvent faux |
| `4871 × 3926` de tête | Bon ordre de grandeur, chiffres faux |
| Inverser une chaîne caractère par caractère | Fragile |
| Compter les lignes d'un fichier collé | Approximatif |

**La règle** : pour tout ce qui se calcule, demandez-lui d'écrire la commande, et
exécutez-la. `bc`, `wc`, `grep -c`, un script de trois lignes. La machine calcule,
le modèle rédige le calcul.

### Nommer précisément une API

Zone d'hallucination classique. Une fonction au nom crédible qui n'existe pas, un
paramètre plausible ajouté à une fonction réelle, un paquet inventé.

Vérification, toujours :

```sh
man 3 fopen           # la doc système fait autorité, pas le modèle
gcc -Wall -Wextra fichier.c -o prog   # ça compile ou non
```

Un nom de fonction qui n'existe pas ne compile pas. **Le compilateur est votre
détecteur d'hallucinations**, et il est gratuit.

### Tout ce qui est récent

Date de coupure. Versions de bibliothèques, API modifiées, outils sortis après.
Le modèle décrira avec assurance la version qu'il a connue.

Réflexe : pour toute question de version, la source est la documentation
officielle ou la machine — jamais le modèle.

### Raisonner sur ce qu'il ne voit pas

```
"Pourquoi mon programme est lent ?"
```

Il ne voit ni votre code, ni votre profil d'exécution, ni la taille de vos
données. Il produira une liste de causes courantes de lenteur : juste, générique,
inutile.

Ce n'est pas un défaut du modèle. C'est un défaut du prompt — et il vous
appartient.

### Les architectures étendues

Sur une fonction, il est fort. Sur la cohérence de quinze fichiers qui
interagissent, il perd le fil : il ne voit que ce qui est dans le contexte, et
raisonne localement. Il proposera une solution correcte **pour le fragment
montré** et incompatible avec le reste.

C'est aussi ce qui rend la revue humaine irremplaçable sur les gros changements.

## Le tableau de décision

À garder sous la main :

| Tâche | Déléguer ? | Vérification obligatoire |
|---|---|---|
| Expliquer un concept classique | **Oui** | Recouper si c'est important |
| Décoder une erreur de compilation | **Oui** | Le correctif compile-t-il ? |
| Relire mon code, lister les défauts | **Oui** | Juger chaque remarque, pas tout appliquer |
| Comparer deux approches | **Oui** | Est-ce que je saurais re-justifier le choix ? |
| Suggérer des cas limites | **Oui** | Les tester réellement |
| M'interroger sur une notion | **Oui** | — |
| Générer du code standard | **Prudence** | Compiler, tester, savoir le réécrire |
| Traduire un concept vers un autre langage | **Prudence** | Seulement si le concept est déjà acquis |
| Nommer une fonction de bibliothèque | **Non** | `man`, doc officielle, compilation |
| Calculer, compter | **Non** | Faire calculer la machine |
| Une information de version, une nouveauté | **Non** | Doc officielle, `--version` |
| **Faire un exercice à ma place** | **Jamais** | — |

La dernière ligne n'est pas une question de fiabilité. C'est une question
d'objectif : elle marcherait très bien, et c'est précisément le problème.

## Vérifier : les quatre niveaux

Toute sortie d'un modèle est une **hypothèse**. Voici comment la traiter, du
moins au plus coûteux.

**Niveau 1 — La machine tranche (secondes).** Le meilleur, à utiliser d'abord.

```sh
gcc -Wall -Wextra -g fichier.c -o prog && ./prog   # ça compile, ça tourne
man 3 strcpy                                        # la fonction existe
echo "2**10" | bc                                   # le calcul est juste
gcc --version                                       # la version réelle
```

Cela couvre la majorité des erreurs : hallucinations d'API, erreurs de syntaxe,
calculs faux.

**Niveau 2 — La source fait autorité (minutes).** Pour les questions de
comportement et de version : documentation officielle, `man`, norme du langage.

Une explication de modèle est une bonne carte pour lire la documentation. Elle ne
la remplace pas.

**Niveau 3 — Le raisonnement (le plus important).** La question à se poser
systématiquement :

> **Est-ce que je saurais réécrire et défendre ce code ?**

Si la réponse est non, la vérification a échoué — même si le code marche.
Surtout s'il marche : c'est là que la dette se cache.

**Niveau 4 — Recouper (pour ce qui compte).** Reposer la question dans une
conversation neuve, ou à un autre modèle. Des réponses divergentes signalent une
zone d'incertitude. Utile pour les choix de conception, pas pour la syntaxe.

## Le piège du code qui marche

Le cas dangereux n'est pas le code faux — il ne compile pas, vous le voyez.

C'est le code **juste** que vous ne comprenez pas.

```c
while ((*d++ = *s++));
```

Cette ligne copie une chaîne. Elle est correcte, elle est idiomatique. Si vous ne
savez pas expliquer l'ordre d'évaluation, ce que vaut l'expression, et pourquoi la
boucle s'arrête, alors elle est dans votre programme sans être dans votre tête.

Ce code passera les tests. Il passera la revue. Il vous manquera le jour où il
faudra le modifier, ou le jour où on vous demandera de l'expliquer.

**La règle du dépôt s'applique sans exception** : ne pas garder du code qu'on ne
saurait pas réécrire. L'IA rend cette règle difficile à tenir, et donc plus
importante à énoncer.

## Le cas particulier de ce parcours

Ce dépôt enseigne le C, les pointeurs, la mémoire manuelle, les structures de
données implémentées à la main. Ce sont **précisément** les tâches que l'IA
génère le mieux.

Ce n'est pas une raison d'arrêter : c'est la raison pour laquelle ce parcours
existe. Une liste chaînée s'écrit en trente secondes avec un modèle. Ce que le
module 3 enseigne n'est pas d'obtenir une liste chaînée — c'est de savoir
pourquoi le chaînage échange la contiguïté contre l'insertion, et de reconnaître
ce troc partout ailleurs.

Cette compétence-là, aucun prompt ne la fournit. Et c'est elle qui permet de
juger si le code que le modèle propose est le bon.

| Ce que l'IA produit | Ce que vous devez acquérir |
|---|---|
| Une liste chaînée qui marche | Savoir quand elle bat un tableau, et pourquoi |
| Un tri correct | Savoir lequel choisir selon les données |
| Un `malloc`/`free` équilibré | Voir la fuite dans le code de quelqu'un d'autre |
| Un algorithme O(n log n) | Reconnaître un O(n²) caché dans une boucle |

La colonne de droite est ce qui vous rend capable de juger la colonne de gauche.
Sans elle, vous n'êtes pas un développeur assisté par IA — vous êtes un
intermédiaire entre un modèle et un compilateur.

## Ce qu'il faut retenir

- Le critère de fiabilité n'est pas la difficulté, c'est **la présence de
  l'information** : dans votre contexte, ou massivement dans l'entraînement.
- **Haute fiabilité** : expliquer à votre niveau, décoder une erreur, relire
  votre code, comparer des approches, suggérer des cas limites, vous interroger.
- **Faible fiabilité** : calculer, compter, nommer une API, tout ce qui est
  récent, raisonner sur ce qu'il ne voit pas, tenir une architecture étendue.
- **Le compilateur est un détecteur d'hallucinations.** Une fonction inventée ne
  compile pas.
- Pour tout ce qui se calcule : **faites calculer la machine**, pas le modèle.
- La vérification a quatre niveaux, et le troisième est le seul non
  négociable : **saurais-je réécrire et défendre ce code ?**
- Le danger n'est pas le code faux, c'est **le code juste qu'on ne comprend
  pas.**

---

## À faire maintenant

**1. Provoquer une hallucination et la faire trancher par le compilateur.**
Demandez : « quelle fonction de la bibliothèque standard C inverse une chaîne en
place ? » Il n'y en a pas de standard. Notez la réponse, puis :

```sh
man 3 strrev        # existe ? sur quel système ?
```

Écrivez un programme de trois lignes qui utilise la fonction proposée, et
compilez-le. Que dit `gcc` ? Vous venez d'utiliser le détecteur.

**2. Comparer sur les deux zones.** Sur la même notion — disons les tables de
hachage — posez deux questions :

- `"Explique-moi le principe d'une table de hachage et la gestion des collisions."`
- `"Quelle est la signature exacte de la fonction de hachage de la glibc et dans quel header ?"`

Vérifiez la seconde dans `man` et dans les headers. Concluez sur la frontière
entre les deux zones.

**3. Faire relire un vrai code.** Prenez une de vos implémentations du module
[3.projects_to_learn_algo](../../../3.projects_to_learn_algo/). Demandez une
relecture avec fuites mémoire et cas limites, sans réécriture.

Pour chaque remarque : est-elle juste ? Vérifiez-la vous-même avant de corriger.
Certaines seront fausses. C'est l'exercice.

**4. Le test du code juste incompris.** Demandez-lui d'écrire `strlen` en une
seule ligne, la plus dense possible. Le code marchera.

Expliquez-le **par écrit**, terme par terme, sans aide. Puis faites corriger
votre explication.

Si vous n'y arrivez pas : vous venez de rencontrer, en conditions réelles, le
piège central de cette leçon.

**5. Construire votre propre tableau.** Sur une semaine de travail dans ce dépôt,
notez chaque fois que le modèle s'est trompé, et sur quel type de tâche.

Au bout d'une semaine, vous aurez votre carte personnelle — plus utile que celle
de cette leçon, parce qu'elle décrit vos usages.

Passez à [04_travailler_avec_un_agent.md](04_travailler_avec_un_agent.md).
