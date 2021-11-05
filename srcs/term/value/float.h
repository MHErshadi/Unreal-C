#ifndef __FLOAT__
#define __FLOAT__

#include <mpfr.h>

typedef struct
{
    mpfr_t num;
} float_t;

#include "cmpx.h"
#include "bool.h"

#define RND_FORM MPFR_RNDN
#define PREC_BIT 64
#define PREC_SHW (PREC_BIT >> 2)

float_t *float_set(mpfr_t num);
float_t *float_set_str(const char *str, int base);

float_t *float_cpy(const float_t *val);

void float_disp(const float_t *val);
void float_free(float_t *val);

float_t *float_add(const float_t *op1, const float_t *op2);
float_t *float_add_int(const float_t *op1, const int_t *op2);
cmpx_t *float_add_cmpx(const float_t *op1, const cmpx_t *op2);
float_t *float_add_ui(const float_t *op1, unsigned long op2);

float_t *float_sub(const float_t *op1, const float_t *op2);
float_t *float_sub_int(const float_t *op1, const int_t *op2);
cmpx_t *float_sub_cmpx(const float_t *op1, const cmpx_t *op2);
float_t *float_sub_ui(const float_t *op1, unsigned long op2);

float_t *float_mul(const float_t *op1, const float_t *op2);
float_t *float_mul_int(const float_t *op1, const int_t *op2);
cmpx_t *float_mul_cmpx(const float_t *op1, const cmpx_t *op2);

float_t *float_div(const float_t *op1, const float_t *op2);
float_t *float_div_int(const float_t *op1, const int_t *op2);
cmpx_t *float_div_cmpx(const float_t *op1, const cmpx_t *op2);

float_t *float_mod(const float_t *op1, const float_t *op2);
float_t *float_mod_int(const float_t *op1, const int_t *op2);

int_t *float_quot(const float_t *op1, const float_t *op2);
int_t *float_quot_int(const float_t *op1, const int_t *op2);

float_t *float_pow(const float_t *op1, const float_t *op2);
float_t *float_pow_int(const float_t *op1, const int_t *op2);
cmpx_t *float_pow_cmpx(const float_t *op1, const cmpx_t *op2);
cmpx_t *float_npow(const float_t *op1, const float_t *op2);

bool_t *float_equal(const float_t *op1, const float_t *op2);
bool_t *float_equal_int(const float_t *op1, const int_t *op2);
bool_t *float_equal_cmpx(const float_t *op1, const cmpx_t *op2);

bool_t *float_nequal(const float_t *op1, const float_t *op2);
bool_t *float_nequal_int(const float_t *op1, const int_t *op2);
bool_t *float_nequal_cmpx(const float_t *op1, const cmpx_t *op2);

bool_t *float_less(const float_t *op1, const float_t *op2);
bool_t *float_less_int(const float_t *op1, const int_t *op2);

bool_t *float_great(const float_t *op1, const float_t *op2);
bool_t *float_great_int(const float_t *op1, const int_t *op2);

bool_t *float_lesseq(const float_t *op1, const float_t *op2);
bool_t *float_lesseq_int(const float_t *op1, const int_t *op2);

bool_t *float_greateq(const float_t *op1, const float_t *op2);
bool_t *float_greateq_int(const float_t *op1, const int_t *op2);

float_t *float_pos(const float_t *op);
float_t *float_neg(const float_t *op);

int float_cmp(const float_t *op1, const float_t *op2);
int float_cmp_int(const float_t *op1, const int_t *op2);
int float_cmp_cmpx(const float_t *op1, const cmpx_t *op2);
int float_sgn(const float_t *val);

#endif /* __FLOAT__ */
