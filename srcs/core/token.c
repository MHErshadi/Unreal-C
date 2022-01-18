#include <string.h>

#include "token.h"
#include "../macro.h"

tok_t set_tok(enum tok_t type, char *val, pos_t poss, pos_t pose)
{
    tok_t tok;

    TYP(tok) = type;
    VAL(tok) = val;

    POSS(tok) = poss;
    if (pos_cmp(pose, NULL_POS))
    {
        pose = poss;
        adv_pos(&pose, '\0');
    }
    POSE(tok) = pose;

    return tok;
}

void free_tok(tok_t tok)
{
    if (!TYP(tok))
        return;
    if (VAL(tok))
        fre(VAL(tok));
}

int tok_type(tok_t tok, enum tok_t type)
{
    return TYP(tok) == type;
}

int tok_types(tok_t tok, const enum tok_t *types, unsigned long long size)
{
    unsigned long long i;
    for (i = 0; i < size; i++)
        if (tok_type(tok, types[i]))
            return 1;
    return 0;
}

int tok_val(tok_t tok, const char *val)
{
    return VAL(tok) && !strcmp(VAL(tok), val);
}

int tok_vals(tok_t tok, const char **vals, unsigned long long size)
{
    unsigned long long i;
    for (i = 0; i < size; i++)
        if (tok_val(tok, vals[i]))
            return 1;
    return 0;
}

int tok_match(tok_t tok, enum tok_t type, const char *val)
{
    return tok_type(tok, type) && tok_val(tok, val);
}

int tok_matchs(tok_t tok, enum tok_t type, const char **vals, unsigned long long size)
{
    unsigned long long i;
    for (i = 0; i < size; i++)
        if (tok_match(tok, type, vals[i]))
            return 1;
    return 0;
}

int is_kwd(const char *val)
{
    unsigned i;
    for (i = 0; i < KWD_NUM; i++)
        if (!strcmp(val, kwds[i]))
            return 1;
    return 0;
}
