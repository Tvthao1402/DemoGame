#define SDL_MAIN_HANDLED
#include<SDL.h>
#include"Player.h"
#include"Map.h"
#include"Bullet.h"
#include<vector>
#include<cmath>
#include<algorithm>
#include<iostream>
#include"Zombie.h"
#include<SDL_image.h>
#include <cstdio>
#include"Menu.h"
#include<SDL_ttf.h>
#include<string>
#include"Button.h"
#include<SDL_mixer.h>

using namespace std;
int main() {
        srand(time(nullptr));
       //SDL_Init(SDL_INIT_VIDEO);
       //IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return -1;
        }

        if (IMG_Init(IMG_INIT_PNG) == 0) {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return -1;
        }
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
        }


        SDL_Window* window = SDL_CreateWindow("Zombie Survival", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        Mix_Chunk* shootSound = nullptr;
        Mix_Music* backgroundMusic = nullptr;

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            printf("SDL_mixer could not initialize! Mix_Error: %s\n", Mix_GetError());
            SDL_Quit();
            return -1;
        }

       shootSound = Mix_LoadWAV("sound/shooted.wav");
       backgroundMusic = Mix_LoadMUS("sound/sound_background.mp3");
      if(!shootSound){
         cout << "loi sound" << endl;
        return -1;
     }
       if (!shootSound|| !backgroundMusic) {
           printf("One or more sound files failed to load. Exiting...\n");
            Mix_CloseAudio();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
             SDL_Quit();
             return -1;
        }

        Menu menu(renderer);
        bool menuRunning = true;
        SDL_Event event;

         Mix_PlayMusic(backgroundMusic, -1);

        while (menuRunning) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    menuRunning = false;
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                }

                int action = menu.handleEvent(event);
                if (action == 1) { // Nếu chọn "Play", thoát vòng lặp menu
                    menuRunning = false;
                }
                if (action == 2) {
                    cout << "Open setting" << endl;
                }
              }
                 menu.render();
        }

        Player player1 , player2;

        player1.randomizePlayerPosition(player2);
        player2.randomizePlayerPosition(player1);

        if (!player1.loadHeartTexture(renderer) || !player2.loadHeartTexture(renderer)) {
            cout << "Không thể load hình trái tim!" << endl;
            return -1;
        }

        Uint32 lastSpawnTime = SDL_GetTicks(); // Thời gian spawn zombie cuối cùng
         const int spawnInterval = 20000; // 2 giây (2000 ms)
         int maxZombies = 5; // Số lượng zom

         const int GAME_DURATION = 150000; // 120000 ms = 2 phút
         Uint32 gameStartTime = SDL_GetTicks();
         Uint32 pauseStartTime = 0;// Thời gian khi game bị Pause
         Uint32 pausedTime = 0;

        bool running = true;
        bool gameOver = false;

        vector<Bullet> bullet1;
        vector<Bullet> bullet2;
        vector<Zombie> zombies;


        spawnZombies(zombies, maxZombies, player1, player2, renderer);
        player1.loadPlayerTextures(renderer, "image/player1_up.png", "image/player1_down.png", "image/player1_left.png", "image/player1_right.png");
        player2.loadPlayerTextures(renderer, "image/player2_up.png", "image/player2_down.png", "image/player2_left.png", "image/player2_right.png");

        TTF_Font* font_score = TTF_OpenFont("Roboto-Black.ttf", 20);
        if (!font_score) {
        cout << "Loi khi load font: " << TTF_GetError() << endl;
        return -1;
        }

        TTF_Font * font_screen = TTF_OpenFont("Roboto-Black.ttf", 50);

        Uint32 lastShotTime1 = 0; // Thời gian bắn cuối cùng của player1
        Uint32 lastShotTime2 = 0; // Thời gian bắn cuối cùng của player2
        const int SHOOT_DELAY = 500;
        string winMessage = "";

        ButtonHandler buttonHandler(renderer);
         bool restartGame = false;
         bool isPaused = false;
         const int FRAME_DELAY = 1000 / 60;

        while (running || gameOver) {
                Uint32 frameStart = SDL_GetTicks();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        running = false;
                        gameOver = false;
                }
                if(running){
                    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                        isPaused = !isPaused;
                         if (isPaused) {
                            pauseStartTime = SDL_GetTicks();
                            Mix_PauseMusic();
                         } else {
                             pausedTime += SDL_GetTicks() - pauseStartTime;
                             Mix_ResumeMusic(); // Cộng dồn thời gian đã pause
                         }
                    }

                if(!isPaused){
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastShotTime1 >= SHOOT_DELAY) { // Kiểm tra thời gian giữa các lần bắn
                        float centerX = player1.x + PLAYER_SIZE / 2;
                        float centerY = player1.y + PLAYER_SIZE / 2;
                        Bullet newBullet(centerX, centerY, player1.facingX, player1.facingY);
                        if (newBullet.loadBulletTextures(renderer,
                        "image/bullet_up.png",
                           "image/bullet_down.png",
                        "image/bullet_left.png",
                        "image/bullet_right.png")) {
                           bullet1.emplace_back(std::move(newBullet));
                          lastShotTime1 = currentTime;
                          Mix_PlayChannel(-1, shootSound, 0);
                        }

                    }

                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                        Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastShotTime2 >= SHOOT_DELAY) { // Kiểm tra thời gian giữa các lần bắn
                        float centerX = player2.x + PLAYER_SIZE / 2;
                        float centerY = player2.y + PLAYER_SIZE / 2;
                        Bullet newBullet(centerX, centerY, player2.facingX, player2.facingY);
                        if (newBullet.loadBulletTextures(renderer,
                        "image/bullet_up.png",
                           "image/bullet_down.png",
                        "image/bullet_left.png",
                        "image/bullet_right.png")) {
                        bullet2.emplace_back(std::move(newBullet));
                          lastShotTime2 = currentTime;
                          Mix_PlayChannel(-1, shootSound, 0);
                      }
                    }
                }
            }  else {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if (buttonHandler.checkRestartClick(mouseX, mouseY)) { // Tái sử dụng "Restart" làm "Resume"
                        isPaused = false;
                        pausedTime += SDL_GetTicks() - pauseStartTime; // Cộng dồn thời gian pause
                        Mix_ResumeMusic();
                    }
                    if (buttonHandler.checkQuitClick(mouseX, mouseY)) {
                        running = false;
                        gameOver = false;
                    }
                }
            }
        }
            if (gameOver) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    if (buttonHandler.checkRestartClick(mouseX, mouseY)) {
                        restartGame = true;
                        gameOver = false;
                        Mix_PlayMusic(backgroundMusic, -1);
                    }
                    if (buttonHandler.checkQuitClick(mouseX, mouseY)) {
                        running = false;
                        gameOver = false;
                    }
                }
            }
        }

            const Uint8* keys = SDL_GetKeyboardState(NULL);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer); // xóa màn hình trc khi vẽ frame mới
            gameMap.renderMap(renderer);


            if(running ){
                if(!isPaused){
                     player1.movePlayer(keys, player1 , player2 );
                     player2.movePlayer(keys, player1, player2);

                     gameMap.loadTextures(renderer);
                    gameMap.generateObstacles();

                    Uint32 elapsedTime = SDL_GetTicks() - gameStartTime - pausedTime;;
                if (!isPaused) {
                    elapsedTime = SDL_GetTicks() - gameStartTime - pausedTime;
                 }
                if (elapsedTime >= GAME_DURATION) {
                if (player1.score > player2.score) {
                          winMessage = "Player 1 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                 } else if (player2.score > player1.score) {
                    winMessage = "Player 2 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                 } else {
                     if(player1.live > player2.live){
                        winMessage = "Player 1 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                     }
                     else if(player2.live > player1.live){
                        winMessage = "Player 2 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                     }
                     else {
                            winMessage = "Both players tied!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                            Mix_HaltMusic();
                     }
                 }
               running = false;
            }
            player1.render(renderer);
            player2.render(renderer);// vẽ nhân vật
            for (auto& zombie : zombies) {
                zombie.moveRandomly(zombies);
                zombie.render(renderer);
           }
            for (auto &bullet : bullet1) {
                    bullet.update(gameMap);
                       bullet.render(renderer);
            }
            for (auto &bullet : bullet2) {
                bullet.update(gameMap);
              bullet.render(renderer);

            }
    // Kiểm tra va chạm đạn - zombie
           for (auto& bullet : bullet1) {
              for (auto& zombie : zombies) {
                  if (zombie.ZombieIsHit(bullet.x, bullet.y)) {

                       bullet.active = false;
                      zombie.alive = false;
                       player1.score++; // Player 1 bắn trúng zombie
                 }
              }
           }
           zombies.erase( remove_if(zombies.begin(), zombies.end(), [](const Zombie &b) { return !b.alive; }),zombies.end());
           for (auto it = zombies.begin(); it != zombies.end();) {
               if (!it->alive) { // Dùng `it->alive` thay vì `(*it)->alive`
                  it = zombies.erase(it); // Không cần delete vì Zombie không phải con trỏ
                    } else {
                         ++it;
               }
           }
           for (auto& bullet : bullet2) {
              for (auto& zombie : zombies) {
                  if (zombie.ZombieIsHit(bullet.x, bullet.y)) {

                      bullet.active = false;
                      zombie.alive = false;
                      player2.score++; // Player 2 bắn trúng zombie
               }
             }
           }
    // Kiểm tra va chạm đạn - người chơi
            for (auto& bullet : bullet1) {
                if (player2.PlayerIsHit(bullet.x, bullet.y)) {
                    bullet.active = false;
                     player2.live--; // Player 2 mất 1 mạng
                     if(player2.live == 0) {
                        winMessage = "Player 1 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                            Mix_HaltMusic();
                     }
                }
            }
            for (auto& bullet : bullet2) {
              if (player1.PlayerIsHit(bullet.x, bullet.y)) {

                  bullet.active = false;
                  player1.live--; // Player 1 mất 1 mạng
                  if(player1.live == 0) {
                       winMessage = "Player 2 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                            Mix_HaltMusic();
                     }
               }
            }
            for (auto& z : zombies) {
                 if (z.checkZombieHitPlayer(z, player1)) {
                     player1.live--;
                     if(player1.live == 0){
                        winMessage = "Player 2 Wins!";
                            running = false;
                            gameOver = true;
                            gameStartTime = SDL_GetTicks();
                            Mix_HaltMusic();
                     }
                    player1.randomizePlayerPosition(player2);
               }
                if (z.checkZombieHitPlayer(z,player2)) {
                      player2.live--;
                      if(player2.live == 0){
                        winMessage = "Player 1 Wins!";
                        running = false;
                        gameOver = true;
                        gameStartTime = SDL_GetTicks();
                        Mix_HaltMusic();
                     }
                      player2.randomizePlayerPosition(player1);
                }
             }

             Uint32 currentTime = SDL_GetTicks();
             if (currentTime - lastSpawnTime >= spawnInterval) {
                 spawnZombies(zombies, 1, player1, player2 , renderer); // Spawn thêm 1 zombie mỗi 2 giây
                 lastSpawnTime = currentTime; // Cập nhật lại thời gian spawn
             }


           int player1ScoreX = 380; // Tọa độ x của "Score1"
           int player2ScoreX = 200; // Tọa độ x của "Score2"

            player1.renderHearts(renderer, 1 , font_score , player1.score);// ve trai tim
            player2.renderHearts(renderer, 2 , font_score , player2.score);

             player1.renderCountdown(renderer, font_score, gameStartTime, GAME_DURATION, player1ScoreX, player2ScoreX , pausedTime);
             bullet1.erase( remove_if(bullet1.begin(), bullet1.end(), [](const Bullet &b) { return !b.active; }),bullet1.end());
             bullet2.erase( remove_if(bullet2.begin(), bullet2.end(), [](const Bullet &b) { return !b.active; }),bullet2.end());
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                buttonHandler.render(renderer, true, false);
            }
         }


             if (gameOver) { // Màn hình chiến thắng
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nền đen
                SDL_RenderClear(renderer);
                // Vẽ thông báo chiến thắng
                player1.renderText(renderer, font_screen, winMessage, SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 200);
                buttonHandler.render(renderer,false , true);
                SDL_RenderPresent(renderer);
            }
            if (restartGame) {
            player1.randomizePlayerPosition(player2);
            player2.randomizePlayerPosition(player1);
            bullet1.clear();
            bullet2.clear();
            zombies.clear();
            spawnZombies(zombies, maxZombies, player1, player2, renderer);
            gameStartTime = SDL_GetTicks();
            lastSpawnTime = SDL_GetTicks();
            pausedTime = 0;
            player1.score = 0;
            player2.score = 0;
            player1.live = 5;
            player2.live = 5;
            running = true;
            restartGame = false;
            Mix_PlayMusic(backgroundMusic, -1);
        }

            SDL_RenderPresent(renderer);
            Uint32 frameTime = SDL_GetTicks() - frameStart;
            if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
            }
        }
        Mix_FreeChunk(shootSound);
        Mix_FreeMusic(backgroundMusic);
        Mix_CloseAudio();
        gameMap.cleanupTextures();
        TTF_CloseFont(font_score);
        TTF_CloseFont(font_screen);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 0;
}

