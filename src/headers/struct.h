#define STRUCT_H

// Structure de pile
struct stack
{
    int value;
    struct stack *next;
};
typedef stack stack_t;

struct player
{
    int tank[4];          // Carte tank de joueur
    int score;            // Score du joueur
    int last_scored_card; // Valeur de la dernière carte jouée
};
typedef player player_t;

struct card
{
    int back[3]; // Dos de la carte
    int face;    // Face de la carte
};
typedef card card_t;

struct game
{
    int players[3];    // Nombre de joueurs
    int player_action; // Joueur actif
    int win[4];        // 0 si joueur n'a pas gagné, 1 si joueur a gagné
    stack *draw_pile;  // Pile de pioche
};
typedef game game_t;

/********************Fonctions utiles pour la*************************/
stack_t * stack_create();
int stack_is_empty(stack_t *stack);
stack_t * stack_push(stack_t *stack, int value);
int stack_top(stack_t *stack);
stack_t * stack_pop(stack_t *stack);
int stack_size(stack_t *stack);
void stack_free(stack_t *stack);
/********************Fonctions utiles pour la*************************/


