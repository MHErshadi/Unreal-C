#include <stdio.h>
#include <string.h>

#include "float.h"
#include "../../macro.h"
#include "valm.h"

float_t *float_set(mpfr_t num)
{
    float_t *val;
    alloc(val, float_t, 1);
    *NUM(val) = *num;

    return val;
}

float_t *float_set_str(const char *str, int base)
{
    mpfr_t num;
    mpfr_init2(num, PREC_BIT);
    mpfr_set_str(num, str, base, RND_FORM);

    return float_set(num);
}

float_t *float_cpy(const float_t *val)
{
    mpfr_t num;
    mpfr_init_set(num, NUM(val), RND_FORM);

    return float_set(num);
}

void float_disp(const float_t *val)
{
    char *str;
    alloc(str, char, PREC_BIT + 1);
    mpfr_sprintf(str, "%.*Rg", PREC_SHW, NUM(val));

    char *end = str + strlen(str) - 1;
    if (strchr(str, '.'))
        for (; end > str && *end == '0'; end--);
    if (*end == '.')
        end--;
    end[1] = '\0';

    printf("%s", str);

    fre(str);
}

void float_free(float_t *val)
{
    mpfr_clear(NUM(val));
    fre(val);
}

float_t *float_add(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_add(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_add_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_add_z(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *float_add_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_add_fr(prod, NUM(op2), NUM(op1), CRND_FORM);

    return cmpx_set(prod);
}

float_t *float_add_ui(const float_t *op1, unsigned long op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_add_ui(prod, NUM(op1), op2, RND_FORM);

    return float_set(prod);
}

float_t *float_sub(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_sub(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_sub_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_sub_z(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *float_sub_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_fr_sub(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

float_t *float_sub_ui(const float_t *op1, unsigned long op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_sub_ui(prod, NUM(op1), op2, RND_FORM);

    return float_set(prod);
}

float_t *float_mul(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_mul(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_mul_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_mul_z(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *float_mul_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_mul_fr(prod, NUM(op2), NUM(op1), CRND_FORM);

    return cmpx_set(prod);
}

float_t *float_div(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_div(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_div_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_div_z(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *float_div_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_fr_div(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

float_t *float_mod(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_fmod(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_mod_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op2), RND_FORM);

    mpfr_fmod(prod, NUM(op1), prod, RND_FORM);

    return float_set(prod);
}

int_t *float_quot(const float_t *op1, const float_t *op2)
{
    mpfr_t prodf;
    mpfr_init2(prodf, PREC_BIT);

    mpfr_div(prodf, NUM(op1), NUM(op2), RND_FORM);

    mpz_t prod;
    mpz_init(prod);
    mpfr_get_z(prod, prodf, MPFR_RNDD);

    mpfr_clear(prodf);
    return int_set(prod);
}

int_t *float_quot_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prodf;
    mpfr_init2(prodf, PREC_BIT);

    mpfr_div_z(prodf, NUM(op1), NUM(op2), RND_FORM);

    mpz_t prod;
    mpz_init(prod);
    mpfr_get_z(prod, prodf, MPFR_RNDD);

    mpfr_clear(prodf);
    return int_set(prod);
}

float_t *float_pow(const float_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_pow(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *float_pow_int(const float_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_pow_z(prod, NUM(op1), NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *float_pow_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_fr(prod, NUM(op1), CRND_FORM);

    mpc_pow(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *float_npow(const float_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_fr(prod, NUM(op1), CRND_FORM);

    mpc_pow_fr(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

bool_t *float_equal(const float_t *op1, const float_t *op2)
{
    int prod = mpfr_equal_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_equal_int(const float_t *op1, const int_t *op2)
{
    int prod = !float_cmp_int(op1, op2);

    return bool_set(prod);
}

bool_t *float_equal_cmpx(const float_t *op1, const cmpx_t *op2)
{
    int prod = !float_cmp_cmpx(op1, op2);

    return bool_set(prod);
}

bool_t *float_nequal(const float_t *op1, const float_t *op2)
{
    int prod = !mpfr_equal_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_nequal_int(const float_t *op1, const int_t *op2)
{
    int prod = float_cmp_int(op1, op2);

    return bool_set(prod);
}

bool_t *float_nequal_cmpx(const float_t *op1, const cmpx_t *op2)
{
    int prod = float_cmp_cmpx(op1, op2);

    return bool_set(prod);
}

bool_t *float_less(const float_t *op1, const float_t *op2)
{
    int prod = mpfr_less_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_less_int(const float_t *op1, const int_t *op2)
{
    int prod = float_cmp_int(op1, op2) < 0;

    return bool_set(prod);
}

bool_t *float_great(const float_t *op1, const float_t *op2)
{
    int prod = mpfr_greater_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_great_int(const float_t *op1, const int_t *op2)
{
    int prod = float_cmp_int(op1, op2) > 0;

    return bool_set(prod);
}

bool_t *float_lesseq(const float_t *op1, const float_t *op2)
{
    int prod = mpfr_lessequal_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_lesseq_int(const float_t *op1, const int_t *op2)
{
    int prod = float_cmp_int(op1, op2) <= 0;

    return bool_set(prod);
}

bool_t *float_greateq(const float_t *op1, const float_t *op2)
{
    int prod = mpfr_greaterequal_p(NUM(op1), NUM(op2));

    return bool_set(prod);
}

bool_t *float_greateq_int(const float_t *op1, const int_t *op2)
{
    int prod = float_cmp_int(op1, op2) >= 0;

    return bool_set(prod);
}

float_t *float_pos(const float_t *op)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_set(prod, NUM(op), RND_FORM);

    return float_set(prod);
}

float_t *float_neg(const float_t *op)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_neg(prod, NUM(op), RND_FORM);

    return float_set(prod);
}

int float_cmp(const float_t *op1, const float_t *op2)
{
    return mpfr_cmp(NUM(op1), NUM(op2));
}

int float_cmp_int(const float_t *op1, const int_t *op2)
{
    return mpfr_cmp_z(NUM(op1), NUM(op2));
}

int float_cmp_cmpx(const float_t *op1, const cmpx_t *op2)
{
    mpc_t op1c;
    mpc_init3(op1c, CPREC_BIT, CPREC_BIT);
    mpc_set_fr(op1c, NUM(op1), CRND_FORM);

    int prod = mpc_cmp(op1c, NUM(op2));

    mpc_clear(op1c);
    return prod;
}

int float_sgn(const float_t *val)
{
    return mpfr_sgn(NUM(val));
}
