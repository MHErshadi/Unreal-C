#ifndef __CMPX__
#define __CMPX__

#include <mpc.h>

typedef struct
{
    mpc_t num;
} cmpx_t;

#include "int.h"
#include "bool.h"

#define CRND_FORM MPC_RNDNN
#define CPREC_BIT 64
#define CPREC_SHW (CPREC_BIT >> 2)

cmpx_t *cmpx_set(mpc_t num);
cmpx_t *cmpx_set_str(const char *str, int base);

cmpx_t *cmpx_cpy(const cmpx_t *val);

void cmpx_disp(const cmpx_t *val);
void cmpx_free(cmpx_t *val);

cmpx_t *cmpx_add(const cmpx_t *op1, const cmpx_t *op2);
cmpx_t *cmpx_add_int(const cmpx_t *op1, const int_t *op2);
cmpx_t *cmpx_add_float(const cmpx_t *op1, const float_t *op2);
cmpx_t *cmpx_add_ui(const cmpx_t *op1, unsigned long op2);

cmpx_t *cmpx_sub(const cmpx_t *op1, const cmpx_t *op2);
cmpx_t *cmpx_sub_int(const cmpx_t *op1, const int_t *op2);
cmpx_t *cmpx_sub_float(const cmpx_t *op1, const float_t *op2);
cmpx_t *cmpx_sub_ui(const cmpx_t *op1, unsigned long op2);

cmpx_t *cmpx_mul(const cmpx_t *op1, const cmpx_t *op2);
cmpx_t *cmpx_mul_int(const cmpx_t *op1, const int_t *op2);
cmpx_t *cmpx_mul_float(const cmpx_t *op1, const float_t *op2);

cmpx_t *cmpx_div(const cmpx_t *op1, const cmpx_t *op2);
cmpx_t *cmpx_div_int(const cmpx_t *op1, const int_t *op2);
cmpx_t *cmpx_div_float(const cmpx_t *op1, const float_t *op2);

cmpx_t *cmpx_pow(const cmpx_t *op1, const cmpx_t *op2);
cmpx_t *cmpx_pow_int(const cmpx_t *op1, const int_t *op2);
cmpx_t *cmpx_pow_float(const cmpx_t *op1, const float_t *op2);

bool_t *cmpx_equal(const cmpx_t *op1, const cmpx_t *op2);
bool_t *cmpx_equal_int(const cmpx_t *op1, const int_t *op2);
bool_t *cmpx_equal_float(const cmpx_t *op1, const float_t *op2);

bool_t *cmpx_nequal(const cmpx_t *op1, const cmpx_t *op2);
bool_t *cmpx_nequal_int(const cmpx_t *op1, const int_t *op2);
bool_t *cmpx_nequal_float(const cmpx_t *op1, const float_t *op2);

cmpx_t *cmpx_pos(const cmpx_t *op);
cmpx_t *cmpx_neg(const cmpx_t *op);

int cmpx_cmp(const cmpx_t *op1, const cmpx_t *op2);
int cmpx_cmp_int(const cmpx_t *op1, const int_t *op2);
int cmpx_cmp_float(const cmpx_t *op1, const float_t *op2);
int cmpx_cmp_si(const cmpx_t *op1, long op2);

#endif /* __CMPX__ */
