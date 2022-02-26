#include "players_mgr.h"
#include <stdio.h>

static bomberman_t **players_mgr = NULL;

void players_mgr_init()
{
    players_mgr = calloc(1, sizeof(bomberman_t **));
    players_mgr[0] = NULL;
}

void players_mgr_free()
{
    if(players_mgr == NULL)
        return;
    bomberman_t *current;
    int index = 0;
    current = players_mgr[index];
    while (current != NULL)
    {
        bomberman_free(current);
        index++;
        current = players_mgr[index];
    }
    free(players_mgr);
    players_mgr = NULL;
}

int players_mgr_increase_size_by_one()
{
    int new_size = players_mgr_get_size() + 1;
    players_mgr = realloc(players_mgr, new_size);
    players_mgr[new_size - 1] = NULL;
    return new_size;
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
    if(players_mgr_contain_code(code))
        return 0;
    int size = players_mgr_increase_size_by_one();
    int index = size - 2;
    players_mgr[index] = bomberman_new(x, y, width, height, speed);
    bomberman_new_texture(players_mgr[index], texture);
    bomberman_update_code(players_mgr[index], code);

    return 1;
}

int players_mgr_get_size()
{
    return players_mgr ? sizeof(players_mgr) / sizeof(bomberman_t **) : 0;
}

bomberman_t *players_mgr_get_by_index(int index)
{
    if (index >= players_mgr_get_size() || index < 0)
        return NULL;
    return players_mgr[index] ? players_mgr[index] : NULL;
}

int players_mgr_update_player(float x, float y, int code)
{
    if(!players_mgr_contain_code(code))
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