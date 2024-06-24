#include <stdbool.h>
#include <SDL2/SDL.h>

#include "headers/sdl_common.h"

#define FLAG_WIDTH 434
#define FLAG_HEIGHT 396

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

// Affiche le fond d'écran.
void draw_background(ui_t *ui)
{
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, 255);
    SDL_RenderClear(ui->renderer);
}

// Affiche les joueurs. Affiches les joueurs sur chaque côté de l'écran, la pile étant au centre.
void draw_players(ui_t *ui, game_t *game)
{
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, 255);
    if (game->players[0] != NULL)
    {
        SDL_SetRenderDrawColor(ui->renderer, 255, 0, 0, 255);
        SDL_Rect draw_player_rect = {ui->screen_w - 40, ui->screen_h - 80, 40, 80};
        SDL_RenderCopy(ui->renderer, ui->player_textures[0], NULL, &draw_player_rect);
    }
}

// Affiche UNE carte.
void draw_card(card_t *card)
{
    return;
}

// Affiche la pioche
void draw_draw_cards(ui_t *ui, game_t *game, int x, int y)
{
    int oui_debug = 1;
    for (int i = 0; i < 3; i++) // 3 = nombre d'élément derrière la carte.
    {

        SDL_Rect draw_indicator_rect = {x + i * 30 + 10, y + 45 + i * 67, FLAG_WIDTH / 2, FLAG_HEIGHT / 2};
        SDL_RenderCopy(ui->renderer, ui->back_flag_textures[oui_debug], NULL, &draw_indicator_rect);
        oui_debug += 1;
    }

    // Affiche la carte
    SDL_Rect draw_pile_rect = {x, y, CARD_WIDTH / 2, CARD_HEIGHT / 2};
    SDL_RenderCopy(ui->renderer, ui->back_card_texture, NULL, &draw_pile_rect);
}

void draw_cards(ui_t *ui, game_t *game)
{
    int dumb = 0;
    for (int i = 0; i < dumb; i++) // Pour chaque joueur
    {
        for (int j = 0; j < dumb; j++)
        {
            // draw_card
        }
    }
    draw_draw_cards(ui, game, ui->screen_w / 2 - (CARD_WIDTH / 4), ui->screen_h / 2 - (CARD_HEIGHT / 4));
}

void draw_buttons(ui_t *ui, game_t *game)
{
    return;
}

void draw_logo(ui_t *ui)
{
    SDL_Rect draw_logo_rect = {10, 10, 200, 200};
    SDL_RenderCopy(ui->renderer, ui->interface_textures[0], NULL, &draw_logo_rect);
}

// Affiche tout les éléments du jeu.
void draw(ui_t *ui, game_t *game)
{
    if (ui->in_pause)
    {
        // Affiche le menu pause
    }
    else
    {
        draw_background(ui);
        draw_players(ui, game);
        draw_cards(ui, game);
        draw_buttons(ui, game);
        draw_logo(ui);
    }

    // Affichage
    SDL_RenderPresent(ui->renderer);
    SDL_Delay(15); // ~ 60 FPS
}