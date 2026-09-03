# Ce qu'est un modèle de langage (et ce qu'il n'est pas)

## Pourquoi commencer par là

On peut utiliser une voiture sans connaître le cycle à quatre temps. On ne peut
pas utiliser un modèle de langage sans savoir ce qu'il fait, parce que ses
erreurs ne ressemblent pas à des erreurs.

Un compilateur qui ne comprend pas votre code affiche une erreur. Un modèle de
langage qui ne connaît pas la réponse **en produit une quand même**, avec la
même assurance que pour une réponse juste. C'est la seule machine de votre
environnement de travail dont la sortie ne signale pas sa propre fiabilité.

Cette leçon explique le mécanisme. Pas les mathématiques — le mécanisme, au
niveau où il change votre façon de l'utiliser.

## Une machine à prédire le mot suivant

Un modèle de langage fait une seule chose : étant donné un texte, prédire ce qui
vient après.

```
"La capitale de la France est"  →  " Paris"
```

Puis il recommence, en incluant sa propre sortie dans l'entrée :

```
"La capitale de la France est Paris"  →  "."
```

Et ainsi de suite, un fragment à la fois, jusqu'à décider de s'arrêter. Une
réponse de trois cents mots est le résultat de trois cents prédictions
successives, chacune ne voyant que le texte qui la précède.

Il n'y a **pas** d'étape de planification. Le modèle ne rédige pas un plan pour
ensuite le remplir. Quand il produit une réponse bien structurée, c'est que du
texte bien structuré était la continuation la plus probable — pas qu'une
intention l'a organisée.

### Le token

Le modèle ne prédit pas des mots mais des **tokens** : des fragments de texte,
souvent une syllabe ou un morceau de mot.

| Texte | Tokens approximatifs |
|---|---|
| `bonjour` | `bon` + `jour` |
| `malloc` | `m` + `alloc` |
| `anticonstitutionnellement` | 7 à 9 tokens |
| `42` | `42` |

Cela paraît un détail d'implémentation. Ce n'en est pas un, et vous croiserez
trois conséquences concrètes :

- **Le modèle ne « voit » pas les lettres.** Demandez-lui de compter les `r`
  dans un mot, ou d'inverser une chaîne caractère par caractère : il échoue
  souvent, non par bêtise, mais parce que le mot lui arrive comme deux ou trois
  blocs opaques.
- **L'arithmétique est fragile.** `1247 × 38` n'est pas calculé, il est
  *prédit*. Le résultat aura la bonne forme et le bon ordre de grandeur, et
  pourra être faux.
- **Tout est facturé et compté en tokens.** La fenêtre de contexte, les limites,
  le prix : tout se mesure dans cette unité.

Retenez la règle d'or qui en découle : **pour tout ce qui se calcule, faites
calculer la machine, pas le modèle.** Un modèle qui écrit un script Python de
trois lignes pour faire l'opération est fiable ; le même modèle qui fait
l'opération de tête ne l'est pas.

## D'où vient ce qu'il sait

L'entraînement se fait en deux temps.

**Le pré-entraînement.** Le modèle lit une quantité massive de texte — des
livres, du code, des forums, des documentations — et s'entraîne uniquement à
prédire la suite. Il n'y a aucune supervision, aucun professeur : juste des
milliards de fois « voici un texte tronqué, devine la suite, voici la vraie
suite, corrige-toi ».

C'est là qu'il acquiert la syntaxe du C, les idiomes de Python, la structure
d'un message de commit, le vocabulaire des pointeurs. Non parce qu'on lui a
enseigné ces règles, mais parce que les respecter améliore la prédiction.

**L'ajustement.** Un second temps, beaucoup plus court, lui apprend à se
comporter en assistant : répondre à une question plutôt que de la continuer,
refuser certaines demandes, adopter un ton. C'est ce qui transforme une machine
à compléter du texte en un interlocuteur.

### Ce que cela implique sur ses connaissances

Le savoir du modèle est **figé** au moment de son entraînement, et il est
**statistique**.

Figé : il existe une date de coupure. Une bibliothèque sortie après, une API
modifiée après, une faille découverte après — le modèle ne les connaît pas.
Pire, il peut décrire avec assurance la version de l'API qu'il a connue.

Statistique : ce qui est fréquent dans le texte est bien maîtrisé, ce qui est
rare l'est mal. Cela crée un profil de compétence très inégal :

