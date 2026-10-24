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
		-25.0f,
		25.0f,
		25.0f
	};
	extra_life = {
		(float)SDL_rand(windowWidth -25),
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
/*	if(image == NULL){
	    SDL_Log("Image Load failed\n");
	    return;
	}
*/
	for(auto &rect : PowerUps){
	    rect = {RandomGenerator(), 50.0f, 25.0f, 25.0f};
	}
}

//Class Destructor
BallGame::~BallGame(){
	Destroy();
	SDL_Quit();
}

//The Destroy Function
void BallGame::Destroy(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}


//The main loop Function
void BallGame::mainLoop(){
	if(!running){
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

//The Update Function
void BallGame::ScreenUpdate(float dt){
	Input(dt);
	if(PLAYING){
		SpawnBall(dt);
		PowerUpGenerator(dt);
		renderColor();
		Render();
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

//Keyboard event handling
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


//SpawnBall function
void BallGame::SpawnBall(float dt){
	float step = ball_speed * dt;
	if(Collided(rectangle, ball)){
		ball.y = -25.0f;
		score++;
		SDL_Log("Score Update: %s:", std::to_string(score).c_str());
		if(checkScore()){
		    ball_speed += 50.0f;
		    bucket_speed += 50.0f;
		}
		ball.x = RandomGenerator();
	}
	ball.y += step;
	if(!Collided(rectangle, ball) && ball.y > windowHeight - 25){
		SDL_Log("Game Over\n");
		PLAYING = false;
	}
}


//The powerUp spawner function
void BallGame::PowerUpGenerator(float dt){
	static bool generated = false;
	if(score == extra_life_score){
		ExtraLifeSpawn();
	}
	if(!generated){
	 	power_score = NumberGenerator();
		generated = true;
	}
	int powerup_type = SDL_rand(2);
	
}

//The random BG color generator
void BallGame::renderColor(){
	static int red = 255, blue = 255, green = 255, alpha = 255;
	static bool called = false;
	if(checkScore() && !called){
		red = SDL_rand(250);
		green = SDL_rand(250);
		blue = SDL_rand(250);
		called = true;
	}
	if(!checkScore()) called = false;
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
}

//The Renderer Function
void BallGame::Render(){
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &rectangle);
	DrawBall();
	DrawPowerUp();
	SDL_RenderPresent(renderer);

}


void BallGame::ExtraLifeSpawn(){}
int BallGame::NumberGenerator(){
	int start = 20;
	return score + start + SDL_rand(start);
}

//A function for spawning the ball
void BallGame::DrawBall(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &ball);
}

//The powerup drawer
void BallGame::DrawPowerUp(){
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &PowerUps[0]);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &PowerUps[1]);
	
	SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
	SDL_RenderFillRect(renderer, &PowerUps[2]);

	SDL_SetRenderDrawColor(renderer, 64, 127, 64, 255);
	SDL_RenderFillRect(renderer, &extra_life);
}



//Extra Caller Functions
//Collision Detection Function
bool BallGame::Collided(SDL_FRect bucket, SDL_FRect item){
	return SDL_HasRectIntersectionFloat(&bucket, &item);
}

//Function to check score and modify attributes
bool BallGame::checkScore(){
    return score > 0 && (score % 10 == 0); 
}

//The random position generator
float BallGame::RandomGenerator(){
    return SDL_rand(windowWidth - 25);
}





