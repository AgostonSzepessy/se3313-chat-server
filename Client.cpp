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

		ByteArray responseCode;
		socket.Read(responseCode);//response code 0 means that you are placed in queue on server
		std::cout<<responseCode.ToString();
		//must wait until server sends up a 1 to be out of queue
		while(responseCode.ToString()=="0"){
			socket.Read(responseCode);
		}

		//start the perpetual reader
		std::thread read(readMessage, std::ref(socket));

		while(true){
			std::cout<<"YOU> ";
			std::string msg;
			std::cin>>msg;
			socket.Write(ByteArray(msg));
		}
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
