#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "funcs.h"
#include "consts.h"

extern char map[8][8];
extern char bitmap[8][8];
extern char pieces[65];
extern char color;
extern int vectors[16][2];

// (microsoft fix)
char* fgets_mf(char* buffer, int n, FILE* fp)
{
  if (fgets(buffer, n, fp) == NULL)
    return NULL;

  int newline_ind = strcspn(buffer, "\r\n");
  int diff = strlen(buffer) - newline_ind;
  if (diff == 0 || (diff == 1 && buffer[newline_ind] == '\r'))
    clear_fp(fp);
  buffer[newline_ind] = 0;

  return buffer;
}

int general_input(void)
{
  char filename[256];
  if (get_filename(filename, sizeof(filename)) != -1)
  {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
      puts("File open error.");
      fclose(fp);
      return -1;
    }

    char buffer[BUFFER_SIZE];
    int line_counter = 0;
    while (fgets_mf(buffer, BUFFER_SIZE, fp) != NULL)
    {
      if (validate_set(buffer) == -1)
      {
        printf("Invalid set number %i.\n", line_counter+1);
        fclose(fp);
        return -1;
      }
      printf("%i. %s\n", line_counter+1, buffer);
      line_counter++;
    }
    rewind(fp);

    if (!line_counter)
    {
      puts("File is empty.");
      fclose(fp);
      return -1;
    }
    if (line_counter > 1)
      input_set_number(fp, buffer, line_counter);

    parse_set(buffer);

    fclose(fp);
  }
  else
  {
    puts("Enter pieces set (for example, Rr)");
    fgets_mf(pieces, 65, stdin);

    puts("Enter color (w, white/b, black):");
    color = getchar();
  }

  return 0;
}

int input_set_number(FILE* fp, char* buffer, int lines_num)
{
  puts("Enter number of set:");
  int set_num;
  scanf("%i", &set_num);
  // printf("set_num: %i\n", set_num);

  if (set_num < 1 || set_num > lines_num)
  {
    puts("Invalid number of set.");
    return -1;
  }

  int line_counter = 1;
  while (fgets_mf(buffer, BUFFER_SIZE, fp) != NULL)
  {
    // printf("buffer: %s\n", buffer);
    // printf("line_counter: %i\n", line_counter);

    if (line_counter == set_num)
      break;
    line_counter++;
  }
  // printf("buffer: %s\n", buffer);

  return 0;
}

void parse_set(char* buffer)
{
  char* space_ptr = strchr(buffer, ' ');
  int pieces_len = strcspn(buffer, " ");
  strncpy(pieces, buffer, pieces_len);
  pieces[pieces_len] = 0;

  color = *(space_ptr + 1);
}

void clear_fp(FILE* fp)
{
  int c;
  while ((c = fgetc(fp)) != '\n' && c != EOF);
}

int get_filename(char *filename, size_t max_len)
{
#if defined(_WIN32) || defined(_WIN64)
  #include <windows.h>

  WIN32_FIND_DATA findData;
  HANDLE hFind = FindFirstFile("*.txt", &findData);
  if (hFind != INVALID_HANDLE_VALUE)
  {
      strncpy(filename, findData.cFileName, max_len);
      FindClose(hFind);
      return 0;
  }
#else
  #include <dirent.h>

  DIR *dir = opendir(".");
  if (dir != NULL)
  {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
      char *ext = strrchr(entry->d_name, '.');
      if (ext != NULL && strcmp(ext, ".txt") == 0)
      {
        strncpy(filename, entry->d_name, max_len);
        closedir(dir);
        return 0;
      }
    }
    closedir(dir);
  }
#endif

  return -1;
}

int validate_set(char* set)
{
  int set_size = strlen(set);
  // if (set[set_size-1] == '\n')
  //   set_size--;
  for (int i = 0; i < set_size-2; i++)
  {
    char _piece = tolower(set[i]);
    if (_piece != 'p' &&
        _piece != 'r' &&
        _piece != 'n' &&
        _piece != 'b' &&
        _piece != 'q')
      return -1;
  }
  if (set[set_size-2] != ' ')
    return -1;
  if (set[set_size-1] != 'w' && set[set_size-1] != 'b')
    return -1;

  return 0;
}

int validate_pieces(void)
{
  for (int i = 0; i < strlen(pieces); i++)
  {
    char _piece = tolower(pieces[i]);
    if (_piece == 'k' || (
        _piece != 'p' &&
        _piece != 'r' &&
        _piece != 'n' &&
        _piece != 'b' &&
        _piece != 'q'))
      return -1;
  }

  return 0;
}

