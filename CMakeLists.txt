cmake_minimum_required(VERSION 3.9.6)
project(chat-server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SERVER "server")

set(SERVER_SRC 
	Blockable.cpp
	Blockable.h
	Semaphore.h
	SharedObject.h
	Server.cpp
	socket.cpp
	socket.h
	socketserver.cpp
	socketserver.h
	thread.cpp
	thread.h
	)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${SERVER} ${SERVER_SRC})

target_link_libraries(${SERVER} Threads::Threads ${Boost_LIBRARIES} rt)

