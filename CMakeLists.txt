cmake_minimum_required(VERSION 3.5.1)
project(chat-server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_CXX_FLAGS "-O0")

set(SERVER "server")
set(CLIENT "client")

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
    User.cpp
    User.h
	)

set(CLIENT_SRC
	Blockable.cpp
	Blockable.h
	Client.cpp
	Semaphore.h
	SharedObject.h
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
add_executable(${CLIENT} ${CLIENT_SRC})

target_link_libraries(${SERVER} Threads::Threads ${Boost_LIBRARIES} rt)
target_link_libraries(${CLIENT} Threads::Threads ${Boost_LIBRARIES} rt)

