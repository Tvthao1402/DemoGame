#include "Button.h"
#include <iostream>
#include "Map.h"
// các nút thao tác khi pause và game over
ButtonHandler::ButtonHandler(SDL_Renderer* renderer) {
    restartTexture = nullptr;
    quitTexture = nullptr;
    continueTexture = nullptr;
    exitTexture = nullptr;
    backTexture = nullptr;
    newGameTexture = nullptr;

    const int restartWidth = 50;
    const int restartHeight = 50;
    const int quitWidth = 100;
    const int quitHeight = 50;
    const int continueWidth = 50;
    const int continueHeight = 50;
    const int exitWidth = 100;
    const int exitHeight = 50;
    const int backWidth = 100;
    const int backHeight = 50;
    const int newGameWidth = 100;
    const int newGameHeight = 50;
    restartRect = {
        SCREEN_WIDTH / 2 - restartWidth / 2 - 70,
        SCREEN_HEIGHT / 2 - restartHeight / 2 - 70,
        restartWidth,
        restartHeight
    }; // kích thươc restart
    quitRect = {
        SCREEN_WIDTH / 2 - quitWidth / 2 - 50,
        restartRect.y + restartHeight + 70,
        quitWidth,
        quitHeight
    };// kích thước nút quit
    continueRect = {
        SCREEN_WIDTH / 2 - continueWidth / 2 - 70,
        SCREEN_HEIGHT / 2 - continueHeight / 2 -100,
        continueWidth,
        continueHeight
    };// kích thước nút continue
    exitRect = {
        SCREEN_WIDTH / 2 - exitWidth / 2 - 50,
        continueRect.y + continueHeight + 100,
        exitWidth,
        exitHeight
    };// kích thước exit
    backRect = {10 ,  10,  backWidth, backHeight}; // kích thước nút back
    newGameRect = {SCREEN_WIDTH / 2 - newGameWidth / 2 - 50, continueRect.y - newGameHeight - 100, newGameWidth, newGameHeight};// kích thước nút new game
    SDL_Surface* restartSurface = IMG_Load("image/reset_game.png");
    SDL_Surface* quitSurface = IMG_Load("image/exit.png");
    SDL_Surface* continueSurface = IMG_Load("image/continue.png");
    SDL_Surface* exitSurface = IMG_Load("image/exit1.png");
    SDL_Surface* backSurface = IMG_Load("image/back.png");
    SDL_Surface* newGameSurface = IMG_Load("image/new_game_1.png");

    if (!restartSurface) {
        std::cout << "Không thể load ảnh reset_game.png: " << IMG_GetError() << std::endl;
    } else {
        restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
        SDL_FreeSurface(restartSurface);
        SDL_QueryTexture(restartTexture, NULL, NULL, &restartRect.w, &restartRect.h);
    }

    if (!quitSurface) {
        std::cout << "Không thể load ảnh exit_gameover.png: " << IMG_GetError() << std::endl;
    } else {
        quitTexture = SDL_CreateTextureFromSurface(renderer, quitSurface);
        SDL_FreeSurface(quitSurface);
        SDL_QueryTexture(quitTexture, NULL, NULL, &quitRect.w, &quitRect.h);
    }

    if (!continueSurface) {
        std::cout << "Không thể load ảnh continue.png: " << IMG_GetError() << std::endl;
    } else {
        continueTexture = SDL_CreateTextureFromSurface(renderer, continueSurface);
        SDL_FreeSurface(continueSurface);
        SDL_QueryTexture(continueTexture, NULL, NULL, &continueRect.w, &continueRect.h);
    }

    if (!exitSurface) {
        std::cout << "Không thể load ảnh exit.png: " << IMG_GetError() << std::endl;
    } else {
        exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
        SDL_FreeSurface(exitSurface);
        SDL_QueryTexture(exitTexture, NULL, NULL, &exitRect.w, &exitRect.h);
    }
    if (!backSurface) {
        std::cout << "Không thể load ảnh back.png: " << IMG_GetError() << std::endl;
    } else {
        backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
        SDL_FreeSurface(backSurface);
        SDL_QueryTexture(backTexture, NULL, NULL, &backRect.w, &backRect.h);
    }
    if (!newGameSurface) {
        std::cout << "Không thể load ảnh new_game.png: " << IMG_GetError() << std::endl;
    } else {
        newGameTexture = SDL_CreateTextureFromSurface(renderer, newGameSurface);
        SDL_FreeSurface(newGameSurface);
        SDL_QueryTexture(newGameTexture, NULL, NULL, &newGameRect.w, &newGameRect.h);
    }
}
// hàm hủy
ButtonHandler::~ButtonHandler() {
    SDL_DestroyTexture(restartTexture);
    SDL_DestroyTexture(quitTexture);
    SDL_DestroyTexture(continueTexture);
    SDL_DestroyTexture(exitTexture);
    SDL_DestroyTexture(backTexture);
    SDL_DestroyTexture(newGameTexture);
}
// hàm vẽ
void ButtonHandler::render(SDL_Renderer* renderer, bool isPaused, bool isGameOver, bool isHowToPlay) {
    if (isPaused) { // nếu pause game
        if (newGameTexture) {
            SDL_RenderCopy(renderer, newGameTexture, NULL, &newGameRect);
        }
        if (continueTexture) {
            SDL_RenderCopy(renderer, continueTexture, NULL, &continueRect);
        }
        if (exitTexture) {
            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        }
    } else if (isGameOver) {// nếu game over
        if (restartTexture) {
            SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
        }
        if (quitTexture) {
            SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);
        }
    } else if (isHowToPlay) {// nếu chọn options
        if (backTexture) {
            SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
        }
    }
}
// check click thành công
bool ButtonHandler::checkRestartClick(int mouseX, int mouseY) {
    return (mouseX >= restartRect.x && mouseX <= restartRect.x + restartRect.w &&
            mouseY >= restartRect.y && mouseY <= restartRect.y + restartRect.h);
}

bool ButtonHandler::checkQuitClick(int mouseX, int mouseY) {
    return (mouseX >= quitRect.x && mouseX <= quitRect.x + quitRect.w &&
            mouseY >= quitRect.y && mouseY <= quitRect.y + quitRect.h);
}

bool ButtonHandler::checkContinueClick(int mouseX, int mouseY) {
    return (mouseX >= continueRect.x && mouseX <= continueRect.x + continueRect.w &&
            mouseY >= continueRect.y && mouseY <= continueRect.y + continueRect.h);
}

bool ButtonHandler::checkExitClick(int mouseX, int mouseY) {
    return (mouseX >= exitRect.x && mouseX <= exitRect.x + exitRect.w &&
            mouseY >= exitRect.y && mouseY <= exitRect.y + exitRect.h);
}
bool ButtonHandler::checkBackClick(int mouseX, int mouseY) {
    return (mouseX >= backRect.x && mouseX <= backRect.x + backRect.w &&
            mouseY >= backRect.y && mouseY <= backRect.y + backRect.h);
}
bool ButtonHandler::checkNewGameClick(int mouseX, int mouseY) {
    return (mouseX >= newGameRect.x && mouseX <= newGameRect.x + newGameRect.w &&
            mouseY >= newGameRect.y && mouseY <= newGameRect.y + newGameRect.h);
}
