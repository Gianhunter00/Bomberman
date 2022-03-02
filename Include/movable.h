#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "level.h"

/** 
 * @struct movable_t
 * @brief Movable struct to move on the map
 * 
 * @param x x position of the movable
 * @param y y position of the movable
 * @param width width of the movable
 * @param height height of the movable
 * @param speed speed of the movable
 * @param x_offset x offset for collision (is both left and right)
 * @param y_offset y offset for collision it's only from top to bottom NOT bottom to top
 */
typedef struct movable
{
    float x;
    float y;
    uint32_t width;
    uint32_t height;
    float speed;
    float x_offset;
    float y_offset;
} movable_t;

/** 
 * @brief Check if collision happened
 *
 * @param level current level
 * @param rect_to_check rect to check the collision with the level
 * @param collision_mask collision mask of the owner of the rect
 * 
 * @result true if there was a collision, false if not
 */
bool check_collision(level_t* level, SDL_Rect *rect_to_check, int collision_mask);

/** 
 * @brief Move the movable on the level
 *
 * @param level current level
 * @param movable the movable to move on the level
 * @param delta_x movement on the horizontal axys
 * @param delta_y movement on the vertical axys
 * @param collision_mask collision mask of the owner of the movable
 * 
 * @result true if there was a collision, false if not
 */
int32_t move_on_level(level_t *level, movable_t *movable, const float delta_x, const float delta_y, int collision_mask);