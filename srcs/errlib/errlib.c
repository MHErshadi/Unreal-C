#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "errlib.h"
#include "../macro.h"

ill_chr_err_t set_ill_chr_err(char ill_chr, pos_t pos)
{
    ill_chr_err_t error;

    ILL_CHR(error) = ill_chr;
    POS(error) = pos;

    return error;
}

void disp_ill_chr_err(ill_chr_err_t error)
{
    fprintf(stderr, "\nIllegal Character Error: '%c'\n", ILL_CHR(error));
    fprintf(stderr, "File %s, line %llu\n\n", FN(POS(error)), LN(POS(error)) + 1);
}

inv_syn_err_t set_inv_syn_err(const char *detail, pos_t poss, pos_t pose)
{
    inv_syn_err_t error;

    DET(error) = detail;
    POSS(error) = poss;
    POSE(error) = pose;

    return error;
}

void disp_inv_syn_err(inv_syn_err_t error)
{
    if (DET(error))
        fprintf(stderr, "\nInvalid Syntax Error: %s\n", DET(error));
    else
        fputs("\nInvalid Syntax Error\n", stderr);
    fprintf(stderr, "File %s, line %llu\n\n", FN(POSS(error)), LN(POSS(error)) + 1);
}

runtime_err_t set_runtime_err(const char *format, enum runerr_t type, struct __ctx *ctx, pos_t poss, pos_t pose, ...)
{
    runtime_err_t error;

    char *detail;
    alloc(detail, char, MAX_DET + 1);

    va_list ap;
    va_start(ap, pose);
    vsprintf(detail, format, ap);
    va_end(ap);
    DET(error) = detail;

    TYP(error) = type;
    CTX(error) = ctx;
    POSS(error) = poss;
    POSE(error) = pose;

    return error;
}

const char *runerr_str[10] =
{
    "IllOp",
    "DivByZero",
    "MemOverflow",
    "IdxOutRange",
    "InvType",
    "NotSubscr",
    "NotDefined",
    "ConstType",
    "ConstAcc",
    "NotAssigned"
};

void disp_runtime_err(runtime_err_t error)
{
    struct __ctx *ctx = CTX(error);
    pos_t pos = POSS(error);

    char *trcback;
    alloc(trcback, char, MAX_TRCBACK_TOT + 1);

    sprintf(trcback, "  File %s, line %llu, in %s\n", FN(pos), LN(pos) + 1, NAM(ctx));
    while (PRN(ctx))
    {
        pos = PRNP(ctx);
        ctx = PRN(ctx);

        char *trcn;
        alloc(trcn, char, MAX_TRCBACK + 1);

        sprintf(trcn, "  File %s, line %llu, in %s\n%s", FN(pos), LN(pos) + 1, NAM(ctx), trcback);
        strcpy(trcback, trcn);

        fre(trcn);
    }
    fprintf(stderr, "\nTraceback (most recent call last):\n%s", trcback);
    fre(trcback);

    fprintf(stderr, "Runtime Error: %s\n", DET(error));
    fre(DET(error));

    fprintf(stderr, "Error Type: %s\n\n", runerr_str[TYP(error) - 1]);
}

void un_crash(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    abort();
}
