# Exercices — Découvrez les réseaux informatiques

Support : [decouvrez_les_reseaux_informatiques.md](../lessons/decouvrez_les_reseaux_informatiques.md)

Ces exercices se font dans un terminal, sur votre propre réseau. Aucune
programmation.

**Toutes les commandes de ce fichier sont en lecture seule.** Elles observent,
elles ne modifient rien — ni votre machine, ni le réseau, ni les serveurs
interrogés.

**Une règle à respecter :** n'exécutez ces commandes que sur **votre** réseau et
vers des serveurs publics conçus pour être interrogés (`example.com`,
`wikipedia.org`). Scanner un réseau ou un serveur qui ne vous appartient pas est
illégal dans la plupart des pays, même sans intention de nuire.

---

# Niveau 1 — Votre configuration

## 1.1 — Votre adresse IP

```sh
ifconfig            # macOS
ip addr             # Linux
```

La sortie est dense. Cherchez la ligne commençant par `inet` (sans le `6`), sur
l'interface active — `en0` en général sur macOS, `wlan0` ou `eth0` sur Linux.

Notez :

- votre adresse IPv4
- votre masque de sous-réseau (`netmask`)
- votre adresse MAC (`ether`)

Votre adresse commence-t-elle par `192.168`, `10.` ou `172.16` à `172.31` ?
Qu'est-ce que cela vous apprend ?

## 1.2 — Publique ou privée

```sh
curl ifconfig.me
```

Cette adresse est différente de celle du 1.1. Expliquez pourquoi.

Laquelle voit le serveur que vous visitez ? Laquelle est partagée avec les autres
appareils de votre logement ?

Faites le test depuis un téléphone connecté au même Wi-Fi (un moteur de recherche
avec « quelle est mon IP » suffit). Obtenez-vous la même adresse publique ? La
même adresse locale ?

## 1.3 — Votre passerelle

```sh
netstat -nr | grep default     # macOS
ip route | grep default        # Linux
```

C'est l'adresse de votre box.

Comparez-la à votre adresse locale : les premiers nombres sont-ils identiques ?
Pourquoi est-ce nécessaire ?

## 1.4 — Local ou distant

Votre machine se pose cette question avant chaque envoi. Faites-le à sa place.

Supposons : adresse `192.168.1.42`, masque `255.255.255.0` (soit `/24`).

Pour chacune de ces destinations, dites si elle est sur le réseau local ou non :

- `192.168.1.1`
- `192.168.1.200`
- `192.168.2.5`
- `10.0.0.3`
- `185.15.58.224`

*Méthode :* avec un masque `/24`, les **trois premiers** nombres doivent être
identiques.

Refaites l'exercice avec votre vraie adresse et votre vrai masque.

## 1.5 — Votre adresse MAC

Reprenez l'adresse MAC du 1.1. Les trois premiers octets identifient le
fabricant.

Cherchez ces trois octets sur un annuaire OUI en ligne (« OUI lookup »). Le
fabricant correspond-il à votre machine ?

Pourquoi une adresse MAC ne permet-elle pas de router un paquet vers l'autre bout
du monde, alors qu'elle est unique ?

## 1.6 — IPv6

Dans la sortie du 1.1, cherchez les lignes `inet6`.

En avez-vous ? Combien ? Comparez leur longueur aux adresses IPv4.

Testez si vous sortez en IPv6 :

```sh
curl -6 ifconfig.me
```

Si la commande échoue, votre fournisseur ne vous en donne pas encore. C'est
courant.

---

# Niveau 2 — Mesurer le réseau

## 2.1 — Le temps d'aller-retour

```sh
ping -c 5 wikipedia.org
```

Notez le temps moyen (`avg`).

Testez maintenant des destinations de plus en plus lointaines :

```sh
ping -c 5 192.168.1.1              # votre box (adaptez l'adresse)
ping -c 5 wikipedia.org
ping -c 5 www.u-tokyo.ac.jp        # Tokyo
ping -c 5 www.usyd.edu.au          # Sydney
```

