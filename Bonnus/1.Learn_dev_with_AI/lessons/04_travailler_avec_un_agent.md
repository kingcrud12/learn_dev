# Travailler avec un agent de code

## Du chat à l'agent

Les trois premières leçons portaient sur une conversation : vous écrivez, le
modèle répond, vous copiez ce qui vous sert. Le modèle ne voyait que ce que vous
lui montriez.

Un **agent de code** change cela. C'est un modèle à qui on a donné des **outils** :
lire vos fichiers, chercher dans le dépôt, écrire du code, exécuter des
commandes, lancer les tests. Il ne répond plus, il **agit**.

C'est un saut de nature. Un chat qui se trompe produit un mauvais paragraphe. Un
agent qui se trompe modifie vos fichiers.

Cette leçon porte sur ce fonctionnement et sur la discipline qu'il exige. Elle
est valable pour Claude Code, et transposable aux autres outils du même type.

## La boucle d'un agent

Le mécanisme reste celui de la leçon 01 — prédire le fragment suivant — mais
inséré dans une boucle :

```
1. Vous donnez une tâche
2. Le modèle décide d'un outil à utiliser        (lire un fichier, grep…)
3. L'outil s'exécute, son résultat entre dans le contexte
4. Le modèle relit tout et décide de la suite
5. Retour en 2, jusqu'à ce qu'il juge la tâche finie
6. Il vous répond
```

Trois conséquences importantes découlent de cette boucle.

**L'agent voit vraiment votre code.** C'est le progrès majeur : il ne raisonne
plus sur une description, mais sur les fichiers réels, la vraie erreur de
compilation, la vraie sortie de test. La zone « ne voit pas ce dont il parle » de
la leçon 03 se referme en grande partie.

**Chaque outil remplit le contexte.** Lire dix fichiers, c'est dix fichiers dans
la fenêtre. Un contexte saturé se dégrade — l'agent oublie les contraintes du
début. D'où une règle contre-intuitive : **des tâches petites et cadrées
donnent de meilleurs résultats qu'une grande tâche vague.**

**L'erreur devient un effet de bord.** Un chat propose du code faux, vous ne le
copiez pas. Un agent écrit du code faux dans vos fichiers, et l'étape suivante
raisonne dessus. Les erreurs se composent au lieu de s'annuler.

## Git est le filet de sécurité

Ce n'est pas un conseil général de bonne pratique. C'est **la** condition qui
rend l'usage d'un agent raisonnable.

Un agent peut modifier plusieurs fichiers en une action. Sans historique, un
mauvais changement est irrécupérable ; avec, il s'annule d'une commande.

```sh
git status                    # avant de lancer l'agent : où j'en suis
git diff                      # après : qu'a-t-il changé, exactement
git checkout -- fichier.c     # annuler un fichier
git stash                     # tout mettre de côté sans perdre
```

**La discipline minimale :**

```
1. Commiter avant de lancer une tâche d'agent — travail propre
2. Laisser l'agent travailler
3. Lire le diff en entier, ligne par ligne
4. Compiler, tester
5. Commiter si c'est bon, annuler sinon
```

L'étape 3 est celle qu'on saute, et c'est la seule qui protège vraiment. Un agent
peut modifier un fichier auquel vous ne pensiez pas. Le diff est la seule source
de vérité sur ce qui s'est passé.

Le module [2.Learn_linux](../../../2.Learn_linux/) couvre Git en détail. Si vous
ne l'avez pas fait, **faites-le avant d'utiliser un agent sur votre code** : c'est
un prérequis, pas un complément.

## Ce qui change pour l'apprentissage

Un agent est plus utile et plus dangereux qu'un chat.

**Plus utile** : il voit votre code réel. Un « pourquoi ce test échoue ? » devient
un diagnostic précis au lieu d'une liste de causes possibles.

**Plus dangereux** : il peut faire l'exercice entier, silencieusement, en trente
secondes. Il n'y a même pas de copier-coller pour vous alerter — le fichier est
juste écrit.

D'où une distinction à tenir fermement :

| Sur votre code d'exercice | Sur le reste |
|---|---|
| **Lecture seule** | Écriture autorisée |
| Il explique, interroge, relit, oriente | Il écrit, refactore, configure |
| Vous tapez chaque ligne vous-même | Vous lisez le diff |
| C'est votre exercice | C'est de l'outillage |

« Le reste » est réel et utile : un `Makefile`, un script de mesure, un
générateur de jeux de données, la mise en forme d'un README, un fichier de
configuration. Rien de tout cela n'est ce que le module enseigne — et le temps
gagné là est du temps rendu aux exercices.

