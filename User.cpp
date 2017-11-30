#include "User.h"

#include "socket.h"

User::User(const Sync::Socket &socket, unsigned id) : socket(socket), id(id)
{
}

unsigned User::getId() const
{
	return id;
}

Sync::Socket User::getSocket()
{
	return socket;
}
