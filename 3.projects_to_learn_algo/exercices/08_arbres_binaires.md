# Exercices — Arbres binaires de recherche

Support : [08_arbres_binaires.md](../lessons/08_arbres_binaires.md)

**Compiler et lancer :**

```sh
gcc -Wall -Wextra -g codes/nom.c -o codes/nom && ./codes/nom
```

Vérifie l'absence de fuite à chaque exercice qui alloue.

---

# Niveau 1 — Construire et parcourir

## 1.1 — Construire à la main

Sans fonction d'insertion : construis l'arbre de la leçon avec des `malloc` et
des affectations directes.

```
                50
              /    \
            30      70
           /  \    /  \
         20   40  60   80
```

Puis libère-le correctement.

## 1.2 — Insertion

`Noeud *inserer(Noeud *racine, int valeur)`.

Construis le même arbre en insérant `50, 30, 70, 20, 40, 60, 80`.

## 1.3 — Les trois parcours récursifs

Infixe, préfixe, suffixe. Vérifie :

```
infixe  : 20 30 40 50 60 70 80
préfixe : 50 30 20 40 70 60 80
suffixe : 20 40 30 60 80 70 50
```

Si ton infixe n'est pas trié, ton insertion est fausse.

## 1.4 — Le parcours en largeur

Avec une file. `50 30 70 20 40 60 80`.

## 1.5 — Par niveau, une ligne chacun

```
50
30 70
20 40 60 80
```

*Indice :* note la taille de la file avant de traiter chaque niveau.

## 1.6 — Affichage en arbre

Affiche l'arbre couché, avec indentation :

```
        80
    70
        60
50
        40
    30
        20
```

*Indice :* parcours infixe **inversé** (droite, nœud, gauche), avec une
indentation proportionnelle à la profondeur.

C'est l'outil de débogage le plus utile de toute la leçon. Écris-le tôt.

## 1.7 — Libérer

Parcours suffixe. Puis essaie en préfixe et observe le rapport de fuite : tous
les nœuds sauf la racine.

Explique pourquoi.

---

# Niveau 2 — Interroger l'arbre

## 2.1 — Rechercher

Récursive, puis itérative. Ajoute un compteur de comparaisons.

## 2.2 — Compter les nœuds

`1 + compter(gauche) + compter(droite)`.

## 2.3 — Compter les feuilles

Un nœud sans enfant. Attention au cas de l'arbre vide.

## 2.4 — Hauteur

`1 + max(hauteur(gauche), hauteur(droite))`, avec `-1` pour l'arbre vide.

Pourquoi `-1` et pas `0` ? Vérifie avec un arbre à un seul nœud : sa hauteur doit
être `0`.

## 2.5 — Minimum et maximum

Le minimum est tout à gauche, le maximum tout à droite. Complexité ?

## 2.6 — Somme et moyenne

Des valeurs de tous les nœuds.

## 2.7 — Compter les nœuds à un niveau donné

`combien_au_niveau(racine, k)`.

Vérifie : niveau 0 → 1, niveau 1 → 2, niveau 2 → 4 sur un arbre complet.

## 2.8 — Vérifier que c'est bien un ABR

Attention : ce code est **faux**.

```c
bool est_abr_faux(const Noeud *r) {
    if (r == NULL) return true;
    if (r->gauche && r->gauche->valeur > r->valeur) return false;
    if (r->droite && r->droite->valeur < r->valeur) return false;
    return est_abr_faux(r->gauche) && est_abr_faux(r->droite);
}
```

Construis l'arbre contre-exemple de la leçon (avec le `60` mal placé) et
montre que cette fonction le valide à tort.

Écris ensuite la version correcte. Deux approches :

- passer un intervalle `[min, max]` en descendant ;
- faire un parcours infixe et vérifier que la suite est croissante.

Écris les deux.

---

# Niveau 3 — Modifier l'arbre

## 3.1 — Suppression, cas 1 et 2

Feuille, et nœud à un seul enfant. Teste chaque cas séparément, en affichant
l'arbre avant et après.

## 3.2 — Suppression, cas 3

Deux enfants, avec le successeur infixe.

Teste en supprimant la racine `50`. Elle doit être remplacée par `60`.

Vérifie que l'infixe reste trié après chaque suppression.

## 3.3 — Le prédécesseur plutôt que le successeur

On peut aussi remplacer par le **maximum du sous-arbre gauche**.

Implémente cette variante. Le résultat est un arbre différent, mais tout aussi
valide. Vérifie-le.

## 3.4 — Supprimer tout

Supprime les 7 valeurs une par une, dans un ordre aléatoire, en vérifiant après
chacune que l'arbre reste un ABR valide (avec ta fonction du 2.8).

Aucune fuite à la fin.

## 3.5 — Copier un arbre

`Noeud *copier(const Noeud *racine)` — une copie profonde.

Vérifie que modifier la copie ne change pas l'original.

Quel parcours utilises-tu ? Pourquoi ?

## 3.6 — Comparer deux arbres

`bool identiques(const Noeud *a, const Noeud *b)` — même structure **et** mêmes
valeurs.

Attention : deux arbres peuvent contenir les mêmes valeurs sans être identiques.
Construis un contre-exemple.

## 3.7 — Miroir

