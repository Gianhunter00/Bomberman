#pragma once
#include "movable.h"
#include <SDL.h>
#define BOMBERMAN_MASK 3

/** \struct bomberman_t
 * \brief bomberman struct
 * @param code Unique code for each bomberman
 * @param movable movable struct for bomberman
 * @param number_of_bombs number of bombs of the bomberman
 * @param dropped_bombs number of dropped bombs of the bomberman
 * @param bomb_power bomb power
 * @param dir Horizontal[0] and Vertical[1] direction, 1 is right down -1 is left up
 * @param rect_to_draw rect to draw the bomberman texture
 * @param texture_to_draw bomberman's texture
 */
typedef struct bomberman
{
    int code;                       /* Unique code for each bomberman */
    movable_t movable;              /* movable struct for bomberman */
    uint32_t number_of_bombs;       /* number of bombs of the bomberman */
    uint32_t dropped_bombs;         /* number of dropped bombs of the bomberman */
    uint32_t bomb_power;            /* bomb power */
    float dir[2];                   /* Horizontal[0] and Vertical[1] direction, 1 is right down -1 is left up */
    SDL_Rect rect_to_draw;          /* rect to draw the bomberman texture */ 
    SDL_Texture* texture_to_draw;   /* bomberman's texture */
} bomberman_t;

/** Creating a new bomberman and return the pointer to it
 * 
 * @param x position x of the bomberman
 * @param y position y of the bomberman
 * @param width width of the bomberman
 * @param height height of the bomberman
 * @param speed speed of the bomberman
 * @result bomberman pointer to the new created bomberman
 */
bomberman_t *bomberman_new(float x, float y, uint32_t width, uint32_t height, float speed);

/** Free for bomberman
 * 
 * @param bomberman bomberman pointer to free
 */
void bomberman_free(bomberman_t* bomberman);

/** Change texture for the given bomberman
 * 
 * @param bomberman bomberman to change texture
 * @param new_texture new texture to give to the bomberman
 */
void bomberman_new_texture(bomberman_t *bomberman, SDL_Texture* new_texture);

/** Input update for the bomberman
 * 
 * @param bomberman bomberman to give input
 * @param event current sdl event in use
 */
void bomberman_input(bomberman_t* bomberman, SDL_Event* event);

/** Bomberman drawer
 * 
 * @param bomberman bomberman to draw
 * @param renderer renderer to use
 */
void bomberman_draw(bomberman_t *bomberman, SDL_Renderer *renderer);

/** Move on level for bomberman using his blocking mask for obstacle
 * 
 * @param bomberman bomberman to move
 * @param level level where bomberman is moving
 */
void bomberman_move_on_level(bomberman_t* bomberman, level_t* level);

/** Bomberman add unique code
 *
 * currently using the port since it can be binded by only one at a time in local
 * 
 * @param bomberman bomberman to add code
 * @param code code of the bomberman
 */
void bomberman_update_code(bomberman_t* bomberman, int code);