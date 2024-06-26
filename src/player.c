#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/struct.h"
#include "headers/player.h"
#include "headers/gameplay.h"

#define NUM_PLAYERS 4       // Nombre de joueurs
#define NUM_ARMS 10         // Nombre de bras
#define NUM_ITERATIONS 1000 // Nombre total d'itérations

float I_k(float G, float C, int n, int n_k)
{
    return G / n_k + C * sqrt(log(n) / n_k);
}

// On utilise comme récompense directement le score des joueur
int *get_reward(game_t *game)
{
    int *rewards = malloc(sizeof(int) * 4);
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        rewards[i] = game->players[i]->score;
    }
    return rewards;
}

// Calcul pour le prochain coup si il faut scorer ou voler à l'aide de l'algorithme UCB (Upper Confidence Bound)
// Renvoie un tableau des estimations de récompenses pour chaque bras
// n le nombre de totale de partie jouée
// Renvoie l'input i avec le meilleur score
int ucb(game_t *game, int n)
{
    /* Initialisation */
    int player = game->player_action;
    float C = 1.4;      // Constante d'exploration
    int G[NUM_PLAYERS]; // Gain accumulé sur la machine k
    int max = 0;
    int n_t[NUM_PLAYERS]; // Nombre de fois où l'on a joué sur la machine k
    int I[NUM_PLAYERS];   // Valeur de l'indice de confiance pour chaque possibilité
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        G[i] = 0;
        I[i] = 0;
    }

    // On joue une fois sur toute les machines
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        game_t *copy = copy_game(game);
        game_play(copy, i % 4);
        int *reward = get_reward(copy);
        I[i] = I_k(G[i], C, n, 1); // 1 = le nombre de fois où l'on a déjà joué sur la machine.
        G[i] += reward[i];
        free_game(copy);
        free(reward);
    }

    // On joue n fois sur toute les machines
    // On choisit la machine avec l'indice de confiance le plus élevé
    for (int i = NUM_PLAYERS; i < n; i++)
    {
        for (int j = 0; j < NUM_PLAYERS; j++)
        {
            I[j] = I_k(G[j], C, n, n_t[j]);
        }
        for (int j = 0; j < NUM_PLAYERS; j++)
        {
            if (I[j] > I[max])
            {
                max = j;
            }
        }
        game_t *copy = copy_game(game);
        game_play(copy, max);
        int *reward = get_reward(copy);
        G[max] += reward[player];
        n_t[max]++;
        free(reward);
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
    for (int i = 0; i < 4; i++)
    {
        if (root->children[i] == NULL)
        {
            return i;
        }
    }

    // Dans l'autre cas.
    int n = 10;
    return ucb(root->state, n);
}

/**
 * @brief MCTS expansion
 *
 * @param node le noeud à étendre
 */
mcts_t *create_node(mcts_t *parent, game_t *game)
{
    mcts_t *node = malloc(sizeof(mcts_t));
    node->state = copy_game(game);
    node->parent = parent;
    node->visits = 0;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        node->gain_coup[i] = 0;
        node->n_coup[i] = 0;
        node->children[i] = NULL;
    }

    return node;
}

void *free_mtsc_node(mcts_t *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        free_mtsc_node(node->children[i]);
    }

    free_game(node->state);
    free(node);
    return NULL;
}

/**
 * @brief MCTS rétropropagation
 *
 * @param node le noeud à simuler
 * @param value la valeur de la simulation
 */
void backpropagate_node(mcts_t *node, int *value)
{
    // Mettre à jour les visites et la valeur accumulée de tous les noeuds parents
    while (node != NULL)
    {
        node->visits++;

        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            node->gain_coup[i] += value[i];
        }

        node = node->parent;
    }
}

void simulate_node(mcts_t *node)
{
    if(node == NULL)
    {
        fprintf(stderr, "[ERREUR] simulate_node le noeud est NULL\n");
    }
    while (node->state->win == -1)
    {
        int input = rand() % NUM_PLAYERS;
        game_play(node->state, input);
    }

    return;
}

bool root_explored(mcts_t *root)
{
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (root->children[i] == NULL)
        {
            return false;
        }
    }
    return true;
}

void *expand_node(mcts_t *parent, rb_tree_t *rb_tree, int input)
{
    if (parent->children[input] != NULL)
    {
        int input2 = select_node(parent->children[input]);
        return parent->children[input]->children[input2] = expand_node(parent->children[input], rb_tree, input2);
    }
    else
    {
        mcts_t *node = create_node(parent, parent->state);
        game_play(node->state, input);
        node->id = gen_id(node->state);

        mcts_t *search = rb_tree_search(rb_tree, node->id);
        if (search != NULL)
        {
            // Le noeud existe déjà.
            free_mtsc_node(node);
            fprintf(stderr, "[WARNING] Le noeud existe déjà !\n");
            parent->children[input] = node;
        }
        else
        {
            rb_tree = rb_tree_insert(rb_tree, node);
        }

        parent->children[input] = node;

        // On simule le noeud
        simulate_node(parent->children[input]);
        parent->n_coup[input] = parent->n_coup[input];

        // On récupère le score
        int *score = get_reward(parent->children[input]->state);

        // On rétro-propage le score obtenu
        backpropagate_node(parent->children[input], score);
        return node;
    }
}

void mcts_aux(mcts_t *root, rb_tree_t *rb_tree)
{
    int i = 0;
    while (i < NUM_ITERATIONS)
    {
        // On sélectionne un coup parmis ceux non exploré
        int input = select_node(root);

        // On étend le noeud
        root->children[input] = expand_node(root, rb_tree, input);

        i++;
    }
}

/**
 * @brief MCTS
 *
 * @param game l'état du jeu
 * @return le meilleur coup à jouer
 */
int mcts(game_t *game)
{
    mcts_t *root = create_node(NULL, game);
    root->id = gen_id(root->state);

    rb_tree_t *rb_tree = rb_tree_create();
    rb_tree = rb_tree_insert(rb_tree, root);

    mcts_aux(root, rb_tree);

    // On choisit le meilleur coup
    int best_input = 0;
    int max = -100;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (max < root->children[i]->gain_coup[game->player_action])
        {
            max = root->children[i]->gain_coup[game->player_action];
            best_input = i;
        }
    }

    free_mtsc_node(root);
    free_rb_tree(rb_tree);

    return best_input;
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
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        player->tank[i] = 0;
    }
    return player;
}