### Le cadrage à poser

Un agent applique les instructions persistantes. Utilisez-les.

```
Dans ce dépôt, j'apprends. Règles permanentes :

- Ne jamais écrire ni modifier de fichier dans les dossiers
  d'exercices ou dans codes/ : ce sont mes exercices.
- Tu peux lire tout le dépôt, compiler, exécuter, lancer gdb ou
  valgrind, et me dire ce que tu observes.
- Quand je suis bloqué : indice gradué, jamais la solution.
- Pour l'outillage (Makefile, scripts de mesure, README), tu peux
  écrire — je lirai le diff.
- Explique toujours le pourquoi avant le comment.
```

Sur Claude Code, ces règles se placent dans un fichier `CLAUDE.md` à la racine du
dépôt : elles sont chargées automatiquement à chaque session. Les autres outils
ont un équivalent. Cherchez-le une fois — cela vaut mieux que de répéter les
règles à chaque conversation.

## Ce qu'un agent fait très bien pour un apprenant

### Diagnostiquer sur votre vraie machine

C'est son emploi le plus rentable, et il est en lecture seule :

```
Mon programme segfault. Compile-le avec -g, lance-le sous gdb,
donne-moi la ligne fautive et l'état des variables à ce moment.
Explique-moi ce que tu observes. Ne corrige rien.
```

L'agent exécute vraiment `gcc` et `gdb`, et vous rapporte des faits. Ce n'est
plus une hypothèse — c'est une observation. Et le « ne corrige rien » vous
garde le diagnostic et la correction.

C'est aussi une excellente façon d'apprendre `gdb` et `valgrind` : vous voyez les
commandes réelles qu'il utilise.

### Explorer un dépôt

```
Où est définie la structure de nœud dans ce dépôt, et quels fichiers
l'utilisent ?
```

Il cherche et répond avec les chemins réels. Sur un projet dont vous héritez,
c'est la façon la plus rapide de vous orienter — et une compétence de métier à
part entière.

### Mesurer

```
Compile mes trois implémentations de tri, génère des tableaux de 1000,
10000 et 100000 éléments, mesure les temps, présente-moi un tableau.
Ne modifie pas mes tris.
```

L'infrastructure de mesure n'est pas ce que le module 3 enseigne — la lecture des
résultats l'est. Déléguer la première pour se concentrer sur la seconde est un
bon échange. C'est exactement l'esprit du projet
[banc de mesure des tris](../../../3.projects_to_learn_algo/projects/03_banc_de_tri/).

### Vous interroger sur votre propre code

```
Lis mon implémentation de table de hachage. Pose-moi cinq questions
sur mes choix de conception, une à la fois. Cherche les points où je
ne saurais pas justifier ce que j'ai écrit.
```

Il ne peut pas inventer : il a le code sous les yeux. C'est la revue de code la
plus honnête que vous puissiez obtenir sur du code que personne d'autre ne lira.

## Les modes de permission

Les agents proposent en général plusieurs niveaux d'autonomie. Les noms varient,
le principe non :

| Mode | Comportement | Pour un apprenant |
|---|---|---|
| Lecture / plan | Lit, analyse, propose — n'écrit rien | **Le défaut sur vos exercices** |
| Confirmation | Demande avant chaque action | Bon compromis sur l'outillage |
| Autonome | Agit sans demander | Seulement sur du jetable, avec Git propre |

**Le réflexe à installer** : le mode lecture est votre défaut, pas votre
exception. Vous ne perdez presque rien — le diagnostic, l'explication, la revue,
l'interrogation fonctionnent tous en lecture seule — et vous gardez la totalité
de l'exercice.

Le mode autonome n'est pas interdit. Il est réservé à ce dont l'écriture ne vous
apprend rien : générer un jeu de données, mettre en forme un fichier, écrire un
script de mesure. Et toujours sur un dépôt commité.

## Le piège du travail plausible

Une erreur particulière aux agents, qu'un chat ne produit pas.

Un agent qui n'arrive pas à faire passer un test peut prendre un chemin de
moindre résistance :

- adapter le test au code au lieu de corriger le code
- traiter le cas qui échoue par une condition spéciale
- entourer l'erreur d'un `try`/`if` qui la masque
- déclarer la tâche finie en ayant traité une partie du problème

Rien de tout cela n'est malveillant : c'est la continuation la plus probable
« d'un texte où un problème est résolu ». Le résultat *ressemble* à du travail
fini.

**La défense est double, et aucune n'est optionnelle :**

