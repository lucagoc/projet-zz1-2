#include <stdbool.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "headers/sdl_common.h"
#include "headers/animation.h"

#define SDL_DELAY 15

#define FLAG_WIDTH 434
#define FLAG_HEIGHT 396

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

#define TANK_HEIGHT 200
#define TANK_WIDTH 200

#define AVATAR_SIZE 100

#define SCORE_WIDTH 30
#define SCORE_HEIGHT 50

#define LOGO_SIZE 180

/**
 * @brief Affiche le fond d'écran
 *
 * @param ui
 */
void draw_background(ui_t *ui)
{
    SDL_SetRenderDrawColor(ui->renderer, 220, 220, 255, 255);
    SDL_RenderClear(ui->renderer);

    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    unsigned long tick = SDL_GetTicks();

    mouse_x -= ui->screen_w / 2;
    mouse_y -= ui->screen_h / 2;

    float parall_x = 0.03;
    float parall_y = 0.04;

    int layer0_w = ui->screen_w * 4;
    int layer0_h = ui->screen_h;

    SDL_Rect layer0_draw = {0, 0, ui->screen_w, ui->screen_h};

    SDL_Rect layer1_draw = {-layer0_w / 2 + ((int)round(tick * 0.02)) % layer0_w + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};
    SDL_Rect layer2_draw = {-layer0_w / 2 + ((int)round(tick * 0.05)) % layer0_w + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};
    SDL_Rect layer3_draw = {-layer0_w / 2 + ((int)round(tick * 0.15)) % layer0_w + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};

    SDL_Rect layer1_draw2 = {-ui->screen_w + ((int)round(tick * 0.02)) % 7600 + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};
    SDL_Rect layer2_draw2 = {-ui->screen_w + ((int)round(tick * 0.05)) % 7600 + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};
    SDL_Rect layer3_draw2 = {-ui->screen_w + ((int)round(tick * 0.15)) % 7600 + (int)round(mouse_x * parall_x), (int)round(mouse_y * parall_y), layer0_w / 2, layer0_h};

    SDL_RenderCopy(ui->renderer, ui->background[0], NULL, &layer0_draw);
    SDL_RenderCopy(ui->renderer, ui->background[1], NULL, &layer1_draw);
    SDL_RenderCopy(ui->renderer, ui->background[2], NULL, &layer2_draw);
    SDL_RenderCopy(ui->renderer, ui->background[3], NULL, &layer3_draw);

    SDL_RenderCopy(ui->renderer, ui->background[1], NULL, &layer1_draw2);
    SDL_RenderCopy(ui->renderer, ui->background[2], NULL, &layer2_draw2);
    SDL_RenderCopy(ui->renderer, ui->background[3], NULL, &layer3_draw2);
}

/**
 * @brief Affiche le tank d'un joueur
 *
 * @param ui
 * @param player
 * @param x
 * @param y
 */
void draw_player_tank(ui_t *ui, player_t *player, int x, int y)
{
    int descale = 7;
    int padding = 10;
    int card_width = CARD_WIDTH / descale;
    int card_height = CARD_HEIGHT / descale;

    for (int i = 0; i < NUM_COLORS; i++)
    {
        if (player->tank[i] > 0)
        {
            for (int j = 0; j < player->tank[i]; j++)
            {
                int card_x = x + i * card_width + i * padding;
                int card_y = y + j * padding;
                SDL_Rect draw_card_rect = {card_x, card_y, card_width, card_height};
                SDL_RenderCopy(ui->renderer, ui->front_card_textures[i], NULL, &draw_card_rect);
            }
        }
    }

    animation_runtime(ui, ui->animations[1], fct_move_animation); // Joue les animations de déplacement des cartes.
}

/**
 * @brief Affiche le message de victoire
 *
 * @param ui
 * @param game
 */
void draw_victory(ui_t *ui, game_t *game)
{
    SDL_Rect vic_rect = {650, 200, 300, 100};
    SDL_RenderCopy(ui->renderer, ui->victory[game->win], NULL, &vic_rect);
    ui->animations[3]->size.x = ui->screen_w;
    ui->animations[3]->size.y = ui->screen_h;
    fct_anim_confettis(ui->animations[3], ui->renderer, SDL_GetTicks());
}

/**
 * @brief Affiche le score d'un joueur
 *
 * @param ui
 * @param game
 * @param player
 * @param pos
 */
void draw_score(ui_t *ui, game_t *game, int player, pos_t pos)
{
    SDL_Rect units = {pos.x, pos.y, SCORE_WIDTH, SCORE_HEIGHT};
    SDL_RenderCopy(ui->renderer, ui->score_textures[game->players[player]->score / 10], NULL, &units);
    SDL_Rect tens = {pos.x + SCORE_WIDTH, pos.y, SCORE_WIDTH, SCORE_HEIGHT};
    SDL_RenderCopy(ui->renderer, ui->score_textures[game->players[player]->score % 10], NULL, &tens);
}

/**
 * @brief Affiche les joueurs
 *
 * @param ui
 * @param game
 */
