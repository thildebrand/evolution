/*
 * client.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "../inc/client.h"
#include "../inc/func.h"

#define CLIENT_RECV_LIMIT 2048
#define CLIENT_RECV_HALF_LIMIT 1024

/**
 * Creates a new client for connections
 * @return a new allocated client structure
 */
Client new_client() {
	Client client;

	client = malloc( sizeof(struct client_struct) );

	if( client == NULL ){
		perror("Client malloc");
		return NULL;
	} else {
	
	    memset( client, 0, sizeof( struct client_struct ) );
	
		client->status = CLIENT_STARTING;

		return client;
	}
}

/**
 * Frees the client structure from mem allocation
 * @param client Client to free
 */
void destroy_client( Client client ) {
	free( client );
}


/**
 * Sets client status to ok
 * @param client Client to set ok
 */
void client_ok( Client client ) {
	client->status = CLIENT_OK;
}

/**
 * Closes the client connection
 * @param client Client to close
 */
void close_client( Client client) {
	if( client != NULL && client->status == CLIENT_OK ){
		client->status = CLIENT_STOPPED;
		close( client->socket );
	}
}


/**
 * Sends a message tot he client
 * @param client Client struct to send to
 * @param message Message to send
 * @return number of bytes transmitted or -1 on fail
 */
int client_send( Client client, char *message ) {


	if( client->status != CLIENT_OK ) return -1;

    int msg_len = strlen( message );
    int tx = 0;

    // ensure all data is sent
    do {
        
	    int n = write( 
	        client->socket, 
	        message + tx,     // position of remaining msg
	        msg_len - tx );   // length of message remaining

	    if( n < 0 ){
		    perror("Error sending to client");
		    return (tx == 0) ? -1 : tx;
	    }
	    
        // increase transmitted value
        tx += n;
    
    } while( tx < msg_len  );

    // return transmitted byte count
	return tx;
}

/**
 * Receives a message from the client
 * @param client Client struct to receive from
 * @param message Pointer to unallocated message buffer to receive into
 * @param len initial length of the message
 * @return number of bytes received  or -1 on fail
 */
int client_recv( Client client, char **message, int *length ) {

	if( client->status != CLIENT_OK ) return -1;

    int rx = 0;
    
    while ( 1 ){
    
        int n = read( 
            client->socket, 
            *message + rx, 
            *length  - rx );
        
	    if( errno != EWOULDBLOCK && errno != EAGAIN && n < 0 ){
		    perror("Error recv from client");
		    rx = (rx == 0) ? -1 : rx;
		    break;
	    } else if( n < 0 ){
	        break;
	    }
	    
	    rx += n;
	    
	    // burn the rest
	    if( rx == CLIENT_RECV_LIMIT ) {
	        int m = 0;
	        char burn[128];
	        while( m >= 0 ){
                m = read( client->socket, burn, 128 );
	        }
	        break;
	    }
	    
	    // full buffer, grow
	    if( rx == *length ){
	    
	        int grow_length = grow_buffer( (void **)message, *length+256 );
	        if( grow_length < 0 ){
                perror("Error realloc client recv");
	            break;
	        }
	        
	        *length = grow_length;
	    }
	    
	    
    }
	
	// currently overwrites last character if recv
	// limit is reached. I judge this acceptable for now
	*message[rx] == '\0';
	
	return rx;
}


/**
 * Prompts user for a response
 * @param client Client to prompt
 * @param prompt Prompt to give to user for input
 * @param response Response buffer to fill
 * @param length Initial length of response buffer
 * @return status of the receive or -1 on failure
 */
int client_prompt( 
    Client client, 
    char *prompt, 
    char **response, 
    int *length
){
    int status = client_send( client, prompt );
    if( status < 0 ) return -1;
    
    return client_recv( client, response, length );
}

