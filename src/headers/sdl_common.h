#define SDL_COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "headers/struct.h"

struct ui_s
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int screen_w;
    int screen_h;
    SDL_Event event;

    /* Textures */
    SDL_Texture *front_card_textures[8]; // front_card_textures[0] inutilisé
    SDL_Texture *back_flag_textures[8];  // back_flag_textures[0] inutilisé
    SDL_Texture *back_card_texture;
    SDL_Texture *player_textures[5]; // player_textures[0] inutilisé
    SDL_Texture *interface_textures[4];
    SDL_Texture *button_textures[8];

    unsigned long long tick;

    bool in_pause;
    bool program_on;
};
typedef struct ui_s ui_t;

ui_t *create_ui();
void refresh_input(ui_t *ui, int *input, game_t *game);
void free_ui(ui_t *ui);