#pragma once
#include "bomberman.h"

void players_mgr_init();

void players_mgr_free();

int players_mgr_next_index();

int players_mgr_get_current_index();

bomberman_t *players_mgr_get_by_index(int index);

bool players_mgr_contain_code(int code);

int players_mgr_add_new_player(float x, float y, float width, float height, float speed, SDL_Texture *texture, int code);

int players_mgr_update_player(float x, float y, int code);

void players_mgr_draw_players(SDL_Renderer *renderer);