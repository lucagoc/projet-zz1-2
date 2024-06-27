#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/gameplay.h"
#include "headers/animation.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

#define CARD_WIDTH 601
#define CARD_HEIGHT 844

/**
 * @file sdl_common.c
 * @brief Fonctions communes à l'initialisation de la SDL
 * @version 1.0
 * @date 2023-06-18
 * @author Team 21
 */

/**
 * @brief Fermeture de la SDL
 *
 * @param ok Fin normale : ok = 0 ; anormale ok = 1
 * @param msg Message à afficher
 * @param window Fenêtre à fermer
 * @param renderer Renderer à fermer
 */

void end_sdl(char ok, char const *msg, SDL_Window *window, SDL_Renderer *renderer)
{
    char msg_formated[255];
    int l;

    if (!ok)
    {
        strncpy(msg_formated, msg, 250);
        l = strlen(msg_formated);
        strcpy(msg_formated + l, " : %s\n");

        SDL_Log(msg_formated, SDL_GetError());
    }

    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    TTF_Quit();
    SDL_Quit();

    if (!ok)
    {
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Décharge toutes les textures du jeu
 *
 * @param textures Tableau de textures
 */
void unload_textures(ui_t *ui)
{
    /* --------------------------------------------- CARTES --------------------------------------------- */
    for (int i = 1; i < 8; i++)
    {
        SDL_DestroyTexture(ui->front_card_textures[i]);
    }
    SDL_DestroyTexture(ui->back_card_texture[0]);
    SDL_DestroyTexture(ui->back_card_texture[1]);

    /* --------------------------------------------- JOUEURS --------------------------------------------- */
    for (int i = 0; i < 4; i++)
    {
        // SDL_DestroyTexture(ui->player_textures[i]);
        SDL_DestroyTexture(ui->victory[i]);
    }

    /* --------------------------------------------- MENU  --------------------------------------------- */
    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(ui->interface_textures[i]);
    }

    /* --------------------------------------------- TEXTE --------------------------------------------- */
    /*for (int i = 1; i < 8; i++)
    {
        SDL_DestroyTexture(ui->interface_textures[i]);
    }*/

    /* --------------------------------------------- SCORE --------------------------------------------- */
    for (int i = 0; i < 10; i++)
    {
        SDL_DestroyTexture(ui->score_textures[i]);
    }

    /* --------------------------------------------- PAUSE --------------------------------------------- */
    for (int i = 0; i < 2; i++)
    {
        SDL_DestroyTexture(ui->pause_texture[i]);
    }
}

/**
 * @brief Charge une texture à partir d'une image
 *
 * @param file_image_name Nom du fichier image
 * @param window Fenêtre SDL
 * @param renderer Renderer SDL
 * @return SDL_Texture* Texture chargée
 */
SDL_Texture *load_texture_from_image(char *file_image_name, SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_Surface *my_image = NULL;   // Variable de passage
    SDL_Texture *my_texture = NULL; // La texture

    my_image = IMG_Load(file_image_name); // Chargement de l'image dans la surface
                                          // image=SDL_LoadBMP(file_image_name); fonction standard de la SDL,
                                          // uniquement possible si l'image est au format bmp */
    if (my_image == NULL)
        end_sdl(0, "Chargement de l'image impossible", window, renderer);

    my_texture = SDL_CreateTextureFromSurface(renderer, my_image); // Chargement de l'image de la surface vers la texture
    SDL_FreeSurface(my_image);                                     // la SDL_Surface ne sert que comme élément transitoire
    if (my_texture == NULL)
        end_sdl(0, "Echec de la transformation de la surface en texture", window, renderer);

    return my_texture;
}

SDL_Texture *render_text(const char *message, const char *font_file, SDL_Color color, int font_size, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont(font_file, font_size);
    if (!font)
    {
        SDL_Log("ERROR: Unable to open font %s: %s\n", font_file, TTF_GetError());
        return NULL;
    }
    SDL_Surface *surface = TTF_RenderText_Blended(font, message, color);
    if (!surface)
    {
        SDL_Log("ERROR: Unable to create text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);
    if (!texture)
    {
        SDL_Log("ERROR: Unable to create texture from text: %s\n", SDL_GetError());
        return NULL;
    }
    return texture;
}

/**
 * @brief Charge toutes les textures du jeu
 *
 * @param textures Tableau de textures
 * @param renderer Renderer SDL
 * @param window Fenêtre SDL
 */
void load_textures(ui_t *ui)
{
    /* --------------------------------------------- CARTES --------------------------------------------- */
    ui->front_card_textures[0] = load_texture_from_image("assets/cards/front_0.png", ui->window, ui->renderer);
    ui->front_card_textures[1] = load_texture_from_image("assets/cards/front_1.png", ui->window, ui->renderer);
    ui->front_card_textures[2] = load_texture_from_image("assets/cards/front_2.png", ui->window, ui->renderer);
    ui->front_card_textures[3] = load_texture_from_image("assets/cards/front_3.png", ui->window, ui->renderer);
    ui->front_card_textures[4] = load_texture_from_image("assets/cards/front_4.png", ui->window, ui->renderer);
    ui->front_card_textures[5] = load_texture_from_image("assets/cards/front_5.png", ui->window, ui->renderer);
    ui->front_card_textures[6] = load_texture_from_image("assets/cards/front_6.png", ui->window, ui->renderer);

    ui->back_card_texture[0] = load_texture_from_image("assets/cards/back.png", ui->window, ui->renderer);
    ui->back_card_texture[1] = load_texture_from_image("assets/cards/back_blank.png", ui->window, ui->renderer);

    ui->back_flag_textures[0] = load_texture_from_image("assets/cards/back_flag_0.png", ui->window, ui->renderer);
    ui->back_flag_textures[1] = load_texture_from_image("assets/cards/back_flag_1.png", ui->window, ui->renderer);
    ui->back_flag_textures[2] = load_texture_from_image("assets/cards/back_flag_2.png", ui->window, ui->renderer);
    ui->back_flag_textures[3] = load_texture_from_image("assets/cards/back_flag_3.png", ui->window, ui->renderer);
    ui->back_flag_textures[4] = load_texture_from_image("assets/cards/back_flag_4.png", ui->window, ui->renderer);
    ui->back_flag_textures[5] = load_texture_from_image("assets/cards/back_flag_5.png", ui->window, ui->renderer);
    ui->back_flag_textures[6] = load_texture_from_image("assets/cards/back_flag_6.png", ui->window, ui->renderer);

    /* --------------------------------------------- JOUEURS --------------------------------------------- */
    // ui->player_textures[0] = load_texture_from_image("assets/players/player_0.png", ui->window, ui->renderer);
    // ui->player_textures[1] = load_texture_from_image("assets/players/player_1.png", ui->window, ui->renderer);
    // ui->player_textures[2] = load_texture_from_image("assets/players/player_2.png", ui->window, ui->renderer);
    // ui->player_textures[3] = load_texture_from_image("assets/players/player_3.png", ui->window, ui->renderer);
    ui->victory[0] = load_texture_from_image("assets/ui/pl1.png", ui->window, ui->renderer);
    ui->victory[1] = load_texture_from_image("assets/ui/pl2.png", ui->window, ui->renderer);
    ui->victory[2] = load_texture_from_image("assets/ui/pl3.png", ui->window, ui->renderer);
    ui->victory[3] = load_texture_from_image("assets/ui/pl4.png", ui->window, ui->renderer);

    /* --------------------------------------------- MENU  --------------------------------------------- */
    ui->interface_textures[0] = load_texture_from_image("assets/ui/logo.png", ui->window, ui->renderer);

    /* --------------------------------------------- TEXTE --------------------------------------------- */
    ui->interface_textures[1] = render_text("SCORE", "assets/fonts/Vividly-Regular.otf", (SDL_Color){204, 136, 80, 255}, 24, ui->renderer);
    ui->interface_textures[2] = render_text("STEAL", "assets/fonts/Vividly-Regular.otf", (SDL_Color){204, 136, 80, 255}, 24, ui->renderer);
    ui->interface_textures[3] = render_text("Good game !", "assets/fonts/Vividly-Regular.otf", (SDL_Color){20, 0, 40, 255}, 48, ui->renderer);
    ui->interface_textures[4] = render_text("Joueur 1", "assets/fonts/Vividly-Regular.otf", (SDL_Color){20, 0, 40, 255}, 108, ui->renderer);
    ui->interface_textures[5] = render_text("Joueur 2", "assets/fonts/Vividly-Regular.otf", (SDL_Color){20, 0, 40, 255}, 108, ui->renderer);
    ui->interface_textures[6] = render_text("Joueur 3", "assets/fonts/Vividly-Regular.otf", (SDL_Color){20, 0, 40, 255}, 108, ui->renderer);
    ui->interface_textures[7] = render_text("Joueur 4", "assets/fonts/Vividly-Regular.otf", (SDL_Color){20, 0, 40, 255}, 108, ui->renderer);

    /* --------------------------------------------- SCORE ------------------------------------------------*/
    ui->score_textures[0] = render_text("0", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[1] = render_text("1", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[2] = render_text("2", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[3] = render_text("3", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[4] = render_text("4", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[5] = render_text("5", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[6] = render_text("6", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[7] = render_text("7", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[8] = render_text("8", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);
    ui->score_textures[9] = render_text("9", "assets/fonts/Vividly-Regular.otf", (SDL_Color){0, 0, 0, 255}, 72, ui->renderer);

    /* --------------------------------------------- PAUSE --------------------------------------------- */

    ui->pause_texture[0] = load_texture_from_image("assets/ui/button_continue.png", ui->window, ui->renderer);
    ui->pause_texture[1] = load_texture_from_image("assets/ui/button_quit.png", ui->window, ui->renderer);

    return;
}

bool is_continue_clicked(ui_t *ui, int x, int y)
{
    (void)ui;
    return (x >= 550 && x <= 1145 && y >= 250 && y <= 390);
}

bool is_quit_clicked(ui_t *ui, int x, int y)
{
    (void)ui;
    return (x >= 540 && x <= 1148 && y >= 500 && y <= 640);
}

/**
 * @brief Initialisation de la SDL
 *
 * @param ui Structure de l'interface utilisateur
 */
void init_sdl(ui_t *ui)
{
    ui->renderer = NULL;
    ui->window = NULL;

    /* Initialisation SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Création de la fenêtre et du renderer */
    ui->window = SDL_CreateWindow("Mantis (pre-alpha)",
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  ui->screen_w,
                                  ui->screen_h,
                                  SDL_WINDOW_SHOWN);
    if (ui->window == NULL)
        end_sdl(0, "ERROR WINDOW CREATION", ui->window, ui->renderer);
    ui->renderer = SDL_CreateRenderer(ui->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ui->renderer == NULL)
        end_sdl(0, "ERROR RENDERER CREATION", ui->window, ui->renderer);
    if (TTF_Init() < 0)
        end_sdl(0, "Couldn't initialize SDL TTF", ui->window, ui->renderer);

    /* Loading de toutes les textures dans un tableau */
    load_textures(ui);

    // Activer le mode de mélange pour la transparence
    SDL_SetRenderDrawBlendMode(ui->renderer, SDL_BLENDMODE_BLEND);
}

/* Mettre ici toutes les initialisations */
anim_props_t **create_animations()
{
    anim_props_t **animations = malloc(sizeof(anim_props_t) * 10);
    for (int i = 0; i < 10; i++)
    {
        animations[i] = malloc(sizeof(anim_props_t));
        animations[i]->texture = malloc(sizeof(SDL_Texture *) * 10);
        animations[i]->playing = false;
        animations[i]->loop = false;
        init_animation(animations[i], (pos_t){0, 0}, 300);
        for (int j = 0; j < 10; j++) // NB de paramètres
        {
            animations[i]->param[j] = 0;
        }
    }
    return animations;
}

void load_textures_anim(ui_t *ui)
{
    for (int j = 1; j < 3; j++)
    {
        for (int i = 0; i < 7; i++) // Pointe les textures des cartes pour l'anim
        {
            ui->animations[j]->texture[i] = ui->front_card_textures[i];
        }
        ui->animations[j]->texture[7] = ui->back_card_texture[0];
        ui->animations[j]->texture[8] = ui->back_card_texture[1];
    }
}

ui_t *create_ui()
{
    ui_t *ui = malloc(sizeof(ui_t));
    ui->screen_w = SCREEN_WIDTH;
    ui->screen_h = SCREEN_HEIGHT;

    init_sdl(ui);
    ui->in_pause = false;
    ui->program_on = true;
    ui->animations = create_animations();
    load_textures_anim(ui);

    return ui;
}

ui_input_t *create_ui_input()
{
    ui_input_t *ui_input = malloc(sizeof(ui_input_t));
    ui_input->click = (pos_t){-1, -1};
    ui_input->cursor = (pos_t){-1, -1};
    ui_input->key = 0;
    ui_input->delay_input = -1;

    return ui_input;
}

// si les coordonnées cliquées correspondent à la pile
bool stack_clicked(ui_input_t *ui_input)
{
    return ui_input->click.x > SCREEN_WIDTH / 2 - CARD_WIDTH / 6 && ui_input->click.x < SCREEN_WIDTH / 2 + CARD_WIDTH / 6 && ui_input->click.y > SCREEN_HEIGHT / 2 - CARD_HEIGHT / 6 && ui_input->click.y < SCREEN_HEIGHT / 2 + CARD_HEIGHT / 6;
}

// si les coordonnées cliquées correspondent à un joueur (pour le voler)
int player_clicked(pos_t click)
{
    int x = click.x;
    int y = click.y;

    int box_size = 100;
    // OUI c'est des magic numbers, mais c'est pour le prototype
    if (x < 800 + box_size && y < 100 + box_size)
    {
        fprintf(stderr, "Clique sur joueur 3\n"); // En haut à gauche
        return 2;
    }
    else if (x > 1000 - box_size && y < 100 + box_size) // En haut à droite
    {
        fprintf(stderr, "Clique sur joueur 2\n");
        return 1;
    }
    else if (x < 800 + box_size && y > 800 - box_size) // En bas à gauche
    {
        fprintf(stderr, "Clique sur joueur 1\n");
        return 0;
    }
    else if (x > 1000 - box_size && y > 800 - box_size) // En bas à droite
    {
        fprintf(stderr, "Clique sur joueur 4\n");
        return 3;
    }
    else
    {
        return -1;
    }
}

void free_ui(ui_t *ui)
{
    free(ui->animations);
    unload_textures(ui);
    free(ui);
}

/*
 * @brief Fonction pour récupérer les événements lié à l'interface utilisateur
 *
 * @param ui Structure de l'interface utilisateur
 * @param input Structure des entrées
 */
void refresh_input(ui_t *ui, ui_input_t *ui_input)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    ui_input->cursor.x = x;
    ui_input->cursor.y = y;

    /* Gestion des événements */
    while (SDL_PollEvent(&ui->event))
    {
        switch (ui->event.type)
        {
        case SDL_QUIT:
            ui->program_on = SDL_FALSE;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (ui->event.button.button == SDL_BUTTON_LEFT)
            {
                ui_input->click.x = x;
                ui_input->click.y = y;

                if(ui->in_pause)
                {
                    if(is_continue_clicked(ui, x, y))
                    {
                        ui->in_pause = false;
                    }
                    else if(is_quit_clicked(ui, x, y))
                    {
                        ui->program_on = false;
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            switch (ui->event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                ui->in_pause = !ui->in_pause;
                break;
            }
            break;

        default:
            break;
        }
    }
}

bool is_anim_blocking_game(anim_props_t **animations)
{
    if (animations[1]->playing || animations[2]->playing)
        return true;
    return false;
}

bool is_steal_clicked(game_t *game, int player)
{
    return player != game->player_action && game->players[player]->tank[game->face_card_color] > 0;
}

void anim_find_target(anim_props_t *anim, int player)
{
    switch (player)
    {
    case 2:
        anim->target = (pos_t){SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8};
        break;
    case 1:
        anim->target = (pos_t){SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT / 8};
        break;
    case 0:
        anim->target = (pos_t){SCREEN_WIDTH / 4, SCREEN_HEIGHT * 7 / 8};
        break;
    case 3:
        anim->target = (pos_t){SCREEN_WIDTH * 3 / 4, SCREEN_HEIGHT * 7 / 8};
        break;
    default:
        break;
    }
}

// Fonction qui traite les inputs de la SDL vers les inputs relative au jeu en fonction de l'état de celui-ci
// Active les animations si besoin
int process_input(ui_input_t *ui_input, game_t *game, ui_t *ui)
{
    int input = -1;

    ui->animations[0]->pos.x = ui_input->cursor.x;
    ui->animations[0]->pos.y = ui_input->cursor.y;

    if (is_anim_blocking_game(ui->animations)) // Blocage des autres entrées
    {
        return input;
    }
    else if (ui_input->delay_input != -1)
    {
        int tmp = ui_input->delay_input;
        ui_input->delay_input = -1;
        return tmp;
    }
    else if (game->win == -1)
    {
        if (ui_input->click.x != -1 && ui_input->click.y != -1)
        {
            fprintf(stderr, "Click: %d %d\n", ui_input->click.x, ui_input->click.y);
            if (stack_clicked(ui_input))
            {
                ui->animations[0]->number_of_frame = 1600;
                ui->animations[0]->loop = true;
                start_animation(ui->animations[0], 0);
            }
            else
            {
                int player = player_clicked(ui_input->click);
                if (player != -1)
                {
                    if (is_steal_clicked(game, player)) // Click sur un joueur non actif et vole
                    {
                        ui->animations[1]->pos.x = ui_input->cursor.x;
                        ui->animations[1]->pos.y = ui_input->cursor.y;
                        anim_find_target(ui->animations[1], game->player_action);
                        ui->animations[1]->number_of_frame = 2000;
                        ui->animations[1]->param[0] = game->face_card_color;
                        ui->animations[1]->size.x = CARD_WIDTH / 6;
                        ui->animations[1]->size.y = CARD_HEIGHT / 6;
                        start_animation(ui->animations[1], 3000);

                    }
                    ui->animations[2]->pos.x = ui_input->cursor.x;
                    ui->animations[2]->pos.y = ui_input->cursor.y;
                    ui->animations[2]->number_of_frame = 3000;
                    ui->animations[2]->param[0] = game->face_card_color;
                    start_animation(ui->animations[2], 0);

                    ui_input->delay_input = player;
                    fprintf(stderr, "ui_input->delay_input : %d\n", ui_input->delay_input);
                }

                end_animation(ui->animations[0]);
            }
        }
    }

    // Reset des entrées
    ui_input->click.x = -1;
    ui_input->click.y = -1;
    ui_input->key = -1;

    return input;
}