# Garder la main

## La question de ce module

Les quatre leçons précédentes ont expliqué le mécanisme, la formulation, la carte
des usages et le travail avec un agent. Reste la question qui les motive toutes :

> **Comment utiliser ces outils tous les jours sans perdre la capacité de s'en
> passer ?**

Ce n'est pas une inquiétude morale. C'est un problème technique avec une réponse
technique, et cette leçon la donne : un protocole, des tests, et une règle de
décision.

## Le mécanisme de l'atrophie

L'apprentissage a une exigence physique : **l'effort de récupération**.

Retrouver une information de mémoire renforce la trace. La recevoir ne la
renforce pas. C'est mesuré, c'est robuste, et ça s'appelle l'*effet de test* :
se tester est plusieurs fois plus efficace que relire.

Toute la difficulté est là. Un modèle **supprime l'effort de récupération** —
c'est exactement son utilité. Quand vous avez oublié la syntaxe de `qsort`, la
recevoir en deux secondes est un gain de temps réel et un apprentissage annulé.

Sur une information triviale — l'ordre des arguments de `memcpy` — c'est un bon
échange. Sur un mécanisme structurant — pourquoi un tableau bat une liste chaînée
en parcours — c'est un mauvais échange, et **il ne se voit pas.**

Vous n'avez pas le sentiment de ne pas apprendre. Vous avez le sentiment
d'avancer vite. C'est précisément ce qui rend le problème difficile à corriger
seul.

### La triple illusion

Trois confusions se renforcent :

| Vous ressentez | Vous mesurez en réalité |
|---|---|
| « J'ai compris » | Que le code est lisible |
| « Je saurais le refaire » | Que vous reconnaissez la solution |
| « J'avance vite » | Que la machine avance vite |

La lisibilité d'un code correct n'est pas une preuve de compréhension. La
reconnaissance n'est pas la production. La vitesse du livrable n'est pas la
vitesse de l'apprentissage.

Un seul test tranche les trois : **fermer, attendre, réécrire.**

## Le test du dépôt fermé

C'est le seul indicateur fiable, et il tient en une phrase :

> **Fermez tout, et écrivez-le de mémoire.**

Fermé veut dire fermé : pas de modèle, pas de leçon, pas votre ancienne solution,
pas de recherche. Un éditeur vide et un compilateur.

Trois issues :

- **Ça compile et ça marche.** Acquis. Passez à la suite.
- **Ça compile après deux ou trois corrections.** En cours. Refaites-le demain.
- **Vous ne savez pas commencer.** Non acquis, quoi que vous ayez ressenti en
  lisant. Refaites l'exercice depuis l'énoncé, sans aide.

Le troisième cas est fréquent après une session d'IA productive. Ce n'est pas un
échec — c'est **l'information** que le module 3 attendait de vous donner, et que
la fluidité de la lecture avait masquée.

### Le calendrier minimal

```
Jour 0    Faire l'exercice. IA autorisée selon le protocole des 20 min.
Jour 1    Réécrire de mémoire, dépôt fermé. Noter les hésitations.
Jour 7    Réécrire à nouveau. Ce qui résiste encore est le vrai trou.
```

Trois passages sur les notions structurantes valent mieux que quinze exercices
faits une fois. Ce n'est pas de la rigueur pour la forme : c'est la seule façon
de convertir « j'ai compris » en « je sais faire ».

## Les notions non négociables

Toutes les connaissances ne se valent pas. Certaines peuvent être consultées à
vie ; d'autres doivent être en mémoire, parce que **c'est avec elles que vous
jugez le code que le modèle produit**.

| À savoir de mémoire, sans aide | Consultable à volonté |
|---|---|
| Ce qu'est un pointeur, ce qu'il contient | L'ordre des arguments de `qsort` |
| Pourquoi `malloc` a besoin de `free` | La syntaxe d'un pointeur de fonction |
| La différence tableau / liste chaînée | Les options de `gcc` au-delà des courantes |
| Reconnaître un O(n²) dans une boucle | Le nom exact d'un flag de `open()` |
| Ce qu'est une pile d'appels | La signature de `strtol` |
| Pourquoi HTTPS chiffre | Le numéro de port d'un service rare |
| Ce que fait un système d'exploitation | La syntaxe de `awk` |
| Lire un diff Git | Les options avancées de `git rebase` |

