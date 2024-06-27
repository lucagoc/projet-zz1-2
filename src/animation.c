#include <SDL2/SDL.h>
#include <stdbool.h>

#include "headers/sdl_common.h"

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

#define NUM_COLORS 7

#define COLOR_RED 166, 68, 65, 255
#define COLOR_PURPLE 110, 65, 166, 255
#define COLOR_BLUE 65, 114, 165, 255
#define COLOR_ORANGE 222, 129, 30, 255
#define COLOR_YELLOW 155, 143, 0, 255
#define COLOR_PINK 187, 82, 144, 255
#define COLOR_GREEN 66, 177, 66, 255

double ease_out_quint(double x)
{
    return 1 - pow(1 - x, 5);
}

void start_animation(anim_props_t *animation, long unsigned delay)
{
    animation->start_frame = SDL_GetTicks() + delay;
    animation->playing = true;
}

void end_animation(anim_props_t *animation)
{
    animation->loop = false;
    animation->playing = false;
}

/*
 * Joue l'animation donnée en entrée
 */
void animation_runtime(ui_t *ui, anim_props_t *animation, void *(func_anim)(anim_props_t *, SDL_Renderer *, long unsigned))
{
    long unsigned actual_frame = SDL_GetTicks();
    long unsigned delta_frame = actual_frame - animation->start_frame;
    if(animation->start_frame > actual_frame)
        return;
    if (animation->number_of_frame < delta_frame)
    {
        animation->playing = false;
    }
    if (animation->playing)
    {
        func_anim(animation, ui->renderer, delta_frame);
    }
    if (animation->loop)
    {
        if (!animation->playing)
        {
            animation->start_frame = actual_frame;
            animation->playing = true;
        }
    }
}

void init_animation(anim_props_t *animation, pos_t pos, int number_of_frame)
{
    animation->pos.x = pos.x;
    animation->pos.y = pos.y;
    animation->start_frame = -100000;
    animation->number_of_frame = number_of_frame;
    animation->playing = false;
}

void fct_move_animation(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame)
{
    float x = anim->pos.x + (anim->target.x - anim->pos.x) * ease_out_quint(frame /(double)anim->number_of_frame);
    float y = anim->pos.y + (anim->target.y - anim->pos.y) * ease_out_quint(frame /(double)anim->number_of_frame);

    // Affichage de l'animation
    SDL_Rect rect = {x, y, anim->size.x, anim->size.y};
    SDL_RenderCopy(renderer, anim->texture[anim->param[0]], NULL, &rect);
}

void fct_anim_confettis(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame)
{
    for (int i = 0; i < 100; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if (i % 4 == 0)
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        else if (i % 4 == 1)
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else if (i % 4 == 2)
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

        int x = fmod((frame * 10 * i), anim->size.x);
        int y = fmod((frame * 10 * i), anim->size.y);
        SDL_Rect confetti = {x, y, 10, 10};
        SDL_RenderFillRect(renderer, &confetti);
    }
}

// Donne un effet de flip à une carte.
void fct_anim_flip(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame)
{
    int descale = 6;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    float speed = 0.2;
    float anime_tick = (frame * speed);

    if (anime_tick < 200)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, anime_tick * 255 / 200);
        SDL_Rect background = {anim->pos.x - card_width / 2, anim->pos.y - card_height / 2 + 20, card_width, card_height - 40};
        SDL_RenderFillRect(renderer, &background);
        SDL_Rect card = {anim->pos.x - card_width / 2, anim->pos.y - card_height / 2, card_width, card_height};
        SDL_RenderCopy(renderer, anim->texture[7], NULL, &card);
    }
    else if (anime_tick < 300)
    {
        card_width = (CARD_WIDTH * (300 - anime_tick) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {anim->pos.x - card_width / 2, anim->pos.y - card_height / 2, card_width, card_height};
        SDL_RenderCopy(renderer, anim->texture[8], NULL, &card);
    }
    else if (anime_tick < 400)
    {
        card_width = (CARD_WIDTH * (anime_tick - 300) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {anim->pos.x - card_width / 2, anim->pos.y - card_height / 2, card_width, card_height};
        SDL_RenderCopy(renderer, anim->texture[anim->param[0]], NULL, &card);
    }
    else
    {
        card_width = CARD_WIDTH / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {anim->pos.x - card_width / 2, anim->pos.y - card_height / 2, card_width, card_height};
        SDL_RenderCopy(renderer, anim->texture[anim->param[0]], NULL, &card);
    }
}

// Affiche des particules tournant autour d'un point
void fct_anim_particles(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame)
{
    // Correction d'un problème d'allignement;
    int x = anim->pos.x;
    int y = anim->pos.y;
    x -= 4;
    y -= 1;

    // Définir les couleurs des particules en fonctions de l'arrière des cartes.
    SDL_Color colors[NUM_COLORS] = {
        {COLOR_YELLOW},
        {COLOR_GREEN},
        {COLOR_PURPLE},
        {COLOR_PINK},
        {COLOR_BLUE},
        {COLOR_ORANGE},
        {COLOR_RED}};

    int particle_number = 50;
    float speed = 0.0001;

    int a = anim->param[0];
    int b = anim->param[1];
    int c = anim->param[2];

    for (int i = 0; i < particle_number; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if (i % 3 == 0)
            SDL_SetRenderDrawColor(renderer, colors[a].r, colors[a].g, colors[a].b, colors[a].a);
        else if (i % 3 == 1)
            SDL_SetRenderDrawColor(renderer, colors[b].r, colors[b].g, colors[b].b, colors[b].a);
        else
            SDL_SetRenderDrawColor(renderer, colors[c].r, colors[c].g, colors[c].b, colors[c].a);

        int x_particle = x + 100 * cos(frame * speed * i);
        int y_particle = y + 100 * sin(frame * speed * i);
        SDL_Rect particle = {x_particle, y_particle, 10, 10};
        SDL_RenderFillRect(renderer, &particle);
    }
}

void draw_face(ui_t *ui, int color, int x, int y)
{
    int descale = 7;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    SDL_Rect draw_card_rect = {x, y, card_width, card_height};
    SDL_RenderCopy(ui->renderer, ui->front_card_textures[color], NULL, &draw_card_rect);
}
