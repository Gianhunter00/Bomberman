#include "players_mgr.h"
#include <stdio.h>

#define MAX_PLAYERS 4
static bomberman_t *players_mgr[MAX_PLAYERS];
static int players_mgr_index = -1;

void players_mgr_init()
{
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        players_mgr[i] = NULL;
    }
}

void players_mgr_free()
{
    bomberman_t *current;
    int index = 0;
    current = players_mgr[index];
    while (current != NULL && index < MAX_PLAYERS)
    {
        bomberman_free(current);
        index++;
        current = players_mgr[index];
    }
    //free(players_mgr);
    //players_mgr = NULL;
}

int players_mgr_get_current_index()
{
    return players_mgr_index;
    // return players_mgr ? sizeof(players_mgr) / sizeof(players_mgr[0]) : 0;
}

int players_mgr_next_index()
{
    if(players_mgr_index >= MAX_PLAYERS)
        return -1;
    players_mgr_index += 1;
    return players_mgr_index;
}

bool players_mgr_contain_code(int code)
{
    int index = 0;
    bomberman_t *current = players_mgr_get_by_index(index);
    bool found = false;
    while (current != NULL)
    {
        if (current->code == code)
        {
            found = true;
        }
        index++;
        current = players_mgr_get_by_index(index);
    }
    return found;
}

int players_mgr_add_new_player(float x, float y, float width, float height, float speed, SDL_Texture *texture, int code)
{
    if (players_mgr_contain_code(code))
        return 0;
    int index = players_mgr_next_index();
    players_mgr[index] = bomberman_new(x, y, width, height, speed);
    bomberman_new_texture(players_mgr[index], texture);
    bomberman_update_code(players_mgr[index], code);
    return 1;
}

bomberman_t *players_mgr_get_by_index(int index)
{
    if (index > players_mgr_get_current_index() || index < 0)
        return NULL;
    return players_mgr[index] ? players_mgr[index] : NULL;
}

int players_mgr_update_player(float x, float y, int code)
{
    if (!players_mgr_contain_code(code))
        return 0;
    int index = 0;
    bomberman_t *current = players_mgr_get_by_index(index);
    while (current != NULL)
    {
        if (current->code == code)
        {
            current->movable.x = x;
            current->movable.y = y;
            current->rect_to_draw.x = x;
            current->rect_to_draw.y = y;
        }
        index++;
        current = players_mgr_get_by_index(index);
    }
    return 1;
}

void players_mgr_draw_players(SDL_Renderer *renderer)
{
    int index = 0;
    while (players_mgr[index] != NULL)
    {
        bomberman_draw(players_mgr[index], renderer);
        index++;
    }
}