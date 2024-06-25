#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "headers/struct.h"

#define NUMBER_FACE 7

/**
 *@brief Initialisation du jeu
 */
game_t *create_game()
{
    game_t *game;
    game = malloc(sizeof(game_t));

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

    game->drawn_card_color = -1;      // Initialisation de la couleur de la carte tirée à -1
    game->player_action = 1;          // Le joueur 1 commence
    game->win = 0;                    // Initialisation du statut de victoire à 0 (personne n'a gagné)
    game->draw_pile = stack_create(); // Initialisation de la pile de pioche avec une pile vide

    return game;
}

/**
 *@brief Libération de la mémoire du jeu
 */
void free_game(game_t *game)
{
    for (int i = 0; i < 4; i++)
    {
        if(game->players[i] != NULL)    // Nombre de joueur variable
        {
            free(game->players[i]);
        }
    }
    stack_free(game->draw_pile);
    free(game);
}

/**
 *@brief Initialisation des cartes
 *
 *@param card La carte qu'on veut initialiser
 *@param face_color La couleur de face
 *@param back_color La couleur de dos
 */

void init_card(card_t *card, int face_color, int back_color[2])
{
    card->face = face_color;
    for (int i = 0; i < 2; i++)
    {
        card->back[i] = back_color[i];
    }
}

/**
 * @brief Initialisation de la pile de pioche de manière aléatoire
 *
 * @param game Le jeu
 */
void init_draw_card(game_t *game)
{
    if (game == NULL)
    {
        fprintf(stderr, "[ERROR] init_draw_card : game is NULL\n"); 
        return;
    }
    game->draw_pile = stack_create();

    srand(time(NULL));

    int number_of_cards = 105;
    int total_card = 0;
    int face_count[NUMBER_FACE];

    for (int i = 0; i < NUMBER_FACE; i++)
    {
        face_count[i] = 0;
    }
    
    card_t card[number_of_cards];

    // Créer les cartes en assurant qu'il y a 15 cartes de chaque couleur
    for (int i = 0; i < NUMBER_FACE; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            card[i].face = i;
            face_count[i] = face_count[i] + 1;
            total_card++;
        }
    }

    // Compléter les cartes restantes aléatoirement (même si le calcul est bon, on ne sait jamais)
    while (total_card < number_of_cards)
    {
        int face = rand() % NUMBER_FACE;
        card[total_card].face = face;
        face_count[face]++;
        total_card++;
    }

    // Mélangez les cartes pour les répartir aléatoirement
    for (int i = 0; i < NUMBER_FACE; i++)
    {
        int j = rand() % number_of_cards;

        card_t temp = card[i];
        card[i] = card[j];
        card[j] = temp;
    }

    // Initialisation des couleurs du dos de la carte
    for (int i = 0; i < NUMBER_FACE; i++)
    {
        int face = card[i].face;
        int back[2];
        for (int j = 0; j < 2; j++)
        {
            do
            {
                back[j] = rand() % NUMBER_FACE;

            } while (back[j] == face && j == 1 && back[0] == face);
        }

        init_card(&card[i], face, back);

        // Ajout de la carte à la pile de pioche
        game->draw_pile = stack_push(game->draw_pile, card[i].face, card[i].back);
    }
}

// Renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon
int is_card_in_tank(int player, game_t *game)
{
    if(game->drawn_card_color > 7 || game->drawn_card_color < 0){
        fprintf(stderr, "[ERROR] is_card_in_tank : no card drawn\n");
        return 0;
    }
    return game->players[player]->tank[game->drawn_card_color];
}

// Dépile la pile et renvoie la couleur de la première carte
int get_draw_card(game_t *game)
{
    if (game->draw_pile != NULL)
    {
        int drawn_card_color = game->draw_pile->card.face; // on récupère la couleur du haut de la pile
        game->draw_pile = game->draw_pile->next;           // on passe à l'élément suivant de la pile
        return drawn_card_color;
    }
    else
    {
        return -1;
    }
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
        game->players[i]->tank[game->draw_pile->card.face] = 1; // on ajoute la carte au tank du joueur
        game->draw_pile = game->draw_pile->next;                // on passe à l'élément suivant de la pile
    }
}

/**
 * @brief Fonction du score
 *
 * @param game le jeu
 */
void score_card(game_t *game)
{
    game->players[game->player_action]->score += game->players[game->player_action]->tank[game->drawn_card_color] + 1; // on ajoute les cartes au score du joueur
    game->players[game->player_action]->tank[game->drawn_card_color] = 0;                                              // on enlève les cartes du tank
    game->players[game->player_action]->last_scored_card = game->drawn_card_color;                                     // on affiche la carte en haut de la pile de score
}

/**
 * @brief Fonction d'ajout de carte dans le tank
 *
 * @param player le joueur
 * @param game le jeu
 */
void add_card_in_tank(int player, game_t *game)
{
    game->players[player]->tank[game->drawn_card_color] = 1; // on ajoute la carte au tank
}

/**
 * @brief Fonction de vol de carte
 *
 * @param input l'input du joueur
 * @param game le jeu
 */
void steal_card(int input, game_t *game)
{
    game->players[game->player_action]->tank[game->drawn_card_color] += game->players[input]->tank[game->drawn_card_color] + 1; // on récupère les cartes volées
    game->players[input]->tank[game->drawn_card_color] = 0;                                                                     // on enlève les cartes au joueur volé
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

    game->drawn_card_color = get_draw_card(game); // on dépile et on affiche

    if (!input) // le joueur actif choisit de marquer
    {
        if (is_card_in_tank(game->player_action, game)) // S'il tombe sur une bonne couleur qu'il a
        {
            score_card(game);
        }
        else
        {
            add_card_in_tank(game->player_action, game); // s'il tombe sur une couleur qu'il n'a pas
        }
    }
    else // le joueur actif choisit de voler
    {

        if (is_card_in_tank(input, game)) // s'il tombe sur une bonne couleur qu'il a
        {

            steal_card(input, game);
        }
        else
        {
            add_card_in_tank(input, game); // s'il tombe sur une couleur qu'il n'a pas
        }
    }

    // passage au joueur suivant
    game->player_action = (game->player_action + 1) % 4;
}
