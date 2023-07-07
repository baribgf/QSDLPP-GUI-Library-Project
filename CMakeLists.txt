cmake_minimum_required(VERSION 3.25.1)
project("Logic simulator project")

set(CMAKE_CXX_STANDARD 11)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp)
add_executable(main main.cpp ${SRC_FILES})

# add_library(myguilib SHARED ${SRC_FILES})

find_library(SDL2 SDL2 REQUIRED)
find_library(SDL2_ttf SDL2_ttf REQUIRED)
target_link_libraries(main SDL2 SDL2_ttf)
# target_link_libraries(myguilib SDL2 SDL2_ttf)

add_custom_target(run
    COMMAND bash -c "\"${CMAKE_CURRENT_SOURCE_DIR}/build/main\""
    COMMENT "Running project"
)
