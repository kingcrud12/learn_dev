# Exercices — Ce que l'IA fait bien, ce qu'elle fait mal

Support : [03_ce_que_lIA_fait_bien.md](../lessons/03_ce_que_lIA_fait_bien.md)

Ces exercices construisent une compétence unique : **savoir, avant de demander,
si la réponse sera fiable.**

Ils demandent un terminal, un modèle, et du code à vous. La plupart des
vérifications se font avec `gcc`, `man` et `bc`.

**La règle du module** : ce qui se vérifie par la machine se vérifie par la
machine. Un modèle ne valide pas un autre modèle.

---

# Niveau 1 — Le compilateur comme arbitre

## 1.1 — La fonction qui n'existe pas

Demandez : « quelle fonction de la bibliothèque standard C inverse une chaîne en
place ? »

Écrivez un programme minimal qui utilise ce qu'il propose, et compilez :

```sh
gcc -Wall -Wextra /tmp/test.c -o /tmp/test
```

Notez le message exact de `gcc`. Combien de secondes pour trancher ?

## 1.2 — Le paramètre en trop

Demandez la signature exacte de cinq fonctions : `fopen`, `strtol`, `snprintf`,
`qsort`, `realloc`.

Vérifiez chacune :

```sh
man 3 fopen strtol snprintf qsort realloc
```

| Fonction | Écart constaté | Nature de l'écart |
|---|---|---|
| | | |

Sur les écarts trouvés : `gcc` les aurait-il tous détectés ? Testez-en un.

## 1.3 — Le paquet inventé

Demandez le nom d'une bibliothèque C pour une tâche très spécifique de votre
choix. Cherchez si elle existe.

Puis demandez trois bibliothèques C **très connues** (JSON, tests, réseau).
Existent-elles ?

Quelle est la différence entre les deux demandes ? Reliez-la au critère de la
leçon.

## 1.4 — Faire calculer la machine

Cinq calculs, sans script autorisé :

- `7919 × 6841`
- `2^24`
- le nombre d'octets dans 3,5 Gio
- `factorielle(15)`
- combien de secondes dans 47 jours

Vérifiez :

```sh
echo "7919 * 6841" | bc
echo "2^24" | bc
echo "3.5 * 2^30" | bc
echo "define f(n){if(n<=1) return 1; return n*f(n-1)} f(15)" | bc
echo "47 * 24 * 3600" | bc
```

Score ? Puis redemandez en exigeant les commandes. Score ?

## 1.5 — La question de version

Demandez la dernière version stable de `gcc`, `make`, `git`, `valgrind`.

```sh
gcc --version | head -1
make --version | head -1
git --version
valgrind --version
```

Puis cherchez les versions réellement actuelles dans la documentation officielle.

Formulez la règle sur les questions de version, en une phrase.

---

# Niveau 2 — La zone de haute fiabilité

Ici, on utilise le modèle **pour ce qu'il fait bien**. Le but est de sentir la
différence de qualité par rapport au niveau 1.

## 2.1 — Décoder une erreur de compilation

Écrivez délibérément cinq erreurs classiques en C :

- affecter un `char **` à un `char *`
- appeler une fonction non déclarée
- oublier un `;`
- `printf("%d", une_chaine)`
- retourner un pointeur vers une variable locale

Compilez avec `-Wall -Wextra`. Collez chaque message **complet**, avec le code,
et demandez une explication.

| Erreur | Diagnostic juste ? | Correctif compile ? |
|---|---|---|
| | | |

Comparez ce taux à celui du niveau 1. Qu'est-ce qui change ?

## 2.2 — L'explication calibrée

Prenez la notion la plus difficile de votre progression actuelle.

Demandez une explication en précisant votre niveau exact et votre blocage exact.
Puis posez trois questions de suivi, celles qu'aucun tutoriel n'aurait prévues.

En quoi cet enchaînement diffère-t-il d'une lecture de documentation ? C'est
l'avantage décisif de la leçon — nommez-le.

## 2.3 — La relecture de code

Prenez une implémentation finie de
[3.projects_to_learn_algo](../../../3.projects_to_learn_algo/).

Demandez : correct / fuite ou crash et sur quelle entrée / ce qu'un développeur
expérimenté écrirait autrement — classé par gravité, **sans réécriture**.

Puis vérifiez chaque remarque de fuite vous-même :

```sh
gcc -Wall -Wextra -g votre_code.c -o prog
valgrind --leak-check=full ./prog
```

| Remarque | Vérifiée comment | Juste ? |
|---|---|---|
| | | |

Combien de remarques justes ? Combien de fausses ? Combien avait-il manquées que
`valgrind` a trouvées ?

