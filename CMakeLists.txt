cmake_minimum_required(VERSION 3.10)
project(CPP_Game)


#creating the game target executable
add_executable(BallGame BallGame.cpp)

#Linking the SDL3 library
find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(BallGame PRIVATE SDL3::SDL3)
