#include <stdlib.h>

#include "utils.h"


int randrange(int a, int b) {
    return rand() % (b - a + 1) + a;
}

Cell make_cell(int row, int col) {
    Cell cl;
    cl.row = row;
    cl.col = col;
    return cl;
}

Selection *create_sel(int val, int min, int max, bool rotate) {
    Selection *sel = calloc(sizeof(Selection), 1);
    sel->val = val;
    sel->min = min;
    sel->max = max;
    sel->rotate = rotate;
    sel->length = max - min;
    return sel;
}

void destroy_sel(Selection *sel) {
    free(sel);
}

void sel_next(Selection *sel) {
    sel->val++;
    if (sel->val > sel->max)
        sel->val = (sel->rotate) ? sel->min : sel->max;
}

void sel_prev(Selection *sel) {
    sel->val--;
    if (sel->val < sel->min)
        sel->val = (sel->rotate) ? sel->max : sel->min;
}

int sel_val(const Selection *sel) {
    return sel->val;
}
