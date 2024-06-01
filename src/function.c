#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// randomPicker permet de créer une case  aléatoire (2 ou 4) après chaque coup
// joué
void randomPicker(int gridSize, int grid[gridSize][gridSize]) {
  int valid = 0;
  int x, y;
  do {
    x = rand() % gridSize;  // aléatoire des lignes
    y = rand() % gridSize;  // aléatoire des colonnes
    if (grid[x][y] == 0) {  // Vérification si la case est libre ou non
      valid = 1;
      grid[x][y] = 2 + 2 * rand() % 2;  // aléatoire entre 2 et 4
    }
  } while (!valid);
}

// moveRight sert à fusionner et déplacer les cases vers la droite
/*Cette fonction est divisée en 2 parties: Une première partie qui va fusionner
les cases entre elles en partant de la droite vers la gauche si elles le
peuvent. Et une seconde partie qui permet de déplacer toutes les cases vers la
droite.*/
int moveRight(int gridSize, int grid[gridSize][gridSize], int writable) {
  int cx, nb_evo = 0, z = 1, nb_changes = 0;
  for (int j = 0; j < gridSize; j++) {
    for (int i = gridSize - 1; i >= 0;
         i--) {  // On commence bien le "for" par la droite vers la gauche
      z = 1;  // L'algorithme part de l'extrême droite et regarde à la case de
              // gauche si
      while (1) {  // si elles sont égales, si ce n'est pas le cas alors on se
                   // déplace de 1 vers la gauche
        if (i - z > -1) {  // et réitère l'algorithme à regarder à gauche pour
                           // voir si il y a la même case.
          if (grid[i - z][j] != grid[i][j] &&
              grid[i - z][j] != 0)  // Si il n'y a pas de case à gauche, z++ et
                                    // on regarde alors à 2 cases à gauche
          {  // Dès qu'une ligne est finie, on passe alors à la suivante
            break;  // Afin de ne pas avoir d'erreurs de taille de table, la
                    // boucle commence par une condition.
          }
          if (grid[i][j] == grid[i - z][j] && grid[i][j] != 0) {
            if (writable == 1) {
              grid[i][j] = grid[i][j] * 2;
              grid[i - z][j] = 0;
            }
            nb_evo = 1;
            nb_changes++;
            break;
          } else {
            z++;
          }
        } else
          break;
      }
    }
  }
  for (int j = 0; j < gridSize; j++) {
    for (int i = gridSize - 2; i > -1; i--) {
      cx = i;  // La deuxième partie est assez similaire que la première mais
               // les
      while (1) {  // vérifications de longueur de table ne sont pas faites
                   // exactement de la même manière.
        if (grid[cx][j] != 0 &&
            grid[cx + 1][j] ==
                0) {  // Le for commence cette fois-ci à l'extrême droite -1 et
                      // vérifie ensuite si la case à sa
          if (writable == 1) {
            grid[cx + 1][j] =
                grid[cx]
                    [j];  // droite est libre pour se déplacer, sinon on passe à
                          // la case qui est à sa gauche et ainsi de suite.
            grid[cx][j] =
                0;  // Enfin si une case est à l'extrême gauche, le while avec
                    // cx permet à la case de se déplacer 3 fois vers la droite
          }
          nb_evo = 1;
          nb_changes++;
        } else
          break;
        cx++;
        if (cx == gridSize - 1) break;
      }
    }
  }
  if (writable == 1)
    return nb_evo;
  else
    return nb_changes;
}

// Les 3 fonctions suivantes sont intellectuellement similaires à moveRight,
// seuls les départs et sens de boucles changent

int moveLeft(int gridSize, int grid[gridSize][gridSize], int writable) {
  int cx, nb_evo = 0, z = 1, nb_changes = 0;
  for (int j = 0; j < gridSize; j++) {
    for (int i = 0; i < gridSize; i++) {
      z = 1;
      while (1) {
        if (i + z < gridSize) {
          if (grid[i + z][j] != grid[i][j] && grid[i + z][j] != 0) {
            break;
          }
          if (grid[i][j] == grid[i + z][j] && grid[i][j] != 0) {
            if (writable == 1) {
              grid[i][j] = grid[i][j] * 2;
              grid[i + z][j] = 0;
            }
            nb_evo = 1;
            nb_changes++;
            break;
          } else {
            z++;
          }
        } else
          break;
      }
    }
  }
  for (int j = 0; j < gridSize; j++) {
    for (int i = 1; i < gridSize; i++) {
      cx = i;
      while (1) {
        if (grid[cx][j] != 0 && grid[cx - 1][j] == 0) {
          if (writable == 1) {
            grid[cx - 1][j] = grid[cx][j];
            grid[cx][j] = 0;
          }
          nb_evo = 1;
          nb_changes++;
        } else
          break;
        cx--;
        if (cx == 0) break;
      }
    }
  }
  if (writable == 1)
    return nb_evo;
  else
    return nb_changes;
}

