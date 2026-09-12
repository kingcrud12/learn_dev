/* tableaux_motifs.c — les 4 motifs qui resolvent 80% des exos de tableaux.
 *
 * MODE D'EMPLOI
 *
 *   1. Remplace UN SEUL "TODO" par ton code.
 *   2. Compile et lance :
 *
 *        make tableaux_motifs && ./tableaux_motifs
 *
 *      ou, avec les sanitizers (VIVEMENT conseille, voir plus bas) :
 *
 *        gcc -Wall -Wextra -g -fsanitize=address,undefined \
 *            tableaux_motifs.c -o tm && ./tm
 *
 *   3. Le programme affiche [ok] ou [KO] pour chaque test, puis un score.
 *      Recommence jusqu'a ce que la fonction passe. Puis la suivante.
 *
 * POURQUOI LES SANITIZERS
 *
 *   En C, sortir d'un tableau ne plante pas : ca lit ou ecrit la memoire du
 *   voisin, en silence. Un algo faux peut donc afficher [ok] par chance.
 *   -fsanitize=address arrete le programme a la ligne exacte du debordement.
 *   C'est ton professeur : sans lui tu apprends de faux reflexes.
 *
 * L'ORDRE COMPTE
 *
 *   Les 12 fonctions sont classees par motif, du plus simple au plus riche.
 *   Ne saute pas : le motif 3 suppose le 2 automatise.
 *
 *   Motif 1  ACCUMULATEUR   une boucle, une variable qui grossit    (1-3)
 *   Motif 2  CHAMPION       une boucle, "le meilleur jusqu'ici"     (4-8)
 *   Motif 3  DEUX INDICES   i part du debut, j de la fin            (9-10)
 *   Motif 4  LECTURE/ECRIT. i lit, j ecrit, j <= i                  (11-12)
 *
 * LA SEULE QUESTION A TE POSER, A CHAQUE FOIS
 *
 *   « Qu'est-ce que je retiens d'un tour de boucle au suivant ? »
 *   Cette variable retenue, c'est l'etat. Tout le reste est mecanique.
 */

#include <stdio.h>
#include <string.h>


/* ==========================================================================
 * MOTIF 1 — L'ACCUMULATEUR
 *
 * Le squelette, une fois pour toutes :
 *
 *     TYPE acc = <valeur neutre>;        // 0 pour une somme, 1 pour un produit
 *     for (int i = 0; i < n; i++) {
 *         acc = acc <combine> tab[i];
 *     }
 *     return acc;
 *
 * Les 3 exercices qui suivent sont ce squelette. Rien d'autre.
 * ========================================================================== */

/* 1. La somme des n elements.
 *
 *    L'exercice de reference. Valeur neutre : 0.
 *    Si tu bloques ici, relis la section 5 de lessons/06_tableaux_et_chaines.md.
 */
int somme(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0;
}

/* 2. La moyenne des n elements.
 *
 *    PIEGE, et il est important : somme(tab, n) / n fait une division
 *    ENTIERE. 7 / 2 vaut 3, pas 3.5. Il faut un cast : (double)somme / n.
 *    Reutilise somme() — une fonction juste ecrite est une brique.
 *
 *    Cas limite : n == 0. Une division par zero est un comportement
 *    indefini. Le test attend 0.0 dans ce cas — protege-toi.
 */
double moyenne(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0.0;
}

/* 3. Combien d'elements sont pairs.
 *
 *    Meme squelette, mais l'accumulateur n'avance que SOUS CONDITION.
 *    Un nombre est pair si son reste modulo 2 est nul : (x % 2 == 0).
 */
int compte_pairs(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0;
}


/* ==========================================================================
 * MOTIF 2 — LE CHAMPION
 *
 * Le squelette :
 *
 *     TYPE best = tab[0];                // on part du 1er, pas d'un neutre
 *     for (int i = 1; i < n; i++) {      // ... donc on demarre a i = 1
 *         if (tab[i] <est meilleur que> best)
 *             best = tab[i];
 *     }
 *     return best;
 *
 * DEUX CHOSES A REMARQUER
 *
 *   - On initialise avec tab[0], pas avec 0. Un tableau de nombres tous
 *     negatifs aurait un maximum de 0, ce qui est faux.
 *   - La boucle part de i = 1 : tab[0] est deja le champion en titre.
 * ========================================================================== */

