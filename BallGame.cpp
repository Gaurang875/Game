#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(){
    //Creating global variables

    SDL_Window *win = nullptr;
    SDL_Renderer *renderer = nullptr;
    bool exit = false;


    //Initializing SDL3
    
    SDL_Init(SDL_INIT_VIDEO);

    if(!SDL_INIT_VIDEO){
	//checking for any errors during initialization
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL Initialization failed: %s\n", SDL_GetError());
	return 1;
    } 


    //Creating the SDL3 Window
    
    win = SDL_CreateWindow(
        "Game Window",
        640,
        480,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_MINIMIZED
    );

    if(win == NULL){
	//Checking for any error during window creation
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    //Creating a renderer to bypass Wayland Rules

    renderer = SDL_CreateRenderer(win, NULL); //1. In which window to render 2.Which rendering driver to use (NULL-> AUTO SELECT)

    if(renderer == NULL){
	//Checking for any errors during the Initial rendering in the screen
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
	SDL_Quit();
        return 1;
    }


    //The game loop to keep window and game running
    while(!exit){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_EVENT_QUIT){
                exit = true;
            }
        }

        SDL_RenderClear(renderer); //Clearing any contents of the rendering everytime in loop iteration to avoid jitters

        SDL_RenderPresent(renderer); //Upadte screen by discarding previous rendering
    }


    //Destroying the loop elements once the loop is exited

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
