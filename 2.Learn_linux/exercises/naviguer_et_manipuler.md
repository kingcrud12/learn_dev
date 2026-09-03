# Exercices — Naviguer et manipuler

Support : [../lessons/02_naviguer_et_manipuler.md](../lessons/02_naviguer_et_manipuler.md)

Vingt-deux exercices, du plus simple au plus exigeant.

**Avant de commencer — un terrain de jeu jetable.** Tous les exercices destructifs
se font ici, jamais dans `learn_dev/` :

```sh
mkdir -p ~/bac_a_sable
cd ~/bac_a_sable
```

**Règle de sécurité :** avant tout `rm` avec un joker, lance la même commande avec
`ls` et regarde ce qu'elle attrape.

---

# Niveau 1 — Se repérer

## 1.1 — Où suis-je

Affiche ton répertoire courant. Puis va dans ton dossier personnel, réaffiche-le,
et reviens au précédent en une seule commande.

*Trois commandes en tout.*

## 1.2 — Regarder autour

Depuis `~/learn_dev`, liste :

- le contenu simple
- le contenu avec les détails (droits, taille, date)
- **tout**, y compris les fichiers cachés
- le contenu de `learn_C/` sans t'y déplacer

## 1.3 — Décoder une ligne

Lance `ls -l learn_C/lessons/`. Pour une des lignes, identifie : le type, les droits
du propriétaire, la taille, la date.

Le premier caractère est-il `-` ou `d` ? Pourquoi ?

## 1.4 — Tailles lisibles

Affiche le contenu de `learn_C/lessons/` avec des tailles en Ko plutôt qu'en octets,
triées par date de modification, la plus récente en premier.

*Indice :* deux options à combiner avec `-l`.

## 1.5 — Naviguer sans se tromper

Depuis `~/learn_dev/learn_C/codes/`, atteins `~/learn_dev/learn_linux/lessons/`
**en une seule commande**, avec un chemin relatif (donc sans `~` ni `/` initial).

---

# Niveau 2 — Lire

## 2.1 — Quatre façons de lire

Sur `learn_C/codes/hello_world.c`, affiche : le fichier entier, ses 3 premières
lignes, ses 2 dernières, et le tout avec les numéros de ligne.

## 2.2 — Compter

Combien de lignes contient `learn_C/lessons/03_variables_et_types.md` ?

Combien de mots ? Combien d'octets ?

*Indice :* `wc` accepte trois options différentes.

## 2.3 — Naviguer dans un long fichier

Ouvre `learn_C/lessons/02_compiler_executer_debugger.md` avec `less`.

Cherche le mot `fork`, passe à l'occurrence suivante, va à la fin du fichier,
reviens au début, sors.

*Touches :* `/`, `n`, `G`, `g`, `q`.

## 2.4 — Le manuel

Ouvre le manuel de `ls`. Trouve ce que fait l'option `-S`. Sors.

Puis vérifie ta réponse en l'utilisant sur `learn_C/lessons/`.

---

# Niveau 3 — Créer et organiser

À faire dans `~/bac_a_sable`.

## 3.1 — Une arborescence d'un coup

Crée en **une seule commande** :

```
projet/
├── src/
├── include/
└── docs/api/
```

*Indice :* une option de `mkdir` évite de répéter la commande.

## 3.2 — Des fichiers vides

Dans `projet/src/`, crée `main.c`, `utils.c` et `math.c` — **en une seule commande**.

## 3.3 — Copier

Copie `main.c` sous le nom `main_sauvegarde.c`, puis copie tout le dossier `src/`
vers `src_backup/`.

*Piège :* copier un dossier demande une option.

## 3.4 — Renommer

Renomme `math.c` en `calcul.c`. Puis déplace-le dans `include/`.

Quelle commande fait les deux ? Pourquoi une seule suffit ?

## 3.5 — Corriger une vraie coquille

Dans `~/learn_dev/learn_C/codes/`, le fichier `prensenting_myself.c` contient une
faute de frappe. Renomme-le en `presenting_myself.c`.

*Celui-ci n'est pas dans le bac à sable — vérifie avec `ls` avant et après.*

---

# Niveau 4 — Les jokers

## 4.1 — Sélectionner

Dans `~/learn_dev`, liste tous les fichiers `.md` de `learn_C/lessons/`.

Puis tous les fichiers commençant par `0`.

## 4.2 — Vérifier avant de détruire

Dans `~/bac_a_sable/projet/`, crée quelques fichiers `.o` :

```sh
touch src/a.o src/b.o src/main.o
```

**Avant de les supprimer**, écris la commande `ls` qui montre exactement ce que
`rm src/*.o` détruirait. Vérifie, puis supprime.

## 4.3 — Un seul caractère

Crée `exo1.c`, `exo2.c`, `exo3.c` et `exo10.c`.

Trouve le motif qui attrape les trois premiers **mais pas** `exo10.c`.

## 4.4 — Un intervalle

