#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "headers/sdl_common.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    int * input=-1;
    ui_t *ui = create_ui();
    game_t * game= init_game();


    while (ui->program_on)
    {
        refresh_input(ui, input);
        draw(ui, game);
        SDL_RenderPresent(ui->renderer);
        SDL_Delay(15); // ~ 60 FPS
    }

    //free_input(input);
    //free_game_state(game);
    //free_ui(ui);

    return 0;
}
