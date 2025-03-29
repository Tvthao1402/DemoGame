#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <SDL.h>
#include <SDL_image.h>

class ButtonHandler {
private:
    SDL_Texture* restartTexture;
    SDL_Texture* quitTexture;
    SDL_Texture* continueTexture;
    SDL_Texture* exitTexture;
    SDL_Rect restartRect;
    SDL_Rect quitRect;
    SDL_Rect continueRect;
    SDL_Rect exitRect;

public:
    ButtonHandler(SDL_Renderer* renderer);
    ~ButtonHandler();

    void render(SDL_Renderer* renderer, bool isPaused, bool isGameOver);
    bool checkRestartClick(int mouseX, int mouseY);
    bool checkQuitClick(int mouseX, int mouseY);
    bool checkContinueClick(int mouseX, int mouseY);
    bool checkExitClick(int mouseX, int mouseY);
};

#endif
