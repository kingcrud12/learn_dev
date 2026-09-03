# Exercices — Garder la main

Support : [05_garder_la_main.md](../lessons/05_garder_la_main.md)

Ce fichier est différent des quatre précédents.

Les autres portaient sur l'outil. **Celui-ci porte sur vous.** Il n'y a rien à
vérifier dans un terminal, aucune hallucination à détecter, aucun diff à lire. Il
n'y a que des mesures à faire sur votre propre compétence, et à répéter dans le
temps.

C'est pour cette raison qu'il est le seul dont les résultats **ne peuvent pas
être obtenus d'un modèle**. Personne ne peut passer ces tests à votre place, et
aucune réponse n'est vérifiable de l'extérieur.

**Une exigence particulière** : plusieurs exercices demandent d'attendre — un
jour, une semaine, un mois. Ce délai n'est pas une contrainte administrative,
c'est le mécanisme même de la mesure. Un test passé immédiatement après avoir lu
la réponse ne mesure rien.

Tenez un fichier de suivi dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/) — vous y
reviendrez.

---

# Niveau 1 — Mesurer où vous en êtes

## 1.1 — Le test du dépôt fermé, sur trois modules

Choisissez trois exercices terminés, dans trois modules différents.

Fermé veut dire fermé : pas de modèle, pas de leçon, pas votre ancienne solution,
pas de recherche. Un éditeur vide et un compilateur.

```sh
gcc -Wall -Wextra -g reecriture.c -o prog && ./prog
```

| Exercice | Module | Compile du 1er coup ? | Nb corrections | Verdict |
|---|---|---|---|---|
| | | | | |
| | | | | |
| | | | | |

Verdicts : **acquis** / **en cours** / **non acquis**.

Ce tableau est votre point de départ réel — pas ce que vous croyiez savoir.
Datez-le.

## 1.2 — Le délai de la page blanche

Éditeur vide, chronomètre. Écrivez de zéro un programme qui compte les mots
d'une ligne saisie au clavier.

Notez : **combien de secondes avant la première ligne écrite ?**

Ce délai est le signal d'alerte le plus précoce de la leçon. Vous le remesurerez
en 5.2. Datez-le.

## 1.3 — L'audit des zones muettes

Cinq fichiers `.c` que vous avez écrits. Expliquez chacun à voix haute, ligne par
ligne.

Notez chaque zone où vous butez :

| Fichier | Ligne / bloc | Ce que je ne sais pas expliquer |
|---|---|---|
| | | |

Combien de zones muettes sur cinq fichiers ? C'est votre dette technique
personnelle, et personne d'autre ne la verra.

## 1.4 — Le tri des trois signaux

Les trois signaux d'alerte de la leçon. Pour chacun, évaluez-vous honnêtement sur
une échelle de 1 à 5 :

| Signal | Note | Preuve concrète |
|---|---|---|
| Je ne sais plus commencer une page blanche | | (le résultat de 1.2) |
| Le code de mon dépôt, je ne saurais pas l'expliquer | | (le résultat de 1.3) |
| Je demande avant de réfléchir | | (votre journal du fichier 02) |

La colonne « preuve » est obligatoire. Une auto-évaluation sans preuve mesure
votre humeur, pas votre compétence.

---

# Niveau 2 — Le calendrier de répétition

Ces exercices s'étalent sur une semaine. Commencez-les maintenant, terminez-les
plus tard.

## 2.1 — Le cycle jour 0 / jour 1 / jour 7

Choisissez une notion structurante que vous n'avez pas encore acquise — une
liste chaînée, un tri fusion, une table de hachage.

```
Jour 0    Faire l'exercice. IA autorisée selon le protocole des 20 min.
Jour 1    Réécrire de mémoire, dépôt fermé. Noter les hésitations.
Jour 7    Réécrire à nouveau. Ce qui résiste encore est le vrai trou.
```

| | Compile ? | Corrections | Ce qui a résisté |
|---|---|---|---|
| Jour 0 | | | |
| Jour 1 | | | |
| Jour 7 | | | |

Que résiste-t-il au jour 7 ? Ce point précis est ce qu'il faut travailler — pas
la notion entière.

## 2.2 — Trois passages contre quinze exercices

La leçon affirme que trois passages sur une notion valent mieux que quinze
exercices faits une fois.

Testez-le. Prenez deux notions de difficulté comparable :

- **notion A** : trois passages selon le calendrier de 2.1
- **notion B** : trois exercices différents, faits une fois chacun

Au jour 14, testez les deux au dépôt fermé. Laquelle tient ?

## 2.3 — L'effet de test

Prenez deux notions non acquises.

