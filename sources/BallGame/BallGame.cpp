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
			720,
			960,
			SDL_WINDOW_OPENGL
			);
	if(window == NULL){
		SDL_Log("Window Initialization failed\n");
		SDL_Quit();
		return;
	}
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	rectangle = {
		(float)windowWidth/2 - 25,
		(float)windowHeight - 100,
		100.0f,
		50.0f
	};
	ball = {
		(float)SDL_rand(windowWidth - 25),
		50.0f,
		25.0f,
		25.0f
	};
	renderer = SDL_CreateRenderer(window, nullptr);
	if(renderer == NULL){
		SDL_Log("Renderer Initialization failed");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return;
	}
//	image = SDL_LoadTexture(renderer, "./Demo.png");
	if(image == NULL){
	    SDL_Log("Image Load failed\n");
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rectangle);
	DrawBall();
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
	Uint64 lastFrameTick = SDL_GetTicks();
	while(running){
		Uint64 currentTick = SDL_GetTicks();
		float dt = (currentTick - lastFrameTick)/1000.0f;
		lastFrameTick = currentTick;
		ScreenUpdate(dt);
		fpsCounter(currentTick, fps, lastTick);
	}
	Destroy();
}

//The Update Function
void BallGame::ScreenUpdate(float dt){
	Input(dt);
	if(PLAYING){
		SpawnBall(dt);    
		Render();
	}
}

//The FPSCounter Function
void BallGame::fpsCounter(Uint64 &ct, Uint64 &fps, Uint64 &lt){
	SDL_Delay(16);
	fps++;
	if(ct > lt + 1000){
		lt = ct;
		auto fps_str="Frame Rate: "+ std::to_string(fps);
		SDL_SetWindowTitle(window, fps_str.c_str());
		fps = 0;
	}
}

//Input Handler Function
void BallGame::Input(float dt){
	while(SDL_PollEvent(&e)){
		if (e.type ==SDL_EVENT_QUIT) running = false;
		else if(e.type == SDL_EVENT_KEY_DOWN){
			if(e.key.key == SDLK_ESCAPE) running = false;
		}
	}
	if(PLAYING){
		Keyboard(dt);
	}

}

//The Destroy Function
void BallGame::Destroy(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

//Keyboard event handling`
void BallGame::Keyboard(float dt){
	const bool *state = SDL_GetKeyboardState(NULL);
	float steps = bucket_speed * dt;
	if (state[SDL_SCANCODE_LEFT]){
		if(rectangle.x > 0.0f) rectangle.x-=steps;
		else SDL_Log("Left screen limit reached");
	}
	if(state[SDL_SCANCODE_RIGHT]){
		if(rectangle.x + rectangle.w < windowWidth) rectangle.x+=steps;
		else SDL_Log("Right Screen Limit Reached");
	}
}

//A function for spawning the ball
void BallGame::DrawBall(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &ball);
}

//A function for one ball per second
void BallGame::SpawnBall(float dt){
	float step = ball_speed * dt;
	if(Collided()){
		ball.y = 0;
		score++;
		SDL_Log("Score Update: %s:", std::to_string(score).c_str());
		checkScore(score);
		ball.x = SDL_rand(windowWidth);
	}
	ball.y += step;
	if(!Collided() && ball.y > windowHeight){
		SDL_Log("Game Over\n");
		PLAYING = false;
	}
}

//Collision Detection Function
bool BallGame::Collided(){
	return SDL_HasRectIntersectionFloat(&rectangle, & ball);
}

//Function to check score and modify attributes
void BallGame::checkScore(int score){
	if(score % 10 == 0){
		//if(bucket_speed <= 1000 && ball_speed <= 1000){
		ball_speed += 50.0f;
		bucket_speed += 50.0f;
		//}
	}
}
