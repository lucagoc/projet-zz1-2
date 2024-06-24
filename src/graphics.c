#include <stdbool.h>
#include <SDL2/SDL.h>

#include "headers/sdl_common.h"
#include "headers/animation.h"

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

    int size_length = ui->screen_w / 2 - 90;
    int size_height = 200;

    if (game->players[0] != NULL)
    {
        SDL_SetRenderDrawColor(ui->renderer, 255, 0, 0, 255);
        SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);

        // SDL_Rect player_avatar_rect = {ui->screen_w - 40, ui->screen_h - 80, 40, 40};
        // SDL_RenderCopy(ui->renderer, ui->player_textures[0], NULL, &player_avatar_rect); Pour l'affichage d'un éventuel avatar.
    }

    if (game->players[1] != NULL)
    {
        SDL_SetRenderDrawColor(ui->renderer, 0, 255, 0, 255);
        SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
    }

    if (game->players[2] != NULL)
    {
        SDL_SetRenderDrawColor(ui->renderer, 0, 0, 255, 255);
        SDL_Rect player_background = {0, 0, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
    }

    if (game->players[3] != NULL)
    {
        SDL_SetRenderDrawColor(ui->renderer, 255, 255, 0, 255);
        SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
    }
}

// Affiche la pioche
void draw_draw_card(ui_t *ui, game_t *game)
{
    int x = 0;
    int y = 0;

    if (ui->follow_mouse)
    {
        x = ui->mouse_pos.x - CARD_WIDTH / 6;
        y = ui->mouse_pos.y - CARD_HEIGHT / 6;
    }
    else
    {
        x = ui->screen_w / 2 - CARD_WIDTH / 6;
        y = ui->screen_h / 2 - CARD_HEIGHT / 6;
    }
    int oui_debug = 1;
    for (int i = 0; i < 3; i++) // 3 = nombre d'élément derrière la carte.
    {
        SDL_Rect draw_indicator_rect = {x + i * 20 + 7, y + 30 + i * 45, FLAG_WIDTH / 3, FLAG_HEIGHT / 3};
        SDL_RenderCopy(ui->renderer, ui->back_flag_textures[oui_debug], NULL, &draw_indicator_rect);
        oui_debug += 1;
    }

    // Affiche la carte
    SDL_Rect draw_pile_rect = {x, y, CARD_WIDTH / 3, CARD_HEIGHT / 3};
    SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &draw_pile_rect);

    if (ui->follow_mouse)
    {
        draw_particles(ui, ui->mouse_pos.x, ui->mouse_pos.y);
    }
}

void draw_buttons(ui_t *ui, game_t *game)
{
    return;
}

void draw_logo(ui_t *ui)
{
    SDL_Rect draw_logo_rect = {10, ui->screen_h / 2 - 100, 200, 200};
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
        draw_draw_card(ui, game);
        draw_buttons(ui, game);
        draw_logo(ui);
    }

    if (game->win != 0)
    {
        draw_confetti(ui);
    }

    // Affichage
    SDL_RenderPresent(ui->renderer);
    SDL_Delay(15); // ~ 60 FPS
    ui->tick = SDL_GetTicks();
}