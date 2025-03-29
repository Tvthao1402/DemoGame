#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include<SDL_image.h>
#include<vector>
#include"Bullet.h"
#include<SDL_ttf.h>
#include<string>
using namespace std;
const int PLAYER_SIZE = 30;
const float PLAYER_SPEED = 3;
enum Direction { UP, DOWN, LEFT, RIGHT };
class Player{
public:
    float x , y;

    int score = 0;
    int live = 5;
    float facingX, facingY; // Hướng cuối cùng nhân vật đối mặt
    double angle;         // Góc xoay của texture (đơn vị: độ
    Player();
    ~Player();

    void renderScore(SDL_Renderer* renderer, TTF_Font* font, int x, int y ,int score, string ID);
    void renderText(SDL_Renderer* renderer, TTF_Font* font, const string& text, int x, int y );

    void renderCountdown(SDL_Renderer* renderer, TTF_Font* font, Uint32 gameStartTime, int gameDuration, int player1ScoreX, int player2ScoreX , int pauseTime);

    SDL_Texture* heartTexture; // Ảnh trái tim
    SDL_Texture* textureUp;
    SDL_Texture* textureDown;
    SDL_Texture* textureLeft;
    SDL_Texture* textureRight;
    SDL_Texture* currentTexture;
    Direction currentDirection;

    bool loadHeartTexture(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer);
    void renderHearts(SDL_Renderer* renderer, int playerID ,TTF_Font* font , int score);
    void movePlayer( const Uint8* keys , Player& player1 , Player& player2);
    void randomizePlayerPosition(Player& otherPlayer);
    bool PlayerIsHit(float Bullet1 , float Bullet2);
    SDL_Texture* playerTexture;
    void loadPlayerTextures(SDL_Renderer* renderer, const char* upPath, const char* downPath, const char* leftPath, const char* rightPath);
};
#endif
