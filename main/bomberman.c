#include "..\Include\bomberman.h"
#include <string.h>
#include <stdio.h>

bomberman_t *bomberman_new(float x, float y, uint32_t width, uint32_t height, float speed)
{
    bomberman_t *bomberman = malloc(sizeof(bomberman_t));
    bomberman->code = 0;
    bomberman->movable.x = x;
    bomberman->movable.y = y;
    bomberman->movable.width = width;
    bomberman->movable.height = height;
    bomberman->movable.speed = speed;
    bomberman->movable.y_offset = ((height / 2) + 12);
    bomberman->movable.x_offset = 6;
    bomberman->dir[0] = 0;
    bomberman->dir[1] = 0;
    bomberman->texture_to_draw = NULL;
    bomberman->rect_to_draw = (SDL_Rect){0, 0, bomberman->movable.width, bomberman->movable.height};
    return bomberman;
}
void bomberman_update_code(bomberman_t* bomberman, int code)
{
    if(bomberman == NULL) return;
    bomberman->code = code;
}

void bomberman_free(bomberman_t *bomberman)
{
    free(bomberman);
    bomberman = NULL;
}

void bomberman_input(bomberman_t *bomberman, SDL_Event *event)
{
    if (bomberman == NULL || event == NULL)
        return;
    memset(bomberman->dir, 0, sizeof(float) * 2);
    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_RIGHT)
        {
            bomberman->dir[0] = bomberman->movable.speed * (1.0 / 60);
            return;
        }
        else if (event->key.keysym.sym == SDLK_LEFT)
        {
            bomberman->dir[0] = -bomberman->movable.speed * (1.0 / 60);
            return;
        }
        else if (event->key.keysym.sym == SDLK_DOWN)
        {
            bomberman->dir[1] = bomberman->movable.speed * (1.0 / 60);
            return;
        }
        else if (event->key.keysym.sym == SDLK_UP)
        {
            bomberman->dir[1] = -bomberman->movable.speed * (1.0 / 60);
            return;
        }
    }
}

void bomberman_new_texture(bomberman_t *bomberman, SDL_Texture *new_texture)
{
    if (new_texture == NULL)
        return;
    bomberman->texture_to_draw = new_texture;
}

void bomberman_draw(bomberman_t *bomberman, SDL_Renderer *renderer)
{
    if (renderer == NULL || bomberman == NULL || bomberman->texture_to_draw == NULL)
        return;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, bomberman->texture_to_draw, NULL, &bomberman->rect_to_draw);
}

void bomberman_move_on_level(bomberman_t* bomberman, level_t* level)
{
    move_on_level(level, &bomberman->movable, bomberman->dir[0], bomberman->dir[1], BOMBERMAN_MASK);
    bomberman->rect_to_draw.x = bomberman->movable.x;
    bomberman->rect_to_draw.y = bomberman->movable.y;
}
