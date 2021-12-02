#ifndef __DICT__
#define __DICT__

#include "../value.h"

typedef struct
{
    size_t kvals_n;
    kval_t *kvals;
} dict_t;

dict_t *dict_set(size_t kvals_n, kval_t *kvals);

dict_t *dict_cpy(const dict_t *val);

void dict_disp(const dict_t *val);
void dict_free(dict_t *val);

dict_t *dict_rem(const dict_t *op1, val_t op2);

bool_t *dict_equal(const dict_t *op1, const dict_t *op2);
bool_t *dict_nequal(const dict_t *op1, const dict_t *op2);

bool_t *dict_has(const dict_t *op1, val_t op2);

val_t dict_idx(const dict_t *op1, val_t op2);

int dict_cmp(const dict_t *op1, const dict_t *op2);

#endif /* __DICT__ */
