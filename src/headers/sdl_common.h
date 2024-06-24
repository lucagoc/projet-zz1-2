#define SDL_COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "headers/gameplay.h"

struct ui_s
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Texture *front_card_textures[7];
    SDL_Texture *back_card_texture;
    SDL_Texture *player_textures[5];
    SDL_Texture *interface_textures[4];
    int screen_w;
    int screen_h;
    int board_size;
    bool in_pause;
    bool program_on;
};
typedef struct ui_s ui_t;

ui_t *create_ui();

