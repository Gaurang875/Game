//Declaration file for the class

#ifndef BALLGAME_H
#define BALLGAME_H

#include <SDL3/SDL.h>
#include <string>
#include <vector>

class BallGame{
    //Private Declaration
    private:
	bool running = true; //to check the state of the game
	void ScreenUpdate(float dt); //Update function to call Input() and Render()
	SDL_Window *window; //The window Creator
	SDL_Renderer *renderer; //The window renderer
	//SDL_Texture *image;
	int windowWidth, windowHeight; //variables to calculate window width and height
	bool PLAYING = true; //Variable to check play state
	bool EXTRA = false;
	SDL_Event e; //Event detector
	SDL_FRect rectangle; //The bucket
	SDL_FRect ball; //The ball
	SDL_FRect extra_life;
	std::vector<SDL_FRect>PowerUps = std::vector<SDL_FRect>(3);
	bool active_state[3] = {false, false, false}; //Independent active state checker for each powerup
	Uint64 timer_count[3] = {5, 5, 5}; //Independent timer counter for each powerup
	bool is_falling[3] = {false, false, false}; //A function to check if a ball is falling or not
	int power_score = 0; //Variable to spawn score at an instance
	int extra_life_score = 0;
	float ball_speed = 200.0f; //Ball falling Speed
	float bucket_speed = 200.0f; //Bucket sliding speed
	float fall_speed = 200.0f; //Powerup falling speed
	int score = 0; //Initial score
	void fpsCounter(Uint64 &ct, Uint64 &fps, Uint64 &lt); //FPS COUNTER
	void Input(float dt); //Function for input checking
	void Render(); //Render function to render onto screen
        void Keyboard(float dt); //Handles all keyboard inputs
	void DrawBall(); //Drawing ball onto screen
	void SpawnBall(float dt); //Finding a random position for ball
	void Destroy(); //Function to destroy once the quit event is passed
	bool Collided(SDL_FRect bucket, SDL_FRect item); //Function to check collision
	bool checkScore(); // The score checker
	void renderColor(); //Random BG generator
	float RandomGenerator(); //The random position Generator
	void PowerUpGenerator(float dt); //The powerup chooser and positioner
	void Timer(); //Timer variable to check time on-screen
	void ExtraLifeSpawn(); //The extra life spawner
	void FallPowerup(int power_type); //The powerup y-change function
    	void DrawPowerUp(); //Drawing powerups onto screen
	int NumberGenerator();
    //Public Declarations
    public:
        BallGame(const char *title); //App constructor
	~BallGame(); //App Destructor:
	void mainLoop(); //The actual loop that will run

};

#endif
