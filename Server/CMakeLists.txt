cmake_minimum_required(VERSION 3.7.2)
project(Server)

set(CMAKE_CXX_STANDARD 11)

find_package (Threads)
add_executable(Server
        ../Commom/Libraries/Socket/Socket.cpp
        ../Commom/Libraries/Socket/ServerSocket.cpp
        ../Commom/Libraries/Socket/ClientSocket.cpp
        ../Commom/Production/Header/Chat.h
        ../Commom/Production/Header/User.h
        ../Commom/Production/Header/Global.h
        ../Commom/Production/Source/Chat.cpp
        ../Commom/Production/Source/User.cpp
        ../Commom/Production/Source/Global.cpp
        Production/Source/Server.cpp)

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT})