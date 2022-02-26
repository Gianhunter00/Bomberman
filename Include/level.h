#pragma once
#include "parse_bmp.h"
#include "level001.h"

#define BLOCK_MASK_UNWALKABLE 0x0100
#define BLOCK_MASK_TELEPORT 0x0200
#define BLOCK_MASK_EXIT 0x0400

#define BLOCK_GROUND 0
#define BLOCK_WALL 1
#define BLOCK_DESTROYABLE 2

/** \struct level_t
 * \brief level structure
 * @param cells content of the level
 * @param cols number of columns of the level grid
 * @param rows number of rows of the level grid
 * @param cell_size size in pixel of a single cell
 */
typedef struct level
{
    int32_t *cells;     
    uint32_t cols;      
    uint32_t rows;      
    uint32_t cell_size; 
} level_t;

/** \enum block_type_t
 * \brief the available texture type
 * @param BLOCK_GROUND_TEXTURE Ground type texture
 * @param BLOCK_WALL_TEXTURE Wall type texture
 * @param BLOCK_DESTROYABLE_TEXTURE Destroyable type texture
 */
typedef enum block_type
{
    BLOCK_GROUND_TEXTURE = 0,   /* Ground type texture*/
    BLOCK_WALL_TEXTURE,         /* Wall type texture*/
    BLOCK_DESTROYABLE_TEXTURE,  /* Destroyable type texture*/
    TEXTURE_MAX                 /* LAST */
} block_type_t;

/** Init of the level texture drawer
 */
void level_level_texture_init();

/** Free of the level texture drawer
 */
void level_level_texture_free();

/** Get the level texture with the given block_type_c code
 *
 * @param keyTexture the block_type_t of the texture you want
 * 
 * @result the texture with the given code
 */
SDL_Texture *get_level_texture(block_type_t keyTexture);

/** Load all the texture needed for the level
 *
 * @param renderer the renderer currently in use
 */
void level_load_texture(SDL_Renderer *renderer);

/** Init the level passed with the given data
 *
 * @param level to initialize
 * @param cols number of cols in the level
 * @param rows number of rows in the level
 * @param cell_size the size of each cell
 * @param cells the map
 * 
 * @result 0 if no error occured -1 otherwise
 */
int level_init(level_t *level, const uint32_t cols, const uint32_t rows, const uint32_t cell_size, int32_t *cells);


/** Get the cell at the given coordinate
 *
 * @param level map
 * @param col col of the cell
 * @param row row of the cell
 * 
 * @result the index of the cell if no error occured -1 otherwise
 */
int32_t level_cell(level_t *level, const uint32_t col, const uint32_t row);

/** Draw of the level
 *
 * @param level_to_draw map to draw
 * @param renderer the renderer currently in use
 */
void level_draw_cell(level_t *level_to_draw, SDL_Renderer *renderer);