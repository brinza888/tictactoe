#ifndef __UTILS_H
#define __UTILS_H

#include <stdbool.h>


int randrange(int a, int b);


typedef struct _Cell {
    int row;
    int col;
} Cell;

Cell make_cell(int row, int col);


typedef struct _Selection {
    int val;
    int min;
    int max;
    int length;
    bool rotate;
} Selection;

Selection *create_sel(int val, int min, int max, bool rotate);
void destroy_sel(Selection *sel);

void sel_next(Selection *sel);
void sel_prev(Selection *sel);
int sel_val(const Selection *sel);


#endif