| Le modèle est solide sur | Le modèle est fragile sur |
|---|---|
| Le C, Python, JavaScript standard | Un langage de niche, un framework récent |
| Les algorithmes classiques | Votre logique métier |
| Les erreurs de compilation courantes | Un bug propre à votre architecture |
| Les idiomes largement diffusés | La version exacte de votre dépendance |
| Expliquer un concept enseigné partout | Un détail précis d'une documentation obscure |

Et la fragilité **ne s'annonce pas**. La confiance du ton est constante.

## L'hallucination

Quand un modèle produit une affirmation fausse mais plausible, on parle
d'**hallucination**. Le terme est trompeur : il suggère un dysfonctionnement.
Ce n'en est pas un — c'est le fonctionnement normal appliqué à une zone où le
modèle n'a pas d'information.

Le mécanisme est simple. Le modèle prédit la continuation la plus probable. Si
vous demandez « quelle fonction de la bibliothèque X fait Y ? », la continuation
la plus probable est **le nom d'une fonction** — parce que c'est ce qui suit une
telle question dans tous les textes qu'il a lus. Que cette fonction existe est
une autre question, et ce n'est pas celle que le mécanisme résout.

D'où la forme typique des hallucinations en programmation :

- une fonction au nom parfaitement crédible, qui n'existe pas
- un paramètre plausible ajouté à une fonction réelle
- un paquet inventé, avec une commande d'installation qui échoue
- une citation de documentation qui n'a jamais été écrite
- un lien vers une page qui n'existe pas

Notez ce qu'elles ont en commun : **elles sont toutes vérifiables en dix
secondes.** Un nom de fonction se cherche dans la documentation ou avec `man`.
Un paquet s'installe ou non. Du code compile ou non. La compétence à développer
n'est pas de détecter l'hallucination à l'œil — c'est d'avoir le réflexe de
vérifier avant d'y croire.

### Pourquoi il ne dit pas « je ne sais pas »

Parce que dans le texte dont il a appris, une question suivie de « je ne sais
pas » est rare. Les documentations, les forums, les tutoriels contiennent
essentiellement des questions suivies de réponses. Répondre est statistiquement
correct ; s'abstenir ne l'est pas.

L'ajustement corrige partiellement ce biais — les modèles récents admettent
davantage leurs limites. Mais le penchant de fond reste : **le modèle préfère
répondre.**

## Le contexte : sa seule mémoire

Le modèle n'a **aucune mémoire** entre deux requêtes. À chaque fois, on lui
renvoie l'intégralité de la conversation, et il la relit en entier.

Ce que vous appelez « la conversation » est un texte qui grossit, réexpédié
complet à chaque tour. Cette réserve s'appelle la **fenêtre de contexte**, et
elle est finie.

Trois conséquences pratiques :

**Ce qui n'est pas dans le contexte n'existe pas.** Le modèle ne voit pas vos
fichiers, ne connaît pas la version de votre compilateur, n'a pas vu le message
d'erreur que vous décrivez de mémoire. Il ne peut raisonner que sur ce que vous
lui montrez. La plus grande part des mauvaises réponses vient de là — pas d'un
défaut du modèle, mais d'un contexte insuffisant.

**Une conversation trop longue se dégrade.** Quand la fenêtre se remplit, le
début est résumé ou oublié. Les contraintes posées au début se perdent. Sur une
longue session, il faut rappeler les règles importantes.

**Le contexte se pollue.** Vingt échanges à corriger une mauvaise piste laissent
vingt échanges de mauvaise piste dans le contexte, et le modèle continue de
raisonner dessus. Quand une conversation part de travers, **recommencer avec un
énoncé propre est souvent plus rapide que corriger.**

## Le même modèle, deux comportements

Deux formulations d'une même demande donnent des résultats de qualité très
différente. Ce n'est pas un caprice, cela découle du mécanisme.

```
"Pourquoi mon code segfault ?"
```

Le modèle n'a pas le code. Il n'a pas l'erreur. Il ne connaît pas le contexte.
La continuation la plus probable est **une liste de causes courantes de
segfault** — un cours général, pas un diagnostic. La réponse sera juste et
inutile.

```
"Ce code segfault à la ligne 12. Voici le code : [...]
 Voici la sortie de lldb : [...]
 Le tableau est alloué ligne 8 avec malloc(n).
 Explique-moi la cause avant de proposer un correctif."
```

Le modèle a le code, l'erreur, l'emplacement. La continuation la plus probable
est maintenant **un diagnostic précis** — parce que c'est ce qui suit ce type
d'énoncé dans les textes dont il a appris.

