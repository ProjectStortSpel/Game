#ifndef NETTYPEMESSAGEID_H
#define NETTYPEMESSAGEID_H

enum NetTypeMessageId
{
	ID_CONNECTION_ACCEPTED,				// Connection to server accepted (will most likely be called after PASSWORD_ATTEMPT)
	ID_CONNECTION_LOST,					// Connection between server-client timed out
	ID_CONNECTION_DISCONNECTED,			// Client disconnected from server or 
	ID_CONNECTION_KICKED,				// Client kicked from server
	ID_CONNECTION_BANNED,				// Client banned from server
	ID_REMOTE_CONNECTION_KICKED,		// Remote connection kicked from server
	ID_REMOTE_CONNECTION_BANNED,		// Remote connection banned from server
	ID_PASSWORD_ATTEMPT,				// Client send password to server
	ID_PASSWORD_INVALID,				// Server return to client if ID_PASSWORD_ATTEMPT is unvalid
	ID_REMOTE_NEW_CONNECTION_ACCEPTED,	// New remote connection accepted to server
	ID_REMOTE_CONNECTION_LOST,			// Remote connection to server timed out
	ID_REMOTE_CONNECTION_DISCONNECTED,	// Remote connection disconencted
	ID_SERVER_FULL,						// Server full
	ID_PING,							// Send an ping message to connection
	ID_PONG,							// Send an response to ID_PING message

	ID_CUSTOM_PACKET					// Custom packet (game related)
};


#endif