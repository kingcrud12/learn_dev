# Exercices — Le développement mobile cross-platform

Leçon associée : [../lessons/01_cross_platform.md](../lessons/01_cross_platform.md)

Ces exercices ne sont pas surtout des exercices de code. Le cross-platform est un domaine
où **le choix technologique engage des années de travail**, et où une mauvaise décision
prise en une journée coûte des mois. Une bonne moitié des exercices ci-dessous te demande
donc de réfléchir et d'écrire un raisonnement, pas de taper du code.

Prends un fichier texte, ou un cahier. Écris tes réponses. Une réponse pensée dans ta tête
sans être formulée n'est pas une réponse : c'est une impression.

Les niveaux 3 et suivants supposent une installation d'outils. Si tu n'y es pas encore,
fais tout de même les niveaux 1 et 2 : ils sont les plus utiles à ce stade.

---

## Niveau 1 — Comprendre les architectures

### Exercice 1.1 — Redessiner les trois schémas

Sans regarder la leçon, redessine de mémoire les trois schémas ASCII d'architecture :
WebView, React Native, Flutter. Puis compare avec la leçon.

Pour chacun, réponds en une phrase à la question : **qui dessine le bouton que
l'utilisateur voit à l'écran ?**

### Exercice 1.2 — La question qui tranche

Formule, en une seule phrase, la question à poser pour classer n'importe quelle technologie
cross-platform. (Indice : elle est en section 2 de la leçon, et elle porte sur ce qui est
partagé.)

Applique-la ensuite à Flutter, React Native, Kotlin Multiplatform et Ionic.

### Exercice 1.3 — Identifier l'approche d'une application

Prends cinq applications installées sur ton téléphone. Pour chacune, essaie de deviner son
approche technique **avant** de chercher sur internet. Utilise ces indices :

- Le défilement a-t-il l'inertie native ou paraît-il « collant » ?
- Les boîtes de dialogue ressemblent-elles à celles du système ?
- Sur iOS, un appui long sur un texte propose-t-il la sélection habituelle ?
- Les animations sont-elles fluides quand tu changes d'onglet ?
- L'application a-t-elle exactement la même tête que sa version Android ?

Note ton hypothèse, puis vérifie. Beaucoup d'entreprises publient un article de blog
technique sur leur choix.

### Exercice 1.4 — Le rôle du pont

Explique avec tes mots, à quelqu'un qui n'y connaît rien, ce qu'était le « bridge » de
React Native et pourquoi il ralentissait les animations. Interdiction d'utiliser les mots
« sérialisation » et « asynchrone » : trouve une image du quotidien.

### Exercice 1.5 — Pourquoi une app Flutter pèse plus lourd

Une application Flutter minimale pèse environ 15 Mo, une application native environ 4 Mo.
D'où viennent les 11 Mo de différence ? Réponds en une phrase, à partir de l'architecture.

### Exercice 1.6 — JIT et AOT

Complète ce tableau de mémoire, puis explique pourquoi le hot reload n'existe **que** en
mode développement.

| | Quand la traduction en code machine a-t-elle lieu ? | Flutter l'utilise pour |
|---|---|---|
| JIT | | |
| AOT | | |

---

## Niveau 2 — Choisir la bonne techno sur des scénarios concrets

Pour chaque scénario : choisis une technologie, écris **trois arguments pour** et **un
argument contre** ton propre choix. L'argument contre est obligatoire — si tu n'en trouves
pas, c'est que tu n'as pas assez creusé.

### Exercice 2.1 — Une banque en ligne

Une banque régionale veut son application. Contraintes : sécurité maximale, lecture de
l'empreinte digitale et de la reconnaissance faciale, calcul de frais et de taux qui doit
être **rigoureusement identique** sur les deux plateformes (une divergence est un incident
réglementaire), équipe existante de quatre développeurs Android et trois développeurs iOS.

Question bonus : ici, la contrainte « identique des deux côtés » porte sur quelle couche ?
L'interface ou la logique ? Est-ce que ça oriente ton choix ?

### Exercice 2.2 — Un jeu de plateforme 2D

Un studio indépendant de trois personnes veut sortir un jeu de plateforme avec défilement,
physique et 60 images par seconde, sur Android et iOS.

Attention : la bonne réponse n'est probablement dans aucune des colonnes du tableau
comparatif de la leçon. Explique pourquoi.

### Exercice 2.3 — Un réseau social