1. **Lire le diff, pas le résumé.** Le résumé de l'agent est ce qu'il croit avoir
   fait. Le diff est ce qu'il a fait.
2. **Vérifier vous-même.** Lancer les tests, compiler, exécuter. « Les tests
   passent » est une affirmation à vérifier, comme toute autre sortie de modèle.

Et l'exigence à formuler explicitement :

```
Si tu n'arrives pas à faire passer ce test, dis-le et explique pourquoi.
Ne modifie pas le test, et ne contourne pas le problème.
```

## La compétence réelle

L'usage d'un agent est aujourd'hui une compétence professionnelle à part entière.
Ce n'est pas « savoir prompter » — c'est :

- **découper** un travail en tâches vérifiables
- **cadrer** ce que l'agent peut et ne peut pas toucher
- **lire un diff** vite et sans complaisance
- **vérifier** par la machine plutôt que par la lecture
- **reconnaître** un travail plausible mais faux
- savoir **quand ne pas déléguer**

Remarquez que toutes ces compétences supposent de savoir juger le code. Aucune ne
s'acquiert en déléguant. C'est pourquoi le socle de ce dépôt — C, Linux, algo —
n'est pas rendu obsolète par ces outils : il est ce qui permet de les utiliser
sans en être dépendant.

## Ce qu'il faut retenir

- Un **agent** est un modèle muni d'outils : il lit, écrit, exécute. Il agit au
  lieu de répondre.
- Sa boucle **remplit le contexte** : des tâches petites et cadrées valent mieux
  qu'une grande tâche vague.
- **Git est la condition de sécurité.** Commiter avant, lire le diff après.
- Sur vos **exercices** : lecture seule. Sur l'**outillage** : écriture
  autorisée, diff relu.
- Le **mode lecture est le défaut** — diagnostic, revue, interrogation y
  fonctionnent tous.
- Un agent excelle à **diagnostiquer sur votre vraie machine** (`gdb`,
  `valgrind`, compilation) sans rien écrire.
- Méfiez-vous du **travail plausible** : test modifié, cas spécial, erreur
  masquée. Lire le diff, pas le résumé.
- La compétence réelle est de **découper, cadrer, lire un diff, vérifier** — et
  elle suppose de savoir juger du code.

---

## À faire maintenant

**1. Prendre le réflexe Git.** Avant toute tâche d'agent, dans ce dépôt :

```sh
git status
git add -A && git commit -m "avant tâche agent"
```

Après, avant de lire quoi que ce soit d'autre :

```sh
git diff HEAD
```

Faites-le cinq fois de suite. C'est un réflexe à installer, pas une idée à
retenir.

**2. Diagnostiquer en lecture seule.** Prenez un programme C avec un bug de
mémoire — ou introduisez-en un exprès, un `free` en trop ou un accès hors bornes.
Demandez un diagnostic complet sous `gdb` et `valgrind`, **sans correction**.

Notez les commandes qu'il a utilisées. Relancez-les vous-même, à la main. Vous
apprenez `valgrind` en le regardant faire.

**3. Écrire vos règles permanentes.** Créez un `CLAUDE.md` à la racine du dépôt
(ou l'équivalent pour votre outil) avec vos règles d'apprentissage. Partez du
cadrage de cette leçon, adaptez-le.

Testez-le : demandez à l'agent de résoudre un exercice. Il devrait refuser et
proposer un indice. Si ce n'est pas le cas, vos règles sont trop faibles —
resserrez-les.

**4. Déléguer ce qui n'est pas l'exercice.** Demandez à l'agent d'écrire un
`Makefile` pour votre dossier `codes/` du module C, avec `-Wall -Wextra -g` et
une cible `clean`.

Lisez-le en entier. Faites-vous expliquer chaque ligne que vous ne comprenez pas.
Vous avez gagné du temps **et** appris `make` — c'est la bonne délégation.

**5. Détecter le travail plausible.** Écrivez une fonction avec un bug réel, et
un test qui échoue. Demandez à l'agent de faire passer le test, en autonomie, sur
un dépôt commité.

Lisez le diff **avant** son résumé. A-t-il corrigé la fonction, ou arrangé le
test ? Refaites l'expérience en interdisant explicitement de toucher au test.
Comparez.

**6. Se faire interroger.** Choisissez votre code le plus complexe dans ce dépôt.
Demandez cinq questions sur vos choix de conception, une à la fois.

Les questions auxquelles vous ne savez pas répondre désignent exactement ce qu'il
reste à comprendre dans du code que vous avez écrit vous-même.

Passez à [05_garder_la_main.md](05_garder_la_main.md).
