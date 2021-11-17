#include "tuple.h"
#include "../value.h"
#include "../operation.h"
#include "../../macro.h"
#include "valm.h"

tuple_t *tuple_set(size_t elems_n, val_t *elems)
{
    tuple_t *val;
    alloc(val, tuple_t, 1);

    val->elems_n = elems_n;
    ELM(val) = elems;

    return val;
}

tuple_t *tuple_cpy(const tuple_t *val)
{
    val_t *elems;
    alloc(elems, val_t, val->elems_n);

    size_t i;
    for (i = 0; i < val->elems_n; i++)
        elems[i] = cpy_val(ELM(val)[i]);

    return tuple_set(val->elems_n, elems);
}

void tuple_disp(const tuple_t *val)
{
    printf("(");

    disp_val(*ELM(val), '\0');
    size_t i;
    for (i = 1; i < val->elems_n; i++)
    {
        printf(", ");
        disp_val(ELM(val)[i], '\0');
    }

    printf(")");
}

void tuple_free(tuple_t *val)
{
    free_vals(ELM(val), val->elems_n);
    fre(val);
}

bool_t *tuple_equal(const tuple_t *op1, const tuple_t *op2)
{
    return bool_set(tuple_cmp(op1, op2));
}

bool_t *tuple_nequal(const tuple_t *op1, const tuple_t *op2)
{
    return bool_set(!tuple_cmp(op1, op2));
}

val_t tuple_idx(const tuple_t *op1, const int_t *op2)
{
    long long idx = int_get_sll(op2);
    if (idx < 0)
        idx = op1->elems_n + idx;

    return cpy_val(ELM(op1)[idx]);
}

int tuple_cmp(const tuple_t *op1, const tuple_t *op2)
{
    if (op1->elems_n != op2->elems_n)
        return 0;

    size_t i;
    for (i = 0; i < op1->elems_n; i++)
        if (!cmp_op(ELM(op1)[i], ELM(op2)[i]))
            return 0;
    return 1;
}
