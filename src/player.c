#include <stdio.h>
#include <stdlib.h>

#include "headers/player.h"
#include "headers/struct.h"


/**
*@brief Initialisation d'un joueur
*/
player_t init_player () 
{
    player_t player;
    player.score = 0;
    player.last_scored_card = 0;
    for (int i = 0; i < 5; i++)
    {
        player.tank[i] = 0;
    }
    return player;
}
