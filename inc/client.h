/*
 * client.h
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#ifndef INC_CLIENT_H_
#define INC_CLIENT_H_

#include <netinet/in.h>
#include <pthread.h>

/* ------------------------------------------
 * Enums
 */
typedef enum client_status {
	CLIENT_STARTING,
	CLIENT_OK,
	CLIENT_STOPPED,
	CLIENT_UNK
} ClientStatus;

/* -------------------------------------------
 * Structs
 */

/** Client structure for accepting new users */
typedef struct client_struct {

	/** socket for client */
	int socket;

	/** address for client socket */
	struct sockaddr_in address;

	/** client thread id */
	pthread_t thread;

	ClientStatus status;

} * Client;

/* --------------------------------------------
 * Methods
 */

// creates a client
Client new_client();

// destorys a client
void destroy_client( Client client);

// set client status to ready
void client_ok( Client client );

// closes the client
void close_client( Client client);

// send message to client
int client_send( Client client, char *message );

// receive message from client
int client_recv( Client client, char **message );

#endif /* INC_CLIENT_H_ */
