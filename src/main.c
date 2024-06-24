#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

        if (*input==-1){ //rien n'est cliqué
            //rien

        } else if (*input==0) { //choix de marquer

            game_play(input, game); //(Attention pour les animations à ne rien faire si aucune input)
            
        } else { //choix de voler un joueur
            game_play(input, game); //(Attention pour les animations à ne rien faire si aucune input)

        }

        draw(ui, game);
        SDL_RenderPresent(ui->renderer);
        SDL_Delay(15); // ~ 60 FPS
    }

    //free_input(input);
    //free_game_state(game);
    //free_ui(ui);

    return 0;
}
