#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include"Map.h"
const int BULLET_SIZE = 10;
const float BULLET_SPEED = 7;
 enum BulletDirection { BULLET_UP, BULLET_DOWN, BULLET_LEFT, BULLET_RIGHT };
class Bullet {
public:
    float x, y, dx, dy;
    bool active = true;
    SDL_Texture* textureBulletUp;
    SDL_Texture* textureBulletDown;
    SDL_Texture* textureBulletLeft;
    SDL_Texture* textureBulletRight;
    SDL_Texture* currentBulletTexture;

    BulletDirection currentDirectionBullet;
    Bullet(float x, float y, float dx, float dy);
    ~Bullet();
    Bullet(const Bullet&) = delete;
    Bullet& operator=(const Bullet&) = delete;
s
    Bullet(Bullet&& other) noexcept;
    Bullet& operator=(Bullet&& other) noexcept;

    bool loadBulletTextures(SDL_Renderer* renderer, const char* pathUp, const char* pathDown,
                               const char* pathLeft, const char* pathRight);
    void render(SDL_Renderer* renderer);

    void update( Map& gameMap);

};
#endif
