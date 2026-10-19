cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем SFML (в версии 3.0 компоненты пишутся с заглавной буквы)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

add_executable(snake main.cpp game.cpp)

# В SFML 3.0 цели называются SFML::Graphics, SFML::Window, SFML::System
target_link_libraries(snake SFML::Graphics SFML::Window SFML::System)