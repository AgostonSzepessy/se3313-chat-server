#pragma once

#include "socket.h"

class User
{
	public:
		User(const Sync::Socket socket);
		Sync::Socket getSocket();
		unsigned getId() const;
	private:
		Sync::Socket socket;
		unsigned id;
};