Une startup lève des fonds et doit sortir un réseau social de partage de photos en quatre
mois. Fil d'actualité infini, envoi de vidéos, appareil photo intégré avec filtres,
notifications. L'équipe est composée de cinq développeurs web (React) et de personne
d'autre.

Question bonus : quelle partie de cette application risque le plus de te forcer à écrire du
code natif quand même ?

### Exercice 2.4 — Une application interne d'entreprise

Une entreprise de logistique veut une application pour ses 200 employés : consulter des
plannings, remplir des formulaires, scanner des codes-barres. Elle est distribuée en
interne, pas sur les stores publics. Budget serré, un seul développeur, délai de six
semaines.

Question bonus : le fait qu'elle ne passe pas par les stores publics change-t-il ton
raisonnement ? En quoi ?

### Exercice 2.5 — Une application santé connectée

Un fabricant de tensiomètres Bluetooth veut une application qui lit son appareil en
continu, enregistre les mesures et les synchronise avec Apple Santé et Google Fit.

### Exercice 2.6 — Le piège du scénario évident

Reprends un des cinq scénarios ci-dessus et défends **la technologie que tu as écartée**.
Trouve-lui les meilleurs arguments possibles. C'est un exercice inconfortable et c'est le
but : il t'apprend que ces choix sont rarement évidents.

### Exercice 2.7 — La grille de décision

À partir de tout ce qui précède, rédige ta propre grille de décision personnelle : une
liste ordonnée de cinq à sept questions à te poser, dans l'ordre, face à un nouveau projet.
Garde ce document, tu le reliras dans un an et tu verras comment ton avis a changé.

---

## Niveau 3 — Installer et configurer l'environnement

> **Prérequis :** ce niveau télécharge plusieurs gigaoctets. Prévois du temps, une bonne
> connexion et de l'espace disque. Vérifie d'abord avec `df -h /`.

### Exercice 3.1 — Inventaire de la machine

Avant d'installer quoi que ce soit, fais l'inventaire de ce que tu as déjà :

```sh
uname -m                  # architecture du processeur
sw_vers                   # version de macOS
echo $SHELL               # ton shell
which brew git python3    # ce qui est déjà installé
xcode-select -p           # Xcode est-il installé ?
```

Note les résultats. Que signifie la sortie de `uname -m` sur ta machine, et pourquoi
est-ce important quand tu choisiras une image d'émulateur Android ?

### Exercice 3.2 — Installer les outils de base

Installe Homebrew s'il n'est pas là, puis les outils en ligne de commande Xcode.

```sh
xcode-select --install
```

Ensuite : dans quel répertoire Homebrew s'installe-t-il sur ta machine ? Vérifie que ce
répertoire est bien dans ton `PATH`.

```sh
echo $PATH | tr ':' '\n'
```

Question : qu'est-ce que le `PATH`, et pourquoi le shell en a-t-il besoin pour trouver une
commande ? Fais le lien avec ce que tu sais des chemins absolus et relatifs.

### Exercice 3.3 — Installer Flutter et lire le diagnostic

Installe le SDK Flutter, puis lance :

```sh
flutter doctor -v
```

Ne cherche pas à tout réparer d'un coup. **Recopie la sortie complète dans un fichier**,
puis, ligne par ligne, écris pour chaque problème signalé :

1. Ce que l'outil te dit qu'il manque
2. Ce que tu penses que c'est
3. Ce que tu vas faire

Le but ici n'est pas d'avoir des coches vertes partout : c'est d'apprendre à lire un
diagnostic sans paniquer.

### Exercice 3.4 — Créer un émulateur Android

Installe Android Studio, puis crée un appareil virtuel. Attention au choix de l'image
système : quelle architecture dois-tu prendre sur ta machine, et pourquoi l'autre serait
catastrophiquement lente ?

Lance-le, puis vérifie depuis le terminal :

```sh
adb devices
```

### Exercice 3.5 — Explorer Android depuis le terminal

Ton émulateur tourne. Ouvre un shell dedans :

```sh
adb shell
```

Tu es maintenant dans un système Linux, sur un téléphone virtuel. Utilise ce que tu sais
déjà :

- Où es-tu ? (`pwd`)
- Que contient `/` ? (`ls -l /`)
- Que contient `/data` ? Que se passe-t-il et pourquoi ?
- Trouve le répertoire où sont stockées les applications installées.
- Regarde les permissions d'un fichier avec `ls -l`. Sais-tu les lire ?

Quitte avec `exit`. Écris trois différences que tu remarques entre ce système de fichiers
et celui de ton Mac.

