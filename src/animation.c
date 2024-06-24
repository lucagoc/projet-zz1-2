#include <SDL2/SDL.h>
#include <stdbool.h>

#include "headers/sdl_common.h"


// Faire pleuvoir des confettis sur l'écran. Utilise ui->tick pour déterminer la position des confettis.
void draw_confetti(ui_t *ui)
{
    float speed = 0.001;
    for (int i = 0; i < 100; i++)
    {
        // Couleur aléatoire mais toujours la même pour un i donné.
        if(i % 4 == 0)
            SDL_SetRenderDrawColor(ui->renderer, 255, 0, 0, 255);
        else if(i % 4 == 1)
            SDL_SetRenderDrawColor(ui->renderer, 0, 255, 0, 255);
        else if(i % 4 == 2)
            SDL_SetRenderDrawColor(ui->renderer, 0, 0, 255, 255);
        else
            SDL_SetRenderDrawColor(ui->renderer, 255, 255, 0, 255);
        
        int x = fmod((ui->tick * 10 * i * speed), ui->screen_w);
        int y =  fmod((ui->tick * 10 * i * speed), ui->screen_h);
        SDL_Rect confetti = {x, y, 10, 10};
        SDL_RenderFillRect(ui->renderer, &confetti);
    }
    
}