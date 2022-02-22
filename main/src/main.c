#include <SDL.h>
#include <stdio.h>
#include "bomberman.h"
#include "level.h"
#include "parse_bmp.h"
#include "client.h"
#include "players_mgr.h"

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
    SDL_Texture *player_texture = create_texture_from_BMP_file("bin\\resources\\Sprites\\BMP\\Bomberman\\Front\\Bman_F_f00.bmp", "rb", renderer);
    bomberman_t *player0 = bomberman_new(100, 100, 64, 128, 100);
    bomberman_new_texture(player0, player_texture);
    players_mgr_init();
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
    client_timer_init(.0167f);

    // loop
    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
            // player input
            bomberman_input(player0, &event);
        }
        // send timer
        if (client_timer_elapsed())
        {
            // send packet
            client_send_packet(socket, sin_send, htons(sin_receive.sin_port), player0->movable.x, player0->movable.y);
        }

        // read from server
        if (client_receive_packet(socket, &receive))
        {
            // update player and add if new
            if (players_mgr_contain_code(receive.auth))
                players_mgr_update_player(receive.x, receive.y, receive.auth);
            else
                players_mgr_add_new_player(receive.x, receive.y, 64, 128, 100, player_texture, receive.auth);
        }
        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        // update player
        bomberman_move_on_level(player0, &level);

        // draw
        level_draw_cell(&level, renderer);
        players_mgr_draw_players(renderer);
        bomberman_draw(player0, renderer);

        // show
        SDL_RenderPresent(renderer);
    }

    bomberman_free(player0);
    SDL_Quit();
    return 0;
}