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

void draw_player_tank(ui_t *ui, player_t *player, int x, int y)
{
    int descale = 7;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    /* DEBUG
    for (int i = 0; i < 7; i++)
    {
        player->tank[i] = 1;
    }*/

    for (int i = 0; i < 7; i++)
    {
        for(int j = 0; j < player->tank[i]; j++)
        {
            int card_x = x + i * card_width + i * 10;
            int card_y = y + j * 10;
            SDL_Rect draw_card_rect = {card_x, card_y, card_width, card_height};
            SDL_RenderCopy(ui->renderer, ui->front_card_textures[i], NULL, &draw_card_rect);
        }
    }
}

// Affiche les joueurs. Affiches les joueurs sur chaque côté de l'écran, la pile étant au centre.
void draw_players(ui_t *ui, game_t *game)
{
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, 255);

    int size_length = ui->screen_w / 2 - 90;
    int size_height = 200;
    int padding = 25;

    if (game->players[0] != NULL) // En bas à gauche
    {
        SDL_SetRenderDrawColor(ui->renderer, 128, 128, 128, 255);
        SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
        draw_player_tank(ui, game->players[0], padding, ui->screen_h - size_height + padding);

        // SDL_Rect player_avatar_rect = {ui->screen_w - 40, ui->screen_h - 80, 40, 40};
        // SDL_RenderCopy(ui->renderer, ui->player_textures[0], NULL, &player_avatar_rect); Pour l'affichage d'un éventuel avatar.
    }

    if (game->players[1] != NULL) // En haut à droite
    {
        SDL_SetRenderDrawColor(ui->renderer, 128, 128, 128, 255);
        SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
        draw_player_tank(ui, game->players[1], ui->screen_w - size_length + padding, padding);
    }

    if (game->players[2] != NULL) // En haut à gauche
    {
        SDL_SetRenderDrawColor(ui->renderer, 128, 128, 128, 255);
        SDL_Rect player_background = {0, 0, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
        draw_player_tank(ui, game->players[2], padding, padding);
    }

    if (game->players[3] != NULL) // En bas à droite
    {
        SDL_SetRenderDrawColor(ui->renderer, 128, 128, 128, 255);
        SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderFillRect(ui->renderer, &player_background);
        draw_player_tank(ui, game->players[3], ui->screen_w - size_length + padding, ui->screen_h - size_height + padding);
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

    if (ui->animate[0]) // flip_the_card
    {
        flip_the_card(ui, game, ui->click_x, ui->click_y);
    }
    else
    {
        for (int i = 0; i < 3; i++) // 3 = nombre d'élément derrière la carte.
        {
            SDL_Rect draw_indicator_rect = {x + i * 20 + 7, y + 30 + i * 45, FLAG_WIDTH / 3, FLAG_HEIGHT / 3};
            SDL_RenderCopy(ui->renderer, ui->back_flag_textures[game->draw_pile->card->back[i]], NULL, &draw_indicator_rect);
        }

        // Affiche la carte
        SDL_Rect draw_pile_rect = {x, y, CARD_WIDTH / 3, CARD_HEIGHT / 3};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &draw_pile_rect);

        if (ui->follow_mouse)
        {
            draw_particles(ui, game, ui->mouse_pos.x, ui->mouse_pos.y);
        }
    }
}

void draw_buttons(ui_t *ui, game_t *game)
{
    return;
}

void draw_active_player(ui_t *ui, game_t *game)
{
    SDL_Rect draw_active_player_rect = {10, ui->screen_h / 2 + 100, 230, 70};
    SDL_RenderCopy(ui->renderer, ui->interface_textures[game->player_action + 4], NULL, &draw_active_player_rect);
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
        draw_active_player(ui, game);
        draw_buttons(ui, game);
        draw_logo(ui);
        draw_draw_card(ui, game);
    }

    if (game->win != 0)
    {
        draw_confetti(ui);
    }

    // Affichage
    SDL_RenderPresent(ui->renderer);
    SDL_Delay(15); // ~ 60 FPS
    ui->tick = SDL_GetTicks();
    ui->delta_t = ui->tick - ui->last_tick;
}