Avec les mêmes fichiers, écris un motif qui n'attrape que `exo1.c` et `exo2.c`.

*Indice :* les crochets acceptent un intervalle.

## 4.5 — Le piège de l'espace

Explique la différence entre ces deux commandes — **sans les exécuter** :

```sh
rm -rf ./mon_dossier
rm -rf . /mon_dossier
```

Que détruit la seconde ? Pourquoi est-ce la faute de frappe la plus dangereuse
du terminal ?

---

# Niveau 5 — Permissions

## 5.1 — Lire des droits

Compare `ls -l` sur `learn_C/codes/hello_world.c` et sur l'exécutable
`learn_C/codes/hello_world` (compile-le d'abord s'il n'existe pas).

Quelle lettre apparaît sur l'un et pas sur l'autre ? Qui l'a posée ?

## 5.2 — Casser puis réparer

Sur une **copie** de ton exécutable dans le bac à sable :

```sh
cp ~/learn_dev/learn_C/codes/hello_world ~/bac_a_sable/
cd ~/bac_a_sable
```

Retire le droit d'exécution, tente de lancer le programme, observe le message.
Puis remets le droit et relance.

## 5.3 — Notation numérique

Traduis en notation `rwx` : `755`, `644`, `600`, `777`.

Lequel donnerais-tu à un script ? À un fichier de mot de passe ? À un fichier de
données partagé en lecture ?

---

# Niveau 6 — Combiner

## 6.1 — Chercher des fichiers

Dans `~/learn_dev`, trouve tous les fichiers `.md`, où qu'ils soient.

Puis compte-les.

*Indice :* `find` pour chercher, un tube et `wc -l` pour compter.

## 6.2 — Les plus récents

Trouve tous les fichiers de `~/learn_dev` modifiés depuis moins de 24 heures.

*Indice :* `find` accepte un critère de temps.

## 6.3 — Le plus gros

Quel est le fichier le plus volumineux de `learn_C/lessons/` ?

Trouve **deux** façons de le déterminer.

## 6.4 — Taille d'un dossier

Quelle place occupe `~/learn_dev` en tout ? Et chacun de ses sous-dossiers ?

*Indice :* `du`, avec deux options différentes.

## 6.5 — Nettoyer un projet

Dans le bac à sable, crée un désordre réaliste :

```sh
cd ~/bac_a_sable/projet
touch src/{a,b,c}.o src/prog docs/brouillon.tmp .DS_Store
```

Écris **une seule commande** qui supprime tous les `.o` et tous les `.tmp`, où
qu'ils soient dans `projet/`.

*Vérifie avec `find` avant de supprimer.*

## 6.6 — Reproduire une structure

Sans copier-coller depuis ce fichier, recrée de mémoire l'arborescence de
`~/learn_dev` (les dossiers seulement) dans `~/bac_a_sable/copie/`.

Vérifie avec `find ~/bac_a_sable/copie -type d`.

---

# Défi final

## 7.1 — Le rangement complet

Dans le bac à sable, pars de ce chantier :

```sh
cd ~/bac_a_sable
rm -rf chantier && mkdir chantier && cd chantier
touch main.c utils.c math.c main.o utils.o notes.txt README.md TODO.txt prog
```

Range-le pour obtenir :

```
chantier/
├── src/        les .c
├── docs/       les .txt et .md
├── build/      les .o et l'exécutable prog
```

**Contraintes :** pas de `mv` fichier par fichier — utilise les jokers. Vérifie le
résultat avec `find . -type f`.

## 7.2 — Le rapport

Écris les commandes qui répondent à ces questions sur `~/learn_dev` :

1. Combien de fichiers en tout ?
2. Combien de dossiers ?
3. Combien de fichiers `.md` ? Combien de `.c` ?
4. Quelle est la taille totale du projet ?
5. Quel fichier a été modifié en dernier ?

---

# Auto-évaluation

Tu peux passer à la suite si tu réponds sans hésiter :

- [ ] Quelle est la différence entre `/temp` et `temp` ?
- [ ] Pourquoi `./programme` et pas `programme` ?
- [ ] Que fait `cd -` ?
- [ ] Comment vérifier ce que `rm *.o` va détruire ?
- [ ] Que signifie le `x` dans `-rwxr-xr-x`, et qui l'a posé sur ton exécutable ?
- [ ] Que fait Ctrl+R ?
- [ ] Pourquoi `rm -rf . /dossier` est-il catastrophique ?

---

# Solutions

Non fournies — volontairement.

Trois recours :

1. **Relire la leçon** — chaque exercice correspond à une section.
2. **`man commande`** — la section EXAMPLES en bas est souvent la plus utile.
3. **Essayer dans le bac à sable.** C'est fait pour ça : rien n'y est précieux.

Un exercice qui résiste plus de vingt minutes mérite une question.

---

## Nettoyer après coup

```sh
rm -rf ~/bac_a_sable
```

*Vérifie le chemin avant d'appuyer sur Entrée.*
