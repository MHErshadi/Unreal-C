#ifndef __ERRLIB__
#define __ERRLIB__

#include "../term/context.h"

typedef struct
{
    char ill_chr;

    pos_t pos;
} ill_chr_err_t;

typedef struct
{
    const char *detail;

    pos_t poss;
    pos_t pose;
} inv_syn_err_t;

enum runerr_t
{
    ILL_OP_E = 1,
    DIV_BY_ZERO_E,
    MEM_OVERFLOW_E,
    IDX_OUT_RNG_E,
    INV_TYPE_E,
    SUB_SCR_E,
    NOT_DEF_E,
    CNT_TYP_E,
    CNT_ACC_E,
    NOT_ASGN_E,
};

typedef struct
{
    char *detail;
    enum runerr_t type;

    struct __ctx *ctx;
    pos_t poss;
    pos_t pose;
} runtime_err_t;

#define MAX_TRCBACK 64
#define MAX_TRCBACK_TOT (MAX_TRCBACK * 64)
#define MAX_DET 128

ill_chr_err_t set_ill_chr_err(char ill_chr, pos_t pos);
void disp_ill_chr_err(ill_chr_err_t error);

inv_syn_err_t set_inv_syn_err(const char *detail, pos_t poss, pos_t pose);
void disp_inv_syn_err(inv_syn_err_t error);

runtime_err_t set_runtime_err(const char *format, enum runerr_t type, struct __ctx *ctx, pos_t poss, pos_t pose, ...);
void disp_runtime_err(runtime_err_t error);

void un_crash(const char *format, ...);

#define ILL_CHR(e) ((e).ill_chr)
#define POS(e) ((e).pos)

#define DET(e) ((e).detail)

#endif /* __ERRLIB__ */
