#define SDL_COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "headers/player.h"

struct pos_s
{
    int x;
    int y;
};
typedef struct pos_s pos_t;

struct ui_s
{
    /* SDL */
    SDL_Window *window;
    SDL_Renderer *renderer;
    int screen_w;
    int screen_h;
    SDL_Event event;
    pos_t mouse_pos;

    /* Textures */
    SDL_Texture *front_card_textures[7]; 
    SDL_Texture *back_flag_textures[7];  
    SDL_Texture *back_card_texture[2];
    SDL_Texture *player_textures[4];
    SDL_Texture *interface_textures[8];
    SDL_Texture *button_textures[8];
    SDL_Texture *victory[4];
    SDL_Texture *score_textures[10];

    SDL_Texture *active_player_textures[4];
    SDL_Texture *triangle;

    /* Animation */
    bool animate[3]; // 0: flip_the_card, 1: draw_particles, 3:vol de cartes
    int click_x;
    int click_y;
    unsigned long long last_tick;
    unsigned long long tick;
    unsigned long long delta_t;
    bool follow_mouse;

    bool in_pause;
    bool program_on;

    int ticks_stealing_init;
};
typedef struct ui_s ui_t;

ui_t *create_ui();
void refresh_input(game_t * game, ui_t *ui, int *input);
void free_ui(ui_t *ui);
void game_interact(int *input, game_t *game, ui_t *ui);