### Exercice 3.6 — Lancer un simulateur iOS

Depuis le terminal :

```sh
xcrun simctl list devices        # lister les simulateurs disponibles
open -a Simulator                # ouvrir le simulateur
```

Puis réponds : pourquoi le simulateur iOS démarre-t-il beaucoup plus vite que l'émulateur
Android ? Quelle est la différence fondamentale entre les deux, et quelles fonctionnalités
ne peux-tu **pas** tester sur un simulateur ?

---

## Niveau 4 — Premier projet et boucle de travail

### Exercice 4.1 — Créer et lancer

```sh
flutter create mon_premier_app
cd mon_premier_app
flutter run
```

L'application de démonstration (un compteur) doit apparaître. Si ce n'est pas le cas, ne
cherche pas de solution toute faite : lis le message d'erreur en entier et essaie de dire
ce qu'il t'apprend.

### Exercice 4.2 — Cartographier les fichiers

Explore l'arborescence créée :

```sh
ls -la
find . -maxdepth 2 -type d -not -path "*/.*"
```

Dresse un tableau qui explique le rôle de chaque élément :

| Élément | Rôle | Est-ce toi qui l'écris ? |
|---|---|---|
| `lib/` | | |
| `lib/main.dart` | | |
| `pubspec.yaml` | | |
| `android/` | | |
| `ios/` | | |
| `test/` | | |
| `build/` | | |

Question importante : pourquoi y a-t-il des dossiers `android/` et `ios/` dans un projet
« cross-platform » ? Que contiennent-ils ? Relie ta réponse à la section 8 de la leçon.

### Exercice 4.3 — Éprouver le hot reload

L'application tourne. Sans l'arrêter :

1. Incrémente le compteur jusqu'à 7.
2. Modifie le texte du titre dans `lib/main.dart`, sauvegarde, observe.
3. Le compteur est-il toujours à 7 ?
4. Maintenant appuie sur `R` (majuscule) dans le terminal — c'est le *hot restart*.
5. Le compteur est-il toujours à 7 ?

Explique la différence entre hot reload et hot restart, et dis dans quel cas tu es obligé
d'utiliser le second.

### Exercice 4.4 — Comparer avec ta boucle en C

Chronomètre honnêtement :

- Le temps entre « je modifie une ligne de C » et « je vois le résultat » (édition, `gcc`,
  exécution).
- Le temps entre « je modifie une ligne de Dart » et « je vois le résultat » avec le hot
  reload.

Écris en trois phrases ce que ce rapport change concrètement dans ta façon d'apprendre.

### Exercice 4.5 — Lire `pubspec.yaml`

Ouvre `pubspec.yaml`. Ce fichier déclare les dépendances du projet.

- Quelles dépendances y a-t-il déjà ?
- Ajoutes-en une (par exemple `http`), lance `flutter pub get`, et regarde ce qui a changé
  dans le projet.
- Un fichier `pubspec.lock` est apparu ou a été modifié. À quoi sert-il ? Pourquoi est-ce
  important qu'il soit versionné avec le code ?

### Exercice 4.6 — Le même projet sur les deux plateformes

Lance la même application sur l'émulateur Android puis sur le simulateur iOS :

```sh
flutter devices
flutter run -d <identifiant_de_l_appareil>
```

Mets les deux côte à côte. Y a-t-il une différence visible ? Explique pourquoi, à partir de
l'architecture de Flutter décrite en section 3.3 de la leçon.

---

## Niveau 5 — Widgets, état et navigation

> **Prérequis :** ce niveau suppose des notions de programmation orientée objet (classes,
> héritage, méthodes) que tu n'as pas encore vues. Si tu bloques, ce n'est pas un problème
> de niveau : c'est que la brique manque. Reviens après.

### Exercice 5.1 — Composer un arbre de widgets

Sans copier de tutoriel, construis un écran contenant :

- une barre de titre,
- une image ou une icône centrée,
- un texte sous l'image,
- deux boutons côte à côte.

Puis **dessine l'arbre de widgets sur papier**, avant d'écrire le code. Compare ensuite ton
dessin à ce que tu as réellement écrit.

### Exercice 5.2 — Stateless contre Stateful

Écris deux widgets : un qui affiche un texte fixe, un qui affiche un compteur cliquable.

Réponds ensuite : pourquoi le premier n'a-t-il pas besoin d'un `State` ? Que se passe-t-il
exactement quand tu appelles `setState()` ? (Indice : relis le passage sur l'interface
déclarative en section 4.2.)

