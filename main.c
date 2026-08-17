#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcs.h"
#include "consts.h"

// #define KNIGHT_MASK 0x0a1100110aULL

// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0
// 1 1 1 1  1 1 1 0

// #define NOT_FILE_A       0xfefefefefefefefeULL
// #define NOT_FILE_AB      0xfcfcfcfcfcfcfcfcULL
// #define NOT_FILE_ABC     0xf8f8f8f8f8f8f8f8ULL
// #define NOT_FILE_ABCD    0xf0f0f0f0f0f0f0f0ULL
// #define NOT_FILE_ABCDE   0xe0e0e0e0e0e0e0e0ULL
// #define NOT_FILE_ABCDEF  0xc0c0c0c0c0c0c0c0ULL
// #define NOT_FILE_ABCDEFG 0x8080808080808080ULL
//
// #define NOT_FILE_H       0x7f7f7f7f7f7f7f7fULL
// #define NOT_FILE_GH      0x3f3f3f3f3f3f3f3fULL
// #define NOT_FILE_FGH     0x1f1f1f1f1f1f1f1fULL
// #define NOT_FILE_EFGH    0x0f0f0f0f0f0f0f0fULL
// #define NOT_FILE_DEFGH   0x0707070707070707ULL
// #define NOT_FILE_CDEFGH  0x03030303030303030ULL
// #define NOT_FILE_BCDEFGH 0x0101010101010101ULL

char map[8][8];
char bitmap[8][8];

// test ahh
uint64_t knight_bitmap[64];
uint64_t king_bitmap[64];
uint64_t pawn_bitmap[2][64];

uint64_t not_files[2][8] =
{
  { 0xffffffffffffffffULL, 0xfefefefefefefefeULL,
    0xfcfcfcfcfcfcfcfcULL, 0xf8f8f8f8f8f8f8f8ULL,
    0xf0f0f0f0f0f0f0f0ULL, 0xe0e0e0e0e0e0e0e0ULL,
    0xc0c0c0c0c0c0c0c0ULL, 0x8080808080808080ULL },

  { 0xffffffffffffffffULL, 0x7f7f7f7f7f7f7f7fULL,
    0x3f3f3f3f3f3f3f3fULL, 0x1f1f1f1f1f1f1f1fULL,
    0x0f0f0f0f0f0f0f0fULL, 0x0707070707070707ULL,
    0x0303030303030303ULL, 0x0101010101010101ULL }
};

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

// 0 1 0 1  0 0 0 0
// 1 0 0 0  1 0 0 0
// 0 0 0 0  0 0 0 0
// 1 0 0 0  1 0 0 0
// 0 1 0 1  0 0 0 0
// 0 0 0 0  0 0 0 0
// 0 0 0 0  0 0 0 0
// 0 0 0 0  0 0 0 0

/*
 * TEST
 */

void show_bitmap_(uint64_t bitmap)
{
  for (int i = 0; i < 64; i++)
  {
    char bit = (char)(bitmap >> i & 1);
    putc(bit + '0', stdout);
    putc(' ', stdout);
    if ((i+1) % 8 == 0)
      putc('\n', stdout);
  }
  putc('\n', stdout);
}

void init_bitmap_()
{
  for (int i = 0; i < 64; i++)
  {
    knight_bitmap[i] = 0ULL;
    int x = i % 8;
    int y = i / 8;
    for (int v = 8; v < 16; v++)
    {
      int mx = x + vectors[v][0];
      int my = y + vectors[v][1];
      // printf("%d %d\n", mx, my);
      if (in_map(mx, my))
      {
        int place = my * 8 + mx;
        // show_bitmap_(knight_bitmap[i]);
        knight_bitmap[i] |= (1ULL << (uint64_t)place);
        // show_bitmap_(knight_bitmap[i]);
      }
    }
  }
}

/*
 * TEST
 */

int main(void)
{
  pcg_srand();

  // init_bitmap_();
  // for (int i = 0; i < 64; i++)
  // {
  //   show_bitmap_(knight_bitmap[i]);
  //   usleep(100000);
  // }

  if (general_input() == -1)
    return -1;

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

  for (int p = 0; p < (int)strlen(pieces); p++)
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

  char url[128] = "https://lichess.org/editor/";
  gen_fen(url);

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

