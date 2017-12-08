#include <iostream>
#include <deque>
#include "Blockable.h"
#include "socketserver.h"
#include "User.h"
#include <thread>
#include <vector>
#include "socket.h"

using namespace Sync;

const int PORT = 2004;

void waitForNewUsers(SocketServer &, bool &);

void listenForUserInput(User, User);

// This application is a chat application where the users are matched randomly
// with other users. When a user connects, 1 of 2 things happens. If there are
// any users who are connected but don't have a partner, they are matched up with
// a partner. Otherwise, if there are no users available, they are put into a queue.
int main(int argc, char **argv)
{
	try
	{
		SocketServer server(PORT);
		bool quit = true;

		// Move socket server to another thread because it has blocking calls,
		// so the only way to kill it is if we shut it down from another thread
		std::thread waitForUsers(waitForNewUsers, std::ref(server), std::ref(quit));

		std::string in;
		std::cout<<"Type quit to stop server...\n";
		while(quit)
		{
			std::cin>>in;
			quit = !(in=="quit");
		}
		std::cout<<"Shutting down"<<std::endl;
		server.Shutdown();
		waitForUsers.join();
	}
	catch(const std::string &exception)
	{
		std::cout<<exception<<std::endl;
		return 1;
	}



	return 0;
}

// Waits for new users to join and either puts them in a waiting queue if there
// aren't any users to match them up with, or matches them up with a user that
// is in the queue.
void waitForNewUsers(SocketServer &server, bool &quit)
{
		std::vector<std::thread> threads;
		std::deque<User> users;
		ThreadSem userSem(1);

		while(quit)
		{
			try {
				// Wait for new connection
				Socket sock = server.Accept();

				User u1(sock);
				userSem.Wait();

				if(users.size()>0)
				{
					User u2 = users.front();
					users.pop_front();

					// Send a 1 to signal that they're ready to start talking to each other
					u1.getSocket().Write(ByteArray("1"));
					u2.getSocket().Write(ByteArray("1"));

					std::thread u1input(listenForUserInput, u1, u2);
					std::thread u2input(listenForUserInput, u2, u1);

					threads.push_back(std::move(u1input));
					threads.push_back(std::move(u2input));
					//start new chat thread with users
				} else
				{
					// Send a 0 to signal that the user needs to wait for someone else can
					// connect before they can start sending messages
					u1.getSocket().Write(ByteArray("a"));
					users.push_back(u1);
				}

				userSem.Signal();

			}
			catch(int exception)
			{
				// Shut down signal
				if(exception==2) {
					std::cout << "Shutting down with execption code 2" << std::endl;
				}
				break;
			}
			catch(const std::string &exception)
			{
				// Some sort of other error occured
				std::cout << "EXCEPTION:" << exception << std::endl;
			}
		}
		// Kill all threads
		for(int i = 0; i < threads.size(); i++)
		{
			threads[i].join();
		}
}

// Server waits for an input from `user`, and when it receives it, it sends
// it to `other`.
void listenForUserInput(User user, User other)
{
	//wait for an input
	while(true)
	{
		ByteArray input;

		if(user.getSocket().Read(input) == 0)
		{
			//user's socket closed
			std::cout<<"User disconnected"<<std::endl;
			other.getSocket().Write(ByteArray("quit"));
			other.getSocket().Close();
			break;
		}

		if(other.getSocket().Write(ByteArray("||"+input.ToString())) == 0)
		{
			//other's socket closed
			std::cout<<"Other disconnected"<<std::endl;
			user.getSocket().Write(ByteArray("quit"));
			user.getSocket().Close();
			break;
		}

	}
}
