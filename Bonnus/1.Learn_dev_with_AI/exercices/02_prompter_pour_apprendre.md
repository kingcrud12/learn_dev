# Exercices — Prompter pour apprendre

Support : [02_prompter_pour_apprendre.md](../lessons/02_prompter_pour_apprendre.md)

Ces exercices se font **sur votre propre code**, dans les modules de ce dépôt.
C'est volontaire : un exercice de prompt sur un sujet fictif n'apprend rien.

**Un avertissement.** Plusieurs exercices demandent de tenir une contrainte
inconfortable — ne pas demander la solution alors qu'elle est à une phrase de
distance. C'est précisément l'exercice. La difficulté ressentie est la mesure de
son utilité.

**Prérequis** : avoir terminé au moins les trois premières leçons de
[1.Learn_C](../../../1.Learn_C/), pour avoir du code à soi.

---

# Niveau 1 — Mesurer l'illusion de compétence

Ces exercices ne demandent pas de modèle. Ils établissent votre point de départ.

## 1.1 — Le test du dépôt fermé

Prenez un exercice terminé il y a **plus d'une semaine** dans
[1.Learn_C](../../../1.Learn_C/) ou
[3.projects_to_learn_algo](../../../3.projects_to_learn_algo/).

Sans rien ouvrir — ni la leçon, ni votre solution, ni un modèle — réécrivez-le
dans un fichier vide. Puis :

```sh
gcc -Wall -Wextra -g votre_fichier.c -o prog && ./prog
```

Remplissez :

| Exercice | Compile du 1er coup ? | Corrections nécessaires | Verdict |
|---|---|---|---|
| | | | |

Trois verdicts possibles : **acquis** / **en cours** / **non acquis**.

## 1.2 — Reconnaître contre produire

Prenez le même exercice. Ouvrez votre solution et lisez-la.

Elle vous paraît évidente. Notez cette sensation.

Comparez maintenant cette sensation au résultat de 1.1. Si l'exercice était « non
acquis » alors que la lecture était fluide, vous venez de mesurer l'écart entre
les deux colonnes de la leçon.

Écrivez en trois phrases ce que cet écart vous apprend sur votre propre
évaluation de ce que vous savez.

## 1.3 — L'audit de votre dépôt

Ouvrez cinq fichiers `.c` que vous avez écrits. Pour chacun, expliquez **à voix
haute**, ligne par ligne, ce qu'il fait et pourquoi vous l'avez écrit ainsi.

Marquez dans un fichier les zones muettes — celles où vous butez.

Combien de zones muettes sur cinq fichiers ? C'est votre dette technique
personnelle. Réécrivez-en une de mémoire.

## 1.4 — Le test de la page blanche

Éditeur vide, aucune aide, quinze minutes. Écrivez de zéro un programme qui lit
une ligne au clavier et affiche le nombre de mots.

Y arrivez-vous ? Combien de temps avant la première ligne écrite ?

Ce délai avant la première ligne est le signal d'alerte le plus précoce de la
leçon 05. Notez-le, vous le remesurerez.

---

# Niveau 2 — Les six formes qui font apprendre

Un exercice par forme. Faites-les sur des sujets **réels** de votre progression.

## 2.1 — Le prompt d'explication

Choisissez une notion qui vous résiste vraiment.

Écrivez **deux** prompts, à envoyer dans deux conversations **séparées** :

- **A** : `"Explique-moi [notion]."`
- **B** : le prompt complet en cinq parties — contexte, blocage précis, tentative,
  forme demandée, limite.

Comparez les réponses. Puis répondez :

- laquelle des cinq parties a le plus changé la réponse ?
- laquelle auriez-vous naturellement omise ?

## 2.2 — Le prompt socratique

Prenez un bug **réel** dans votre code. Utilisez le prompt socratique de la
leçon, avec le « une question à la fois ».

**Tenez la contrainte** : ne demandez pas la réponse, même si c'est frustrant.
Répondez à chaque question avant la suivante.

À la fin, notez :

- à quelle question avez-vous compris le problème ?
- l'aviez-vous compris avant que le modèle ne le dise ?

Retirez le « une question à la fois » et recommencez sur un autre bug. Que se
passe-t-il ?

## 2.3 — Le prompt de vérification

Prenez une implémentation **finie** — liste chaînée, table de hachage, tri.
Demandez une relecture : correct / va planter / à écrire autrement, classée par
gravité, **sans réécriture**.

Pour chaque remarque, remplissez :

| Remarque | Juste ? | Comment je l'ai vérifiée | Corrigée ? |
|---|---|---|---|
| | | | |

La deuxième colonne est le vrai exercice. Certaines remarques seront fausses.

## 2.4 — Le prompt d'indice gradué

