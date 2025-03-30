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
    SDL_Texture* backTexture;
    SDL_Texture* newGameTexture;
    SDL_Rect restartRect;
    SDL_Rect quitRect;
    SDL_Rect continueRect;
    SDL_Rect exitRect;
    SDL_Rect backRect;
    SDL_Rect newGameRect;

public:
    ButtonHandler(SDL_Renderer* renderer);
    ~ButtonHandler();

    void render(SDL_Renderer* renderer, bool isPaused, bool isGameOver,bool isHowToPlay);
    bool checkRestartClick(int mouseX, int mouseY);
    bool checkQuitClick(int mouseX, int mouseY);
    bool checkContinueClick(int mouseX, int mouseY);
    bool checkExitClick(int mouseX, int mouseY);
    bool checkBackClick(int mouseX, int mouseY);
    bool checkNewGameClick(int mouseX, int mouseY);
};

#endif
