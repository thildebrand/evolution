/*
 * action.h
 *
 *  Created on: Oct 9, 2015
 *      Author: thor
 */

#ifndef INC_ACTION_H_
#define INC_ACTION_H_

#include "user.h"
#include "management.h"
#include "hash.h"

// action structure
typedef struct action_struct {
	void *(*action_func)( const char *, User, Management );
	char links; // number of links in this function, used for freeing
} * Action;


// add actions to the manager
void action_add_to_manager( Management );

// clean up  actions from the manager
void action_clean( HashTable );

// destroy the action
void destroy_action( void * );

// parse the response
int action_parse_response(
		char *response,
		int length,
		User user,
		Management manager );

// create a new action
Action new_action( void *(*action_func)( const char *, User, Management ) );

// create a new action with extra links
Action new_action_linked( void *(*action_func)( const char *, User, Management ), char );

// exit action
void *action_quit( const char *, User, Management);

// look action
void *action_look( const char *, User, Management);

// shout action
void *action_shout( const char *, User, Management);

/* ------------------------------------------------
 * Actions to move around
 */

// go to a room - master action for movement
void *action_go_to_room( Room, User, Management );

// go north
void *action_north( const char *, User, Management);
void *action_south( const char *, User, Management);
void *action_east( const char *, User, Management);
void *action_west( const char *, User, Management);



#endif /* INC_ACTION_H_ */
