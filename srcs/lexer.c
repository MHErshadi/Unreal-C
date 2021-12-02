#include <string.h>
#include <ctype.h>

#include "lexer.h"
#include "macro.h"

#define TOKS_A 1024
#define IDNT_A 32
#define NUM_A 32
#define STR_A 128

lres_t set_lres(tok_t *toks, int haserr, ill_chr_err_t error);
void free_toks(tok_t *toks, size_t toks_s);

tok_t gen_idnt(pos_t *curr_pos, char term);
tok_t gen_num(pos_t *curr_pos, char term);
tok_t gen_str(pos_t *curr_pos, char quote, char term);
tok_t gen_double(enum tok_t t1, enum tok_t t2, char diff, pos_t *curr_pos);
tok_t gen_triple(enum tok_t t1, enum tok_t t2, enum tok_t t3, char diff1, char diff2, pos_t *curr_pos);
tok_t gen_mul(pos_t *curr_pos);
tok_t gen_div(pos_t *curr_pos);
tok_t gen_lt(pos_t *curr_pos);
tok_t gen_gt(pos_t *curr_pos);

const char *code_ext;

lres_t gen_toks(const char *code, const char *fn, char term)
{
    code_ext = code;

    tok_t *toks;
    alloc(toks, tok_t, TOKS_A);

    size_t m = TOKS_A;
    size_t toks_s = 0;

    pos_t curr_pos = set_pos(0, 0, fn);

    while (*code_ext != term)
    {
        if (toks_s == m)
            ralloc(toks, tok_t, m += TOKS_A);

        if (isspace(*code_ext))
        {
            adv_pos(&curr_pos, *code_ext++);
            continue;
        }
        if (isalpha(*code_ext) || *code_ext == '_')
        {
            toks[toks_s++] = gen_idnt(&curr_pos, term);
            continue;
        }
        if (isdigit(*code_ext) || *code_ext == '.')
        {
            toks[toks_s++] = gen_num(&curr_pos, term);
            continue;
        }
        switch (*code_ext)
        {
        case '"':
        case '\'':
            toks[toks_s++] = gen_str(&curr_pos, *code_ext, term);
            break;
        case '+':
            toks[toks_s++] = gen_triple(PLUS_T, PLUSE_T, INC_T, '=', '+', &curr_pos);
            break;
        case '-':
            toks[toks_s++] = gen_triple(MINUS_T, MINUSE_T, DEC_T, '=', '-', &curr_pos);
            break;
        case '*':
            toks[toks_s++] = gen_mul(&curr_pos);
            break;
        case '/':
            toks[toks_s++] = gen_div(&curr_pos);
            break;
        case '%':
            toks[toks_s++] = gen_double(MOD_T, MODE_T, '=', &curr_pos);
            break;
        case '&':
            toks[toks_s++] = gen_triple(BAND_T, BANDE_T, AND_T, '=', '&', &curr_pos);
            break;
        case '|':
            toks[toks_s++] = gen_triple(BIOR_T, BIORE_T, IOR_T, '=', '|', &curr_pos);
            break;
        case '^':
            toks[toks_s++] = gen_triple(BXOR_T, BXORE_T, XOR_T, '=', '^', &curr_pos);
            break;
        case '~':
            toks[toks_s++] = set_tok(BNOT_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case '=':
            toks[toks_s++] = gen_double(EQ_T, EE_T, '=', &curr_pos);
            break;
        case '!':
            toks[toks_s++] = gen_double(NOT_T, NE_T, '=', &curr_pos);
            break;
        case '<':
            toks[toks_s++] = gen_lt(&curr_pos);
            break;
        case '>':
            toks[toks_s++] = gen_gt(&curr_pos);
            break;
        case '(':
            toks[toks_s++] = set_tok(LPAREN_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case ')':
            toks[toks_s++] = set_tok(RPAREN_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case '{':
            toks[toks_s++] = set_tok(LBRAC_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case '}':
            toks[toks_s++] = set_tok(RBRAC_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case '[':
            toks[toks_s++] = set_tok(LSQBRAC_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case ']':
            toks[toks_s++] = set_tok(RSQBRAC_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case ':':
            toks[toks_s++] = set_tok(COL_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case ',':
            toks[toks_s++] = set_tok(COMMA_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        case '\n':
        case ';':
            toks[toks_s++] = set_tok(NEWLN_T, NULL, curr_pos, NULL_POS);
            adv_pos(&curr_pos, *code_ext++);
            break;
        default:
            free_toks(toks, toks_s);

            ill_chr_err_t error = set_ill_chr_err(*code_ext, curr_pos);
            lres_t res = set_lres(NULL, 1, error);
            return res;
        }
    }
    toks[toks_s++] = set_tok(EOF_T, NULL, curr_pos, NULL_POS);
    ralloc(toks, tok_t, toks_s);

    lres_t res = set_lres(toks, 0, (ill_chr_err_t){'\0', NULL_POS});
    return res;
}

lres_t set_lres(tok_t *toks, int haserr, ill_chr_err_t error)
{
    lres_t res;

    TOKS(res) = toks;
    HERR(res) = haserr;
    ERR(res) = error;

    return res;
}

void free_toks(tok_t *toks, size_t toks_s)
{
    size_t i;
    for (i = 0; i < toks_s; i++)
        free_tok(toks[i]);

    fre(toks);
}

tok_t gen_idnt(pos_t *curr_pos, char term)
{
    char *id_str;
    alloc(id_str, char, IDNT_A + 1);

    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    size_t m = IDNT_A;
    size_t i;
    for (i = 0; (isalnum(*code_ext) || *code_ext == '_') && *code_ext != term; i++)
    {
        if (i == m)
            ralloc(id_str, char, (m += IDNT_A) + 1);

        adv_pos(curr_pos, *code_ext);
        id_str[i] = *code_ext++;
    }
    id_str[i++] = '\0';

    ralloc(id_str, char, i);
    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    if (is_kwd(id_str))
        return set_tok(KWD_T, id_str, poss, pose);
    return set_tok(IDNT_T, id_str, poss, pose);
}

tok_t gen_num(pos_t *curr_pos, char term)
{
    char *num_str;
    alloc(num_str, char, NUM_A + 1);
    int is_f = 0;

    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    size_t m = NUM_A;
    size_t i;
    for (i = 0; (isdigit(*code_ext) || *code_ext == '.') && *code_ext != term; i++)
    {
        if (i == m)
            ralloc(num_str, char, (m += NUM_A) + 1);

        if (*code_ext == '.')
        {
            if (is_f)
                break;
            is_f = 1;
        }

        adv_pos(curr_pos, *code_ext);
        num_str[i] = *code_ext++;
    }
    if (*code_ext == 'e')
    {
        is_f = 1;
        if (i == m)
            ralloc(num_str, char, (m += NUM_A) + 1);

        adv_pos(curr_pos, *code_ext);
        num_str[i++] = *code_ext++;

        if (*code_ext == '+' || *code_ext == '-')
        {
            if (i == m)
                ralloc(num_str, char, (m += NUM_A) + 1);

            adv_pos(curr_pos, *code_ext);
            num_str[i++] = *code_ext++;
        }

        for (; isdigit(*code_ext) && *code_ext != term; i++)
        {
            if (i == m)
                ralloc(num_str, char, (m += NUM_A) + 1);

            adv_pos(curr_pos, *code_ext);
            num_str[i] = *code_ext++;
        }
    }
    else if (*code_ext == 'i')
    {
        if (i == m)
            ralloc(num_str, char, m + 1);

        adv_pos(curr_pos, *code_ext++);
        num_str[i++] = '\0';

        pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

        return set_tok(CMPX_T, num_str, poss, pose);
    }
    num_str[i++] = '\0';

    ralloc(num_str, char, i);
    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    if (!is_f)
        return set_tok(INT_T, num_str, poss, pose);
    return set_tok(FLOAT_T, num_str, poss, pose);
}

tok_t gen_str(pos_t *curr_pos, char quote, char term)
{
    char *str;
    alloc(str, char, STR_A + 1);

    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    *code_ext++;

    size_t m = STR_A;
    size_t i, j = 0;
    int esc_char = 0;
    for (i = 0; (*code_ext != quote || esc_char) && *code_ext != term; i++)
    {
        if (j == m)
            ralloc(str, char, (m += STR_A) + 1);

        if (esc_char)
        {
            esc_char = 0;
            adv_pos(curr_pos, *code_ext);

            switch (*code_ext)
            {
            case 'a':
                str[j++] = '\a';
                break;
            case 'b':
                str[j++] = '\b';
                break;
            case 'e':
                str[j++] = '\e';
                break;
            case 'f':
                str[j++] = '\f';
                break;
            case 'n':
                str[j++] = '\n';
                break;
            case 'r':
                str[j++] = '\r';
                break;
            case 't':
                str[j++] = '\t';
                break;
            case 'v':
                str[j++] = '\v';
                break;
            default:
                str[j++] = *code_ext;
                break;
            }

            code_ext++;
            continue;
        }

        if (*code_ext == '\\')
        {
            esc_char = 1;
            adv_pos(curr_pos, *code_ext++);
            continue;
        }

        adv_pos(curr_pos, *code_ext);
        str[j++] = *code_ext++;
    }
    str[j++] = '\0';
    adv_pos(curr_pos, *code_ext++);

    ralloc(str, char, j);
    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(STR_T, str, poss, pose);
}

tok_t gen_double(enum tok_t t1, enum tok_t t2, char diff, pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = t1;
    if (*code_ext == diff)
    {
        type = t2;
        adv_pos(curr_pos, *code_ext++);
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}

tok_t gen_triple(enum tok_t t1, enum tok_t t2, enum tok_t t3, char diff1, char diff2, pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = t1;
    if (*code_ext == diff1)
    {
        type = t2;
        adv_pos(curr_pos, *code_ext++);
    }
    else if (*code_ext == diff2)
    {
        type = t3;
        adv_pos(curr_pos, *code_ext++);
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}

tok_t gen_mul(pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = MUL_T;
    if (*code_ext == '=')
    {
        type = MULE_T;
        adv_pos(curr_pos, *code_ext++);
    }
    else if (*code_ext == '*')
    {
        type = POW_T;
        adv_pos(curr_pos, *code_ext++);

        if (*code_ext == '=')
        {
            type = POWE_T;
            adv_pos(curr_pos, *code_ext++);
        }
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}

tok_t gen_div(pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = DIV_T;
    if (*code_ext == '=')
    {
        type = DIVE_T;
        adv_pos(curr_pos, *code_ext++);
    }
    else if (*code_ext == '/')
    {
        type = QUOT_T;
        adv_pos(curr_pos, *code_ext++);

        if (*code_ext == '=')
        {
            type = QUOTE_T;
            adv_pos(curr_pos, *code_ext++);
        }
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}

tok_t gen_lt(pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = LT_T;
    if (*code_ext == '=')
    {
        type = LTE_T;
        adv_pos(curr_pos, *code_ext++);
    }
    else if (*code_ext == '<')
    {
        type = LSHIFT_T;
        adv_pos(curr_pos, *code_ext++);

        if (*code_ext == '=')
        {
            type = LSHIFTE_T;
            adv_pos(curr_pos, *code_ext++);
        }
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}

tok_t gen_gt(pos_t *curr_pos)
{
    pos_t poss = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));
    adv_pos(curr_pos, *code_ext++);

    enum tok_t type = GT_T;
    if (*code_ext == '=')
    {
        type = GTE_T;
        adv_pos(curr_pos, *code_ext++);
    }
    else if (*code_ext == '>')
    {
        type = RSHIFT_T;
        adv_pos(curr_pos, *code_ext++);

        if (*code_ext == '=')
        {
            type = RSHIFTE_T;
            adv_pos(curr_pos, *code_ext++);
        }
    }

    pos_t pose = set_pos(IDX(*curr_pos), LN(*curr_pos), FN(*curr_pos));

    return set_tok(type, NULL, poss, pose);
}