Complétez :

| Destination | Distance approximative | Temps mesuré |
|---|---|---|
| Votre box | ~5 m | |
| Wikipedia | | |
| Tokyo | ~9 700 km | |
| Sydney | ~17 000 km | |

## 2.2 — La limite physique

La lumière parcourt 300 000 km/s dans le vide, environ **200 000 km/s** dans une
fibre optique.

Calculez le temps **minimum théorique** d'un aller-retour Paris–Tokyo
(9 700 km × 2).

Comparez à votre mesure. L'écart s'explique par les routeurs traversés et les
détours du câblage.

**La question qui compte :** pouvez-vous, avec plus d'argent, descendre en
dessous du minimum théorique ? Que peut-on faire alors pour qu'un site soit
rapide à Tokyo ?

## 2.3 — Suivre le chemin

```sh
traceroute wikipedia.org
```

Chaque ligne est un routeur.

- Combien de sauts au total ?
- À quel saut sortez-vous de votre réseau local ?
- Repérez le moment où le temps augmente d'un coup : que se passe-t-il là ?

Certaines lignes affichent `* * *`. Cela ne signifie pas une panne : c'est un
routeur configuré pour ne pas répondre à ce type de sonde.

## 2.4 — Deux chemins

```sh
traceroute wikipedia.org
traceroute www.u-tokyo.ac.jp
```

Les premiers sauts sont-ils identiques ? À partir d'où les chemins divergent-ils ?

Que vous apprend le fait que les premiers sauts soient communs ?

## 2.5 — Le chemin change

Relancez `traceroute wikipedia.org` trois fois, à quelques minutes d'intervalle.

Le chemin est-il exactement le même à chaque fois ?

Reliez votre observation à cette phrase de la leçon : *« aucune machine ne
connaît le chemin complet »*.

## 2.6 — Les pertes

```sh
ping -c 100 wikipedia.org
```

Regardez la ligne de statistiques : quel pourcentage de paquets perdus ?

Sur une connexion filaire, vous devriez être à 0 %. En Wi-Fi, quelques pertes
sont normales.

Si un paquet se perd, qui s'en aperçoit ? Le protocole IP ? Et qui le
retransmet ?

---

# Niveau 3 — Les noms et le DNS

## 3.1 — Résoudre un nom

```sh
dig wikipedia.org
```

Cherchez la section `ANSWER SECTION`. Quelle adresse ?

Version courte :

```sh
dig +short wikipedia.org
```

## 3.2 — Plusieurs adresses

```sh
dig +short google.com
dig +short github.com
```

Certains noms renvoient **plusieurs** adresses. Pourquoi un site aurait-il besoin
de plusieurs adresses IP ?

Relancez la commande plusieurs fois : l'ordre change-t-il ?

## 3.3 — Le cache

```sh
dig wikipedia.org | grep "Query time"
dig wikipedia.org | grep "Query time"
```

La seconde est plus rapide. Où la réponse a-t-elle été trouvée la seconde fois ?

Cherchez aussi la valeur `TTL` dans la sortie complète (le nombre avant `IN A`).
C'est la durée de validité du cache, en secondes. Combien de temps ?

## 3.4 — La hiérarchie

```sh
dig +trace wikipedia.org
```

La sortie est longue. Elle montre la résolution **étape par étape** :

- les serveurs racine
- les serveurs de `.org`
- les serveurs de `wikipedia.org`

Combien d'étapes ? Reliez cela au schéma de la leçon.

## 3.5 — Le sens inverse

```sh
dig -x 185.15.58.224
```

On part d'une adresse pour retrouver un nom. Cela s'appelle une résolution
inverse.

Testez avec votre adresse publique. Obtenez-vous un nom ? À quoi ressemble-t-il ?

## 3.6 — Quand le DNS tombe

Vous ne pouvez plus accéder à aucun site, mais cette commande fonctionne :

```sh
ping -c 2 185.15.58.224
```

