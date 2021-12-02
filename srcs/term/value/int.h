#ifndef __INT__
#define __INT__

#include <gmp.h>

typedef struct
{
    mpz_t num;
} int_t;

#include "float.h"
#include "bool.h"

int_t *int_set(mpz_t num);
int_t *int_set_str(const char *str, int base);
int_t *int_set_ui(unsigned long ui);

int_t *int_cpy(const int_t *val);

unsigned long long int_get_ull(const int_t *val);
long long int_get_sll(const int_t *val);

void int_disp(const int_t *val);
void int_free(int_t *val);

int_t *int_add(const int_t *op1, const int_t *op2);
float_t *int_add_float(const int_t *op1, const float_t *op2);
cmpx_t *int_add_cmpx(const int_t *op1, const cmpx_t *op2);
int_t *int_add_ui(const int_t *op1, unsigned long op2);
void int_addself(int_t *op1, const int_t *op2);
void int_addself_ui(int_t *op1, unsigned long op2);

int_t *int_sub(const int_t *op1, const int_t *op2);
float_t *int_sub_float(const int_t *op1, const float_t *op2);
cmpx_t *int_sub_cmpx(const int_t *op1, const cmpx_t *op2);
int_t *int_sub_ui(const int_t *op1, unsigned long op2);
void int_subself(int_t *op1, const int_t *op2);
void int_subself_ui(int_t *op1, unsigned long op2);

int_t *int_mul(const int_t *op1, const int_t *op2);
float_t *int_mul_float(const int_t *op1, const float_t *op2);
cmpx_t *int_mul_cmpx(const int_t *op1, const cmpx_t *op2);

float_t *int_div(const int_t *op1, const int_t *op2);
float_t *int_div_float(const int_t *op1, const float_t *op2);
cmpx_t *int_div_cmpx(const int_t *op1, const cmpx_t *op2);

int_t *int_mod(const int_t *op1, const int_t *op2);
float_t *int_mod_float(const int_t *op1, const float_t *op2);

int_t *int_quot(const int_t *op1, const int_t *op2);
int_t *int_quot_float(const int_t *op1, const float_t *op2);

int_t *int_pow(const int_t *op1, const int_t *op2);
float_t *int_pow_float(const int_t *op1, const float_t *op2);
cmpx_t *int_pow_cmpx(const int_t *op1, const cmpx_t *op2);
float_t *int_pown(const int_t *op1, const int_t *op2);
cmpx_t *int_npow(const int_t *op1, const float_t *op2);

int_t *int_lshift(const int_t *op1, const int_t *op2);
int_t *int_rshift(const int_t *op1, const int_t *op2);
int_t *int_band(const int_t *op1, const int_t *op2);
int_t *int_bior(const int_t *op1, const int_t *op2);
int_t *int_bxor(const int_t *op1, const int_t *op2);

bool_t *int_equal(const int_t *op1, const int_t *op2);
bool_t *int_equal_float(const int_t *op1, const float_t *op2);
bool_t *int_equal_cmpx(const int_t *op1, const cmpx_t *op2);

bool_t *int_nequal(const int_t *op1, const int_t *op2);
bool_t *int_nequal_float(const int_t *op1, const float_t *op2);
bool_t *int_nequal_cmpx(const int_t *op1, const cmpx_t *op2);

bool_t *int_less(const int_t *op1, const int_t *op2);
bool_t *int_less_float(const int_t *op1, const float_t *op2);

bool_t *int_great(const int_t *op1, const int_t *op2);
bool_t *int_great_float(const int_t *op1, const float_t *op2);

bool_t *int_lesseq(const int_t *op1, const int_t *op2);
bool_t *int_lesseq_float(const int_t *op1, const float_t *op2);

bool_t *int_greateq(const int_t *op1, const int_t *op2);
bool_t *int_greateq_float(const int_t *op1, const float_t *op2);

int_t *int_pos(const int_t *op);
int_t *int_neg(const int_t *op);

int_t *int_bnot(const int_t *op);

int int_cmp(const int_t *op1, const int_t *op2);
int int_cmp_float(const int_t *op1, const float_t *op2);
int int_cmp_cmpx(const int_t *op1, const cmpx_t *op2);
int int_cmp_ull(const int_t *op1, unsigned long long op2);
int int_sgn(const int_t *op);

int int_fits_ui(const int_t *op);
int int_fits_ull(const int_t *op);

#endif /* __INT__ */
