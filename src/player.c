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

// Calcul pour le prochain coup si il faut scorer ou voler à l'aide de l'algorithme UCB (Upper Confidence Bound)
// Renvoie un tableau des estimations de récompenses pour chaque bras
// n le nombre de totale de partie jouée
// Renvoie l'input i avec le meilleur score
int ucb(game_t *game, int n)
{
    /* Initialisation */
    int player = game->player_action;
    int C = 1; // Constante d'exploration
    int G[4];  // Gain accumulé sur la machine k
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
        game_play(copy, max);
        int reward = get_reward(copy, player);
        G[max] += reward;
        n_t[max]++;
        free_game(copy);
    }

    return max;
}

/*----------------------------------------------------------------------Implémentation des phases de MCTS-----------------------------------------------------------*/

/**
 * @brief Sélection de la branche à explorer à l'aide de UCB
 *
 * @param root la racine de l'arbre
 */
int select_node(mcts_t *root)
{
    int n = 100;
    return ucb(root->state, n);
}


/**
 * @brief MCTS expansion
 * 
 * @param node le noeud à étendre
 */
void expand_node(mcts_t * node)
{
    if (node->children == NULL)
    {
        node->children = malloc(4 * sizeof(mcts_t ));
        {
            for (int i = 0; i < 4 ; i++)
            {
                node->children[i] = malloc(sizeof(mcts_t));
                node->children[i]->state = copy_game(node->state);
                game_play(node->children[i]->state, i);
                node->children[i]->parent = node;
                node->children[i]->children = NULL;
                node->children[i]->visits = 0;
                node->children[i]->accumulated_value = 0;
            }
        }
    }
} 

/**
 * @brief MCTS rétropropagation
 *
 * @param node le noeud à simuler
 * @param value la valeur de la simulation
 */
void backpropagate_node(mcts_t *node, double value)
{
    // Mettre à jour les visites et la valeur accumulée de tous les noeuds parents
    while (node != NULL)
    {
        node->visits++;
        node->accumulated_value += value;
        node = node->parent;
    }
}


/*----------------------------------------------------------------------Implémentation des phases de MCTS-----------------------------------------------------------*/

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