cmake_minimum_required(VERSION 3.20)
project(SocialNetwork)

set(CMAKE_CXX_STANDARD 17)

# Tell compiler where to find headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
add_executable(SocialNetwork
        src/main.cpp
        src/Readers.cpp
        src/Graphs.cpp
)
