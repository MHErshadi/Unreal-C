#ifndef __LEXER__
#define __LEXER__

#include "core/token.h"
#include "errlib/errlib.h"

typedef struct
{
    tok_t *toks;

    int haserr : 1;
    ill_chr_err_t error;
} lres_t;

lres_t gen_toks(const char *code, const char *fn, char term);

#define TOKS(lres) ((lres).toks)

#endif /* __LEXER__ */
