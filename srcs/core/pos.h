#ifndef __POS__
#define __POS__

#include <stddef.h>

typedef struct
{
    size_t idx;
    size_t ln;
    const char *fn;
} pos_t;

#define NULL_POS ((pos_t){0, 0, ""})

pos_t set_pos(size_t idx, size_t ln, const char *fn);
int pos_cmp(pos_t pos1, pos_t pos2);
void adv_pos(pos_t *pos, char curr_chr);

#define IDX(p) ((p).idx)
#define LN(p) ((p).ln)
#define FN(p) ((p).fn)

#endif /* __POS__ */
