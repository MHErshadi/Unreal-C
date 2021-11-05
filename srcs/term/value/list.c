#include <string.h>

#include "list.h"
#include "../value.h"
#include "../operation.h"
#include "../../macro.h"
#include "valm.h"

list_t *list_set(size_t elems_n, val_t *elems)
{
    list_t *val;
    alloc(val, list_t, 1);

    val->elems_n = elems_n;
    ELM(val) = elems;

    return val;
}

list_t *list_cpy(const list_t *val)
{
    val_t *elems;
    alloc(elems, val_t, val->elems_n);

    size_t i;
    for (i = 0; i < val->elems_n; i++)
        elems[i] = cpy_val(ELM(val)[i]);

    return list_set(val->elems_n, elems);
}

void list_disp(const list_t *val)
{
    printf("[");
    if (!val->elems_n)
    {
        printf("]");
        return;
    }

    disp_val(*ELM(val), '\0');
    size_t i;
    for (i = 1; i < val->elems_n; i++)
    {
        printf(", ");
        disp_val(ELM(val)[i], '\0');
    }

    printf("]");
}

void list_free(list_t *val)
{
    free_vals(ELM(val), val->elems_n);
    fre(val);
}

list_t *list_append(const list_t *op1, const val_t op2)
{
    size_t prod_s = op1->elems_n + 1;

    val_t *prod;
    alloc(prod, val_t, prod_s);

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        prod[i] = cpy_val(ELM(op1)[i]);
    prod[op1->elems_n] = cpy_val(op2);

    return list_set(prod_s, prod);
}

list_t *list_rem(const list_t *op1, const int_t *op2)
{
    size_t prod_s = op1->elems_n - 1;

    val_t *prod;
    alloc(prod, val_t, prod_s);

    long long idx = int_get_sll(op2);
    if (idx < 0)
        idx = op1->elems_n + idx;

    size_t i;
    for (i = 0; i < idx; i++)
        prod[i] = cpy_val(ELM(op1)[i]);
    size_t j;
    for (j = idx + 1; j < op1->elems_n; j++, i++)
        prod[i] = cpy_val(ELM(op1)[j]);

    return list_set(prod_s, prod);
}

list_t *list_repeat(const list_t *op1, const int_t *op2)
{
    unsigned long long rep_n = int_get_ull(op2);
    size_t prod_s = op1->elems_n * rep_n;

    val_t *prod;
    alloc(prod, val_t, prod_s);

    size_t i;
    for (i = 0; i < prod_s; i += op1->elems_n)
    {
        size_t j;
        for (j = 0; j < op1->elems_n; j++)
            prod[i + j] = cpy_val(ELM(op1)[j]);
    }

    return list_set(prod_s, prod);
}

bool_t *list_equal(const list_t *op1, const list_t *op2)
{
    if (op1->elems_n != op2->elems_n)
        return bool_set(0);

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        if (!cmp_op(ELM(op1)[i], ELM(op2)[i]))
            return bool_set(0);
    return bool_set(1);
}

bool_t *list_nequal(const list_t *op1, const list_t *op2)
{
    if (op1->elems_n != op2->elems_n)
        return bool_set(1);

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        if (!cmp_op(ELM(op1)[i], ELM(op2)[i]))
            return bool_set(1);
    return bool_set(0);
}

val_t list_idx(const list_t *op1, const int_t *op2)
{
    long long idx = int_get_sll(op2);
    if (idx < 0)
        idx = op1->elems_n + idx;

    return cpy_val(ELM(op1)[idx]);
}

int list_cmp(const list_t *op1, const list_t *op2)
{
    if (op1->elems_n != op2->elems_n)
        return 0;

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        if (!cmp_op(ELM(op1)[i], ELM(op2)[i]))
            return 0;
    return 1;
}