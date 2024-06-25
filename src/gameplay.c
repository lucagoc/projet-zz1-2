#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "headers/struct.h"

#define NUMBER_FACE 7       // Nombre de couleurs
#define NUMBER_BACK 3       // Nombre d'indicateur au dos de la carte
#define NUMBER_CARD 105     // Nombre de cartes
#define NUMBER_MIX_STACK 20 // Nombre de mélange de cartes

/**
 *@brief Libération de la mémoire du jeu
 *
 * @param game le jeu
 */
void free_game(game_t *game)
{
    for (int i = 0; i < 4; i++)
    {
        if (game->players[i] != NULL) // Nombre de joueur variable
        {
            free(game->players[i]);
        }
    }
    stack_free(game->draw_pile);
    free(game);
}

/**
 * @brief Fonction de vérification de la victoire
 * 
 * @param game le jeu
 * @param nb_player le nombre de joueur
 */
void check_win(game_t * game , int nb_player)
{
    for (int i = 0; i < nb_player; i++)
    {
        if (game->players[i]->score >= 10)
        {
            game->win = i + 1;
        }
    }
}

/**
 *@brief Initialisation des cartes
 *
 *@param card La carte qu'on veut initialiser
 *@param face_color La couleur de face
 *@param back_color La couleur de dos
 */

card_t *init_card(int face_color)
{
    card_t *card = malloc(sizeof(card_t));
    card->face = face_color; // Définition de la face avant

    /* Couleur d'indicateur */
    int a = -1;
    int b = -1;
    while (a == -1 || a == face_color)
    {
        a = rand() % NUMBER_FACE;
    }
    while (b == -1 || b == face_color || b == a)
    {
        b = rand() % NUMBER_FACE;
    }

    // Mettre les 3 couleurs en positions aléatoires
    int r = rand() % 3;
    card->back[r] = face_color;
    card->back[(r + 1) % 3] = a;
    card->back[(r + 2) % 3] = b;

    return card;
}

stack_t *mix_cards(stack_t *origin)
{
    // On crée 7 pile
    stack_t *stacks[NUMBER_MIX_STACK];
    for (int i = 0; i < NUMBER_MIX_STACK; i++)
    {
        stacks[i] = stack_create();
    }

    // On distribue les cartes aléatoirement dans les piles
    stack_t *current = origin;
    while (current != NULL)
    {
        int r = rand() % NUMBER_MIX_STACK;
        fprintf(stderr, "[DEBUG] mix_cards : %d\n", r);
        stacks[r] = stack_push(stacks[r], current->card);
        current = current->next;
    }

    // On remet les cartes dans la pile originale
    stack_t *res = NULL;
    for (int i = 0; i < NUMBER_MIX_STACK; i++)
    {
        while (!stack_is_empty(stacks[i]))
        {
            res = stack_push(res, stacks[i]->card);
            stacks[i] = stack_pop(stacks[i]);
        }
    }

    return res;
}

/**
 * @brief Initialisation de la pile de pioche de manière aléatoire
 *
 * @param game Le jeu
 */
stack_t *init_draw_card()
{
    stack_t *res = stack_create();
    srand(time(NULL));

    // Créer les cartes en assurant qu'il y a 15 cartes de chaque couleur
    for (int i = 0; i < NUMBER_FACE; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            card_t *new = init_card(i);
            res = stack_push(res, new);
        }
    }

    // Mélanger les cartes
    for (int i = 0; i < 1000; i++) // Mélanger 100 fois
    {
        res = mix_cards(res);
    }

    return res;
}

// Affiche toutes les cartes d'une stack (debug)
void print_all_cards(stack_t *stack)
{
    stack_t *current = stack;
    while (current != NULL)
    {
        printf("Face : %d\n", current->card->face);
        printf("Back : %d %d %d\n", current->card->back[0], current->card->back[1], current->card->back[2]);
        current = current->next;
    }
}

/**
 *@brief Initialisation du jeu
 */
game_t *create_game()
{
    game_t *game;
    game = malloc(sizeof(game_t));

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

    game->drawn_card_color = -1; // Initialisation de la couleur de la carte tirée à -1
    game->player_action = 0;     // Le joueur 1 commence
    game->win = 0;               // Initialisation du statut de victoire à 0 (personne n'a gagné)

    game->draw_pile = init_draw_card(); // Initialisation de la pile de pioche
    print_all_cards(game->draw_pile);
    game->stealing=0;

    return game;
}

// Renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon
int is_card_in_tank(int player, game_t *game)
{
    if (game->drawn_card_color > 7 || game->drawn_card_color < 0)
    {
        fprintf(stderr, "[ERROR] is_card_in_tank : no card drawn\n");
        return 0;
    }
    fprintf(stderr, "[DEBUG] is_card_in_tank : player %d, card %d\n", player, game->drawn_card_color);

    return game->players[player]->tank[game->drawn_card_color];
}