Choisissez un exercice **non commencé** de
[3.projects_to_learn_algo](../../../3.projects_to_learn_algo/).

Appliquez le protocole : vingt minutes seul, puis indice de niveau 1 seulement.

Notez après chaque niveau : avez-vous progressé ? Jusqu'à quel niveau avez-vous
eu besoin d'aller ?

Un indice de niveau 1 qui suffit est le signe d'un bon dosage. Si vous êtes allé
au niveau 3, l'exercice était-il trop dur, ou vos vingt minutes trop courtes ?

## 2.5 — Le prompt d'interrogation

Demandez dix questions sur les pointeurs, une à la fois, avec « va vers ce sur
quoi je me trompe ».

Notez votre score, et **la liste de ce que vous avez raté**.

Refaites l'interrogation une semaine plus tard, sans réviser entretemps. Le score
a-t-il bougé ? Sur quoi ?

## 2.6 — Le prompt d'inversion

Expliquez au modèle, **de mémoire et sans rien relire**, ce que fait `malloc`,
pourquoi `free` est nécessaire, et ce qui se passe si on oublie.

Demandez une correction exigeante : faux, imprécis, omis.

Ce qu'il pointe comme **omis** est le plus intéressant : c'est ce que vous
croyiez savoir. Écrivez-le quelque part.

## 2.7 — Le tableau comparatif

Vous avez maintenant utilisé les six formes. Classez-les :

| Forme | Effort demandé | Ce que j'ai appris | Utile pour |
|---|---|---|---|
| Explication | | | |
| Socratique | | | |
| Vérification | | | |
| Indice gradué | | | |
| Interrogation | | | |
| Inversion | | | |

Y a-t-il une corrélation entre les colonnes « effort demandé » et « ce que j'ai
appris » ? Que cela confirme-t-il ?

---

# Niveau 3 — Les prompts qui freinent

Ici, on fait délibérément l'erreur, pour en mesurer l'effet.

## 3.1 — Se faire livrer la solution

Prenez un exercice non fait. Demandez `"Écris-moi la solution de : [énoncé]"`.

Lisez le code. Il vous paraîtra clair. Notez cette impression.

**Fermez tout. Attendez une heure.** Réécrivez de mémoire.

Y arrivez-vous ? Comparez avec l'impression de clarté d'il y a une heure.

## 3.2 — La correction sans diagnostic

Introduisez un bug dans un programme qui marche. Demandez `"corrige mon code"`,
sans plus.

Vous obtenez du code juste. Question : **quel était le bug ?** Le savez-vous ?

Refaites l'expérience avec : `"ne corrige pas — dis-moi ce qui est faux et
pourquoi, je corrigerai."`

Comparez ce que vous avez appris dans chaque cas.

## 3.3 — « C'est bon ? »

Écrivez une fonction dont vous n'êtes pas sûr. Demandez `"c'est bon ?"` sans
avoir cherché vous-même.

Puis, sur une autre fonction : cherchez d'abord vous-même les défauts, écrivez
votre liste, **et ensuite** demandez une relecture.

Comparez vos deux listes. Combien de défauts avez-vous trouvés seul ? C'est cette
compétence-là qu'on cherche à construire — et le premier prompt la contourne.

## 3.4 — L'explication générique

Demandez `"explique-moi tout sur les tables de hachage"`. Lisez la réponse.

Une heure plus tard, sans relire : qu'en reste-t-il ?

Puis posez trois questions **précises** issues de votre blocage réel. Comparez ce
qu'il en reste une heure après.

## 3.5 — Le bilan

Sur les quatre pièges du niveau 3, remplissez :

| Piège | Ce que j'ai obtenu | Ce que j'ai appris | Ce que j'ai perdu |
|---|---|---|---|
| | | | |

La dernière colonne est l'objet de cette leçon.

---

# Niveau 4 — Raisonner

Réponses par écrit, quelques phrases. Pas de réponse unique.

## 4.1 — Pourquoi la lecture ne suffit pas

Lire du code correct donne un sentiment de compréhension. Pourquoi ce sentiment
est-il trompeur ?

Quelle est la différence de nature entre reconnaître une solution et la produire ?

## 4.2 — Deux objectifs, deux prompts

Un développeur en poste et un apprenant demandent la même chose. Pourquoi
devraient-ils la demander différemment ?

Y a-t-il un moment de votre progression où vous devrez basculer d'un mode à
l'autre ? Comment saurez-vous que ce moment est arrivé ?

## 4.3 — Le rôle de la tentative

Montrer sa tentative est l'élément le plus souvent omis d'un prompt.

Pourquoi est-ce le plus précieux ? Reliez votre réponse au mécanisme de la
leçon 01 — pourquoi la tentative change-t-elle la prédiction ?

## 4.4 — Le canard en plastique

