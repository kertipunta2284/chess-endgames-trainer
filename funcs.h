#ifndef FUNCS_H
#define FUNCS_H

char* fgets_mf(char* buffer, int n, FILE* fp);
int general_input();
int input_set_number(FILE* fp, char* buffer, int lines_num);
int parse_set(char* buffer);
void clear_fp(FILE* fp);
int get_filename(char *filename, size_t max_len);
int validate_set(char* set);
int validate_pieces();
void show_map();
void show_bitmap();
void init_map();
void init_bitmap();
int calc_busy_cells();
void to_fen(char *str);
char in_map(int x, int y);
void update_bitmap(int i, int j);
void add_piece(int start_i, int space, char piece);

#endif