/* 4. Le plus grand element. */
int maximum(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0;
}

/* 5. Le plus petit element.
 *
 *    Exactement le 4, avec le signe de comparaison retourne. Ecris-le
 *    quand meme sans regarder : c'est la repetition qui cree le reflexe.
 */
int minimum(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0;
}

/* 6. La POSITION du plus grand element, pas sa valeur.
 *
 *    Le vrai saut de ce fichier. Deux facons de faire :
 *      a) garder best_val et best_idx, deux variables ;
 *      b) ne garder que best_idx, et comparer tab[i] > tab[best_idx].
 *
 *    La (b) est plus courte et ne peut pas desynchroniser. Prefere-la.
 *
 *    Retourner un indice au lieu d'une valeur, c'est ce qui rend une
 *    fonction reutilisable : l'exercice 12 s'appuiera sur celle-ci.
 *    En cas d'egalite, renvoie le PREMIER des maximums.
 */
int indice_du_max(const int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
    return 0;
}

/* 7. Le tableau contient-il cible ? Renvoie 1 si oui, 0 sinon.
 *
 *    Des que tu l'as trouve, return 1 tout de suite : inutile de finir la
 *    boucle. C'est la "sortie anticipee", et c'est un reflexe de perf.
 *    Le return 0 se met APRES la boucle : "j'ai tout vu, rien trouve".
 */
int contient(const int tab[], int n, int cible)
{
    /* TODO */
    (void)tab; (void)n; (void)cible;
    return 0;
}

/* 8. L'indice de la premiere occurrence de cible, ou -1 si absente.
 *
 *    LA fonction la plus formatrice du fichier. Le -1 est une convention
 *    universelle : "cette valeur n'existe pas". On ne peut pas renvoyer 0,
 *    qui est un indice valide.
 *
 *    Ce reflexe — un code de retour distinct pour le cas "rien" — est la
 *    frontiere entre ecrire du code et concevoir un algorithme.
 */
int indice_de(const int tab[], int n, int cible)
{
    /* TODO */
    (void)tab; (void)n; (void)cible;
    return -1;
}


/* ==========================================================================
 * MOTIF 3 — LES DEUX INDICES
 *
 * Le squelette :
 *
 *     int i = 0, j = n - 1;
 *     while (i < j) {
 *         ... on travaille sur tab[i] et tab[j] ...
 *         i++;
 *         j--;
 *     }
 *
 * Le motif qui fait passer de "je parcours" a "je manipule". La condition
 * est i < j, pas i <= j : quand i == j les deux indices designent la meme
 * case, il n'y a plus rien a faire.
 *
 * C'est aussi le coeur du tri a bulles et de la partition du quicksort.
 * Un seul motif, trois algorithmes.
 * ========================================================================== */

/* 9. Inverser le tableau SUR PLACE (sans tableau auxiliaire).
 *
 *    Echanger deux cases demande une variable temporaire :
 *        int tmp = tab[i];  tab[i] = tab[j];  tab[j] = tmp;
 *    Sans le tmp, la premiere affectation ecrase ce dont tu as besoin.
 *
 *    Noter que tab n'est PAS const ici : la fonction modifie le tableau de
 *    l'appelant. C'est ta lecon 07 en action — un tableau passe en argument
 *    devient un pointeur, l'appele ecrit dans la memoire de l'appelant.
 */
void inverser(int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
}

/* 10. La chaine se lit-elle pareil dans les deux sens ? 1 si oui, 0 sinon.
 *
 *     Meme motif, applique a une chaine. La longueur s'obtient avec
 *     strlen(s) de <string.h> — qui compte les caracteres AVANT le '\0'.
 *
 *     Des que deux caracteres differents se font face : return 0. Si les
 *     indices se croisent sans desaccord, c'est un palindrome : return 1.
 *
 *     Une chaine vide est un palindrome (rien ne la contredit).
 */
