cmake_minimum_required (VERSION 3.6)
project(renoir)

add_executable(renoir_bot ligma.cpp)

#Linking sleepy discord
add_subdirectory(deps/sleepy-discord)
target_link_libraries(renoir_bot sleepy-discord)
