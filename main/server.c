#include "server.h"
#include <stdio.h>

#define MAX_PLAYERS 4
static player_data_t *players_connected[MAX_PLAYERS];
static int players_connected_index = -1;

int server_init()
{
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data;
    // memset(&wsa_data, 0, sizeof(wsa_data));
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
    {
        printf("unable to initialize winsock2\n");
        return -1;
    }
#endif
    int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s < 0)
    {
        printf("unable to initialize the UDP socket\n");
        return -1;
    }
    u_long imode;
    ioctlsocket(s, FIONBIO, &imode);
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        players_connected[i] = NULL;
    }
    return s;
}

void server_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char *addr, int port, bool to_bind)
{
    inet_pton(AF_INET, addr, &sin->sin_addr); // this will create a big entian 32 bit address
    sin->sin_family = AF_INET;
    sin->sin_port = htons(port); // converts port to big endian
    if (to_bind)
    {
        while (bind(s, (struct sockaddr *)sin, sizeof(*sin)) != 0)
        {
            port -= 1;
            sin->sin_port = htons(port);
        }
    }
}

int server_player_data_current_index()
{
    return players_connected_index;
    //return players_connected ? sizeof(players_connected) / sizeof(players_connected[0]) : 0;
}

void server_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y)
{
    server_packet_t packet;
    packet.auth = auth;
    packet.x = x;
    packet.y = y;
    char mss[sizeof(server_packet_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(server_packet_t));
    int sent_bytes = sendto(s, mss, sizeof(server_packet_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

int server_player_data_next_index()
{
    if(players_connected_index >= MAX_PLAYERS)
        return -1;
    players_connected_index += 1;
    return players_connected_index;
}

void server_player_data_add_new_player(player_data_t *new_player)
{
    int index = server_player_data_next_index();
    players_connected[index] = new_player;
}

player_data_t *server_player_data_get_by_index(int index)
{
    if (index > server_player_data_current_index() || index < 0)
        return NULL;
    return players_connected[index] ? players_connected[index] : NULL;
}

bool server_receive_packet(SOCKET s, server_packet_t *sendother)
{
    char buffer[4096];
    struct sockaddr_in sender_in;
    int sender_in_size = sizeof(sender_in);
    int len = recvfrom(s, buffer, 4095, 0, (struct sockaddr *)&sender_in, &sender_in_size);
    if (len > 0)
    {
        buffer[len] = 0;
        char addr_as_string[64];
        inet_ntop(AF_INET, &sender_in.sin_addr, addr_as_string, 64);
        if (len == 4)
        {
            printf("new player\n");
            int bytes;
            memcpy(&bytes, buffer, 4);
            sender_in.sin_family = AF_INET;
            sender_in.sin_port = htons(bytes);
            struct sockaddr_in *sender = malloc(sizeof(struct sockaddr_in));
            memcpy(sender, &sender_in, sizeof(struct sockaddr_in));
            player_data_t *player_new = malloc(sizeof(player_data_t));
            player_new->address = sender;
            player_new->auth = bytes;
            server_player_data_add_new_player(player_new);
            return false;
        }
        server_packet_t receive;
        int auth;
        float x;
        float y;
        memcpy(&auth, buffer, sizeof(int));
        memcpy(&x, buffer + sizeof(int), sizeof(float));
        memcpy(&y, buffer + sizeof(int) + sizeof(float), sizeof(float));
        sendother->auth = auth;
        sendother->x = x;
        sendother->y = y;
        return true;
    }
    return false;
}