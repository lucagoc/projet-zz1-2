#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/graphics.h"
#include "headers/gameplay.h"

// Fonction qui traite les inputs de la SDL vers les inputs relative au jeu en fonction de l'état de celui-ci
// Active les animations si besoin
int process_input(ui_input_t *ui_input, game_t *game, ui_t *ui)
{
    int input = 0;

    if (game->player_action == 0 && game->win == -1)
    {
        if (is_draw_clicked(ui_input))
        {
            play_animation(ui->animations[0]);
        }
        else
        {
            int player = is_player_clicked(ui_input);
            if (player != -1)
            {
                if(is_steal_clicked(game, player))
                {
                    ui->animations[1].pos.x = ui->mouse_pos.x;
                    play_animation(ui->animations[1]);
                }
                else
                {
                    
                }
            }
        }
    }

    return input;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    ui_t *ui = create_ui();
    ui_input_t *ui_input = create_ui_input();
    game_t *game = create_game();
    anim_props_t *anim = create_animations();

    while (ui->program_on)
    {
        refresh_input(ui, ui_input);
        int input = process_input(ui_input, game, ui, anim);
        game_interact(input, game); // (Attention pour les animations à ne rien faire si aucune input)
        draw(ui, game, anim);
    }

    free(anim);
    free_game(game);
    free_ui(ui);

    return 0;
}
