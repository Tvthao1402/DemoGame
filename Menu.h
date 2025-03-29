#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();
    void render();
    int handleEvent(SDL_Event& e);

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* playButton, *settingButton;
    SDL_Rect playRect, settingRect;

    SDL_Texture* loadTexture(const char* path);
};

#endif
