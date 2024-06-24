#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "headers/gameplay.h"


int is_card_in_tank(int player, game_t * game){
    //renvoie 0 si la couleur n'est pas dans le tank de player, i>0 sinon

    return game->player[game->active_player]->tank[game->drawn_card_color];

}

int draw_card(game_t * game){
    //dépile la pile et renvoie la couleur de la première carte

    if (game->stack != NULL){
        int drawn_card_color = game->stack->value; //on récupère la couleur du haut de la pile

        game->stack=game->stack->next; //on passe à l'élément suivant de la pile

        return drawn_card_color;
    } else {
        return NULL;
    }
    
}

void score_card(game_t * game){

    game->player[game->active_player]->score += game->player[game->active_player]->tank[game->drawn_card_color] + 1; //on ajoute les cartes au score du joueur
    
    game->player[game->active_player]->tank[game->drawn_card_color] = 0; //on enlève les cartes du tank

    game->player[game->active_player]->last_scored_card= game->drawn_card_color; //on affiche la carte en haut de la pile de score

}

void add_card_in_tank(int player, game_t * game){

    game->player[player]->tank[game->drawed_card_color] = 1; //on ajoute la carte au tank


}

void steal_card(int input, game_t * game){

    game->player[game->active_player]->tank[game->drawn_card_color] += game->player[input]->tank[game->drawn_card_color] +1; //on récupère les cartes volées

    game->player[input]->tank[game->drawn_card_color]=0; //on enlève les cartes au joueur volé

}



void game_play(game_t * game, int input){ 

    //input vaut 0 si le joueur actif clique sur sa propre pile et i>0 s'il clique sur le joueur i pour le voler
    
    game->drawn_card_color = draw_card(game->draw_pile); //on dépile et on affiche 

    if (input){ //le joueur actif choisit de marquer



        if (is_card_in_tank(game->active_player, game)){ //s'il tombe sur une bonne couleur qu'il a
            
            score_card(game);

        } else { //s'il tombe sur une couleur qu'il n'a pas

            add_card_in_tank(game->active_player,game);

        }

    } else {    //le joueur actif choisit de voler

        if (is_card_in_tank(input, game)){ //s'il tombe sur une bonne couleur qu'il a
            
            steal_card(input, game);

        } else { //s'il tombe sur une couleur qu'il n'a pas
            add_card_in_tank(input, game);

        }
    }
}