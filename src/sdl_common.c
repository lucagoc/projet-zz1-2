#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

#include "headers/sdl_common.h"
#include "headers/gameplay.h"
#include "headers/animation.h"
#include "headers/player.h"

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
    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(ui->front_card_textures[i]);
    }
    SDL_DestroyTexture(ui->back_card_texture[0]);
    SDL_DestroyTexture(ui->back_card_texture[1]);

    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(ui->back_flag_textures[i]);
    }

    /* --------------------------------------------- JOUEURS --------------------------------------------- */
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(ui->player_textures[i]);
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

    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(ui->active_player_textures[i]);
    }

    SDL_DestroyTexture(ui->triangle);
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

/**
 * @brief Rend un texte en texture
 *
 * @param message Message à afficher
 * @param font_file Nom du fichier de police
 * @param color Couleur du texte
 * @param font_size Taille de la police
 * @param renderer Renderer SDL
 * @return SDL_Texture* Texture du texte
 */
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
    ui->back_card_texture[2] = load_texture_from_image("assets/cards/back_black.png", ui->window, ui->renderer);

    ui->back_flag_textures[0] = load_texture_from_image("assets/cards/back_flag_0.png", ui->window, ui->renderer);
    ui->back_flag_textures[1] = load_texture_from_image("assets/cards/back_flag_1.png", ui->window, ui->renderer);
    ui->back_flag_textures[2] = load_texture_from_image("assets/cards/back_flag_2.png", ui->window, ui->renderer);
    ui->back_flag_textures[3] = load_texture_from_image("assets/cards/back_flag_3.png", ui->window, ui->renderer);
    ui->back_flag_textures[4] = load_texture_from_image("assets/cards/back_flag_4.png", ui->window, ui->renderer);
    ui->back_flag_textures[5] = load_texture_from_image("assets/cards/back_flag_5.png", ui->window, ui->renderer);
    ui->back_flag_textures[6] = load_texture_from_image("assets/cards/back_flag_6.png", ui->window, ui->renderer);

    /* --------------------------------------------- JOUEURS --------------------------------------------- */
    ui->player_textures[0] = load_texture_from_image("assets/players/player_0.png", ui->window, ui->renderer);
    ui->player_textures[1] = load_texture_from_image("assets/players/player_1.png", ui->window, ui->renderer);
    ui->player_textures[2] = load_texture_from_image("assets/players/player_2.png", ui->window, ui->renderer);
    ui->player_textures[3] = load_texture_from_image("assets/players/player_3.png", ui->window, ui->renderer);
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
    ui->active_player_textures[0] = load_texture_from_image("assets/ui/bottom_left.png", ui->window, ui->renderer);
    ui->active_player_textures[1] = load_texture_from_image("assets/ui/upper_right.png", ui->window, ui->renderer);
    ui->active_player_textures[2] = load_texture_from_image("assets/ui/upper_left.png", ui->window, ui->renderer);
    ui->active_player_textures[3] = load_texture_from_image("assets/ui/bottom_right.png", ui->window, ui->renderer);
    ui->triangle = load_texture_from_image("assets/ui/triangle.png", ui->window, ui->renderer);
    ui->background[0] = load_texture_from_image("assets/ui/layer0.png", ui->window, ui->renderer);
    ui->background[1] = load_texture_from_image("assets/ui/layer1.png", ui->window, ui->renderer);
    ui->background[2] = load_texture_from_image("assets/ui/layer2.png", ui->window, ui->renderer);
    ui->background[3] = load_texture_from_image("assets/ui/layer3.png", ui->window, ui->renderer);

    return;
}

/**
 * @brief Vérifie si le bouton continuer est cliqué
 *
 * @param ui Structure de l'interface utilisateur
 * @param pos Position du clic
 */
bool is_continue_clicked(ui_t *ui, pos_t pos)
{
    int button_w = 300;
    int button_h = 100;

    // Calculer les positions pour centrer les boutons
    int button_x = (ui->screen_w - button_w) / 2;
    int continue_button_y = 2 * (ui->screen_h - button_h) / 5;

    return (pos.x >= button_x && pos.x <= button_x + button_w && pos.y >= continue_button_y && pos.y <= continue_button_y + button_h);
}

/**
 * @brief Vérifie si le bouton quitter est cliqué
 *
 * @param ui Structure de l'interface utilisateur
 * @param pos Position du clic
 */
