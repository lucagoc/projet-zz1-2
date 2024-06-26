#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "headers/struct.h"

/*************************** Fonctions de la structure de données pile *****************************/
/**
 * @brief Création d'une pile vide
 *
 * @return stack_t*
 */
stack_t *stack_create()
{
    return NULL;
}

/**
 * @brief Vérifie si la pile est vide
 *
 * @param stack la pile à vérifier
 * @return int
 */
bool stack_is_empty(stack_t *stack)
{
    return stack == NULL;
}

/**
 * @brief Ajoute un élément à la pile
 *
 * @param stack la pile
 * @param face la face de la carte
 * @param back le dos de la carte
 * @return stack_t*
 */
stack_t *stack_push(stack_t *stack, card_t *card)
{
    stack_t *new_stack = malloc(sizeof(stack_t));
    if (new_stack == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    new_stack->card = card;
    new_stack->next = stack;
    return new_stack;
}

/*************************** Fonctions de la structure de données pile *****************************/

// Arbre rouge-Noir
rb_tree_t *rb_tree_create()
{
    return NULL;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sort_three(int *x, int *y, int *z)
{
    if (*x > *y)
    {
        swap(x, y);
    }
    if (*x > *z)
    {
        swap(x, z);
    }
    if (*y > *z)
    {
        swap(y, z);
    }
}

int concat_sorted(int a, int b, int c)
{
    sort_three(&a, &b, &c);
    int concatenated = a * 100 + b * 10 + c;

    return concatenated;
}

node_id_t *gen_id(game_t *game)
{
    node_id_t *id = malloc(sizeof(node_id_t));
    if (id == NULL)
    {
        fprintf(stderr, "gen_id : Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // On génère un id unique pour chaque noeud
    id->id_game = 1;
    for (int i = 0; i < 7; i++)
    {
        id->id_game = id->id_game * 100 + game->draw_pile_left[i];
    }

    // Ajouter le numéro des cartes au dos en triant par ordre croissant
    // En effet, une carte avec les 3 mêmes couleurs au dos sont indépendantes
    // de leur ordre d'apparition sur la carte
    int res = concat_sorted(game->back_card_color[0], game->back_card_color[1], game->back_card_color[2]);
    id->id_game = id->id_game * 1000 + res;

    // Ajouter le joueur qui joue
    id->id_game = id->id_game * 10 + game->player_action;

    id->id_player_0 = 1;
    for (int i = 0; i < 7; i++)
    {
        id->id_player_0 = id->id_player_0 * 100 + game->players[0]->tank[i];
    }
    id->id_player_0 = id->id_player_0 * 100 + game->players[0]->score;

    id->id_player_1 = 1;
    for (int i = 0; i < 7; i++)
    {
        id->id_player_1 = id->id_player_1 * 100 + game->players[1]->tank[i];
    }
    id->id_player_1 = id->id_player_1 * 100 + game->players[1]->score;

    id->id_player_2 = 1;
    for (int i = 0; i < 7; i++)
    {
        id->id_player_2 = id->id_player_2 * 100 + game->players[2]->tank[i];
    }
    id->id_player_2 = id->id_player_2 * 100 + game->players[2]->score;

    id->id_player_3 = 1;
    for (int i = 0; i < 7; i++)
    {
        id->id_player_3 = id->id_player_3 * 100 + game->players[3]->tank[i];
    }
    id->id_player_3 = id->id_player_3 * 100 + game->players[3]->score;

    // On prend le nombre de cartes restantes dans la pioche
    return id;
}

// Cherche un ID dans l'arbre rouge-noir et renvoie le noeud correspondant ou null si introuvable
mcts_t *rb_tree_search(rb_tree_t *tree, node_id_t *id)
{
    if (tree == NULL)
    {
        return NULL;
    }

    int cmp = memcmp(&tree->value->id, id, sizeof(node_id_t));
    if (cmp == 0)
    {
        return tree->value;
    }
    else if (cmp < 0)
    {
        return rb_tree_search(tree->left, id);
    }
    else
    {
        return rb_tree_search(tree->right, id);
    }
}

void rb_tree_rotate_left(rb_tree_t **tree)
{
    rb_tree_t *right = (*tree)->right;
    (*tree)->right = right->left;
    right->left = *tree;
    *tree = right;
}

void rb_tree_rotate_right(rb_tree_t **tree)
{
    rb_tree_t *left = (*tree)->left;
    (*tree)->left = left->right;
    left->right = *tree;
    *tree = left;
}

void rb_tree_flip_colors(rb_tree_t *tree)
{
    tree->color = RED;
    tree->left->color = BLACK;
    tree->right->color = BLACK;
}

void rb_tree_insert_fixup(rb_tree_t **tree)
{
    if ((*tree)->left != NULL && (*tree)->left->color == RED && (*tree)->left->left != NULL && (*tree)->left->left->color == RED)
    {
        rb_tree_rotate_right(tree);
    }
    if ((*tree)->right != NULL && (*tree)->right->color == RED)
    {
        rb_tree_rotate_left(tree);
    }
    if ((*tree)->left != NULL && (*tree)->left->color == RED && (*tree)->right != NULL && (*tree)->right->color == RED)
    {
        rb_tree_flip_colors(*tree);
    }
}

// Les rb_tree utilise strcmp comme relation d'ordre
rb_tree_t *rb_tree_insert(rb_tree_t *tree, mcts_t *value)
{
    if (tree == NULL)
    {
        tree = malloc(sizeof(rb_tree_t));
        if (tree == NULL)
        {
            fprintf(stderr, "rb_tree_insert : Erreur d'allocation de mémoire\n");
            exit(EXIT_FAILURE);
        }
        tree->value = value;
        tree->left = NULL;
        tree->right = NULL;
        tree->color = RED;
        return tree;
    }

    int cmp = memcmp(&tree->value->id, &value->id, sizeof(node_id_t));
    if (cmp == 0)
    {
        return tree;
    }
    else if (cmp < 0)
    {
        tree->left = rb_tree_insert(tree->left, value);
    }
    else
    {
        tree->right = rb_tree_insert(tree->right, value);
    }

    rb_tree_insert_fixup(&tree);
    return tree;
}

void *free_rb_tree(rb_tree_t *tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    free_rb_tree(tree->left);
    free_rb_tree(tree->right);
    free(tree);
    return NULL;
}