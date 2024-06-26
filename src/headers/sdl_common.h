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

/*  Type propriété d'animation
 *  (Bien dommage qu'il n'y a pas de fonction dans les structs en C :'( )
 */
struct anim_props_s
{
    pos_t pos;           // Position de l'animation
    int start_frame;     // Frame de départ
    int number_of_frame; // Pendant combien de frame est jouée l'animation
    float speed;         // Vitesse
    bool loop;           // Doit-elle boucler ?
    bool playing;        // L'animation est-elle terminée ?
};
typedef struct anim_props_s anim_props_t;

struct ui_s
{
    /* SDL */
    SDL_Window *window;
    SDL_Renderer *renderer;
    int screen_w;
    int screen_h;
    SDL_Event event;
    pos_t mouse_pos;
    pos_t click;

    /* Textures */
    SDL_Texture *front_card_textures[7];
    SDL_Texture *back_flag_textures[7];
    SDL_Texture *back_card_texture[2];
    SDL_Texture *player_textures[4];
    SDL_Texture *interface_textures[4];
    SDL_Texture *button_textures[8];
    SDL_Texture *victory[4];
    SDL_Texture *score_textures[10];

    /* Animation */
    anim_props_t *animations;   // Tableau d'animation.

    bool in_pause;
    bool program_on;

    int ticks_stealing_init;
};
typedef struct ui_s ui_t;

struct ui_input_s
{
    pos_t click;
    pos_t cursor;
    int key;
};

ui_t *create_ui();
void refresh_input(ui_t *ui, int *input, game_t *game);
void free_ui(ui_t *ui);
void game_interact(int *input, game_t *game, ui_t *ui);