#include "Menu.h"
#include<SDL.h>
#include <SDL_image.h>
#include"Map.h"
Menu::Menu(SDL_Renderer* renderer) : renderer(renderer) {
     backgroundTexture = IMG_LoadTexture(renderer, "image/back_ground.png"); // Đặt ảnh nền vào thư mục game

    playRect = {200, 200, 200, 80};
    settingRect = {200, 300, 200, 80};
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    playButton = loadTexture("image/new_game.png"); // nút play
    settingButton = loadTexture("image/options.png");// nút options

}
// hàm hủy
Menu::~Menu() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(settingButton);

}
// hàm ktra load menu
SDL_Texture* Menu::loadTexture(const char* path) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path);
    if (!newTexture) std::cout << "Failed to load " << path << " SDL Error: " << SDL_GetError() << std::endl;
    return newTexture;
}
// hàm vẽ
void Menu::render() {
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, playButton, NULL, &playRect);
    SDL_RenderCopy(renderer, settingButton, NULL, &settingRect);
    SDL_RenderPresent(renderer);
}
// thao tác bằng chuột (click)
int Menu::handleEvent(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int x = e.button.x, y = e.button.y;
        SDL_Point mousePoint = {x, y};
        if (SDL_PointInRect(&mousePoint, &playRect)) {
            return 1;
        }
        if (SDL_PointInRect(&mousePoint, &settingRect)) {
            return 2;
        }
    }
    return 0;
}

