#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define TAILLE 8
typedef struct {
    int pion[TAILLE][TAILLE];
} Plateau;

typedef struct {
    int etat;
    int score;
} Joueur;

#define MAX_JOUEURS 4
typedef struct {
    Plateau plateau;
    Joueur joueur[MAX_JOUEURS];
    int nb_joueurs;
    int joueur_courant;
    int tour;
    int pion_est_saisi;
    int pion_i, pion_j;
} Jeu;

enum {
    P_VIDE = 0,
    P_BLANC = 1,
    P_ROUGE = 2,
    P_NOIR = 3
};

#define case_valide(i, j) (i >= 0 && i < 8 && j >= 0 && j < 8)
#define score_pion(pion) (pion == P_VIDE ? 0 : pion == P_BLANC ? 1 : pion == P_ROUGE ? 5 : 8)

int saut_valide(Plateau *plateau, int i, int j, int di, int dj) {
    if (!di && !dj) return 0;
    if (plateau->pion[i][j] == P_VIDE) return 0;
    if ((i + 2*di > 7) || (i + 2*di < 0) || (j + 2*dj > 7) || (j + 2*dj < 0)) return 0;
    if (plateau->pion[i + di][j + dj] == P_VIDE) return 0;
    if (plateau->pion[i + 2*di][j + 2*dj] != P_VIDE) return 0;
    return 1;
}

int peut_sauter(Plateau *plateau, int i, int j)
{
    for (int di = -1; di <= 1; di++)
    for (int dj = -1; dj <= 1; dj++)
        if (saut_valide(plateau, i, j, di, dj))
            return 1;

    return 0;
}

void afficher_info(Jeu *jeu)
{
    printf("Score:\n    ");
    for (unsigned char i = 0; i < jeu->nb_joueurs; i++)
        printf("J%d ", i+1);
    printf("\n    ");
    for (unsigned char i = 0; i < jeu->nb_joueurs; i++)
        printf("%2d ", jeu->joueur[i].score);
    printf("\nTour: %d\nJoueur %d (score: %d)\nPlateau:\n", jeu->tour, jeu->joueur_courant + 1, jeu->joueur[jeu->joueur_courant].score);
}

void afficher_plateau(Jeu* jeu)
{
    puts("  | 1 2 3 4 5 6 7 8");
    puts("--+-----------------");
    for (int i = 0; i < TAILLE; i++) {
        printf("%d | ", i+1);
        for (int j = 0; j < TAILLE; j++) {
            if (jeu->pion_est_saisi && jeu->pion_i == i && jeu->pion_j == j) printf("\033[4m");
            printf("%s \033[0m", jeu->plateau.pion[i][j] == P_VIDE ? "\033[100m " : jeu->plateau.pion[i][j] == P_BLANC ? "\033[30;47mb" : jeu->plateau.pion[i][j] == P_ROUGE ? "\033[101mr" : "\033[37;40mn");
        }
        putchar('\n');
    }
}

void jeu_charger(Jeu *jeu)
{
    scanf("%d%d%d", &jeu->nb_joueurs, &jeu->tour, &jeu->joueur_courant);
    for (int j = 0; j < jeu->nb_joueurs; j++)
        scanf("%d%d", &jeu->joueur[j].etat, &jeu->joueur[j].score);
    scanf("%d%d%d", &jeu->pion_est_saisi, &jeu->pion_i, &jeu->pion_j);
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            scanf("%d", &jeu->plateau.pion[i][j]);
}

/* VERSION PLATON
plateau::pion -> plateau::grille / jeu::pion_i -> jeu::i / jeu::pion_j -> jeu::j

void jeu_ecrire(Jeu *jeu)
{
    printf("%d %d %d\n", jeu->nb_joueurs, jeu->tour, jeu->joueur_courant);
    for (int j = 0; j < jeu->nb_joueurs; j++)
        printf("%d %d\n", jeu->joueur[j].etat, jeu->joueur[j].score);
    printf("%d %d %d\n", jeu->pion_est_saisi, jeu->i, jeu->j);
    for (int i = 0; i < TAILLE; i++) {
        printf("%d", jeu->plateau.grille[i][0]);
        for (int j = 1; j < TAILLE; j++)
            printf(" %d", jeu->plateau.grille[i][j]);
        putchar('\n');
    }
}
*/

void jeu_ecrire(Jeu *jeu)
{
    printf("%d %d %d\n", jeu->nb_joueurs, jeu->tour, jeu->joueur_courant);
    for (int j = 0; j < jeu->nb_joueurs; j++)
        printf("%d %d\n", jeu->joueur[j].etat, jeu->joueur[j].score);
    printf("%d %d %d\n", jeu->pion_est_saisi, jeu->pion_i, jeu->pion_j);
    for (int i = 0; i < TAILLE; i++) {
        printf("%d", jeu->plateau.pion[i][0]);
        for (int j = 1; j < TAILLE; j++)
            printf(" %d", jeu->plateau.pion[i][j]);
        putchar('\n');
    }
}

int jeu_capturer(Jeu *jeu, int i, int j)
{
    jeu->joueur[jeu->joueur_courant].score += score_pion(jeu->plateau.pion[i][j]);
    jeu->plateau.pion[i][j] = P_VIDE;
    return 1;
}

int jeu_saisir_pion(Jeu *jeu, int i, int j)
{
    if (!peut_sauter(&jeu->plateau, i, j)) return 0;
    
    jeu->pion_est_saisi = 1;
    jeu->pion_i = i;
    jeu->pion_j = j;
    return 1;
}

