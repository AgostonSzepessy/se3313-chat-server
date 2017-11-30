#include "User.h"

#include "socket.h"

User::User(const Sync::Socket &socket) : socket(socket)
{
	static unsigned u_id = 0;

	this->id = u_id;
	u_id++;
}

unsigned User::getId() const
{
	return id;
}

Sync::Socket User::getSocket()
{
	return socket;
}
