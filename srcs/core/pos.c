#include <string.h>

#include "pos.h"

pos_t set_pos(size_t idx, size_t ln, const char *fn)
{
    pos_t pos;

    IDX(pos) = idx;
    LN(pos) = ln;
    FN(pos) = fn;

    return pos;
}

int pos_cmp(pos_t pos1, pos_t pos2)
{
    return IDX(pos1) == IDX(pos2) && !strcmp(FN(pos1), FN(pos2));
}

void adv_pos(pos_t *pos, char curr_chr)
{
    IDX(*pos)++;
    if (curr_chr == '\n')
        LN(*pos)++;
}
