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
    for (int i = 0; i < 4; i++)
    {
        player->tank[i] = 0;
    }
    return player;
}