void draw_players(ui_t *ui, game_t *game)
{
    SDL_SetRenderDrawColor(ui->renderer, 255, 255, 255, 255);

    int size_length = ui->screen_w / 2 - 90;
    int size_height = 200;
    int padding = 25;

    if (game->players[0] != NULL) // En bas à gauche
    {
        SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);

        draw_player_tank(ui, game->players[0], padding, ui->screen_h - size_height + padding);
        draw_score(ui, game, 0, (pos_t){10, ui->screen_h - SCORE_HEIGHT});
        if (game->player_action == 0)
        {
            SDL_Rect player_background = {0, ui->screen_h - size_height, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
        SDL_Rect player_avatar_rect = {0, ui->screen_h - (AVATAR_SIZE + TANK_HEIGHT), AVATAR_SIZE, AVATAR_SIZE};
        SDL_RenderCopy(ui->renderer, ui->player_textures[0], NULL, &player_avatar_rect);
    }

    if (game->players[1] != NULL) // En haut à droite
    {
        SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);

        draw_player_tank(ui, game->players[1], ui->screen_w - (size_length) + padding, padding);
        draw_score(ui, game, 1, (pos_t){ui->screen_w - (SCORE_WIDTH * 2), 10});
        if (game->player_action == 1)
        {
            SDL_Rect player_background = {ui->screen_w - size_length, 0, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
        SDL_Rect player_avatar_rect = {ui->screen_w - AVATAR_SIZE, TANK_HEIGHT, AVATAR_SIZE, AVATAR_SIZE};
        SDL_RenderCopy(ui->renderer, ui->player_textures[1], NULL, &player_avatar_rect);
    }

    if (game->players[2] != NULL) // En haut à gauche
    {
        SDL_Rect player_background = {0, 0, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);
        draw_player_tank(ui, game->players[2], padding, padding);
        draw_score(ui, game, 2, (pos_t){10, 10});
        if (game->player_action == 2)
        {
            SDL_Rect player_background = {0, 0, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
        SDL_Rect player_avatar_rect = {0, TANK_HEIGHT, AVATAR_SIZE, AVATAR_SIZE};
        SDL_RenderCopy(ui->renderer, ui->player_textures[2], NULL, &player_avatar_rect);
    }

    if (game->players[3] != NULL) // En bas à droite
    {
        SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
        SDL_RenderCopy(ui->renderer, ui->triangle, NULL, &player_background);
        draw_player_tank(ui, game->players[3], ui->screen_w - size_length + padding, ui->screen_h - size_height + padding);
        draw_score(ui, game, 3, (pos_t){ui->screen_w - (SCORE_WIDTH * 2), ui->screen_h - SCORE_HEIGHT});
        if (game->player_action == 3)
        {
            SDL_Rect player_background = {ui->screen_w - size_length, ui->screen_h - size_height, size_length, size_height};
            SDL_RenderCopy(ui->renderer, ui->active_player_textures[game->player_action], NULL, &player_background);
        }
        SDL_Rect player_avatar_rect = {ui->screen_w - AVATAR_SIZE, ui->screen_h - (AVATAR_SIZE + TANK_HEIGHT), AVATAR_SIZE, AVATAR_SIZE};
        SDL_RenderCopy(ui->renderer, ui->player_textures[3], NULL, &player_avatar_rect);
    }
}

/**
 * @brief Affiche la carte en haut de pioche
 *
 * @param ui
 * @param game
 */
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

    if (!ui->animations[2]->playing)
    {
        for (int i = 0; i < 3; i++) // 3 = nombre d'élément derrière la carte.
        {
            SDL_Rect draw_indicator_rect = {x + i * 20 + 7, y + 30 + i * 45, FLAG_WIDTH / 3, FLAG_HEIGHT / 3};
            SDL_RenderCopy(ui->renderer, ui->back_flag_textures[game->back_card_color[i]], NULL, &draw_indicator_rect);
        }

        // Affiche la carte
        SDL_Rect draw_pile_rect = {x, y, CARD_WIDTH / 3, CARD_HEIGHT / 3};
        SDL_RenderCopy(ui->renderer, ui->back_card_texture[0], NULL, &draw_pile_rect);
    }

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
    SDL_Rect draw_logo_rect = {10, ui->screen_h / 2 - 100, LOGO_SIZE, LOGO_SIZE};
    SDL_RenderCopy(ui->renderer, ui->interface_textures[0], NULL, &draw_logo_rect);
}

/*------------------------------------------ PAUSE -----------------------------------------------*/
void draw_background_pause(ui_t *ui)
{
    SDL_SetRenderDrawColor(ui->renderer, 0, 0, 0, 200);
    SDL_Rect rect = {0, 0, ui->screen_w, ui->screen_h};
    SDL_RenderFillRect(ui->renderer, &rect);
}

void draw_menu_pause(ui_t *ui)
{
    draw_background_pause(ui);

    // Charger les textures des boutons
    SDL_Texture *continue_button = ui->pause_texture[0];
    SDL_Texture *quit_button = ui->pause_texture[1];

    int button_w = 300;
    int button_h = 100;

    // Calculer les positions pour centrer les boutons
    int button_x = (ui->screen_w - button_w) / 2;
    int continue_button_y = 2 * (ui->screen_h - button_h) / 5;
    int quit_button_y = 4 * (ui->screen_h - button_h) / 5;

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

    draw_background(ui);
    draw_players(ui, game);
    draw_active_player(ui, game);
    draw_logo(ui);
    draw_draw_card(ui, game);

    if (ui->in_pause)
    {
        draw_menu_pause(ui);
    }

    if (game->win > -1)
    {
        draw_victory(ui, game);
    }

    SDL_RenderPresent(ui->renderer);
    SDL_Delay(SDL_DELAY);
}