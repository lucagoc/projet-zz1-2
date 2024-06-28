#define GAMEPLAY_H

void game_play(game_t *game, int input);
game_t *create_game();
void free_game(game_t *game);
game_t *copy_game(game_t *game);
void score_card(game_t *game);
void steal_card(int input, game_t *game);
int get_score(game_t *game, int player);
int is_victory(game_t *game);
stack_t *add_card_to_stack(stack_t *stack, card_t *card);
stack_t *remove_card_from_stack(stack_t *stack);
stack_t *create_stack();
void free_stack(stack_t *stack);
stack_t *mix_stack(stack_t *stack);
stack_t *gen_stack(game_t *game);
int get_draw_card(game_t *game);