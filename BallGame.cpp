#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
using namespace std;
int main(){
	SDL_Window *win;
	bool exit=false;
	if (!SDL_Init(SDL_INIT_VIDEO)) {
        	SDL_Log("SDL_Init failed: %s", SDL_GetError());
		return 1;
    	}
	win=SDL_CreateWindow(
		"Game Window",
		640,
		480,
		0
	);
	if(win == NULL){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window Creation Error: %s\n",SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_ShowWindow(win);
	while(!exit){
		SDL_Event e;
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT){
				exit = true;
			}
		}
	}
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