### Exercice 5.3 — Une liste

Affiche une liste de vingt éléments avec `ListView.builder`.

Question : pourquoi utilise-t-on `ListView.builder` plutôt que de construire les vingt
widgets d'un coup ? Que se passerait-il avec vingt mille éléments ?

### Exercice 5.4 — Naviguer entre deux écrans

Ajoute un second écran, atteignable en appuyant sur un élément de ta liste, et qui affiche
le détail de l'élément choisi.

Puis observe le comportement du geste retour : appui sur le bouton retour d'Android,
balayage depuis le bord gauche sur iOS. Les deux fonctionnent-ils ? Qui s'en est chargé ?

### Exercice 5.5 — Faire remonter l'état

Tu as maintenant deux écrans. Fais en sorte qu'une modification sur l'écran de détail se
répercute sur la liste au retour.

C'est le premier vrai problème d'architecture que tu rencontres. Écris, **avant de coder**,
où tu penses que la donnée doit vivre et pourquoi. Puis code, et vois si ton intuition
tenait.

### Exercice 5.6 — Une différence entre les plateformes

Ajoute un composant qui n'a pas le même aspect natif sur les deux plateformes : un sélecteur
de date, ou une boîte de dialogue de confirmation.

Lance sur les deux. Que constates-tu ? Est-ce ce que tu veux ? Flutter propose des widgets
`Cupertino` (style iOS) en plus des widgets `Material` : dans quel cas t'en servirais-tu ?

---

## Niveau 6 — Architecture, publication, décisions de conception

### Exercice 6.1 — Découper une application

On te demande une application de suivi de dépenses : saisir une dépense, la catégoriser,
voir un total mensuel, synchroniser avec un serveur.

Sans écrire une ligne de code, découpe-la en couches (interface / logique métier / accès
aux données) et dis, pour chaque couche, ce qui serait partagé et ce qui serait spécifique
à une plateforme. Fais l'exercice deux fois : une fois dans une hypothèse Flutter, une fois
dans une hypothèse Kotlin Multiplatform. Compare les deux découpages.

### Exercice 6.2 — Une permission, deux plateformes

Ton application a besoin de l'appareil photo. Trouve dans ton projet Flutter :

- le fichier Android où la permission se déclare,
- le fichier iOS où la justification se déclare.

```sh
find android -name "AndroidManifest.xml"
find ios -name "Info.plist"
```

Ouvre-les et lis-les. Écris ce que tu devrais ajouter dans chacun. Pourquoi Apple exige-t-il
une phrase en langage humain, et pas seulement un nom de permission ?

### Exercice 6.3 — Le cycle de vie

Lance ton application, puis mets-la en arrière-plan et reviens. Puis force sa fermeture et
relance-la.

Que se passe-t-il pour l'état de ton compteur dans chaque cas ? Quelles données devrais-tu
sauvegarder de façon persistante pour que l'utilisateur ne perde rien ? Où les stocker ?

### Exercice 6.4 — Construire un paquet de production

```sh
flutter build apk --release
```

Puis mesure la taille du résultat :

```sh
ls -lh build/app/outputs/flutter-apk/
du -h build/app/outputs/flutter-apk/app-release.apk
```

Compare avec la version de débogage. Pourquoi une telle différence ? Quel lien avec la
distinction JIT / AOT de la section 4.5 de la leçon ?

### Exercice 6.5 — Le parcours de publication

Sans rien publier, documente le chemin complet vers les deux stores. Réponds à :

- Combien coûte chaque compte développeur, et selon quel rythme ?
- Qu'est-ce qu'une clé de signature Android ? Que se passe-t-il si tu la perds ?
- Qu'est-ce qu'un profil de provisionnement Apple ?
- Combien de temps prend en moyenne une revue sur chaque store ?
- Cite trois motifs fréquents de refus par Apple.

### Exercice 6.6 — Le débat honnête

Rédige un texte d'une page défendant ce point de vue : **« le cross-platform ne dispense
jamais d'apprendre les deux plateformes »**. Appuie-toi sur des exemples concrets que tu as
rencontrés dans les exercices 6.2 et 6.3, pas sur des généralités.

### Exercice 6.7 — Ton plan personnel

En reprenant la section 12 de la leçon, écris **ton** plan d'apprentissage des douze
prochains mois : quelles briques, dans quel ordre, avec quel projet concret pour valider
chacune. Sois réaliste sur ton temps disponible.

Date ce document. Relis-le dans six mois.

---

## Auto-évaluation

