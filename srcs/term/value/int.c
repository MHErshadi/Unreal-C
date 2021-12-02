#include "int.h"
#include "../../macro.h"
#include "valm.h"

int_t *int_set(mpz_t num)
{
    int_t *val;
    alloc(val, int_t, 1);
    *NUM(val) = *num;

    return val;
}

int_t *int_set_str(const char *str, int base)
{
    mpz_t num;
    mpz_init_set_str(num, str, base);

    return int_set(num);
}

int_t *int_set_ui(unsigned long ui)
{
    mpz_t num;
    mpz_init_set_ui(num, ui);

    return int_set(num);
}

int_t *int_cpy(const int_t *val)
{
    mpz_t num;
    mpz_init_set(num, NUM(val));

    return int_set(num);
}

unsigned long long int_get_ull(const int_t *val)
{
    unsigned long long ull = 0;
    mpz_export(&ull, NULL, -1, sizeof(unsigned long long), 0, 0, NUM(val));

    return ull;
}

long long int_get_sll(const int_t *val)
{
    long long sll = int_get_ull(val) & 0x7fffffffffffffffLL;
    if (mpz_sgn(NUM(val)) != -1)
        return sll;
    return -sll;
}

void int_disp(const int_t *val)
{
    gmp_printf("%Zd", NUM(val));
}

void int_free(int_t *val)
{
    mpz_clear(NUM(val));
    fre(val);
}

