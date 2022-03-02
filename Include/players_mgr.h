#pragma once
#include "bomberman.h"
#define MAX_PLAYERS 3

/**
 * @brief Players mgr init
 * 
 */
void players_mgr_init();

/**
 * @brief Players mgr free
 * 
 */
void players_mgr_free();

/**
 * @brief Get the current index
 * 
 * @return the index of the last player connected
 */
int players_mgr_get_current_index();

/**
 * @brief Get the next index
 * 
 * @return the next index of the last player connected or -1 if MAX_PLAYERS reached 
 */
int players_mgr_next_index();

/**
 * @brief Check if any players has the given code (doesn't control self)
 * 
 * @param code the code to check
 * 
 * @return true if found false otherwise
 */
bool players_mgr_contain_code(int code);

/**
 * @brief Add a new player if the code isn't already in use
 * 
 * @param x the x position of the new player
 * @param y the y position of the new player
 * @param width the width of the new player
 * @param height the height of the new player
 * @param speed the speed of the new player
 * @param texture the texture of the new player
 * @param code the code of the new player
 * 
 * @return 1 if the code isn't already in use 0 otherwise
 */
int players_mgr_add_new_player(float x, float y, float width, float height, float speed, SDL_Texture *texture, int code);

/**
 * @brief Get a player using the index
 * 
 * @param index the index of the player you want
 * 
 * @return the player at the given index or NULL if index is invalid
 */
bomberman_t *players_mgr_get_by_index(int index);

/**
 * @brief Update the connected players position if connected
 * 
 * @param x the x of the connected player
 * @param y the y of the connected player
 * @param code the code of the connected player
 * 
 * @return 1 if the code exist 0 otherwise
 */
int players_mgr_update_player(float x, float y, int code);

/**
 * @brief Draw all the connected players
 * 
 * @param renderer the renderer in use
 */
void players_mgr_draw_players(SDL_Renderer *renderer);

/**
 * @brief Remove a player using the code
 * 
 * @param code the code of the player to remove
 */
void players_mgr_remove_player(int code);