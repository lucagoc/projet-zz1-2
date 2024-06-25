#define STRUCT_H

#include <stdbool.h>
struct card
{
    int back[3]; // Dos de la carte
    int face;    // Face de la carte
};
typedef struct card card_t;

// Structure de pile
struct stack
{
    card_t *card;
    struct stack *next;
};
typedef struct stack stack_t;

struct player
{
    int tank[7];          // Carte tank de joueur
    int score;            // Score du joueur
    int last_scored_card; // Valeur de la dernière carte jouée
};
typedef struct player player_t;

struct game
{
    player_t *players[4]; // Tableau des joueurs, numéroté de 0 à 3
    stack_t *draw_pile;   // Pile de pioche
    int drawn_card_color;
    int player_action;    // Joueur actif
    int win;              // 0 si joueur n'a pas gagné, 1 si joueur 1 a gagné
};
typedef struct game game_t;

/******************** Fonctions utiles pour la pile *************************/
stack_t *stack_create();
bool stack_is_empty(stack_t *stack);
stack_t *stack_push(stack_t *stack, card_t *card);
int stack_top(stack_t *stack);
stack_t *stack_pop(stack_t *stack);
int stack_size(stack_t *stack);
void stack_free(stack_t *stack);
/******************** Fonctions utiles pour la pile *************************/
