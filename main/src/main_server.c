#include "server.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    SOCKET socket = server_init();
    struct sockaddr_in sin_receive;
    server_init_sockaddr(socket, &sin_receive, "127.0.0.1", 9999, true);
    struct sockaddr_in sender;
    server_packet_t receive;
    player_data_t *current = NULL;
    int index = 0;
    while (true)
    {
        if (server_receive_packet(socket, &receive))
        {
            current = server_player_data_get_by_index(index);
            while (current != NULL)
            {
                if (current->auth != receive.auth)
                {
                    server_send_packet(socket, *current->address, receive.auth, receive.x, receive.y);
                }
                index++;
                current = server_player_data_get_by_index(index);
            }
            current = NULL;
            index = 0;
        }
    }
}