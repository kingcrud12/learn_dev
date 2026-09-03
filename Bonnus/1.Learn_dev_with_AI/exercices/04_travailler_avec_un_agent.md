# Exercices — Travailler avec un agent de code

Support : [04_travailler_avec_un_agent.md](../lessons/04_travailler_avec_un_agent.md)

Ces exercices demandent **un agent de code** (Claude Code, ou équivalent) et un
dépôt Git propre.

**Prérequis absolu : savoir utiliser Git.** Commiter, lire un diff, annuler un
changement. Si le module [2.Learn_linux](../../../2.Learn_linux/) n'est pas fait,
faites-le d'abord — ce n'est pas un complément, c'est la condition de sécurité.

**La règle de tous ces exercices :**

```sh
git status                                  # avant
git add -A && git commit -m "avant exo"     # avant
# ... l'agent travaille ...
git diff HEAD                               # après, LU EN ENTIER
```

Plusieurs exercices demandent de laisser un agent modifier des fichiers. Aucun ne
le demande sur du travail non commité.

---

# Niveau 1 — Le réflexe Git

Sans agent. On installe le filet avant de sauter.

## 1.1 — Le cycle complet

Dans ce dépôt, cinq fois de suite :

```sh
git status
echo "test" >> /tmp/scratch.txt
git status
git diff
```

Puis apprenez à annuler. Modifiez un fichier réel du dépôt, constatez, annulez :

```sh
git diff
git checkout -- le_fichier
git status
```

Refaites-le jusqu'à ce que ce soit un réflexe et non une consultation.

## 1.2 — Lire un diff

Prenez un vrai commit de ce dépôt :

```sh
git log --oneline | head -10
git show <hash>
```

Lisez-le en entier. Répondez sans relire : quels fichiers ont changé ? Combien de
lignes ajoutées, supprimées ? Y a-t-il un changement auquel vous ne vous
attendiez pas ?

Cette lecture est la compétence centrale de cette leçon. Entraînez-la sur cinq
commits.

## 1.3 — Le stash

```sh
echo "// travail en cours" >> 1.Learn_C/codes/hello_world.c
git stash
git status
git stash pop
git status
git checkout -- 1.Learn_C/codes/hello_world.c
```

À quoi sert `stash` avant de lancer un agent ? En quoi diffère-t-il d'un commit ?

## 1.4 — Simuler le dégât

Sur un dépôt commité, modifiez délibérément trois fichiers différents, comme le
ferait un agent maladroit.

Annulez tout d'un coup :

```sh
git diff --stat
git checkout -- .
git status
```

Combien de temps a pris l'annulation ? C'est le coût réel d'une erreur d'agent
**quand le dépôt est propre**. Sans Git, quel aurait été ce coût ?

---

# Niveau 2 — L'agent en lecture seule

Le mode par défaut. Tous ces exercices se font **sans aucune écriture**.

## 2.1 — Diagnostiquer un segfault

Prenez un programme C de votre module 1 et introduisez un bug mémoire : accès
hors bornes, ou déréférencement de `NULL`.

Demandez à l'agent :

```
Compile ce programme avec -g, lance-le sous lldb, donne-moi la ligne
fautive et l'état des variables à ce moment. Explique ce que tu
observes. Ne corrige rien.
```

Notez **les commandes qu'il a réellement exécutées**. Relancez-les vous-même, à
la main, une par une.

Comparez : est-ce une hypothèse ou une observation ? Reliez à la zone « ne voit
pas ce dont il parle » de la leçon 03 — que devient-elle ?

## 2.2 — Diagnostiquer une fuite

Introduisez une fuite mémoire — un `malloc` sans `free`, dans une boucle.

```
Compile ce programme, puis lance le détecteur de fuites de mon
système (leaks sur macOS, valgrind sous Linux — vérifie lequel est
installé). Dis-moi combien d'octets fuient, sur quelle ligne
l'allocation a eu lieu, et pourquoi. Ne corrige pas.
```

Exécutez la commande vous-même :

```sh
leaks --atExit -- ./prog                  # macOS
valgrind --leak-check=full ./prog         # Linux
```

La sortie brute est illisible au début : faites-la vous expliquer ligne par ligne.

**Une observation à faire au passage :** si l'agent a proposé `valgrind` alors
que votre machine ne l'a pas, vous venez de voir une hallucination d'outil — la
même erreur que celle du fichier 01, sur un autre objet. Vérifiez toujours :

```sh
which valgrind leaks
```

Vous apprenez l'outil en le regardant faire. C'est le meilleur usage d'un agent
pour un apprenant — expliquez pourquoi en une phrase.

## 2.3 — Explorer le dépôt

```
Où est définie la structure de nœud dans ce dépôt, et quels fichiers
l'utilisent ?
```

Vérifiez ses réponses vous-même :

```sh
grep -rn "struct" 3.projects_to_learn_algo/ | head -20
```

Ses chemins sont-ils réels ? A-t-il manqué des occurrences ? En a-t-il inventé ?

## 2.4 — Se faire interroger sur son propre code

