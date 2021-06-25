#include <stdio.h>
#include <time.h>

void Save(int playerCount, int gridSize, int gamemode,
          int grid[gridSize][gridSize], int playerTurn) {
  FILE *file;
  char path[300];
  sprintf(path, "saves/last_%d", playerTurn);
  file = fopen(path, "w");  // import du fichier

  fprintf(file, "%d\n%d\n%d\n", playerCount, gridSize, gamemode);
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      fprintf(file, "%d\n", grid[i][j]);
    }
  }
  fclose(file);
}

void Load(int *playerCount, int *gridSize, int *gamemode, int grid[8][8],
          int playerTurn) {
  char path[300];
  sprintf(path, "saves/last_%d", playerTurn);
  FILE *file = fopen(path, "r");
  fscanf(file, "%d\n%d\n%d\n", playerCount, gridSize, gamemode);
  for (int i = 0; i < *gridSize; i++) {
    for (int j = 0; j < *gridSize; j++) {
      fscanf(file, "%d\n", &grid[i][j]);
    }
  }
  fclose(file);
}