#pragma once
#include "..\Include\movable.h"
#include "..\Include\parse_bmp.h"
#include "..\Include\level001.h"

typedef enum block_type
{
    BLOCK_GROUND_TEXTURE = 0,
    BLOCK_WALL_TEXTURE = 1,
    BLOCK_DESTROYABLE_TEXTURE = 2,
    TEXTURE_MAX = 3
} block_type_t;

void level_level_texture_init();

void level_level_texture_free();

SDL_Texture *get_level_texture(block_type_t keyTexture);

void level_load_texture(SDL_Renderer *renderer);

int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size, int32_t *cells);

int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row);

void level_draw_cell(level_t *level_to_draw, SDL_Renderer *renderer);