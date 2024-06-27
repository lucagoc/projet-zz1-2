#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "headers/sdl_common.h"
#include "headers/animation.h"

#define FLAG_WIDTH 434
#define FLAG_HEIGHT 396

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

// Affiche le fond d'écran.
// Affiche le fond d'écran.
void draw_background(ui_t *ui)
{

    SDL_SetRenderDrawColor(ui->renderer, 220, 220, 255, 255);
    SDL_RenderClear(ui->renderer);

    int mouseX;
    int mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    unsigned long tick = SDL_GetTicks();

    mouseX -= ui->screen_w / 2;
    mouseY -= ui->screen_h / 2;

    float parallX = 0.03;
    float parallY = 0.04;

    SDL_Rect layer0_draw = {0, 0, ui->screen_w, ui->screen_h};

    SDL_Rect layer1_draw = {-3200 + ((int)round(tick * 0.02)) % 6400 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};
    SDL_Rect layer2_draw = {-3200 + ((int)round(tick * 0.05)) % 6400 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};
    SDL_Rect layer3_draw = {-3200 + ((int)round(tick * 0.15)) % 6400 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};
  
    SDL_Rect layer1_draw2 = {-6400+ ((int)round(tick * 0.02)) % 7600 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};
    SDL_Rect layer2_draw2 = {-6400+ ((int)round(tick * 0.05)) % 7600 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};
    SDL_Rect layer3_draw2 = {-6400+ ((int)round(tick * 0.15)) % 7600 + (int)round(mouseX * parallX),  (int)round(mouseY * parallY),   3200,   900};

    SDL_RenderCopy(ui->renderer, ui->background[0], NULL, &layer0_draw);
    SDL_RenderCopy(ui->renderer, ui->background[1], NULL, &layer1_draw);
    SDL_RenderCopy(ui->renderer, ui->background[2], NULL, &layer2_draw);
    SDL_RenderCopy(ui->renderer, ui->background[3], NULL, &layer3_draw);
    
    SDL_RenderCopy(ui->renderer, ui->background[1], NULL, &layer1_draw2);
    SDL_RenderCopy(ui->renderer, ui->background[2], NULL, &layer2_draw2);
    SDL_RenderCopy(ui->renderer, ui->background[3], NULL, &layer3_draw2);
}

void draw_player_tank(ui_t *ui, player_t *player, int x, int y)
{
    int descale = 7;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < player->tank[i]; j++)
        {
            int card_x = x + i * card_width + i * 10;
            int card_y = y + j * 10;

            if (ui->animations[1]->playing && ui->animations[1]->param[0] == i && ui->animations[1]->param[1] == j)
            {
                //
            }
            else
            {
                SDL_Rect draw_card_rect = {card_x, card_y, card_width, card_height};
                SDL_RenderCopy(ui->renderer, ui->front_card_textures[i], NULL, &draw_card_rect);
            }
        }
    }

    animation_runtime(ui, ui->animations[1], fct_move_animation); // Joue les animations de déplacement des cartes.
}

// teste la victoire
void draw_victory(ui_t *ui, game_t *game)
{
    SDL_Rect drawvic = {650, 200, 300, 100};

    if (game->win > -1)
    {
        SDL_RenderCopy(ui->renderer, ui->victory[game->win], NULL, &drawvic);
    }
}

void draw_score(ui_t *ui, game_t *game, int player, int x, int y)
{
    SDL_Rect units = {x, y, 30, 50};
    SDL_RenderCopy(ui->renderer, ui->score_textures[game->players[player]->score / 10], NULL, &units);
    SDL_Rect tens = {x + 30, y, 30, 50};
    SDL_RenderCopy(ui->renderer, ui->score_textures[game->players[player]->score % 10], NULL, &tens);
}

