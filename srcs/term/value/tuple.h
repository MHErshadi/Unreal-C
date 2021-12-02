#ifndef __TUPLE__
#define __TUPLE__

#include "../context.h"
#include "int.h"

typedef struct
{
    size_t elems_n;
    val_t *elems;
} tuple_t;

tuple_t *tuple_set(size_t elems_n, val_t *elems);

tuple_t *tuple_cpy(const tuple_t *val);

void tuple_disp(const tuple_t *val);
void tuple_free(tuple_t *val);

bool_t *tuple_equal(const tuple_t *op1, const tuple_t *op2);
bool_t *tuple_nequal(const tuple_t *op1, const tuple_t *op2);

bool_t *tuple_has(const tuple_t *op1, val_t op2);

val_t tuple_idx(const tuple_t *op1, const int_t *op2);

int tuple_cmp(const tuple_t *op1, const tuple_t *op2);

#endif /* __TUPLE__ */
