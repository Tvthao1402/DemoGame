#include"Player.h"
#include "Map.h"
#include"Bullet.h"
#include<iostream>
using namespace std;

void Bullet::update( Map& gameMap) {
    if(!active) return;
    x += dx * BULLET_SPEED ;
    y += dy * BULLET_SPEED ;
    if(gameMap.isBulletColliding(x,y)){
        active = false;
    }

}
Bullet::Bullet(float x, float y, float dx, float dy) : x(x), y(y), dx(dx), dy(dy), active(true)
,textureBulletUp(nullptr), textureBulletDown(nullptr),
      textureBulletLeft(nullptr), textureBulletRight(nullptr),
      currentBulletTexture(nullptr), currentDirectionBullet(BULLET_DOWN) {
    if (dx > 0 && dy == 0) {
        currentDirectionBullet = BULLET_RIGHT;
        currentBulletTexture = textureBulletRight;
    } else if (dx < 0 && dy == 0) {
        currentDirectionBullet = BULLET_LEFT;
        currentBulletTexture = textureBulletLeft;
    } else if (dy > 0 && dx == 0) {
        currentDirectionBullet = BULLET_DOWN;
        currentBulletTexture = textureBulletDown;
    } else if (dy < 0 && dx == 0) {
        currentDirectionBullet = BULLET_UP;
        currentBulletTexture = textureBulletUp;
    }
}
Bullet::~Bullet() {
    if (textureBulletUp) SDL_DestroyTexture(textureBulletUp);
    if (textureBulletDown) SDL_DestroyTexture(textureBulletDown);
    if (textureBulletLeft) SDL_DestroyTexture(textureBulletLeft);
    if (textureBulletRight) SDL_DestroyTexture(textureBulletRight);
}
Bullet::Bullet(Bullet&& other) noexcept
    : x(other.x), y(other.y), dx(other.dx), dy(other.dy),
      active(other.active),
      textureBulletUp(other.textureBulletUp),
      textureBulletDown(other.textureBulletDown),
      textureBulletLeft(other.textureBulletLeft),
      textureBulletRight(other.textureBulletRight),
      currentBulletTexture(other.currentBulletTexture),
      currentDirectionBullet(other.currentDirectionBullet) {
    other.textureBulletUp = nullptr;
    other.textureBulletDown = nullptr;
    other.textureBulletLeft = nullptr;
    other.textureBulletRight = nullptr;
    other.currentBulletTexture = nullptr;
}

Bullet& Bullet::operator=(Bullet&& other) noexcept {
    if (this != &other) {
        if (textureBulletUp) SDL_DestroyTexture(textureBulletUp);
        if (textureBulletDown) SDL_DestroyTexture(textureBulletDown);
        if (textureBulletLeft) SDL_DestroyTexture(textureBulletLeft);
        if (textureBulletRight) SDL_DestroyTexture(textureBulletRight);

        x = other.x;
        y = other.y;
        dx = other.dx;
        dy = other.dy;
        active = other.active;
        textureBulletUp = other.textureBulletUp;
        textureBulletDown = other.textureBulletDown;
        textureBulletLeft = other.textureBulletLeft;
        textureBulletRight = other.textureBulletRight;
        currentBulletTexture = other.currentBulletTexture;
        currentDirectionBullet = other.currentDirectionBullet;

        other.textureBulletUp = nullptr;
        other.textureBulletDown = nullptr;
        other.textureBulletLeft = nullptr;
        other.textureBulletRight = nullptr;
        other.currentBulletTexture = nullptr;
    }
    return *this;
}

bool Bullet::loadBulletTextures(SDL_Renderer* renderer, const char* upPath, const char* downPath, const char* leftPath, const char* rightPath) {
    SDL_Surface* surface;

    surface = IMG_Load(upPath);
    if (!surface) return false;
    textureBulletUp = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(downPath);
    if (!surface) return false;
    textureBulletDown = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(leftPath);
    if (!surface) return false;
    textureBulletLeft = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load(rightPath);
    if (!surface) return false;
    textureBulletRight = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    switch (currentDirectionBullet) {
        case BULLET_UP:
            currentBulletTexture = textureBulletUp;
            break;
        case BULLET_DOWN:
            currentBulletTexture = textureBulletDown;
            break;
        case BULLET_LEFT:
            currentBulletTexture = textureBulletLeft;
            break;
        case BULLET_RIGHT:
            currentBulletTexture = textureBulletRight;
            break;
    }

    return true;
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!active || !currentBulletTexture) return;
        SDL_Rect playerRect = {static_cast<int> (x) , static_cast<int> (y), BULLET_SIZE , BULLET_SIZE};
        SDL_RenderCopy(renderer, currentBulletTexture, nullptr, &playerRect);
    }