int jeu_sauter_vers(Jeu *jeu, int i, int j)
{
    jeu->plateau.pion[i][j] = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;
    int entre_i = (i + jeu->pion_i) / 2, entre_j = (j + jeu->pion_j) / 2;
    jeu_capturer(jeu, entre_i, entre_j);
    return 1;
}

int jeu_arreter(Jeu *jeu)
{   
    jeu->joueur[jeu->joueur_courant].etat = 0;
    return 1;
}

int jeu_joueur_suivant(Jeu *jeu)
{
    do {
        jeu->joueur_courant = (jeu->joueur_courant+1)%jeu->nb_joueurs;
    } while (!jeu->joueur[jeu->joueur_courant].etat);
    return 1;
}

void entree_joueur(int *i, int *j, const char* prompt, ...) {
    va_list valist;
    do {
        va_start(valist, prompt);
        vprintf(prompt, valist);
        va_end(valist);
        printf(" (ligne colonne) ");
        scanf("%d%d", i, j);
        (*i)--; (*j)--;
    } while (!case_valide(*i, *j) && puts("Case invalide. "));
}

int compte_joueurs_actifs(Jeu *jeu) {
    int joueurs_actifs = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++)
        if (jeu->joueur[i].etat)
            joueurs_actifs++;
    
    return joueurs_actifs;
}

void init_jeu(Jeu *jeu)
{
    jeu->pion_est_saisi = 0;

    do {
        printf("Nombres joueurs ? (2-4) ");
        scanf("%d", &jeu->nb_joueurs);
    } while (jeu->nb_joueurs < 2 || jeu->nb_joueurs > 4);

    for (int i = 0; i < jeu->nb_joueurs; i++)
        jeu->joueur[i] = (Joueur){ .etat = 1, .score = 0 };
    
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            jeu->plateau.pion[i][j] = P_VIDE;

    srand(time(NULL));
    int to_init[] = {0, 34, 20, 10};
    for (int p = 1; p < 4; p++)
    while (to_init[p]) {
        int p_i = rand()%TAILLE, p_j = rand()%TAILLE;
        if (jeu->plateau.pion[p_i][p_j] == P_VIDE) {
            jeu->plateau.pion[p_i][p_j] = p;
            to_init[p]--;
        }
    }

    for (int p = 0; p < jeu->nb_joueurs; p++) {
        jeu->joueur_courant = p;
        printf("\033[2J\033[1;1H");  // you know what this does. everyone's seen it.
        afficher_info(jeu);
        afficher_plateau(jeu);
        int i, j;
        do {
            entree_joueur(&i, &j, "Joueur %d, retirez un pion :", p+1);
        } while ((jeu->plateau.pion[i][j] == P_VIDE && printf("Ce pion a deja ete retire.\n")) ||
                (jeu->plateau.pion[i][j] != P_BLANC && printf("Ce pion n'est pas blanc.\n")));
        jeu_capturer(jeu, i, j);
    }
    jeu->joueur_courant = 0;
}

int jeu_est_fini(Jeu *jeu) {
    for (unsigned char i = 0; i < 8; i++)
    for (unsigned char j = 0; j < 8; j++)
        if (jeu->plateau.pion[i][j] && peut_sauter(&jeu->plateau, i, j))
            return 0;

    return 1;
}

void afficher_gagnant(Jeu *jeu) {
    int gagnant = 0;
    for (unsigned char i = 1; i < jeu->nb_joueurs; i++) {
        int diff = jeu->joueur[i].score - jeu->joueur[gagnant].score;
        if (diff > 0) {
            gagnant = i;
        } else if (diff == 0) {
            printf("Egalite !\n");
            return;
        }
    }
    printf("Le gagnant est le joueur %d !\n", gagnant + 1);
}

void liste_sauts_possibles(Plateau *plateau, int i, int j) {
    printf("Sauts possibles: ");
    for (int di = -1; di <= 1; di++)
    for (int dj = -1; dj <= 1; dj++)
        if (saut_valide(plateau, i, j, di, dj))
            printf("(%d, %d) ", i+2*di+1, j+2*dj+1);
    putchar('\n');
}

int main()
{
    Jeu jeu;

    init_jeu(&jeu);

    while (!jeu_est_fini(&jeu)) {
        int i, j;

        printf("\033[2J\033[1;1H");  // you know what this does. everyone's seen it.
        afficher_info(&jeu);
        afficher_plateau(&jeu);
        do {
            entree_joueur(&i, &j, "Position du pion sauteur ?");
        } while (!peut_sauter(&jeu.plateau, i, j) && puts("Le pion ne peut pas sauter."));
        jeu_saisir_pion(&jeu, i, j);

        do {
            printf("\033[2J\033[1;1H");  // you know what this does. everyone's seen it.
            afficher_info(&jeu);
            afficher_plateau(&jeu);

            liste_sauts_possibles(&jeu.plateau, i, j);
            do {
                entree_joueur(&i, &j, "Entrer un saut.");
            } while (!saut_valide(&jeu.plateau, jeu.pion_i, jeu.pion_j, (i - jeu.pion_i)/2, (j - jeu.pion_j)/2) && puts("Saut invalide."));

            jeu_sauter_vers(&jeu, i, j);
            jeu_saisir_pion(&jeu, i, j);
        } while (peut_sauter(&jeu.plateau, i, j));
        jeu.pion_est_saisi = 0;

        if (compte_joueurs_actifs(&jeu) > 1) {
            int quitter;
            do {
                printf("Arrêter ? (1=oui, 0=non) ");
                scanf("%d", &quitter);
            } while (quitter != 0 && quitter != 1);
            if (quitter) jeu_arreter(&jeu);
        }

        jeu_joueur_suivant(&jeu);
    };

    afficher_info(&jeu);
    afficher_gagnant(&jeu);

    return 0;
}
