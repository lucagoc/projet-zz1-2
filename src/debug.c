#include <stdio.h>
#include <stdlib.h>

#include "headers/struct.h"

/*
 * @file debug.c
 * @brief Fonctions de debug
 *
 */

void print_node_id(node_id_t id)
{
    printf("---------------------------------------- \n");
    printf("id_game : %llu\n", id.id_game);
    printf("id_player_0 : %llu\n", id.id_player_0);
    printf("id_player_1 : %llu\n", id.id_player_1);
    printf("id_player_2 : %llu\n", id.id_player_2);
    printf("id_player_3 : %llu\n", id.id_player_3);
    printf("---------------------------------------- \n");
}

void debug_print_id(game_t *game)
{
    node_id_t *gen = gen_id(game);
    print_node_id(*gen);
    free(gen);
}

