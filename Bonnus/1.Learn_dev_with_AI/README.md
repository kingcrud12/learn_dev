# Apprendre à développer avec l'IA

Module **bonus**, hors du socle. Il n'est pas un prérequis, et il ne remplace
aucun module du parcours.

## Pourquoi ce module existe

Un modèle de langage écrit une liste chaînée en trente secondes. C'est
exactement ce que le module
[3.projects_to_learn_algo](../../3.projects_to_learn_algo/) demande des semaines
à apprendre.

On peut en tirer deux conclusions opposées. La première — « le socle est
obsolète » — est fausse, et ce module explique pourquoi. La seconde — « il ne
faut pas utiliser ces outils » — est intenable : ils font partie du métier.

La bonne question n'est ni l'une ni l'autre :

> **Comment utiliser ces outils tous les jours sans perdre la capacité de s'en
> passer ?**

Ce module y répond de façon technique : un mécanisme à comprendre, une carte des
usages, un protocole, et des tests pour se mesurer.

## Ce qui distingue ce module

Il n'enseigne pas à faire écrire du code par une IA. Il enseigne à **apprendre
plus vite avec une IA, sans hollowing** — sans que le dépôt se remplisse de code
qu'on ne saurait pas réécrire.

La règle n°1 du dépôt reste la référence :

> *Ne pas copier-coller du code que je ne saurais pas réécrire.*

Ces outils rendent cette règle très facile à enfreindre, et beaucoup plus
importante à tenir. C'est le sujet du module.

## Contenu

