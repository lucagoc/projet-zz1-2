#include <SDL2/SDL.h>
#include <stdbool.h>

#include "headers/sdl_common.h"

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

// Faire pleuvoir des confettis sur l'écran.
void draw_confetti(ui_t *ui)
{
    float speed = 0.001;
    for (int i = 0; i < 100; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if (i % 4 == 0)
            SDL_SetRenderDrawColor(ui->renderer, 255, 0, 0, 255);
        else if (i % 4 == 1)
            SDL_SetRenderDrawColor(ui->renderer, 0, 255, 0, 255);
        else if (i % 4 == 2)
            SDL_SetRenderDrawColor(ui->renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(ui->renderer, 255, 255, 0, 255);

        int x = fmod((ui->tick * 10 * i * speed), ui->screen_w);
        int y = fmod((ui->tick * 10 * i * speed), ui->screen_h);
        SDL_Rect confetti = {x, y, 10, 10};
        SDL_RenderFillRect(ui->renderer, &confetti);
    }
}

// Donne un effet de flip à une carte.
void flip_the_card(ui_t *ui, game_t *game)
{
    int x = ui->screen_w / 2 - CARD_WIDTH / 2;
    int y = ui->screen_h / 2 - CARD_HEIGHT / 2;
    int descale = 4;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    float speed = 0.1;
    float anime_tick = (ui->tick * speed);

    if (anime_tick < 200)
    {
        SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, anime_tick * 255 / 200);
        SDL_Rect background = {x - card_width / 2, y + 20, card_width, card_height - 40};
        SDL_RenderFillRect(ui->renderer, &background);
        SDL_Rect card = {x - card_width / 2, y, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &card);
    }
    else if (anime_tick < 300)
    {
        card_width = (CARD_WIDTH * (300 - anime_tick) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {x - card_width / 2, y, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[1], NULL, &card);
    }
    else if (anime_tick < 400)
    {
        card_width = (CARD_WIDTH * (anime_tick - 300) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {x - card_width / 2, y, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->front_card_textures[1], NULL, &card);
    }
    else
    {
        ui->animate[0] = false;
        ui->animate[1] = false;
        ui->tick = 0;
    }
}

// Donne un effet de luminescences à un SDL_Rect.
void draw_luminescence(ui_t *ui, SDL_Rect *rect)
{
    int x = rect->x;
    int y = rect->y;
    int w = rect->w;
    int h = rect->h;

    int luminescence = 50;
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, luminescence);
    SDL_Rect luminescence_rect = {x - 10, y - 10, w + 20, h + 20};
    SDL_RenderFillRect(ui->renderer, &luminescence_rect);
}

// Affiche des particules tournant autour d'un point
void draw_particles(ui_t *ui, int x, int y)
{
    // Correction d'un problème d'allignement;
    x -= 4;
    y -= 1;

    int particle_number = 100;
    float speed = 0.00005;
    for (int i = 0; i < particle_number; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if (i % 4 == 0)
            SDL_SetRenderDrawColor(ui->renderer, 255, 0, 0, 255);
        else if (i % 4 == 1)
            SDL_SetRenderDrawColor(ui->renderer, 0, 255, 0, 255);
        else if (i % 4 == 2)
            SDL_SetRenderDrawColor(ui->renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(ui->renderer, 255, 255, 0, 255);

        int x_particle = x + 100 * cos(ui->tick * speed * i);
        int y_particle = y + 100 * sin(ui->tick * speed * i);
        SDL_Rect particle = {x_particle, y_particle, 10, 10};
        SDL_RenderFillRect(ui->renderer, &particle);
    }
}