## 2.4 — Les cas limites

Prenez votre recherche dichotomique. Demandez la liste des cas limites qui
devraient la casser — **la liste seulement**, pas les tests écrits.

Avant de lire : écrivez **votre** liste.

Comparez. Ce qu'il a vu et vous non est votre angle mort sur les cas limites.
Testez-les tous réellement.

## 2.5 — Comparer deux approches

Posez une question de choix réel : table de hachage ou arbre binaire pour un cas
que vous décrivez précisément.

Exigez **le critère de décision**, pas la réponse.

Puis, sans relire : re-justifiez le choix de mémoire. Si vous ne pouvez pas, vous
avez reçu une réponse et non un critère — c'est la vérification de niveau 3.

## 2.6 — Explorer du code inconnu

Prenez un fichier de ce dépôt que vous n'avez pas écrit — une leçon avec du code,
ou un projet.

Demandez une explication de sa structure. Vérifiez ses affirmations dans le
fichier réel.

Y a-t-il des affirmations inventées ? Le code était dans le contexte — cela
change-t-il quelque chose ?

---

# Niveau 3 — Le code juste qu'on ne comprend pas

Le piège central de la leçon. Ces exercices sont les plus importants du fichier.

## 3.1 — La ligne dense

Demandez d'écrire `strlen` en une seule ligne, la plus dense possible. Le code
marchera.

Expliquez-le **par écrit**, terme par terme, sans aide : l'ordre d'évaluation, la
valeur de chaque expression, la condition d'arrêt.

Faites corriger votre explication. Combien d'erreurs ?

## 3.2 — La copie de chaîne

Voici du code correct et idiomatique :

```c
while ((*d++ = *s++));
```

Sans aide, répondez par écrit :

- que vaut l'expression `*d++ = *s++` ?
- dans quel ordre l'incrémentation et l'affectation ont-elles lieu ?
- pourquoi la boucle s'arrête-t-elle ?
- que faut-il garantir sur `d` pour que ce soit sûr ?

Vérifiez ensuite. Écrivez un programme qui l'utilise et exécutez-le sous
`valgrind`.

## 3.3 — Le code généré non compris

Demandez une fonction que vous ne sauriez pas écrire — un tri par tas, ou une
table de hachage avec adressage ouvert.

Le code marchera. Maintenant :

1. Compilez, testez. Ça marche.
2. Fermez tout. Attendez une heure.
3. Réécrivez de mémoire.

Y arrivez-vous ? Ce code est-il dans votre programme, ou dans votre tête ?

## 3.4 — Le test des quatre niveaux

Prenez le code de 3.3 et appliquez les quatre niveaux de vérification :

| Niveau | Ce que j'ai fait | Résultat |
|---|---|---|
| 1 — la machine | | |
| 2 — la source | | |
| 3 — saurais-je le réécrire ? | | |
| 4 — recouper | | |

Lequel des quatre a échoué ? La leçon prédit lequel — avait-elle raison ?

## 3.5 — La dette invisible

Cherchez dans votre dépôt du code que vous avez obtenu d'un modèle et gardé.

Pour chaque morceau : sauriez-vous le réécrire ? Sauriez-vous le défendre en
entretien ?

Réécrivez de mémoire ceux dont la réponse est non. C'est votre remboursement de
dette, et personne ne vous le demandera.

---

# Niveau 4 — Raisonner sur la carte

Réponses par écrit.

## 4.1 — Le critère

La leçon donne un seul critère de fiabilité. Énoncez-le de mémoire.

Puis testez-le : prenez cinq questions que vous avez posées cette semaine, et
prédisez leur fiabilité avec le critère seul. Aviez-vous raison ?

## 4.2 — Le paradoxe de compétence

Un modèle explique un arbre rouge-noir mieux qu'un tutoriel, et se trompe sur un
paramètre de `fopen`.

Pourquoi ce n'est pas incohérent. Puis : donnez trois autres paires de ce type,
tirées de ce dépôt.

## 4.3 — Le détecteur d'hallucinations

`gcc` tranche une hallucination d'API en secondes.

Faites deux listes :

- les erreurs de modèle que le compilateur détecte
- celles qui lui échappent complètement

La seconde liste est la plus importante. Pourquoi ? Quel type d'erreur y
figure-t-il, et pourquoi est-il plus coûteux ?

## 4.4 — Le niveau 3 de vérification

« Saurais-je réécrire et défendre ce code ? »

Pourquoi la leçon en fait-elle le seul niveau non négociable, alors que les trois
autres sont plus objectifs ?

## 4.5 — Le danger du code juste