Que pouvez-vous en déduire ? Le réseau est-il en panne ? Les serveurs
sont-ils en panne ?

Formulez le diagnostic en une phrase. C'est un raisonnement de dépannage
classique : **isoler la couche fautive**.

---

# Niveau 4 — Les protocoles

## 4.1 — Une requête HTTP en direct

```sh
curl -v http://example.com
```

Les lignes commençant par `>` sont votre requête, celles commençant par `<` la
réponse.

Recopiez la première ligne de la requête et la première ligne de la réponse.

Quel code de statut ? Que signifie-t-il ?

## 4.2 — Les codes de statut

```sh
curl -s -o /dev/null -w "%{http_code}\n" http://example.com
curl -s -o /dev/null -w "%{http_code}\n" http://example.com/page-inexistante
curl -s -o /dev/null -w "%{http_code}\n" http://httpbin.org/status/500
```

Trois codes différents. Pour chacun : est-ce le client ou le serveur qui a un
problème ?

Un `404` est-il une panne du serveur ? Justifiez.

## 4.3 — Les en-têtes seuls

```sh
curl -I https://wikipedia.org
```

Le `-I` ne demande que les en-têtes, pas le contenu.

Repérez :

- le code de statut
- le serveur utilisé (`Server`)
- le type de contenu (`Content-Type`)

Le `Content-Type` mentionne un `charset`. Lequel ? Reliez-le à ce que vous avez
vu sur UTF-8 dans la leçon précédente.

## 4.4 — Une redirection

```sh
curl -I http://wikipedia.org
```

Vous obtenez probablement un `301`. Cherchez l'en-tête `Location`.

Que vous dit le serveur ? Vers quoi redirige-t-il — et surtout, de `http` vers
quoi ?

Suivez la redirection :

```sh
curl -IL http://wikipedia.org
```

Combien de réponses recevez-vous maintenant ?

## 4.5 — HTTP à la main

Parlez le protocole directement, sans navigateur :

```sh
{ printf 'GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n'; sleep 3; } | nc example.com 80
```

Vous venez d'écrire une requête HTTP caractère par caractère et de recevoir la
réponse.

*Pourquoi le `sleep 3` ?* Sans lui, `nc` ferme la connexion dès que son entrée
est épuisée — c'est-à-dire immédiatement après l'envoi, avant que le serveur
n'ait eu le temps de répondre. Le `sleep` maintient la connexion ouverte le temps
de recevoir. Retirez-le pour constater : vous n'obtiendrez rien.

C'est une illustration directe de la couche transport : une connexion TCP a deux
sens, et fermer le sien trop tôt coupe aussi la réponse.

*Les `\r\n` ne sont pas décoratifs :* HTTP impose ce couple retour-chariot +
saut de ligne comme fin de ligne, et une ligne vide (`\r\n\r\n`) marque la fin
des en-têtes. Un simple `\n` ne suffit pas toujours.

Que se passe-t-il si vous omettez la ligne `Host:` ? Essayez, et expliquez
pourquoi elle est obligatoire. *(Indice : plusieurs sites peuvent partager la
même adresse IP.)*

## 4.6 — HTTP contre HTTPS

```sh
curl -v http://example.com  2>&1 | head -20
curl -v https://example.com 2>&1 | head -30
```

La version HTTPS ajoute des lignes sur `TLS`, un certificat, une négociation.

Trouvez le nom de l'autorité qui a délivré le certificat.

**La question qui compte :** un site frauduleux peut-il avoir un cadenas vert ?
Que garantit exactement HTTPS, et que ne garantit-il pas ?

## 4.7 — Les ports

```sh
curl -I https://example.com:443     # port explicite
curl -I https://example.com         # port implicite
```

Même résultat. Pourquoi ?

Que se passe-t-il avec un mauvais port ?

```sh
curl --max-time 5 http://example.com:8080
```

Expliquez le message d'erreur.

## 4.8 — Vos connexions ouvertes

