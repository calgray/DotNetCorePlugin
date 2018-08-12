cmake_minimum_required(VERSION 3.10)

project(SimpleCoreCLRHost)

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Werror -pedantic -Wall -Wextra -Wshadow -Wstrict-overflow=5")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory (dependencies)
add_subdirectory (src)