« Le danger n'est pas le code faux, c'est le code juste qu'on ne comprend pas. »

Développez. Pourquoi le code faux est-il moins dangereux ? Quand, exactement, le
code juste incompris devient-il un problème ?

## 4.6 — Ce que ce parcours enseigne

Une liste chaînée s'obtient en trente secondes avec un modèle. Le module 3 prend
des semaines.

Le module 3 est-il obsolète ? Répondez avec le tableau de la fin de la leçon.

Puis : qu'est-ce qui distingue « développeur assisté par IA » et « intermédiaire
entre un modèle et un compilateur » ?

## 4.7 — L'architecture étendue

Sur une fonction, un modèle est fort. Sur quinze fichiers qui interagissent, il
perd le fil.

Expliquez-le avec le contexte de la leçon 01.

Quelle pratique cela impose-t-il sur les changements importants ?

---

# Niveau 5 — Construire votre carte

## 5.1 — Le journal des erreurs

Pendant une semaine complète, notez dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/) chaque erreur du
modèle :

| Date | Question | Erreur | Type de tâche | Détectée par |
|---|---|---|---|---|

Au bout d'une semaine, regroupez par type de tâche. Vous avez votre carte
personnelle — plus utile que celle de la leçon, parce qu'elle décrit vos usages.

## 5.2 — Prédire avant de tester

Dix questions techniques sur votre travail en cours. Pour chacune, **avant de la
poser** : prédisez fiable / à vérifier / non fiable.

Posez-les, vérifiez.

| Question | Prédiction | Réalité | Correct ? |
|---|---|---|---|

Score sur dix. Un score élevé signifie que le critère est intégré — c'est
l'objectif de la leçon.

## 5.3 — Le tableau de décision, version personnelle

Réécrivez le tableau de décision de la leçon avec **vos** lignes : les tâches que
vous faites réellement dans ce dépôt.

Rangez-le dans
[../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/). Relisez-le dans
un mois et corrigez ce que l'expérience a démenti.

## 5.4 — Apprendre valgrind en le regardant faire

Introduisez trois bugs mémoire dans un programme : fuite, `free` double, accès
hors bornes.

Demandez un diagnostic par `valgrind`, en demandant les commandes exactes.

Exécutez-les vous-même. Lisez la sortie brute. Comprenez chaque ligne.

Vous venez d'apprendre `valgrind` — c'est un des meilleurs usages du modèle, et
il ne vous a rien coûté en apprentissage.

## 5.5 — Le contre-test

Choisissez une affirmation technique du modèle qui vous paraît juste et que vous
ne pouvez pas vérifier par la machine — un jugement sur une pratique, un choix de
conception.

Cherchez-la dans une source qui fait autorité : documentation officielle, norme,
livre de référence.

Que trouvez-vous ? La question est-elle même tranchée par une autorité ?

C'est la catégorie la plus difficile : ni vérifiable par la machine, ni tranchée
par une source. Comment décidez-vous, alors ?

---

# Auto-évaluation

- [ ] Quel est le critère unique qui prédit la fiabilité d'une réponse ?
- [ ] Citez cinq tâches de la zone de haute fiabilité.
- [ ] Citez cinq tâches de la zone de faible fiabilité.
- [ ] Pourquoi le compilateur est-il un détecteur d'hallucinations ?
- [ ] Quelles erreurs de modèle échappent au compilateur ?
- [ ] Que faire de toute demande de calcul ou de comptage ?
- [ ] Énoncez les quatre niveaux de vérification. Lequel est non négociable ?
- [ ] Pourquoi le code juste incompris est-il plus dangereux que le code faux ?
- [ ] Pourquoi un modèle perd-il le fil sur quinze fichiers ?
- [ ] Le module 3 est-il rendu obsolète par l'IA ? Justifiez.

---

# Solutions

Aucune n'est donnée.

Mais pour ce fichier en particulier, la plupart des réponses des niveaux 1 à 3
**sont dans votre terminal** :

```sh
gcc -Wall -Wextra fichier.c -o prog    # la fonction existe, le type est bon
valgrind --leak-check=full ./prog      # la fuite est réelle ou non
man 3 fonction                          # la signature exacte
echo "..." | bc                         # le calcul juste
```

C'est le sujet même de la leçon : vous n'avez pas besoin d'un corrigé, vous avez
besoin du réflexe de vérifier. Le corrigé serait contre-pédagogique ici — il
remplacerait la vérification par une lecture, ce que la leçon entière déconseille.

Les questions du niveau 4 se discutent en prompt d'inversion : vous répondez
d'abord, le modèle critique ensuite.
