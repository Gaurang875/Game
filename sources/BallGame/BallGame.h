//Declaration file for the class

#ifndef BALLGAME_H
#define BALLGAME_H

#include <SDL3/SDL.h>
#include <string>


class BallGame{
    //Private Declaration
    private:
	bool running = true; //to check the state of the game
	void ScreenUpdate(float dt); //Update function to call Input() and Render()
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *image;
	int windowWidth, windowHeight;
	bool PLAYING = true;
	SDL_Event e;
	SDL_FRect rectangle;
	SDL_FRect ball;
	float ball_speed = 200.0f;
	float bucket_speed = 200.0f;
	int score = 0;
	void fpsCounter(Uint64 &ct, Uint64 &fps, Uint64 &lt); //FPS COUNTER
	void Input(float dt); //Function for input checking
	void Render(); //Render function to render onto screen
        void Keyboard(float dt); //Handles all keyboard inputs
	void DrawBall();
	void SpawnBall(float dt);
	void Destroy(); //Function to destroy once the quit event is passed
	bool Collided(); //Function to check collision
	void checkScore(int score);
    //Public Declarations
    public:
        BallGame(const char *title); //App constructor
	~BallGame(); //App Destructor:
	void mainLoop(); //The actual loop that will run

};

#endif
