#include <SDL2/SDL.h>
#include <stdbool.h>

#include "headers/sdl_common.h"

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

#define COLOR_RED 166,68,65,255
#define COLOR_PURPLE 110,65,166,255
#define COLOR_BLUE 65,114,165,255
#define COLOR_ORANGE 222,129,30,255
#define COLOR_YELLOW 155,143,0,255
#define COLOR_PINK 187,82,144,255
#define COLOR_GREEN 66,177,66,255

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

        int x = fmod((ui->delta_t * 10 * i * speed), ui->screen_w);
        int y = fmod((ui->delta_t * 10 * i * speed), ui->screen_h);
        SDL_Rect confetti = {x, y, 10, 10};
        SDL_RenderFillRect(ui->renderer, &confetti);
    }
}

// Donne un effet de flip à une carte.
void flip_the_card(ui_t *ui, game_t *game, int x, int y)
{
    int descale = 6;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    float speed = 0.2;
    float anime_tick = (ui->delta_t * speed);

    if (anime_tick < 200)
    {
        SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, anime_tick * 255 / 200);
        SDL_Rect background = {x - card_width / 2, y - card_height/2 + 20, card_width, card_height - 40};
        SDL_RenderFillRect(ui->renderer, &background);
        SDL_Rect card = {x - card_width / 2, y - card_height/2, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &card);
    }
    else if (anime_tick < 300)
    {
        card_width = (CARD_WIDTH * (300 - anime_tick) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {x - card_width / 2, y - card_height/2, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[1], NULL, &card);
    }
    else if (anime_tick < 400)
    {
        card_width = (CARD_WIDTH * (anime_tick - 300) / 100) / descale;
        card_height = CARD_HEIGHT / descale;
        SDL_Rect card = {x - card_width / 2, y - card_height/2, card_width, card_height};
        SDL_RenderCopy(ui->renderer, ui->front_card_textures[game->drawn_card_color], NULL, &card);
    }
    else
    {
        ui->animate[0] = false;
        ui->animate[1] = false;
        ui->last_tick = SDL_GetTicks();
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
void draw_particles(ui_t *ui, game_t *game, int x, int y)
{
    // Correction d'un problème d'allignement;
    x -= 4;
    y -= 1;

    // Définir les couleurs des particules en fonctions de l'arrière des cartes.
    SDL_Color colors[7] = {
        {COLOR_YELLOW},
        {COLOR_GREEN},
        {COLOR_PURPLE},
        {COLOR_PINK},
        {COLOR_BLUE},
        {COLOR_ORANGE},
        {COLOR_RED}};

    int particle_number = 50;
    float speed = 0.0001;

    int a = game->draw_pile->card->back[0];
    int b = game->draw_pile->card->back[1];
    int c = game->draw_pile->card->back[2];

    for (int i = 0; i < particle_number; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if (i % 3 == 0)
            SDL_SetRenderDrawColor(ui->renderer, colors[a].r, colors[a].g, colors[a].b, colors[a].a);
        else if (i % 3 == 1)
            SDL_SetRenderDrawColor(ui->renderer, colors[b].r, colors[b].g, colors[b].b, colors[b].a);
        else
            SDL_SetRenderDrawColor(ui->renderer, colors[c].r, colors[c].g, colors[c].b, colors[c].a);

        int x_particle = x + 100 * cos(ui->delta_t * speed * i);
        int y_particle = y + 100 * sin(ui->delta_t * speed * i);
        SDL_Rect particle = {x_particle, y_particle, 10, 10};
        SDL_RenderFillRect(ui->renderer, &particle);
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

// animation de vol de carte
void draw_steal(ui_t *ui, game_t *game)
{

    float speed = 0.2;
    float anime_tick = (ui->delta_t * speed);

    if (ui->ticks_stealing_init==0){

        //on démarre l'animation
        ui->animate[2]=1;
        ui->ticks_stealing_init=1;
        ui->last_tick = SDL_GetTicks();


    } else if (anime_tick <1000){
        //on joue l'animation pendant 1000 ticks

        int size_length = ui->screen_w / 2 - 90;
        int size_height = 200;

        int debx;
        int finx;
        int deby;
        int finy;

        float speed = 0.001;
        int param = anime_tick * speed;
        int number_cards_stolen = game->players[game->stealing]->tank[game->drawn_card_color]; //nombre de cartes volées

        if (game->stealing == 0)  //selon la position du volé on définit d'où partent les cartes
        {
            debx = 0;
            deby = ui->screen_h - size_height;
        }
        else if (game->stealing == 1)
        {
            debx = ui->screen_w - size_length;
            deby = 0;
        }
        else if (game->stealing == 2)
        {
            debx = 0;
            deby = 0;
        }
        else if (game->stealing == 3)
        {
            debx = ui->screen_w - size_length;
            deby = ui->screen_h - size_height;
        }

        if (game->player_action == 0) //selon la position du voleur on définit où arrivent les cartes
        {
            finx = 0;
            finy = ui->screen_h - size_height;
        }
        else if (game->player_action == 1)
        {
            finx = ui->screen_w - size_length;
            finy = 0;
        }
        else if (game->player_action == 2)
        {
            finx = 0;
            finy = 0;
        }
        else if (game->player_action == 3)
        {
            finx = ui->screen_w - size_length;
            finy = ui->screen_h - size_height;
        }

        for (int i = 0; i < number_cards_stolen; i++)
        {

            draw_face(ui, game->drawn_card_color, param * finx + (100 - param) * debx, param * finy + (100 - param) * deby +i*20);
        }

    } else {
        //l'animation termine
        ui->animate[2]=0;
        fprintf(stderr, "[DEBUG] steal_card : player %d, card %d\n", game->stealing, game->drawn_card_color);
        game->players[game->player_action]->tank[game->drawn_card_color] += game->players[game->stealing]->tank[game->drawn_card_color] + 1; // on récupère les cartes volées
        game->players[game->stealing]->tank[game->drawn_card_color] = 0;
        game->stealing=0;
        ui->ticks_stealing_init=0; //on remet à 0 pour la prochaine animation
        game->player_action = (game->player_action + 1) % 4; //on passe au joueur suivant
        ui->last_tick = SDL_GetTicks();

    }

}