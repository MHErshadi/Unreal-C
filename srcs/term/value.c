#include "value.h"
#include "../errlib/errlib.h"
#include "../macro.h"

val_t set_val(enum val_t type, void *val, struct __ctx *ctx, pos_t poss, pos_t pose)
{
    val_t value;

    TYP(value) = type;
    VAL(value) = val;
    CTX(value) = ctx;
    POSS(value) = poss;
    POSE(value) = pose;

    return value;
}

val_t cpy_val(val_t value)
{
    if (!TYP(value))
        return value;
    if (val_type(value, INT_V))
    {
        int_t *cpy = int_cpy((int_t*)VAL(value));
        val_t value_c = set_val(INT_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, FLOAT_V))
    {
        float_t *cpy = float_cpy((float_t*)VAL(value));
        val_t value_c = set_val(FLOAT_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, CMPX_V))
    {
        cmpx_t *cpy = cmpx_cpy((cmpx_t*)VAL(value));
        val_t value_c = set_val(CMPX_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, BOOL_V))
    {
        bool_t *cpy = bool_cpy((bool_t*)VAL(value));
        val_t value_c = set_val(BOOL_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, NONE_V))
        return value;
    if (val_type(value, STR_V))
    {
        str_t *cpy = str_cpy((str_t*)VAL(value));
        val_t value_c = set_val(STR_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, LIST_V))
    {
        list_t *cpy = list_cpy((list_t*)VAL(value));
        val_t value_c = set_val(LIST_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }
    if (val_type(value, TUPLE_V))
    {
        tuple_t *cpy = tuple_cpy((tuple_t*)VAL(value));
        val_t value_c = set_val(TUPLE_V, cpy, CTX(value), POSS(value), POSE(value));

        return value_c;
    }

    un_crash("cpy_val function: invalid value type `%d`", TYP(value));
}

void disp_val(val_t value, char end)
{
    if (!TYP(value))
        return;
    if (val_type(value, INT_V))
    {
        int_disp((int_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, FLOAT_V))
    {
        float_disp((float_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, CMPX_V))
    {
        cmpx_disp((cmpx_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, BOOL_V))
    {
        bool_disp((bool_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, NONE_V))
    {
        printf("none");
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, STR_V))
    {
        str_disp((str_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, LIST_V))
    {
        list_disp((list_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }
    if (val_type(value, TUPLE_V))
    {
        tuple_disp((tuple_t*)VAL(value));
        if (end != '\0')
            printf("%c", end);
        return;
    }

    un_crash("disp_val function: invalid value type `%d`", TYP(value));
}

void free_vals(val_t *vals, size_t vals_s)
{
    size_t i;
    for (i = 0; i < vals_s; i++)
        free_val(vals[i]);
    fre(vals);
}

void free_val(val_t value)
{
    if (!TYP(value))
        return;
    if (val_type(value, INT_V))
    {
        int_free((int_t*)VAL(value));
        return;
    }
    if (val_type(value, FLOAT_V))
    {
        float_free((float_t*)VAL(value));
        return;
    }
    if (val_type(value, CMPX_V))
    {
        cmpx_free((cmpx_t*)VAL(value));
        return;
    }
    if (val_type(value, BOOL_V))
    {
        bool_free((bool_t*)VAL(value));
        return;
    }
    if (val_type(value, NONE_V))
        return;
    if (val_type(value, STR_V))
    {
        str_free((str_t*)VAL(value));
        return;
    }
    if (val_type(value, LIST_V))
    {
        list_free((list_t*)VAL(value));
        return;
    }
    if (val_type(value, TUPLE_V))
    {
        tuple_free((tuple_t*)VAL(value));
        return;
    }

    un_crash("free_val function: invalid value type `%d`", TYP(value));
}

int val_logic(val_t value)
{
    if (!TYP(value))
        return 0;
    if (val_type(value, INT_V))
    {
        int_t *val = (int_t*)VAL(value);

        return int_sgn(val) != 0;
    }
    if (val_type(value, FLOAT_V))
    {
        float_t *val = (float_t*)VAL(value);

        return float_sgn(val) != 0;
    }
    if (val_type(value, CMPX_V))
    {
        cmpx_t *val = (cmpx_t*)VAL(value);

        return cmpx_cmp_si(val, 0) != 0;
    }
    if (val_type(value, BOOL_V))
    {
        bool_t *val = (bool_t*)VAL(value);

        return val->id;
    }
    if (val_type(value, NONE_V))
        return 0;
    if (val_type(value, STR_V))
    {
        str_t *val = (str_t*)VAL(value);

        return str_len(val) != 0;
    }
    if (val_type(value, LIST_V))
    {
        list_t *val = (list_t*)VAL(value);

        return val->elems_n != 0;
    }
    if (val_type(value, TUPLE_V))
    {
        tuple_t *val = (tuple_t*)VAL(value);

        return val->elems_n != 0;
    }

    un_crash("val_logic function: invalid value type `%d`", TYP(value));
}

int val_type(val_t value, enum val_t type)
{
    return TYP(value) == type;
}
