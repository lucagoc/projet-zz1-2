#define ANIMATION_H

void draw_confetti(ui_t *ui);
void flip_the_card(ui_t *ui, game_t *game, int x, int y);
void draw_particles(ui_t *ui, game_t *game, int x, int y);
void draw_steal(ui_t *ui, game_t *game);
void fct_move_animation(anim_props_t *anim, SDL_Renderer *renderer, int frame);
void animation_runtime(ui_t *ui, anim_props_t *animation, void (func_anim)(anim_props_t*, SDL_Renderer*, int));