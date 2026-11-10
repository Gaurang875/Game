cmake_minimum_required(VERSION 3.10)
project(CPP_Game)

include_directories(sources/BallGame
	sources/BallGame/headers
)
#creating the game target executable
add_executable(Game 
	MainGame.cpp
	sources/BallGame/Main.cpp
	sources/BallGame/files/Bucket.cpp
)

#Linking the SDL3 library
find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(Game PRIVATE SDL3::SDL3)
