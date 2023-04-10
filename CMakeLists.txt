cmake_minimum_required(VERSION 3.22)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")
endif()

include_directories(.)

add_executable(ft_irc
        Channel.cpp
        Channel.hpp
        Client.cpp
        Client.hpp
        main.cpp
        MessageHandler.cpp
        MessageHandler.hpp
        Modifier.hpp
        TcpListener.cpp
        TcpListener.hpp
        utils.hpp
        utils.cpp
        CommandHandler.hpp
        commands/join.cpp
        commands/ping.cpp
        commands/names.cpp
        commands/motd.cpp
        commands/oper.cpp
        commands/mode.cpp
        commands/topic.cpp
        commands/kill.cpp)

add_definitions(-DCATCH_CONFIG_MAIN)