int est_palindrome(const char s[])
{
    /* TODO */
    (void)s;
    return 0;
}


/* ==========================================================================
 * MOTIF 4 — LECTURE / ECRITURE  (deux indices qui avancent dans le meme sens)
 *
 * Le squelette :
 *
 *     int j = 0;                             // ou j'ecris
 *     for (int i = 0; i < n; i++) {          // ou je lis
 *         if (<je garde tab[i]>) {
 *             tab[j] = tab[i];
 *             j++;
 *         }
 *     }
 *     return j;                              // la nouvelle taille
 *
 * L'INVARIANT, et c'est ce qui rend le motif correct :
 *
 *     j <= i, TOUJOURS.
 *
 * L'ecriture ne depasse jamais la lecture, donc on n'ecrase jamais une case
 * pas encore lue. C'est pour cela qu'on peut filtrer sur place, sans copie.
 *
 * Un tableau en C ne rapetissit pas : on renvoie la nouvelle taille, et les
 * cases au-dela de j deviennent des dechets qu'on ignore.
 * ========================================================================== */

/* 11. Supprimer toutes les occurrences de v. Renvoie la nouvelle taille.
 *
 *     Applique le squelette ci-dessus : on garde tab[i] quand tab[i] != v.
 */
int supprime_valeur(int tab[], int n, int v)
{
    /* TODO */
    (void)tab; (void)n; (void)v;
    return 0;
}

/* 12. Trier le tableau en ordre croissant, par selection.
 *
 *     L'ALGORITHME, en une phrase : trouve le plus grand des i+1 premiers
 *     elements, echange-le avec le dernier de cette zone, recommence sur
 *     une zone d'un cran plus courte.
 *
 *     for (int fin = n - 1; fin > 0; fin--) {
 *         int m = indice_du_max(tab, fin + 1);   // le max de tab[0..fin]
 *         echanger tab[m] et tab[fin];
 *     }
 *
 *     REUTILISE indice_du_max — c'est pour ca qu'on l'a ecrite. Voila le
 *     retour sur investissement d'une fonction qui renvoie un indice : elle
 *     se compose. Une qui renvoie une valeur ne l'aurait pas permis.
 *
 *     Complexite : n passes, chacune parcourant jusqu'a n cases -> O(n^2).
 *     Le module 3.projects_to_learn_algo te montrera comment tomber a
 *     O(n log n). Tu auras alors deja ecrit le point de comparaison.
 */
void tri_selection(int tab[], int n)
{
    /* TODO */
    (void)tab; (void)n;
}


/* ==========================================================================
 * LES TESTS — tu n'as rien a modifier en dessous de cette ligne.
 * ========================================================================== */

static int reussis = 0;
static int total   = 0;

static void verifie_int(const char *titre, int obtenu, int attendu)
{
    total++;
    if (obtenu == attendu) {
        reussis++;
        printf("  [ok] %s\n", titre);
    } else {
        printf("  [KO] %s : obtenu %d, attendu %d\n", titre, obtenu, attendu);
    }
}

static void verifie_double(const char *titre, double obtenu, double attendu)
{
    double ecart = obtenu - attendu;
    if (ecart < 0) ecart = -ecart;

    total++;
    if (ecart < 0.0001) {
        reussis++;
        printf("  [ok] %s\n", titre);
    } else {
        printf("  [KO] %s : obtenu %.4f, attendu %.4f\n", titre, obtenu, attendu);
    }
}

/* Compare les n premieres cases de deux tableaux. */
static void verifie_tab(const char *titre, const int obtenu[],
                        const int attendu[], int n)
{
    int i;

    total++;
    for (i = 0; i < n; i++) {
        if (obtenu[i] != attendu[i]) break;
    }

    if (i == n) {
        reussis++;
        printf("  [ok] %s\n", titre);
        return;
    }

    printf("  [KO] %s\n", titre);
    printf("        obtenu  :");
    for (i = 0; i < n; i++) printf(" %d", obtenu[i]);
    printf("\n        attendu :");
    for (i = 0; i < n; i++) printf(" %d", attendu[i]);
    printf("\n");
}

