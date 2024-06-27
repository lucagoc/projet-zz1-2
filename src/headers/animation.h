#define ANIMATION_H

void draw_confetti(ui_t *ui);
void fct_anim_flip(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame);
void fct_anim_particles(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame);
void fct_move_animation(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame);
void fct_anim_confettis(anim_props_t *anim, SDL_Renderer *renderer, long unsigned frame);
void animation_runtime(ui_t *ui, anim_props_t *animation, void (func_anim)(anim_props_t*, SDL_Renderer*, long unsigned));
void start_animation(anim_props_t *animation, long unsigned delay);
void end_animation(anim_props_t *animation);