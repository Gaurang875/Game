#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<string>
#include<iostream>

//Defining the class for clean code

class App{
    private:
	bool running = true;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	SDL_Event e;

	void update(){
		while(SDL_PollEvent(&e)){
		    if(e.type == SDL_EVENT_QUIT){
			running = false;
		    }
		    else if(e.type == SDL_EVENT_KEY_DOWN){
			if(e.key.key==SDLK_ESCAPE) running = false;
		    }
		    SDL_RenderClear(renderer);
		    SDL_RenderPresent(renderer);
	        }
	}

	void destroy(){
	    SDL_DestroyRenderer(renderer);
	    SDL_DestroyWindow(window);
	}


    public:
	App(const char *title){
	    if(!SDL_Init(SDL_INIT_VIDEO)){
		SDL_Log("Video Initialization failed\n");
		return;
	    }

	    window = SDL_CreateWindow(title,1280,720,SDL_WINDOW_RESIZABLE
			    );
	    if(window == NULL){
		SDL_Log("Window Initialization failed\n");
		//SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	    }

	    renderer=SDL_CreateRenderer(window, NULL);
	    if(renderer == NULL){
		    SDL_Log("Renderer Initialization failed\n");
		    SDL_DestroyWindow(window);
		    SDL_Quit();
		    return;
	    }
	}

	~App(){
	    SDL_Quit();
	}
	
	void mainLoop(){
		if(!running){
		    destroy();
		    return;
		}
		while(running) update();
		destroy();
	}
};

int main(){
	App gameApp("Game Window");
	gameApp.mainLoop();
	return 0;
}


