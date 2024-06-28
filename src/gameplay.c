#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "headers/struct.h"
#include "headers/player.h"
#include "headers/debug.h"

#define NUMBER_FACE 7       // Nombre de couleurs
#define NUMBER_BACK 3       // Nombre d'indicateur au dos de la carte
#define NUMBER_CARD 105     // Nombre de cartes
#define NUMBER_MIX_STACK 20 // Nombre de mélange de cartes

bool is_draw_pile_empty(game_t *game)
{
    for (int i = 0; i < 7; i++)
    {
        if (game->draw_pile_left[i] > 0)
        {
            return false;
        }
    }
    return true;
}

// Retourne le joueur qui a le plus de points
int max_4(int p1, int p2, int p3, int p4)
{
    if (p1 >= p2 && p1 >= p3 && p1 >= p4)
    {
        return 1;
    }
    else if (p2 >= p1 && p2 >= p3 && p2 >= p4)
    {
        return 2;
    }
    else if (p3 >= p1 && p3 >= p2 && p3 >= p4)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}

/*
 * @brief Test si un joueur gagne.
 *
 * @param game le jeu
 */
int is_victory(game_t *game)
{
    if (is_draw_pile_empty(game))
    {
        int score_1 = game->players[0]->score;
        int score_2 = game->players[1]->score;
        int score_3 = game->players[2]->score;
        int score_4 = game->players[3]->score;

        return max_4(score_1, score_2, score_3, score_4);
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            if (game->players[i]->score >= 10)
            {
                return i;
            }
        }
    }
    return -1;
}

void free_stack(stack_t *stack)
{
    stack_t *current = stack;
    while (current != NULL)
    {
        stack_t *next = current->next;
        free(current);
        current = next;
    }
}

/**
 * @brief Libération de la mémoire du jeu
 *
 * @param game le jeu
 */
void free_game(game_t *game)
{
    if (game == NULL)
    {
        return;
    }
    for (int i = 0; i < 4; i++)
    {
        if (game->players[i] != NULL) // Nombre de joueur variable
        {
            free(game->players[i]);
        }
    }
    free_stack(game->stack);

    free(game);
}

stack_t *add_card_to_stack(stack_t *stack, card_t *card)
{
    stack_t *new_stack = malloc(sizeof(stack_t));
    new_stack->card = card;
    new_stack->next = stack;
    return new_stack;
}

stack_t *remove_card_from_stack(stack_t *stack)
{
    stack_t *new_stack = stack->next;
    free(stack);
    return new_stack;
}

stack_t *create_stack()
{
    return NULL;
}

/* Mélange la stack sauf la première carte */
stack_t *mix_stack(stack_t *stack)
{
    if (stack == NULL)
    {
        return NULL;
    }
    if(stack->next == NULL)
    {
        return stack;
    }
    
    // retenir la première carte
    card_t *first_card = stack->card;
    stack = remove_card_from_stack(stack);

    // Mélanger la stack
    for (int i = 0; i < NUMBER_MIX_STACK; i++)
    {
        stack_t *current = stack;
        stack_t *previous = NULL;
        while (current->next != NULL)
        {
            previous = current;
            current = current->next;
        }
        previous->next = NULL;
        current->next = stack;
        stack = current;
    }

    // Remettre la première carte
    stack = add_card_to_stack(stack, first_card);

    return stack;
}

// Génère la pile de cartes
stack_t *gen_stack(game_t *game)
{
    stack_t *stack = create_stack();
    // Générer le nombre de carte par rapport au nombre disponible dans game->draw_pile_left[couleur]
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < game->draw_pile_left[i]; j++)
        {
            card_t *card = malloc(sizeof(card_t));
            card->face = i;

            // Générer le dos, position aléatoire dans le tableau de la couleur face et 2 couleur aléatoire et différentes à coté
            int back = rand() % 3;
            card->back[back] = i;

            int color1 = rand() % 7;
            while (color1 == i)
            {
                color1 = rand() % 7;
            }

            int color2 = rand() % 7;
            while (color2 == i || color2 == color1)
            {
                color2 = rand() % 7;
            }

            card->back[(back + 1) % 3] = color1;
            card->back[(back + 2) % 3] = color2;
            stack = add_card_to_stack(stack, card);
        }
    }

    return stack;
}

/**
 * @brief Génére la prochaine carte à tirer
 *
 * @param game le jeu
 * @return 1 si la carte est tirée, -1 sinon
 */
int get_draw_card(game_t *game)
{
    // Dépiler la carte si possible
    if (game->stack != NULL)
    {
        card_t *card = game->stack->card;
        game->face_card_color = card->face;
        game->back_card_color[0] = card->back[0];
        game->back_card_color[1] = card->back[1];
        game->back_card_color[2] = card->back[2];
        game->stack = remove_card_from_stack(game->stack);
        return 1;
    }
    else
    {
        return -1;
    }
}

/**
 *@brief Initialisation du jeu
 */
