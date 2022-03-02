#include "client.h"
#include "players_mgr.h"
#include <stdio.h>
#include <time.h>

time_t timer;
float trigger_timer;

void client_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char *addr, int port, bool to_bind)
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
            // printf("unable to bind the UDP socket: %d \n", WSAGetLastError());
        }
    }
}

int client_init()
{
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data;
    // memset(&wsa_data, 0, sizeof(wsa_data));
    if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0)
    {
        printf("unable to initialize 1winsock2\n");
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
    return s;
}

void client_timer_init(float trigger_seconds)
{
    timer = clock();
    trigger_timer = trigger_seconds;
}

bool client_timer_elapsed()
{
    if (((double)(clock() - timer)) / CLOCKS_PER_SEC >= trigger_timer)
    {
        timer = clock();
        return true;
    }
    return false;
}

void client_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y)
{
    client_packet_t packet;
    packet.code = 1;
    packet.auth = auth;
    packet.x = x;
    packet.y = y;
    char mss[sizeof(client_packet_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(client_packet_t));
    int sent_bytes = sendto(s, mss, sizeof(client_packet_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

void client_handshake(SOCKET s, struct sockaddr_in sin, struct sockaddr_in sinread)
{
    client_handshake_t packet;
    packet.code = 0;
    packet.port = htons(sinread.sin_port);
    char mss[sizeof(client_handshake_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(client_handshake_t));
    int sent_bytes = sendto(s, mss, sizeof(client_handshake_t), 0, (struct sockaddr *)&sin, sizeof(sin));
    //"127.0.0.1"
}

void client_close_connection(SOCKET s, struct sockaddr_in sin, struct sockaddr_in sinread)
{
    client_close_t packet;
    packet.code = 2;
    packet.auth = htons(sinread.sin_port);
    char mss[sizeof(client_close_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(client_close_t));
    int sent_bytes = sendto(s, mss, sizeof(client_close_t), 0, (struct sockaddr *)&sin, sizeof(sin));
}

void client_remove_dead_client(int auth)
{
    players_mgr_remove_player(auth);
}

bool client_receive_packet(SOCKET s, client_packet_t *updateother)
{
    char buffer[4096];
    struct sockaddr_in sender_in;
    int sender_in_size = sizeof(sender_in);
    int len = recvfrom(s, buffer, 4095, 0, (struct sockaddr *)&sender_in, &sender_in_size);
    if (len > 0)
    {
        buffer[len] = 0;
        int code;
        memcpy(&code, buffer, sizeof(int));
        if (code == 0 && len == 8)
        {
            bool can_connect;
            memcpy(&can_connect, buffer + sizeof(int), sizeof(bool));
            if(!can_connect)
            {
                printf("to many players connected at the same time, try later\n");
                return false;
            }
            return false;
        }
        if (code == 1 && len == 16)
        {
            client_packet_t receive;
            int auth;
            float x;
            float y;
            memcpy(&auth, buffer + sizeof(int), sizeof(int));
            memcpy(&x, buffer + sizeof(int) + sizeof(int), sizeof(float));
            memcpy(&y, buffer + sizeof(int) + sizeof(int) + sizeof(float), sizeof(float));
            updateother->auth = auth;
            updateother->x = x;
            updateother->y = y;
            return true;
        }
        if(code == 2 && len == 8)
        {
            int auth;
            memcpy(&auth, buffer + sizeof(int), sizeof(int));
            client_remove_dead_client(auth);
        }
    }
    return false;
}