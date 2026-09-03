# flyxy_clone

Une application mobile qui affiche les **prochains passages de transports en
commun autour de ta position GPS**, en Île-de-France.

Flutter pour l'application, Go/Gin pour le serveur, l'API **PRIM**
(Île-de-France Mobilités) comme source de données temps réel.

---

## Ce que ce dossier est vraiment

Ce n'est pas un tutoriel « tape ce code ». C'est un **projet d'architecture**.

L'objectif n'est pas d'avoir une application qui marche le plus vite possible —
n'importe qui peut coller trois appels HTTP dans un widget Flutter et voir des
horaires s'afficher. L'objectif est de savoir **où va chaque morceau de code, et
pourquoi**, pour que le projet tienne encore debout au dixième écran.

On conçoit d'abord. On code ensuite.

> **État actuel : phase de conception.** Aucun code d'implémentation n'est
> écrit. Les extraits de code dans les documents sont des **illustrations** de
> forme, pas des fichiers à recopier.

## Le parcours

Les documents se lisent **dans l'ordre**. Chacun suppose le précédent.

| # | Document | Ce que tu y apprends |
|---|---|---|
| 0 | [00_le_probleme.md](docs/00_le_probleme.md) | Le domaine métier, avant toute technique |
| 1 | [01_architecture_generale.md](docs/01_architecture_generale.md) | Pourquoi un backend, où passe la frontière |
| 2 | [02_backend_go_architecture.md](docs/02_backend_go_architecture.md) | Hexagonal en Go : domaine, ports, adaptateurs |
| 3 | [03_clean_code_go.md](docs/03_clean_code_go.md) | Ce que « propre » veut dire en Go, concrètement |
| 4 | [04_flutter_architecture.md](docs/04_flutter_architecture.md) | Couches, état, et pourquoi pas tout dans le widget |
| 5 | [05_contrat_api.md](docs/05_contrat_api.md) | Le contrat entre les deux, conçu avant le code |
| 6 | [06_feuille_de_route.md](docs/06_feuille_de_route.md) | Dans quel ordre construire, et quoi couper |

Les décisions d'architecture importantes sont tracées dans [docs/adr/](docs/adr/).

## Le principe directeur

Une seule idée traverse tous ces documents, et si tu n'en retiens qu'une, que ce
soit celle-là :

> **Le cœur du logiciel ne doit rien savoir du monde extérieur.**

Ni de PRIM, ni de HTTP, ni de Flutter, ni de la base de données. Ce sont des
**détails remplaçables**. Le cœur, lui, connaît des arrêts, des lignes et des
passages — des choses qui existeraient encore si PRIM fermait demain.

Tout le reste — la structure des dossiers, les interfaces, les tests — n'est que
la mise en œuvre mécanique de cette phrase.

## Rattachement au cours

Ce projet met en pratique les leçons de `mobile_dev/` :

- [backend/rest_api/](../../backend/rest_api/) — les fondations REST du serveur
- [cross_platform/](../../cross_platform/) — pourquoi Flutter plutôt que natif
- [backend/web_socket_api/](../../backend/web_socket_api/) — pour plus tard, si
  on veut du vrai temps réel poussé

## Pré-requis techniques

Déjà installés sur cette machine :

- Go 1.26
- Flutter (canal stable)
- Une clé API PRIM

La clé **ne sera jamais commitée** ni embarquée dans l'application mobile. Le
document [01_architecture_generale.md](docs/01_architecture_generale.md) explique
pourquoi c'est une raison d'architecture, pas seulement d'hygiène.
