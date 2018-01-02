cmake_minimum_required(VERSION 3.7.2)
project(Client)

set(CMAKE_CXX_STANDARD 11)

add_executable(Client
        ../Commom/Libraries/Socket/Socket.cpp
        ../Commom/Libraries/Socket/ServerSocket.cpp
        ../Commom/Libraries/Socket/ClientSocket.cpp
        ../Commom/Production/Header/Chat.h
        ../Commom/Production/Header/User.h
        ../Commom/Production/Source/Chat.cpp
        ../Commom/Production/Source/User.cpp
        Production/Source/Client.cpp)