La colonne de gauche n'est pas de la syntaxe : ce sont des **modèles mentaux**.
Ils ne s'obtiennent pas par consultation, ils s'obtiennent par construction — et
c'est ce que le socle de ce dépôt construit.

Le critère qui sépare les deux colonnes :

> **Si je ne le sais pas, est-ce que je peux juger si la réponse du modèle est
> bonne ?**

Un nom de fonction faux ne compile pas : la machine vous corrige. Un choix
d'architecture faux compile parfaitement et vous coûte six mois. La colonne de
gauche est exactement l'ensemble des choses dont l'erreur **ne se signale pas**.

## Le protocole complet

À afficher au-dessus de l'écran.

```
AVANT DE DEMANDER
  1. Vingt minutes seul, minimum.
  2. Écrire le problème en trois phrases :
       ce que je veux / ce que j'obtiens / ce que j'ai essayé.
  3. (L'étape 2 résout souvent le problème.)

PENDANT
  4. Indice avant solution. Toujours.
  5. Sur mes exercices : lecture seule.
  6. Demander le POURQUOI, pas seulement le QUOI.

APRÈS
  7. Vérifier par la machine : compiler, exécuter, man.
  8. Fermer et réécrire de mémoire.
  9. Si je ne peux pas réécrire : ce n'est pas acquis. Recommencer.

CHAQUE SEMAINE
 10. Un exercice complet sans aucune assistance.
```

Le point 10 est le garde-fou. Une session par semaine entièrement sans IA — pas
par principe, mais pour **mesurer**. C'est votre indicateur : s'il devient
difficile, quelque chose a glissé, et vous le saurez tôt.

## Trois signaux d'alerte

Ils sont concrets et faciles à observer sur soi.

**« Je ne sais plus commencer une page blanche. »** Vous savez juger et corriger,
vous ne savez plus initier. C'est le signe le plus précoce, et il apparaît avant
tout autre. Traitement : un exercice par jour, du premier caractère au dernier,
sans assistance.

**« Le code dans mon dépôt, je ne saurais pas l'expliquer. »** Prenez un fichier
que vous avez « écrit » la semaine dernière et expliquez-le à voix haute, ligne
par ligne. S'il y a des zones muettes, il y a de la dette. Traitement : réécrire
ces zones de mémoire.

**« Je demande avant de réfléchir. »** Le réflexe s'est inversé : le modèle est
devenu la première étape au lieu de la troisième. C'est le plus difficile à voir
parce qu'il se sent comme de l'efficacité. Traitement : le protocole des 20
minutes, appliqué strictement pendant deux semaines.

## Le cas des débutants complets

Un mot particulier, parce que le risque n'est pas le même.

Un développeur expérimenté qui délègue perd de la vitesse d'écriture et garde son
jugement. **Un débutant qui délègue ne construit jamais le jugement.** Il n'a
rien pour évaluer ce qu'il reçoit, et l'écart entre ce qu'il produit et ce qu'il
comprend s'installe dès le premier mois.

Le symptôme classique : des projets impressionnants dans le dépôt, et
l'impossibilité d'écrire une boucle sans aide. Ce n'est pas de la paresse — c'est
la conséquence mécanique de l'ordre dans lequel les choses ont été faites.

D'où une recommandation nette pour ce parcours :

| Module | Usage recommandé |
|---|---|
| [0.Discover_IT](../../../0.Discover_IT/) | **IA en explication seule.** Aucun code à produire — c'est le module idéal pour prendre l'habitude du prompt d'explication et du prompt socratique. |
| [1.Learn_C](../../../1.Learn_C/) — jusqu'aux pointeurs | **Aucune génération de code.** Explications, relecture, interrogation. Vous tapez tout. C'est là que le modèle mental se construit ou ne se construit pas. |
| [1.Learn_C](../../../1.Learn_C/) — après les pointeurs | Génération autorisée sur l'outillage. Exercices toujours à la main. |
| [2.Learn_linux](../../../2.Learn_linux/) | **IA très utile** : elle explique les commandes obscures. Mais tapez-les vous-même, et lisez `man`. |
| [3.projects_to_learn_algo](../../../3.projects_to_learn_algo/) | **Structures à la main, sans exception.** L'infrastructure de mesure peut être déléguée. |
| Spécialisations | Usage professionnel : générer, relire le diff, vérifier. Le socle est là pour juger. |

