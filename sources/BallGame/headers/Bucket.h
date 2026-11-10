#ifndef BUCKET_H
#define BUCKET_H
#include<SDL3/SDL.h>

class Bucket{
	private:
		float bucket_speed = 200.0f;
		int windowWidth, windowHeight;

	public:
		Bucket(int windowWidth, int windowHeight);
		SDL_FRect bucket;
		void increaseSpeed();    
		void DrawBucket(SDL_Renderer *renderer);
		void MoveBucket(float dt);
};

#endif
