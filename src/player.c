#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "headers/struct.h"
#include "headers/player.h"
#include "headers/gameplay.h"

#define NUM_PLAYERS 4      // Nombre de joueurs
#define NUM_ARMS 10        // Nombre de bras
#define NUM_ITERATIONS 200 // Nombre total d'itérations
#define UCB_ITERATIONS 100 // Nombre d'itérations pour UCB

float I_k(float G, float C, int n, int n_k)
{
    float res = G / n_k + C * sqrt(log(n) / n_k);
    return res;
}

// On utilise comme récompense directement le score des joueur
int get_reward(game_t *game, int player)
{
    return game->players[player]->score;
}

// Calcul pour le prochain coup si il faut scorer ou voler à l'aide de l'algorithme UCB (Upper Confidence Bound)
// Renvoie un tableau des estimations de récompenses pour chaque bras
// n le nombre de totale de partie jouée
// Renvoie l'input i avec le meilleur score
int ucb(mcts_t *root, int n)
{
    game_t *game = root->state;
    /* Initialisation */
    int player = game->player_action;
    float C = 1.4; // Constante d'exploration
    int max = 0;
    int G[NUM_PLAYERS];   // Valeur de la récompense pour chaque possibilité
    int n_t[NUM_PLAYERS]; // Nombre de fois où l'on a joué sur la machine
    int I[NUM_PLAYERS];   // Valeur de l'indice de confiance pour chaque possibilité
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        G[i] = root->gain_coup[i];
        I[i] = 0;
        n_t[i] = root->n_coup[i];
    }

    // On joue une fois sur toute les machines
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        game_t *copy = copy_game(game);
        game_play(copy, i % 4);
        int reward = get_reward(copy, player);
        I[i] = I_k(G[i], C, n, 1); // 1 = le nombre de fois où l'on a déjà joué sur la machine.
        G[i] += reward;
        n_t[i]++;
        free_game(copy);
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
    for (int i = 0; i < 4; i++)
    {
        if (root->children[i] == NULL)
        {
            return i;
        }
    }

    // Dans l'autre cas.
    return ucb(root, UCB_ITERATIONS);
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
    node->gain_coup = malloc(sizeof(int) * NUM_PLAYERS);
    node->n_coup = malloc(sizeof(int) * NUM_PLAYERS);
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
    free(node->gain_coup);
    free(node->n_coup);
    free(node->id);
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
void backpropagate_node(mcts_t *node, int value)
{
    int back_input = node->from_input;
    node = node->parent;
    // Mettre à jour les visites et la valeur accumulée de tous les noeuds parents
    while (node != NULL)
    {
        node->visits++;

        node->gain_coup[back_input] += value;
        node->n_coup[back_input]++;

        back_input = node->from_input;
        node = node->parent;
    }
}

void simulate_node(mcts_t *node)
{
    if (node == NULL)
    {
        fprintf(stderr, "[ERROR] copy_game : Can't allocate memory\n");
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

rb_tree_t *expand_node(mcts_t *parent, rb_tree_t *rb_tree, int input)
{
    if (parent->children[input] != NULL)
    {
        int input2 = select_node(parent->children[input]);
        rb_tree = expand_node(parent->children[input], rb_tree, input2);
    }
    else
    {
        int player = parent->state->player_action;
        mcts_t *node = create_node(parent, parent->state);
        node->from_input = input;
        game_play(node->state, input); // Jouer une fois

        // Jouer N-1 coups aléatoires pour revenir à la prochaine vue
        for (int i = 0; i < NUM_PLAYERS - 1; i++)
        {
            game_play(node->state, rand() % NUM_PLAYERS);
        }

        node->id = gen_id(node->state);

        mcts_t *search = rb_tree_search(rb_tree, node->id);
        if (search != NULL)
        {
            // Le noeud existe déjà.
            free_mtsc_node(node);
            fprintf(stderr, "[WARNING] expand_node : the node already exist (safe to ignore)\n");
            parent->children[input] = node;
        }
        else
        {
            rb_tree = rb_tree_insert(rb_tree, node);
        }

        parent->children[input] = node;

        // On simule le noeud
        simulate_node(parent->children[input]);
        parent->n_coup[input] = parent->n_coup[input] + 1;

        // On récupère le score
        int score = get_reward(parent->children[input]->state, player);
        node->gain_coup[input] = score;

        // On rétro-propage le score obtenu
        backpropagate_node(parent->children[input], score);
    }

    return rb_tree;
}

rb_tree_t *mcts_aux(mcts_t *root, rb_tree_t *rb_tree)
{
    int i = 0;
    while (i < NUM_ITERATIONS)
    {
        // On sélectionne un coup parmis ceux non exploré
        int input = select_node(root);

        // On étend le noeud, simule, rétro-propage
        rb_tree = expand_node(root, rb_tree, input);

        i++;
    }

    return rb_tree;
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
    root->from_input = -1; // utilisé pour faire remonter le score

    rb_tree_t *rb_tree = rb_tree_create();
    rb_tree = rb_tree_insert(rb_tree, root);

    rb_tree = mcts_aux(root, rb_tree);

    // On choisit le meilleur coup
    int best_input = 0;
    float max = -100;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        float a = root->gain_coup[i] / (float)root->n_coup[i];
        if (max < a)
        {
            max = a;
            best_input = i;
        }
    }

    printf("Best input : %d\n", best_input);

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
