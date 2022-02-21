#include <SDL.h>
#include <stdio.h>
#include "..\..\Include\bomberman.h"
#include "..\..\Include\level.h"
#include "..\..\Include\parse_bmp.h"
#include "..\..\Include\client.h"

int main(int argc, char **argv)
{
    // level init
    level_t level;
    level_init(&level, 8, 8, 64, level001_cells);
    SDL_Rect cell_rect = {0, 0, level.cell_size, level.cell_size};

    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Bomberman",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          level.cols * level.cell_size,
                                          level.rows * level.cell_size,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // player creation
    SDL_Texture *player_texture = create_texture_from_BMP_file("bin\\Sprites\\BMP\\Bomberman\\Front\\Bman_F_f00.bmp", "rb", renderer);
    bomberman_t *player0 = bomberman_new(100, 100, 64, 128, 100);
    bomberman_new_texture(player0, player_texture);
    bomberman_t **players = calloc(10, sizeof(bomberman_t **));
    // load level texture
    level_level_texture_init();
    level_load_texture(renderer);

    // client init
    SOCKET socket = client_init();
    struct sockaddr_in sin_send;
    client_init_sockaddr(socket, &sin_send, "127.0.0.1", 9999, false);
    struct sockaddr_in sin_receive;
    client_init_sockaddr(socket, &sin_receive, "127.0.0.1", 9998, true);
    client_handshake(socket, sin_send, sin_receive);
    bomberman_update_code(player0, htons(sin_receive.sin_port));
    client_packet_t receive;

    // timer init
    client_timer_init(0.005f);
    // loop
    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            bomberman_input(player0, &event);
        }
        if (client_timer_elapsed())
        {
            client_send_packet(socket, sin_send, htons(sin_receive.sin_port), player0->movable.x, player0->movable.y);
        }

        if (client_receive_packet(socket, &receive))
        {
            bomberman_t *current = NULL;
            int index = 0;
            current = players[index];
            bool found = false;
            while (current != NULL)
            {
                if (current->code == receive.auth)
                {
                    current->movable.x = receive.x;
                    current->movable.y = receive.y;
                    current->rect_to_draw.x = receive.x;
                    current->rect_to_draw.y = receive.y;
                    found = true;
                }
                index += 1;
                current = players[index];
            }
            if (found == false)
            {
                bomberman_t *new_player = bomberman_new(receive.x, receive.y, 64, 128, 100);
                bomberman_new_texture(new_player, player_texture);
                bomberman_update_code(new_player, receive.auth);
                new_player->rect_to_draw.x = receive.x;
                new_player->rect_to_draw.y = receive.y;
                players[index] = new_player;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        level_draw_cell(&level, renderer);

        bomberman_move_on_level(player0, &level);
        if (players != NULL)
        {
            bomberman_t *current = NULL;
            int index = 0;
            current = players[index];
            bool found = false;
            while (current != NULL)
            {
                bomberman_draw(current, renderer);
                index += 1;
                current = players[index];
            }
        }
        bomberman_draw(player0, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_Quit();
    return 0;
}