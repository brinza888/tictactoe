#include <stdlib.h>

#include "utils.h"


int randrange(int a, int b) {
    return rand() % (b - a + 1) + a;
}
