#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace Sync;

const unsigned PORT = 2000;

int main(void)
{
	// Repeatedly send data to server and read it back
	for(;;)
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

		std::string input;
		std::cout << "Enter message: ";
		std::cin >> input;

		if(input == "done")
		{
			break;
		}

		ByteArray data(input);
		socket.Write(data);
		ByteArray serverData;
		socket.Read(serverData);

		std::cout << serverData.ToString() << std::endl;
		socket.Close();
	}

	return 0;
}