```sh
netstat -an | grep ESTABLISHED | head -20
```

Chaque ligne est une conversation TCP en cours.

Repérez la colonne des adresses distantes et leurs ports. Combien se terminent
par `.443` ? Que cela indique-t-il ?

Y a-t-il plus de connexions que de programmes ouverts ? Pourquoi une seule page
web ouvre-t-elle plusieurs connexions ?

---

# Niveau 5 — Raisonner

Questions de compréhension. Répondez par écrit, en quelques phrases.

## 5.1 — Pourquoi des couches

Un ingénieur propose de supprimer les couches et d'écrire un seul gros programme
gérant tout, du signal électrique jusqu'à l'affichage. Il argue que ce serait
plus rapide, sans en-têtes intermédiaires.

Donnez trois raisons de refuser.

## 5.2 — MAC et IP

Pourquoi deux systèmes d'adressage ? Une seule adresse ne suffirait-elle pas ?

Précisément : qu'est-ce que l'adresse IP permet et que l'adresse MAC ne permet
pas ?

## 5.3 — Le choix de ne rien garantir

IP ne garantit ni la livraison, ni l'ordre, ni l'unicité.

Expliquez pourquoi c'est un **choix de conception** et non un défaut.

Que faudrait-il stocker dans chaque routeur pour garantir la fiabilité à ce
niveau ? Pourquoi cela ne passerait-il pas à l'échelle ?

## 5.4 — TCP ou UDP

Pour chaque usage, choisissez et **justifiez en une phrase** :

| Usage | TCP ou UDP ? | Pourquoi |
|---|---|---|
| Télécharger un fichier | | |
| Appel visio | | |
| Envoyer un mail | | |
| Jeu en ligne rapide | | |
| Requête DNS | | |
| Diffusion vidéo en direct | | |
| Transaction bancaire | | |

Le critère unique : **une donnée arrivée en retard a-t-elle encore de la
valeur ?**

## 5.5 — Les trois échanges

L'établissement TCP demande trois messages : SYN, SYN-ACK, ACK.

Pourquoi pas deux ? Que ne saurait-on pas si le troisième n'existait pas ?

## 5.6 — Diagnostiquer une panne

Pour chaque symptôme, dites quelle couche est probablement en cause et **quelle
commande** le confirmerait :

1. Aucun site ne s'ouvre, mais `ping 1.1.1.1` fonctionne.
2. `ping` vers votre box échoue.
3. Un seul site est inaccessible, les autres fonctionnent.
4. Tout est très lent, mais tout fonctionne.
5. Le navigateur affiche un avertissement de certificat.

C'est la démarche du dépannage réseau : partir du bas de la pile et remonter.

## 5.7 — Le NAT

Trois appareils chez vous, une seule adresse publique. Tous consultent le même
site en même temps.

Comment la box sait-elle à quel appareil renvoyer chaque réponse ?

*Indice :* elle utilise quelque chose de la couche transport.

Puis : pourquoi est-il difficile d'héberger un serveur derrière un NAT ?

## 5.8 — La géographie compte

Un site hébergé uniquement à Paris est lent depuis Tokyo, quelle que soit la
puissance du serveur.

Expliquez pourquoi le problème n'est pas le serveur.

Quelle est la seule solution ? Comment s'appelle ce dispositif ? *(Cherchez
« CDN ».)*

---

# Niveau 6 — Observer plus loin

## 6.1 — Le débit contre la latence

Deux notions souvent confondues.

- Le **débit** est la quantité de données par seconde.
- La **latence** est le temps du premier octet.

Un camion rempli de disques durs traversant la France a un débit énorme et une
latence catastrophique.

Pour chaque usage, dites lequel compte le plus :

- télécharger un film de 10 Go
- jouer à un jeu de tir en ligne
- une visioconférence
- sauvegarder des photos dans le cloud

## 6.2 — Mesurer votre débit

```sh
curl -o /dev/null -w "debit : %{speed_download} octets/s\n" \
  https://speed.cloudflare.com/__down?bytes=25000000
```

