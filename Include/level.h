#pragma once
#include "movable.h"
#include "parse_bmp.h"
#include "level001.h"

/*
 * \enum block_type_t
 * \brief the available texture type
 * @param BLOCK_GROUND_TEXTURE
 * @param BLOCK_WALL_TEXTURE
 * @param BLOCK_DESTROYABLE_TEXTURE
 */
typedef enum block_type
{
    BLOCK_GROUND_TEXTURE = 0,   /* Ground type texture*/
    BLOCK_WALL_TEXTURE,         /* Wall type texture*/
    BLOCK_DESTROYABLE_TEXTURE,  /* Destroyable type texture*/
    TEXTURE_MAX                 /* LAST */
} block_type_t;

/*
 * Init of the level texture drawer
 */
void level_level_texture_init();

/*
 * Free of the level texture drawer
 */
void level_level_texture_free();

/*
 * Get the level texture with the given block_type_c code
 *
 * @param keyTexture the block_type_t of the texture you want
 * 
 * @result the texture with the given code
 */
SDL_Texture *get_level_texture(block_type_t keyTexture);

void level_load_texture(SDL_Renderer *renderer);

int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size, int32_t *cells);

int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row);

void level_draw_cell(level_t *level_to_draw, SDL_Renderer *renderer);