// Affiche les joueurs. Affiches les joueurs sur chaque côté de l'écran, la pile étant au centre.
void draw_players(ui_t *ui, game_t *game)
{
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, 255);

    int size_length = ui->screen_w / 2 - 90;
    int size_height = 200;
    int padding = 25;

    SDL_Color inactive = {128, 128, 128, 255};
    SDL_Color active = {255, 0, 0, 150};

    if (game->players[0] != NULL) // En bas à gauche
    {
        if (game->player_action == 0)
            SDL_SetRenderDrawColor(ui->renderer, active.r, active.g, active.b, active.a);
        else
            SDL_SetRenderDrawColor(ui->renderer, inactive.r, inactive.g, inactive.b, inactive.a);
        SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);

        // SDL_RenderFillRect(ui->renderer, &player_background);
        draw_player_tank(ui, game->players[0], padding, ui->screen_h - size_height + padding);
        draw_score(ui, game, 0, 10, ui->screen_h - 50);
        if (game->player_action == 0)
        {
            SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
        // SDL_Rect player_avatar_rect = {ui->screen_w - 40, ui->screen_h - 80, 40, 40};
        // SDL_RenderCopy(ui->renderer, ui->player_textures[0], NULL, &player_avatar_rect); Pour l'affichage d'un éventuel avatar.
    }

    if (game->players[1] != NULL) // En haut à droite
    {
        if (game->player_action == 1)
            SDL_SetRenderDrawColor(ui->renderer, active.r, active.g, active.b, active.a);
        else
            SDL_SetRenderDrawColor(ui->renderer, inactive.r, inactive.g, inactive.b, inactive.a);
        SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);

        draw_player_tank(ui, game->players[1], ui->screen_w - size_length + padding, padding);
        draw_score(ui, game, 1, ui->screen_w - 60, 10);
        if (game->player_action == 1)
        {
            SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
    }

    if (game->players[2] != NULL) // En haut à gauche
    {
        if (game->player_action == 2)
            SDL_SetRenderDrawColor(ui->renderer, active.r, active.g, active.b, active.a);
        else
            SDL_SetRenderDrawColor(ui->renderer, inactive.r, inactive.g, inactive.b, inactive.a);
        SDL_Rect player_background = {0, 0, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);
        draw_player_tank(ui, game->players[2], padding, padding);
        draw_score(ui, game, 2, 10, 10);
        if (game->player_action == 2)
        {
            SDL_Rect player_background = {0, 0, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
    }

    if (game->players[3] != NULL) // En bas à droite
    {
        if (game->player_action == 3)
            SDL_SetRenderDrawColor(ui->renderer, active.r, active.g, active.b, active.a);
        else
            SDL_SetRenderDrawColor(ui->renderer, inactive.r, inactive.g, inactive.b, inactive.a);
        SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);
        draw_player_tank(ui, game->players[3], ui->screen_w - size_length + padding, ui->screen_h - size_height + padding);
        draw_score(ui, game, 3, ui->screen_w - 60, ui->screen_h - 50);
        if (game->player_action == 3)
        {
            SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
    }
}

// Affiche la pioche
void draw_draw_card(ui_t *ui, game_t *game)
{
    int x;
    int y;
    if (ui->animations[0]->playing)
    {
        x = ui->animations[0]->pos.x - CARD_WIDTH / 6;
        y = ui->animations[0]->pos.y - CARD_HEIGHT / 6;
    }
    else
    {
        x = ui->screen_w / 2 - CARD_WIDTH / 6;
        y = ui->screen_h / 2 - CARD_HEIGHT / 6;
    }

    
        for (int i = 0; i < 3; i++) // 3 = nombre d'élément derrière la carte.
        {
            SDL_Rect draw_indicator_rect = {x + i * 20 + 7, y + 30 + i * 45, FLAG_WIDTH / 3, FLAG_HEIGHT / 3};
            SDL_RenderCopy(ui->renderer, ui->back_flag_textures[game->back_card_color[i]], NULL, &draw_indicator_rect);
        }

    // Affiche la carte
    SDL_Rect draw_pile_rect = {x, y, CARD_WIDTH / 3, CARD_HEIGHT / 3};
    SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &draw_pile_rect);

    // Animation 
    animation_runtime(ui, ui->animations[0], fct_anim_particles);
    animation_runtime(ui, ui->animations[2], fct_anim_flip);
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

/*------------------------------------------ PAUSE -----------------------------------------------*/
void draw_background_pause(ui_t *ui)
{

    srand(time(NULL));

    // Couleurs de départ et de fin pour le gradient

    int start_R = rand() % 256;
    int start_G = rand() % 256;
    int start_B = rand() % 256;

    int end_R = rand() % 256;
    int end_G = rand() % 256;
    int end_B = rand() % 256;

    int screen_height = ui->screen_h;
    int screen_width = ui->screen_w;

    // Calculer les incréments de couleur

    float step_R = (float)(end_R - start_R) / screen_height;
    float step_G = (float)(end_G - start_G) / screen_height;
    float step_B = (float)(end_B - start_B) / screen_height;

    // Dessiner le gradient

    for (int y = 0; y < screen_height; ++y)
    {
        int current_R = start_R + (int)(step_R * y);
        int current_G = start_G + (int)(step_G * y);
        int current_B = start_B + (int)(step_B * y);

        SDL_SetRenderDrawColor(ui->renderer, current_R, current_G, current_B, 255);

        SDL_RenderDrawLine(ui->renderer, 0, y, screen_width, y);
    }
}

void draw_menu_pause(ui_t *ui)
{
    draw_background_pause(ui);

    // Charger les textures des boutons
    SDL_Texture *continue_button = ui->pause_texture[0];
    SDL_Texture *quit_button = ui->pause_texture[1];

    int button_w = 2000;
    int button_h = 900;

    // Calculer les positions pour centrer les boutons
    int button_x = (ui->screen_w - button_w) / 2;
    int continue_button_y = (ui->screen_h - button_h) / 4;
    int quit_button_y = 3 * (ui->screen_h - button_h) / 4;

    // Définir les rectangles de destination pour les boutons
    SDL_Rect dest_rect_continue = {button_x, continue_button_y, button_w, button_h};
    SDL_Rect dest_rect_quit = {button_x, quit_button_y, button_w, button_h};

    // Dessiner les boutons
    SDL_RenderCopy(ui->renderer, continue_button, NULL, &dest_rect_continue);
    SDL_RenderCopy(ui->renderer, quit_button, NULL, &dest_rect_quit);
}
/*------------------------------------------ PAUSE -----------------------------------------------*/

// Affiche tout les éléments du jeu.
void draw(ui_t *ui, game_t *game)
{
    if (ui->in_pause)
    {
        // Affiche le menu pause
        draw_menu_pause(ui);
    }
    else
    {
        draw_background(ui);
        draw_players(ui, game);
        draw_active_player(ui, game);
        draw_logo(ui);
        draw_draw_card(ui, game);
    }

    if (game->win > -1)
    {
        draw_victory(ui, game);
    }


    // Affichage
    SDL_RenderPresent(ui->renderer);
    SDL_Delay(15); // ~ 60 FPS
}