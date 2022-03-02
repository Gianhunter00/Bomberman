#include "players_mgr.h"
#include <stdio.h>


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
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_mgr[i];
        if (current == NULL)
        {
            break;
        }
        bomberman_free(current);
    }
}

int players_mgr_get_current_index()
{
    return players_mgr_index;
    // return players_mgr ? sizeof(players_mgr) / sizeof(players_mgr[0]) : 0;
}

int players_mgr_next_index()
{
    if (players_mgr_index >= (MAX_PLAYERS - 1))
        return -1;
    players_mgr_index += 1;
    return players_mgr_index;
}

bool players_mgr_contain_code(int code)
{
    bomberman_t *current;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_mgr_get_by_index(i);
        if (current == NULL)
        {
            break;
        }
        if (current->code == code)
        {
            return true;
        }
    }
    return false;
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
    bomberman_t *current;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_mgr_get_by_index(i);
        if (current == NULL)
            break;
        if (current->code == code)
        {
            current->movable.x = x;
            current->movable.y = y;
            current->rect_to_draw.x = x;
            current->rect_to_draw.y = y;
        }
    }
    return 1;
}

void players_mgr_draw_players(SDL_Renderer *renderer)
{
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        if (players_mgr[i] == NULL)
            break;
        bomberman_draw(players_mgr[i], renderer);
    }
}

void players_mgr_remove_player(int code)
{
    players_mgr_index -= 1;
        bomberman_t *current;
    int index_removed = -1;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_mgr[i];
        if (current == NULL)
        {
            break;
        }
        if (index_removed != -1)
        {
            players_mgr[index_removed] = players_mgr[i];
            players_mgr[i] = NULL;
            index_removed++;
            continue;
        }
        if (current->code == code)
        {
            bomberman_free(current);
            current = NULL;
            players_mgr[i] = NULL;
            index_removed = i;
        }
    }
}