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
        bool showHowToPlay = false;
        SDL_Event event;

         Mix_PlayMusic(backgroundMusic, -1);
         ButtonHandler buttonHandler(renderer);
         Map gameMap;

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
                if (action == 1) { //
                    menuRunning = false;
                }
                if (action == 2) {
                    showHowToPlay = true;
                }
                if (showHowToPlay && event.type == SDL_MOUSEBUTTONDOWN) {
                      int mouseX, mouseY;
                      SDL_GetMouseState(&mouseX, &mouseY);
                      if (buttonHandler.checkBackClick(mouseX, mouseY)) {
                          showHowToPlay = false;
                      }
                 }
              }
            if (showHowToPlay) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                TTF_Font* titleFont = TTF_OpenFont("Roboto-Black.ttf", 50);

                TTF_Font* instructionFont = TTF_OpenFont("Roboto-Black.ttf", 20);

                if (!titleFont || !instructionFont) {
                  printf("Không thể load font: %s\n", TTF_GetError());
                } else {
                   SDL_Color textColor = {255, 255, 255, 255};
                   SDL_Surface* titleSurface = TTF_RenderText_Solid(titleFont, "How to Play", textColor);
                   SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
                   int titleWidth, titleHeight;
                   TTF_SizeText(titleFont, "How to Play", &titleWidth, &titleHeight);
                   SDL_Rect titleRect = {SCREEN_WIDTH / 2 - titleWidth / 2, 50, titleWidth, titleHeight};
                   SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

        const char* instructions[] = {
            "-2 player game each person has 5 lives",
            "-Player 1: move with {A,D,S,W}, shoot by = SPACE",
            "-Player 2: move with {L,R,U,D}, shoot by = ENTER",
            "-Kill zombies and avoid enemy bullets to survive",
            "-Hitting a zombie adds 1 point, hitting a person",
            " adds 3 points",
            "-How to Determine the Winner: After 5 minutes",
            "-The person with more points wins.",
            "-If the points are equal, consider the remaining",
            " lives. Whoever has more lives wins.",
            "-If both the points and the lives are equal,",
            " the match ends in a draw",
        };

            for (int i = 0; i < 12; i++) {
               SDL_Surface* textSurface = TTF_RenderText_Solid(instructionFont, instructions[i], textColor);
               SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
               int textWidth, textHeight;
               TTF_SizeText(instructionFont, instructions[i], &textWidth, &textHeight);
               SDL_Rect textRect = {SCREEN_WIDTH / 2 - textWidth / 2, 150 + i * 30, textWidth, textHeight};
               SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
               SDL_FreeSurface(textSurface);
               SDL_DestroyTexture(textTexture);
           }

           SDL_FreeSurface(titleSurface);
           SDL_DestroyTexture(titleTexture);
           TTF_CloseFont(titleFont);
           TTF_CloseFont(instructionFont);
        }
            buttonHandler.render(renderer, false, false, true);
          } else {
                  menu.render();
            }
            SDL_RenderPresent(renderer);
        }


        Player player1 , player2;

        player1.randomizePlayerPosition(player2);
        player2.randomizePlayerPosition(player1);

        if (!player1.loadHeartTexture(renderer) || !player2.loadHeartTexture(renderer)) {
            cout << "Không thể load hình trái tim!" << endl;
            return -1;
        }

        Uint32 lastSpawnTime = SDL_GetTicks();
         const int spawnInterval = 20000;
         int maxZombies = 5; //

         const int GAME_DURATION = 300000;
         Uint32 gameStartTime = SDL_GetTicks();
         Uint32 pauseStartTime = 0;//
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

        Uint32 lastShotTime1 = 0;
        Uint32 lastShotTime2 = 0;
        const int SHOOT_DELAY = 500;
        string winMessage = "";


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
                             Mix_ResumeMusic();
                         }
                    }

                if(!isPaused){
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    Uint32 currentTime = SDL_GetTicks();
                    if (currentTime - lastShotTime1 >= SHOOT_DELAY) {
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
                    if (currentTime - lastShotTime2 >= SHOOT_DELAY) {
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
                    if (buttonHandler.checkRestartClick(mouseX, mouseY)) {
                        isPaused = false;
                        pausedTime += SDL_GetTicks() - pauseStartTime;
                        Mix_ResumeMusic();
                    }
                    if (buttonHandler.checkNewGameClick(mouseX, mouseY)) {
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
                            isPaused = false;
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
            SDL_RenderClear(renderer);
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
            player2.render(renderer);
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

           for (auto& bullet : bullet1) {
              for (auto& zombie : zombies) {
                  if (zombie.ZombieIsHit(bullet.x, bullet.y)) {

                       bullet.active = false;
                      zombie.alive = false;
                       player1.score++;
                 }
              }
           }
           zombies.erase( remove_if(zombies.begin(), zombies.end(), [](const Zombie &b) { return !b.alive; }),zombies.end());
           for (auto it = zombies.begin(); it != zombies.end();) {
               if (!it->alive) {
                  it = zombies.erase(it);
                    } else {
                         ++it;
               }
           }
           for (auto& bullet : bullet2) {
              for (auto& zombie : zombies) {
                  if (zombie.ZombieIsHit(bullet.x, bullet.y)) {

                      bullet.active = false;
                      zombie.alive = false;
                      player2.score++;
               }
             }
           }

            for (auto& bullet : bullet1) {
                if (player2.PlayerIsHit(bullet.x, bullet.y)) {
                    bullet.active = false;
                     player2.live--;
                     player1.score += 3;
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
                  player1.live--;
                  player2.score += 3;
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
                 spawnZombies(zombies, 1, player1, player2 , renderer); //
                 lastSpawnTime = currentTime;
             }

           int player1ScoreX = 380;
           int player2ScoreX = 200;

            player1.renderHearts(renderer, 1 , font_score , player1.score);// ve trai tim
            player2.renderHearts(renderer, 2 , font_score , player2.score);

             player1.renderCountdown(renderer, font_score, gameStartTime, GAME_DURATION, player1ScoreX, player2ScoreX , pausedTime);
             bullet1.erase( remove_if(bullet1.begin(), bullet1.end(), [](const Bullet &b) { return !b.active; }),bullet1.end());
             bullet2.erase( remove_if(bullet2.begin(), bullet2.end(), [](const Bullet &b) { return !b.active; }),bullet2.end());
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                buttonHandler.render(renderer, true, false, false);
            }
         }


             if (gameOver) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                player1.renderText(renderer, font_screen, winMessage, SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 200);
                buttonHandler.render(renderer,false , true, false);
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

