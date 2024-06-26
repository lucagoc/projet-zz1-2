#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "headers/struct.h"

#define NUMBER_FACE 7       // Nombre de couleurs
#define NUMBER_BACK 3       // Nombre d'indicateur au dos de la carte
#define NUMBER_CARD 105     // Nombre de cartes
#define NUMBER_MIX_STACK 20 // Nombre de mélange de cartes

/*
 * @brief Test si un joueur gagne.
 * 
 * @param game le jeu
 */
int is_victory(game_t *game)
{

    for (int i = 0; i < 4; i++)
    {
        if (game->players[i]->score >= 10)
        {
            return i;
        }
    }

    return 0;
}

/**
 *@brief Libération de la mémoire du jeu
 *
 * @param game le jeu
 */
void free_game(game_t *game)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->players[i] != NULL) // Nombre de joueur variable
        {
            free(game->players[i]);
        }
    }
    free(game);
}

/**
 * @brief Fonction de vérification de la victoire
 *
 * @param game le jeu
 * @param nb_player le nombre de joueur
 */
bool check_win(game_t *game)
{
    for (int  i = 0; i < 4 ; i++)
    {
        if (game->draw_pile_left[i] > 0)
        {
            game->win = 1;
            return true;
        }
    }
    return false;
}

// Dépile la pile et renvoie la couleur de la première carte, retourne -1 si la pile est vide
// Retourne 1 si la carte à bien été générée
// Les cartes sont générée à la volée
int get_draw_card(game_t *game)
{
    if (is_draw_pile_empty(game))
    {
        return -1;
    }

    // Prendre une carte dans le paquet
    int r = (rand() % NUMBER_FACE);
    while (game->draw_pile_left[r] <= 0)
    {
        r = rand() % NUMBER_FACE;
    }
    game->face_card_color = r;

    /* Couleur d'indicateur */
    int a = -1;
    int b = -1;
    while (a == -1 || a == r)
    {
        a = rand() % NUMBER_FACE;
    }
    while (b == -1 || b == r || b == a)
    {
        b = rand() % NUMBER_FACE;
    }

    // Mettre les 3 couleurs en positions aléatoires
    int d = rand() % 3;
    game->back_card_color[d] = r;
    game->back_card_color[(d + 1) % 3] = a;
    game->back_card_color[(d + 2) % 3] = b;

    game->draw_pile_left[r] = game->draw_pile_left[r] - 1;
    return 1;
}

/**
 *@brief Initialisation du jeu
 */
game_t *create_game()
{
    game_t *game;
    game = malloc(sizeof(game_t));

    srand(time(NULL)); // Initialisation de la graine pour la génération aléatoire

    // Initialisation des joueurs à 0
    for (int i = 0; i < 4; i++)
    {
        player_t *newplayer = malloc(sizeof(player_t));
        for (int j = 0; j < 7; j++)
        {
            newplayer->tank[j] = 0;
        }
        newplayer->score = 0;
        newplayer->last_scored_card = -1;
        game->players[i] = newplayer;
    }

    game->face_card_color = -1; // Initialisation de la couleur de la carte tirée à -1
    game->player_action = 0;    // Le joueur 1 commence
    game->win = 0;              // Initialisation du statut de victoire à 0 (personne n'a gagné)
    game->back_card_color[0] = -1;
    game->back_card_color[1] = -1;
    game->back_card_color[2] = -1;

    for (int i = 0; i < 7; i++)
    {
        game->draw_pile_left[i] = 15; // Initialisation de la pile de cartes
    }

    // Tirer une première carte
    get_draw_card(game);
    game->stealing = 0;

    return game;
}

// Renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon
int is_card_in_tank(int player, game_t *game)
{
    return game->players[player]->tank[game->face_card_color];
}

/**
 * @brief Fonction distribuant les cartes à chaque joueur à partir de la pile de pioche
 *
 * @param game le jeu
 */

void distribute_card(game_t *game, int nb_player)
{
    for (int i = 0; i < nb_player; i++)
    {
        game->players[i]->tank[game->face_card_color] = 1; // on ajoute la carte au tank du joueur
        get_draw_card(game);                               // on dépile
    }
}