- **notion C** : relisez la leçon trois fois, à un jour d'intervalle.
- **notion D** : lisez la leçon une fois, puis testez-vous trois fois (prompt
  d'interrogation, ou réécriture) à un jour d'intervalle.

Au jour 7, testez les deux. Quel écart ?

Vous venez de mesurer sur vous-même l'effet de test cité par la leçon. Le
résultat est-il conforme ?

---

# Niveau 3 — Le partage des colonnes

## 3.1 — Votre classement

Listez dix notions vues dans ce dépôt. Pour chacune, décidez : **mémoire** ou
**consultable** ?

Justifiez chaque ligne avec le critère de la leçon :

> Si je ne le sais pas, est-ce que je peux juger si la réponse du modèle est
> bonne ?

| Notion | Mémoire / consultable | Justification |
|---|---|---|
| | | |

## 3.2 — Défendre votre classement

Faites critiquer votre tableau de 3.1 par un modèle.

**Défendez vos choix.** Ne les changez que si l'argument vous convainc
réellement — et notez lesquels vous avez changés et pourquoi.

C'est un exercice de jugement, pas de correction. Un modèle n'a pas autorité sur
ce que **vous** devez savoir de mémoire.

## 3.3 — Le test du critère

Prenez cinq erreurs que le modèle a commises cette semaine (votre journal du
fichier 03).

Pour chacune : l'erreur s'est-elle **signalée** ? Comment — compilateur, test,
exécution, ou pas du tout ?

Celles qui ne se sont pas signalées relèvent de la colonne « mémoire ». Votre
classement de 3.1 les y avait-il mises ?

## 3.4 — Ce que la machine ne rattrape pas

Faites deux listes, à partir de votre expérience réelle dans ce dépôt :

- les erreurs que `gcc`, un détecteur de fuites ou un test rattrapent
- celles qui compilent, passent, et coûtent cher plus tard

Donnez un exemple concret de la seconde liste, tiré de votre propre code ou d'un
choix que vous avez fait.

La seconde liste est la définition opérationnelle de la colonne « mémoire ».
Reformulez-la avec vos mots.

---

# Niveau 4 — Raisonner sur l'autonomie

Réponses par écrit. Ces questions n'ont pas de réponse unique, et un modèle n'a
aucune autorité particulière sur elles.

## 4.1 — L'effort de récupération

Expliquez avec vos mots pourquoi retrouver une information de mémoire renforce
l'apprentissage, alors que la recevoir ne le renforce pas.

Puis : en quoi un modèle de langage est-il, structurellement, une machine à
supprimer cet effort ?

## 4.2 — Le bon et le mauvais échange

Recevoir l'ordre des arguments de `memcpy` en deux secondes : bon échange.
Recevoir pourquoi un tableau bat une liste chaînée en parcours : mauvais échange.

Où passe exactement la frontière ? Formulez-la sans reprendre les mots de la
leçon.

Donnez trois exemples de chaque côté, tirés de votre travail réel.

## 4.3 — La triple illusion

Les trois confusions de la leçon : lisibilité prise pour compréhension,
reconnaissance pour production, vitesse du livrable pour vitesse d'apprentissage.

Laquelle avez-vous personnellement le plus vécue ? Décrivez une occasion précise,
datée.

Pourquoi un seul test les tranche-t-il toutes les trois ?

## 4.4 — Le débutant et l'expérimenté

« Un développeur expérimenté qui délègue perd de la vitesse d'écriture et garde
son jugement. Un débutant qui délègue ne construit jamais le jugement. »

Développez. Pourquoi l'ordre dans lequel les choses sont faites est-il décisif ?

Puis : à quel moment de votre progression cesserez-vous d'être dans le second cas ?
Quel critère observable utiliserez-vous pour le savoir ?

## 4.5 — L'outil comme plafond

« Un modèle entre les mains de quelqu'un qui sait juger du code est un
multiplicateur. Entre les mains de quelqu'un qui ne sait pas, c'est un plafond. »

Expliquez le mot « plafond ». Pourquoi est-il atteint vite, et jamais dépassé ?

## 4.6 — Pourquoi ce module est un bonus

Ce module n'est pas dans le socle. Justifiez ce choix.

Puis prenez le contre-pied : quel serait l'argument pour le placer **avant** le
socle ? Pourquoi cet argument est-il, ou n'est-il pas, convaincant ?

## 4.7 — La définition du métier

« Être capable de dire, sur n'importe quel code : ceci est juste, ceci est faux,
et voici pourquoi. »

Sur quelles catégories de code en êtes-vous capable aujourd'hui ? Sur lesquelles
non ?

Quel module de ce dépôt vous rapprocherait le plus de la seconde liste ?

## 4.8 — La résolution du paradoxe

Le paradoxe du module : l'outil qui aide le plus à court terme peut nuire le plus
à long terme.

Formulez sa résolution en une phrase. Ce n'est ni « ne pas l'utiliser », ni
« l'utiliser sans limite ».

Puis : votre phrase est-elle applicable demain matin, concrètement ? Si non,
reformulez-la jusqu'à ce qu'elle le soit.

---

# Niveau 5 — Installer la pratique

## 5.1 — La semaine sans assistance

Une semaine complète sur ce dépôt, zéro IA. Notez chaque envie de demander :

| Date | Ce que je voulais demander | Ce que j'ai fait à la place | Résolu ? | Temps |
|---|---|---|---|---|

À la fin, répondez honnêtement :

- qu'avez-vous perdu en vitesse ?
- qu'avez-vous gagné en autonomie ?
- combien de vos envies de demander se sont résolues seules ?

Cette dernière réponse est la plus instructive du module.

## 5.2 — La remesure

Refaites 1.1, 1.2 et 1.3 à l'identique, après la semaine sans.

| Mesure | Jour 0 | Après la semaine sans | Écart |
|---|---|---|---|
| Exercices acquis (sur 3) | | | |
| Délai page blanche (s) | | | |
| Zones muettes (sur 5 fichiers) | | | |

L'écart est le seul résultat de ce module qui compte, parce que c'est le seul
qui soit une donnée sur vous et non une opinion.

## 5.3 — La session hebdomadaire

Installez le point 10 du protocole : **un exercice complet par semaine, sans
aucune assistance.**

Tenez une trace :

| Semaine | Exercice | Fait sans aide ? | Difficulté ressentie (1-5) |
|---|---|---|---|

Sur un mois, la colonne « difficulté ressentie » est votre indicateur. Si elle
monte, quelque chose a glissé — et vous le saurez tôt, ce qui est exactement le
but.

## 5.4 — Le remboursement de dette

Reprenez votre tableau de zones muettes (1.3).

Pour chacune : réécrivez le bloc de mémoire, sans aide, jusqu'à ce qu'il compile
et que vous sachiez l'expliquer.

| Zone muette | Réécrite le | Sais-je l'expliquer ? |
|---|---|---|

Videz ce tableau. C'est le seul travail de ce module qui améliore directement
votre code.

## 5.5 — Votre protocole personnel

Créez `mon_protocole.md` dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/).

