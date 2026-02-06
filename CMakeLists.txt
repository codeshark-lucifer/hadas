cmake_minimum_required(VERSION 4.2.0)
project(Hades LANGUAGES C CXX)

if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(app
    src/main.cpp
    src/core/utils.cpp
    src/core/platform.cpp
    src/core/interface.cpp
    src/core/assets.cpp
)

target_include_directories(app PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(app PRIVATE
    user32
    opengl32
    gdi32
    dwmapi
)

add_library(game SHARED
    src/core/game.cpp
    src/core/interface.cpp
    src/core/assets.cpp
)

target_include_directories(game PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(game PROPERTIES
    PREFIX ""
    OUTPUT_NAME "app"
)
