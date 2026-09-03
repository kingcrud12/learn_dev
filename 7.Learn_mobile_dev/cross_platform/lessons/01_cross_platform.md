# Le développement mobile cross-platform

## 1. Le problème de départ : pourquoi tout écrire deux fois ?

Quand tu veux publier une application mobile, tu ne vises pas « le mobile ». Tu vises
deux mondes séparés qui n'ont presque rien en commun sous le capot : Android et iOS.

Ces deux mondes ont été construits par deux entreprises différentes, à des époques
différentes, avec des philosophies différentes. Résultat concret :

| | Android | iOS |
|---|---|---|
| Éditeur | Google | Apple |
| Langage principal | Kotlin (avant : Java) | Swift (avant : Objective-C) |
| Outil de développement | Android Studio | Xcode |
| Machine nécessaire | Windows, macOS ou Linux | **macOS uniquement** |
| Boutique | Google Play | App Store |
| Format du paquet livré | `.aab` / `.apk` | `.ipa` |
| Bibliothèque d'interface | Jetpack Compose / Views | SwiftUI / UIKit |

Imagine que tu veux faire une application toute simple : une liste de tâches avec un
bouton « ajouter ». En natif pur, tu écris ce bouton deux fois. Tu écris la liste deux
fois. Tu écris la sauvegarde des données deux fois. Tu écris la logique « si le champ est
vide, ne rien faire » deux fois. Et surtout : quand tu corriges un bug, tu le corriges
deux fois, dans deux langages, dans deux éditeurs, sur deux calendriers de publication.

Dans une entreprise, ça se traduit par deux équipes. Deux équipes qui doivent rester
synchronisées, qui interprètent parfois la même spécification différemment, et dont les
applications finissent par diverger subtilement. Le bouton n'est pas tout à fait au même
endroit, l'animation n'est pas la même, une des deux versions a un bug que l'autre n'a
pas.

Le cross-platform, c'est la tentative de répondre à cette question : **peut-on écrire le
code une seule fois et le faire tourner sur les deux plateformes ?**

Tu vas voir que la réponse est « oui, largement, mais avec des contreparties », et que
selon la technologie choisie, on ne partage pas du tout les mêmes choses.

---

## 2. Ce que « cross-platform » veut dire vraiment

Le mot est trompeur. Il laisse croire qu'il y a une seule technique. En réalité, il faut
te poser la question : **qu'est-ce qui est partagé, exactement ?**

Une application mobile, c'est grossièrement trois couches :

```
+---------------------------------------------------+
|  INTERFACE (UI)                                   |
|  Ce que l'utilisateur voit et touche :            |
|  boutons, listes, textes, animations              |
+---------------------------------------------------+
|  LOGIQUE MÉTIER                                   |
|  Les règles : valider un formulaire, calculer     |
|  un total, décider quoi afficher                  |
+---------------------------------------------------+
|  ACCÈS PLATEFORME                                 |
|  Appareil photo, GPS, notifications, stockage,    |
|  Bluetooth, capteur d'empreinte                   |
+---------------------------------------------------+
```

Les différentes technologies cross-platform ne partagent pas les mêmes couches :

