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
void Map::generateObstacles() {
    srand(time(0)); // Khởi tạo random
    for (int row = 0; row < MAP_ROWS ; row++) {
        for (int col = 0; col < MAP_COLS ; col++) {
            if (mapData[row][col] == 1  && (row != 1 || col != 1 || row != MAP_COLS - 1 || col != MAP_ROWS - 1)) {
                mapData[row][col] = rand() % OBSTACLE_COUNT + 2; // Random từ 2 đến 4

            }
        }
    }
}
SDL_Texture* obstacleTextures[OBSTACLE_COUNT] = { nullptr, nullptr, nullptr };
void Map::renderMap(SDL_Renderer* renderer) {
    for (int row = 0; row < MAP_ROWS; row++) {
        for (int col = 0; col < MAP_COLS; col++) {
            SDL_Rect tile = { col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            if (mapData[row][col] == 0) {
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Màu nền
                SDL_RenderFillRect(renderer, &tile);
            } else {
                int obstacleType = mapData[row][col];

                SDL_Texture* texture = nullptr;
                if (obstacleType == 2) texture = obstacleTextures[0]; // Xe hỏng
                if (obstacleType == 3) texture = obstacleTextures[1]; // Thùng gỗ
                if (obstacleType == 4) texture = obstacleTextures[2]; // Cây

                if (texture) {
                    //SDL_RenderCopy(renderer, texture, NULL, &tile);
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Màu xám
                    SDL_RenderFillRect(renderer, &tile); // Vẽ nền
                    SDL_RenderCopy(renderer, texture, NULL, &tile); // Vẽ texture lên trê
                } else {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Màu xám cho tường
                    SDL_RenderFillRect(renderer, &tile);
                }
            }
        }
}

    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Màu nâu
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
        obstacleTextures[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);

        SDL_FreeSurface(tempSurface);
    }
    return true;
}
bool Map::checkCollision(float newX, float newY, int PLAYER_SIZE) {
    int left   = static_cast<int>(newX) / TILE_SIZE;
    int right  = static_cast<int>(newX + PLAYER_SIZE - 1) / TILE_SIZE;
    int top    = static_cast<int>(newY) / TILE_SIZE;
    int bottom = static_cast<int>(newY + PLAYER_SIZE - 1) / TILE_SIZE;

    // Kiểm tra nếu vị trí nằm ngoài map
    if (left < 1 || right >= MAP_COLS - 1|| top < 1 || bottom >= MAP_ROWS -1) {
        return true; // Va chạm với tường ngoài biên
    }

    int obstacles[] = {1, 2, 3, 4}; // Các vật cản không thể đi qua
    for (int i = 0; i < 4; i++) {
        if (mapData[top][left] == obstacles[i] || mapData[top][right] == obstacles[i] ||
            mapData[bottom][left] == obstacles[i] || mapData[bottom][right] == obstacles[i]) {
            return true; // Va chạm với vật cản
        }
    }

    return false; // Không có va chạm
}
bool Map::isBulletColliding(float x, float y) {
    int left   = (int)x / TILE_SIZE;
    int right  = (int)(x + BULLET_SIZE - 1) / TILE_SIZE;
    int top    = (int)y / TILE_SIZE;
    int bottom = (int)(y + BULLET_SIZE - 1) / TILE_SIZE;

    // Kiểm tra nếu vị trí nằm ngoài bản đồ
    if (left < 1 || right >= MAP_COLS - 1|| top < 1 || bottom >= MAP_ROWS -1) {
        return true; // Va chạm với biên ngoài
    }

    // Kiểm tra nếu bất kỳ góc nào của viên đạn nằm trong tường
    if (mapData[top][left] != 0 || mapData[top][right] != 0 ||
        mapData[bottom][left] != 0 || mapData[bottom][right] != 0) {
        return true; // Đạn đã chạm tường
    }

    return false; // Không có va chạm
}
void Map::cleanupTextures() {
    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        if (obstacleTextures[i]) {
            SDL_DestroyTexture(obstacleTextures[i]);
            obstacleTextures[i] = nullptr;
        }
    }
}
