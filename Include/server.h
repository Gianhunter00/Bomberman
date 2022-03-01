#pragma once
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdbool.h>

/** \struct server_packet_t
 * \brief packet sender and receiver format
 * @param auth code of the bomberman
 * @param x x of the bomberman
 * @param y y of the bomberman
 */
typedef struct server_packet
{
    int auth;
    float x;
    float y;
} server_packet_t;

/** \struct server_handshake_t
 * \brief packet sender format for the handshake
 * @param port the port the client is binded to read
 */
typedef struct server_handshake
{
    int port;
} server_handshake_t;

typedef struct player_data
{
    struct sockaddr_in *address;
    int auth;
} player_data_t;

int server_init();

void server_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char *addr, int port, bool to_bind);

int server_player_data_current_index();

void server_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y);

int server_player_data_next_index();

void server_player_data_add_new_player(player_data_t *new_player);

player_data_t *server_player_data_get_by_index(int index);

bool server_receive_packet(SOCKET s, server_packet_t *sendother);
