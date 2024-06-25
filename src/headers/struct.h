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
    player_t *players[4];           // Tableau des joueurs, numéroté de 0 à 3
    stack_t *draw_pile;             // Pile de pioche
    int drawn_card_color;
    int player_action;              // Joueur actif
    int win;                        // 0 si joueur n'a pas gagné, 1 si joueur 1 a gagné etc.
};
typedef struct game game_t;

struct mcts
{
    game_t *state;                      // L'état du jeu correspondant à ce noeud
    int player;                         // Le joueur actif dans cet état du jeu
    struct mcts *parent;                // Le noued parent dans l'arbre MCTS
    struct mcts **possible_move;        // Les noueds enfants (mouvements possibles à partir de cet état)
    int num_children;                   // Le nombre d'enfants
    int visits;                         // Le nombre de fois que ce noeud a été visité
    double accumulated_value;           // La valeur accumulée des simulations
};
typedef struct mcts mcts_t;

/******************** Fonctions utiles pour la pile *************************/
stack_t *stack_create();
bool stack_is_empty(stack_t *stack);
stack_t *stack_push(stack_t *stack, card_t *card);
int stack_top(stack_t *stack);
stack_t *stack_pop(stack_t *stack);
int stack_size(stack_t *stack);
void stack_free(stack_t *stack);
/******************** Fonctions utiles pour la pile *************************/
