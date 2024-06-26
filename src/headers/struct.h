#define STRUCT_H

#include <stdbool.h>

#define RED 0
#define BLACK 1

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
    int draw_pile_left[7];   // Nombre de cartes restantes dans la pioche pour chaque couleur
    int face_card_color;
    int back_card_color[3];
    int player_action; // Joueur actif
    int win;           // 0 si joueur n'a pas gagné, 1 si joueur 1 a gagné
    int stealing;
};
typedef struct game game_t;

// 19 chiffres par id
struct node_id
{
    unsigned long long id_game;         // Nombre de cartes dans la pile par couleur, joueur qui joue. 
    unsigned long long id_player_0;     // Nombre de carte dans le deck, score
    unsigned long long id_player_1;     // ...
    unsigned long long id_player_2;     // ...
    unsigned long long id_player_3;     // ...
};
typedef struct node_id node_id_t;

struct mcts
{
    node_id_t id;             // L'identifiant du noeud
    game_t *state;            // L'état du jeu correspondant à ce noeud
    struct mcts *parent;      // Le noeud parent dans l'arbre MCTS
    struct mcts **children;   // Les noeuds enfants (mouvements possibles à partir de cet état) [input donnée en entrée]
    int visits;               // Le nombre de fois que ce noeud a été visité
    double accumulated_value; // La valeur accumulée des simulations
};
typedef struct mcts mcts_t;

struct rb_tree
{
    mcts_t *value;
    struct rb_tree *left;
    struct rb_tree *right;
    int color;
};
typedef struct rb_tree rb_tree_t;

/******************** Fonctions utiles pour la pile *************************/
stack_t *stack_create();
bool stack_is_empty(stack_t *stack);
stack_t *stack_push(stack_t *stack, card_t *card);
int stack_top(stack_t *stack);
stack_t *stack_pop(stack_t *stack);
int stack_size(stack_t *stack);
void stack_free(stack_t *stack);
/******************** Fonctions utiles pour la pile *************************/

void print_node_id(node_id_t id);
node_id_t *gen_id(game_t *game);