**Pas une copie de la leçon.** Vos règles, tirées de ce que les exercices 1 à 4
vous ont appris sur vos propres raccourcis. Il doit contenir :

- vos règles avant / pendant / après
- vos trois signaux d'alerte personnels, avec la mesure qui les détecte
- votre partage des colonnes (3.1, révisé)
- votre rendez-vous hebdomadaire

Datez-le. Relisez-le dans un mois. Ajustez ce que l'expérience a démenti.

C'est le seul document de ce module qui compte vraiment, parce que c'est le seul
que vous avez écrit vous-même.

## 5.6 — Enseigner pour vérifier

Choisissez la notion la plus difficile que vous ayez acquise dans ce parcours.

Écrivez une explication complète pour un débutant : analogie, exemple minimal,
piège classique. **De mémoire, sans rien relire, sans modèle.**

Faites-la critiquer. Corrigez. Publiez-la dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/).

Ce que vous avez omis est ce que vous croyiez savoir. C'est le dernier exercice
du module, et c'est le même que le premier de la leçon 02 — l'inversion. Il n'a
pas changé de forme, mais votre réponse, si.

---

# Auto-évaluation

Ce module est terminé si vous répondez sans hésiter :

- [ ] Qu'est-ce que l'effort de récupération ? Pourquoi un modèle le supprime-t-il ?
- [ ] Citez la triple illusion. Quel test la tranche ?
- [ ] Énoncez le test du dépôt fermé et ses trois issues.
- [ ] Quel est le calendrier minimal de répétition ?
- [ ] Quel critère sépare « à savoir de mémoire » de « consultable » ?
- [ ] Énoncez le protocole complet, de mémoire.
- [ ] Citez les trois signaux d'alerte et le traitement de chacun.
- [ ] Pourquoi le risque n'est-il pas le même pour un débutant ?
- [ ] Que délègue-t-on, que ne délègue-t-on jamais ? Énoncez la règle.
- [ ] Pourquoi ce module n'est-il pas dans le socle ?

---

# Solutions

Aucune n'est donnée, et pour ce fichier c'est structurel : **il n'y en a pas.**

Les quatre fichiers précédents avaient un arbitre — `gcc`, `man`, le détecteur de
fuites, le diff. Celui-ci n'en a aucun, parce que ses réponses sont des mesures sur vous :
le résultat de 1.1, votre délai de page blanche, vos zones muettes, l'écart de
5.2.

Trois recours, dans l'ordre :

1. **Refaire la mesure.** Un test du dépôt fermé qui échoue n'est pas une
   question sans réponse, c'est une réponse. Elle dit : refaire l'exercice.
2. **Relire la leçon**, pour les questions du niveau 4.
3. **Le modèle en prompt d'inversion** — vous répondez, il critique. Jamais
   l'inverse sur ce fichier : sur ce que vous devez savoir de mémoire, votre
   jugement a plus d'autorité que le sien.

Et un avertissement final, qui est le sujet du module entier : **ne demandez pas
à un modèle de remplir ces tableaux.** Il le fera très bien, avec des valeurs
plausibles. Ce sera le meilleur exemple de travail plausible de tout ce module —
et le seul qui vous coûte quelque chose que personne ne pourra vous rendre.
