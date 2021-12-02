#ifndef __LIST__
#define __LIST__

#include "../context.h"
#include "int.h"

typedef struct
{
    size_t elems_n;
    val_t *elems;
} list_t;

list_t *list_set(size_t elems_n, val_t *elems);

list_t *list_cpy(const list_t *val);

void list_disp(const list_t *val);
void list_free(list_t *val);

list_t *list_append(const list_t *op1, val_t op2);
list_t *list_rem(const list_t *op1, const int_t *op2);
list_t *list_repeat(const list_t *op1, const int_t *op2);

bool_t *list_equal(const list_t *op1, const list_t *op2);
bool_t *list_nequal(const list_t *op1, const list_t *op2);

bool_t *list_has(const list_t *op1, val_t op2);

val_t list_idx(const list_t *op1, const int_t *op2);

int list_cmp(const list_t *op1, const list_t *op2);

#endif /* __LIST__ */
