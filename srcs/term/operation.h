#ifndef __OPERATION__
#define __OPERATION__

#include "value.h"
#include "../errlib/errlib.h"

typedef struct
{
    val_t val;

    int haserr : 1;
    runtime_err_t error;
} ores_t;

ores_t b_add(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_sub(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_mul(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_div(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_mod(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_quot(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_pow(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_lshift(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_rshift(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_band(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_bior(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_bxor(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_equal(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_nequal(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_less(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_great(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_lesseq(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_greateq(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_and(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_ior(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_xor(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_is(val_t op1, val_t op2, struct __ctx *ctx);
ores_t b_are(val_t op1, val_t op2, struct __ctx *ctx);

ores_t b_in(val_t op1, val_t op2, struct __ctx *ctx);

ores_t u_pos(val_t op, struct __ctx *ctx);
ores_t u_neg(val_t op, struct __ctx *ctx);

ores_t u_bnot(val_t op, struct __ctx *ctx);

ores_t u_not(val_t op, struct __ctx *ctx);

ores_t u_idx(val_t op, val_t idxs, pos_t pose, struct __ctx *ctx);

ores_t u_inc(val_t op, struct __ctx *ctx);
ores_t u_dec(val_t op, struct __ctx *ctx);

int cmp_op(val_t op1, val_t op2);

#endif /* __OPERATION__ */