Vous n'avez pas rendu le modèle plus intelligent. Vous avez rendu la bonne
réponse plus probable que la mauvaise. **C'est tout ce que fait un bon prompt**,
et c'est l'objet de la leçon suivante.

## Ce que cela dit de votre usage

Le mécanisme entraîne directement une répartition des tâches :

| Type de tâche | Fiabilité | Pourquoi |
|---|---|---|
| Expliquer un concept classique | **Haute** | Massivement présent dans l'entraînement, et vous pouvez recouper |
| Reformuler, traduire, résumer | **Haute** | L'information est dans le contexte, il la transforme |
| Générer du code standard | **Moyenne** | Souvent juste, à compiler et tester systématiquement |
| Relire du code, suggérer des pistes | **Haute** | Le code est dans le contexte ; vous jugez les suggestions |
| Expliquer une erreur de compilation | **Haute** | Motif très fréquent, et le message est dans le contexte |
| Nommer une fonction d'une bibliothèque | **Faible** | Zone d'hallucination classique — vérifier |
| Calculer, compter des caractères | **Faible** | Ce n'est pas calculé, c'est prédit |
| Donner une information récente | **Faible** | Hors de la date de coupure |
| Raisonner sur votre architecture | **Faible** | Il n'en voit que ce que vous montrez |

La ligne de partage n'est pas la difficulté de la tâche. C'est **la présence de
l'information nécessaire** : dans le contexte que vous fournissez, ou massivement
dans son entraînement. Un modèle explique brillamment un arbre rouge-noir et se
trompe sur le nom d'un paramètre. Ce n'est pas incohérent — c'est exactement ce
que le mécanisme prédit.

## Ce qu'il faut retenir

- Un modèle de langage **prédit le fragment de texte suivant**, un fragment à la
  fois. Il n'y a pas de plan, pas de vérification, pas de calcul.
- Il manipule des **tokens**, pas des lettres ni des nombres. D'où sa fragilité
  sur l'arithmétique et le comptage de caractères : faites calculer la machine.
- Son savoir est **figé** à une date et **statistique** : excellent sur le
  fréquent, fragile sur le rare — sans que le ton change.
- L'**hallucination** n'est pas une panne, c'est le mécanisme normal appliqué à
  une zone sans information. Elle est presque toujours vérifiable en dix
  secondes.
- Le **contexte est sa seule mémoire**. Ce qui n'y est pas n'existe pas ; une
  conversation longue se dégrade ; un contexte pollué se recommence.
- Un bon prompt ne rend pas le modèle plus intelligent : il rend **la bonne
  réponse plus probable**.

---

## À faire maintenant

Quatre observations, dans un terminal et dans une conversation avec un modèle.
Le but n'est pas de le piéger, c'est de **voir le mécanisme**.

**1. La limite des tokens.** Demandez-lui combien de fois la lettre `r` apparaît
dans `raréfaction`, puis comptez à la main. Demandez-lui ensuite d'écrire une
commande shell qui compte, et exécutez-la :

```sh
echo -n "raréfaction" | grep -o "r" | wc -l
```

Comparez les deux méthodes. Laquelle feriez-vous confiance sur un mot de
cinquante lettres ?

**2. L'arithmétique.** Demandez-lui le résultat de `4871 × 3926`, sans script.
Vérifiez :

```sh
echo "4871 * 3926" | bc
```

Puis redemandez-lui, cette fois en lui demandant d'écrire un calcul à exécuter.
Notez la différence de fiabilité.

**3. La date de coupure.** Demandez-lui sa date de coupure de connaissances.
Demandez-lui ensuite quelle est la dernière version stable de `gcc`. Vérifiez
la version réelle :

```sh
gcc --version
```

Que vaut sa réponse ? Que devriez-vous conclure sur les questions de version en
général ?

**4. Le contexte qui change tout.** Prenez un de vos programmes C qui a un bug.
Posez d'abord la question sans montrer le code : « pourquoi mon programme
plante-t-il ? ». Notez la réponse. Ouvrez une **nouvelle** conversation, et
reposez la question en collant le code, la commande de compilation et la sortie
d'erreur complète.

Comparez les deux réponses. Vous venez de mesurer l'effet du contexte — c'est le
seul levier réel que vous ayez, et l'objet de la leçon 2.

Passez à [02_prompter_pour_apprendre.md](02_prompter_pour_apprendre.md).
