#include <SDL.h>
#include "../SDL_image/SDL_image.h"
#include <sstream>
#include "trace.hpp"
#include "utils.hpp"

void DrawMap(SDL_Renderer* render, SDL_Texture* texture, int offsetToCenter);

#if ANDROID
int SDL_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
    SDL_Window *win = 0;
    SDL_Renderer *ren = 0;
    SDL_Surface *bmp = 0;
    SDL_Texture* tex = 0;
    SDL_Surface *mapSurf = nullptr;
    SDL_Texture *mapTex = nullptr;
    SDL_bool done = SDL_FALSE;

    Trace::Info("SDL","SDL_main starting...");

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
        std::stringstream msg;
        msg << "SDL_Init:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }

    SDL_Rect screenRect = { 0, 0, 608, 512 };

    //Get device display mode
    SDL_DisplayMode displayMode;
    if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
    {
        screenRect.w = displayMode.w;
        screenRect.h = displayMode.h;
    }
    #if ANDROID
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    #endif

    win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenRect.w, screenRect.h, SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    if (win == 0){
        std::stringstream msg;
        msg << "SDL_CreateWindow:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == 0){
        std::stringstream msg;
        msg << "SDL_CreateRenderer:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }

    //bmp = SDL_LoadBMP("image.bmp");
    bmp = IMG_Load(Utils::ResourcePath("player.png").c_str());
    if (!bmp) {
        std::stringstream msg;
        msg << "IMG_Load:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }
    tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (!tex) {
        std::stringstream msg;
        msg << "SDL_CreateTextureFromSurface:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }

    mapSurf = IMG_Load(Utils::ResourcePath("walls.png").c_str());
    if (!mapSurf) {
        std::stringstream msg;
        msg << "IMG_Load:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }
    mapTex = SDL_CreateTextureFromSurface(ren, mapSurf);
    SDL_FreeSurface(mapSurf);
    if (!mapTex) {
        std::stringstream msg;
        msg << "SDL_CreateTextureFromSurface:" << SDL_GetError();
        Trace::Info("SDL",msg.str());
        return 1;
    }

    //SDL_RenderSetLogicalSize(ren, 1.0f, 1.0f);
    float scaleFactor = screenRect.h/(512*1.0f);
    SDL_RenderSetScale(ren, scaleFactor, scaleFactor);
    int offsetToCenter = ((screenRect.w / scaleFactor)-(32*19))/2;



    std::stringstream msg;
    msg << "SDL_Success:" << SDL_GetError();
    Trace::Info("SDL",msg.str());    

    SDL_Rect orig = {0,0,32,32};
    SDL_Rect dest = {455-16,256-32,32,32};

    Uint32 lFPSTime = SDL_GetTicks();
    Uint32 frameTime;
    const int MIN_FRAME_TIME = 16;
    int iNumOfFPS, iFPS;
    while (!done) {
        frameTime = SDL_GetTicks();
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT || event.type == SDL_KEYDOWN || event.type == SDL_FINGERDOWN) {
                done = SDL_TRUE;
            }
        }

        SDL_SetRenderDrawColor(ren, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderClear(ren);
        DrawMap(ren, mapTex, offsetToCenter);
        SDL_RenderCopy(ren, tex, &orig, &dest);

        std::stringstream msg;
        msg << "FPS:" << iNumOfFPS;
        Trace::Info("SDL",msg.str());

		if(SDL_GetTicks() - 1000 >= lFPSTime) {
			lFPSTime = SDL_GetTicks();
			iNumOfFPS = iFPS;
			iFPS = 0;
		}
		++iFPS;

        SDL_RenderPresent(ren);

        msg.str("");
        msg << "DT:" << (SDL_GetTicks () - frameTime)*0.001;
        Trace::Info("SDL",msg.str());
        if(SDL_GetTicks() - frameTime < MIN_FRAME_TIME) {
			SDL_Delay(MIN_FRAME_TIME - (SDL_GetTicks () - frameTime));
		}
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

void DrawMap(SDL_Renderer* render, SDL_Texture* texture, int offsetToCenter)
{
    std::stringstream msg;
    msg << "OffsetToCenter:" << offsetToCenter;
    Trace::Info("SDL",msg.str());
    int map[16][19] = {
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1}
    };
    SDL_Rect orig = {0,0,32,32};

    for(int r=0; r<16; r++) {
        for (int c = 0; c < 19; c++) {
            if (map[r][c] == 1) {
                SDL_Rect dest = {32 * c + offsetToCenter, 32 * r, 32, 32};
                SDL_RenderCopy(render, texture, &orig, &dest);
            }
        }
    }
}