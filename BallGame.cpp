#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<string>
#include<cassert>
#include<iostream>

//Defining the class for clean code

class App{
    private:
	bool running = true;
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
	int windowWidth, windowHeight;
	SDL_FRect rectangle;
	SDL_Event e;

	void update(){
		
		Input();
		Render();

	}

	void fpsCounter(Uint64 *currentTick ,Uint64 *fps, Uint64 *lastTick){
	    SDL_Delay(16);
	    (*fps)++;
	    if(*currentTick > *lastTick + 1000){
	        *lastTick = *currentTick;
		auto fps_str = "Frame Rate: " + std::to_string(*fps);
		SDL_SetWindowTitle(window, fps_str.c_str());
		(*fps)=0;
	    }

	}

	void Input(){
	    while(SDL_PollEvent(&e)){
	        if(e.type == SDL_EVENT_QUIT){
		    running = false;
		}
		else if(e.type == SDL_EVENT_KEY_DOWN){
		    if(e.key.key==SDLK_ESCAPE) running = false;
		}
		   
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

	    window = SDL_CreateWindow(
			    title, //The actual Title of the window
			    1280, //The width of the window
			    720, //The height of the window
			    SDL_WINDOW_OPENGL //Flags for the window creation
		     );
            rectangle={
	    		(float)windowWidth/2-25, 
	    		(float)windowHeight-100, 
	    		50.0, 
	    		50.0
	    	      };	

	    if(window == NULL){
		SDL_Log("Window Initialization failed\n");
		//SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	    }
	    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	   
	    renderer=SDL_CreateRenderer(window, nullptr);

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

	void Render(){

	    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //Giving the background a blue color
            SDL_RenderClear(renderer); //Clearing the renderer before actually drawing a rectangle onto the screen
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //Giving the rectangle a green color
	    SDL_RenderFillRect(renderer, &rectangle); //Filling the reactangle onto the screen
	    SDL_RenderPresent(renderer); //Presenting the renderer to the user
	}
	
	void mainLoop(){
		if(!running){
		    destroy();
		    return;
		}
		Uint64 fps=0;
		Uint64 lastTick=0;
		while(running){
		    Uint64 currentTick = SDL_GetTicks();
		    update();
		    fpsCounter(&currentTick,&fps, &lastTick);
		}
		destroy();
	}
};

int main(){
	App gameApp("Game Window");
	gameApp.mainLoop();
	return 0;

}


