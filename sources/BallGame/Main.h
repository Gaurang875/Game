#ifndef MAIN_H
#define MAIN_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>
#include "Bucket.h"

class BallGame{
	private:
		bool running = true; 
		void ScreenUpdate(float dt); 
		SDL_Window *window; 
		SDL_Renderer *renderer; 
		int windowWidth, windowHeight; 
		Bucket *myBucket = nullptr;
		bool PLAYING = true; 
		bool ALL_ACTIVE = false;
		SDL_Event e; 
		SDL_FRect ball; 
		SDL_FRect extra_life;
		std::vector<SDL_FRect>PowerUps = std::vector<SDL_FRect>(3);
		bool power_state[3] = {false, false, false}; 
		float timer_count[3] = {5.0f, 5.0f, 5.0f}; 
		int power_score = 0; 
		bool fall_state[3] = {false, false, false};
		int extra_life_score = 150;
		float ball_speed = 200.0f; 
		float powerfall_speed = 200.0f; 
		bool GHOST_BALL = false; 
		bool SCORE2X = false; 
		int score = 0; 

		void fpsCounter(Uint64 &ct, Uint64 &fps, Uint64 &lt); 
		void Input(float dt); 
		void Render(); 
		void DrawBall(); 
		void SpawnBall(float dt); 
		void Destroy(); 
		bool Collided(SDL_FRect bucket, SDL_FRect item); 
		bool checkScore(); 
		void renderColor(); 
		float RandomGenerator(); 
		void PowerUpGenerator(float dt); 
		void DrawPowerUp(); 
		int NumberGenerator();                             
		bool allActive(); 
		void countTimer(float dt); 
		void FallPowerup(float dt);
		void ChooseBuff(int powerType);
		void RemoveBuff(int powerType);

	public:
		BallGame(const char *title); 
		~BallGame(); 
		void mainLoop(); 
};

#endif
