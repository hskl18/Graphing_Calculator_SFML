cmake_minimum_required(VERSION 3.24)
project(ttt)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)
add_executable(ttt main.cpp includes/config/config.cpp includes/config/config.h includes/header/header.cpp includes/header/header.h includes/input_box/input_box.h includes/engine/engine.cpp includes/engine/engine.h includes/button/button.cpp includes/button/button.h includes/lib/linked_list_functions/linked_list_functions.h includes/lib/lru/lru.h includes/lib/node/node.h includes/lib/queue/MyQueue.h includes/lib/rpn/rpn.cpp includes/lib/shunting_yard/shunting_yard.cpp includes/lib/token/function.cpp includes/lib/token/integer.cpp includes/lib/token/leftparen.cpp includes/lib/token/operator.cpp includes/lib/token/rightparen.cpp includes/lib/token/token.cpp includes/lib/token/variable.cpp includes/plot/plot.cpp includes/plot/plot.cpp includes/tokenizer/tokenizer/tokenizer.cpp includes/tokenizer/tokenizer_functions/tokenizer_functions.cpp)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(ttt sfml-system sfml-window sfml-graphics sfml-audio sfml-network)