#include"Zombie.h"
#include<iostream>
using namespace std;
Zombie::Zombie(float x, float y) : x(x), y(y), alive(true), texture(nullptr) {}
Zombie::~Zombie() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}
//sao chép dữ liệu từ other rồi để other là null , đỡ tốn bộ nhờ
Zombie::Zombie(Zombie&& other) noexcept
    : x(other.x), y(other.y), alive(other.alive), texture(other.texture) {
    other.texture = nullptr;
}
// tránh  rò rỉ bộ nhớ
Zombie& Zombie::operator=(Zombie&& other) noexcept {
    if (this != &other) {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
        x = other.x;
        y = other.y;
        alive = other.alive;
        texture = other.texture;
        other.texture = nullptr;// giải phóng texture cũ
    }
    return *this;
}
// hàm di chuyển zombie ngẫu nhiên
void Zombie::moveRandomly(const vector<Zombie> & zombies ) {
    if (!alive) return;
    int direction = rand() % 4;// random 4 hướng
    float newX = x, newY = y;

    switch (direction) {
        case 0: newY -= ZOMBIE_SPEED; break;
        case 1: newY += ZOMBIE_SPEED; break;
        case 2: newX -= ZOMBIE_SPEED; break;
        case 3: newX += ZOMBIE_SPEED; break;
    }
    // ktra vị trí zombie hợp lệ trong map
      if (newX < 0 || newX + ZOMBIE_SIZE > MAP_COLS * TILE_SIZE ||
        newY < 0 || newY + ZOMBIE_SIZE > MAP_ROWS * TILE_SIZE) {
        return;
    }
    // kiểm tra va chạm tường
    if (!gameMap.checkCollision(newX, newY, ZOMBIE_SIZE)) {
        bool canMove = true;
        for(const auto & z : zombies){
           if (&z != this && abs(newX - z.x) < ZOMBIE_SIZE && abs(newY - z.y) < ZOMBIE_SIZE) {
                canMove = false;
                break;
            }
    }
    if(canMove){
        x = newX;
        y = newY;
    }
}
}
// load ảnh zombie
bool Zombie::loadZombieTexture(SDL_Renderer * renderer , const char * filePath){
    SDL_Surface* surface = IMG_Load(filePath);
    if(!surface){
        cout << "Không thể load ảnh" << surface << endl;
        return false;
    }
   texture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);
    return texture != nullptr;
}
// vẽ zombie
void Zombie::render(SDL_Renderer* renderer) {
    if (!alive || !texture) return;
    SDL_Rect zombieRect = {static_cast<int>(x), static_cast<int>(y), ZOMBIE_SIZE, ZOMBIE_SIZE};
    SDL_RenderCopy(renderer ,texture , NULL , &zombieRect);
}
// ktra zombie có trúng đạn ko
bool Zombie::ZombieIsHit(float bulletX, float bulletY) {
    return alive && (bulletX >= x && bulletX <= x + ZOMBIE_SIZE && bulletY >= y && bulletY <= y + ZOMBIE_SIZE);
}
// sinh zombie ngẫu nhiên
void spawnZombies(vector<Zombie>& zombies , int numZombies, const Player& player1, const Player& player2 , SDL_Renderer* renderer) {
    zombies.reserve(zombies.size() + numZombies);
    for (int i = 0; i < numZombies; i++) {
        int row, col;
        float newX, newY;
        bool valid;

        do {
            row = rand() % (MAP_ROWS - 2);
            col = rand() % (MAP_COLS - 2);
            newX = col * TILE_SIZE;
            newY = row * TILE_SIZE;
            valid = (mapData[row][col] == 0) &&
                    (abs(newX - player1.x) > 100 || abs(newY - player1.y) > 100) &&
                    (abs(newX - player2.x) > 100 || abs(newY - player2.y) > 100);
        for (const auto& z : zombies) {
                if (abs(newX - z.x) < ZOMBIE_SIZE && abs(newY - z.y) < ZOMBIE_SIZE) {
                    valid = false;
                    break;
                }
            }
        } while (!valid);
        zombies.emplace_back(newX, newY);//tránh việc tạo đối tượng tạm thời và sao chép/di chuyển.
        if (!zombies.back().loadZombieTexture(renderer, "image/zombie.png")) {
            zombies.pop_back();
        }
    }
}
// ktra va chạm vơi người chơi
bool Zombie::checkZombieHitPlayer(const Zombie& zombie, Player& player) {
    return (player.x < zombie.x + ZOMBIE_SIZE && player.x + PLAYER_SIZE > zombie.x &&
            player.y < zombie.y + ZOMBIE_SIZE && player.y + PLAYER_SIZE > zombie.y);
}

