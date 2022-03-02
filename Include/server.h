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

/** 
 * @struct server_packet_t
 * @brief Packet sender and receiver format
 * 
 * @param code code of the packet you are sending Data is 1
 * @param auth code of the bomberman
 * @param x x of the bomberman
 * @param y y of the bomberman
 */
typedef struct server_packet
{
    int code;
    int auth;
    float x;
    float y;
} server_packet_t;

/** 
 * @struct server_handshake_t
 * @brief Packet sender format for the handshake
 * 
 * @param code code of the packet you are sending Handshake is 0
 * @param port the port the client is binded to read
 */
typedef struct server_handshake
{
    int code;
    bool can_connect;
} server_handshake_t;

/**
 * @struct server_dead_client_t
 * @brief Packet sender format for dead client
 * 
 * @param code code of the packet you are sending Dead is 2
 * @param auth auth identifier of the dead client
 */
typedef struct server_dead_client
{
    int code;
    int auth;
} server_dead_client_t;

/**
 * @struct player_data_t
 * @brief Format of the player data stored
 * 
 * @param address pointer to the sender for the player
 * @param auth the auth of the player
 */
typedef struct player_data
{
    struct sockaddr_in *address;
    int auth;
} player_data_t;

/**
 * @brief Initialize the server
 * 
 * @return the socket if no error occured otherwise -1
 */
int server_init();

/**
 * @brief Initialize a sockaddr_in
 * 
 * @param s the socket in use
 * @param sin the sockaddr_in to initialize
 * @param addr the address to use
 * @param port the port to use
 * @param to_bind true if you want to read with this sin false if the sin is for sending
 */
void server_init_sockaddr(SOCKET s, struct sockaddr_in *sin, char *addr, int port, bool to_bind);

/**
 * @brief Send packet to the given sockaddr_in
 * 
 * @param s the socket in use
 * @param sin the sockaddr_in to send the message
 * @param auth the code for the current bomberman
 * @param x the current x position of the bomberman
 * @param y the current y position of the bomberman
 */
void server_send_packet(SOCKET s, struct sockaddr_in sin, int auth, float x, float y);

/**
 * @brief Handshake to the client to accept or reject connection based on how many players are connected
 *
 * @param s the socket in use
 * @param sin the sockaddr_in to send the message
 */
void server_handshake(SOCKET s, struct sockaddr_in sin);

/**
 * @brief Send a dead message of a client
 * 
 * @param s the socket in use
 * @param sin the sockaddr_in to send the message
 * @param auth the auth of the dead client
 */
void server_dead_client(SOCKET s, struct sockaddr_in sin, int auth);

/**
 * @brief Inform all other clients that a client is dead
 * 
 * @param s the socket in use
 * @param auth the auth of the dead client
 */
void server_send_dead_client_notification(SOCKET s, int auth);

/**
 * @brief Get the current index
 * 
 * @return The index of the last player connected
 */
int server_player_data_current_index();

/**
 * @brief Get the next index
 * 
 * @return the next index of the last player connected or -1 if MAX_PLAYERS reached 
 */
int server_player_data_next_index();

/**
 * @brief Check if any players has the given auth
 * 
 * @param auth the auth to check
 * @return true if found false otherwise
 */
bool server_contain_player(int auth);

/**
 * @brief Get the player data using the index
 * 
 * @param index the index of the player you want
 * 
 * @return the player data at the given index or NULL if index is invalid
 */
player_data_t *server_player_data_get_by_index(int index);

/**
 * @brief Add a new player
 * 
 * @param new_player the player to add
 */
void server_player_data_add_new_player(player_data_t *new_player);

/**
 * @brief Remove a player using the auth
 * 
 * @param auth the auth of the player to remove
 */
void server_remove_dead_client(int auth);

/**
 * @brief Reader for client message populate sendother when a player update its position
 * 
 * @param s the socket in use
 * @param sendother the packet of the new position of a player
 * 
 * @return true if a player updated its position false otherwise
 */
bool server_receive_packet(SOCKET s, server_packet_t *sendother);