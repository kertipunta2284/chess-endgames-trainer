#ifndef FUNCS_H
#define FUNCS_H

#include <stdint.h>

char* fgets_mf(char* buffer, int n, FILE* fp);
int general_input(void);
int input_set_number(FILE* fp, char* buffer, int lines_num);
void parse_set(char* buffer);
void clear_fp(FILE* fp);
int get_filename(char *filename, size_t max_len);
int validate_set(char* set);
int validate_pieces(void);
void show_map(void);
void show_bitmap(void);
void init_map(void);
void init_bitmap(void);
int calc_busy_cells(void);
void to_fen(char *str);
char in_map(int x, int y);
void update_bitmap(int i, int j);
void add_piece(int start_i, int space, char piece);
void pcg_srand(void);
uint32_t pcg_rand(void);

#endif

