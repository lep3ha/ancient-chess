cmake_minimum_required(VERSION 3.21)

project(chess LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Use an out-of-source build: cmake -S . -B build")
endif()

# The original game uses the SFML 2 API (SFML 3 is incompatible).
find_package(SFML 2.5 CONFIG REQUIRED COMPONENTS graphics window system)

add_executable(chess
    src/main.cpp
    src/board/Board.cpp
    src/board/Board.hpp
    src/board/Cell.cpp
    src/board/Cell.hpp
    src/pieces/Figure.cpp
    src/pieces/Figure.hpp
    src/moves/Move.cpp
    src/moves/Move.hpp
    src/game/Game.hpp
    src/common/Definitions.hpp
)

# Pawn.cpp/Pawn.hpp were not part of the original Visual Studio build.
target_compile_features(chess PRIVATE cxx_std_14)
target_include_directories(chess PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
set_target_properties(chess PROPERTIES
    CXX_EXTENSIONS OFF
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:chess>"
)
target_link_libraries(chess PRIVATE sfml-graphics sfml-window sfml-system)

if(MSVC)
    target_compile_options(chess PRIVATE /utf-8)
endif()

# Keep resources beside the executable for single- and multi-config generators.
add_custom_target(chess_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:chess>/assets"
    COMMENT "Copying chess assets"
    VERBATIM
)
add_dependencies(chess chess_assets)

if(WIN32)
    foreach(module IN ITEMS graphics window system)
        get_target_property(module_type sfml-${module} TYPE)
        if(module_type STREQUAL "SHARED_LIBRARY")
            add_custom_command(TARGET chess POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "$<TARGET_FILE:sfml-${module}>"
                    "$<TARGET_FILE_DIR:chess>"
                VERBATIM
            )
        endif()
    endforeach()
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT chess)
