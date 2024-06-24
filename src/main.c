#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/graphics.h"
#include "headers/gameplay.h"

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

    free_game(game);
    free_ui(ui);

    return 0;
}
