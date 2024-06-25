#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/struct.h"
#include "headers/player.h"
#include "headers/gameplay.h"

#define NUM_ARMS 10         // Nombre de bras
#define NUM_ITERATIONS 1000 // Nombre total d'itérations

float I_k(float G, float C, int n, int n_k)
{
    return G / n_k + C * sqrt(log(n) / n_k);
}

// On utilise comme récompense directement le score du joueur
int get_reward(game_t *game, int player)
{
    return game->players[player]->score;
}

// Possède le même comportement que game_play mais en simulant la partie.
void game_simulate(game_t *game, int input)
{
    // On "cache" en donnant une valeur au hasard à l'avant par rapport au information à l'arrière de la carte
    game->draw_pile->card->face = game->draw_pile->card->back[rand()%3];
    game_play(game, input);
}

// Calcul pour le prochain coup si il faut scorer ou voler à l'aide de l'algorithme UCB (Upper Confidence Bound)
// Renvoie un tableau des estimations de récompenses pour chaque bras
// n le nombre de totale de partie jouée
// Renvoie l'input i avec le meilleur score
int ucb(game_t *game, int player, int n)
{
    /* Initialisation */
    int C = 1; // Constante d'exploration
    int G[4];  // Gain accumulé sur la machine k
    int input = 0;
    int max = 0;
    int n_t[4]; // Nombre de fois où l'on a joué sur la machine k
    int I[4];   // Valeur de l'indice de confiance pour chaque possibilité
    for (int i = 0; i < 4; i++)
    {
        G[i] = 0;
        I[i] = 0;
    }

    // On joue une fois sur toute les machines
    for (int i = 0; i < 4; i++)
    {
        game_t *copy = copy_game(game);
        game_play(copy, i % 4);
        int reward = get_reward(copy, player);
        I[i] = I_k(G[i], C, n, 1); // 1 = le nombre de fois où l'on a déjà joué sur la machine.
        G[i] += reward;
        free_game(copy);
    }

    // On joue n fois sur toute les machines
    // On choisit la machine avec l'indice de confiance le plus élevé
    for (int i = 4; i < n; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            I[j] = I_k(G[j], C, n, n_t[j]);
        }
        for (int j = 0; j < 4; j++)
        {
            if (I[j] > I[max])
            {
                max = j;
            }
        }
        game_t *copy = copy_game(game);
        game_simulate(copy, max);
        int reward = get_reward(copy, player);
        G[max] += reward;
        n_t[max]++;
        free_game(copy);
    }

    return max;
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

/**
 * @brief MCTS expansion
 *
 * @param node le noeud à étendre
 
void expand_node(mcts_t *node)
{
    int *legal_moves = get_possible_moves(node->player); // Récupérer les mouvements possibles
    node->possible_move = malloc(4 * sizeof(mcts_t *));
    for (int i = 0; i < 4; i++) // Pour chaque mouvement possible
    {
        game_t *new_state = copy_game(node->state);

        // Appliquer le mouvement pour obtenir le nouvel état

        if (legal_moves[i] == 0)
        {
            score_card(new_state);
        }
        else
        {
            steal_card(new_state->player_action, new_state);
        }
        // Créer un nouveau noeud pour le nouvel état
        node->possible_move[i] = malloc(sizeof(mcts_t));
        node->possible_move[i]->state = new_state;
        node->possible_move[i]->parent = node;
        node->possible_move[i]->player = (node->player + 1) % 4; // Prochain joueur
        node->possible_move[i]->num_children = 0;
        node->possible_move[i]->possible_move = NULL;
        node->possible_move[i]->visits = 0;
        node->possible_move[i]->accumulated_value = 0;
    }
    node->num_children = 4;
    free(legal_moves);
}*/

/**
 * @brief MCTS rétropropagation
 *
 * @param node le noeud à simuler
 * @param value la valeur de la simulation
 */
void backpropagate(mcts_t *node, double value)
{
    // Mettre à jour les visites et la valeur accumulée de tous les noeuds parents
    while (node != NULL)
    {
        node->visits++;
        node->accumulated_value += value;
        node = node->parent;
    }
}
/**
 * @brief MCTS simulation
 *
 * @param game l'état du jeu
 

double simulate_game(game_t *game)
{
    for (int i = 0; i < 4; i++)
    {
        while (game->players[i]->score >= 10)
        {
            int num_moves;
            int *legal_moves = get_possible_moves(game->player_action);
            int move = legal_moves[rand() % num_moves]; // Choisir un mouvement aléatoire
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
    }
    return get_score(game, game->player_action); // Retourne le score du joueur actif
}*/

/*----------------------------------------------------------------------Implémentation des 4 phases de MCTS-----------------------------------------------------------*/

/**
 *@brief Initialisation d'un joueur
 */
player_t *create_player()
{
    player_t *player = malloc(sizeof(player_t));
    player->score = 0;
    player->last_scored_card = 0;
    for (int i = 0; i < 4; i++)
    {
        player->tank[i] = 0;
    }
    return player;
}