void show_map(void)
{
  printf(ANSI_COLOR_BOLD_WHITE);
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if ((i+j) % 2 == 0)
        printf(ANSI_BG_COLOR_BLUE);
      else
        printf(ANSI_BG_COLOR_RESET);

      putc(map[i][j], stdout);
      putc(' ', stdout);
    }
    printf(ANSI_BG_COLOR_RESET);
    putc('\n', stdout);
  }
  printf(ANSI_COLOR_RESET);
}

void show_bitmap(void)
{
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (bitmap[i][j] == '1')
        printf(ANSI_COLOR_RED);
      putc(bitmap[i][j], stdout);
      if (bitmap[i][j] == '1')
        printf(ANSI_COLOR_RESET);
      putc(' ', stdout);
    }
    putc('\n', stdout);
  }
}

void init_map(void)
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      map[i][j] = EMPTY_CELL;
}

void init_bitmap(void)
{
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      bitmap[i][j] = '0';
}

int calc_busy_cells(void)
{
  int counter = 0;

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (bitmap[i][j] == '1' || map[i][j] != EMPTY_CELL)
        counter += 1;

  return counter;
}

void to_fen(char *str)
{
  int len = strlen(str);
  int counter = 0;

  for (int i = 0; i < 8; i++)
  {
    str[len++] = '/';

    for (int j = 0; j < 8; j++)
    {
      if (map[i][j] == EMPTY_CELL)
        counter++;
      else
      {
        if (counter != 0)
        {
          str[len++] = counter + '0';
          counter = 0;
        }
        str[len++] = map[i][j];
      }
    }
    if (counter != 0)
    {
      str[len++] = counter + '0';
      counter = 0;
    }
  }
  str[len] = 0;
}

char in_map(int x, int y)
{
  return !(x < 0 || x > 7 || y < 0 || y > 7);
}

void update_bitmap(int i, int j)
{
  char piece = map[i][j];
  // printf("%c %i; isup: %i\n", piece, color, (isupper(piece)));
  if (!!isupper(piece) != color)
    return;

  char _piece = tolower(piece);
  if (_piece == 'k')
  {
    for (int x = j-1; x <= j+1; x++)
      for (int y = i-1; y <= i+1; y++)
        if (in_map(x, y))
          bitmap[y][x] = '1';
  }
  else if (_piece == 'p')
  {
    char d = (color) ? -1 : 1;
    if (j-1 >= 0) bitmap[i+d][j-1] = '1';
    if (j+1 < 8) bitmap[i+d][j+1] = '1';
  }
  else if (_piece == 'r' || _piece == 'b' || _piece == 'q')
  {
    int start_v;
    int end_v;
    if (_piece == 'r')
    {
      start_v = 0;
      end_v = 4;
    }
    else if (_piece == 'b')
    {
      start_v = 4;
      end_v = 8;
    }
    else if (_piece == 'q')
    {
      start_v = 0;
      end_v = 8;
    }

    for (int v = start_v; v < end_v; v++)
    {
      int x = j+vectors[v][0];
      int y = i+vectors[v][1];
      // printf("v: %i; %i %i\n", v, x, y);
      while (1)
      {
        if (!in_map(x, y) || map[y][x] != EMPTY_CELL)
          break;

        bitmap[y][x] = '1';

        x += vectors[v][0];
        y += vectors[v][1];
      }
    }
  }
  else if (_piece == 'n')
    for (int v = 8; v < 16; v++)
    {
      int x = j+vectors[v][0];
      int y = i+vectors[v][1];

      if (x < 0 || x > 7 || y < 0 || y > 7 || map[y][x] != EMPTY_CELL)
        continue;

      bitmap[y][x] = '1';
    }
}

// start_i
// place
// p
void add_piece(int start_i, int space, char piece)
{
  int place = pcg_rand() % space;

  for (int i = start_i; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      char is_free = (map[i][j] == EMPTY_CELL);
      if (tolower(piece) == 'k' && !!isupper(piece) != color)
        is_free &= (bitmap[i][j] == '0');

      if (is_free)
      {
        if (place == 0)
        {
          map[i][j] = piece;
          if (tolower(piece) == 'k' && !!isupper(piece) == color)
            update_bitmap(i, j);
          return;
        }
        place--;
      }
    }
}

static uint64_t pcg_state = 0x853c49e6748fea9bULL;
static uint64_t const pcg_multiplier = 6364136223846793005ULL;

void pcg_srand(void)
{
  pcg_state = (uint64_t)time(NULL) ^ (uintptr_t)&pcg_srand;
}

uint32_t pcg_rand(void)
{
  uint64_t oldstate = pcg_state;
  pcg_state = oldstate * pcg_multiplier + 1442695040888963407ULL;
  uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
  uint32_t rot = oldstate >> 59u;
  return (xorshifted >> rot) | (xorshifted << ((-rot) & 31u));
}

