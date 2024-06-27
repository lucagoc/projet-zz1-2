#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "headers/sdl_common.h"
#include "headers/graphics.h"
#include "headers/gameplay.h"
#include "headers/player.h"
#include "headers/debug.h"

/**
 * @brief Fonction qui permet de rafraichir les inputs
 *
 * @param ui
 * @param ui_input
 */
int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    ui_t *ui = create_ui();
    ui_input_t *ui_input = create_ui_input();
    game_t *game = create_game();
    Mix_PlayChannel(-1, ui->music[0], 0);

    while (ui->program_on)
    {
        refresh_input(ui, ui_input);
        if (game->win == -1) // Personne gagne
        {
            if (game->player_action == 0) // C'est le joueur 1
            {
                int input = process_input(ui_input, game, ui);
                if (input != -1)
                {
                    game_play(game, input);
                }
            }
            else if (game->player_action != 0 && game->win == -1) // C'est les robots
            {
                int input = process_input_robot(ui_input, game, ui);
                if (input != -1)
                {
                    game_play(game, input);
                }
            }
        }

        draw(ui, game);
        play_sound(ui, game);
    }

    free_game(game);
    free_ui(ui);
    free(ui_input);

    return 0;
}
