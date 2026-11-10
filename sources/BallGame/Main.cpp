#include "Main.h"
#include <cmath>

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

	myBucket = new Bucket(windowWidth, windowHeight);

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
	for(auto &rect : PowerUps){
		rect = {RandomGenerator(), 50.0f, 25.0f, 25.0f};
	}
	power_score = NumberGenerator();
	SDL_Log("Next powerup generated at: %d\n",power_score);
}

BallGame::~BallGame(){
	Destroy();
	SDL_Quit();
}

void BallGame::Destroy(){
	delete myBucket;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

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

void BallGame::ScreenUpdate(float dt){
	Input(dt);
	if(PLAYING){
		SpawnBall(dt);
		PowerUpGenerator(dt);
		renderColor();
		Render();
	}
}

void BallGame::Input(float dt){
	while(SDL_PollEvent(&e)){
		if (e.type ==SDL_EVENT_QUIT) running = false;
		else if(e.type == SDL_EVENT_KEY_DOWN){
			if(e.key.key == SDLK_ESCAPE) running = false;
		}
	}
	if(PLAYING){
		myBucket->MoveBucket(dt);
	}
}

void BallGame::SpawnBall(float dt){
	float step = ball_speed * dt;
	if(!Collided(myBucket->bucket, ball) && ball.y > windowHeight - 25){
		if(GHOST_BALL){
			if(SCORE2X) score += 2;
			else score++;
			SDL_Log("Score Update: %s:", std::to_string(score).c_str());
			ball.y = -25.0f;
		}
		else{
			SDL_Log("Game Over\n");
			PLAYING = false;
		}
	}
	else{
		if(Collided(myBucket->bucket, ball)){
			ball.y = -25.0f;
			if(GHOST_BALL){
				if(SCORE2X) score +=2;
				else score++;
			}
			else score ++;
			SDL_Log("Score Update: %s:", std::to_string(score).c_str());
			if(checkScore()){
				ball_speed += 50.0f;
				myBucket->increaseSpeed();
			}
			ball.x = RandomGenerator();
		}
		ball.y += step;
	}
}

void BallGame::PowerUpGenerator(float dt){
	if(allActive()){
		countTimer(dt);
		return;
	}
	if(score >= power_score){
		int pType = SDL_rand(2);
		if(!fall_state[pType]){
			fall_state[pType] = true;
			PowerUps[pType].x = RandomGenerator();
			PowerUps[pType].y = -25;
		}
		power_score = NumberGenerator();
		SDL_Log("Next powerup generated at: %d\n", power_score);
	}
	FallPowerup(dt);
}

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

void BallGame::Render(){
	SDL_RenderClear(renderer);
	myBucket->DrawBucket(renderer);    
	DrawPowerUp();
	DrawBall();
	SDL_RenderPresent(renderer);
}

int BallGame::NumberGenerator(){
	int start = 1;
	return score + start + SDL_rand(start + 1);
}

bool BallGame::allActive(){
	for(bool check : power_state){
		if(!check) return false;
	}
	return true;
}

void BallGame::FallPowerup(float dt){
	float steps = powerfall_speed * dt;
	for(int i=0;i<2;i++){
		if(fall_state[i]){
			PowerUps[i].y += steps;
			if(Collided(myBucket->bucket, PowerUps[i])){
				power_state[i] = true;
				fall_state[i] = false;
				timer_count[i] = 10.0f;
				ChooseBuff(i);
			}        
			else if(PowerUps[i].y > windowHeight){
				fall_state[i] = false;
			}
		}
		else continue;
	}
	countTimer(dt);
}

void BallGame::countTimer(float dt){
	bool canPrint = false;
	static float cooldown = 1.0f;
	cooldown -= dt;
	if(cooldown <= 0){
		canPrint = true;
		cooldown = 1.0f;
	}
	else canPrint = false;

	for(int i=0;i<2;i++){
		if(power_state[i] == true){
			timer_count[i] -= dt;
			if(canPrint){
				SDL_Log("time remaining for buff %d is %.2f",i, std::round(timer_count[i]));
			}
			if(timer_count[i] <= 0){
				power_state[i] = false;
				RemoveBuff(i);
			}
		}
	}
}

void BallGame::ChooseBuff(int powerType){
	switch(powerType){
		case 0:
			GHOST_BALL = true;
			SDL_Log("Ghost Ball Active");
			PowerUps[0].y = 50;
			break;
		case 1:
			SCORE2X = true;
			SDL_Log("2XSCORE Active\n");
			PowerUps[1].y = 50;
			break;
	}
}

void BallGame::RemoveBuff(int powerType){
	switch(powerType){
		case 0:
			GHOST_BALL = false;
			SDL_Log("Ghost ball deactivated\n");
			break;
		case 1:
			SCORE2X = false;
			SDL_Log("2XSCORE deactivated\n");
			break;
	}
}

void BallGame::DrawBall(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &ball);
}

void BallGame::DrawPowerUp(){
	for(int i=0; i<2; i++){
		if(fall_state[i]){
			if(i == 0){
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			if(i == 1){
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &PowerUps[i]);
		}
	}
}

bool BallGame::Collided(SDL_FRect bucket, SDL_FRect item){
	return SDL_HasRectIntersectionFloat(&bucket, &item);
}

bool BallGame::checkScore(){
	return score > 0 && (score % 10 == 0); 
}

float BallGame::RandomGenerator(){
	return SDL_rand(windowWidth - 25);
}
