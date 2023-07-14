cmake_minimum_required(VERSION 3.25.1)
project("Logic simulator project")

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_library(QSDL++ SHARED ${SRC_FILES})

find_library(SDL2 SDL2 REQUIRED)
find_library(SDL2_ttf SDL2_ttf REQUIRED)
find_library(SDL2_image SDL2_image REQUIRED)
target_link_libraries(QSDL++ SDL2 SDL2_ttf SDL2_image)

install(
    TARGETS QSDL++
    LIBRARY DESTINATION lib
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/QSDL++ DESTINATION include
)
