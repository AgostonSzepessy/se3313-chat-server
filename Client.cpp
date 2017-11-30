#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <thread>

using namespace Sync;

const unsigned PORT = 2000;

void readMessage(Socket &);

int main(void)
{
		// Connect to localhost on port 2000
		Socket socket("127.0.0.1", PORT);

		// Only start client if the server has been started
		try
		{
			socket.Open();
		}
		catch(const std::string &)
		{
			std::cout << "Start server before starting client" << std::endl;
			return 1;
		}

		ByteArray response;
		socket.Read(response);//response code 0 means that you are placed in queue on server

		auto responseCode = response.ToString();
		std::cout << "response code is " << responseCode << std::endl;;

		while(responseCode == "a") {
			socket.Read(response);
			responseCode = response.ToString();
		}

//		while(true) {
//			if(responseCode == "a") {
////				std::cout << "waiting" << std::endl;
//				socket.Read(response);
//				responseCode = response.ToString();
//			}
//			else {
//				break;
//			}
//		}



		socket.Close();

		return 0;
}

void readMessage(Socket &socket){
	while(true){
		ByteArray msg;
		socket.Read(msg);
		//std::cout<<std::endl<<"OTHER> "<<msg.ToString()<<std::endl;
	}
}