int moveDown(int gridSize, int grid[gridSize][gridSize], int writable) {
  int cx, nb_evo = 0, z = 1, nb_changes = 0;
  for (int i = 0; i < gridSize; i++) {
    for (int j = gridSize - 1; j >= 0; j--) {
      z = 1;
      while (1) {
        if (j - z > -1) {
          if (grid[i][j - z] != grid[i][j] && grid[i][j - z] != 0) {
            break;
          }
          if (grid[i][j] == grid[i][j - z] && grid[i][j] != 0) {
            if (writable == 1) {
              grid[i][j] = grid[i][j] * 2;
              grid[i][j - z] = 0;
            }
            nb_evo = 1;
            nb_changes++;
            break;
          } else {
            z++;
          }
        } else
          break;
      }
    }
  }
  for (int i = 0; i < gridSize; i++) {
    for (int j = gridSize - 2; j > -1; j--) {
      cx = j;
      while (1) {
        if (grid[i][cx] != 0 && grid[i][cx + 1] == 0) {
          if (writable == 1) {
            grid[i][cx + 1] = grid[i][cx];
            grid[i][cx] = 0;
          }
          nb_evo = 1;
          nb_changes++;
        } else
          break;
        cx++;
        if (cx == gridSize - 1) break;
      }
    }
  }
  if (writable == 1)
    return nb_evo;
  else
    return nb_changes;
}

int moveUp(int gridSize, int grid[gridSize][gridSize], int writable) {
  int cx, nb_evo = 0, z = 1, nb_changes = 0;
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      z = 1;
      while (1) {
        if (j + z < gridSize) {
          if (grid[i][j + z] != grid[i][j] && grid[i][j + z] != 0) {
            break;
          }
          if (grid[i][j] == grid[i][j + z] && grid[i][j] != 0) {
            if (writable == 1) {
              grid[i][j] = grid[i][j] * 2;
              grid[i][j + z] = 0;
            }
            nb_evo = 1;
            nb_changes++;
            break;
          } else {
            z++;
          }
        } else
          break;
      }
    }
  }
  for (int i = 0; i < gridSize; i++) {
    for (int j = 1; j < gridSize; j++) {
      cx = j;
      while (1) {
        if (grid[i][cx] != 0 && grid[i][cx - 1] == 0) {
          if (writable == 1) {
            grid[i][cx - 1] = grid[i][cx];
            grid[i][cx] = 0;
          }
          nb_evo = 1;
          nb_changes++;
        } else
          break;
        cx--;
        if (cx == 0) break;
      }
    }
  }
  if (writable == 1)
    return nb_evo;
  else
    return nb_changes;
}

int SumGrid(int gridSize, int grid[gridSize][gridSize]) {
  int sum = 0;
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      sum += grid[i][j];
    }
  }
  return sum;
}

int BestTile(int gridSize, int grid[gridSize][gridSize]) {
  int best = 0;
  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      best = (best > grid[i][j]) ? best : grid[i][j];
    }
  }
  return best;
}

void bot_player(int gridSize, int grid[gridSize][gridSize]) {
  int result = 0;
  int result1 = moveRight(gridSize, grid, 0);
  int result2 = moveDown(gridSize, grid, 0);
  int result3 = moveUp(gridSize, grid, 0);
  if (result1 > result2) {
    result = moveRight(gridSize, grid, 1);
  } else if (result1 < result2) {
    result = moveDown(gridSize, grid, 1);
  } else if (result1 == result2 && result2 != 0) {
    result = moveRight(gridSize, grid, 1);
  } else if (result3 != 0) {
    result = moveUp(gridSize, grid, 1);
  } else {
    result = moveLeft(gridSize, grid, 1);
  }
  if (result) {
    randomPicker(gridSize, grid);
  }
}