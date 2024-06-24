#include <stdbool.h>
#include <SDL2/SDL.h>

#include "headers/sdl_common.h"
#include "headers/gameplay.h"

// Affiche les joueurs. Affiches les joueurs sur chaque côté de l'écran, la pile étant au centre.
void draw_players(ui_t *ui, game_state_t *game)
{
    return;
}

// Affiche UNE carte.
void draw_card(card_t *card)
{
    return;
}

void draw_cards(ui_t *ui, game_state_t *game)
{
    for (int i = 0; i < game->nb_players; i++) // Pour chaque joueur
    {
        for (int j = 0; j < game->??? ; j++)
        {
            //draw_card
        }
    }
}

// Affiche la pile de carte du jeu.
void draw_stack(ui_t *ui, game_state_t *game)
{
    return;
}



void draw(ui_t *ui, game_state_t *game)
{
    SDL_SetRenderDrawColor(ui->renderer, 0, 0, 0, 255);
    SDL_RenderClear(ui->renderer);

    // Draw des éléments
    draw_cards(ui, game);
    draw_players(ui, game);
    draw_buttons(ui, game);

    SDL_RenderPresent(ui->renderer);
}