# Prompter pour apprendre, pas pour livrer

## Deux objectifs incompatibles

Un développeur en poste et un apprenant demandent la même chose à un modèle, et
devraient la demander différemment.

Le premier veut **du code qui marche, vite**. Le second veut **savoir écrire ce
code sans aide**. Ce sont deux objectifs distincts, et le prompt qui sert l'un
dessert l'autre.

```
"Écris-moi une fonction C qui inverse une liste chaînée."
```

Cette demande est parfaite en production. Elle est **contre-productive** pendant
l'apprentissage : elle produit exactement le code que vous étiez censé apprendre
à écrire. Vous obtenez la réponse et vous perdez l'exercice. Pire, vous obtenez
la *sensation* d'avoir compris — parce que du code correct est facile à lire —
sans en avoir la capacité.

Cette leçon porte sur la seconde catégorie de prompts. Elle suppose la
leçon [01](01_ce_quest_un_LLM.md) : un bon prompt ne rend pas le modèle plus
intelligent, il rend la bonne réponse plus probable. Ici, « la bonne réponse »
est celle qui vous fait progresser, pas celle qui vous débloque.

## L'illusion de compétence

C'est le risque central, et il faut le nommer précisément.

Lire du code correct est facile. Chaque ligne paraît évidente, la logique coule,
on hoche la tête. Cette fluidité est réelle — et elle ne mesure rien. Elle
mesure votre capacité à **reconnaître** une solution, pas à la **produire**.

Ce sont deux compétences distinctes, et l'écart entre elles est énorme :

| Reconnaître | Produire |
|---|---|
| « Oui, il faut un pointeur `prev` » | Trouver seul qu'il faut un pointeur `prev` |
| « Cette condition d'arrêt est juste » | Écrire la condition d'arrêt du premier coup |
| Suivre un raisonnement | Choisir le raisonnement parmi cinq possibles |
| Comprendre une leçon | Résoudre l'exercice sans la relire |

L'IA accélère massivement l'acquisition de la première colonne. Elle peut
**empêcher** l'acquisition de la seconde, en supprimant l'effort qui la produit.

Le test est simple, et il est le seul qui compte : **fermez tout, et réécrivez.**
Si vous ne pouvez pas réécrire de mémoire ce que vous venez de lire, vous ne
l'avez pas appris. C'est la règle n°1 de ce dépôt, et l'IA la rend beaucoup plus
facile à enfreindre.

## Les six prompts qui font apprendre

Voici les formes qui marchent. Ce ne sont pas des formules magiques : ce sont des
énoncés qui rendent une réponse pédagogique plus probable qu'une réponse-solution.

### 1. Le prompt d'explication

À utiliser quand une leçon ne passe pas.

```
Explique-moi les pointeurs en C. Je viens de finir le module sur les
tableaux. Je comprends qu'une variable a une adresse, mais je ne vois
pas pourquoi on manipulerait l'adresse plutôt que la valeur.
Utilise une analogie, puis un exemple de code minimal.
Ne me donne pas de code que je pourrais avoir à écrire en exercice.
```

Ce qui le rend efficace :

- **il dit où vous en êtes** — le modèle calibre son niveau
- **il dit ce qui bloque précisément** — pas « explique les pointeurs », mais
  « je ne vois pas pourquoi »
- **il demande une forme** — analogie puis code minimal
- **il pose une limite** — pas de code d'exercice

### 2. Le prompt socratique

Le plus utile de tous, et le plus sous-utilisé.

```
Je veux comprendre pourquoi mon tri fusion est plus lent que prévu.
Ne me donne pas la réponse. Pose-moi des questions, une à la fois,
qui me guident vers la cause. Attends ma réponse avant de continuer.
```

Vous ne recevez pas une solution, vous recevez un interrogatoire. Le travail de
raisonnement reste chez vous — et c'est ce travail qui installe la compétence.

Le « **une à la fois** » est essentiel. Sans lui, le modèle pose ses questions
*et* y répond dans le même message.

### 3. Le prompt de vérification

À utiliser après avoir résolu, jamais avant.

```
Voici mon implémentation de liste chaînée : [code]
Ne la réécris pas. Dis-moi :
- ce qui est correct
- ce qui va fuir en mémoire ou planter, et sur quelle entrée
- ce qu'un développeur expérimenté écrirait autrement, et pourquoi
Classe tes remarques de la plus grave à la plus cosmétique.
```

