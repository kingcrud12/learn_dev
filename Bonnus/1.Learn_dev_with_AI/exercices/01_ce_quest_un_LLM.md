# Exercices — Ce qu'est un modèle de langage

Support : [01_ce_quest_un_LLM.md](../lessons/01_ce_quest_un_LLM.md)

Ces exercices demandent **un terminal et un accès à un modèle de langage**. Ils
ne demandent aucune programmation.

Le but n'est pas de piéger le modèle. C'est de **voir le mécanisme** décrit dans
la leçon, pour ne plus jamais avoir à le croire sur parole.

**Une règle pour tout ce module :** quand un exercice demande de vérifier, la
vérification se fait dans le terminal. Une réponse de modèle vérifiée par un
autre modèle n'est pas vérifiée.

Aucune commande de ces exercices ne modifie votre machine.

---

# Niveau 1 — Voir les tokens

## 1.1 — Compter des lettres

Demandez au modèle combien de fois la lettre `s` apparaît dans
`assassinassions`. Notez sa réponse.

Comptez à la main. Puis :

```sh
echo -n "assassinassions" | grep -o "s" | wc -l
```

Recommencez avec `r` dans `perpendiculairement`, et `e` dans
`événementiellement`.

Sur combien d'essais s'est-il trompé ? Expliquez la cause en une phrase, en
utilisant le mot « token ».

## 1.2 — Faire calculer la machine

Redemandez le comptage de `1.1`, mais en exigeant cette fois une commande shell
que vous exécuterez vous-même.

Est-ce fiable ? Pourquoi la seconde méthode n'a-t-elle pas la faiblesse de la
première ?

Formulez la règle générale qui en découle, en une phrase.

## 1.3 — L'arithmétique

Sans autoriser de script, demandez :

- `4871 × 3926`
- `2^17`
- `987654321 / 12345`

Vérifiez :

```sh
echo "4871 * 3926" | bc
echo "2^17" | bc
echo "scale=4; 987654321 / 12345" | bc
```

Les réponses fausses le sont-elles de beaucoup, ou de peu ? Qu'est-ce que cela
dit sur la difficulté à les repérer à l'œil ?

## 1.4 — L'inversion de chaîne

Demandez d'inverser `anticonstitutionnellement` caractère par caractère, sans
script.

```sh
echo -n "anticonstitutionnellement" | rev
```

Comparez lettre par lettre. Où sont les erreurs — au début, à la fin, au milieu ?

## 1.5 — Découper en tokens

Demandez-lui de découper ces mots comme il pense qu'ils le sont en tokens :
`malloc`, `printf`, `anticonstitutionnellement`, `0xFF`, `café`.

Sa réponse est une estimation, pas une observation — il n'a pas accès à son
propre découpage. Que vaut-elle ?

Cherchez « tokenizer » ou « tokenisation LLM » et trouvez un outil en ligne qui
affiche le vrai découpage. Comparez.

---

# Niveau 2 — La date de coupure et les hallucinations

## 2.1 — La date de coupure

Demandez sa date de coupure de connaissances. Notez-la.

Demandez ensuite la dernière version stable de `gcc`, de `python3`, et du noyau
Linux. Vérifiez ce que vous avez :

```sh
gcc --version
python3 --version
uname -r
```

Et la version réelle actuelle, dans la documentation officielle du projet.

Que valent ses trois réponses ? Formulez la règle sur les questions de version.

## 2.2 — Provoquer une hallucination d'API

Demandez : « quelle fonction de la bibliothèque standard C inverse une chaîne en
place ? »

Il n'existe pas de telle fonction dans la norme C. Notez ce qu'il répond.

Vérifiez :

```sh
man 3 strrev
```

Puis écrivez trois lignes qui l'utilisent, et compilez :

```sh
printf '#include <string.h>\nint main(void){char s[]="abc"; strrev(s); return 0;}\n' > /tmp/t.c
gcc -Wall /tmp/t.c -o /tmp/t
```

Que dit `gcc` ? Vous venez d'utiliser ce que la leçon 03 appellera le détecteur
d'hallucinations.

## 2.3 — Le paramètre plausible

Demandez la signature exacte de `fopen`, `strtol` et `snprintf`. Notez-les.

Vérifiez chacune :

```sh
man 3 fopen
man 3 strtol
man 3 snprintf
```

Y a-t-il des écarts ? Sur quelle partie de la signature — le nom, le nombre
d'arguments, les types, l'ordre ?

## 2.4 — Le paquet inventé

Demandez le nom d'une bibliothèque C qui fait une chose très spécifique et
inhabituelle — par exemple « manipuler des dates du calendrier républicain
français ».

Notez le nom proposé. Cherchez-le. Existe-t-il ?