game_t *create_game()
{
    game_t *game;
    game = malloc(sizeof(game_t));

    srand(time(NULL)); // Initialisation de la graine pour la génération aléatoire

    // Initialisation des joueurs à 0
    for (int i = 0; i < 4; i++)
    {
        player_t *newplayer = malloc(sizeof(player_t));
        for (int j = 0; j < 7; j++)
        {
            newplayer->tank[j] = 0;
        }
        newplayer->score = 0;
        newplayer->last_scored_card = -1;
        game->players[i] = newplayer;
    }

    game->face_card_color = -1; // Initialisation de la couleur de la carte tirée à -1
    game->player_action = 0;    // Le joueur 1 commence
    game->win = -1;             // Initialisation du statut de victoire à 0 (personne n'a gagné)
    game->back_card_color[0] = -1;
    game->back_card_color[1] = -1;
    game->back_card_color[2] = -1;

    for (int i = 0; i < NUMBER_FACE; i++)
    {
        game->draw_pile_left[i] = 15; // Initialisation de la pile de cartes
    }

    game->stack = mix_stack(gen_stack(game));

    // Tirer une première carte
    get_draw_card(game);

    return game;
}

// Renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon
int is_card_in_tank(int player, game_t *game)
{
    return game->players[player]->tank[game->face_card_color];
}

/**
 * @brief Fonction distribuant les cartes à chaque joueur à partir de la pile de pioche
 *
 * @param game le jeu
 */

void distribute_card(game_t *game, int nb_player)
{
    for (int i = 0; i < nb_player; i++)
    {
        game->players[i]->tank[game->face_card_color] = 1; // on ajoute la carte au tank du joueur
        get_draw_card(game);                               // on dépile
    }
}

/**
 * @brief Fonction du score
 *
 * @param game le jeu
 */
void score_card(game_t *game)
{
    game->players[game->player_action]->score += game->players[game->player_action]->tank[game->face_card_color] + 1; // on ajoute les cartes au score du joueur
    game->players[game->player_action]->tank[game->face_card_color] = 0;                                              // on enlève les cartes du tank
    game->players[game->player_action]->last_scored_card = game->face_card_color;                                     // on affiche la carte en haut de la pile de score
}

/**
 * @brief Fonction d'ajout de carte dans le tank
 *
 * @param player le joueur
 * @param game le jeu
 */
void add_card_in_tank(int player, game_t *game)
{
    game->players[player]->tank[game->face_card_color] += 1; // on ajoute la carte au tank
}

/**
 * @brief Fonction de vol de carte
 *
 * @param input l'input du joueur
 * @param game le jeu
 */
void steal_card(int input, game_t *game)
{
    game->players[game->player_action]->tank[game->face_card_color] += game->players[input]->tank[game->face_card_color] + 1; // on récupère les cartes volées
    game->players[input]->tank[game->face_card_color] = 0;                                                                    // on enlève les cartes au joueur volé
}

stack_t *copy_stack(stack_t *stack)
{
    stack_t *copy_stack = create_stack();
    stack_t *current = stack;
    while (current != NULL)
    {
        copy_stack = add_card_to_stack(copy_stack, current->card);
        current = current->next;
    }
    return copy_stack;
}

/**
 * @brief Fonction copie l'état du jeu
 *
 * @param game l'état du jeu
 * @return copy_game_state la copie de l'état du jeu
 */
game_t *copy_game(game_t *game)
{
    game_t *copy_game_state = malloc(sizeof(game_t));
    if (copy_game_state == NULL)
    {
        fprintf(stderr, "[ERROR] copy_game : Can't allocate memory\n");
        return NULL;
    }

    /* Copie des joueurs */
    for (int i = 0; i < 4; i++)
    {
        player_t *copy_player = malloc(sizeof(player_t));
        if (copy_player == NULL)
        {
            fprintf(stderr, "[ERROR] copy_game : Can't allocate memory\n");
            return NULL;
        }
        for (int j = 0; j < NUMBER_FACE; j++)
        {
            copy_player->tank[j] = game->players[i]->tank[j];
        }
        copy_player->score = game->players[i]->score;
        copy_player->last_scored_card = game->players[i]->last_scored_card;
        copy_game_state->players[i] = copy_player;
    }

    for (int i = 0; i < NUMBER_FACE; i++)
    {
        copy_game_state->draw_pile_left[i] = game->draw_pile_left[i];
    }
    for (int i = 0; i < 3; i++)
    {
        copy_game_state->back_card_color[i] = game->back_card_color[i];
    }

    /* Copie des caractéristiques */
    copy_game_state->face_card_color = game->face_card_color;
    copy_game_state->player_action = game->player_action;
    copy_game_state->win = game->win;
    copy_game_state->face_card_color = game->face_card_color;

    /* Copie de la stack */
    stack_t *copied_stack = copy_stack(game->stack);
    copy_game_state->stack = copied_stack;

    return copy_game_state;
}

/**
 * @brief Fonction de jeu
 *
 * @param game le jeu
 * @param input l'input du joueur
 */

void game_play(game_t *game, int input)
{
    if (input == game->player_action) // le joueur actif choisit de marquer
    {
        if (is_card_in_tank(game->player_action, game))
        {
            score_card(game);
        }
        else
        {
            add_card_in_tank(game->player_action, game);
        }
    }
    else // le joueur actif choisit de voler
    {
        if (is_card_in_tank(input, game))
        {
            steal_card(input, game);
        }
        else
        {
            add_card_in_tank(input, game);
        }
    }

    get_draw_card(game);
    game->win = is_victory(game);
    game->player_action = (game->player_action + 1) % 4;
}