C'est l'usage le plus rentable de l'IA en apprentissage : vous avez fait
l'effort, vous recevez le retour. Le « **ne la réécris pas** » compte — sans
lui, vous recevez une version corrigée que vous ne saurez pas reproduire.

### 4. Le prompt d'indice gradué

Quand vous êtes bloqué mais pas prêt à abandonner.

```
Je suis bloqué sur cet exercice : [énoncé]
Voici où j'en suis : [votre tentative]
Donne-moi un indice de niveau 1 seulement : oriente-moi vers la bonne
piste sans nommer la solution ni écrire de code.
Si je redemande, tu passeras au niveau 2.
```

Ce prompt maintient l'effort à la limite de vos capacités — c'est là que
l'apprentissage se produit. Un indice bien dosé fait progresser ; une solution
complète clôt le sujet.

### 5. Le prompt d'interrogation

Pour tester ce que vous croyez savoir.

```
Interroge-moi sur les pointeurs en C, niveau débutant-intermédiaire.
Une question à la fois. Après chaque réponse, dis-moi si j'ai juste et
ce qui manque, puis enchaîne. Va vers ce sur quoi je me trompe.
Dix questions.
```

Le « **va vers ce sur quoi je me trompe** » transforme un questionnaire en
diagnostic. Vous découvrez vos trous plutôt que de réviser ce que vous maîtrisez
déjà.

### 6. Le prompt d'inversion

Le plus puissant, et le plus inconfortable.

```
Je vais t'expliquer comment fonctionne malloc. Écoute, puis dis-moi
ce qui est faux, imprécis, ou ce que j'ai omis d'important.
Sois exigeant. Voici mon explication : [...]
```

Vous expliquez, le modèle corrige. C'est la technique Feynman avec un
correcteur : impossible de tricher, parce qu'il faut produire l'explication
avant de recevoir le retour.

## Les cinq prompts qui vous freinent

| Prompt | Pourquoi c'est un piège |
|---|---|
| `"Écris-moi la fonction qui..."` | Fait l'exercice à votre place |
| `"Corrige mon code"` | Vous recevez du code juste sans savoir ce qui était faux |
| `"Fais mon exercice"` | Zéro apprentissage, et vous ne le saurez qu'à l'examen suivant |
| `"C'est bon ?"` (sans avoir cherché) | Délègue le jugement, qui est la compétence visée |
| `"Explique tout sur X"` | Réponse générique, non calibrée, oubliée en une heure |

Le motif commun : **ils suppriment l'effort.** Or c'est l'effort qui produit
l'apprentissage — pas l'information reçue.

## Anatomie d'un bon prompt d'apprentissage

Cinq éléments. Aucun n'est obligatoire, mais chacun améliore la réponse.

```
[1 CONTEXTE]   Je fais le module algo de learn_dev, leçon 6 sur les
               listes chaînées. J'ai fini le C jusqu'aux pointeurs.

[2 DEMANDE]    Je ne comprends pas pourquoi on garde un pointeur vers
               le nœud précédent dans la suppression.

[3 TENTATIVE]  Ma logique : je parcours jusqu'au nœud à supprimer et
               je fais free(). Mais la liste se casse après.

[4 FORME]      Explique le mécanisme avec un schéma en texte des
               pointeurs avant/après. Cinq lignes max.

[5 LIMITE]     Ne me donne pas le code de la fonction, je dois
               l'écrire moi-même.
```

Le point 3 est celui qu'on omet le plus, et c'est le plus précieux : montrer sa
tentative permet au modèle de corriger **votre** raisonnement plutôt que
d'exposer le sien. C'est la différence entre un cours et un tutorat.

Le point 5 est celui qui distingue l'apprenant du professionnel. Sans lui, le
modèle vous rend service — et vous coûte l'exercice.

## Le protocole des 20 minutes

Une règle de discipline, à appliquer avant tout prompt de déblocage.

