cmake_minimum_required(VERSION 3.20)

project(GraphingCalculator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GRAPHING_CALCULATOR_BUILD_APP "Build the SFML desktop application" ON)
option(GRAPHING_CALCULATOR_ENABLE_SANITIZERS "Enable ASan and UBSan" OFF)

if(GRAPHING_CALCULATOR_ENABLE_SANITIZERS)
    if(MSVC)
        message(FATAL_ERROR "Sanitizers require GCC or Clang")
    endif()
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

set(GRAPHING_CALCULATOR_RUNTIME_ASSETS
    Poppins-Regular.ttf
    Poppins-OFL.txt
    THIRD_PARTY_NOTICES.md
)

function(package_runtime_assets target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            "$<TARGET_FILE_DIR:${target}>/assets"
    )
    foreach(asset IN LISTS GRAPHING_CALCULATOR_RUNTIME_ASSETS)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/assets/${asset}"
                "$<TARGET_FILE_DIR:${target}>/assets/${asset}"
        )
    endforeach()
endfunction()

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
    enable_strict_warnings(graphing_calculator)
    package_runtime_assets(graphing_calculator)

    install(TARGETS graphing_calculator RUNTIME DESTINATION .)
    install(
        FILES
            assets/Poppins-Regular.ttf
            assets/Poppins-OFL.txt
            assets/THIRD_PARTY_NOTICES.md
        DESTINATION assets
    )
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(calculator_tests tests/calculator_tests.cpp)
    target_link_libraries(calculator_tests PRIVATE calculator_core)
    enable_strict_warnings(calculator_tests)
    add_test(NAME calculator_behavior COMMAND calculator_tests)

    add_executable(evaluator_contract_tests tests/evaluator_contract_tests.cpp)
    target_link_libraries(evaluator_contract_tests PRIVATE calculator_core)
    enable_strict_warnings(evaluator_contract_tests)
    add_test(NAME evaluator_contract COMMAND evaluator_contract_tests)

    if(GRAPHING_CALCULATOR_BUILD_APP)
        add_executable(calculator_resource_smoke
            tests/resource_smoke.cpp
            includes/config/config.cpp
        )
        target_include_directories(calculator_resource_smoke PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}"
        )
        target_link_libraries(calculator_resource_smoke PRIVATE
            sfml-graphics
            sfml-window
            sfml-system
        )
        enable_strict_warnings(calculator_resource_smoke)
        package_runtime_assets(calculator_resource_smoke)

        set(RESOURCE_SMOKE_WORKING_DIRECTORY
            "${CMAKE_CURRENT_BINARY_DIR}/resource-smoke-working-directory"
        )
        file(MAKE_DIRECTORY "${RESOURCE_SMOKE_WORKING_DIRECTORY}")
        add_test(NAME calculator_resources COMMAND calculator_resource_smoke)
        set_tests_properties(calculator_resources PROPERTIES
            WORKING_DIRECTORY "${RESOURCE_SMOKE_WORKING_DIRECTORY}"
        )

        add_executable(plot_contract_tests
            tests/plot_contract_tests.cpp
            includes/plot/plot.cpp
        )
        target_link_libraries(plot_contract_tests PRIVATE
            calculator_core
            sfml-graphics
            sfml-window
            sfml-system
        )
        enable_strict_warnings(plot_contract_tests)
        add_test(NAME plot_contract COMMAND plot_contract_tests)
    endif()
endif()