La logique est constante : **on délègue ce qu'on saurait faire, jamais ce qu'on
apprend à faire.**

## Pourquoi ce module est un bonus

Il n'est pas dans le socle, et c'est volontaire.

Le socle enseigne à programmer. Ce module enseigne à utiliser un outil. L'ordre
compte : l'outil amplifie ce que vous savez déjà, il ne le remplace pas. Un
modèle entre les mains de quelqu'un qui sait juger du code est un multiplicateur.
Entre les mains de quelqu'un qui ne sait pas, c'est un plafond — atteint vite, et
jamais dépassé.

Ce que ce dépôt vise n'est pas de vous rendre plus rapide que le modèle. C'est de
vous rendre capable de dire, sur n'importe quel code qu'il produit : **ceci est
juste, ceci est faux, et voici pourquoi.**

Cette phrase est la définition du métier. Aucun outil ne la fournit.

## Ce qu'il faut retenir

- L'apprentissage exige un **effort de récupération**. Un modèle le supprime :
  c'est son utilité, et c'est le risque.
- L'illusion est triple : lisibilité prise pour compréhension, reconnaissance
  pour production, vitesse du livrable pour vitesse d'apprentissage.
- Un seul test tranche : **fermer, attendre, réécrire de mémoire.**
- Certaines notions doivent être **en mémoire** — celles dont l'erreur ne se
  signale pas. Le reste se consulte.
- Le **protocole** : 20 minutes seul, indice avant solution, vérifier par la
  machine, réécrire à la fin, une session sans IA par semaine.
- Trois signaux d'alerte : **page blanche impossible**, **code inexplicable**,
  **demander avant de réfléchir.**
- Pour un débutant, l'ordre est décisif : **on délègue ce qu'on saurait faire,
  jamais ce qu'on apprend à faire.**

---

## À faire maintenant

**1. Passer le test du dépôt fermé.** Choisissez trois exercices terminés dans
ce dépôt, de trois modules différents. Réécrivez-les de mémoire, sans rien
ouvrir. Compilez.

Remplissez honnêtement :

| Exercice | Compile du 1er coup ? | Nb de corrections | Verdict |
|---|---|---|---|
| | | | |

Le résultat est votre point de départ réel — pas ce que vous croyiez savoir.

**2. Faire votre partage de colonnes.** Listez dix notions vues dans ce dépôt.
Pour chacune, décidez : mémoire ou consultable ? Justifiez avec le critère de la
leçon — *son erreur se signalerait-elle ?*

Faites critiquer votre classement par un modèle. Défendez vos choix : c'est
l'exercice.

**3. Tenir une semaine sans assistance.** Une semaine complète sur ce dépôt, zéro
IA. Notez chaque fois que vous avez voulu demander, et ce que vous avez fait à la
place.

À la fin : qu'avez-vous perdu en vitesse ? Qu'avez-vous gagné en autonomie ? La
réponse honnête à ces deux questions est plus utile que toute cette leçon.

**4. Auditer votre propre dépôt.** Ouvrez cinq fichiers que vous avez écrits.
Expliquez chacun à voix haute, ligne par ligne.

Marquez les zones muettes — celles que vous ne savez pas expliquer. Réécrivez-les
de mémoire. C'est votre dette technique personnelle, et personne d'autre ne la
verra.

**5. Écrire vos propres règles.** Créez `mon_protocole.md` dans
[../2.practice_dev_with_AI/](../2.practice_dev_with_AI/). Pas une copie de cette
leçon : **vos** règles, tirées de ce que les exercices 1 à 4 vous ont appris sur
vos propres raccourcis.

Relisez-le dans un mois. Ajustez-le. C'est le seul document de ce module qui
compte vraiment, parce que c'est le seul que vous avez écrit vous-même.

---

Ce module est terminé. Les exercices sont dans
[../exercices/](../exercices/), et
[../2.practice_dev_with_AI/](../2.practice_dev_with_AI/) accueille vos fichiers
de règles et vos traces de session.

Le socle reste la priorité : [0.Discover_IT](../../../0.Discover_IT/) →
[1.Learn_C](../../../1.Learn_C/) → [2.Learn_linux](../../../2.Learn_linux/) →
[3.projects_to_learn_algo](../../../3.projects_to_learn_algo/).
