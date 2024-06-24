#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/graphics.h"
#include "headers/gameplay.h"

/**
 * @brief Initialisation du jeu
 */
game_t *create_game()
{
    game_t *game = malloc(sizeof(game_t));

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

    game->player_action = 1;          // Le joueur 1 commence
    game->win = 0;                    // Initialisation du statut de victoire à 0 (personne n'a gagné)
    game->draw_pile = stack_create(); // Initialisation de la pile de pioche avec une pile vide

    return game;
}

int game_loop(int input, game_t *game, ui_t *ui)
{
    return 1;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    int input = -1;
    ui_t *ui = create_ui();
    game_t *game = create_game();

    while (ui->program_on)
    {
        refresh_input(ui, &input, game);
        game_loop(input, game, ui); // (Attention pour les animations à ne rien faire si aucune input)
        draw(ui, game);
    }

    // free_input(input);
    // free_game_state(game);
    free_ui(ui);

    return 0;
}
