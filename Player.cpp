#include"Player.h"
#include "Map.h"
#include<ctime>
#include<cstdlib>
#include<cstdio>
#include<iostream>
#include"Bullet.h"
#include<cmath>
#include<sstream>
using namespace std;
// hàm khởi tạo
Player::Player()
    : x(0), y(0), facingX(0), facingY(1), angle(0),live(5),  score(0),
      textureUp(nullptr), textureDown(nullptr), textureLeft(nullptr), textureRight(nullptr),
      currentTexture(nullptr), heartTexture(nullptr), currentDirection(DOWN) {
}
// hàm hủy
Player::~Player() {
    if (textureUp) SDL_DestroyTexture(textureUp);
    if (textureDown) SDL_DestroyTexture(textureDown);
    if (textureLeft) SDL_DestroyTexture(textureLeft);
    if (textureRight) SDL_DestroyTexture(textureRight);
    if (heartTexture) SDL_DestroyTexture(heartTexture);
}
// hàm di chuyển nhân vật
void Player::movePlayer( const Uint8* keys, Player& player1, Player& player2) {
        float dx = 0 , dy = 0;
      if(this == &player1){
            // di chuyển lên
            if (keys[SDL_SCANCODE_W] && !gameMap.checkCollision(player1.x, player1.y - PLAYER_SPEED , PLAYER_SIZE)) {
                player1.y -= PLAYER_SPEED;
                player1.facingX = 0; player1.facingY = -1;
                currentDirection = UP;
            currentTexture = textureUp;
            }
      // di chuyển xuống
            if (keys[SDL_SCANCODE_S] && !gameMap.checkCollision(player1.x, player1.y + PLAYER_SPEED, PLAYER_SIZE)) {
                player1.y += PLAYER_SPEED;
                player1.facingX = 0; player1.facingY = 1;
                currentDirection = DOWN;
            currentTexture = textureDown;
            }
            // di chuyển sang trái
            if (keys[SDL_SCANCODE_A] && !gameMap.checkCollision(player1.x - PLAYER_SPEED, player1.y,PLAYER_SIZE)) {
                player1.x -= PLAYER_SPEED;
                player1.facingX = -1; player1.facingY = 0;
                currentDirection = LEFT;
            currentTexture = textureLeft;
            }
            //di chuyển sang phải
            if (keys[SDL_SCANCODE_D] && !gameMap.checkCollision(player1.x + PLAYER_SPEED, player1.y,PLAYER_SIZE)) {
                player1.x += PLAYER_SPEED;
                player1.facingX = 1; player1.facingY = 0;
                currentDirection = RIGHT;
            currentTexture = textureRight;
            }
      }
        if(this == &player2){
                // di chuyển lên
            if (keys[SDL_SCANCODE_UP] && !gameMap.checkCollision(player2.x, player2.y - PLAYER_SPEED,PLAYER_SIZE)) {
                player2.y -= PLAYER_SPEED;
                player2.facingX = 0; player2.facingY = -1;
                currentDirection = UP;
            currentTexture = textureUp;
            }
        // di chuyển xuống
            if (keys[SDL_SCANCODE_DOWN] && !gameMap.checkCollision(player2.x, player2.y + PLAYER_SPEED,PLAYER_SIZE)) {
                player2.y += PLAYER_SPEED;
                player2.facingX = 0; player2.facingY = 1;
           currentDirection = DOWN;
            currentTexture = textureDown;
            }
            // di chuyển sang trái
            if (keys[SDL_SCANCODE_LEFT] && !gameMap.checkCollision(player2.x - PLAYER_SPEED, player2.y,PLAYER_SIZE)) {
                player2.x -= PLAYER_SPEED;
                player2.facingX = -1; player2.facingY = 0;
                currentDirection = LEFT;
            currentTexture = textureLeft;
            }
            // di chuyển sang phải
            if (keys[SDL_SCANCODE_RIGHT] && !gameMap.checkCollision(player2.x + PLAYER_SPEED, player2.y,PLAYER_SIZE)) {
                player2.x += PLAYER_SPEED;
                player2.facingX = 1; player2.facingY = 0;
                currentDirection = RIGHT;
            currentTexture = textureRight;
            }
        }
        // tính góc xoay nhân vật
         if (facingX != 0 || facingY != 0) {
            dx = facingX;
            dy = facingY ;
            angle = atan2(dx, dy) * 180 / M_PI + 90;
        }

        // ktra va chạm ngoài map
        if (player1.x < 0) player1.x = 0;
        if (player1.x > SCREEN_WIDTH - PLAYER_SIZE ) player1.x = SCREEN_WIDTH - PLAYER_SIZE;
        if (player1.y < 0) player1.y = 0;
        if (player1.y > SCREEN_HEIGHT - PLAYER_SIZE ) player1.y = SCREEN_HEIGHT - PLAYER_SIZE ;

        if (player2.x < 0) player2.x = 0;
        if (player2.x > SCREEN_WIDTH - PLAYER_SIZE ) player2.x = SCREEN_WIDTH - PLAYER_SIZE;
        if (player2.y < 0) player2.y = 0;
        if (player2.y > SCREEN_HEIGHT - PLAYER_SIZE ) player2.y = SCREEN_HEIGHT - PLAYER_SIZE ;
}
// hàm load ảnh người chơi
void Player::loadPlayerTextures(SDL_Renderer* renderer, const char* upPath, const char* downPath, const char* leftPath, const char* rightPath) {
    SDL_Surface* surface;
    surface = IMG_Load(upPath);
    if (!surface) {
        printf("Không thể load ảnh %s! SDL_image Error: %s\n", upPath, IMG_GetError());
        return;
    }
    textureUp = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = IMG_Load(downPath);
    if (!surface) {
        printf("Không thể load ảnh %s! SDL_image Error: %s\n", downPath, IMG_GetError());
        return;
    }
    textureDown = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(leftPath);
    if (!surface) {
        printf("Không thể load ảnh %s! SDL_image Error: %s\n", leftPath, IMG_GetError());
        return;
    }
    textureLeft = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = IMG_Load(rightPath);
    if (!surface) {
        printf("Không thể load ảnh %s! SDL_image Error: %s\n", rightPath, IMG_GetError());
        return;
    }
    textureRight = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    currentTexture = textureDown;
    currentDirection = DOWN;
}
// hàm vẽ người chơi
void Player::render(SDL_Renderer* renderer) {
        SDL_Rect playerRect = {static_cast<int> (x) , static_cast<int> (y), PLAYER_SIZE , PLAYER_SIZE};
        SDL_RenderCopy(renderer, currentTexture, nullptr, &playerRect);
}
// hàm kiểm tra load ảnh trái tim ( mạng sống )
bool Player::loadHeartTexture(SDL_Renderer* renderer) {
        heartTexture = IMG_LoadTexture(renderer, "image/heart.png");
        if (!heartTexture) {
            cout << "Failed to load heart image! SDL_Error: " << SDL_GetError() << endl;
            return false;
        }
        return true;
}
// hàm vẽ điểm người chơi
void Player::renderScore(SDL_Renderer* renderer, TTF_Font* font, int x, int y , int score , string ID) {
        if (!font) return;

        SDL_Color white = {255, 255, 255}; // Màu trắng
        string scoreText = "Score" + ID + " : " + to_string(score);

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        if (!textSurface) return;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
}
// hàm vẽ thời gian đếm ngược
void Player::renderCountdown(SDL_Renderer* renderer, TTF_Font* font, Uint32 gameStartTime, int gameDuration, int player1ScoreX, int player2ScoreX , int pauseTime) {
        Uint32 currentTime = SDL_GetTicks(); // lấy thời gian hiện tại
        Uint32 elapsedTime = currentTime - gameStartTime - pauseTime; // thời gian đã trôi qua
        int timeLeftMs = gameDuration - elapsedTime;// thời gian còn lại
        if (timeLeftMs < 0) timeLeftMs = 0;

        int totalSeconds = timeLeftMs / 1000;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        stringstream ss;
        ss << "Time: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds;

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, ss.str().c_str(), textColor);
        if (!surface) {
            std::cout << "Không thể tạo surface cho đồng hồ: " << TTF_GetError() << std::endl;
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cout << "Không thể tạo texture cho đồng hồ: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }

        int textW = surface->w;
        int textH = surface->h;
        int midX = (player1ScoreX + player2ScoreX) / 2 - textW / 2;
        int midY = 10;

        SDL_Rect destRect = {midX, midY, textW, textH};

        SDL_RenderCopy(renderer, texture, NULL, &destRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
}
// hàm vẽ heart + vẽ điểm
void Player::renderHearts(SDL_Renderer* renderer, int playerID , TTF_Font* font, int score) {
         if (!heartTexture) return;
        int heartSize = 15;
        int spacing = 5;
        int startX, startY = 10;
        int scoreX, scoreY = 10;

        if (playerID == 1) {
            startX = 0;
            scoreX = startX + 110;
        } else {
            startX = SCREEN_WIDTH - (100) - 0;
            scoreX = startX - 110;
        }
        for (int i = 0; i < live; i++) {
            SDL_Rect heartRect = {startX + i * (heartSize + spacing), startY, heartSize, heartSize};
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect);
        }
        renderScore(renderer, font, scoreX, scoreY, score , to_string(playerID)); // vẽ điểm người chơi

}
//hàm randon vị trí người chơi
void Player::randomizePlayerPosition(Player& otherPlayer) {
        srand(static_cast<unsigned int>(time(nullptr)));

        int row, col;
        bool valid;
        do {
            row = rand() % (MAP_ROWS - 2);
            col = rand() % (MAP_COLS - 2);
            float newX = col * TILE_SIZE;
            float newY = row * TILE_SIZE;
            valid = (mapData[row][col] == 0) &&
                    (fabs(newX - otherPlayer.x) >= 2 * TILE_SIZE) &&
                    (fabs(newY - otherPlayer.y) >= 2 * TILE_SIZE);
        } while (!valid);

        x = col * TILE_SIZE;
        y = row * TILE_SIZE;

}
// hàm kiểm tra người chơi bị bắn
bool Player::PlayerIsHit(float BulletX , float BulletY){
        return (BulletX >= x && BulletX <= x + PLAYER_SIZE && BulletY >= y && BulletY <= y + PLAYER_SIZE);
}
void Player::renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y) {
        if (!font) return;

        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
        if (!textSurface) {
            printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return;
        }

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
}


