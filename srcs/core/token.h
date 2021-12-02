#ifndef __TOKEN__
#define __TOKEN__

#include "pos.h"

enum tok_t
{
    KWD_T = 1,
    IDNT_T,

    INT_T,
    FLOAT_T,
    CMPX_T,
    STR_T,

    PLUS_T,
    MINUS_T,
    MUL_T,
    DIV_T,
    MOD_T,
    QUOT_T,
    POW_T,

    LSHIFT_T,
    RSHIFT_T,
    BAND_T,
    BXOR_T,
    BIOR_T,

    BNOT_T,

    EE_T,
    NE_T,
    LT_T,
    GT_T,
    LTE_T,
    GTE_T,

    AND_T,
    XOR_T,
    IOR_T,

    NOT_T,

    EQ_T,

    INC_T,
    DEC_T,

    PLUSE_T,
    MINUSE_T,
    MULE_T,
    DIVE_T,
    MODE_T,
    QUOTE_T,
    POWE_T,
    LSHIFTE_T,
    RSHIFTE_T,
    BANDE_T,
    BXORE_T,
    BIORE_T,

    LPAREN_T,
    RPAREN_T,
    LBRAC_T,
    RBRAC_T,
    LSQBRAC_T,
    RSQBRAC_T,

    COL_T,
    COMMA_T,

    NEWLN_T,
    EOF_T
};

#define KWD_NUM 29
static const char *kwds[KWD_NUM] =
{
    "var",
    "private", "public",
    "local", "global",
    "static", "const",
    "true", "false",
    "none",
    "and", "or", "xor",
    "not",
    "is", "are",
    "in",
    "if", "elif", "else",
    "switch", "case", "default",
    "for", "to", "step",
    "while", "do",
    "loop"
};

typedef struct
{
    enum tok_t type;
    char *val;

    pos_t poss;
    pos_t pose;
} tok_t;

tok_t set_tok(enum tok_t type, char *val, pos_t poss, pos_t pose);
void free_tok(tok_t tok);

int tok_type(tok_t tok, enum tok_t type);
int tok_types(tok_t tok, const enum tok_t *types, size_t size);
int tok_val(tok_t tok, const char *val);
int tok_vals(tok_t tok, const char **vals, size_t size);
int tok_match(tok_t tok, enum tok_t type, const char *val);
int tok_matchs(tok_t tok, enum tok_t type, const char **vals, size_t size);

int is_kwd(const char *val);

#endif /* __TOKEN__ */
