#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    VIDE = 0,
    BLANC = 1,
    ROUGE = 2,
    NOIR = 3
};

void afficher_plateau(Plateau *pl)
{
    puts("  | 1 2 3 4 5 6 7 8");
    puts("--+-----------------");
    for (int i = 0; i < TAILLE; i++) {
        printf("%d | ", i+1);
        for (int j = 0; j < TAILLE; j++)
            printf("%s \033[0m", pl->pion[i][j] == VIDE ? " " : pl->pion[i][j] == BLANC ? "\033[30;47mb" : pl->pion[i][j] == ROUGE ? "\033[101mr" : "\033[37;40mn");
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

void jeu_ecrire(Jeu *jeu)
{
    printf("%d %d %d\n", jeu->nb_joueurs, jeu->tour, jeu->joueur_courant);
    for (int j = 0; j < jeu->nb_joueurs; j++)
        printf("%d %d\n", jeu->joueur[j].etat, jeu->joueur[j].score);
    printf("%d %d %d\n", jeu->pion_est_saisi, jeu->pion_i, jeu->pion_j);
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
            printf("%d ", jeu->plateau.pion[i][j]);
        putchar('\n');
    }
}

int jeu_capturer(Jeu *jeu, int i, int j)
{
    jeu->joueur[jeu->joueur_courant].score += jeu->plateau.pion[i][j];
    jeu->plateau.pion[i][j] = 0;
    return 1;
}

int jeu_saisir_pion(Jeu *jeu, int i, int j)
{
    if (0 /*pion[i][j] peut pas sauter*/) return 0;
    
    jeu->pion_est_saisi = 1;
    jeu->pion_i = i;
    jeu->pion_j = j;
    return 1;
}


int jeu_sauter_vers(Jeu *jeu, int i, int j)
{
    jeu->plateau.pion[i][j] = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;
    return 1;
}

int jeu_arreter(Jeu *jeu)
{
    int joueurs_actifs = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++)
        if (jeu->joueur[i].etat)
            joueurs_actifs++;
    
    if (joueurs_actifs == 1)
        return 0;
    
    jeu->joueur[jeu->joueur_courant].etat = 0;
    return 1;
}

int jeu_joueur_suivant(Jeu *jeu)
{
    for (unsigned char i = 0; i < jeu->nb_joueurs; i++)
    {
        unsigned char j = (jeu->joueur_courant + i + 1)%jeu->nb_joueurs;
        if (jeu->joueur[j].etat) {
            jeu->joueur_courant = j;
            break;
        }
    }
    return 0;
}

void init_jeu(Jeu *jeu)
{
    do {
        printf("Nombres joueurs ? (2-4) ");
        scanf("%d", &jeu->nb_joueurs);
    } while (jeu->nb_joueurs < 2 || jeu->nb_joueurs > 4);

    for (int i = 0; i < jeu->nb_joueurs; i++)
        jeu->joueur[i] = (Joueur){ .etat = 1, .score = 0 };
    
    for (int i = 0; i < TAILLE; i++)
        for (int j = 0; j < TAILLE; j++)
            jeu->plateau.pion[i][j] = VIDE;

    srand(time(NULL));
    int to_init[] = {0, 34, 20, 10};
    for (int p = 1; p < 4; p++)
    while (to_init[p])
    {
        int p_i = rand()%TAILLE, p_j = rand()%TAILLE;
        if (jeu->plateau.pion[p_i][p_j] == VIDE) {
            jeu->plateau.pion[p_i][p_j] = p;
            to_init[p]--;
        }
    }
}

int main()
{
    Jeu jeu;

    init_jeu(&jeu);
    afficher_plateau(&jeu.plateau);

    (void)jeu;
    return 0;
}
