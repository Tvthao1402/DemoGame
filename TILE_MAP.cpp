#include "Map.h"
#include"Player.h"
#include"Bullet.h"
#include<iostream>
using namespace std;
#define OBSTACLE_COUNT 3
Map gameMap;
Map::Map() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        obstacleTextures[i] = nullptr;
    }
}
int mapData[MAP_ROWS][MAP_COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,1,0,1,0,0,1,1,0,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,0,1},
    {1,0,1,0,0,0,1,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,0,0,0,1,0,0,1,1,0,1,0,0,0,1},
    {1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1},
    {1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,0,1,0,0,1,1,0,1,0,1,0,0,0,1},
    {1,0,1,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1},
    {1,0,0,0,0,1,1,0,0,1,0,1,0,0,0,0,0,1},
    {1,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0,1},
    {1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,0,0,0,1,0,0,1,0,0,1,0,1},
    {1,0,0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
    {1,0,1,0,1,0,0,1,0,0,0,1,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0},
    {1,0,0,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1}
};
Map::~Map() {
    cleanupTextures();
}
// hàm sinh ngẫu nhiên các vị trí 1 thành 2, 3 , 4 để vẽ các chướng ngại vật
void Map::generateObstacles() {
    srand(time(0));
    for (int row = 0; row < MAP_ROWS ; row++) {
        for (int col = 0; col < MAP_COLS ; col++) {
            if (mapData[row][col] == 1  && (row != 1 || col != 1 || row != MAP_COLS - 1 || col != MAP_ROWS - 1)) {
                mapData[row][col] = rand() % OBSTACLE_COUNT + 2;

            }
        }
    }
}
SDL_Texture* obstacleTextures[OBSTACLE_COUNT] = { nullptr, nullptr, nullptr };
// hàm vẽ các chướng nhân vật
void Map::renderMap(SDL_Renderer* renderer) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_Rect tile = { col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (mapData[row][col] == 0) {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);// màu xám
                SDL_RenderFillRect(renderer, &tile);
            } else {
                int obstacleType = mapData[row][col];

                SDL_Texture* texture = nullptr;
                if (obstacleType == 2) texture = obstacleTextures[0];// vẽ côt chỉ đường
                if (obstacleType == 3) texture = obstacleTextures[1];// vẽ thùng độc
                if (obstacleType == 4) texture = obstacleTextures[2];// vẽ mộ

                if (texture) {

                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);// màu xám
                    SDL_RenderFillRect(renderer, &tile);
                    SDL_RenderCopy(renderer, texture, NULL, &tile);
                } else {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);// màu nâu viền
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    for (int i = 0; i < MAP_COLS; i++) {
        SDL_Rect topBorder = { i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE };
        SDL_Rect bottomBorder = { i * TILE_SIZE, (MAP_ROWS - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &topBorder);
        SDL_RenderFillRect(renderer, &bottomBorder);
    }
    for (int i = 0; i < MAP_ROWS; i++) {
        SDL_Rect leftBorder = { 0, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_Rect rightBorder = { (MAP_COLS - 1) * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
        SDL_RenderFillRect(renderer, &leftBorder);
        SDL_RenderFillRect(renderer, &rightBorder);
    }
}
// load ảnh chướng ngại vật
bool Map::loadTextures(SDL_Renderer* renderer) {
    cleanupTextures();
    const char* textureFiles[3] = {"image/col.png", "image/box1.png", "image/tomb.png" };

    for (int i = 0; i <= 2; i++) {
        SDL_Surface* tempSurface = IMG_Load(textureFiles[i]);
        if (!tempSurface) {
            std::cout << "Failed to load " << textureFiles[i] << ": " << IMG_GetError() << "\n";
            cleanupTextures();
            return false;
        }
        obstacleTextures[i] = SDL_CreateTextureFromSurface(renderer, tempSurface); // chuyển surface sang texture

        SDL_FreeSurface(tempSurface);// giải phóng tempSurface
    }
    return true;
}
// hàm kiểm tra va chạm
bool Map::checkCollision(float newX, float newY, int PLAYER_SIZE) {
    int left   = static_cast<int>(newX) / TILE_SIZE;
    int right  = static_cast<int>(newX + PLAYER_SIZE - 1) / TILE_SIZE;
    int top    = static_cast<int>(newY) / TILE_SIZE;
    int bottom = static_cast<int>(newY + PLAYER_SIZE - 1) / TILE_SIZE;


    if (left < 1 || right >= MAP_COLS - 1|| top < 1 || bottom >= MAP_ROWS -1) {
        return true;
    }

    int obstacles[] = {1, 2, 3, 4};
    for (int i = 0; i < 4; i++) {
        if (mapData[top][left] == obstacles[i] || mapData[top][right] == obstacles[i] ||
            mapData[bottom][left] == obstacles[i] || mapData[bottom][right] == obstacles[i]) {
            return true;
        }
    }

    return false;
}
// kiêm tra va chạm với đạn
bool Map::isBulletColliding(float x, float y) {
    int left   = (int)x / TILE_SIZE;
    int right  = (int)(x + BULLET_SIZE - 1) / TILE_SIZE;
    int top    = (int)y / TILE_SIZE;
    int bottom = (int)(y + BULLET_SIZE - 1) / TILE_SIZE;

    if (left < 1 || right >= MAP_COLS - 1|| top < 1 || bottom >= MAP_ROWS -1) {
        return true;
    }
    if (mapData[top][left] != 0 || mapData[top][right] != 0 ||
        mapData[bottom][left] != 0 || mapData[bottom][right] != 0) {
        return true;
    }

    return false;
}
// xóa các texture ko dùng
void Map::cleanupTextures() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (obstacleTextures[i]) {
            SDL_DestroyTexture(obstacleTextures[i]);
            obstacleTextures[i] = nullptr;
        }
    }
}
