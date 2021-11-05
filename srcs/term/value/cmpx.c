#include <stdio.h>
#include <string.h>

#include "cmpx.h"
#include "../../macro.h"
#include "valm.h"

cmpx_t *cmpx_set(mpc_t num)
{
    cmpx_t *val;
    alloc(val, cmpx_t, 1);
    *NUM(val) = *num;

    return val;
}

cmpx_t *cmpx_set_str(const char *str, int base)
{
    char *newstr;
    alloc(newstr, char, strlen(str) + 6);
    sprintf(newstr, "(0 +%s)", str);

    mpc_t num;
    mpc_init3(num, CPREC_BIT, CPREC_BIT);
    mpc_strtoc(num, newstr, NULL, 10, CRND_FORM);

    fre(newstr);
    return cmpx_set(num);
}

cmpx_t *cmpx_cpy(const cmpx_t *val)
{
    mpc_t num;
    mpc_init3(num, CPREC_BIT, CPREC_BIT);
    mpc_set(num, NUM(val), CRND_FORM);

    return cmpx_set(num);
}

void cmpx_disp(const cmpx_t *val)
{
    mpfr_t realf, imagf;
    mpfr_inits2(PREC_BIT, realf, imagf, NULL);
    mpc_real(realf, NUM(val), CRND_FORM);
    mpc_imag(imagf, NUM(val), CRND_FORM);

    float_t *real = float_set(realf);
    float_t *imag = float_set(imagf);

    if (float_sgn(real))
    {
        if (float_sgn(imag))
        {
            printf("(");
            float_disp(real);
            if (float_sgn(imag) == 1)
                printf("+");
            float_disp(imag);
            printf("i)");
        }
        else
            float_disp(real);
    }
    else
    {
        if (float_sgn(imag))
        {
            float_disp(imag);
            printf("i");
        }
        else
            printf("0");
    }

    float_free(real);
    float_free(imag);
}

void cmpx_free(cmpx_t *val)
{
    mpc_clear(NUM(val));
    fre(val);
}

cmpx_t *cmpx_add(const cmpx_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_add(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_add_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op2), CRND_FORM);

    mpc_add(prod, NUM(op1), prod, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_add_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_add_fr(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_add_ui(const cmpx_t *op1, unsigned long op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_add_ui(prod, NUM(op1), op2, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_sub(const cmpx_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_sub(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_sub_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op2), CRND_FORM);

    mpc_sub(prod, NUM(op1), prod, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_sub_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_sub_fr(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_sub_ui(const cmpx_t *op1, unsigned long op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_sub_ui(prod, NUM(op1), op2, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_mul(const cmpx_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_mul(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_mul_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op2), CRND_FORM);

    mpc_mul(prod, NUM(op1), prod, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_mul_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_mul_fr(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_div(const cmpx_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_div(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_div_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op2), CRND_FORM);

    mpc_div(prod, NUM(op1), prod, CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_div_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_div_fr(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_pow(const cmpx_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_pow(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_pow_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_pow_z(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_pow_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_pow_fr(prod, NUM(op1), NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

bool_t *cmpx_equal(const cmpx_t *op1, const cmpx_t *op2)
{
    int prod = !cmpx_cmp(op1, op2);

    return bool_set(prod);
}

bool_t *cmpx_equal_int(const cmpx_t *op1, const int_t *op2)
{
    int prod = !cmpx_cmp_int(op1, op2);

    return bool_set(prod);
}

bool_t *cmpx_equal_float(const cmpx_t *op1, const float_t *op2)
{
    int prod = !cmpx_cmp_float(op1, op2);

    return bool_set(prod);
}

bool_t *cmpx_nequal(const cmpx_t *op1, const cmpx_t *op2)
{
    int prod = cmpx_cmp(op1, op2);

    return bool_set(prod);
}

bool_t *cmpx_nequal_int(const cmpx_t *op1, const int_t *op2)
{
    int prod = cmpx_cmp_int(op1, op2);

    return bool_set(prod);
}

bool_t *cmpx_nequal_float(const cmpx_t *op1, const float_t *op2)
{
    int prod = cmpx_cmp_float(op1, op2);

    return bool_set(prod);
}

cmpx_t *cmpx_pos(const cmpx_t *op)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_set(prod, NUM(op), RND_FORM);

    return cmpx_set(prod);
}

cmpx_t *cmpx_neg(const cmpx_t *op)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);

    mpc_neg(prod, NUM(op), RND_FORM);

    return cmpx_set(prod);
}

int cmpx_cmp(const cmpx_t *op1, const cmpx_t *op2)
{
    return mpc_cmp(NUM(op1), NUM(op2));
}

int cmpx_cmp_int(const cmpx_t *op1, const int_t *op2)
{
    mpc_t op2c;
    mpc_init3(op2c, CPREC_BIT, CPREC_BIT);
    mpc_set_z(op2c, NUM(op2), CRND_FORM);

    int prod = mpc_cmp(NUM(op1), op2c);

    mpc_clear(op2c);
    return prod;
}

int cmpx_cmp_float(const cmpx_t *op1, const float_t *op2)
{
    mpc_t op2c;
    mpc_init3(op2c, CPREC_BIT, CPREC_BIT);
    mpc_set_fr(op2c, NUM(op2), CRND_FORM);

    int prod = mpc_cmp(NUM(op1), op2c);

    mpc_clear(op2c);
    return prod;
}

int cmpx_cmp_si(const cmpx_t *op1, long op2)
{
    int prod = mpc_cmp_si(NUM(op1), op2);

    return prod;
}