```
1. Vingt minutes seul, minimum.
   Relire la leçon. Écrire au brouillon. Tracer avec printf.
   Faire tourner le débogueur.

2. Formuler le problème par écrit, en trois phrases :
   - ce que je veux obtenir
   - ce que j'obtiens
   - ce que j'ai déjà essayé

3. Souvent, l'étape 2 résout le problème.
   Formuler clairement est déjà la moitié du diagnostic.

4. Sinon : prompt d'indice de niveau 1. Pas de solution.

5. Toujours bloqué : niveau 2, puis niveau 3.

6. Solution obtenue : fermer, attendre, réécrire de mémoire.
   Si ça ne compile pas, ce n'est pas acquis.
```

L'étape 6 n'est pas négociable. C'est elle qui transforme une réponse reçue en
compétence acquise, et c'est exactement la règle n°1 du dépôt.

L'étape 3 mérite d'être remarquée : le fait d'écrire un bon prompt vous fait
souvent trouver la réponse. Ce n'est pas un hasard — c'est l'effet
« [rubber duck debugging](https://fr.wikipedia.org/wiki/M%C3%A9thode_du_canard_en_plastique) »,
et le modèle en est un très bon canard.

## Les instructions de cadrage

Une phrase posée en début de session vaut mieux que dix corrections. Posez vos
règles une fois, et rappelez-les quand la conversation s'allonge.

```
Contexte pour toute cette session : j'apprends le C, module 1 de
learn_dev. Règles :
- ne jamais écrire de code complet que je pourrais avoir à écrire
- toujours expliquer le pourquoi avant le comment
- si je demande une solution directe, propose-moi d'abord un indice
- me dire quand tu n'es pas sûr, plutôt que de deviner
```

Sur les outils qui le permettent (fichiers d'instructions persistants,
préférences de projet), ces règles peuvent être posées une fois pour toutes.
Sinon, gardez-les dans un fichier et collez-les en début de session.

## Ce qu'il faut retenir

- **Le prompt qui livre du code et le prompt qui fait apprendre sont
  différents.** Le premier vous rend service ; le second vous rend compétent.
- L'**illusion de compétence** est le risque central : lire du code correct
  donne un sentiment de maîtrise qui ne mesure rien.
- Le seul test valable : **fermer et réécrire de mémoire.**
- Les six formes qui marchent : **explication calibrée, socratique,
  vérification, indice gradué, interrogation, inversion.**
- Montrer sa **tentative** transforme un cours en tutorat. C'est l'élément le
  plus souvent omis et le plus utile.
- Poser une **limite** explicite (« pas de code complet ») est ce qui protège
  l'exercice.
- Le **protocole des 20 minutes** : chercher, formuler, indice, jamais la
  solution en premier — et toujours réécrire à la fin.

---

## À faire maintenant

Ces exercices se font avec un modèle, sur du code que vous avez déjà écrit dans
ce dépôt.

**1. Mesurer l'écart reconnaître / produire.** Prenez un exercice du module
[1.Learn_C](../../../1.Learn_C/) que vous avez terminé il y a plus d'une
semaine. Sans regarder votre solution, réécrivez-la. Compilez.

Puis comparez. L'écart entre « je me souviens l'avoir compris » et « ça compile
du premier coup » est la mesure exacte de ce qui reste à faire.

**2. Comparer deux prompts.** Prenez une notion qui vous résiste. Dans deux
conversations **séparées** :

- conversation A : `"Explique-moi [notion]."`
- conversation B : le prompt d'explication complet de cette leçon, avec
  contexte, blocage précis, forme demandée et limite.

Comparez. Notez laquelle des cinq parties du prompt a le plus changé la réponse.

**3. Tenir une session socratique.** Choisissez un bug réel dans votre code.
Utilisez le prompt socratique, et **tenez-le** : ne demandez pas la réponse, même
si c'est frustrant. Répondez à chaque question avant la suivante.

Notez à quelle question vous avez compris le problème. C'était souvent avant que
le modèle ne l'ait dit.

**4. Faire l'inversion.** Expliquez au modèle, de mémoire et sans rien relire,
ce qu'est un pointeur et à quoi sert `malloc`. Demandez une correction exigeante.

Ce que vous avez omis est ce que vous croyiez savoir.

**5. Écrire votre fichier de cadrage.** Créez `mes_regles_IA.md` dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/) : vos instructions de
session, adaptées à votre niveau et à ce que vous savez de vos propres
raccourcis. Vous le collerez en début de session, et le ferez évoluer.

Passez à [03_ce_que_lIA_fait_bien.md](03_ce_que_lIA_fait_bien.md).