bool is_quit_clicked(ui_t *ui, pos_t pos)
{
    int button_w = 300;
    int button_h = 100;

    int button_x = (ui->screen_w - button_w) / 2;
    int quit_button_y = 4 * (ui->screen_h - button_h) / 5;

    return (pos.x >= button_x && pos.x <= button_x + button_w && pos.y >= quit_button_y && pos.y <= quit_button_y + button_h);
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

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("Erreur d'initialisation de SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialisation de SDL_mixer
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
    {
        printf("Erreur d'initialisation de SDL_mixer: %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }

    ui->window = SDL_CreateWindow("Mantis 1.0",
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

    load_textures(ui);
    SDL_SetRenderDrawBlendMode(ui->renderer, SDL_BLENDMODE_BLEND);
}

void load_sound(ui_t *ui)
{
    // Chargement des sons
    ui->music[0] = Mix_LoadWAV("assets/sounds/main_loop.wav");
    if (ui->music[0] == NULL)
    {
        printf("Erreur de chargement du fichier audio: %s\n", Mix_GetError());
        return;
    }

    ui->music[1] = Mix_LoadWAV("assets/sounds/congratulations.wav");
    if (ui->music[1] == NULL)
    {
        printf("Erreur de chargement du fichier audio: %s\n", Mix_GetError());
        return;
    }
}

/**
 * @brief Crée une structure d'animation
 *
 * @return anim_props_t** Tableau de structure d'animation
 */
anim_props_t **create_animations()
{
    anim_props_t **animations = malloc(sizeof(anim_props_t) * 10);
    for (int i = 0; i < 10; i++)
    {
        animations[i] = malloc(sizeof(anim_props_t));
        animations[i]->texture = malloc(sizeof(SDL_Texture *) * 10);
        animations[i]->playing = false;
        animations[i]->loop = false;
        for (int j = 0; j < 10; j++)
        {
            animations[i]->param[j] = 0;
        }
    }
    return animations;
}

/**
 * @brief Charge les textures pour les animations
 *
 * @param ui Structure de l'interface utilisateur
 */
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
        ui->animations[j]->texture[9] = ui->back_card_texture[2];
    }
}

/**
 * @brief Crée une structure d'interface utilisateur
 *
 * @return ui_t* Structure d'interface utilisateur
 */
ui_t *create_ui()
{
    ui_t *ui = malloc(sizeof(ui_t));
    ui->screen_w = SCREEN_WIDTH;
    ui->screen_h = SCREEN_HEIGHT;

    init_sdl(ui);
    ui->in_pause = false;
    ui->program_on = true;
    ui->congrats_played = false;
    ui->animations = create_animations();
    load_textures_anim(ui);
    load_sound(ui);

    return ui;
}

/**
 * @brief Crée une structure d'entrée utilisateur
 *
 * @return ui_input_t* Structure d'entrée utilisateur
 */
ui_input_t *create_ui_input()
{
    ui_input_t *ui_input = malloc(sizeof(ui_input_t));
    ui_input->click = (pos_t){-1, -1};
    ui_input->cursor = (pos_t){-1, -1};
    ui_input->key = 0;
    ui_input->delay_input = -1;

    return ui_input;
}

/**
 * @brief Vérifie si la pile est cliquée
 *
 * @param ui_input Entrée utilisateur
 * @return true si la pile est cliquée
 * @return false sinon
 */
bool stack_clicked(ui_input_t *ui_input)
{
    return ui_input->click.x > SCREEN_WIDTH / 2 - CARD_WIDTH / 6 && ui_input->click.x < SCREEN_WIDTH / 2 + CARD_WIDTH / 6 && ui_input->click.y > SCREEN_HEIGHT / 2 - CARD_HEIGHT / 6 && ui_input->click.y < SCREEN_HEIGHT / 2 + CARD_HEIGHT / 6;
}

/**
 * @brief Récupère le joueur cliqué
 *
 * @param click Position du clic
 * @return int Joueur cliqué
 * @return -1 Aucun joueur cliqué
 */
int player_clicked(pos_t click)
{
    int x = click.x;
    int y = click.y;

    int box_size = 100;
    // OUI c'est des magic numbers, mais c'est pour le prototype
    if (x < 800 + box_size && y < 100 + box_size)
    {
        return 2;
    }
    else if (x > 1000 - box_size && y < 100 + box_size) // En haut à droite
    {
        return 1;
    }
    else if (x < 800 + box_size && y > 800 - box_size) // En bas à gauche
    {
        return 0;
    }
    else if (x > 1000 - box_size && y > 800 - box_size) // En bas à droite
    {
        return 3;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief Libère la mémoire de l'interface utilisateur
 *
 * @param ui Structure de l'interface utilisateur
 */
void free_ui(ui_t *ui)
{
    for (int i = 0; i < 10; i++)
    {
        free(ui->animations[i]->texture);
        free(ui->animations[i]);
    }

    free(ui->animations);
    Mix_FreeChunk(ui->music[0]); // Libération de la musique
    Mix_FreeChunk(ui->music[1]);
    unload_textures(ui);
    free(ui);
}

/**
 * @brief Fonction pour récupérer les événements lié à l'interface utilisateur
 *
 * @param ui Structure de l'interface utilisateur
 * @param ui_input Structure des entrées
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

/**
 * @brief Vérifie si une animation est en cours
 *
 * @param animations Tableau d'animations
 * @return true si une animation est en cours
 * @return false sinon
 */
bool is_anim_blocking_game(anim_props_t **animations)
{
    if (animations[1]->playing || animations[2]->playing)
        return true;
    return false;
}

/**
 * @brief Vérifie si le joueur peut voler pour l'animation
 *
 * @param game Structure du jeu
 * @param player Joueur cible
 * @return true si c'est un vol
 * @return false sinon
 */
bool is_steal_clicked(game_t *game, int player)
{
    return player != game->player_action && game->players[player]->tank[game->face_card_color] > 0;
}

/**
 * @brief Trouve la position de la cible
 *
 * @param anim Structure de l'animation
 * @param player Joueur cible
 */
void anim_find_target(anim_props_t *anim, int player)
{
    switch (player)
    {
    case 2:
        anim->target.x = SCREEN_WIDTH / 4;
        anim->target.y = SCREEN_HEIGHT / 8;
        break;
    case 1:
        anim->target.x = SCREEN_WIDTH * 3 / 4;
        anim->target.y = SCREEN_HEIGHT / 8;
        break;
    case 0:
        anim->target.x = SCREEN_WIDTH / 4;
        anim->target.y = SCREEN_HEIGHT * 7 / 8;
        break;
    case 3:
        anim->target.x = SCREEN_WIDTH * 3 / 4;
        anim->target.y = SCREEN_HEIGHT * 7 / 8;
        break;
    default:
        break;
    }
}

/**
 * @brief Fonction pour récupérer les inputs du joueur
 *
 * @param ui_input Structure des entrées
 * @param game Structure du jeu
 * @param ui Structure de l'interface utilisateur
 */
int process_input(ui_input_t *ui_input, game_t *game, ui_t *ui)
{
    int input = -1;

    if (ui->in_pause)
    {
        if (ui_input->click.x != -1 && ui_input->click.y != -1)
        {
            if (is_continue_clicked(ui, ui_input->click))
            {
                ui->in_pause = false;
            }
            else if (is_quit_clicked(ui, ui_input->click))
            {
                ui->program_on = false;
            }

            // Réinitialiser l'état du bouton de la souris après traitement
            ui_input->click.x = -1;
            ui_input->click.y = -1;
            ui_input->key = -1;
        }
        return input; 
    }

    // Traitement des entrées en jeu
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
            if (stack_clicked(ui_input))
            {
                ui->animations[0]->number_of_frame = -1; // Infini
                ui->animations[0]->loop = true;
                ui->animations[0]->param[0] = game->back_card_color[0];
                ui->animations[0]->param[1] = game->back_card_color[1];
                ui->animations[0]->param[2] = game->back_card_color[2];
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
                }

                end_animation(ui->animations[0]);
            }

            // Réinitialiser l'état du bouton de la souris après traitement
            ui_input->click.x = -1;
            ui_input->click.y = -1;
        }
    }
    else
    {
        if (ui_input->click.x != -1 && ui_input->click.y != -1)
        {
            ui->program_on = false;
        }
    }
    ui_input->key = -1;

    return input;
}


/**
 * @brief Fonction pour récupérer les inputs du robot
 *
 * @param ui_input Structure des entrées
 * @param game Structure du jeu
 * @param ui Structure de l'interface utilisateur
 */
int process_input_robot(ui_input_t *ui_input, game_t *game, ui_t *ui)
{
    int input = -1;
    if (ui->in_pause)
    {
        if (ui_input->click.x != -1 && ui_input->click.y != -1)
        {
            if (is_continue_clicked(ui, ui_input->click))
            {
                ui->in_pause = false;
            }
            else if (is_quit_clicked(ui, ui_input->click))
            {
                ui->program_on = false;
            }

            // Réinitialiser l'état du bouton de la souris après traitement
            ui_input->click.x = -1;
            ui_input->click.y = -1;
            ui_input->key = -1;
        }
        return input; 
    }

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
    if (game->player_action != 0)
    {
        int player = mcts(game);            // Récupère l'input du robot
        if (is_steal_clicked(game, player)) // Click sur un joueur non actif et vole
        {
            anim_find_target(ui->animations[1], player); // Trouve la position du joueur
            ui->animations[1]->pos.x = ui->animations[1]->target.x;
            ui->animations[1]->pos.y = ui->animations[1]->target.y;
            anim_find_target(ui->animations[1], game->player_action); // Trouve la position du joueur (cible)
            ui->animations[1]->number_of_frame = 2000;
            ui->animations[1]->param[0] = game->face_card_color;
            ui->animations[1]->size.x = CARD_WIDTH / 6;
            ui->animations[1]->size.y = CARD_HEIGHT / 6;
            start_animation(ui->animations[1], 3000);
        }
        anim_find_target(ui->animations[2], player);
        ui->animations[2]->pos.x = ui->animations[2]->target.x;
        ui->animations[2]->pos.y = ui->animations[2]->target.y;
        ui->animations[2]->number_of_frame = 3000;
        ui->animations[2]->param[0] = game->face_card_color;
        start_animation(ui->animations[2], 0);

        ui_input->delay_input = player;
    }

    return input;
}

void play_sound(ui_t *ui, game_t *game)
{
    if (game->win == -1)
    {
        if (Mix_Playing(-1) == 0)
        {
            Mix_PlayChannel(-1, ui->music[0], 0);
        }
    }
    else
    {
        if (!(ui->congrats_played))
        {
            Mix_HaltChannel(-1);
            Mix_PlayChannel(-1, ui->music[1], 0);
            ui->congrats_played = true;
        }
    }
}