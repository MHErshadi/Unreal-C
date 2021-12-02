#include "str.h"
#include "../../macro.h"
#include "valm.h"

str_t *str_set(ustr_t str)
{
    str_t *val;
    alloc(val, str_t, 1);

    *STR(val) = *str;

    return val;
}

str_t *str_set_str(const char *str)
{
    ustr_t ustr;
    ustr_iniset_str(ustr, str);

    return str_set(ustr);
}

str_t *str_cpy(const str_t *val)
{
    ustr_t ustr;
    ustr_iniset(ustr, STR(val));

    return str_set(ustr);
}

void str_disp(const str_t *val)
{
    printf("\"");
    ustr_puts(STR(val), '"');
}

void str_free(str_t *val)
{
    ustr_free(STR(val));
    fre(val);
}

str_t *str_concat(const str_t *op1, const str_t *op2)
{
    ustr_t prod;
    ustr_init(prod);

    ustr_concat(prod, STR(op1), STR(op2));

    return str_set(prod);
}

str_t *str_rem(const str_t *op1, const int_t *op2)
{
    ustr_t prod;
    ustr_init(prod);

    ustr_rem_char(prod, STR(op1), int_get_sll(op2));

    return str_set(prod);
}

str_t *str_repeat(const str_t *op1, const int_t *op2)
{
    ustr_t prod;
    ustr_init(prod);

    ustr_repeat(prod, STR(op1), int_get_ull(op2));

    return str_set(prod);
}

str_t *str_reverse(const str_t *op)
{
    ustr_t prod;
    ustr_init(prod);

    ustr_reverse(prod, STR(op));

    return str_set(prod);
}

bool_t *str_equal(const str_t *op1, const str_t *op2)
{
    int prod = ustr_equal(STR(op1), STR(op2));

    return bool_set(prod);
}

bool_t *str_nequal(const str_t *op1, const str_t *op2)
{
    int prod = ustr_nequal(STR(op1), STR(op2));

    return bool_set(prod);
}

bool_t *str_has(const str_t *op1, const str_t *op2)
{
    int prod = ustr_have(STR(op1), STR(op2));

    return bool_set(prod);
}

str_t *str_idx(const str_t *op1, const int_t *op2)
{
    ustrpos_s idx = int_get_sll(op2);

    ustr_t prod;
    ustr_iniset_char(prod, ustr_oget_char(STR(op1), idx));

    return str_set(prod);
}

ustr_s str_len(const str_t *val)
{
    return ustr_len(STR(val));
}

int str_cmp(const str_t *op1, const str_t *op2)
{
    return ustr_equal(STR(op1), STR(op2));
}