Coche honnêtement. Une case non cochée n'est pas un échec : c'est ton prochain objectif.

**Compréhension des architectures**

- [ ] Je peux expliquer les trois approches (WebView, pont natif, moteur propre) sans notes
- [ ] Je sais dire, pour chacune, qui dessine les pixels à l'écran
- [ ] Je sais expliquer pourquoi l'ancien pont de React Native était lent
- [ ] Je comprends ce que JSI et Fabric ont changé
- [ ] Je sais pourquoi une application Flutter est plus lourde qu'une native
- [ ] Je sais expliquer la différence entre JIT et AOT, et à quoi sert chacun

**Choix technologique**

- [ ] Je sais poser la question « qu'est-ce qui est partagé ? » face à n'importe quelle techno
- [ ] Je peux argumenter un choix avec au moins trois raisons et un contre-argument
- [ ] Je comprends pourquoi Kotlin Multiplatform partage la logique et pas l'interface
- [ ] Je sais nommer au moins trois cas où le natif reste le bon choix
- [ ] J'ai écrit ma propre grille de décision

**Environnement et outillage**

- [ ] `flutter doctor` tourne et je sais interpréter sa sortie
- [ ] J'ai un émulateur Android fonctionnel, avec la bonne architecture d'image
- [ ] J'ai un simulateur iOS fonctionnel
- [ ] Je sais la différence entre un émulateur et un simulateur
- [ ] Je sais utiliser `adb devices`, `adb logcat` et `adb shell`
- [ ] Je sais où Homebrew s'installe sur Apple Silicon, et pourquoi c'est là

**Pratique**

- [ ] J'ai créé, lancé et modifié un projet Flutter
- [ ] Je connais le rôle de chaque dossier d'un projet Flutter
- [ ] Je sais ce que fait le hot reload et en quoi il diffère du hot restart
- [ ] J'ai lancé la même application sur les deux plateformes
- [ ] J'ai construit un `.apk` de production et compris pourquoi sa taille diffère

**Lucidité**

- [ ] Je sais nommer quatre choses que le cross-platform ne résout pas
- [ ] Je sais où se déclarent les permissions sur chaque plateforme
- [ ] Je comprends pourquoi compiler pour iOS exige un Mac
- [ ] Je sais dire ce qui me manque encore avant d'attaquer Flutter sérieusement

---

## Où chercher les réponses

Ce projet ne fournit pas de corrigés. Ce n'est pas de la paresse : quand tu développeras
pour de vrai, il n'y aura pas de corrigé non plus, et la compétence qui compte est de
savoir trouver et évaluer une information. Voici où chercher, par ordre de fiabilité.

**La documentation officielle d'abord.** C'est la source qui a raison quand les autres se
contredisent.

- Flutter : `docs.flutter.dev` — la section « Flutter for beginners » et le catalogue de
  widgets sont excellents.
- Dart : `dart.dev/language` — la référence du langage.
- React Native : `reactnative.dev`
- Kotlin Multiplatform : `kotlinlang.org/docs/multiplatform.html`
- Android : `developer.android.com`
- Apple : `developer.apple.com/documentation`

**Les outils eux-mêmes.** Beaucoup de réponses sont dans ta machine :

```sh
flutter --help
flutter doctor -v
adb --help
man ls
```

Cette dernière commande, `man`, est ton meilleur outil pour tout ce qui est Unix. Elle
donne la vérité sur ta machine, dans ta version des outils BSD de macOS — ce qui n'est pas
toujours ce que raconte un article de blog écrit sous Linux.

**Les messages d'erreur.** Lis-les en entier, jusqu'au bout. Ceux de Flutter sont
particulièrement bavards et te disent souvent littéralement quoi corriger. L'erreur est un
texte à lire, pas un mur à contourner.

**Le code source du projet généré.** `flutter create` t'écrit un projet complet et
commenté. Lis `lib/main.dart` en entier, ligne par ligne, même les lignes que tu ne
comprends pas. C'est un exercice en soi.

**Les communautés, en dernier recours.** Stack Overflow, r/FlutterDev, le Discord Flutter.
Attention : beaucoup de réponses y sont périmées. Vérifie toujours la date, et méfie-toi de
tout ce qui date d'avant 2023 sur React Native (la nouvelle architecture a tout changé) ou
d'avant 2022 sur Kotlin Multiplatform.

**Une règle de méthode pour finir.** Quand tu bloques, écris le problème en une phrase
complète avant de chercher. La moitié du temps, la phrase contient la réponse.