Pourquoi le modèle a-t-il proposé un nom plutôt que de dire qu'il n'en connaît
pas ? Répondez avec le mécanisme de la leçon.

## 2.5 — Le tableau des hallucinations

Sur les exercices 2.2 à 2.4, remplissez :

| Question | Réponse du modèle | Réalité | Temps pour vérifier |
|---|---|---|---|
| | | | |

La dernière colonne est celle qui compte. Quelle conclusion pratique en tirez-vous ?

---

# Niveau 3 — Le contexte

## 3.1 — La même question, deux contextes

Prenez un programme C de votre module [1.Learn_C](../../../1.Learn_C/) et
introduisez-y un bug volontaire (un `free` en trop, un accès hors bornes, un
`scanf` sans `&`).

**Conversation A** — demandez seulement : « mon programme C plante, pourquoi ? »

**Conversation B** — nouvelle conversation. Donnez le code complet, la commande
de compilation, la sortie d'erreur exacte, et le comportement observé.

Comparez les deux réponses. Laquelle est un cours ? Laquelle est un diagnostic ?

Vous n'avez pas changé de modèle. Qu'avez-vous changé ?

## 3.2 — Le contexte manquant invisible

Dans une nouvelle conversation, demandez : « pourquoi mon code ne compile
pas ? » — sans rien montrer.

Le modèle va probablement vous demander le code. S'il répond quand même, que
répond-il ?

Notez : dans quelle mesure la qualité de sa réponse dépendait-elle de vous, et
non de lui ?

## 3.3 — La conversation qui se pollue

Ouvrez une conversation sur une notion que vous connaissez bien. Faites-la
dériver volontairement : posez cinq questions hors sujet, insistez sur une
mauvaise piste, contredisez-le deux fois.

Puis reposez votre question initiale, dans la **même** conversation.

Ouvrez une conversation neuve et posez la même question proprement.

Comparez. Que vous apprend cela sur la bonne réaction quand une conversation
part de travers ?

## 3.4 — La mémoire entre sessions

