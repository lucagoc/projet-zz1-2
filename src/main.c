#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/sdl_common.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    ui_t *ui = create_ui();
    // create_game_state
    // create_input

    while (ui->program_on)
    {
        //refresh_input(ui, input);
        //draw(ui, game)
    }

    //free_input(input);
    //free_game_state(game);
    //free_ui(ui);

    return 0;
}