// Dépile la pile et renvoie la couleur de la première carte
int get_draw_card(game_t *game)
{
    if (game->draw_pile != NULL)
    {
        int drawn_card_color = game->draw_pile->card->face; // on récupère la couleur du haut de la pile
        game->draw_pile = game->draw_pile->next;            // on passe à l'élément suivant de la pile
        return drawn_card_color;
    }
    else
    {
        return -1;
    }
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
        game->players[i]->tank[game->draw_pile->card->face] = 1; // on ajoute la carte au tank du joueur
        game->draw_pile = game->draw_pile->next;                 // on passe à l'élément suivant de la pile
    }
}

/**
 * @brief Fonction du score
 *
 * @param game le jeu
 */
void score_card(game_t *game)
{
    game->players[game->player_action]->score += game->players[game->player_action]->tank[game->drawn_card_color] + 1; // on ajoute les cartes au score du joueur
    game->players[game->player_action]->tank[game->drawn_card_color] = 0;                                              // on enlève les cartes du tank
    game->players[game->player_action]->last_scored_card = game->drawn_card_color;                                     // on affiche la carte en haut de la pile de score
}

/**
 * @brief Fonction d'ajout de carte dans le tank
 *
 * @param player le joueur
 * @param game le jeu
 */
void add_card_in_tank(int player, game_t *game)
{
    fprintf(stderr, "[DEBUG] add_card_in_tank : player %d, card %d\n", player, game->drawn_card_color);
    game->players[player]->tank[game->drawn_card_color] = 1; // on ajoute la carte au tank
}

/**
 * @brief Fonction de vol de carte
 *
 * @param input l'input du joueur
 * @param game le jeu
 */
void steal_card(int input, game_t *game)
{
    game->stealing=input;                                                                  // on enlève les cartes au joueur volé

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
        fprintf(stderr, "Erreur d'alloction de mémoire\n");
        return NULL;
    }
    //Copie des joueurs
    for (int i = 0 ; i < 4 ; i++)
    {
        player_t * copy_player = malloc(sizeof(player_t));
        if (copy_player == NULL)
        {
            fprintf(stderr, "Erreur d'alloction de mémoire\n");
            return NULL;
        }
        for (int j = 0 ; j < 7 ; j++)
        {
            copy_player->tank[j] = game->players[i]->tank[j];
        }
        copy_player->score = game->players[i]->score;
        copy_player->last_scored_card = game->players[i]->last_scored_card;
        copy_game_state->players[i] = copy_player;
    }
    copy_game_state->drawn_card_color = game->drawn_card_color;
    copy_game_state->player_action = game->player_action;
    copy_game_state->win = game->win;

    // Copie de la pile de pioche
    stack_t *copy_draw_pile = stack_create();
    stack_t *current = game->draw_pile;
    while (current != NULL)
    {
        card_t *copy_card = malloc(sizeof(card_t));
        if (copy_card == NULL)
        {
            fprintf(stderr, "Erreur d'alloction de mémoire\n");
            return NULL;
        }
        copy_card->face = current->card->face;
        for (int i = 0 ; i < 3 ; i++)
        {
            copy_card->back[i] = current->card->back[i];
        }
        copy_draw_pile = stack_push(copy_draw_pile, copy_card);
        current = current->next;
    }
    copy_game_state->draw_pile = copy_draw_pile;

    return copy_game_state;
}


/**
 * @brief Obtenir le score du joueur 
 * 
 * @param game état du jeu
 * @param player joueur actif
 * @return score le score du joueur
 */
int get_score(game_t *game, int player)
{
    return game->players[player]->score;
}

/**
 * @brief Fonction de jeu
 *
 * @param game le jeu
 * @param input l'input du joueur
 */

void game_play(game_t *game, int input)
{

    // input vaut 0 si le joueur actif clique sur sa propre pile et i>0 s'il clique sur le joueur i pour le voler

    game->drawn_card_color = get_draw_card(game); // on dépile et on affiche

    if (input == game->player_action) // le joueur actif choisit de marquer
    {
        if (is_card_in_tank(game->player_action, game)) // S'il tombe sur une bonne couleur qu'il a
        {
            score_card(game);
            game->player_action = (game->player_action + 1) % 4;

        }
        else
        {
            add_card_in_tank(game->player_action, game); // s'il tombe sur une couleur qu'il n'a pas
            game->player_action = (game->player_action + 1) % 4;

        }
    }
    else // le joueur actif choisit de voler
    {
        fprintf(stderr, "[DEBUG] input value = %d", input);
        if (is_card_in_tank(input, game)) // s'il tombe sur une bonne couleur qu'il a
        {

            steal_card(input, game);
        }
        else
        {
            add_card_in_tank(input, game); // s'il tombe sur une couleur qu'il n'a pas
            game->player_action = (game->player_action + 1) % 4;

        }
    }
    
    // passage au joueur suivant
    fprintf(stderr, "[DEBUG] game_play : switching to player %d\n", game->player_action);
}