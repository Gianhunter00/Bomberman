#include "level.h"
#include <string.h>
#include <stdio.h>

static SDL_Texture **level_texture = NULL;

void level_level_texture_init()
{
    level_texture = malloc(sizeof(SDL_Texture *) * TEXTURE_MAX);
    return;
}
void level_level_texture_free()
{
    free(level_texture);
    level_texture = NULL;
    return;
}

SDL_Texture *get_level_texture(block_type_t key_texture)
{
    if (key_texture >= TEXTURE_MAX)
        return NULL;
    return (level_texture) ? level_texture[key_texture] : NULL;
}

void level_load_texture(SDL_Renderer *renderer)
{
    if (level_texture == NULL || renderer == NULL)
        return;
    level_texture[BLOCK_GROUND_TEXTURE] = create_texture_from_BMP_file("bin\\resources\\Sprites\\BMP\\Block\\BackgroundTile.bmp", "rb", renderer);
    level_texture[BLOCK_WALL_TEXTURE] = create_texture_from_BMP_file("bin\\resources\\Sprites\\BMP\\Block\\SolidBlock.bmp", "rb", renderer);
    level_texture[BLOCK_DESTROYABLE_TEXTURE] = create_texture_from_BMP_file("bin\\resources\\Sprites\\BMP\\Block\\ExplodableBlock.bmp", "rb", renderer);
    level_texture[TEXTURE_MAX] = NULL;
}

int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size, int32_t *cells)
{
    if (!level || !cols || !rows || !cell_size)
        return -1;

    level->cols = cols;
    level->rows = rows;
    level->cell_size = cell_size;
    level->cells = cells;
    return 0;
}

int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row)
{
    if (!level->cells)
        return -1;
    if (col >= level->cols)
        return -1;
    if (row >= level->rows)
        return -1;

    return level->cells[row * level->cols + col];
}

void level_draw_cell(level_t* level_to_draw, SDL_Renderer* renderer)
{
    SDL_Rect cell_rect = {0, 0, level_to_draw->cell_size, level_to_draw->cell_size};
    for (uint32_t row = 0; row < level_to_draw->rows; row++)
    {
        for (uint32_t col = 0; col < level_to_draw->cols; col++)
        {
            int32_t cell = level_cell(level_to_draw, col, row);
            int32_t cell_texture = cell & 0xff;
            cell_rect.x = col * level_to_draw->cell_size;
            cell_rect.y = row * level_to_draw->cell_size;

            if (cell_texture == BLOCK_GROUND)
            {
                SDL_RenderCopy(renderer, get_level_texture(BLOCK_GROUND_TEXTURE), NULL, &cell_rect);
            }
            else if (cell_texture == BLOCK_WALL)
            {
                SDL_RenderCopy(renderer, get_level_texture(BLOCK_WALL_TEXTURE), NULL, &cell_rect);
            }
            else if (cell_texture == BLOCK_DESTROYABLE)
            {
                SDL_RenderCopy(renderer, get_level_texture(BLOCK_DESTROYABLE_TEXTURE), NULL, &cell_rect);
            }
        }
    }
}