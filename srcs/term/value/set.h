#ifndef __SET__
#define __SET__

#include "../context.h"
#include "int.h"

typedef struct
{
    size_t elems_n;
    val_t *elems;
} set_t;

set_t *set_set(size_t elems_n, val_t *elems);

set_t *set_cpy(const set_t *val);

void set_disp(const set_t *val);
void set_free(set_t *val);

set_t *set_append(const set_t *op1, const val_t op2);

bool_t *set_equal(const set_t *op1, const set_t *op2);
bool_t *set_nequal(const set_t *op1, const set_t *op2);

int set_cmp(const set_t *op1, const set_t *op2);

#endif /* __SET__ */
