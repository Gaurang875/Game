//Declaration file for the class

#ifndef BALLGAME_H
#define BALLGAME_H

#include <SDL3/SDL.h>
#include <string>

class BallGame{
    //Private Declaration
    private:
	bool running = true; //to check the state of the game
	void Update(); //Update function to call Input() and Render()
	SDL_Window *window;
	SDL_Renderer *renderer;
	int windowWidth, windowHeight;
	SDL_FRect rectangle;
	SDL_Event e;
	void fpsCounter(Uint64 *ct, Uint64 *fps, Uint64 *lt); //FPS COUNTER
	void Input(); //Function for input checking
	void Render(); //Render function to render onto screen

	void Destroy(); //Function to destroy once the quit event is passed
	void Keyboard(); //Function to manage continuous Keyboard press

    //Public Declarations
    public:
        BallGame(const char *title); //App constructor
	~BallGame(); //App Destructor
	void mainLoop(); //The actual loop that will run

};

#endif
