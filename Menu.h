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

    void render(); // Vẽ menu
    int handleEvent(SDL_Event& e); // Xử lý sự kiện chuột

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    SDL_Texture* playButton, *settingButton;
    SDL_Rect playRect, settingRect;

    SDL_Texture* loadTexture(const char* path); // Hàm tải ảnh
};

#endif