Prenez votre code le plus complexe dans ce dépôt.

```
Lis ce fichier. Pose-moi cinq questions sur mes choix de conception,
une à la fois. Cherche les points où je ne saurais pas justifier ce
que j'ai écrit.
```

Il ne peut pas inventer : il a le code sous les yeux.

Les questions auxquelles vous ne savez pas répondre désignent exactement ce qui
reste à comprendre **dans du code que vous avez écrit vous-même**. Notez-les.

## 2.5 — Le plan sans exécution

Choisissez une tâche réelle : réorganiser un dossier, ajouter une cible à un
Makefile, écrire un script de mesure.

```
Explique-moi exactement ce que tu ferais, fichier par fichier, sans
rien modifier. Je veux le plan, pas le résultat.
```

Lisez le plan. Auriez-vous fait pareil ? Y a-t-il un fichier auquel vous n'auriez
pas pensé — ou un auquel il ne fallait pas toucher ?

Le mode plan est ce qui rend visible l'intention avant l'action. Notez ce que
vous auriez laissé passer.

---

# Niveau 3 — L'agent en écriture, sur ce qui n'est pas l'exercice

Ici on autorise l'écriture — sur de l'outillage, jamais sur des exercices. Dépôt
commité avant chaque exercice.

## 3.1 — Un Makefile

```
Écris un Makefile pour 1.Learn_C/codes/ : compile chaque .c en un
binaire séparé avec -Wall -Wextra -g, avec une cible all et une
cible clean.
```

Puis, dans l'ordre :

```sh
git diff HEAD          # lire EN ENTIER avant tout
make
make clean
```

Faites-vous expliquer chaque ligne que vous ne comprenez pas. Une syntaxe de
Makefile est notoirement obscure : `$@`, `$<`, `%.o: %.c`.

Vous avez gagné du temps **et** appris `make`. Pourquoi est-ce la bonne
délégation ? Formulez-le avec le tableau de la leçon.

## 3.2 — Un générateur de données

```
Écris un programme C qui génère un fichier de N entiers aléatoires,
N passé en argument. Ne touche à aucun de mes fichiers existants.
```

Lisez le diff. Vérifiez : a-t-il touché **uniquement** au nouveau fichier ?

```sh
git diff --stat HEAD
```

Utilisez-le pour tester vos tris. L'infrastructure n'est pas ce que le module 3
enseigne — la lecture des résultats l'est.

## 3.3 — Un banc de mesure

```
Compile mes trois implémentations de tri, génère des tableaux de
1000, 10000 et 100000 éléments, mesure les temps, présente-moi un
tableau. Ne modifie aucun de mes tris.
```

Vérifiez d'abord qu'il n'a pas touché à vos tris :

```sh
git diff HEAD --stat
```

Puis lisez les résultats. Correspondent-ils aux complexités attendues ? Un
résultat qui contredit la théorie est plus intéressant qu'un résultat conforme —
cherchez pourquoi.

## 3.4 — La frontière

Demandez délibérément ce qui est interdit par vos règles :

```
Résous pour moi l'exercice de liste chaînée du module 3.
```

Que se passe-t-il ? S'il le fait :

```sh
git checkout -- .
```

Vos règles sont trop faibles. C'est l'objet de l'exercice 5.1 — resserrez-les et
refaites le test.

---

# Niveau 4 — Détecter le travail plausible

Les exercices les plus importants du fichier. Ils demandent de laisser l'agent
travailler en autonomie sur du jetable, dépôt commité.

## 4.1 — Le test arrangé

Écrivez une fonction avec un **bug réel** et un test qui échoue légitimement.

```sh
git add -A && git commit -m "bug volontaire + test qui echoue"
```

Demandez à l'agent, en autonomie : « fais passer ce test ».

**Lisez le diff avant son résumé** :

```sh
git diff HEAD
```

Répondez : a-t-il corrigé la fonction, ou arrangé le test ? A-t-il ajouté une
condition spéciale pour le cas qui échouait ?

## 4.2 — La même tâche, avec la contrainte

```sh
git checkout -- .
```

Refaites 4.1 avec :

```
Si tu n'arrives pas à faire passer ce test, dis-le et explique
pourquoi. Ne modifie pas le test, et ne contourne pas le problème.
```

Comparez les deux diffs. Qu'a changé la contrainte explicite ?

Pourquoi fallait-il l'énoncer ? Reliez au mécanisme de la leçon 01 — quelle est
la « continuation la plus probable » d'un texte où un problème est résolu ?

## 4.3 — Résumé contre diff

Sur une tâche d'écriture réelle, faites deux lectures dans cet ordre strict :

1. Lisez **le diff** en entier. Écrivez ce que l'agent a fait, selon vous.
2. Lisez **son résumé**.

Comparez vos deux comptes rendus. Y a-t-il un écart ? Dans quel sens — a-t-il
omis quelque chose, ou décrit quelque chose qu'il n'a pas fait ?

La leçon dit : le résumé est ce qu'il croit avoir fait, le diff est ce qu'il a
fait. Avez-vous constaté cet écart ?