Échange récursivement les sous-arbres gauche et droit.

Le résultat est-il encore un ABR ? Que devient le parcours infixe ?

---

# Niveau 4 — L'équilibrage

C'est le niveau qui compte.

## 4.1 — Provoquer la dégénérescence

Insère `1` à `100` dans l'ordre croissant. Affiche la hauteur.

Tu dois obtenir **99**. Affiche aussi l'arbre (exercice 1.6) sur les 10 premiers
pour visualiser.

## 4.2 — Mélanger

Insère les mêmes 100 valeurs, mais mélangées (algorithme de Fisher-Yates).

Hauteur attendue : entre 10 et 14.

Répète 100 fois avec des mélanges différents et affiche la hauteur moyenne.
Compare à `log₂(100) ≈ 6,6`.

Pourquoi la moyenne est-elle nettement plus grande que `log₂(n)` ?

## 4.3 — Le coût de la dégénérescence

Sur les deux arbres du 4.1 et 4.2, mesure le nombre moyen de comparaisons pour
1 000 recherches aléatoires.

Note le rapport. C'est le prix de l'équilibrage.

## 4.4 — Le facteur d'équilibre

Écris `int facteur_equilibre(const Noeud *n)` :
`hauteur(gauche) - hauteur(droite)`.

Affiche-le pour chaque nœud d'un arbre dégénéré, puis d'un arbre équilibré.

Un arbre AVL garantit que ce facteur reste dans `{-1, 0, 1}` partout. Vérifie
lesquels de tes arbres respectent cette condition.

## 4.5 — Équilibrer par reconstruction

Une méthode simple, quoique brutale :

1. Parcours infixe → un tableau trié.
2. Reconstruis en prenant le milieu comme racine, récursivement.

Applique-la à l'arbre dégénéré du 4.1. La hauteur doit tomber de 99 à 6.

Complexité de cette opération ? Pourquoi ne peut-on pas l'appeler après chaque
insertion ?

## 4.6 — Les rotations

Implémente `rotation_droite` et `rotation_gauche`.

Vérifie après chaque rotation : le parcours infixe est **inchangé**, la hauteur a
diminué (ou pas, selon les cas).

C'est la brique élémentaire des arbres AVL. L'implémentation complète d'un AVL
dépasse ce cours, mais les rotations valent d'être écrites une fois.

---

# Niveau 5 — Applications

## 5.1 — Compter des mots

Lis un fichier texte. Pour chaque mot, insère-le dans un ABR ou incrémente son
compteur s'il existe.

Affiche les mots par ordre alphabétique avec leur fréquence.

*Note :* l'ordre alphabétique sort gratuitement du parcours infixe. C'est ce
qu'une table de hachage ne sait pas faire.

## 5.2 — Un dictionnaire

Charge `/usr/share/dict/words` dans un ABR. Affiche la hauteur.

**Attends-toi à un crash ou à une hauteur énorme** : le fichier est déjà trié
alphabétiquement.

Corrige en mélangeant les mots avant insertion. Compare les hauteurs et les
temps de recherche.

C'est la démonstration en conditions réelles du problème d'équilibrage.

## 5.3 — Recherche par intervalle

`afficher_intervalle(racine, a, b)` — toutes les valeurs entre `a` et `b`, dans
l'ordre.

*Optimisation :* n'explore pas les sous-arbres qui ne peuvent rien contenir de
pertinent. Si `racine->valeur < a`, inutile de descendre à gauche.

Compte les nœuds visités avec et sans cette optimisation.

Cette opération est **impossible** avec une table de hachage. C'est le critère de
choix entre les deux structures.

## 5.4 — Le k-ième plus petit élément

Sans parcourir tout l'arbre.

*Indice :* stocke dans chaque nœud la taille de son sous-arbre. La recherche
devient O(h).

Ce champ doit être maintenu à l'insertion et à la suppression. C'est le coût.

## 5.5 — Sérialiser et désérialiser

Écris l'arbre dans un fichier, puis reconstruis-le à l'identique.

*Question :* le parcours infixe suffit-il ? Essaie, et vois ce que tu obtiens en
réinsérant les valeurs dans cet ordre.

Quel parcours faut-il utiliser ? Pourquoi ?

---

# Auto-évaluation

- [ ] Quelle est la propriété d'un ABR, énoncée précisément ?
- [ ] Pourquoi vérifier seulement parent-enfant ne suffit-il pas ?
- [ ] Pourquoi le parcours infixe donne-t-il les valeurs triées ?
- [ ] Pourquoi le parcours suffixe est-il le seul correct pour libérer ?
- [ ] Que se passe-t-il si on insère des données déjà triées ?
- [ ] Pourquoi remplace-t-on par le successeur infixe en cas de deux enfants ?
- [ ] Quand préférer un arbre à une table de hachage ?

---

# Solutions

Aucune n'est donnée.

Trois recours :

1. **L'affichage en arbre de l'exercice 1.6.** Écris-le en premier, utilise-le
   partout. Sur un arbre, un bug se voit.
2. **Le parcours infixe comme test.** S'il n'est pas trié, l'arbre est cassé.
   Appelle-le après chaque modification.
3. **Dessiner les trois cas de suppression** sur papier avant de coder.
