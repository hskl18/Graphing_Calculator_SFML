cmake_minimum_required(VERSION 3.24)
project(ttt)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(ttt main.cpp includes/config/config.cpp includes/config/config.h includes/header/header.cpp includes/header/header.h includes/input_box/input_box.cpp includes/input_box/input_box.h includes/engine/engine.cpp includes/engine/engine.h includes/button/button.cpp includes/button/button.h includes/button/buttons.cpp includes/button/buttons.h includes/config/system.cpp includes/config/system.h)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(ttt sfml-system sfml-window sfml-graphics sfml-audio sfml-network)