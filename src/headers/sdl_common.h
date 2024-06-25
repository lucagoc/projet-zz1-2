#define SDL_COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "headers/struct.h"

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
    SDL_Texture *interface_textures[4];
    SDL_Texture *button_textures[8];
    SDL_Texture *victory[4];

    /* Animation */
    bool animate[2]; // 0: flip_the_card, 1: draw_particles
    unsigned long long tick;
    bool follow_mouse;

    bool in_pause;
    bool program_on;
};
typedef struct ui_s ui_t;

ui_t *create_ui();
void refresh_input(ui_t *ui, int *input, game_t *game);
void free_ui(ui_t *ui);