#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/struct.h"

#define NUMBER_FACE 7
#define NUMBER_BACK 3

/**
 *@brief Initialisation du jeu
 */
game_t init_game()
{
    game_t game;

    // Initialisation des joueurs à 0
    for (int i = 0; i < 3; i++)
    {
        game.players[i] = 0;
    }
    // Le joueur 1 commence
    game.player_action = 1;
    // Initialisation du statut de victoire de chaque joueur à 0 (personne n'a gagné)
    for (int i = 0; i < 4; i++)
    {
        game.win[i] = 0;
    }
    // Initialisation de la pile de pioche avec une pile vide
    game.draw_pile = stack_create();

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

    int number_of_cards = 70;

    // Initialisation des couleurs de face
    for (int i = 0; i < number_of_cards; i++)
    {
        int face = rand() % NUMBER_FACE;
        int back[2];

        // Initialisation des couleurs de dos
        for (int j = 0; j < 2; j++)
        {
            do
            {
                back[j] = rand() % NUMBER_BACK;
            } while (back[j] == face && j == 1 && back[0] == face);
        }

        card_t card;
        init_card(&card, face, back);

        game->draw_pile = stack_push(game->draw_pile, card.back);
    }
}
