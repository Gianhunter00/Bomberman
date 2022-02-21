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

extern time_t timer;
extern float trigger_timer;

typedef struct client_packet
{
    int auth;
    float x;
    float y;
} client_packet_t;


typedef struct client_handshake
{
    int port;
}client_handshake_t;

void client_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char* addr, int port, bool to_bind);

int client_init();

void client_timer_init(float trigger_seconds);

bool client_timer_elapsed();

void client_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y);

bool client_receive_packet(SOCKET s, client_packet_t *updateother);

void client_handshake(SOCKET s, struct sockaddr_in sin, struct sockaddr_in sinread);