## 4.4 — « Les tests passent »

Après une tâche où l'agent affirme que les tests passent, vérifiez vous-même :

```sh
make clean && make && ./tests
```

Passent-ils ? Combien ? Y en avait-il avant qui ne tournent plus ?

Formulez la règle : que vaut l'affirmation « les tests passent » ?

## 4.5 — Le contexte saturé

Donnez à l'agent une tâche large et vague, qui l'obligera à lire beaucoup de
fichiers — par exemple : « analyse la cohérence de tous les README de ce dépôt et
propose des corrections ».

Puis, à la fin de la conversation, rappelez une contrainte que vous aviez posée
au tout début. La respecte-t-il encore ?

Refaites la même tâche découpée en trois tâches petites et cadrées. Comparez la
qualité.

Quelle règle contre-intuitive de la leçon venez-vous de vérifier ?

---

# Niveau 5 — Construire votre pratique

## 5.1 — Vos règles permanentes

Créez un `CLAUDE.md` à la racine du dépôt (ou l'équivalent de votre outil).

Partez du cadrage de la leçon, adaptez-le à votre progression réelle. Il doit
définir :

- les dossiers en **lecture seule absolue** (vos exercices, `codes/`)
- ce qu'il peut lire, compiler, exécuter
- le comportement attendu quand vous êtes bloqué (indice, jamais solution)
- ce qu'il peut écrire (outillage) et à quelle condition

Testez-le avec 3.4. Itérez jusqu'à ce que le refus soit fiable.

## 5.2 — Le journal des diffs

Pendant une semaine, pour chaque tâche d'agent :

| Date | Tâche | Fichiers touchés (diff) | Attendus ? | Accepté / annulé |
|---|---|---|---|---|

Rangez-le dans [../../2.practice_dev_with_AI/](../../2.practice_dev_with_AI/).

Au bout d'une semaine : combien de fois a-t-il touché un fichier inattendu ?
Combien de fois avez-vous annulé ? Le taux d'annulation baisse-t-il à mesure que
vos tâches se resserrent ?

## 5.3 — Le découpage

Prenez une tâche que vous jugez trop grosse pour un agent.

Découpez-la en tâches **vérifiables individuellement** — chacune avec un critère
de succès observable par la machine.

Exécutez-les une par une, en lisant le diff entre chaque.

Combien de tâches ? Cette compétence de découpage est celle que la leçon nomme en
premier dans « la compétence réelle ». Pourquoi vient-elle avant les autres ?

## 5.4 — Apprendre lldb en le regardant

Demandez à l'agent de diagnostiquer trois bugs différents sous `lldb`, en
demandant chaque fois les commandes exactes.

Notez-les dans un fichier `mes_commandes_lldb.md`. Après trois diagnostics, vous
aurez l'essentiel : `break`, `run`, `bt`, `print`, `next`, `step`.

Refaites le quatrième diagnostic **vous-même**, sans agent, avec votre fiche.

## 5.5 — La tâche refaite à la main

Choisissez une tâche que l'agent a faite pour vous cette semaine, en écriture.

Annulez-la. Refaites-la vous-même, entièrement.

Combien de temps ? Qu'avez-vous appris que le diff ne vous avait pas appris ?

C'est le test qui distingue « j'ai lu le diff » de « je saurais l'écrire ». Les
deux ne sont pas la même chose, et seul le second compte pour la colonne de
gauche de la leçon 05.

---

# Auto-évaluation

- [ ] Qu'est-ce qui distingue un agent d'un chat ?
- [ ] Décrivez la boucle d'un agent, en cinq étapes.
- [ ] Pourquoi des tâches petites valent-elles mieux qu'une grande tâche vague ?
- [ ] Quelle est la discipline Git minimale autour d'une tâche d'agent ?
- [ ] Quelle est la frontière entre vos exercices et l'outillage ?
- [ ] Pourquoi le mode lecture seule est-il le défaut, et que perd-on avec lui ?
- [ ] Qu'est-ce que le travail plausible ? Citez trois formes.
- [ ] Quelle est la double défense contre le travail plausible ?
- [ ] Pourquoi le résumé de l'agent ne remplace-t-il pas le diff ?
- [ ] Citez les six compétences réelles de la leçon. Que supposent-elles toutes ?

---

# Solutions

Aucune n'est donnée.

Pour ce fichier, l'arbitre est **le diff** :

```sh
git diff HEAD              # ce qu'il a réellement fait
git diff --stat HEAD       # quels fichiers, combien de lignes
git checkout -- .          # annuler si c'est mauvais
```

Aucun corrigé ne peut vous dire ce que l'agent a fait sur **votre** dépôt. C'est
exactement la compétence visée : la source de vérité est le diff, pas une
explication — ni celle de l'agent, ni celle de ce fichier.

Les questions du niveau 4 se traitent en prompt d'inversion. Mais attention : sur
« l'agent a-t-il arrangé le test ? », **ne lui demandez pas.** Regardez le diff.
