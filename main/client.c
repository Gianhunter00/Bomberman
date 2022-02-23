#include "client.h"
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

void client_handshake(SOCKET s, struct sockaddr_in sin, struct sockaddr_in sinread)
{
    client_handshake_t packet;
    packet.port = htons(sinread.sin_port);
    char mss[sizeof(client_handshake_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(client_handshake_t));
    int sent_bytes = sendto(s, mss, sizeof(client_handshake_t), 0, (struct sockaddr *)&sin, sizeof(sin));
    printf("sent %d bytes via UDP\n", sent_bytes);
    //"127.0.0.1"
}

void client_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y)
{
    client_packet_t packet;
    packet.auth = auth;
    packet.x = x;
    packet.y = y;
    char mss[sizeof(client_packet_t) + sizeof(char)];
    memcpy(mss, &packet, sizeof(client_packet_t));
    int sent_bytes = sendto(s, mss, sizeof(client_packet_t), 0, (struct sockaddr *)&sin, sizeof(sin));
    //printf("sent %d bytes via UDP\n", sent_bytes);
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
        char addr_as_string[64];
        inet_ntop(AF_INET, &sender_in.sin_addr, addr_as_string, 64);
        //printf("received %d bytes from %s:%d\n", len, addr_as_string, ntohs(sender_in.sin_port));
        if (len == 4)
        {
            int bytes;
            memcpy(&bytes, buffer, 4);
            if (bytes == 1)
            {
                return false;
            }
        }
        client_packet_t receive;
        int auth;
        float x;
        float y;
        memcpy(&auth, buffer, sizeof(int));
        memcpy(&x, buffer + sizeof(int), sizeof(float));
        memcpy(&y, buffer + sizeof(int) + sizeof(float), sizeof(float));
        updateother->auth = auth;
        updateother->x = x;
        updateother->y = y;
        printf("auth: %d x: %f y: %f\n", auth, x, y);
        return true;
    }
    return false;
}