| Leçon | Exercices | Sujet |
|---|---|---|
| 1. [Ce qu'est un modèle de langage](lessons/01_ce_quest_un_LLM.md) | [→](exercices/01_ce_quest_un_LLM.md) | Prédiction, tokens, hallucination, contexte |
| 2. [Prompter pour apprendre](lessons/02_prompter_pour_apprendre.md) | [→](exercices/02_prompter_pour_apprendre.md) | Six formes utiles, cinq pièges, le protocole des 20 min |
| 3. [Ce que l'IA fait bien, ce qu'elle fait mal](lessons/03_ce_que_lIA_fait_bien.md) | [→](exercices/03_ce_que_lIA_fait_bien.md) | La carte des usages, les quatre niveaux de vérification |
| 4. [Travailler avec un agent de code](lessons/04_travailler_avec_un_agent.md) | [→](exercices/04_travailler_avec_un_agent.md) | Outils, Git comme filet, le travail plausible |
| 5. [Garder la main](lessons/05_garder_la_main.md) | [→](exercices/05_garder_la_main.md) | Atrophie, test du dépôt fermé, protocole complet |

Lisez-les dans cet ordre. La leçon 1 est le socle des quatre autres : sans le
mécanisme, les conseils des leçons suivantes ne sont que des recettes.

## Quand faire ce module

**Pas avant** d'avoir écrit du code à soi. Les exercices portent sur *votre*
code : sans lui, ils n'ont pas de matière.

| Vous en êtes à | Recommandation |
|---|---|
| Avant [0.Discover_IT](../../0.Discover_IT/) | Attendez. Lisez la leçon 1 seule si vous êtes curieux. |
| Pendant [0.Discover_IT](../../0.Discover_IT/) | Leçons 1 à 3. Le module ne demande pas de code, c'est idéal pour prendre l'habitude du prompt d'explication. |
| Après les pointeurs en [1.Learn_C](../../1.Learn_C/) | **Le bon moment.** Tout le module, exercices inclus. |
| Après [2.Learn_linux](../../2.Learn_linux/) | La leçon 4 devient pleinement exploitable — elle suppose Git. |
| En spécialisation | Relisez les leçons 3 et 5. L'usage change de nature. |

## Prérequis

- **Un accès à un modèle de langage.** N'importe lequel : le module porte sur le
  mécanisme commun, pas sur un outil.
- **Un terminal.** La plupart des vérifications se font avec `gcc`, `man`, `bc`,
  et un détecteur de fuites (`leaks` sur macOS, `valgrind` sous Linux).
- **Du code à soi.** Au moins les trois premières leçons de
  [1.Learn_C](../../1.Learn_C/).
- **Git**, pour la leçon 4 — voir [2.Learn_linux](../../2.Learn_linux/). C'est
  une condition de sécurité, pas un confort : un agent modifie des fichiers.

## Comment travailler dans ce module

**Vérifier par la machine, jamais par un modèle.** C'est la règle qui traverse
tout le module. Un modèle ne valide pas un autre modèle.

```sh
gcc -Wall -Wextra -g fichier.c -o prog    # la fonction existe, le type est bon
leaks --atExit -- ./prog                   # la fuite est réelle ou non (macOS)
valgrind --leak-check=full ./prog          # idem (Linux)
man 3 fonction                              # la signature exacte
echo "2^24" | bc                            # le calcul juste
git diff HEAD                               # ce que l'agent a réellement fait
```

**Répondre par écrit.** Les niveaux 4 de chaque fichier d'exercices sont des
questions de raisonnement sans réponse unique. Les formuler par écrit *est*
l'exercice.

**Ne pas faire remplir les tableaux par un modèle.** Plusieurs exercices
demandent de se mesurer — délai avant la première ligne, zones muettes, exercices
réécrits de mémoire. Un modèle remplira ces tableaux très bien, avec des valeurs
plausibles, et vous coûtera la seule information que le module pouvait produire.

**Aucun corrigé n'est fourni.** Comme partout dans ce dépôt, et ici pour une
raison supplémentaire : la plupart des réponses sont dans votre terminal ou sur
vous.

## Le protocole, en résumé

À afficher au-dessus de l'écran. Détaillé en
[leçon 5](lessons/05_garder_la_main.md).

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

## Usage recommandé, module par module

La règle : **on délègue ce qu'on saurait faire, jamais ce qu'on apprend à
faire.**

| Module | Usage recommandé |
|---|---|
| [0.Discover_IT](../../0.Discover_IT/) | **Explication seule.** Aucun code à produire — le module idéal pour s'entraîner au prompt d'explication et au socratique. |
| [1.Learn_C](../../1.Learn_C/) jusqu'aux pointeurs | **Aucune génération de code.** Explications, relecture, interrogation. Vous tapez tout. |
| [1.Learn_C](../../1.Learn_C/) après les pointeurs | Génération autorisée sur l'outillage. Exercices toujours à la main. |
| [2.Learn_linux](../../2.Learn_linux/) | **Très utile** pour décoder les commandes obscures. Mais tapez-les, et lisez `man`. |
| [3.projects_to_learn_algo](../../3.projects_to_learn_algo/) | **Structures à la main, sans exception.** L'infrastructure de mesure peut être déléguée. |
| Spécialisations | Usage professionnel : générer, lire le diff, vérifier. Le socle est là pour juger. |

## Progression

- [ ] Leçon 1 — le mécanisme
- [ ] Exercices 1 — voir les tokens, provoquer une hallucination
- [ ] Leçon 2 — les formes de prompt
- [ ] Exercices 2 — mesurer l'illusion de compétence
- [ ] Leçon 3 — la carte des usages
- [ ] Exercices 3 — le compilateur comme arbitre
- [ ] Leçon 4 — les agents (nécessite Git)
- [ ] Exercices 4 — détecter le travail plausible
- [ ] Leçon 5 — garder la main
- [ ] Exercices 5 — le test du dépôt fermé, la semaine sans
- [ ] `mon_protocole.md` écrit dans [../2.practice_dev_with_AI/](../2.practice_dev_with_AI/)

## Ce que ce module ne couvre pas

Dit franchement :

- **Le fonctionnement interne d'un transformeur.** Attention, plongement,
  descente de gradient. On s'arrête au niveau où le mécanisme change votre usage.
- **La comparaison des outils.** Aucun n'est recommandé ni évalué : les leçons
  portent sur ce qui leur est commun, et le paysage change trop vite.
- **L'ingénierie de prompt avancée** pour la production — chaînage,
  auto-cohérence, jugement automatisé. Ce module porte sur l'apprentissage.
- **Construire des applications avec l'IA** — appels d'API, RAG, agents à écrire
  soi-même. C'est un sujet de spécialisation, et il suppose le socle.
- **Les questions d'éthique, de droit d'auteur et d'emploi.** Réelles, mais hors
  du champ d'un module technique.

## Où ranger votre travail

[../2.practice_dev_with_AI/](../2.practice_dev_with_AI/) accueille ce que ce
module vous fait produire :

```
2.practice_dev_with_AI/
├── mes_regles_IA.md      # votre cadrage de session       (exo 2.5.1)
├── journal_IA.md         # une semaine de demandes        (exo 2.5.2)
├── mon_protocole.md      # vos règles personnelles        (exo 5.5.5)
└── ...                   # vos explications, vos mesures
```

Un `CLAUDE.md` à la racine du dépôt — ou l'équivalent de votre outil — porte vos
règles permanentes pour les agents. Voir [leçon 4](lessons/04_travailler_avec_un_agent.md).

## Ce que ce module prépare

| Notion vue ici | Où elle resservira |
|---|---|
| Vérifier par la machine | Tout le parcours, et le métier |
| Lire un diff sans complaisance | [2.Learn_linux](../../2.Learn_linux/), et toute revue de code |
| Découper une tâche en unités vérifiables | Les projets, et le travail en équipe |
| Le test du dépôt fermé | Chaque module du socle |
| Savoir ce qu'on doit savoir de mémoire | Les entretiens, et les décisions de conception |
| Reconnaître un travail plausible mais faux | La revue de code, sur du code humain comme généré |

---

Le socle reste la priorité : [0.Discover_IT](../../0.Discover_IT/) →
[1.Learn_C](../../1.Learn_C/) → [2.Learn_linux](../../2.Learn_linux/) →
[3.projects_to_learn_algo](../../3.projects_to_learn_algo/).

Ce module amplifie ce que vous savez déjà. Il ne le remplace pas — c'est
précisément sa thèse.
