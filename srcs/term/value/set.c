#include "set.h"
#include "../value.h"
#include "../operation.h"
#include "../../macro.h"
#include "valm.h"

set_t *set_set(size_t elems_n, val_t *elems)
{
    set_t *val;
    alloc(val, set_t, 1);

    val->elems_n = elems_n;
    ELM(val) = elems;

    return val;
}

set_t *set_cpy(const set_t *val)
{
    val_t *elems;
    alloc(elems, val_t, val->elems_n);

    size_t i;
    for (i = 0; i < val->elems_n; i++)
        elems[i] = cpy_val(ELM(val)[i]);

    return set_set(val->elems_n, elems);
}

void set_disp(const set_t *val)
{
    printf("{");
    if (!val->elems_n)
    {
        printf("}");
        return;
    }

    disp_val(*ELM(val), '\0');
    size_t i;
    for (i = 1; i < val->elems_n; i++)
    {
        printf(", ");
        disp_val(ELM(val)[i], '\0');
    }

    printf("}");
}

void set_free(set_t *val)
{
    free_vals(ELM(val), val->elems_n);
    fre(val);
}

set_t *set_append(const set_t *op1, const val_t op2)
{
    size_t prod_s = op1->elems_n + 1;

    val_t *prod;
    alloc(prod, val_t, prod_s);

    int sadd = 1;
    size_t i;
    for (i = 0; i < op1->elems_n; i++)
    {
        if (cmp_op(ELM(op1)[i], op2))
            sadd = 0;
        prod[i] = cpy_val(ELM(op1)[i]);
    }
    if (sadd)
        prod[op1->elems_n] = cpy_val(op2);
    else
        ralloc(prod, val_t, --prod_s);

    return set_set(prod_s, prod);
}

bool_t *set_equal(const set_t *op1, const set_t *op2)
{
    return bool_set(set_cmp(op1, op2));
}

bool_t *set_nequal(const set_t *op1, const set_t *op2)
{
    return bool_set(!set_cmp(op1, op2));
}

val_t set_idx(const set_t *op1, const int_t *op2)
{
    long long idx = int_get_sll(op2);
    if (idx < 0)
        idx = op1->elems_n + idx;

    return cpy_val(ELM(op1)[idx]);
}

int set_cmp(const set_t *op1, const set_t *op2)
{
    if (op1->elems_n != op2->elems_n)
        return 0;

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        if (!vals_has(ELM(op2), op2->elems_n, ELM(op1)[i]))
            return 0;
    return 1;
}
