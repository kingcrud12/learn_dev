# Backend pour applications mobiles

Une application mobile ne vit pas seule : elle affiche des données qui viennent
d'ailleurs. Ce dossier couvre les quatre façons d'organiser ce dialogue.

> **Prérequis.** Ces leçons sont en avance sur le parcours principal. Elles
> supposent HTTP, JSON et un langage de haut niveau. Elles sont écrites pour être
> **lues** maintenant et **pratiquées** plus tard — sauf les sections `curl` de
> REST, faisables immédiatement dans ton terminal.

---

## Les quatre approches

| Dossier | Ce que c'est | À retenir |
|---|---|---|
| [rest_api/](rest_api/) | Le standard universel | **Commence ici** — les trois autres s'expliquent par rapport à lui |
| [web_socket_api/](web_socket_api/) | Connexion permanente, bidirectionnelle | Quand le serveur doit parler en premier |
| [grpc_api/](grpc_api/) | Format binaire, contrat strict | Performance et économie de bande passante |
| [firebase/](firebase/) | Backend clé en main | Ne pas écrire de serveur du tout |

## Dans quel ordre

**REST d'abord**, sans hésiter. C'est le socle : les autres technologies se
définissent par ce qu'elles corrigent chez lui.

- WebSocket existe parce que REST ne permet pas au serveur de parler spontanément
- gRPC existe parce que JSON est verbeux et le contrat REST informel
- Firebase existe parce qu'écrire et administrer un backend coûte cher

Lire les trois autres sans connaître REST, c'est lire des réponses sans connaître
la question.

## Le tableau de décision

| Besoin | Choix |
|---|---|
| Lire et écrire des données classiques | **REST** |
| Chat, notifications, temps réel | **WebSocket** |
| Beaucoup de données, réseau contraint | **gRPC** |
| Prototype rapide, pas d'équipe backend | **Firebase** |

Une application réelle en combine plusieurs : REST pour l'essentiel, WebSocket
pour le temps réel, et des notifications push pour réveiller l'application quand
elle est fermée.

## Ce qui revient partout

Quatre thèmes traversent les quatre dossiers, et ce sont eux qui comptent
vraiment :

**Le contrat.** Client et serveur ne partagent aucun code. Ce qui les lie est une
convention — explicite avec gRPC, documentaire avec REST. C'est la même idée que
les headers en C : `stdio.h` déclare `printf` sans contenir son code.

**L'authentification.** Prouver qui tu es à chaque requête, sans que le serveur ne
se souvienne de rien.

**Le réseau échoue.** Un tunnel, un ascenseur, une batterie faible. La conception
doit supposer l'échec permanent, pas l'exception.

**La sécurité côté serveur.** Une application mobile est téléchargeable et
décompilable : rien de ce qu'elle contient n'est secret, et aucune vérification
qu'elle fait n'est fiable.

## Structure

Chaque dossier suit le format du projet :

```
technologie/
├── lessons/      la leçon
└── exercices/    les exercices, sans solutions
```