int main(void)
{
    printf("\n=== MOTIF 1 — L'ACCUMULATEUR ===\n\n");
    {
        int a[] = {3, 1, 4, 1, 5};              /* somme 14, 2 pairs */
        int negs[] = {-5, -2, -8};              /* somme -15 */
        int un[] = {7};

        printf("1. somme\n");
        verifie_int("somme({3,1,4,1,5}) == 14", somme(a, 5), 14);
        verifie_int("somme({-5,-2,-8}) == -15", somme(negs, 3), -15);
        verifie_int("somme(tableau vide) == 0", somme(a, 0), 0);

        printf("2. moyenne\n");
        verifie_double("moyenne({3,1,4,1,5}) == 2.8", moyenne(a, 5), 2.8);
        verifie_double("moyenne({7}) == 7.0", moyenne(un, 1), 7.0);
        verifie_double("moyenne(vide) == 0.0 (pas de div par 0)",
                       moyenne(a, 0), 0.0);

        printf("3. compte_pairs\n");
        verifie_int("compte_pairs({3,1,4,1,5}) == 1", compte_pairs(a, 5), 1);
        verifie_int("compte_pairs({-5,-2,-8}) == 2", compte_pairs(negs, 3), 2);
        verifie_int("compte_pairs(vide) == 0", compte_pairs(a, 0), 0);
    }

    printf("\n=== MOTIF 2 — LE CHAMPION ===\n\n");
    {
        int a[] = {3, 1, 4, 1, 5};
        int negs[] = {-5, -2, -8};              /* piege du max initialise a 0 */
        int un[] = {7};
        int ega[] = {2, 9, 9, 1};               /* deux maximums */

        printf("4. maximum\n");
        verifie_int("maximum({3,1,4,1,5}) == 5", maximum(a, 5), 5);
        verifie_int("maximum({-5,-2,-8}) == -2  <- tous negatifs !",
                    maximum(negs, 3), -2);
        verifie_int("maximum({7}) == 7", maximum(un, 1), 7);

        printf("5. minimum\n");
        verifie_int("minimum({3,1,4,1,5}) == 1", minimum(a, 5), 1);
        verifie_int("minimum({-5,-2,-8}) == -8", minimum(negs, 3), -8);
        verifie_int("minimum({7}) == 7", minimum(un, 1), 7);

        printf("6. indice_du_max\n");
        verifie_int("indice_du_max({3,1,4,1,5}) == 4", indice_du_max(a, 5), 4);
        verifie_int("indice_du_max({-5,-2,-8}) == 1", indice_du_max(negs, 3), 1);
        verifie_int("indice_du_max({2,9,9,1}) == 1  <- le premier max",
                    indice_du_max(ega, 4), 1);

        printf("7. contient\n");
        verifie_int("contient({3,1,4,1,5}, 4) == 1", contient(a, 5, 4), 1);
        verifie_int("contient({3,1,4,1,5}, 9) == 0", contient(a, 5, 9), 0);
        verifie_int("contient(vide, 3) == 0", contient(a, 0, 3), 0);

        printf("8. indice_de\n");
        verifie_int("indice_de({3,1,4,1,5}, 3) == 0  <- indice 0 valide",
                    indice_de(a, 5, 3), 0);
        verifie_int("indice_de({3,1,4,1,5}, 1) == 1  <- la 1re occurrence",
                    indice_de(a, 5, 1), 1);
        verifie_int("indice_de({3,1,4,1,5}, 9) == -1 <- absente",
                    indice_de(a, 5, 9), -1);
        verifie_int("indice_de(vide, 3) == -1", indice_de(a, 0, 3), -1);
    }

    printf("\n=== MOTIF 3 — LES DEUX INDICES ===\n\n");
    {
        printf("9. inverser\n");
        {
            int t[] = {1, 2, 3, 4, 5};          /* taille impaire : pivot fixe */
            int att[] = {5, 4, 3, 2, 1};
            inverser(t, 5);
            verifie_tab("inverser({1,2,3,4,5}) (taille impaire)", t, att, 5);
        }
        {
            int t[] = {1, 2, 3, 4};             /* taille paire */
            int att[] = {4, 3, 2, 1};
            inverser(t, 4);
            verifie_tab("inverser({1,2,3,4}) (taille paire)", t, att, 4);
        }
        {
            int t[] = {7};
            int att[] = {7};
            inverser(t, 1);
            verifie_tab("inverser({7}) inchange", t, att, 1);
        }

        printf("10. est_palindrome\n");
        verifie_int("est_palindrome(\"radar\") == 1", est_palindrome("radar"), 1);
        verifie_int("est_palindrome(\"ressasser\") == 1",
                    est_palindrome("ressasser"), 1);
        verifie_int("est_palindrome(\"bonjour\") == 0",
                    est_palindrome("bonjour"), 0);
        verifie_int("est_palindrome(\"ab\") == 0", est_palindrome("ab"), 0);
        verifie_int("est_palindrome(\"a\") == 1", est_palindrome("a"), 1);
        verifie_int("est_palindrome(\"\") == 1  <- rien ne la contredit",
                    est_palindrome(""), 1);
    }

    printf("\n=== MOTIF 4 — LECTURE / ECRITURE ===\n\n");
    {
        printf("11. supprime_valeur\n");
        {
            int t[] = {3, 1, 4, 1, 5};
            int att[] = {3, 4, 5};
            int m = supprime_valeur(t, 5, 1);
            verifie_int("supprime_valeur({3,1,4,1,5}, 1) renvoie 3", m, 3);
            verifie_tab("  ... et laisse {3,4,5} en tete", t, att, 3);
        }
        {
            int t[] = {2, 2, 2};
            int m = supprime_valeur(t, 3, 2);
            verifie_int("supprime_valeur({2,2,2}, 2) renvoie 0 <- tout enleve",
                        m, 0);
        }
        {
            int t[] = {3, 1, 4};
            int att[] = {3, 1, 4};
            int m = supprime_valeur(t, 3, 9);
            verifie_int("supprime_valeur({3,1,4}, 9) renvoie 3 <- rien a "
                        "enlever", m, 3);
            verifie_tab("  ... et ne touche a rien", t, att, 3);
        }

        printf("12. tri_selection\n");
        {
            int t[] = {3, 1, 4, 1, 5, 9, 2, 6};
            int att[] = {1, 1, 2, 3, 4, 5, 6, 9};
            tri_selection(t, 8);
            verifie_tab("tri_selection({3,1,4,1,5,9,2,6})", t, att, 8);
        }
        {
            int t[] = {5, 4, 3, 2, 1};          /* pire cas */
            int att[] = {1, 2, 3, 4, 5};
            tri_selection(t, 5);
            verifie_tab("tri_selection({5,4,3,2,1}) (ordre inverse)", t, att, 5);
        }
        {
            int t[] = {1, 2, 3};                /* meilleur cas */
            int att[] = {1, 2, 3};
            tri_selection(t, 3);
            verifie_tab("tri_selection({1,2,3}) (deja trie)", t, att, 3);
        }
        {
            int t[] = {-3, 7, -8, 0};
            int att[] = {-8, -3, 0, 7};
            tri_selection(t, 4);
            verifie_tab("tri_selection({-3,7,-8,0}) (avec negatifs)", t, att, 4);
        }
    }

    printf("\n-----------------------------------------\n");
    printf("  SCORE : %d / %d\n", reussis, total);
    printf("-----------------------------------------\n");

    if (reussis == total) {
        printf("\n  Les 4 motifs sont acquis.\n");
        printf("  Prochaine etape : 3.projects_to_learn_algo/\n");
        printf("  00_passerelle_depuis_le_C.md, partie 1 (la complexite).\n\n");
    } else {
        printf("\n  %d test(s) a faire passer. Un TODO a la fois.\n\n",
               total - reussis);
    }

    /* Renvoie 0 si tout passe : utilisable dans un script. */
    return (reussis == total) ? 0 : 1;
}
