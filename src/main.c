
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/function.h"
#include "../headers/graphics.h"
#include "../headers/save.h"

int main(int argc, char **argv) {
  srand(time(NULL));
Start:;
  int nb_players = nb_players_Message();  // Solo or Multijoueur
  int Game_mode_multi = 1;
  if (nb_players == 2)
    Game_mode_multi = Game_mode_multi_Message();  // if Multi: IA or Player IRL
  int Game_mode_3 = Game_mode_3_Message(
      nb_players, Game_mode_multi);  // Time limited or the faster or free
  int gridSize = 4;
  if (nb_players == 1) gridSize = Game_size_grid_Message(nb_players);

  int grid[2][gridSize][gridSize];
  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < gridSize; i++) {  // Initialistion de la grille
      for (int j = 0; j < gridSize; j++) {
        grid[k][i][j] = 0;
      }
    }
  }

  // tirage de deux cases au hasard
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      randomPicker(gridSize, grid[i]);
    }
  }

  // première sauvegarde
  Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
  Save(nb_players, gridSize, Game_mode_3, grid[1], 1);

  // initialisation de la fenetre et de la police

  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  int Height = 480;

  SDL_Window *window = NULL;
  int delta = (nb_players == 2) ? 2 : 1;
  window = SDL_CreateWindow("Jeu 2048", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, Height * 1.2 * delta,
                            Height * 1.3, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  // Initialisation des variables et du temps
  int continuer = 1, canclick = 0, mouseCanClick = 0;
  time_t start = time(NULL), roundTimer = time(NULL) + 1;
  char timer[50] = "temps restant: sec", timerRound[20] = "sec restantes",
       endMessage[50] = "Quitter ?";
  SDL_Point TimerPoint = {Height / 2, Height * 1.2},
            roundTimerPoint = {Height * 1.2, Height * 0.4};
  int playerTurn = 0, result, alowed, restart = 0, canPlay[2] = {1, 1},
      ended = 0;
  if (Game_mode_multi == 1) canPlay[1] = -1;

  SDL_Point rewind1 = {Height / 2, Height * 1.1},
            rewind2 = {Height * 1.9, Height * 1.1};

  int activated = 0;
  SDL_Event event;

  while (continuer) {
    SDL_PollEvent(&event);
    switch (event.type) {
      case SDL_QUIT:
          continuer = 0;
          break;
      case SDL_KEYDOWN:
          canclick = 1;
          break;
      case SDL_KEYUP:
        if (canclick == 1) {

          int two_players_mode = (nb_players == 2) && (Game_mode_multi == 0);

          // commandes joueur 1
          alowed = (Game_mode_3 == 0)
                       ? (playerTurn == 0)
                       : 1 && canPlay[0];  // si on est mode chacun son tour, on
                                           // regarde si c'est à nous, alors
                                           // alowed = 1
          SDL_Keycode key_down = two_players_mode ? SDLK_s : SDLK_DOWN;
          SDL_Keycode key_right = two_players_mode ? SDLK_d : SDLK_RIGHT;
          SDL_Keycode key_left = two_players_mode ? SDLK_q : SDLK_LEFT;
          SDL_Keycode key_up = two_players_mode ? SDLK_z : SDLK_UP;

          if (event.key.keysym.sym == key_right && alowed) {
              Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
              activated = 1;
              result = moveRight(gridSize, grid[0], 1);
          } else if (event.key.keysym.sym == key_down && alowed) {
              Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
              activated = 1;
              result = moveDown(gridSize, grid[0], 1);
          } else if (event.key.keysym.sym == key_left && alowed) {
              Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
              activated = 1;
              result = moveLeft(gridSize, grid[0], 1);
          } else if (event.key.keysym.sym == key_up && alowed) {
              Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
              activated = 1;
              result = moveUp(gridSize, grid[0], 1);
          }

          if (result) {
              randomPicker(gridSize, grid[0]);
              result = 0;
          }

          // Commandes joueur 2, si présent
          if (nb_players == 2) {
              alowed = (Game_mode_3 == 0) ? (playerTurn == 1) : 1 && canPlay[1];
              if (event.key.keysym.sym == SDLK_RIGHT && alowed) {
                  Save(nb_players, gridSize, Game_mode_3, grid[1], 1);
                  activated = 1;
                  result = moveRight(gridSize, grid[1], 1);
              } else if (event.key.keysym.sym == SDLK_DOWN && alowed) {
                  Save(nb_players, gridSize, Game_mode_3, grid[1], 1);
                  activated = 1;
                  result = moveDown(gridSize, grid[1], 1);
              } else if (event.key.keysym.sym == SDLK_LEFT && alowed) {
                  Save(nb_players, gridSize, Game_mode_3, grid[1], 1);
                  activated = 1;
                  result = moveLeft(gridSize, grid[1], 1);
              } else if (event.key.keysym.sym == SDLK_UP && alowed) {
                  Save(nb_players, gridSize, Game_mode_3, grid[1], 1);
                  activated = 1;
                  result = moveUp(gridSize, grid[1], 1);
              }

              if (result) {
                  randomPicker(gridSize, grid[1]);
                  result = 0;
              }
          }

          if (Game_mode_3 == 0 && activated == 1) {
            roundTimer = time(NULL);
            playerTurn = (playerTurn + 1) % nb_players;
            if (canPlay[1] == -1) {
              bot_player(gridSize, grid[1]);
              playerTurn = 0;
              roundTimer = time(NULL);
            }
          }
          activated = 0;
          canclick = 0;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseCanClick = 1;
        break;
      case SDL_MOUSEBUTTONUP:
        if (mouseCanClick == 1) {
          if (event.button.x > Height / 2 &&
              event.button.x < Height / 2 + Height / 10 &&
              event.button.y > Height * 1.1 &&
              event.button.y < Height * 1.1 + Height / 20) {
            int temp[8][8];
            for (int i = 0; i < 8; i++) {  // Initialistion de la grille
              for (int j = 0; j < 8; j++) {
                temp[i][j] = 0;
              }
            }
            Load(&nb_players, &gridSize, &Game_mode_3, temp, 0);
            for (int i = 0; i < gridSize; i++) {  // Initialistion de la grille
              for (int j = 0; j < gridSize; j++) {
                grid[0][i][j] = temp[i][j];
              }
            }
          }
          if (event.button.x > Height * 1.9 &&
              event.button.x < Height * 1.9 + Height / 10 &&
              event.button.y > Height * 1.1 &&
              event.button.y < Height * 1.1 + Height / 20) {
            int temp[8][8];
            for (int i = 0; i < 8; i++) {  // Initialistion de la grille
              for (int j = 0; j < 8; j++) {
                temp[i][j] = 0;
              }
            }
            Load(&nb_players, &gridSize, &Game_mode_3, temp, 1);
            for (int i = 0; i < gridSize; i++) {  // Initialistion de la grille
              for (int j = 0; j < gridSize; j++) {
                grid[1][i][j] = temp[i][j];
              }
            }
          }
        }
        mouseCanClick = 0;
        break;
    }
    resetScreen(renderer);

    for (int i = 0; i < 2; i++) {
      if (canPlay[i] != 0 && moveRight(gridSize, grid[i], 0) +
                                     moveLeft(gridSize, grid[i], 0) +
                                     moveDown(gridSize, grid[i], 0) +
                                     moveUp(gridSize, grid[i], 0) ==
                                 0) {
        canPlay[i] = 0;
      }
    }

    if (canPlay[0] == 0 && canPlay[1] == 0) {
      ended = 1;
      int score0 = SumGrid(gridSize, grid[0]);
      int score1 = SumGrid(gridSize, grid[1]);
      if (score0 > score1) {
        sprintf(endMessage, "Le joueur 1 à gagné avec %d pts", score0);
      } else if (score1 > score0) {
        sprintf(endMessage, "Le joueur 2 à gagné avec %d pts", score1);
      } else
        sprintf(endMessage, "égalitée !");
    }

    time_t current = time(NULL);
    int SecTimer = current - start;  // Calcul tu temps
    if (Game_mode_3 == 1) {          // Affichage timer en mode chrono
      sprintf(timer, "Temps restant: %d sec", 300 - SecTimer);
      printText(timer, renderer, TimerPoint);
      if (SecTimer == 300) {
        ended = 1;
        if (nb_players == 1)
          sprintf(endMessage, "Partie finie, ton score est de %d",
                  SumGrid(gridSize, grid[0]));
        else {
          int score0 = SumGrid(gridSize, grid[0]);
          int score1 = SumGrid(gridSize, grid[1]);
          if (score0 > score1) {
            sprintf(endMessage, "Le joueur 1 à gagné avec %d pts", score0);
          } else if (score1 > score0) {
            sprintf(endMessage, "Le joueur 2 à gagné avec %d pts", score1);
          } else
            sprintf(endMessage, "égalitée !");
        }
      }
    }

    if (Game_mode_3 == 0 && nb_players == 2) {
      // printf("%d", best);
      int tmptimer = current - roundTimer;
      sprintf(timerRound, "%d sec", 5 - tmptimer);
      printText(timerRound, renderer, roundTimerPoint);
      if (tmptimer == 5) {
        playerTurn = !playerTurn;
        if (canPlay[1] == -1) {
          bot_player(gridSize, grid[1]);
          playerTurn = 0;
        }
        roundTimer = time(NULL);
      }

      if (BestTile(gridSize, grid[0]) == 2048) {
        ended = 1;
        if (nb_players == 2)
          sprintf(endMessage, "Joueur 1 à gagné !");
        else
          sprintf(endMessage, "Partie finie !");
      } else if (BestTile(gridSize, grid[1]) == 2048) {
        ended = 1;
        sprintf(endMessage, "Joueur 2 à gagné !");
      }
    }

    printGrid(renderer, Height, gridSize, grid[0],
              0);  // Affichage de la grille
    if (nb_players == 2) {
      printGrid(renderer, Height, gridSize, grid[1],
                1);  // Affichage de la 2eme grille
      if (Game_mode_3 == 0) hideGrid(renderer, Height, !playerTurn);
    }
    createButton("annuler", renderer, rewind1);
    if (nb_players == 2) createButton("annuler", renderer, rewind2);
    SDL_RenderPresent(renderer);

    if (ended) continuer = 0;
  }
  restart = EndPopUp(endMessage);

  SDL_DestroyWindow(window);
  SDL_Quit();
  if (!ended) {
    Save(nb_players, gridSize, Game_mode_3, grid[0], 0);
    Save(nb_players, gridSize, Game_mode_3, grid[1], 1);
  } else {
    remove("saves/last_0");
    remove("saves/last_1");
  }
  if (restart == 1) goto Start;
  return 0;
}