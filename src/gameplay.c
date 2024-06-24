#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "headers/struct.h"

#define NUMBER_FACE 7

/**
 *@brief Initialisation du jeu
 */
game_t *init_game()
{
    game_t *game;
    game = malloc(sizeof(game_t));

    // Initialisation des joueurs à 0
    for (int i = 0; i < 4; i++)
    {
        player_t *newplayer = malloc(sizeof(player_t));
        for (int j = 0; j < 6; j++)
        {
            newplayer->tank[j] = 0;
        }
        newplayer->score = 0;
        newplayer->last_scored_card = -1;
        game->players[i] = newplayer;
    }
    // Le joueur 1 commence
    game->player_action = 1;
    // Initialisation du statut de victoire à 0 (personne n'a gagné)
    game->win = 0;
    // Initialisation de la pile de pioche avec une pile vide
    game->draw_pile = stack_create();

    return game;
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
        return;
    }
    game->draw_pile = stack_create();

    srand(time(NULL));

    int number_of_cards = 105;
    int total_card = 0;
    int face_count[NUMBER_FACE] = {0};
    card_t card[number_of_cards];

    for (int i = 0; i < NUMBER_FACE; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            card[total_card].face = i;
            face_count[i]++;
            total_card++;
        }
    }

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

int is_card_in_tank(int player, game_t *game)
{
    // renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon
    return game->players[player]->tank[game->drawn_card_color];
}

// dépile la pile et renvoie la couleur de la première carte
int get_draw_card(game_t *game)
{
    if (game->draw_pile != NULL)
    {
        int drawn_card_color = game->draw_pile->card.face; // on récupère la couleur du haut de la pile

        game->draw_pile = game->draw_pile->next; // on passe à l'élément suivant de la pile

        return drawn_card_color;
    }
    else
    {
        return -1;
    }
}

void score_card(game_t *game)
{
    game->players[game->player_action]->score += game->players[game->player_action]->tank[game->drawn_card_color] + 1; // on ajoute les cartes au score du joueur

    game->players[game->player_action]->tank[game->drawn_card_color] = 0; // on enlève les cartes du tank

    game->players[game->player_action]->last_scored_card = game->drawn_card_color; // on affiche la carte en haut de la pile de score
}

void add_card_in_tank(int player, game_t *game)
{
    game->players[player]->tank[game->drawn_card_color] = 1; // on ajoute la carte au tank
}

void steal_card(int input, game_t *game)
{
    game->players[game->player_action]->tank[game->drawn_card_color] += game->players[input]->tank[game->drawn_card_color] + 1; // on récupère les cartes volées

    game->players[input]->tank[game->drawn_card_color] = 0; // on enlève les cartes au joueur volé
}

void game_play(game_t *game, int input)
{

    // input vaut 0 si le joueur actif clique sur sa propre pile et i>0 s'il clique sur le joueur i pour le voler

    game->drawn_card_color = get_draw_card(game); // on dépile et on affiche

    if (input)
    { // le joueur actif choisit de marquer

        if (is_card_in_tank(game->player_action, game))
        { // s'il tombe sur une bonne couleur qu'il a

            score_card(game);
        }
        else
        { // s'il tombe sur une couleur qu'il n'a pas

            add_card_in_tank(game->player_action, game);
        }
    }
    else
    { // le joueur actif choisit de voler

        if (is_card_in_tank(input, game))
        { // s'il tombe sur une bonne couleur qu'il a

            steal_card(input, game);
        }
        else
        { // s'il tombe sur une couleur qu'il n'a pas
            add_card_in_tank(input, game);
        }
    }
}
