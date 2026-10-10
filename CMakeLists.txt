cmake_minimum_required(VERSION 3.10)
project(CPP_Game)

include_directories(sources/BallGame)
#creating the game target executable
add_executable(Main 
	MainGame.cpp
	sources/BallGame/BallGame.cpp
)

#Linking the SDL3 library
find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(Main PRIVATE SDL3::SDL3)