int_t *int_add(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_add(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

float_t *int_add_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_add_z(prod, NUM(op2), NUM(op1), RND_FORM);

    return float_set(prod);
}

cmpx_t *int_add_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_add(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

int_t *int_add_ui(const int_t *op1, unsigned long op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_add_ui(prod, NUM(op1), op2);

    return int_set(prod);
}

void int_addself(int_t *op1, const int_t *op2)
{
    mpz_add(NUM(op1), NUM(op1), NUM(op2));
}

void int_addself_ui(int_t *op1, unsigned long op2)
{
    mpz_add_ui(NUM(op1), NUM(op1), op2);
}

int_t *int_sub(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_sub(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

float_t *int_sub_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_sub_z(prod, NUM(op2), NUM(op1), RND_FORM);
    mpfr_neg(prod, prod, RND_FORM);

    return float_set(prod);
}

cmpx_t *int_sub_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_sub(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

int_t *int_sub_ui(const int_t *op1, unsigned long op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_sub_ui(prod, NUM(op1), op2);

    return int_set(prod);
}

void int_subself(int_t *op1, const int_t *op2)
{
    mpz_sub(NUM(op1), NUM(op1), NUM(op2));
}

void int_subself_ui(int_t *op1, unsigned long op2)
{
    mpz_sub_ui(NUM(op1), NUM(op1), op2);
}

int_t *int_mul(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_mul(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

float_t *int_mul_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);

    mpfr_mul_z(prod, NUM(op2), NUM(op1), RND_FORM);

    return float_set(prod);
}

cmpx_t *int_mul_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_mul(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

float_t *int_div(const int_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op1), RND_FORM);

    mpfr_div_z(prod, prod, NUM(op2), RND_FORM);

    return float_set(prod);
}

float_t *int_div_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op1), RND_FORM);

    mpfr_div(prod, prod, NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *int_div_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_div(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

int_t *int_mod(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_mod(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

float_t *int_mod_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op1), RND_FORM);

    mpfr_fmod(prod, prod, NUM(op2), RND_FORM);

    return float_set(prod);
}

int_t *int_quot(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_fdiv_q(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

int_t *int_quot_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prodf;
    mpfr_init2(prodf, PREC_BIT);
    mpfr_set_z(prodf, NUM(op1), RND_FORM);

    mpfr_div(prodf, prodf, NUM(op2), RND_FORM);

    mpz_t prod;
    mpz_init(prod);
    mpfr_get_z(prod, prodf, MPFR_RNDD);

    mpfr_clear(prodf);
    return int_set(prod);
}

int_t *int_pow(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_pow_ui(prod, NUM(op1), mpz_get_ui(NUM(op2)));

    return int_set(prod);
}

float_t *int_pow_float(const int_t *op1, const float_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op1), RND_FORM);

    mpfr_pow(prod, prod, NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *int_pow_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_pow(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

float_t *int_pown(const int_t *op1, const int_t *op2)
{
    mpfr_t prod;
    mpfr_init2(prod, PREC_BIT);
    mpfr_set_z(prod, NUM(op1), RND_FORM);

    mpfr_pow_z(prod, prod, NUM(op2), RND_FORM);

    return float_set(prod);
}

cmpx_t *int_npow(const int_t *op1, const float_t *op2)
{
    mpc_t prod;
    mpc_init3(prod, CPREC_BIT, CPREC_BIT);
    mpc_set_z(prod, NUM(op1), CRND_FORM);

    mpc_pow_fr(prod, prod, NUM(op2), CRND_FORM);

    return cmpx_set(prod);
}

int_t *int_lshift(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_mul_2exp(prod, NUM(op1), mpz_get_ui(NUM(op2)));

    return int_set(prod);
}

int_t *int_rshift(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_fdiv_q_2exp(prod, NUM(op1), mpz_get_ui(NUM(op2)));

    return int_set(prod);
}

int_t *int_band(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_and(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

int_t *int_bior(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_ior(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

int_t *int_bxor(const int_t *op1, const int_t *op2)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_xor(prod, NUM(op1), NUM(op2));

    return int_set(prod);
}

bool_t *int_equal(const int_t *op1, const int_t *op2)
{
    int prod = !int_cmp(op1, op2);

    return bool_set(prod);
}

bool_t *int_equal_float(const int_t *op1, const float_t *op2)
{
    int prod = !int_cmp_float(op1, op2);

    return bool_set(prod);
}

bool_t *int_equal_cmpx(const int_t *op1, const cmpx_t *op2)
{
    int prod = !int_cmp_cmpx(op1, op2);

    return bool_set(prod);
}

bool_t *int_nequal(const int_t *op1, const int_t *op2)
{
    int prod = int_cmp(op1, op2);

    return bool_set(prod);
}

bool_t *int_nequal_float(const int_t *op1, const float_t *op2)
{
    int prod = int_cmp_float(op1, op2);

    return bool_set(prod);
}

bool_t *int_nequal_cmpx(const int_t *op1, const cmpx_t *op2)
{
    int prod = int_cmp_cmpx(op1, op2);

    return bool_set(prod);
}

bool_t *int_less(const int_t *op1, const int_t *op2)
{
    int prod = int_cmp(op1, op2) < 0;

    return bool_set(prod);
}

bool_t *int_less_float(const int_t *op1, const float_t *op2)
{
    int prod = int_cmp_float(op1, op2) < 0;

    return bool_set(prod);
}

bool_t *int_great(const int_t *op1, const int_t *op2)
{
    int prod = int_cmp(op1, op2) > 0;

    return bool_set(prod);
}

bool_t *int_great_float(const int_t *op1, const float_t *op2)
{
    int prod = int_cmp_float(op1, op2) > 0;

    return bool_set(prod);
}

bool_t *int_lesseq(const int_t *op1, const int_t *op2)
{
    int prod = int_cmp(op1, op2) <= 0;

    return bool_set(prod);
}

bool_t *int_lesseq_float(const int_t *op1, const float_t *op2)
{
    int prod = int_cmp_float(op1, op2) <= 0;

    return bool_set(prod);
}

bool_t *int_greateq(const int_t *op1, const int_t *op2)
{
    int prod = int_cmp(op1, op2) >= 0;

    return bool_set(prod);
}

bool_t *int_greateq_float(const int_t *op1, const float_t *op2)
{
    int prod = int_cmp_float(op1, op2) >= 0;

    return bool_set(prod);
}

int_t *int_pos(const int_t *op)
{
    mpz_t prod;

    mpz_init_set(prod, NUM(op));

    return int_set(prod);
}

int_t *int_neg(const int_t *op)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_neg(prod, NUM(op));

    return int_set(prod);
}

int_t *int_bnot(const int_t *op)
{
    mpz_t prod;
    mpz_init(prod);

    mpz_com(prod, NUM(op));

    return int_set(prod);
}

int int_cmp(const int_t *op1, const int_t *op2)
{
    return mpz_cmp(NUM(op1), NUM(op2));
}

int int_cmp_float(const int_t *op1, const float_t *op2)
{
    return -mpfr_cmp_z(NUM(op2), NUM(op1));
}

int int_cmp_cmpx(const int_t *op1, const cmpx_t *op2)
{
    mpc_t op1c;
    mpc_init3(op1c, CPREC_BIT, CPREC_BIT);
    mpc_set_z(op1c, NUM(op1), CRND_FORM);

    int prod = mpc_cmp(op1c, NUM(op2));

    mpc_clear(op1c);
    return prod;
}

int int_cmp_ull(const int_t *op1, unsigned long long op2)
{
    if (!int_fits_ull(op1))
        return 1;

    unsigned long long ull = int_get_ull(op1);
    if (ull < op2)
        return -1;
    return ull > op2;
}

int int_sgn(const int_t *op)
{
    return mpz_sgn(NUM(op));
}

int int_fits_ui(const int_t *op)
{
    return mpz_fits_ulong_p(NUM(op));
}

int int_fits_ull(const int_t *op)
{
    return mpz_size(NUM(op)) <= 1;
}
