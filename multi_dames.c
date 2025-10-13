#include <stdio.h>

#define TAILLE 8
typedef struct {
    int pion[TAILLE][TAILLE]
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

int jeu_capturer(Jeu *jeu, int i, int j);
int jeu_saisir_pion(Jeu *jeu, int i, int j);
int jeu_sauter_vers(Jeu *jeu, int i, int j);
int jeu_arreter(Jeu *jeu);
int jeu_joueur_suivant(Jeu *jeu);
void jeu_charger(Jeu *jeu);
void jeu_ecrire(Jeu *jeu);

