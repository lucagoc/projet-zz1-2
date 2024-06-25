#define ANIMATION_H

void draw_confetti(ui_t *ui);
void flip_the_card(ui_t *ui, game_t *game);
void draw_particles(ui_t *ui, int x, int y);
void draw_steal(ui_t *ui, card_t *carte, game_t *game, player_t *stolen);