- Certaines partagent **tout** (l'interface comprise) : Flutter, React Native.
- Certaines partagent **seulement la logique métier** et laissent chaque plateforme
  dessiner sa propre interface : Kotlin Multiplatform.

Cette distinction est la plus importante de toute la leçon. Retiens-la : elle explique
90 % des débats entre développeurs sur le sujet.

---

## 3. Les trois grandes approches historiques

### 3.1 Approche 1 — La WebView (Cordova, Ionic, PhoneGap)

C'est la plus ancienne idée, née vers 2009. Le raisonnement était le suivant : les
navigateurs web savent déjà afficher des interfaces sur n'importe quel appareil. Un
téléphone contient un navigateur. Donc si j'écris mon application en HTML, CSS et
JavaScript, et que je l'affiche dans un navigateur sans barre d'adresse, j'ai une
application qui marche partout.

Techniquement, on emballe un site web dans une coquille native. Cette coquille contient
un composant appelé **WebView** : un navigateur web sans décoration, qui occupe tout
l'écran.

```
  APPLICATION CORDOVA / IONIC
  +-----------------------------------------------+
  |  Coquille native (Android ou iOS)             |
  |  +-----------------------------------------+  |
  |  |  WEBVIEW (un navigateur invisible)      |  |
  |  |                                         |  |
  |  |  Ton code : HTML + CSS + JavaScript     |  |
  |  |  Rendu par le moteur du navigateur      |  |
  |  |  (WebKit sur iOS, Chromium sur Android) |  |
  |  +-----------------------------------------+  |
  |            |                                  |
  |            | pont JavaScript <-> natif        |
  |            v                                  |
  |  Plugins natifs (caméra, GPS, contacts...)    |
  +-----------------------------------------------+
```

**Ce qui est bien :** si tu sais déjà faire des sites web, tu sais déjà faire des
applications. Le code est partagé à quasiment 100 %.

**Pourquoi c'est en déclin :** parce qu'une page web dans un cadre, ça se sent. Le
défilement n'a pas la même inertie qu'une vraie liste native. Les animations saccadent
sur les téléphones d'entrée de gamme. Le clavier ne se comporte pas exactement comme
attendu. L'utilisateur ne sait pas expliquer pourquoi, mais il trouve l'application
« bizarre ». On appelle ça la vallée de l'étrange de l'interface.

À cela s'ajoute un problème de performance structurel : le JavaScript de la WebView doit
demander la permission au natif pour tout ce qui touche au matériel, en passant par un
pont de communication lent qui sérialise chaque message.

Aujourd'hui cette approche survit surtout pour des applications internes d'entreprise ou
des projets à très petit budget. Capacitor (le successeur moderne de Cordova) l'a
modernisée, mais le fond du problème reste.

### 3.2 Approche 2 — Le pont vers le natif (React Native)

En 2015, Facebook propose une idée différente : et si, au lieu d'afficher une page web,
on écrivait du JavaScript qui **pilote de vrais composants natifs** ?

Tu écris `<Button>` en JavaScript. React Native ne dessine pas un bouton lui-même : il
demande à Android de créer un vrai `android.widget.Button`, ou à iOS de créer un vrai
`UIButton`. Ce que l'utilisateur touche est authentiquement natif.

```
  APPLICATION REACT NATIVE (architecture historique)
  +-----------------------------------------------+
  |  Ton code JavaScript / TypeScript             |
  |  (composants React, logique, état)            |
  +-----------------------------------------------+
  |  Moteur JavaScript (Hermes / JavaScriptCore)  |
  +-----------------------------------------------+
                      |
              LE BRIDGE (le pont)
        messages sérialisés en JSON, asynchrones
                      |
                      v
  +-----------------------------------------------+
  |  Composants NATIFS réels                      |
  |  Android : android.widget.*                   |
  |  iOS     : UIKit                              |
  +-----------------------------------------------+
```

> **Prérequis :** cette section suppose que tu saches ce qu'est le JSON (un format
> d'échange de données textuel) et ce qu'est un appel asynchrone (une opération dont on
> n'attend pas immédiatement le résultat). Tu ne les as pas encore vus. Retiens juste
> pour l'instant que le « pont » est un tuyau par lequel deux mondes se parlent en
> s'envoyant des messages écrits, et que traduire dans les deux sens coûte du temps.

Le point faible de cette architecture, c'était justement ce pont. Chaque interaction
traversait un goulot d'étranglement. Une animation pilotée depuis le JavaScript pouvait
saccader parce que les messages n'arrivaient pas assez vite.

C'est pour ça que React Native a été profondément réarchitecturé (voir section 5).

### 3.3 Approche 3 — Le moteur de rendu propriétaire (Flutter)

En 2018, Google prend le contre-pied total. Le raisonnement : le problème vient de ce
qu'on essaie de négocier avec les composants natifs de chaque plateforme, qui ne se
comportent jamais pareil. Alors arrêtons de négocier. **Dessinons tout nous-mêmes.**

Flutter ne demande rien à Android ni à iOS, sauf une chose : « donne-moi une surface de
pixels vierge, plein écran, et transmets-moi les touchers ». Ensuite, Flutter dessine
chaque bouton, chaque texte, chaque ombre, pixel par pixel, avec son propre moteur
graphique — exactement comme le fait un jeu vidéo.

```
  APPLICATION FLUTTER
  +-----------------------------------------------+
  |  Ton code Dart                                |
  |  (arbre de widgets, logique, état)            |
  +-----------------------------------------------+
  |  Framework Flutter (widgets, layout, anim.)   |
  +-----------------------------------------------+
  |  MOTEUR DE RENDU (Skia / Impeller)            |
  |  compilé en code machine, écrit en C++        |
  +-----------------------------------------------+
                      |
        « donne-moi une toile de pixels »
                      |
                      v
  +-----------------------------------------------+
  |  Android / iOS                                |
  |  fournit : une surface, les événements tactiles|
  |  l'OS ne connaît RIEN de tes boutons          |
  +-----------------------------------------------+
```

La conséquence est radicale : un bouton Flutter est **rigoureusement identique** sur
Android et iOS, au pixel près, parce que ce n'est pas Android ni iOS qui le dessine.

C'est à la fois la grande force et la grande faiblesse de Flutter. Force : plus de
divergence entre les plateformes, contrôle total du rendu. Faiblesse : ton application ne
suit pas automatiquement les évolutions visuelles du système, et l'accessibilité ou la
sélection de texte doivent être réimplémentées par Flutter plutôt qu'héritées de l'OS.

Note aussi que ce moteur graphique est écrit en **C++**, un descendant du C que tu viens
de commencer. Le C n'est pas un détour inutile : c'est le langage dans lequel sont écrits
les moteurs sur lesquels reposent toutes ces technologies de haut niveau.

---

## 4. Flutter en détail

### 4.1 Le langage : Dart

Flutter s'écrit en Dart, un langage créé par Google. Dart est typé, orienté objet, et sa
syntaxe ressemble beaucoup à Java, C# ou JavaScript.

> **Prérequis :** cette section suppose la programmation orientée objet (les classes, les
> objets, l'héritage), que tu n'as pas encore vue. Tu la travailleras avant d'attaquer
> Dart sérieusement. Lis le code ci-dessous comme une illustration de la *forme*, pas
> comme quelque chose à comprendre entièrement aujourd'hui.

```dart
class Compteur extends StatefulWidget {
  const Compteur({super.key});

  @override
  State<Compteur> createState() => _CompteurState();
}

class _CompteurState extends State<Compteur> {
  int valeur = 0;

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Text('Valeur : $valeur'),
        ElevatedButton(
          onPressed: () => setState(() => valeur++),
          child: const Text('Incrémenter'),
        ),
      ],
    );
  }
}
```

Ce que tu peux déjà remarquer, même sans connaître Dart : les accolades, les
points-virgules et les types (`int`) te sont familiers grâce au C. La grosse nouveauté par
rapport au C, c'est que tout est construit à partir de classes.

### 4.2 L'arbre de widgets

En Flutter, **tout est un widget**. Un texte est un widget. Un bouton est un widget. Le
fait de mettre un espacement autour d'un élément est un widget. Le fait de centrer est un
widget. Ces widgets s'emboîtent les uns dans les autres et forment un arbre :

```
        MaterialApp
             |
          Scaffold
          /       \
      AppBar      Center
        |            |
      Text        Column
                 /      \
              Text   ElevatedButton
                            |
                          Text
```

Décrire une interface, c'est décrire cet arbre. Tu ne dis jamais « change la couleur de ce
bouton ». Tu dis « voici à quoi l'arbre doit ressembler maintenant », et Flutter calcule
la différence avec l'arbre précédent puis ne redessine que ce qui a changé. Ce style
s'appelle l'**interface déclarative** — c'est aujourd'hui la façon dominante de faire de
l'interface, sur mobile comme sur le web.

### 4.3 Le hot reload

C'est l'argument commercial numéro un de Flutter, et il est mérité.

Souviens-toi de ta boucle de travail en C : tu modifies le fichier, tu lances `gcc`, tu
attends, tu exécutes le binaire, tu regardes le résultat. Quelques secondes à chaque fois.

Avec le hot reload, tu modifies ton code, tu sauvegardes, et **moins d'une seconde plus
tard** l'application déjà en cours d'exécution sur le téléphone a changé — sans redémarrer,
sans perdre l'état où tu en étais (l'écran affiché, le texte déjà saisi). Pour apprendre,
c'est un accélérateur considérable : tu essaies, tu vois, tu corriges, en boucle serrée.

### 4.4 Skia et Impeller

**Skia** est la bibliothèque graphique historique de Flutter (c'est aussi celle de
Chrome). Elle sait dessiner des formes, du texte et des images sur une surface de pixels.

Son défaut : elle compile ses instructions graphiques (les *shaders*) au moment où elle en
a besoin, ce qui provoque des micro-saccades la première fois qu'une animation apparaît.

**Impeller** est le nouveau moteur qui remplace progressivement Skia. Il précompile ces
instructions à l'avance, ce qui supprime ces saccades. Il est aujourd'hui le moteur par
défaut sur iOS et Android.

### 4.5 La compilation AOT

Deux façons d'exécuter du code existent :

| Mode | Fonctionnement | Utilisé par Flutter pour |
|---|---|---|
| **JIT** (Just In Time) | Le code est traduit en instructions machine pendant l'exécution | Le développement — c'est ce qui permet le hot reload |
| **AOT** (Ahead Of Time) | Le code est traduit en instructions machine **avant** la livraison | La production — l'application publiée |

En production, ton code Dart est donc compilé en vrai code machine ARM, comme le fait
`gcc` avec ton C. Il n'y a pas d'interprète qui tourne pendant que l'utilisateur se sert
de l'application. C'est la raison technique pour laquelle Flutter est rapide.

---

## 5. React Native en détail

### 5.1 Le langage : JavaScript et TypeScript

React Native s'écrit en JavaScript, ou de plus en plus en **TypeScript** — qui est du
JavaScript auquel on a ajouté des types, exactement comme le C t'oblige à déclarer `int x`.
Dans le monde professionnel, TypeScript est devenu la norme.

La syntaxe de l'interface utilise **JSX** : du code qui ressemble à du HTML, écrit
directement au milieu du JavaScript.

```jsx
function Compteur() {
  const [valeur, setValeur] = useState(0);

  return (
    <View>
      <Text>Valeur : {valeur}</Text>
      <Button title="Incrémenter" onPress={() => setValeur(valeur + 1)} />
    </View>
  );
}
```

Compare avec l'exemple Dart plus haut : c'est exactement la même idée (décrire un arbre
d'éléments, réagir à un changement d'état), avec une syntaxe plus légère.

### 5.2 La nouvelle architecture : JSI et Fabric

Le pont sérialisé décrit en 3.2 a été démoli et remplacé. Deux briques à connaître :

**JSI** (JavaScript Interface) supprime la sérialisation. Au lieu d'envoyer des messages
texte, le JavaScript peut désormais tenir directement une référence vers un objet C++ et
appeler ses fonctions. C'est un appel direct, synchrone, sans traduction intermédiaire.

**Fabric** est le nouveau système de rendu bâti sur JSI. Il gère l'arbre des composants
côté C++, ce qui lui permet de calculer la mise en page de façon synchrone et d'éviter les
décalages visuels.

```
  REACT NATIVE — NOUVELLE ARCHITECTURE
  +-----------------------------------------------+
  |  Ton code JavaScript / TypeScript             |
  +-----------------------------------------------+
  |  Moteur Hermes                                |
  +-----------------------------------------------+
  |  JSI : appels DIRECTS vers le C++             |
  |  (plus de sérialisation, plus de file d'attente)|
  +-----------------------------------------------+
  |  Fabric (rendu) + TurboModules (modules natifs)|
  +-----------------------------------------------+
                      |
                      v
  +-----------------------------------------------+
  |  Composants NATIFS réels                      |
  +-----------------------------------------------+
```

Le résultat : les critiques de performance historiquement adressées à React Native sont
en grande partie obsolètes.

### 5.3 L'écosystème npm

C'est l'atout majeur de React Native. **npm** est le dépôt de bibliothèques JavaScript, et
c'est de très loin le plus grand écosystème de code réutilisable au monde. Pour presque
n'importe quel besoin, une bibliothèque existe déjà.

C'est aussi son point faible : la qualité est très inégale, beaucoup de paquets sont
abandonnés, et une application peut finir par dépendre de centaines de bibliothèques dont
tu n'as vérifié aucune. On appelle ça la dette de dépendances.

Enfin, **Expo** est un ensemble d'outils qui simplifie énormément le démarrage : tu peux
lancer un projet React Native et le voir tourner sur ton téléphone sans installer Xcode ni
Android Studio le premier jour. Pour un débutant, c'est le chemin le plus court vers un
premier résultat visible.

---

## 6. Kotlin Multiplatform : partager la logique, pas l'interface

KMP (Kotlin Multiplatform) part d'un constat différent. Ses concepteurs disent : l'interface
est justement la partie qu'il **faut** écrire deux fois, parce qu'un utilisateur iOS et un
utilisateur Android n'ont pas les mêmes attentes visuelles et gestuelles. En revanche, la
logique métier — les règles de calcul, la validation, l'accès au réseau, le cache local —
est strictement identique des deux côtés. C'est **ça** qu'il faut partager.

```
  KOTLIN MULTIPLATFORM
                  +---------------------------+
                  |   CODE KOTLIN PARTAGÉ     |
                  |   - logique métier        |
                  |   - modèles de données    |
                  |   - accès réseau, cache   |
                  +---------------------------+
                    /                       \
    compilé vers   /                         \   compilé vers
    bytecode JVM  /                           \  binaire natif
                 v                             v
  +---------------------+          +---------------------------+
  |  UI Android         |          |  UI iOS                   |
  |  Jetpack Compose    |          |  SwiftUI (en Swift)       |
  |  écrite en Kotlin   |          |  écrite à la main         |
  +---------------------+          +---------------------------+
```

Concrètement, le code partagé ressemble à ceci :

```kotlin
// Code commun aux deux plateformes
class ValidateurPanier(private val articles: List<Article>) {
    fun total(): Double = articles.sumOf { it.prix * it.quantite }

    fun estValide(): Boolean = articles.isNotEmpty() && total() > 0.0
}
```

Ce fichier est compilé une fois vers du bytecode pour Android, une fois vers un binaire
natif pour iOS. Les deux applications appellent la même règle métier, donc elles ne
peuvent pas diverger sur le calcul du total.

**Pourquoi c'est un compromis intéressant :** tu ne renonces à rien du côté de l'interface
(chaque plateforme est authentiquement native, avec ses animations, son accessibilité, ses
conventions), et tu élimines quand même la duplication là où elle est la plus dangereuse —
dans les règles métier, là où un bug se traduit par un mauvais montant facturé.

**Le coût :** tu dois savoir écrire de l'interface Android *et* de l'interface iOS. Ce
n'est pas une technologie qui te dispense d'apprendre les deux plateformes. C'est plutôt
l'outil des équipes qui les connaissent déjà et veulent arrêter de dupliquer.

À noter : **Compose Multiplatform** est une extension de KMP qui permet aussi de partager
l'interface, en rapprochant KMP du modèle de Flutter. C'est prometteur mais plus récent, et
son support iOS est encore jeune.

---

## 7. Tableau comparatif honnête

| Critère | Flutter | React Native | Kotlin Multiplatform | Natif (Kotlin + Swift) |
|---|---|---|---|---|
| Langage | Dart | TypeScript / JS | Kotlin (+ Swift pour l'UI iOS) | Kotlin et Swift |
| Ce qui est partagé | UI + logique | UI + logique | logique seulement | rien |
| Performance UI | très bonne (rendu propre) | très bonne depuis Fabric | native (c'est du natif) | référence absolue |
| Performance calcul | très bonne (AOT) | bonne (Hermes) | native | référence absolue |
| Taille de l'app (min.) | ~15-20 Mo (moteur embarqué) | ~8-12 Mo | ~5-8 Mo | ~3-6 Mo |
| Accès aux API natives | via plugins, sinon code natif à écrire | via modules natifs, très fourni | direct, sans intermédiaire | direct et total |
| Nouveautés OS disponibles | avec un délai | avec un délai | immédiatement | immédiatement |
| Courbe d'apprentissage | moyenne (Dart à apprendre) | douce si tu connais le web | raide (deux UI à connaître) | la plus raide (deux mondes) |
| Maturité | mûr et stable | très mûr | jeune mais stabilisé | maximale |
| Cohérence visuelle entre OS | identique au pixel | proche du natif, petits écarts | volontairement différente | volontairement différente |
| Marché de l'emploi | fort, surtout hors très grandes entreprises | très fort, y compris dans les grandes entreprises | en croissance, encore de niche | fort et durable, mieux rémunéré |
| Idéal pour | app soignée et cohérente, petite équipe | équipe venant du web, itération rapide | équipe déjà native voulant mutualiser | exigences extrêmes |

Un mot sur la taille de l'application : Flutter embarque son propre moteur graphique dans
chaque application, d'où le surcoût. Sur un téléphone récent ça n'a aucune importance ; sur
un marché où les gens ont peu de stockage et un forfait data limité, ça compte.

---

## 8. Ce que le cross-platform NE résout PAS

C'est la partie que les tutoriels enthousiastes oublient, et c'est probablement la plus
importante à retenir.

**Tu dois toujours comprendre les deux plateformes.** Aucune de ces technologies ne t'en
dispense. Voici pourquoi.

### 8.1 Les permissions

Demander l'accès à l'appareil photo ne fonctionne pas pareil. Sur Android, tu déclares la
permission dans un fichier `AndroidManifest.xml`, et tu la demandes à l'exécution. Sur
iOS, tu déclares dans `Info.plist` une chaîne expliquant *pourquoi* tu la veux, en langage
humain — et si cette chaîne est absente ou jugée insuffisante, Apple refuse ton
application. Ton framework cross-platform t'offre une fonction unique pour demander la
permission, mais **c'est toi qui remplis ces deux fichiers**.

### 8.2 Le cycle de vie de l'application

Ce qui se passe quand l'utilisateur quitte ton application, reçoit un appel, ou revient
trois jours plus tard, diffère profondément. Android peut détruire ton application à tout
moment pour récupérer de la mémoire, et s'attend à ce que tu saches restaurer l'état. iOS
suspend puis tue selon d'autres règles, et limite sévèrement le travail en arrière-plan.
Une application qui ignore ça perd les données de ses utilisateurs.

### 8.3 La publication sur les stores

Deux processus complètement distincts, avec chacun ses comptes payants, ses certificats,
ses règles.

| | Google Play | App Store |
|---|---|---|
| Compte développeur | ~25 $ une fois | ~99 $ par an |
| Revue humaine | légère, souvent rapide | systématique, parfois plusieurs jours |
| Signature | clé de signature à conserver précieusement | certificats et profils de provisionnement Apple |
| Refus fréquents | rares | métadonnées, vie privée, guidelines |

### 8.4 Les guidelines de design

Android suit **Material Design**, iOS suit les **Human Interface Guidelines**. Ils ne sont
pas d'accord sur presque rien : la position du bouton retour, la place du titre, la manière
de présenter des onglets, le style des boîtes de dialogue.

Flutter te permet de faire une application identique partout — mais un utilisateur iOS
peut trouver cette application « pas à sa place ». C'est un choix de produit, pas un
détail technique.

### 8.5 Et parfois, il faut quand même écrire du natif

Dès que tu as besoin d'une fonctionnalité pour laquelle aucun plugin n'existe, tu dois
écrire du Kotlin et du Swift toi-même, et brancher ce code sur ton framework. C'est un
scénario normal, pas un échec. Les dossiers `native_android/` et `native_ios/` de ce
projet existent exactement pour cette raison : tu finiras par y aller.

---

## 9. Quand le natif reste le bon choix

Le cross-platform est excellent pour la grande majorité des applications. Mais il y a des
cas où il est le mauvais outil :

**Les jeux.** Tu n'utilises alors ni Flutter ni React Native, mais un moteur de jeu :
Unity, Unreal, Godot. Ce sont eux aussi des technologies cross-platform, mais d'une autre
famille, conçue autour d'une boucle de rendu à 60 images par seconde.

**La réalité augmentée.** ARKit sur iOS et ARCore sur Android sont des interfaces natives
profondes, qui évoluent vite et dont les fonctionnalités les plus récentes n'ont pas
d'équivalent cross-platform.

**Les applications très liées au matériel.** Un appareil photo professionnel qui contrôle
finement l'exposition, une application de santé qui lit un capteur, un objet connecté en
Bluetooth basse consommation : tu passeras ton temps dans du code natif, et l'abstraction
cross-platform ne fera que t'ajouter une couche à traverser.

**La performance extrême.** Traitement vidéo en temps réel, audio à faible latence,
modèles d'apprentissage automatique embarqués. Ici on descend même sous le natif, vers du
C et du C++ — le langage que tu es en train d'apprendre.

**Les applications vitrines d'une seule plateforme.** Si ton public est à 95 % sur iOS,
écrire deux fois n'a aucun sens : écris une fois, en Swift.

---

## 10. Le rôle de macOS : un avantage que tu as déjà

Une règle incontournable, imposée par Apple : **compiler une application iOS exige un Mac.**

Ce n'est pas un choix technique, c'est une contrainte de licence. Xcode, la chaîne de
compilation d'Apple, n'existe que sur macOS. Il n'y a pas de contournement légal et
pérenne.

Concrètement :

| Machine | Peut développer pour Android | Peut développer pour iOS |
|---|---|---|
| Windows | oui | non |
| Linux | oui | non |
| **macOS** | **oui** | **oui** |

Tu es sur un Mac Apple Silicon. C'est la seule configuration qui te permet de faire les
deux plateformes sur une seule machine, et le simulateur iOS y tourne nativement, donc
très vite. Un développeur sous Windows qui veut faire de l'iOS doit louer un Mac dans le
cloud ou en acheter un.

C'est un vrai avantage de départ. Ne le gâche pas en ne développant que pour Android.

---

## 11. La chaîne d'outils

Voici le vocabulaire que tu vas rencontrer partout. Rien à installer tout de suite ; il
s'agit de savoir de quoi on parle.

**Le SDK** (Software Development Kit) est l'ensemble des outils et bibliothèques d'une
plateforme : compilateurs, bibliothèques système, outils en ligne de commande. Il y a un
SDK Android, un SDK iOS, un SDK Flutter.

**L'émulateur Android** est un vrai Android qui tourne dans une machine virtuelle sur ton
Mac. Sur Apple Silicon, tu prends une image système ARM64 : elle tourne alors à vitesse
quasi native.

**Le simulateur iOS** n'est pas un émulateur. Il ne simule pas un processeur : il exécute
directement une version de ton application compilée pour le Mac. C'est plus rapide, mais
certaines choses (l'appareil photo, le Bluetooth, les vraies performances) ne sont pas
testables dessus.

**adb** (Android Debug Bridge) est l'outil en ligne de commande qui parle à un appareil ou
un émulateur Android. Tu es déjà à l'aise dans un terminal, donc tu vas t'en servir
naturellement :

```sh
adb devices              # lister les appareils connectés
adb install app.apk      # installer une application
adb logcat               # lire les journaux en direct
adb shell                # ouvrir un shell SUR le téléphone
```

Ce dernier est amusant : `adb shell` t'ouvre un shell Unix à l'intérieur du téléphone. Tes
`ls`, `cd`, `chmod` et tes notions de permissions y marchent — Android est un Linux.

**Xcode** est l'environnement d'Apple. Même en Flutter ou React Native, tu en auras besoin
pour compiler, signer et publier vers l'App Store. Son outil en ligne de commande
s'installe ainsi :

```sh
xcode-select --install
```

**Homebrew** est le gestionnaire de paquets de facto sur macOS. Sur Apple Silicon il
s'installe dans `/opt/homebrew`, et non `/usr/local` comme sur les Mac Intel — une
distinction de chemin absolu que tu sauras interpréter.

Enfin, `flutter doctor` est un outil de diagnostic qui vérifie ton installation et te dit
précisément ce qui manque. Tu le lanceras beaucoup au début.

---

## 12. Recommandation argumentée pour un débutant

Voici mon avis, avec les raisons — pas un décret.

**Commence par Flutter.** Quatre arguments :

1. **Un seul écosystème à apprendre.** Flutter fournit lui-même les widgets, la
   navigation, les animations, les tests. React Native t'oblige à assembler des
   bibliothèques npm concurrentes dès le premier jour, ce qui est écrasant quand on ne
   sait pas encore juger de la qualité d'une bibliothèque.

2. **Dart après le C, c'est une transition douce.** Accolades, points-virgules, types
   déclarés : tu retrouveras beaucoup de repères. Le JavaScript, avec son typage dynamique
   et ses règles de conversion surprenantes, apprend de mauvais réflexes à un débutant.

3. **Le hot reload rend l'apprentissage visuel.** Tu vois immédiatement l'effet de chaque
   modification. À ce stade, la vitesse de la boucle essai/erreur compte plus que tout le
   reste.

4. **Les messages d'erreur de Flutter sont exceptionnellement pédagogiques.** Ils
   t'expliquent souvent quoi faire, pas juste ce qui a cassé.

**Mais respecte l'ordre.** Ne saute pas dans Flutter demain matin. Il te manque des
fondations, et les poser d'abord te fera gagner du temps :

| Ordre | À acquérir | Pourquoi c'est bloquant sinon |
|---|---|---|
| 1 | Finir les bases du C | pointeurs, mémoire, compilation — ça éclaire tout le reste |
| 2 | La programmation orientée objet | Dart est entièrement bâti dessus, Flutter aussi |
| 3 | HTTP et JSON | une application mobile sans réseau ne fait presque rien |
| 4 | L'asynchrone | tout appel réseau est asynchrone, sans exception |
| 5 | Alors seulement : Dart, puis Flutter | |

Ensuite, quand tu seras à l'aise en Flutter, va faire un vrai détour par le natif — le
Kotlin dans `native_android/`, le Swift dans `native_ios/`. Pas pour changer de camp :
pour comprendre ce que ton framework fait à ta place. C'est cette compréhension qui sépare
un développeur qui suit des tutoriels d'un développeur qui sait déboguer.

---

## Mémo

- Deux plateformes, deux langages, deux outils : Android/Kotlin/Android Studio et
  iOS/Swift/Xcode. Le cross-platform existe pour ne pas tout écrire deux fois.
- La vraie question n'est jamais « quelle techno », mais **« qu'est-ce qui est partagé »** :
  l'UI et la logique, ou seulement la logique.
- **WebView** (Cordova/Ionic) : une page web déguisée. Simple, mais ça se sent. En déclin.
- **React Native** : du JS qui pilote de *vrais* composants natifs. Son ancien pont
  sérialisé était lent ; JSI et Fabric l'ont remplacé et ont réglé le problème.
- **Flutter** : dessine tout lui-même avec son propre moteur (Skia, puis Impeller), comme
  un jeu vidéo. Rendu identique partout, au pixel près.
- Flutter s'écrit en **Dart**, tout y est **widget**, le **hot reload** met à jour l'app en
  moins d'une seconde, et la production est compilée **AOT** en code machine.
- **Kotlin Multiplatform** partage la logique métier et laisse chaque plateforme faire son
  interface nativement. Excellent compromis, mais il faut connaître les deux UI.
- Le cross-platform ne dispense **jamais** de comprendre les deux plateformes :
  permissions, cycle de vie, publication, guidelines de design.
- Le natif reste le bon choix pour les jeux, l'AR, le matériel et la performance extrême.
- **Compiler pour iOS exige un Mac.** Tu en as un : c'est un vrai avantage.
- Vocabulaire outils : SDK, émulateur (Android, virtualisé), simulateur (iOS, pas
  virtualisé), `adb`, Xcode, `flutter doctor`.
- Recommandation : **Flutter en premier**, mais après le C, la POO, HTTP/JSON et
  l'asynchrone. Puis un détour par le natif pour comprendre le dessous.

---

Exercices associés : [../exercices/cross_platform.md](../exercices/cross_platform.md)
