#define SDL_COMMON_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>

#include "headers/struct.h"

#define NUM_PLAYERS 4
#define NUM_COLORS 7

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
    pos_t pos;                     // Position de l'animation
    pos_t target;                  // Position cible (si besoin)
    long unsigned start_frame;     // Frame de départ
    long unsigned number_of_frame; // Pendant combien de frame est jouée l'animation
    float speed;                   // Vitesse
    bool loop;                     // Doit-elle boucler ?
    bool playing;                  // L'animation est-elle terminée ?
    SDL_Texture **texture;         // Tableau de texture pour l'animation
    pos_t size;                    // Taille de la texture
    int param[10];                 // Valeur qui peuvent être utilisée pour passer des informations
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

    /* Textures */
    SDL_Texture *front_card_textures[NUM_COLORS];
    SDL_Texture *back_flag_textures[NUM_COLORS];
    SDL_Texture *back_card_texture[3];
    SDL_Texture *player_textures[NUM_PLAYERS];
    SDL_Texture *interface_textures[4];
    SDL_Texture *button_textures[8];
    SDL_Texture *victory[4];
    SDL_Texture *score_textures[10];
    SDL_Texture *background[4];
    SDL_Texture *active_player_textures[4];
    SDL_Texture *triangle;

    SDL_Texture *pause_texture[2];

    /* Animation */
    anim_props_t **animations; // Tableau d'animation.

    /* Sons */
    Mix_Chunk *music[2];
    bool congrats_played;

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
    int delay_input; // input à donner à la fin de l'animation
};
typedef struct ui_input_s ui_input_t;

ui_t *create_ui();
void refresh_input(ui_t *ui, ui_input_t *ui_input);
void free_ui(ui_t *ui);
void game_interact(int input, game_t *game);
int player_clicked(pos_t click);
ui_input_t *create_ui_input();
void init_animation(anim_props_t *animation, pos_t pos, int number_of_frame);
bool stack_clicked(ui_input_t *ui_input);
int process_input(ui_input_t *ui_input, game_t *game, ui_t *ui);
int process_input_robot(ui_input_t *ui_input, game_t *game, ui_t *ui);
void play_sound(ui_t *ui, game_t *game);