Convertissez en Mo/s puis en Mbit/s (× 8).

Comparez au débit annoncé par votre abonnement. L'écart est normal — expliquez
d'où il vient.

## 6.3 — Un serveur en une ligne

Dans un terminal :

```sh
python3 -m http.server 8000
```

Dans un autre :

```sh
curl -v http://localhost:8000
```

Vous venez de faire tourner un serveur web et de l'interroger.

Regardez le premier terminal : il journalise votre requête. Quelle méthode ? Quel
code de statut ?

Ouvrez `http://localhost:8000` dans un navigateur. Combien de requêtes voyez-vous
apparaître pour **une** page ?

Arrêtez avec `Ctrl+C`.

## 6.4 — localhost

Dans l'exercice précédent, vous avez utilisé `localhost`.

```sh
ping -c 2 localhost
dig +short localhost
grep localhost /etc/hosts
```

Quelle adresse ? Le temps d'aller-retour vous surprend-il ?

`dig` ne renvoie probablement **rien**, alors que `ping` fonctionne. Ce n'est pas
une erreur : `localhost` n'est pas résolu par le DNS mais par le fichier
`/etc/hosts`, consulté **avant** lui. Que contient ce fichier ?

Cela vous apprend que la résolution de noms ne passe pas toujours par le réseau.

Ce trafic sort-il de votre machine ? Passe-t-il par votre carte réseau ?

Que se passerait-il si vous débranchiez le câble réseau ? Testez, si vous le
pouvez.

## 6.5 — Ce que voit un site

```sh
curl -s https://httpbin.org/headers
```

Le serveur vous renvoie les en-têtes que vous lui avez envoyés.

Quelles informations transmettez-vous **sans le vouloir** ?

Comparez avec ce qu'envoie un navigateur (ouvrez la même URL dans votre
navigateur). Lequel en dit le plus sur vous ?

## 6.6 — Le voyage complet, décrit

Rédigez, avec vos mots, tout ce qui se passe entre le moment où vous tapez
`https://wikipedia.org` et celui où la page s'affiche.

Contraintes :

- au moins dix étapes
- mentionner : DNS, TCP, TLS, HTTP, IP, routage
- préciser la couche concernée à chaque étape

C'est l'exercice de synthèse de la leçon. Si vous y parvenez sans relire, elle
est acquise.

---

# Auto-évaluation

- [ ] Pourquoi organise-t-on un réseau en couches ?
- [ ] Quelle différence entre une adresse MAC et une adresse IP ?
- [ ] À quoi sert le masque de sous-réseau ? Quelle question permet-il de
      trancher ?
- [ ] Pourquoi IP ne garantit-il rien ? Est-ce un défaut ?
- [ ] Quand choisir UDP plutôt que TCP ?
- [ ] À quoi sert un port ?
- [ ] Que fait le DNS, et que se passe-t-il quand il tombe ?
- [ ] Que garantit HTTPS ? Que ne garantit-il pas ?
- [ ] Que signifie un code 404 ? Un 500 ? Qui est en cause dans chaque cas ?
- [ ] Pourquoi un site à Paris est-il lent depuis Tokyo, même sur un serveur
      puissant ?

---

# Solutions

Aucune n'est donnée — volontairement.

Trois recours dans l'ordre :

1. **Relire la leçon.** Chaque exercice correspond à une section précise.
2. **Exécuter la commande.** La particularité de cette leçon est que presque
   toutes les réponses s'observent directement. Une hypothèse se vérifie en une
   commande.
3. **Isoler la couche.** Devant un comportement incompris, remontez la pile :
   la liaison fonctionne-t-elle ? l'IP ? le DNS ? l'application ? C'est la
   méthode de dépannage réseau, et elle marche aussi pour comprendre.

Sur cette leçon : les questions des niveaux 5 et 6 n'ont pas de réponse unique.
Ce qui compte est le raisonnement.
