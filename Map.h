#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include<SDL_image.h>
#include<cstdlib>
#include<cmath>
#include<memory>
const int SCREEN_HEIGHT = 800;// chiều cao map
const int SCREEN_WIDTH = 576; // chiều rộng
const int TILE_SIZE = 32; // kích thước các ô
const int MAP_ROWS = SCREEN_HEIGHT/TILE_SIZE;
const int MAP_COLS = SCREEN_WIDTH/TILE_SIZE;
const int OBSTACLE_COUNT = 3; // số lượng chướng ngại vật
extern SDL_Texture* obstacleTextures[OBSTACLE_COUNT];
extern int mapData[MAP_ROWS][MAP_COLS];

class Map{
public:
    Map();
    ~Map();
     Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;
void generateObstacles();

bool loadTextures(SDL_Renderer * renderer);
void renderMap(SDL_Renderer* renderer );
bool checkCollision(float newX, float newY, int sizee);
bool isBulletColliding(float x, float y);
void cleanupTextures();
};
extern Map gameMap;
#endif
