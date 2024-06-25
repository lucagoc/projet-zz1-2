#include <stdio.h>
#include <stdlib.h>

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

 


//Imprémentation des 4 phases de MCTS

/**
 * @brief MCTS selection
 * 
 * @param root la racine de l'arbre MCTS
 * @return mcts_t* le noeud sélectionné
 */
mcts_t *mcts_selection(mcts_t *root)
{
    //TODO
    return NULL;
}

/**
 * @brief MCTS expansion
 * 
 * @param node le noeud à étendre
 * @return mcts_t* le noeud étendu
 */
mcts_t *mcts_expansion(mcts_t *node)
{
    //TODO
    return NULL;
}

/**
 * @brief MCTS simulation
 * 
 * @param node le noeud à simuler
 */
void mcts_simulation(mcts_t *node)
{
    //TODO
    return
}

/**
 * @brief MCTS backpropagation
 * 
 * @param node le noeud à mettre à jour
 * @param accumulated_value la valeur à ajouter
 */
void mcts_backpropagation(mcts_t *node, double accumulated_value)
{
    //TODO
    return;
}
