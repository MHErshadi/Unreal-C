#include "operation.h"
#include "../macro.h"
#include "value/valm.h"

void op_succ(ores_t *op, val_t value);
void op_fail(ores_t *op, runtime_err_t error);

ores_t fl_res(val_t op1, val_t op2, struct __ctx *ctx);
ores_t tr_res(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_ill_op(const char *op_str, val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_div_zero(val_t op2, struct __ctx *ctx);
ores_t b_overflow(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_idx_out(val_t op2, struct __ctx *ctx);

ores_t u_ill_op(const char *op_str, val_t op, struct __ctx *ctx);
ores_t u_sub_scr(val_t op, struct __ctx *ctx);

ores_t b_add(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_add(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = int_add_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = int_add_cmpx(op1i, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            float_t *val = float_add_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = float_add(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = float_add_cmpx(op1f, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            cmpx_t *val = cmpx_add_int(op1c, op2i);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            cmpx_t *val = cmpx_add_float(op1c, op2f);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = cmpx_add(op1c, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, STR_V))
        {
            str_t *op2s = (str_t*)VAL(op2);

            str_t *val = str_concat(op1s, op2s);
            val_t value = set_val(STR_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            str_free(op1s);
            str_free(op2s);
            return res;
        }

        str_free(op1s);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        list_t *val = list_append(op1l, op2);
        val_t value = set_val(LIST_V, val, ctx, POSS(op1), POSE(op2));
        op_succ(&res, value);

        list_free(op1l);
        free_val(op2);
        return res;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("+", op1, op2, ctx);
}

ores_t b_sub(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_sub(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = int_sub_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = int_sub_cmpx(op1i, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            float_t *val = float_sub_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = float_sub(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = float_sub_cmpx(op1f, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            cmpx_t *val = cmpx_sub_int(op1c, op2i);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            cmpx_t *val = cmpx_sub_float(op1c, op2f);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = cmpx_sub(op1c, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int cmp_ull;
            if (int_sgn(op2i) != -1)
                cmp_ull = int_cmp_ull(op2i, str_len(op1s)) != -1;
            else
                cmp_ull = int_cmp_ull(op2i, str_len(op1s)) == 1;
            if (cmp_ull)
            {
                str_free(op1s);
                int_free(op2i);
                goto idx_out;
            }

            str_t *val = str_rem(op1s, op2i);
            val_t value = set_val(STR_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            str_free(op1s);
            int_free(op2i);
            return res;
        }

        str_free(op1s);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int cmp_ull;
            if (int_sgn(op2i) != -1)
                cmp_ull = int_cmp_ull(op2i, op1l->elems_n) != -1;
            else
                cmp_ull = int_cmp_ull(op2i, op1l->elems_n) == 1;
            if (cmp_ull)
            {
                list_free(op1l);
                int_free(op2i);
                goto idx_out;
            }

            list_t *val = list_rem(op1l, op2i);
            val_t value = set_val(LIST_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            list_free(op1l);
            int_free(op2i);
            return res;
        }

        list_free(op1l);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("-", op1, op2, ctx);

    idx_out:
    return b_idx_out(op2, ctx);
}

ores_t b_mul(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_mul(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = int_mul_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = int_mul_cmpx(op1i, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            float_t *val = float_mul_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            float_t *val = float_mul(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = float_mul_cmpx(op1f, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            cmpx_t *val = cmpx_mul_int(op1c, op2i);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            cmpx_t *val = cmpx_mul_float(op1c, op2f);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = cmpx_mul(op1c, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_fits_ull(op2i))
            {
                str_free(op1s);
                int_free(op2i);
                goto overflow;
            }

            str_t *val = str_repeat(op1s, op2i);
            val_t value = set_val(STR_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            str_free(op1s);
            int_free(op2i);
            return res;
        }

        str_free(op1s);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_fits_ull(op2i))
            {
                list_free(op1l);
                int_free(op2i);
                goto overflow;
            }

            list_t *val = list_repeat(op1l, op2i);
            val_t value = set_val(LIST_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            list_free(op1l);
            int_free(op2i);
            return res;
        }

        list_free(op1l);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("*", op1, op2, ctx);

    overflow:
    return b_overflow(op1, op2, ctx);
}

ores_t b_div(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                int_free(op1i);
                int_free(op2i);
                goto div_zero;
            }

            float_t *val = int_div(op1i, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                int_free(op1i);
                float_free(op2f);
                goto div_zero;
            }

            float_t *val = int_div_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            if (!cmpx_cmp_si(op2c, 0))
            {
                int_free(op1i);
                cmpx_free(op2c);
                goto div_zero;
            }

            cmpx_t *val = int_div_cmpx(op1i, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                float_free(op1f);
                int_free(op2i);
                goto div_zero;
            }

            float_t *val = float_div_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                float_free(op1f);
                float_free(op2f);
                goto div_zero;
            }

            float_t *val = float_div(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            if (!cmpx_cmp_si(op2c, 0))
            {
                float_free(op1f);
                cmpx_free(op2c);
                goto div_zero;
            }

            cmpx_t *val = float_div_cmpx(op1f, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            cmpx_t *val = cmpx_div_int(op1c, op2i);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            cmpx_t *val = cmpx_div_float(op1c, op2f);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = cmpx_div(op1c, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("/", op1, op2, ctx);

    div_zero:
    return b_div_zero(op2, ctx);
}

ores_t b_mod(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                int_free(op1i);
                int_free(op2i);
                goto div_zero;
            }

            int_t *val = int_mod(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                int_free(op1i);
                float_free(op2f);
                goto div_zero;
            }

            float_t *val = int_mod_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                float_free(op1f);
                int_free(op2i);
                goto div_zero;
            }

            float_t *val = float_mod_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                float_free(op1f);
                float_free(op2f);
                goto div_zero;
            }

            float_t *val = float_mod(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("%%", op1, op2, ctx);

    div_zero:
    return b_div_zero(op2, ctx);
}

ores_t b_quot(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                int_free(op1i);
                int_free(op2i);
                goto div_zero;
            }

            int_t *val = int_quot(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                int_free(op1i);
                float_free(op2f);
                goto div_zero;
            }

            int_t *val = int_quot_float(op1i, op2f);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (!int_sgn(op2i))
            {
                float_free(op1f);
                int_free(op2i);
                goto div_zero;
            }

            int_t *val = float_quot_int(op1f, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (!float_sgn(op2f))
            {
                float_free(op1f);
                float_free(op2f);
                goto div_zero;
            }

            int_t *val = float_quot(op1f, op2f);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("//", op1, op2, ctx);

    div_zero:
    return b_div_zero(op2, ctx);
}

ores_t b_pow(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            if (int_sgn(op2i) == -1)
            {
                float_t *val = int_pown(op1i, op2i);
                val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
                op_succ(&res, value);

                int_free(op1i);
                int_free(op2i);
                return res;
            }

            if (!int_fits_ui(op2i))
            {
                int_free(op1i);
                int_free(op2i);
                goto overflow;
            }

            int_t *val = int_pow(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (int_sgn(op1i) == -1)
            {
                cmpx_t *val = int_npow(op1i, op2f);
                val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
                op_succ(&res, value);

                int_free(op1i);
                float_free(op2f);
                return res;
            }

            float_t *val = int_pow_float(op1i, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = int_pow_cmpx(op1i, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            float_t *val = float_pow_int(op1f, op2i);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            if (float_sgn(op1f) == -1)
            {
                cmpx_t *val = float_npow(op1f, op2f);
                val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
                op_succ(&res, value);

                float_free(op1f);
                float_free(op2f);
                return res;
            }

            float_t *val = float_pow(op1f, op2f);
            val_t value = set_val(FLOAT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = float_pow_cmpx(op1f, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            cmpx_t *val = cmpx_pow_int(op1c, op2i);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            cmpx_t *val = cmpx_pow_float(op1c, op2f);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            cmpx_t *val = cmpx_pow(op1c, op2c);
            val_t value = set_val(CMPX_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("**", op1, op2, ctx);

    overflow:
    return b_overflow(op1, op2, ctx);
}

ores_t b_lshift(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_lshift(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("<<", op1, op2, ctx);
}

ores_t b_rshift(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_rshift(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op(">>", op1, op2, ctx);
}

ores_t b_band(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_band(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("&", op1, op2, ctx);
}

ores_t b_bior(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_bior(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("|", op1, op2, ctx);
}

ores_t b_bxor(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            int_t *val = int_bxor(op1i, op2i);
            val_t value = set_val(INT_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("^", op1, op2, ctx);
}

ores_t b_equal(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_equal(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_equal_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = int_equal_cmpx(op1i, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_equal_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_equal(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = float_equal_cmpx(op1f, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = cmpx_equal_int(op1c, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = cmpx_equal_float(op1c, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = cmpx_equal(op1c, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, BOOL_V))
    {
        bool_t *op1b = (bool_t*)VAL(op1);

        if (val_type(op2, BOOL_V))
        {
            bool_t *op2b = (bool_t*)VAL(op2);

            bool_t *val = bool_equal(op1b, op2b);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            bool_free(op1b);
            bool_free(op2b);
            return res;
        }

        bool_free(op1b);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, NONE_V))
    {
        if (val_type(op2, NONE_V))
            goto tr_res;
        goto fl_res;
    }

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, STR_V))
        {
            str_t *op2s = (str_t*)VAL(op2);

            bool_t *val = str_equal(op1s, op2s);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            str_free(op1s);
            str_free(op2s);
            return res;
        }

        str_free(op1s);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        if (val_type(op2, LIST_V))
        {
            list_t *op2l = (list_t*)VAL(op2);

            bool_t *val = list_equal(op1l, op2l);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            list_free(op1l);
            list_free(op2l);
            return res;
        }

        list_free(op1l);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, TUPLE_V))
    {
        tuple_t *op1t = (tuple_t*)VAL(op1);

        if (val_type(op2, TUPLE_V))
        {
            tuple_t *op2t = (tuple_t*)VAL(op2);

            bool_t *val = tuple_equal(op1t, op2t);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            tuple_free(op1t);
            tuple_free(op2t);
            return res;
        }

        tuple_free(op1t);
        free_val(op2);
        goto fl_res;
    }

    free_val(op1);
    free_val(op2);
    goto fl_res;

    fl_res:
    return fl_res(op1, op2, ctx);

    tr_res:
    return tr_res(op1, op2, ctx);
}

ores_t b_nequal(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_nequal(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_nequal_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = int_nequal_cmpx(op1i, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            cmpx_free(op2c);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_nequal_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_nequal(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = float_nequal_cmpx(op1f, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            cmpx_free(op2c);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = cmpx_nequal_int(op1c, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = cmpx_nequal_float(op1c, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            float_free(op2f);
            return res;
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);

            bool_t *val = cmpx_nequal(op1c, op2c);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            cmpx_free(op1c);
            cmpx_free(op2c);
            return res;
        }

        cmpx_free(op1c);
        free_val(op2);
        goto fl_res;
    }

    if (val_type(op1, BOOL_V))
    {
        bool_t *op1b = (bool_t*)VAL(op1);

        if (val_type(op2, BOOL_V))
        {
            bool_t *op2b = (bool_t*)VAL(op2);

            bool_t *val = bool_nequal(op1b, op2b);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            bool_free(op1b);
            bool_free(op2b);
            return res;
        }

        bool_free(op1b);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, NONE_V))
    {
        if (val_type(op2, NONE_V))
            goto fl_res;
        goto tr_res;
    }

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, STR_V))
        {
            str_t *op2s = (str_t*)VAL(op2);

            bool_t *val = str_nequal(op1s, op2s);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            str_free(op1s);
            str_free(op2s);
            return res;
        }

        str_free(op1s);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        if (val_type(op2, LIST_V))
        {
            list_t *op2l = (list_t*)VAL(op2);

            bool_t *val = list_nequal(op1l, op2l);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            list_free(op1l);
            list_free(op2l);
            return res;
        }

        list_free(op1l);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, TUPLE_V))
    {
        tuple_t *op1t = (tuple_t*)VAL(op1);

        if (val_type(op2, TUPLE_V))
        {
            tuple_t *op2t = (tuple_t*)VAL(op2);

            bool_t *val = tuple_nequal(op1t, op2t);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            tuple_free(op1t);
            tuple_free(op2t);
            return res;
        }

        tuple_free(op1t);
        free_val(op2);
        goto tr_res;
    }

    free_val(op1);
    free_val(op2);
    goto tr_res;

    tr_res:
    return tr_res(op1, op2, ctx);

    fl_res:
    return fl_res(op1, op2, ctx);
}

ores_t b_less(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_less(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_less_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_less_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_less(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("<", op1, op2, ctx);
}

ores_t b_great(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_great(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_great_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_great_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_great(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op(">", op1, op2, ctx);
}

ores_t b_lesseq(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_lesseq(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_lesseq_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_lesseq_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_lesseq(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op("<=", op1, op2, ctx);
}

ores_t b_greateq(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = int_greateq(op1i, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = int_greateq_float(op1i, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            int_free(op1i);
            float_free(op2f);
            return res;
        }

        int_free(op1i);
        free_val(op2);
        goto ill_op;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);

            bool_t *val = float_greateq_int(op1f, op2i);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            int_free(op2i);
            return res;
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);

            bool_t *val = float_greateq(op1f, op2f);
            val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
            op_succ(&res, value);

            float_free(op1f);
            float_free(op2f);
            return res;
        }

        float_free(op1f);
        free_val(op2);
        goto ill_op;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    ill_op:
    return b_ill_op(">=", op1, op2, ctx);
}

ores_t b_and(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(val_logic(op1) & val_logic(op2));
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);

    free_val(op1);
    free_val(op2);
    return res;
}

ores_t b_ior(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(val_logic(op1) | val_logic(op2));
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);

    free_val(op1);
    free_val(op2);
    return res;
}

ores_t b_xor(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(val_logic(op1) ^ val_logic(op2));
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);

    free_val(op1);
    free_val(op2);
    return res;
}

ores_t b_is(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(TYP(op1) == TYP(op2));
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);

    free_val(op1);
    free_val(op2);
    return res;
}

ores_t b_are(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        for (int i = 0; i < op1l->elems_n; i++)
            if (TYP(ELM(op1l)[i]) != TYP(op2))
            {
                list_free(op1l);
                free_val(op2);
                goto fl_res;
            }

        list_free(op1l);
        free_val(op2);
        goto tr_res;
    }

    if (val_type(op1, TUPLE_V))
    {
        tuple_t *op1t = (tuple_t*)VAL(op1);

        for (int i = 0; i < op1t->elems_n; i++)
            if (TYP(ELM(op1t)[i]) != TYP(op2))
            {
                tuple_free(op1t);
                free_val(op2);
                goto fl_res;
            }

        tuple_free(op1t);
        free_val(op2);
        goto tr_res;
    }

    free_val(op1);
    free_val(op2);
    goto ill_op;

    fl_res:
    return fl_res(op1, op2, ctx);

    tr_res:
    return tr_res(op1, op2, ctx);

    ill_op:
    return b_ill_op("are", op1, op2, ctx);
}

ores_t u_pos(val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, INT_V))
    {
        int_t *opi = (int_t*)VAL(op);

        int_t *val = int_pos(opi);
        val_t value = set_val(INT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        int_free(opi);
        return res;
    }

    if (val_type(op, FLOAT_V))
    {
        float_t *opf = (float_t*)VAL(op);

        float_t *val = float_pos(opf);
        val_t value = set_val(FLOAT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        float_free(opf);
        return res;
    }

    if (val_type(op, CMPX_V))
    {
        cmpx_t *opc = (cmpx_t*)VAL(op);

        cmpx_t *val = cmpx_pos(opc);
        val_t value = set_val(CMPX_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        cmpx_free(opc);
        return res;
    }

    free_val(op);
    goto ill_op;

    ill_op:
    return u_ill_op("+", op, ctx);
}

ores_t u_neg(val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, INT_V))
    {
        int_t *opi = (int_t*)VAL(op);

        int_t *val = int_neg(opi);
        val_t value = set_val(INT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        int_free(opi);
        return res;
    }

    if (val_type(op, FLOAT_V))
    {
        float_t *opf = (float_t*)VAL(op);

        float_t *val = float_neg(opf);
        val_t value = set_val(FLOAT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        float_free(opf);
        return res;
    }

    if (val_type(op, CMPX_V))
    {
        cmpx_t *opc = (cmpx_t*)VAL(op);

        cmpx_t *val = cmpx_neg(opc);
        val_t value = set_val(CMPX_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        cmpx_free(opc);
        return res;
    }

    free_val(op);
    goto ill_op;

    ill_op:
    return u_ill_op("-", op, ctx);
}

ores_t u_bnot(val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, INT_V))
    {
        int_t *opi = (int_t*)VAL(op);

        int_t *val = int_bnot(opi);
        val_t value = set_val(INT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        int_free(opi);
        return res;
    }

    free_val(op);
    goto ill_op;

    ill_op:
    return u_ill_op("~", op, ctx);
}

ores_t u_not(val_t op, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(!val_logic(op));
    val_t value = set_val(BOOL_V, val, ctx, POSS(op), POSE(op));
    op_succ(&res, value);

    free_val(op);
    return res;
}

ores_t u_idx(val_t op, val_t idxs, pos_t pose, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, STR_V))
    {
        str_t *ops = (str_t*)VAL(op);

        if (val_type(idxs, INT_V))
        {
            int_t *idxsi = (int_t*)VAL(idxs);

            int cmp_ull;
            if (int_sgn(idxsi) != -1)
                cmp_ull = int_cmp_ull(idxsi, str_len(ops)) != -1;
            else
                cmp_ull = int_cmp_ull(idxsi, str_len(ops)) == 1;
            if (cmp_ull)
            {
                str_free(ops);
                int_free(idxsi);
                goto idx_out;
            }

            str_t *val = str_idx(ops, idxsi);
            val_t value = set_val(STR_V, val, ctx, POSS(op), pose);
            op_succ(&res, value);

            str_free(ops);
            int_free(idxsi);
            return res;
        }

        str_free(ops);
        free_val(idxs);
        goto inv_type;
    }

    if (val_type(op, LIST_V))
    {
        list_t *opl = (list_t*)VAL(op);

        if (val_type(idxs, INT_V))
        {
            int_t *idxsi = (int_t*)VAL(idxs);

            int cmp_ull;
            if (int_sgn(idxsi) != -1)
                cmp_ull = int_cmp_ull(idxsi, opl->elems_n) != -1;
            else
                cmp_ull = int_cmp_ull(idxsi, opl->elems_n) == 1;
            if (cmp_ull)
            {
                list_free(opl);
                int_free(idxsi);
                goto idx_out;
            }

            val_t value = list_idx(opl, idxsi);
            op_succ(&res, value);

            list_free(opl);
            int_free(idxsi);
            return res;
        }

        list_free(opl);
        free_val(idxs);
        goto inv_type;
    }

    if (val_type(op, TUPLE_V))
    {
        tuple_t *opt = (tuple_t*)VAL(op);

        if (val_type(idxs, INT_V))
        {
            int_t *idxsi = (int_t*)VAL(idxs);

            int cmp_ull;
            if (int_sgn(idxsi) != -1)
                cmp_ull = int_cmp_ull(idxsi, opt->elems_n) != -1;
            else
                cmp_ull = int_cmp_ull(idxsi, opt->elems_n) == 1;
            if (cmp_ull)
            {
                tuple_free(opt);
                int_free(idxsi);
                goto idx_out;
            }

            val_t value = tuple_idx(opt, idxsi);
            op_succ(&res, value);

            tuple_free(opt);
            int_free(idxsi);
            return res;
        }

        tuple_free(opt);
        free_val(idxs);
        goto inv_type;
    }

    free_val(op);
    free_val(idxs);
    goto sub_scr;

    sub_scr:
    return u_sub_scr(op, ctx);

    idx_out:
    return b_idx_out(idxs, ctx);

    inv_type:
    do
    {
        runtime_err_t error = set_runtime_err("Index can not be <%s>",
            INV_TYPE_E, ctx, POSS(idxs), POSE(idxs), types_str[TYP(idxs) - 1]);
        op_fail(&res, error);
        return res;
    } while (0);
}

ores_t u_inc(val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, INT_V))
    {
        int_t *opi = (int_t*)VAL(op);

        int_t *val = int_add_ui(opi, 1);
        val_t value = set_val(INT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        int_free(opi);
        return res;
    }

    if (val_type(op, FLOAT_V))
    {
        float_t *opf = (float_t*)VAL(op);

        float_t *val = float_add_ui(opf, 1);
        val_t value = set_val(FLOAT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        float_free(opf);
        return res;
    }

    if (val_type(op, CMPX_V))
    {
        cmpx_t *opc = (cmpx_t*)VAL(op);

        cmpx_t *val = cmpx_add_ui(opc, 1);
        val_t value = set_val(CMPX_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        cmpx_free(opc);
        return res;
    }

    free_val(op);
    goto ill_op;

    ill_op:
    return u_ill_op("++", op, ctx);
}

ores_t u_dec(val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (val_type(op, INT_V))
    {
        int_t *opi = (int_t*)VAL(op);

        int_t *val = int_sub_ui(opi, 1);
        val_t value = set_val(INT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        int_free(opi);
        return res;
    }

    if (val_type(op, FLOAT_V))
    {
        float_t *opf = (float_t*)VAL(op);

        float_t *val = float_sub_ui(opf, 1);
        val_t value = set_val(FLOAT_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        float_free(opf);
        return res;
    }

    if (val_type(op, CMPX_V))
    {
        cmpx_t *opc = (cmpx_t*)VAL(op);

        cmpx_t *val = cmpx_sub_ui(opc, 1);
        val_t value = set_val(CMPX_V, val, ctx, POSS(op), POSE(op));
        op_succ(&res, value);

        cmpx_free(opc);
        return res;
    }

    free_val(op);
    goto ill_op;

    ill_op:
    return u_ill_op("--", op, ctx);
}

int cmp_op(val_t op1, val_t op2)
{
    if (val_type(op1, INT_V))
    {
        int_t *op1i = (int_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);
            return !int_cmp(op1i, op2i);
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);
            return !int_cmp_float(op1i, op2f);
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);
            return !int_cmp_cmpx(op1i, op2c);
        }

        return 0;
    }

    if (val_type(op1, FLOAT_V))
    {
        float_t *op1f = (float_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);
            return !float_cmp_int(op1f, op2i);
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);
            return !float_cmp(op1f, op2f);
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);
            return !float_cmp_cmpx(op1f, op2c);
        }

        return 0;
    }

    if (val_type(op1, CMPX_V))
    {
        cmpx_t *op1c = (cmpx_t*)VAL(op1);

        if (val_type(op2, INT_V))
        {
            int_t *op2i = (int_t*)VAL(op2);
            return !cmpx_cmp_int(op1c, op2i);
        }

        if (val_type(op2, FLOAT_V))
        {
            float_t *op2f = (float_t*)VAL(op2);
            return !cmpx_cmp_float(op1c, op2f);
        }

        if (val_type(op2, CMPX_V))
        {
            cmpx_t *op2c = (cmpx_t*)VAL(op2);
            return !cmpx_cmp(op1c, op2c);
        }

        return 0;
    }

    if (val_type(op1, BOOL_V))
    {
        bool_t *op1b = (bool_t*)VAL(op1);

        if (val_type(op2, BOOL_V))
        {
            bool_t *op2b = (bool_t*)VAL(op2);
            return op1b->id == op2b->id;
        }

        return 0;
    }

    if (val_type(op1, NONE_V))
        return val_type(op2, NONE_V);

    if (val_type(op1, STR_V))
    {
        str_t *op1s = (str_t*)VAL(op1);

        if (val_type(op2, STR_V))
        {
            str_t *op2s = (str_t*)VAL(op2);
            return str_cmp(op1s, op2s);
        }

        return 0;
    }

    if (val_type(op1, LIST_V))
    {
        list_t *op1l = (list_t*)VAL(op1);

        if (val_type(op2, LIST_V))
        {
            list_t *op2l = (list_t*)VAL(op1);
            return list_cmp(op1l, op2l);
        }

        return 0;
    }

    if (val_type(op1, TUPLE_V))
    {
        tuple_t *op1t = (tuple_t*)VAL(op1);

        if (val_type(op2, TUPLE_V))
        {
            tuple_t *op2t = (tuple_t*)VAL(op1);
            return tuple_cmp(op1t, op2t);
        }

        return 0;
    }

    return 0;
}

void op_succ(ores_t *op, val_t value)
{
    HERR(*op) = 0;
    VAL(*op) = value;
}

void op_fail(ores_t *op, runtime_err_t error)
{
    HERR(*op) = 1;
    ERR(*op) = error;
}

ores_t fl_res(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(0);
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);
    return res;
}

ores_t tr_res(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    bool_t *val = bool_set(1);
    val_t value = set_val(BOOL_V, val, ctx, POSS(op1), POSE(op2));
    op_succ(&res, value);
    return res;
}

ores_t b_ill_op(const char *op_str, val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    if (!TYP(op1))
    {
        runtime_err_t error = set_runtime_err("Operator1 is not assigned yet",
            NOT_ASGN_E, ctx, POSS(op1), POSE(op1));
        op_fail(&res, error);
        return res;
    }
    if (!TYP(op2))
    {
        runtime_err_t error = set_runtime_err("Operator2 is not assigned yet",
            NOT_ASGN_E, ctx, POSS(op2), POSE(op2));
        op_fail(&res, error);
        return res;
    }

    runtime_err_t error = set_runtime_err("Illegal operation `%s` between <%s> and <%s>",
        ILL_OP_E, ctx, POSS(op1), POSE(op2), op_str, types_str[TYP(op1) - 1], types_str[TYP(op2) - 1]);
    op_fail(&res, error);
    return res;
}

ores_t b_div_zero(val_t op2, struct __ctx *ctx)
{
    ores_t res;

    runtime_err_t error = set_runtime_err("Division by zero",
        DIV_BY_ZERO_E, ctx, POSS(op2), POSE(op2));
    op_fail(&res, error);
    return res;
}

ores_t b_overflow(val_t op1, val_t op2, struct __ctx *ctx)
{
    ores_t res;

    runtime_err_t error = set_runtime_err("Memory overflow",
        MEM_OVERFLOW_E, ctx, POSS(op1), POSE(op2));
    op_fail(&res, error);
    return res;
}

ores_t b_idx_out(val_t op2, struct __ctx *ctx)
{
    ores_t res;

    runtime_err_t error = set_runtime_err("Index out of range",
        IDX_OUT_RNG_E, ctx, POSS(op2), POSE(op2));
    op_fail(&res, error);
    return res;
}

ores_t u_ill_op(const char *op_str, val_t op, struct __ctx *ctx)
{
    ores_t res;

    if (!TYP(op))
    {
        runtime_err_t error = set_runtime_err("Operator is not assigned yet",
            NOT_ASGN_E, ctx, POSS(op), POSE(op));
        op_fail(&res, error);
        return res;
    }

    runtime_err_t error = set_runtime_err("Illegal operation `%s` for <%s>",
        ILL_OP_E, ctx, POSS(op), POSE(op), op_str, types_str[TYP(op) - 1]);
    op_fail(&res, error);
    return res;
}

ores_t u_sub_scr(val_t op, struct __ctx *ctx)
{
    ores_t res;

    runtime_err_t error = set_runtime_err("<%s> is not subscriptable",
        SUB_SCR_E, ctx, POSS(op), POSE(op), types_str[TYP(op) - 1]);
    op_fail(&res, error);
    return res;
}
