#include "dict.h"
#include "../value.h"
#include "../operation.h"
#include "../../macro.h"
#include "valm.h"

dict_t *dict_set(size_t kvals_n, kval_t *kvals)
{
    dict_t *dict;
    alloc(dict, dict_t, 1);

    dict->kvals_n = kvals_n;
    KVAL(dict) = kvals;

    return dict;
}

dict_t *dict_cpy(const dict_t *val)
{
    kval_t *kvals;
    alloc(kvals, kval_t, val->kvals_n);

    size_t i;
    for (i = 0; i < val->kvals_n; i++)
        kvals[i] = cpy_kval(KVAL(val)[i]);

    return dict_set(val->kvals_n, kvals);
}

void dict_disp(const dict_t *val)
{
    printf("{");
    if (!val->kvals_n)
    {
        printf("}");
        return;
    }

    disp_val(KEY(*KVAL(val)), ':');
    printf(" ");
    disp_val(VAL(*KVAL(val)), '\0');
    size_t i;
    for (i = 1; i < val->kvals_n; i++)
    {
        printf(", ");
        disp_val(KEY(KVAL(val)[i]), ':');
        printf(" ");
        disp_val(VAL(KVAL(val)[i]), '\0');
    }

    printf("}");
}

void dict_free(dict_t *val)
{
    free_kvals(KVAL(val), val->kvals_n);
    fre(val);
}

dict_t *dict_rem(const dict_t *op1, val_t op2)
{
    size_t prod_s = op1->kvals_n;

    kval_t *prod;
    alloc(prod, kval_t, prod_s);

    size_t i;
    for (i = 0; i < op1->kvals_n; i++)
    {
        if (cmp_op(op2, KEY(KVAL(op1)[i])))
            break;
        prod[i] = cpy_kval(KVAL(op1)[i]);
    }
    if (i != prod_s)
    {
        ralloc(prod, kval_t, prod_s--);
        size_t j = i;
        for (i++ ; i < op1->kvals_n; i++)
            prod[j] = cpy_kval(KVAL(op1)[i]);
    }

    return dict_set(prod_s, prod);
}

bool_t *dict_equal(const dict_t *op1, const dict_t *op2)
{
    return bool_set(dict_cmp(op1, op2));
}

bool_t *dict_nequal(const dict_t *op1, const dict_t *op2)
{
    return bool_set(!dict_cmp(op1, op2));
}

val_t dict_idx(const dict_t *op1, val_t op2)
{
    size_t i;
    for (i = 0; i < op1->kvals_n; i++)
        if (cmp_op(op2, KEY(KVAL(op1)[i])))
            return cpy_val(VAL(KVAL(op1)[i]));
    return set_val(0, NULL, NULL, NULL_POS, NULL_POS);
}

int dict_cmp(const dict_t *op1, const dict_t *op2)
{
    if (op1->kvals_n != op2->kvals_n)
        return 0;

    size_t i;
    for (i = 0; i < op1->kvals_n; i++)
        if (!kvals_haskv(KVAL(op2), op2->kvals_n, KVAL(op1)[i]))
            return 0;

    return 1;
}
