#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
stack_t *stack_push(stack_t *stack, int face, int back[2])
{
    stack_t *new_stack = malloc(sizeof(stack_t));
    if (new_stack == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    new_stack->card.face = face;
    for (int i = 0; i < 2; i++)
    {
        new_stack->card.back[i] = back[i];
    }
    new_stack->next = stack;
    return new_stack;
}

/**
 * @brief Retourne la face du sommet de la pile
 *
 * @param stack la pile
 * @return int
 */
int stack_top(stack_t *stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Erreur: pile vide\n");
        exit(EXIT_FAILURE);
    }
    return stack->card.face;
}

/**
 * @brief Retire le sommet de la pile
 *
 * @param stack la pile
 */
stack_t *stack_pop(stack_t *stack)
{
    if (stack_is_empty(stack))
    {
        fprintf(stderr, "Erreur: pile vide\n");
        exit(EXIT_FAILURE);
    }
    stack_t *new_stack = stack->next;
    free(stack);
    return new_stack;
}

/**
 * @brief Retourne la taille de la pile
 *
 * @param stack la pile
 * @return int
 */
int stack_size(stack_t *stack)
{
    int size = 0;
    while (!stack_is_empty(stack))
    {
        size++;
        stack = stack->next;
    }
    return size;
}

/**
 * @brief Libère la mémoire allouée à la pile
 *
 * @param stack la pile
 */
void stack_free(stack_t *stack)
{
    while (!stack_is_empty(stack))
    {
        stack = stack_pop(stack);
    }
}

/*************************** Fonctions de la structure de données pile *****************************/