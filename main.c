#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"
#include "consts.h"

char map[8][8];
char bitmap[8][8];

char pieces[65];
char color;

int vectors[16][2] = {
  // rook
  {-1, 0},
  {0, -1},
  {0, 1},
  {1, 0},

  // bishop
  {-1, -1},
  {-1, 1},
  {1, -1},
  {1, 1},

  // knight
  {-2, -1},
  {-2, 1},
  {-1, -2},
  {-1, 2},
  {1, -2},
  {1, 2},
  {2, -1},
  {2, 1}
};

int main()
{
  srand(time(NULL));

  if (general_input() == -1)
    return -1;

  if (validate_pieces() == -1)
  {
    puts("Invalid pieces set.");
    return -1;
  }

  if (color != 'w' && color != 'b')
  {
    puts("Invalid color.");
    return -1;
  }
  color = (color == 'w');

  init_map();
  init_bitmap();

  char first = 'K';
  char second = 'k';

  if (color == 0)
  {
    first = 'k';
    second = 'K';
  }

  add_piece(0, 64, first);

  for (int p = 0; p < strlen(pieces); p++)
  {
    // normal piece
    int space = 64 - p - 1;
    int start_i = 0;

    // pawn
    if (tolower(pieces[p]) == 'p')
    {
      space = 48 - p - 1;
      start_i = 1;
    }

    add_piece(start_i, space, pieces[p]);
  }

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (map[i][j] != EMPTY_CELL)
        update_bitmap(i, j);
  
  add_piece(0, 64 - calc_busy_cells(), second);

  show_map();
  show_bitmap();

  char url[128] = "https://lichess.org/editor";
  to_fen(url);

  char parameters[16] = "_?_-_-_0_1";
  parameters[1] = (color) ? 'w' : 'b';
  strcat(url, parameters);

  puts(url);

  char cmd[256];
#if defined(_WIN32) || defined(_WIN64)
  snprintf(cmd, sizeof(cmd), "start %s", url);
  system(cmd);
#elif defined(__APPLE__)
  snprintf(cmd, sizeof(cmd), "open '%s'", url);
  system(cmd);
#elif defined(__linux__)
  snprintf(cmd, sizeof(cmd), "xdg-open '%s' > /dev/null 2>&1", url);
  system(cmd);
#endif

  return 0;
}

