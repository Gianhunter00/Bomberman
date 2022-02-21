#pragma once
#include "..\Include\movable.h"
#include <SDL.h>
#define BOMBERMAN_MASK 3

typedef struct bomberman
{
    int code;
    movable_t movable;
    uint32_t number_of_bombs;
    uint32_t dropped_bombs;
    uint32_t bomb_power;
    float dir[2];
    SDL_Rect rect_to_draw; 
    SDL_Texture* texture_to_draw;
} bomberman_t;

bomberman_t *bomberman_new(float x, float y, uint32_t width, uint32_t height, float speed);

void bomberman_free(bomberman_t* bomberman);

void bomberman_new_texture(bomberman_t *bomberman, SDL_Texture* new_texture);

void bomberman_input(bomberman_t* bomberman, SDL_Event* event);

void bomberman_draw(bomberman_t *bomberman, SDL_Renderer *renderer);

void bomberman_move_on_level(bomberman_t* bomberman, level_t* level);

void bomberman_update_code(bomberman_t* bomberman, int code);