#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/graphics.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    int *input = malloc(sizeof(int));
    *input = -1;
    ui_t *ui = create_ui();
    game_t *game = create_game();

    while (ui->program_on)
    {
        refresh_input(game, ui, input);
        game_interact(input, game, ui); // (Attention pour les animations à ne rien faire si aucune input)
        draw(ui, game);
    }

    free_game(game);
    free_ui(ui);

    return 0;
}
