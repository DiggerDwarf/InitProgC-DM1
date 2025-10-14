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

inline int abs(n) { return x < 0 ? -x : x; }

/**
 * \brief Vérifie si un saut est valide
 * \param plateau Le plateau pour vérifier si le saut est valide
 * \param i La ligne du pion sauteur
 * \param j La colonne du pion sauteur
 * \param di La ligne de destination
 * \param dj Le colonne de destination
 * \return 1 si le saut est valide, 0 sinon
 */
int saut_valide(Plateau *plateau, int i, int j, int di, int dj) {
    if (!(abs(i - di) == 2 || i - di == 0) || !(abs(j - dj) == 2 || j - dj == 0)) return 0;
    if (di == i && dj == j) return 0;
    if (plateau->pion[i][j] == P_VIDE) return 0;
    if ((di > 7) || (di < 0) || (dj > 7) || (dj < 0)) return 0;
    if (plateau->pion[(di + i)/2][(dj + j)/2] == P_VIDE) return 0;
    if (plateau->pion[di][dj] != P_VIDE) return 0;
    return 1;
}

/**
 * \brief Vérifie si un pion peut sauter dans une quelconque direction
 * \param plateau Le plateau pour vérifier si le pion peut sauter
 * \param i La ligne du pion à vérifier
 * \param j La colonne du pion à vérifier
 * \return 1 si le pion peut sauter, 0 sinon
 */
int peut_sauter(Plateau *plateau, int i, int j)
{
    for (int di = -1; di <= 1; di++)
    for (int dj = -1; dj <= 1; dj++)
        if (saut_valide(plateau, i, j, i + di*2, j + dj*2))
            return 1;

    return 0;
}

/**
 * \brief Affiche les informations de la partie
 * \param jeu Le struct contenant les informations de la partie
 */
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

/**
 * \brief Afficher le plateau
 * \param jeu Le jeu contenant le plateau
 */
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

/**
 * \brief Charge une partie
 * \param jeu Le struct dans lequel charger la partie
 */
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

/**
 * \brief Print le struct dans la console
 * \param jeu La partie
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

/**
 * \brief Capturer un pion
 * \param jeu La partie dans laquelle le pion est capturé
 * \param i La ligne du pion a capturer
 * \param j La colonne du pion a capturer
 * \return 1 si tout s'est bien passé
 */
int jeu_capturer(Jeu *jeu, int i, int j)
{
    jeu->joueur[jeu->joueur_courant].score += score_pion(jeu->plateau.pion[i][j]);
    jeu->plateau.pion[i][j] = P_VIDE;
    return 1;
}

/**
 * \brief Sélectionner un pion
 * \param jeu La partie dans laquelle sélectionner le pion
 * \param i La ligne du pion sélectionné
 * \param j La colonne du pion sélectionné
 * \return 1 si tout s'est bien passé
 */
int jeu_saisir_pion(Jeu *jeu, int i, int j)
{
    if (!peut_sauter(&jeu->plateau, i, j)) return 0;
    
    jeu->pion_est_saisi = 1;
    jeu->pion_i = i;
    jeu->pion_j = j;
    return 1;
}

/**
 * \brief Fait sauter le pion sélectionné vers une case
 * \param jeu La partie dans laquelle faire sauter le pion
 * \param i La ligne de destination
 * \param j La colinne de destination
 * \return 1 si tout s'est bien passé
 */
int jeu_sauter_vers(Jeu *jeu, int i, int j)
{
    if (!saut_valide(&jeu->plateau, jeu->pion_i, jeu->pion_j, i, j)) return 0;
    jeu->plateau.pion[i][j] = jeu->plateau.pion[jeu->pion_i][jeu->pion_j];
    jeu->plateau.pion[jeu->pion_i][jeu->pion_j] = 0;
    int entre_i = (i + jeu->pion_i) / 2, entre_j = (j + jeu->pion_j) / 2;
    jeu_capturer(jeu, entre_i, entre_j);
    jeu->pion_i = i;
    jeu->pion_j = j;
    return 1;
}

/**
 * \brief Permet à un joueur d'arrêter de jouer
 * \param jeu La partie dans laquelle le joueur arrête de jouer
 * \return 1 si tout s'est bien passé
 */
int jeu_arreter(Jeu *jeu)
{   
    jeu->joueur[jeu->joueur_courant].etat = 0;
    return 1;
}

/**
 * \brief Change le joueur courant
 * \return 1 si tout s'est bien passé
 */
int jeu_joueur_suivant(Jeu *jeu)
{
    int joueur_initial = jeu->joueur_courant;
    do {
        jeu->joueur_courant = (jeu->joueur_courant+1)%jeu->nb_joueurs;
    } while (!jeu->joueur[jeu->joueur_courant].etat);
    if (joueur_initial >= jeu->joueur_courant) jeu->tour++;
    return 1;
}

/**
 * \brief Demande au joueur des coordonnées (ligne colonne)
 * \param i Le pointeur vers la variable où stocker la ligne
 * \param j Le pointeur vers la variable où stocker la colonne
 * \param prompt Le prompt affiché lors de la demande
 * \param ... Les arguments éventuels pour le formattage du prompt
 */
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

/**
 * \brief Compte le nombre de joueurs actifs
 * \param jeu La partie dans laquelle compter le nombre de joueurs actifs
 */
int compte_joueurs_actifs(Jeu *jeu) {
    int joueurs_actifs = 0;
    for (int i = 0; i < jeu->nb_joueurs; i++)
        if (jeu->joueur[i].etat)
            joueurs_actifs++;
    
    return joueurs_actifs;
}

/**
 * \brief Initialise la partie
 * \param jeu La partie à intialiser 
 */
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

/**
 * \brief Vérifie si la partie est terminée
 * \param jeu La partie à vérfier
 * \return 1 si la partie est finie, 0 sinon
 */
int jeu_est_fini(Jeu *jeu) {
    for (unsigned char i = 0; i < 8; i++)
    for (unsigned char j = 0; j < 8; j++)
        if (jeu->plateau.pion[i][j] && peut_sauter(&jeu->plateau, i, j))
            return 0;

    return 1;
}

/**
 * \brief Affiche le gagnant de la partie
 * \param jeu La partie
 */
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

/**
 * \brief Affiche les sauts possibles pour un pion
 * \param plateau Le plateau pour vérifier les sauts
 * \param i La ligne du pion sauteur
 * \param j La colonne du pion sauteur

 */
void liste_sauts_possibles(Plateau *plateau, int i, int j) {
    printf("Sauts possibles: ");
    for (int di = -1; di <= 1; di++)
    for (int dj = -1; dj <= 1; dj++)
        if (saut_valide(plateau, i, j, i+2*di, j+2*dj))
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
            } while (!saut_valide(&jeu.plateau, jeu.pion_i, jeu.pion_j, i, j) && puts("Saut invalide."));

            jeu_sauter_vers(&jeu, i, j);
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