L'étape 2 du protocole des 20 minutes (formuler le problème en trois phrases)
résout souvent le problème à elle seule.

Pourquoi ? Qu'est-ce que l'acte de formuler accomplit ?

En quoi le modèle est-il un bon canard, et en quoi est-il un mauvais canard ?

## 4.5 — La limite explicite

« Ne me donne pas de code que je pourrais avoir à écrire en exercice. »

Pourquoi cette phrase est-elle nécessaire ? Le modèle ne devrait-il pas le
deviner ?

Reliez au mécanisme : que fait cette phrase à la prédiction ?

## 4.6 — L'étape 6 du protocole

Le protocole se termine par : fermer, attendre, réécrire de mémoire.

Pourquoi cette étape n'est-elle pas négociable ? Que se passe-t-il exactement, si
on la saute, entre « avoir reçu la réponse » et « savoir faire » ?

## 4.7 — Le paradoxe de l'aide

Formulez en une phrase le paradoxe central de ce module : l'outil qui aide le
plus à court terme peut nuire le plus à long terme.

Puis : quelle est la résolution de ce paradoxe ? (Ce n'est pas « ne pas utiliser
l'outil ».)

---

# Niveau 5 — Construire votre pratique

## 5.1 — Votre fichier de cadrage

Créez `mes_regles_IA.md` dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/).

Il doit contenir :

- où vous en êtes dans le parcours
- ce que le modèle **ne doit jamais** faire à votre place
- ce que vous l'autorisez à faire
- vos raccourcis personnels — ceux que le niveau 3 vous a révélés

Testez-le : collez-le en début de session, puis demandez délibérément une
solution. Refuse-t-il ? Sinon, resserrez vos règles.

## 5.2 — Un journal de session

Pendant une semaine, tenez `journal_IA.md` dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/) :

| Date | Ce que j'ai demandé | Forme utilisée | Ai-je cherché 20 min avant ? | Ai-je réécrit après ? |
|---|---|---|---|---|

Relisez-le au bout d'une semaine. Deux questions :

- quelle est votre forme de prompt par défaut ?
- combien de fois avez-vous sauté l'étape des 20 minutes ?

Le journal est plus instructif que la leçon, parce qu'il parle de vous.

## 5.3 — Votre prompt d'interrogation permanent

Écrivez un prompt d'interrogation réutilisable, couvrant tout ce que vous avez vu
dans le parcours jusqu'ici.

Rangez-le. Lancez-le tous les vendredis. C'est votre contrôle continu, et
personne ne vous le fera passer à votre place.

## 5.4 — Enseigner

Choisissez la notion la plus difficile que vous ayez acquise dans ce dépôt.

Écrivez une explication pour quelqu'un qui débute — sans modèle, de mémoire, avec
une analogie et un exemple minimal.

Faites-la critiquer par un modèle. Corrigez. Publiez-la dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/).

Rien ne révèle un trou de compréhension comme la nécessité d'expliquer.

## 5.5 — La semaine sans

Une semaine entière sur ce dépôt, zéro assistance. Notez chaque envie de
demander, et ce que vous avez fait à la place.

À la fin, répondez honnêtement :

- qu'avez-vous perdu en vitesse ?
- qu'avez-vous gagné en autonomie ?
- refaites 1.4 (le test de la page blanche). Le délai avant la première ligne
  a-t-il changé ?

---

# Auto-évaluation

- [ ] Quelle est la différence entre un prompt qui livre et un prompt qui fait apprendre ?
- [ ] Qu'est-ce que l'illusion de compétence ? Quel test la tranche ?
- [ ] Citez les six formes de prompt d'apprentissage et à quoi sert chacune.
- [ ] Quelles sont les cinq parties d'un bon prompt ? Laquelle omet-on le plus ?
- [ ] Pourquoi « une question à la fois » est-il essentiel au prompt socratique ?
- [ ] Énoncez le protocole des 20 minutes de mémoire.
- [ ] Pourquoi l'étape 6 (réécrire) n'est-elle pas négociable ?
- [ ] Citez trois prompts qui freinent l'apprentissage, et ce qu'ils ont en commun.
- [ ] Pourquoi formuler un problème le résout-il souvent ?
- [ ] Quel est le seul test valable de ce que vous savez ?

---

# Solutions

Aucune n'est donnée.

Et une raison propre à cette leçon : **les réponses de ce fichier sont sur vous,
pas sur le sujet.** Le résultat de 1.1, vos zones muettes de 1.3, votre journal de
5.2 — personne d'autre ne peut les produire, et aucun modèle ne peut les vérifier.

Les questions du niveau 4 peuvent être discutées avec un modèle, mais en prompt
d'inversion : vous répondez d'abord, il critique ensuite. Dans l'autre sens, vous
reproduisez exactement le piège du niveau 3.
