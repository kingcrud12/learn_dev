# 5. Le contrat d'API

Ce que les deux côtés se promettent — conçu avant le code, pas déduit après.

---

## 1. Pourquoi le contrat vient en premier

Le contrat est le seul point où le monde Go et le monde Dart se touchent. Tout
le reste de chaque côté est libre de changer ; **ça, non**.

L'erreur courante est de le laisser émerger : on code le serveur, on regarde ce
qu'il crache, on écrit le client d'après. Le contrat devient alors le **reflet
accidentel** des structures internes du serveur — et le jour où tu renommes un
champ Go, l'application mobile casse.

En le concevant d'abord :

- les deux côtés avancent **en parallèle** ;
- le client peut se développer contre une réponse d'exemple, avant même que le
  serveur existe ;
- le contrat exprime **le besoin de l'écran**, pas la forme du code serveur.

> **Un contrat se conçoit du point de vue de celui qui consomme.** Pars de
> l'écran à peindre, remonte vers la donnée. Jamais l'inverse.

## 2. La surface, volontairement minuscule

Un seul point d'entrée métier, plus un de santé.

```
GET /v1/departures?lat=..&lon=..&radius=..    les passages autour d'un point
GET /health                                    le serveur est-il vivant
```

C'est tout. Pas de `/stops`, pas de `/lines`, pas de `/departures/{id}`.

**Pourquoi si peu ?** Parce que le document 1 l'a décidé : *un écran, une
requête*. Ajouter des points d'entrée « parce qu'ils pourraient servir » crée du
code à maintenir, à documenter et à tester, pour un besoin qui n'existe pas.

> **Une API se conçoit à partir des écrans, pas à partir des tables de données.**

## 3. La requête

```
GET /v1/departures?lat=48.8566&lon=2.3522&radius=400&modes=metro,bus&limit=3
```

| Paramètre | Type | Obligatoire | Défaut | Contrainte |
|---|---|---|---|---|
| `lat` | float | ✅ | — | 48.0 – 49.3 (Île-de-France) |
| `lon` | float | ✅ | — | 1.4 – 3.6 |
| `radius` | entier (m) | | `400` | 50 – 1000 |
| `modes` | liste | | tous | `bus,metro,tram,rer,train` |
| `limit` | entier | | `3` | 1 – 10, passages par ligne |

### Pourquoi ces bornes

Chaque borne est une **décision de produit**, pas une précaution technique :

- **`radius` plafonné à 1000 m.** Au-delà, la réponse contient des dizaines
  d'arrêts, l'écran devient illisible et le serveur fait 200 appels à PRIM.
  La limite protège l'utilisateur autant que le serveur.
- **`lat`/`lon` bornés à l'Île-de-France.** Une requête depuis Marseille ne peut
  rien donner : autant répondre immédiatement et clairement, plutôt que de
  parcourir le référentiel pour rendre une liste vide.
- **`limit` par défaut à 3.** Personne ne lit le dixième passage d'un bus.

> **Valider tôt, refuser clairement.** Une entrée invalide doit être rejetée à la
> frontière, avec un message qui dit *quoi* et *pourquoi* — jamais propagée dans
> le domaine.

## 4. La réponse

```json
{
  "generated_at": "2026-09-03T18:42:11+02:00",
  "query": { "lat": 48.8566, "lon": 2.3522, "radius": 400 },
  "stops": [
    {
      "id": "IDFM:463158",
      "name": "Châtelet",
      "distance_meters": 87,
      "position": { "lat": 48.8583, "lon": 2.3470 },
      "departures": [
        {
          "line": {
            "id": "IDFM:C01371",
            "name": "4",
            "mode": "metro",
            "color": "#BE418D",
            "text_color": "#FFFFFF"
          },
          "destination": "Porte de Clignancourt",
          "expected_time": "2026-09-03T18:44:00+02:00",
          "realtime": true
        }
      ]
    }
  ]
}
```

### Chaque choix, justifié

**Un objet à la racine, pas un tableau.** Un tableau nu ne peut jamais accueillir
de métadonnée. En enveloppant, on peut ajouter demain un champ `warnings` ou
`next_refresh` sans casser personne.

**`generated_at`.** Le client sait à quel instant les données ont été produites.
Il peut afficher « il y a 12 s » et décider de rafraîchir. Sans ce champ, il ne
peut que deviner.

**`distance_meters`, pas `distance`.** L'unité dans le nom (document 3). Un
entier : le mètre au centimètre près n'a aucun sens pour de la marche à pied.

**`expected_time` en ISO 8601 avec fuseau.** Le point du document 1 : le serveur
envoie un **fait**, le client produit le rendu. Le fuseau explicite évite toute
ambiguïté au changement d'heure.

**`realtime: true|false`.** Honnêteté envers l'utilisateur. Un horaire théorique
et un horaire temps réel n'ont pas la même valeur, et l'interface peut le
signaler discrètement.

