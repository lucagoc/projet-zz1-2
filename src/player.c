#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/struct.h"
#include "headers/player.h"

/**
 *@brief Initialisation d'un joueur
 */
player_t *create_player()
{
    player_t *player = malloc(sizeof(player_t));
    player->score = 0;
    player->last_scored_card = 0;
    for (int i = 0; i < 5; i++)
    {
        player->tank[i] = 0;
    }
    return player;
}

/*----------------------------------------------------------------------Implémentation des 4 phases de MCTS-----------------------------------------------------------*/

/**
 * @brief MCTS selection
 *
 * @param root la racine de l'arbre
 */
mcts_t *select_node(mcts_t *root)
{
    mcts_t *node = root;
    while (node->num_children > 0)
    {
        double max_uct = -1;
        mcts_t *selected_child = NULL;
        for (int i = 0; i < node->num_children; i++)
        {
            mcts_t *child = node->possible_move[i];

            double c = sqrt(2); // Constante d'exploration en théorie c'est sqrt(2)

            double uct = (child->accumulated_value / (child->visits + 1)) + c * sqrt(log(node->visits + 1) / (child->visits + 1)); // Calcul de l'UCT [nombre de parties gagnées / nombre de parties jouées + c * racine carrée de log(nombre de parties jouées) / nombre de parties jouées]
            if (uct > max_uct)
            {
                max_uct = uct;
                selected_child = child;
            }
        }
        node = selected_child;
    }
    return node;
}

void expand_node(mcts_t *node)
{
    int num_moves;
    int *legal_moves = get_possible_moves(node->state, node->player, &num_moves);
    node->possible_move = malloc(num_moves * sizeof(mcts_t *));
    for (int i = 0; i < num_moves; i++)
    {
        game_t *new_state = copy_game(node->state);
        // Appliquer le mouvement pour obtenir le nouvel état
        if (legal_moves[i] == 0)
        { // Exemple : marquer
            score_card(new_state);
        }
        else if (legal_moves[i] == 1)
        { // Exemple : voler
            steal_card(new_state->player_action, new_state);
        }
        node->possible_move[i] = malloc(sizeof(mcts_t));
        node->possible_move[i]->state = new_state;
        node->possible_move[i]->parent = node;
        node->possible_move[i]->player = (node->player + 1) % 4; // Prochain joueur
        node->possible_move[i]->num_children = 0;
        node->possible_move[i]->possible_move = NULL;
        node->possible_move[i]->visits = 0;
        node->possible_move[i]->accumulated_value = 0;
    }
    node->num_children = num_moves;
    free(legal_moves);
}

void backpropagate(mcts_t *node, double value)
{
    while (node != NULL)
    {
        node->visits++;
        node->accumulated_value += value;
        node = node->parent;
    }
}

/*
double simulate_game(game_t *game)
{
    while (!is_terminal(game))
    {
        // Implémentez la condition de terminaison
        int num_moves;
        int *legal_moves = get_possible_moves(game, game->player_action, &num_moves);
        int move = legal_moves[rand() % num_moves];
        if (move == 0)
        {
            score_card(game);
        }
        else if (move == 1)
        {
            steal_card(game->player_action, game);
        }
        free(legal_moves);
    }
    return get_score(game, game->player_action); // Implémentez get_score pour obtenir le score final
}*/

/*----------------------------------------------------------------------Implémentation des 4 phases de MCTS-----------------------------------------------------------*/
