#include "server.h"
#include <stdio.h>

#define MAX_PLAYERS 3

static player_data_t *players_connected[MAX_PLAYERS];
static int players_connected_index = -1;
static bool max_players_reached = false;

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
    if (ioctlsocket(s, FIONBIO, &imode) != 0)
    {
        printf("non blocking error");
    }

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

void server_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y)
{
    server_packet_t packet;
    packet.code = 1;
    packet.auth = auth;
    packet.x = x;
    packet.y = y;
    char mss[sizeof(server_packet_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(server_packet_t));
    int sent_bytes = sendto(s, mss, sizeof(server_packet_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

void server_handshake(SOCKET s, struct sockaddr_in sin)
{
    server_handshake_t packet;
    packet.code = 0;
    packet.can_connect = server_player_data_current_index() < (MAX_PLAYERS - 1);
    max_players_reached = !packet.can_connect;
    char mss[sizeof(server_handshake_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(server_handshake_t));
    int sent_bytes = sendto(s, mss, sizeof(server_handshake_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

void server_dead_client(SOCKET s, struct sockaddr_in sin, int auth)
{
    server_dead_client_t packet;
    packet.code = 2;
    packet.auth = auth;
    char mss[sizeof(server_dead_client_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(server_dead_client_t));
    int sent_bytes = sendto(s, mss, sizeof(server_dead_client_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

void server_send_dead_client_notification(SOCKET s, int auth)
{
    player_data_t *current;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_connected[i];
        if (current == NULL)
        {
            break;
        }
        server_dead_client(s, *current->address, auth);
    }
}

int server_player_data_current_index()
{
    return players_connected_index;
}

int server_player_data_next_index()
{
    if (players_connected_index >= (MAX_PLAYERS - 1))
        return -1;
    players_connected_index += 1;
    return players_connected_index;
}

bool server_contain_player(int auth)
{
    player_data_t *current;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_connected[i];
        if (current == NULL)
        {
            break;
        }
        if (current->auth == auth)
        {
            return true;
        }
    }
    return false;
}

player_data_t *server_player_data_get_by_index(int index)
{
    if (index > server_player_data_current_index() || index < 0)
        return NULL;
    return players_connected[index] ? players_connected[index] : NULL;
}

void server_player_data_add_new_player(player_data_t *new_player)
{
    int index = server_player_data_next_index();
    players_connected[index] = new_player;
}

void server_remove_dead_client(int auth)
{
    player_data_t *current;
    int index_removed = -1;
    for (size_t i = 0; i < MAX_PLAYERS; i++)
    {
        current = players_connected[i];
        if (current == NULL)
        {
            break;
        }
        if (index_removed != -1)
        {
            players_connected[index_removed] = players_connected[i];
            players_connected[i] = NULL;
            index_removed++;
            continue;
        }
        if (current->auth == auth)
        {
            free(current->address);
            free(current);
            current = NULL;
            players_connected[i] = NULL;
            index_removed = i;
        }
    }
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
        int code;
        memcpy(&code, buffer, sizeof(int));
        if (code == 0 && len == 8)
        {
            int port;
            memcpy(&port, buffer + sizeof(int), sizeof(int));
            sender_in.sin_family = AF_INET;
            sender_in.sin_port = htons(port);
            server_handshake(s, sender_in);
            if (max_players_reached)
            {
                return false;
            }
            struct sockaddr_in *sender = malloc(sizeof(struct sockaddr_in));
            memcpy(sender, &sender_in, sizeof(struct sockaddr_in));
            player_data_t *player_new = malloc(sizeof(player_data_t));
            player_new->address = sender;
            player_new->auth = port;
            server_player_data_add_new_player(player_new);
            return false;
        }
        if (code == 1 && len == 16)
        {
            server_packet_t receive;
            int auth;
            float x;
            float y;
            memcpy(&auth, buffer + sizeof(int), sizeof(int));
            if (!server_contain_player(auth))
            {
                return false;
            }
            memcpy(&x, buffer + sizeof(int) + sizeof(int), sizeof(float));
            memcpy(&y, buffer + sizeof(int) + sizeof(int) + sizeof(float), sizeof(float));
            sendother->auth = auth;
            sendother->x = x;
            sendother->y = y;
            return true;
        }
        if (code == 2 && len == 8)
        {
            int auth;
            memcpy(&auth, buffer + sizeof(int), sizeof(int));
            if (server_contain_player(auth))
            {
                server_remove_dead_client(auth);
                server_send_dead_client_notification(s, auth);
                players_connected_index -= 1;
                max_players_reached = false;
            }
        }
    }
    return false;
}