**Les couleurs de ligne fournies par le serveur.** Elles font partie de
l'identité officielle des lignes : ce sont des **données**, pas des choix de
design. Les mettre côté serveur évite de publier une version mobile à chaque
nouvelle ligne.

**`stops` déjà trié par distance, `departures` par heure.** Le serveur décide
(document 1). Le client n'a rien à retrier.

### Ce que la réponse ne contient pas

| Absent | Pourquoi |
|---|---|
| `"dans 3 min"` | Faux dès la seconde suivante — c'est un rendu, pas un fait |
| Les identifiants de quais | Détail interne du domaine, inutile au client |
| La forme SIRI de PRIM | La membrane du document 2 ne laisse rien passer |
| Les arrêts sans aucun passage | Une carte vide n'apporte rien à l'écran |

## 5. Les erreurs

Une forme unique, pour que le client n'ait **qu'un seul décodeur d'erreur** :

```json
{
  "error": {
    "code": "invalid_position",
    "message": "La position doit se situer en Île-de-France."
  }
}
```

| Code HTTP | `code` | Quand | Le client fait |
|---|---|---|---|
| `400` | `invalid_parameter` | `radius` hors bornes, `lat` absente | Corrige la requête (bug client) |
| `400` | `invalid_position` | Hors Île-de-France | Message « zone non couverte » |
| `503` | `realtime_unavailable` | PRIM injoignable | Propose de réessayer |
| `500` | `internal` | Bug serveur | Message générique |

**Le champ `code` est pour la machine, `message` pour l'humain.** Le client
branche sa logique sur `code` — jamais sur le texte de `message`, qui doit
pouvoir être reformulé ou traduit sans rien casser.

### Le cas « aucun arrêt » n'est pas une erreur

```json
{ "generated_at": "...", "query": {...}, "stops": [] }
```

Code **200**, liste vide. La question « qu'y a-t-il autour de moi ? » a reçu une
réponse valide : *rien*. C'est le `NearbyEmpty` du document 4, et l'application
affiche un écran calme, pas une alerte rouge.

> **Distinguer « pas de résultat » de « échec » est une des marques d'une API
> bien conçue.** Confondre les deux fait afficher des erreurs à des utilisateurs
> à qui il ne s'est rien passé d'anormal.

## 6. Le versionnage : pourquoi `/v1/`

Trois caractères qui coûtent zéro aujourd'hui et sauvent le projet plus tard.

Une application mobile n'est **pas** un site web : tu ne contrôles pas la version
installée chez tes utilisateurs. Certains garderont la v1 pendant deux ans, et
tu ne peux rien y faire.

Quand tu devras changer la forme de la réponse, `/v2/departures` coexistera avec
`/v1/` le temps que le parc se renouvelle. Sans préfixe de version, tu as deux
options, toutes deux mauvaises : casser les anciennes applications, ou ne jamais
faire évoluer l'API.

### Ce qui casse et ce qui ne casse pas

| Changement | Compatible ? |
|---|---|
| Ajouter un champ à la réponse | ✅ Un client bien écrit ignore ce qu'il ne connaît pas |
| Ajouter un paramètre optionnel | ✅ |
| Renommer un champ | ❌ Nouvelle version |
| Retirer un champ | ❌ Nouvelle version |
| Changer un type (`int` → `string`) | ❌ Nouvelle version |
| Rendre un paramètre obligatoire | ❌ Nouvelle version |

La règle qui en découle : **ajouter est gratuit, modifier coûte une version.**
Conçois donc en pensant à ce que tu ajouteras.

## 7. Le contrat comme document vivant

Le contrat doit exister **en un seul endroit**, sous une forme que les deux côtés
lisent :

- une **spécification OpenAPI** (`api/openapi.yaml`) — la référence ;
- une **réponse d'exemple** figée (`api/examples/departures.json`).

L'exemple figé a une vertu discrète mais considérable : il sert de **jeu de test
aux deux côtés**.

- Le test Go vérifie que le serveur produit exactement cette forme.
- Le test Dart vérifie que le client sait la décoder.

Le jour où quelqu'un change la forme sans y penser, **un test casse des deux
côtés**. Le contrat n'est plus une intention dans un document : c'est une
contrainte exécutable.

> **Un contrat qui n'est vérifié par aucun test n'est pas un contrat, c'est un
> souhait.**

## 8. Résumé

1. Le contrat se conçoit **avant** le code, du point de vue de **l'écran**.
2. **Un écran, une requête.** La surface reste minuscule.
3. Le serveur envoie des **faits** (heures absolues), pas des rendus.
4. Toute entrée est **validée et bornée** à la frontière, avec des bornes
   justifiées par le produit.
5. Les erreurs ont une **forme unique** : `code` pour la machine, `message` pour
   l'humain.
6. **Vide ≠ erreur.**
7. `/v1/` dès le premier jour : le parc mobile ne se met pas à jour sur commande.
8. Le contrat est **testé des deux côtés** à partir d'un exemple figé.

---

**Suite :** [06_feuille_de_route.md](06_feuille_de_route.md) — dans quel ordre
construire tout cela.
