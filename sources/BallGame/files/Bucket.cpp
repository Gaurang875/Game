#include "Bucket.h"

Bucket::Bucket(int windowWidth, int windowHeight){
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    bucket = {
        (float)windowWidth/2 - 25,
        (float)windowHeight - 100,
        100.0f,
        50.0f
    };
}

void Bucket::increaseSpeed(){
    this->bucket_speed += 50.0f;    
}

void Bucket::DrawBucket(SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &bucket);
}

void Bucket::MoveBucket(float dt){
    const bool *state = SDL_GetKeyboardState(NULL);
    float steps = bucket_speed * dt;
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]){
        if(bucket.x > 0.0f) bucket.x-=steps;
        else SDL_Log("Left screen limit reached");
    }
    if(state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]){
        if(bucket.x + bucket.w < windowWidth) bucket.x+=steps;
        else SDL_Log("Right Screen Limit Reached");
    }
}
