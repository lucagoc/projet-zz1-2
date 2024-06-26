#define GAMEPLAY_H
#include "headers/sdl_common.h"

void game_play(game_t *game, int input);
game_t *create_game();
void free_game(game_t *game);
game_t *copy_game(game_t *game);
void score_card(game_t *game);
void steal_card(game_t *game);
int get_score(game_t *game, int player);
int is_victory(game_t *game);
int is_card_in_tank(int player, game_t *game);