Dites-lui un fait arbitraire sur vous (« mon exercice s'appelle `tri_fusion.c` »).
Fermez la conversation. Ouvrez-en une nouvelle et demandez comment s'appelle
votre exercice.

Selon l'outil, il s'en souviendra ou non. S'il s'en souvient, cherchez pourquoi :
quel mécanisme l'a mis dans le contexte ? Est-ce le modèle qui se souvient, ou
l'outil qui réinjecte ?

## 3.5 — La contrainte oubliée

Ouvrez une conversation en posant une règle stricte : « dans toute cette
conversation, ne me donne jamais de code, seulement des explications. »

Tenez une conversation longue — vingt échanges au moins, sur un sujet technique.
Puis demandez quelque chose qui appelle naturellement du code.

Respecte-t-il encore la règle ? Sinon, à partir de quel moment l'a-t-il perdue ?
Quelle pratique cela vous impose-t-il sur les longues sessions ?

---

# Niveau 4 — Raisonner sur le mécanisme

Des questions de raisonnement. Répondez **par écrit**, en quelques phrases. Elles
n'ont pas de réponse unique — c'est le raisonnement qui est l'exercice.

## 4.1 — Pourquoi il ne dit pas « je ne sais pas »

Vous avez vu le comportement en 2.4. Expliquez-le avec le mécanisme du
pré-entraînement.

Puis : si vous deviez entraîner un modèle qui admet davantage son ignorance,
qu'est-ce qu'il faudrait changer dans ce qu'il apprend ?

## 4.2 — La confiance constante

Un compilateur qui échoue affiche une erreur. Un modèle qui « échoue » produit
une réponse de la même forme qu'une réponse juste.

Pourquoi ? Est-ce un défaut réparable, ou une conséquence du mécanisme ?

Quelle conséquence pratique cela a-t-il sur votre façon de travailler ?

## 4.3 — La structure sans plan

La leçon affirme qu'il n'y a pas d'étape de planification : chaque token est
prédit à partir de ce qui précède.

Comment expliquez-vous alors qu'il produise des réponses bien structurées, avec
une introduction et une conclusion cohérentes ?

## 4.4 — Le profil de compétence inégal

Un modèle explique un arbre rouge-noir mieux que beaucoup de tutoriels, et se
trompe sur le nom d'un paramètre de `fopen`.

Ce n'est pas incohérent. Expliquez pourquoi, avec le critère de la leçon.

Puis : donnez trois autres exemples de ce contraste, tirés de ce dépôt.

## 4.5 — Pourquoi un bon prompt marche

La leçon affirme qu'un bon prompt ne rend pas le modèle plus intelligent, mais
rend la bonne réponse **plus probable**.

Reformulez cette phrase avec vos mots, en utilisant le mécanisme de prédiction.

Puis : que cela dit-il sur la responsabilité de la qualité d'une réponse ?

## 4.6 — La limite du contexte

Le modèle relit toute la conversation à chaque tour, et cette réserve est finie.

Trois conséquences pratiques sont énoncées dans la leçon. Retrouvez-les sans
relire, puis vérifiez.

Ajoutez-en une quatrième que la leçon ne mentionne pas.

## 4.7 — Le compilateur comme arbitre

Vous avez utilisé `gcc` en 2.2 pour trancher une hallucination.

Quelles catégories d'erreurs du modèle le compilateur détecte-t-il ? Lesquelles
lui échappent complètement ?

La seconde liste est la plus importante. Pourquoi ?

---

# Niveau 5 — Aller plus loin

## 5.1 — Deux modèles sur la même question

Posez la même question technique précise à deux modèles différents (deux outils,
ou deux versions).

Où divergent-ils ? Sur les faits vérifiables, ou sur les jugements ?

En quoi la divergence est-elle un signal utile ? Que signale-t-elle exactement ?

## 5.2 — La température

Cherchez ce qu'est le paramètre de « température » dans un modèle de langage.

Reliez-le au mécanisme de prédiction : que change-t-il concrètement dans le choix
du token suivant ?

Pour du code, faut-il une température haute ou basse ? Justifiez.

## 5.3 — Estimer un coût en tokens

Une règle approximative : un token vaut environ 4 caractères en anglais, un peu
moins en français.

Estimez le nombre de tokens de :

- cette page d'exercices
- un de vos fichiers `.c`
- le dépôt entier

```sh
wc -c exercices/01_ce_quest_un_LLM.md
find /Users/Hugotestas/learn_dev -name "*.md" -o -name "*.c" | xargs wc -c | tail -1
```

Divisez par 4. Le dépôt entier tiendrait-il dans une fenêtre de contexte de
200 000 tokens ? Et de 1 million ?

## 5.4 — Le RAG

Cherchez ce qu'est le **RAG** (*Retrieval-Augmented Generation*).

En quoi répond-il aux deux limites vues dans cette leçon — la date de coupure, et
« ce qui n'est pas dans le contexte n'existe pas » ?

Quelle limite ne résout-il **pas** ?

## 5.5 — Prédire une faiblesse

Sans tester, prédisez si le modèle réussira ou échouera sur ces cinq tâches, et
**pourquoi** :

- donner le 47ᵉ nombre premier
- expliquer la différence entre `++i` et `i++`
- dire combien de lignes fait un fichier que vous collez
- expliquer pourquoi un `malloc` sans `free` est un problème
- donner le numéro de port par défaut de PostgreSQL

Testez ensuite. Combien de prédictions correctes ?

Un score élevé signifie que vous avez intégré le mécanisme — c'est l'objectif de
cette leçon.

---

# Auto-évaluation

Vous pouvez passer à la leçon 02 si vous répondez sans hésiter :

- [ ] Que fait un modèle de langage, en une phrase ?
- [ ] Qu'est-ce qu'un token, et quelles sont ses trois conséquences pratiques ?
- [ ] Pourquoi l'arithmétique de tête est-elle fragile ? Que faire à la place ?
- [ ] Qu'est-ce qu'une hallucination, et pourquoi n'est-ce pas une panne ?
- [ ] Pourquoi un modèle préfère-t-il répondre plutôt que de s'abstenir ?
- [ ] Qu'est-ce que la fenêtre de contexte ? Le modèle a-t-il de la mémoire ?
- [ ] Que faire quand une conversation part de travers ? Pourquoi ?
- [ ] Quel est le critère qui prédit la fiabilité d'une réponse ?
- [ ] Que fait un bon prompt, exactement ?
- [ ] Quelles erreurs le compilateur détecte-t-il ? Lesquelles lui échappent ?

---

# Solutions

Aucune n'est donnée — volontairement, et particulièrement ici.

Trois recours dans l'ordre :

1. **La machine.** `bc`, `wc`, `grep -c`, `man`, `gcc`. La plupart des exercices
   des niveaux 1 à 3 se tranchent dans le terminal en quelques secondes.
2. **Relire la leçon.** Chaque exercice correspond à une section précise.
3. **Le modèle lui-même** — mais en prompt socratique, pas en prompt de solution.
   Voir la leçon [02](../lessons/02_prompter_pour_apprendre.md).

Une remarque propre à ce module : **ne demandez pas au modèle de corriger vos
réponses du niveau 4.** Ce sont des questions sur son propre fonctionnement, et
il n'a aucun accès privilégié à ce fonctionnement — il en a lu des descriptions,
comme vous. Sur ces questions, sa réponse n'a pas plus d'autorité que la vôtre.
