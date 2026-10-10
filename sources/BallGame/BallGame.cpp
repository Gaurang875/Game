//Implementation file for the header

#include "BallGame.h"

//Class constructor
BallGame::BallGame(const char *title){
    if(!SDL_Init(SDL_INIT_VIDEO)){
	SDL_Log("Video Initialization failed\n");
	return;
    }
    window = SDL_CreateWindow(
		    title, 
		    1280,
		    720,
		    SDL_WINDOW_OPENGL
	    );
    if(window == NULL){
	SDL_Log("Window Initialization failed\n");
	SDL_Quit();
	return;
    }
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    rectangle={
	(float)windowWidth/2-25,
	(float)windowHeight-100,
	50.0,
	50.0
    };
    renderer = SDL_CreateRenderer(window, nullptr);
    if(renderer == NULL){
	    SDL_Log("Renderer Initialization failed");
	    SDL_DestroyWindow(window);
	    SDL_Quit();
	    return;
    }
}

//Class Destructor
BallGame::~BallGame(){
	Destroy();
	SDL_Quit();
}

//The Renderer Function
void BallGame::Render(){
	SDL_SetRenderDrawColor(renderer, 255,0,0,255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rectangle);
	SDL_RenderPresent(renderer);
}

//The main loop Function
void BallGame::mainLoop(){
	if(!running){
	    Destroy();
	    return;
	}
	Uint64 fps = 0;
	Uint64 lastTick = 0;
	while(running){
		Uint64 currentTick=SDL_GetTicks();
		Update();
		fpsCounter(&currentTick, &fps, &lastTick);
	}
	Destroy();
}

//The Update Function
void BallGame::Update(){
    Input();
    Render();
}

//The FPSCounter Function
void BallGame::fpsCounter(Uint64 *ct, Uint64 *fps, Uint64 *lt){
    SDL_Delay(16);
    (*fps)++;
    if(*ct>*lt+1000){
	*lt=*ct;
	auto fps_str="Frame Rate: "+ std::to_string(*fps);
	SDL_SetWindowTitle(window, fps_str.c_str());
	(*fps)=0;
    }
}

//Input Handler Function
void BallGame::Input(){
	while(SDL_PollEvent(&e)){
	    if (e.type ==SDL_EVENT_QUIT) running = false;
	        else if(e.type == SDL_EVENT_KEY_DOWN){
		    if(e.key.key == SDLK_ESCAPE) running = false;
	        }
	    }
            Keyboard();	    
}

//The Destroy Function
void BallGame::Destroy(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

//Keyboard event handling
void BallGame::Keyboard(){
    const bool *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT]){
        if(rectangle.x > 0) rectangle.x-=5;
        else SDL_Log("Left window Limit reached\n");
    }
    if(state[SDL_SCANCODE_RIGHT]){
        if(rectangle.x + rectangle.w < windowWidth) rectangle.x+=5;
        else SDL_Log("Right Window Limit reached\n");
    }
}
