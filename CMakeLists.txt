cmake_minimum_required(VERSION 3.20)

project(GraphingCalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GRAPHING_CALCULATOR_BUILD_APP "Build the SFML desktop application" ON)

function(enable_strict_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

set(CALCULATOR_CORE_SOURCES
    includes/evaluator/expression_evaluator.cpp
    includes/lib/rpn/rpn.cpp
    includes/lib/shunting_yard/shunting_yard.cpp
    includes/lib/token/function.cpp
    includes/lib/token/integer.cpp
    includes/lib/token/leftparen.cpp
    includes/lib/token/operator.cpp
    includes/lib/token/rightparen.cpp
    includes/lib/token/token.cpp
    includes/lib/token/variable.cpp
    includes/tokenizer/tokenizer/tokenizer.cpp
    includes/tokenizer/tokenizer_functions/tokenizer_functions.cpp
)

add_library(calculator_core STATIC ${CALCULATOR_CORE_SOURCES})
target_include_directories(calculator_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
enable_strict_warnings(calculator_core)

if(GRAPHING_CALCULATOR_BUILD_APP)
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

    add_executable(graphing_calculator
        main.cpp
        includes/button/button.cpp
        includes/config/config.cpp
        includes/engine/engine.cpp
        includes/header/header.cpp
        includes/plot/plot.cpp
    )
    target_link_libraries(graphing_calculator PRIVATE
        calculator_core
        sfml-graphics
        sfml-window
        sfml-system
    )
    target_compile_definitions(graphing_calculator PRIVATE
        GRAPHING_CALCULATOR_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
    )
    enable_strict_warnings(graphing_calculator)
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(calculator_tests tests/calculator_tests.cpp)
    target_link_libraries(calculator_tests PRIVATE calculator_core)
    enable_strict_warnings(calculator_tests)
    add_test(NAME calculator_behavior COMMAND calculator_tests)
endif()
