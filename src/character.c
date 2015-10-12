/*
 * character.c
 *
 *  Created on: Oct 3, 2015
 *      Author: thor
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#include "../inc/character.h"
#include "../inc/client.h"

/**
 * Create a new character struct
 * @return the new character struct pointer
 */
Character new_character( ) {

	Character character = (Character)malloc(
			sizeof( struct character_struct )
		);

    // check
	if( character == NULL ){
	    perror("malloc character");
		return NULL;
	}

    // init
    memset( character, 0, sizeof( struct character_struct ) );

	// set default user to none
	character->user = NULL;

	strcpy(character->name, "Newman");
	character->location = 0;

	// mutex
	pthread_mutex_t mutex;
	pthread_mutex_init( &mutex, NULL );
	character->mutex = mutex;

	return character;
}

/**
 * Destroys the character struct
 * @param character Character to free
 */
void destroy_character( Character character ) {
	pthread_mutex_destroy( &(character->mutex) );
	free( character );
}

/**
 * Locks the character struct for threaded use
 * @param character Character to lock
 */
void char_lock( Character character ) {
	pthread_mutex_lock( &(character->mutex) );
}

/**
 * Unlocks teh character for threaded use
 * @param character Character struct to unlock
 */
void char_unlock( Character character ) {
	pthread_mutex_unlock( &(character->mutex) );
}