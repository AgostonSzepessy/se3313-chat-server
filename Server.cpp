#include <iostream>
#include <deque>
#include "Blockable.h"
#include "socketserver.h"
#include "User.h"
#include <thread>
#include <vector>
#include "socket.h"

using namespace Sync;

const int PORT = 2000;

void waitForNewUsers(SocketServer &, bool &);

void chatRoom(User, User);

void listenForUserInput(User, User);

int main(int argc, char **argv)
{
	try{
		SocketServer server(PORT);
		bool quit = true;

		std::thread waitForUsers(waitForNewUsers, std::ref(server), std::ref(quit));

		std::string in;
		std::cout<<"Type quit to stop server...\n";
		while(quit)
		{
			std::cin>>in;
			quit = in=="quit";
		}

		server.Shutdown();
		waitForUsers.join();
	}catch(const std::string &exception){
		std::cout<<exception<<std::endl;
		return 1;
	}



	return 0;
}

void waitForNewUsers(SocketServer &server, bool &quit)
{
		std::vector<std::thread> threads;
		std::deque<User> users;
		ThreadSem userSem(1);

		while(quit){
			try{
				Socket sock = server.Accept();

				User u1(sock);
				userSem.Wait();
				if(users.size()>0){
					User u2 = users.front();
					users.pop_front();
					//start new chat thread with users
				}else{
					users.push_back(u1);
				}

				userSem.Signal();

			}catch(int exception){
				if(exception==2){
					std::cout<<exception<<std::endl;
				}
				break;
			}catch(const std::string &exception){
				std::cout<<exception<<std::endl;
			}
		}

		for(int i = 0; i < threads.size(); i++){
			threads[i].join();
		}
}

void chatRoom(User u1, User u2){
	//spawn 2 new threads
	std::thread u1input(listenForUserInput, u1, u2);
	std::thread u2input(listenForUserInput, u2, u1);


}

void listenForUserInput(User user, User other){
	//wait for an input
	ByteArray input;
	user.getSocket().Read(input);

	other.getSocket().Write(input);

	//give input to other user in the chatroom
}