/**
 * @brief Fonction du score
 *
 * @param game le jeu
 */
void score_card(game_t *game)
{
    game->players[game->player_action]->score += game->players[game->player_action]->tank[game->face_card_color] + 1; // on ajoute les cartes au score du joueur
    game->players[game->player_action]->tank[game->face_card_color] = 0;                                              // on enlève les cartes du tank
    game->players[game->player_action]->last_scored_card = game->face_card_color;                                     // on affiche la carte en haut de la pile de score
}

/**
 * @brief Fonction d'ajout de carte dans le tank
 *
 * @param player le joueur
 * @param game le jeu
 */
void add_card_in_tank(int player, game_t *game)
{
    fprintf(stderr, "[DEBUG] add_card_in_tank : player %d, card %d\n", player, game->face_card_color);
    game->players[player]->tank[game->face_card_color] = 1; // on ajoute la carte au tank
}

/**
 * @brief Fonction de vol de carte
 *
 * @param input l'input du joueur
 * @param game le jeu
 */
void steal_card(int input, game_t *game)
{
    game->stealing = input; // on enlève les cartes au joueur volé
}

/**
 * @brief Fonction copie l'état du jeu
 *
 * @param game l'état du jeu
 * @return copy_game_state la copie de l'état du jeu
 */
game_t *copy_game(game_t *game)
{
    game_t *copy_game_state = malloc(sizeof(game_t));
    if (copy_game_state == NULL)
    {
        fprintf(stderr, "Erreur d'alloction de mémoire\n");
        return NULL;
    }
    // Copie des joueurs
    for (int i = 0; i < 4; i++)
    {
        player_t *copy_player = malloc(sizeof(player_t));
        if (copy_player == NULL)
        {
            fprintf(stderr, "Erreur d'alloction de mémoire\n");
            return NULL;
        }
        for (int j = 0; j < 7; j++)
        {
            copy_player->tank[j] = game->players[i]->tank[j];
        }
        copy_player->score = game->players[i]->score;
        copy_player->last_scored_card = game->players[i]->last_scored_card;
        copy_game_state->players[i] = copy_player;
    }
    copy_game_state->face_card_color = game->face_card_color;
    copy_game_state->player_action = game->player_action;
    copy_game_state->win = game->win;

    for (int i = 0; i < 7; i++)
    {
        copy_game_state->draw_pile_left[i] = game->draw_pile_left[i];
    }
    copy_game_state->stealing = game->stealing;
    copy_game_state->back_card_color[0] = game->back_card_color[0];
    copy_game_state->back_card_color[1] = game->back_card_color[1];
    copy_game_state->back_card_color[2] = game->back_card_color[2];
    copy_game_state->face_card_color = game->face_card_color;

    return copy_game_state;
}

/**
 * @brief Fonction de jeu
 *
 * @param game le jeu
 * @param input l'input du joueur
 */

void game_play(game_t *game, int input)
{

    // input vaut 0 si le joueur actif clique sur sa propre pile et i>0 s'il clique sur le joueur i pour le voler

    if (input == game->player_action) // le joueur actif choisit de marquer
    {
        if (is_card_in_tank(game->player_action, game)) // S'il tombe sur une bonne couleur qu'il a
        {
            score_card(game);
            game->player_action = (game->player_action + 1) % 4;
        }
        else
        {
            add_card_in_tank(game->player_action, game); // s'il tombe sur une couleur qu'il n'a pas
            game->player_action = (game->player_action + 1) % 4;
        }
    }
    else // le joueur actif choisit de voler
    {
        fprintf(stderr, "[DEBUG] input value = %d", input);
        if (is_card_in_tank(input, game)) // s'il tombe sur une bonne couleur qu'il a
        {

            steal_card(input, game);
        }
        else
        {
            add_card_in_tank(input, game); // s'il tombe sur une couleur qu'il n'a pas
            game->player_action = (game->player_action + 1) % 4;
        }
    }

    // Si quelqu'un gagne
    game->win = is_victory(game);
    
    // Changement de carte
    get_draw_card(game);

    // passage au joueur suivant
    fprintf(stderr, "[DEBUG] game_play : switching to player %d\n", game->player_action);
}