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

extern time_t timer;        /* timer in use */
extern float trigger_timer; /* timer interval */

/** \struct client_packet_t
 * \brief packet sender and receiver format
 * @param auth code of the bomberman
 * @param x x of the bomberman
 * @param y y of the bomberman
 */
typedef struct client_packet
{
    int auth;   
    float x;    
    float y;    
} client_packet_t;

/** \struct client_handshake_t
 * \brief packet sender format for the handshake
 * @param port the port you are binded for reading
 */
typedef struct client_handshake
{
    int port;
}client_handshake_t;

/** Init of the sockaddr for sending or reading.
 * If to_bind is true and the port is already in use it will try to bind to the port given less one until it finds a free port to bind
 *
 * @param s the socket in use
 * @param sin the sockaddr_in to initialize
 * @param addr the address to use
 * @param port the port to use
 * @param to_bind true if you want to read with this sin false if the sin is for sending
 */
void client_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char* addr, int port, bool to_bind);

/** Init winsock dll if on windows
 * Then create the socket
 *
 * @result the socket created or -1 if error occured
 */
int client_init();

/** Init the timer
 *
 * @param trigger_seconds interval for the timer
 */
void client_timer_init(float trigger_seconds);

/** Check if the timer elapsed surpassing the interval given
 *
 * @result true if it has passed more time than the interval given otherwise false
 */
bool client_timer_elapsed();

/** Send packet to the given sockaddr_in
 *
 * @param s the socket in use
 * @param sin the sockaddr_in to send the message
 * @param auth the code for the current bomberman
 * @param x the current x position of the bomberman
 * @param y the current y position of the bomberman
 */
void client_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y);

/** Reader for server message with other players information
 *
 * @param s the socket in use
 * @param updateother the packet to fill with the information from the server message
 *
 * @result true if the server message is received and is correct, if there is no message to read return false
 */
bool client_receive_packet(SOCKET s, client_packet_t *updateother);

/** Handshake to the server to inform it on which port to send the message for this client
 *
 * @param s the socket in use
 * @param sin the sockaddr_in to send the message
 * @param sinread the sockaddr_in with the information of the current bind of the client for reading
 */
void client_handshake(SOCKET s, struct sockaddr_in sin, struct sockaddr_in sinread);