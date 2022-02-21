#include "..\Include\movable.h"
#include <stdio.h>

bool check_collision(level_t *level, SDL_Rect *rect_to_check, int collision_mask)
{
    SDL_Rect cell_rect = {0, 0, level->cell_size, level->cell_size};
    SDL_Rect result = {0, 0, 0, 0};
    for (uint32_t row = 0; row < level->rows; row++)
    {
        for (uint32_t col = 0; col < level->cols; col++)
        {
            int32_t cell = level_cell(level, col, row);
            int32_t cell_collision_mask = cell & 0xff;
            cell_rect.x = col * level->cell_size;
            cell_rect.y = row * level->cell_size;

            if ((cell_collision_mask & collision_mask) != 0)
            {
                if (SDL_IntersectRect(&cell_rect, rect_to_check, &result) == SDL_TRUE)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

int32_t move_on_level(level_t *level, movable_t *movable, const float delta_x, const float delta_y, int collision_mask)
{
    float new_x = movable->x + delta_x;
    float new_y = movable->y + delta_y;
    SDL_Rect rect_next_pos = {new_x + movable->x_offset, new_y + movable->y_offset, movable->width - 2 * movable->x_offset, movable->height - movable->y_offset};

    if (collision_mask != 0)
        if (check_collision(level, &rect_next_pos, collision_mask))
        {
            return 0;
        }

    if (new_x + movable->x_offset < 0)
        new_x = -movable->x_offset;
    else if (new_x + movable->width - movable->x_offset >= level->cols * level->cell_size)
        new_x = (level->cols * level->cell_size) - movable->width + movable->x_offset;
    if (new_y + movable->y_offset < 0)
        new_y = -movable->y_offset;
    else if (new_y + movable->height >= level->rows * level->cell_size)
        new_y = (level->rows * level->cell_size) - movable->height;

    movable->x = new_x;
    movable->y = new_y;

    return -1;
}