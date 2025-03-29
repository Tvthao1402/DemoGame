#include"Player.h"
#include"Map.h"
#include"Bullet.h"
#include<vector>
#include<ctime>
#include<stdlib.h>
#ifndef ZOMBIE_H
#define ZOMBIE_H
#include<SDL.h>
#include <memory>
using namespace std;
const int ZOMBIE_SIZE = 30;
const float ZOMBIE_SPEED = 1;
class Zombie {
public:
    float x, y;
    bool alive;
    SDL_Texture* texture;

    Zombie(float x, float y);
    ~Zombie();
    Zombie(const Zombie&) = delete;
    Zombie& operator=(const Zombie&) = delete;

    // Cho phép di chuyển
    Zombie(Zombie&& other) noexcept;
    Zombie& operator=(Zombie&& other) noexcept;

    bool loadZombieTexture(SDL_Renderer * renderer , const char *filePath);

    void moveRandomly(const vector<Zombie> &zombies );
    void render(SDL_Renderer* renderer);
    bool ZombieIsHit(float bulletX, float bulletY);
    bool checkZombieHitPlayer(const Zombie& zombie, Player& player);
};

void spawnZombies(vector<Zombie>& zombies, int numZombies, const Player& player1, const Player& player2, SDL_Renderer * renderer);
#endif
