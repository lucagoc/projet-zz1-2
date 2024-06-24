#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
    SDL_DestroyTexture(ui->back_card_texture);
    
    /* --------------------------------------------- JOUEURS --------------------------------------------- */
    for (int i = 0; i < 5; i++)
    {
        SDL_DestroyTexture(ui->player_textures[i]);
    }
    
    /* --------------------------------------------- MENU  --------------------------------------------- */
    for (int i = 0; i < 4; i++)
    {
        SDL_DestroyTexture(ui->interface_textures[i]);
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
    ui->front_card_textures[0] = load_texture_from_image("assets/cards/front_0.png", ui->window, ui->renderer); // Texture de debug
    ui->front_card_textures[1] = load_texture_from_image("assets/cards/front_1.png", ui->window, ui->renderer);
    ui->front_card_textures[2] = load_texture_from_image("assets/cards/front_2.png", ui->window, ui->renderer);
    ui->front_card_textures[3] = load_texture_from_image("assets/cards/front_3.png", ui->window, ui->renderer);
    ui->front_card_textures[4] = load_texture_from_image("assets/cards/front_4.png", ui->window, ui->renderer);
    ui->front_card_textures[5] = load_texture_from_image("assets/cards/front_5.png", ui->window, ui->renderer);
    ui->front_card_textures[6] = load_texture_from_image("assets/cards/front_6.png", ui->window, ui->renderer);
    ui->back_card_texture = load_texture_from_image("assets/cards/back.png", ui->window, ui->renderer);

    /* --------------------------------------------- JOUEURS --------------------------------------------- */    
    ui->player_textures[0] = load_texture_from_image("assets/players/player_0.png", ui->window, ui->renderer);
    ui->player_textures[1] = load_texture_from_image("assets/players/player_1.png", ui->window, ui->renderer);
    ui->player_textures[2] = load_texture_from_image("assets/players/player_2.png", ui->window, ui->renderer);
    ui->player_textures[3] = load_texture_from_image("assets/players/player_3.png", ui->window, ui->renderer);
    ui->player_textures[4] = load_texture_from_image("assets/players/player_4.png", ui->window, ui->renderer);

    /* --------------------------------------------- MENU  --------------------------------------------- */
    ui->interface_textures[0] = load_texture_from_image("assets/ui/menu_pause.png", ui->window, ui->renderer);

    /* --------------------------------------------- TEXTE --------------------------------------------- */
    //ui->interface_textures[2] = render_text("SCORE", "assets/otf/metal_lord.otf", (SDL_Color){204, 136, 80, 255}, 24, ui->renderer);
    //ui->interface_textures[1] = render_text("STEAL", "assets/otf/metal_lord.otf", (SDL_Color){204, 136, 80, 255}, 24, ui->renderer);
    //ui->interface_textures[3] = render_text("Good game !", "assets/otf/metal_lord.otf", (SDL_Color){20, 0, 40, 255}, 48, ui->renderer);

    return;
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

ui_t* create_ui()
{
    ui_t* ui = malloc(sizeof(ui_t));
    ui->screen_w = SCREEN_WIDTH;
    ui->screen_h = SCREEN_HEIGHT;

    init_sdl(ui);
    ui->in_pause = false;
    ui->program_on = true;

    return ui;
}

bool stack_clicked(game_t * game, int x, int y){
    return 0;
}

int player_clicked(game, x, y){
    return 1;
}


/*
 * @brief Fonction pour récupérer les événements
 *
 * @param ui Structure de l'interface utilisateur
 * @param input Structure des entrées
 */
void refresh_input(ui_t *ui, int *input, game_t * game)
{
    (void) input;
    
    /* Gestion des événements */
    while (SDL_PollEvent(&ui->event))
    {
        switch (ui->event.type)
        {
        case SDL_QUIT:
            ui->program_on = SDL_FALSE;
            break;

        case SDL_MOUSEBUTTONDOWN:

            if (ui->event.button.button == SDL_BUTTON_LEFT){
                
                int x = ui->event.button.x;
                int y = ui->event.button.y;

                if (stack_clicked(game, x, y)){
                    *input = 0; //si on clique sur la pile pour marquer

                } else {
                    
                    int player_chosen = player_clicked(game, x, y);
                    if (player_chosen){ //si on clique sur un autre joueur pour le voler
                        *input = player_chosen;
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
        }
    }

    *input=-1;
}
