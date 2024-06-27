#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/graphics.h"
#include "headers/gameplay.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    ui_t *ui = create_ui();
    ui_input_t *ui_input = create_ui_input();
    game_t *game = create_game();

    while (ui->program_on)
    {
        refresh_input(ui, ui_input);
        int input = process_input(ui_input, game, ui);

        if (game->win == -1 && input != -1)
        {
            game_play(game, input);
        }

        draw(ui, game);
    }

    free_game(game);
    free_ui(ui);

    return 0;
}
