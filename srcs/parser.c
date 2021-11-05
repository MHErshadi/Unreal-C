#include "parser.h"
#include "macro.h"
#include <stdio.h>

void parse_succ(pres_t *res, node_t node);
void parse_fail(pres_t *res, inv_syn_err_t error);
node_t reg_parse_res(pres_t *res, pres_t other);

pres_t tuple(void);
pres_t assign(void);
pres_t ior(void);
pres_t xor(void);
pres_t and(void);
pres_t bior(void);
pres_t bxor(void);
pres_t band(void);
pres_t type(void);
pres_t cmp1(void);
pres_t cmp2(void);
pres_t shift(void);
pres_t expr(void);
pres_t term(void);
pres_t fact(void);
pres_t expo(void);
pres_t idx(void);
pres_t core(void);

pres_t bin_op(pres_t (*func1)(void), const enum tok_t *ops, size_t ops_s, pres_t (*func2)(void));
pres_t bin_opi(pres_t (*func1)(void), const enum tok_t *ops, size_t ops_s, const char **opsi, size_t opsi_s, pres_t (*func2)(void));
pres_t list_expr(tok_t tok);
pres_t dict_expr(tok_t tok);
pres_t set_expr(tok_t tok, node_t fst_nd);
pres_t if_expr(pos_t poss);
pres_t switch_expr(pos_t poss);
int *var_props(void);

tok_t *toks_ext;

pres_t parse(tok_t *toks)
{
    toks_ext = toks;

    pres_t res = tuple();
    if (!HERR(res) && !tok_type(*toks_ext, EOF_T))
    {
        free_node(NOD(res));

        inv_syn_err_t error = set_inv_syn_err("Expected EOF", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
    }

    if (HERR(res))
        while (!tok_type(*toks_ext, EOF_T))
            free_tok(*toks_ext++);
    fre(toks);
    return res;
}

void parse_succ(pres_t *res, node_t node)
{
    HERR(*res) = 0;
    NOD(*res) = node;
}

void parse_fail(pres_t *res, inv_syn_err_t error)
{
    HERR(*res) = 1;
    ERR(*res) = error;
}

node_t reg_parse_res(pres_t *res, pres_t other)
{
    if (HERR(other))
    {
        HERR(*res) = 1;
        ERR(*res) = ERR(other);
    }

    return NOD(other);
}

pres_t tuple(void)
{
    pres_t res;
    HERR(res) = 0;

    node_t fst = reg_parse_res(&res, assign());
    if (HERR(res))
        return res;

    if (tok_type(*toks_ext, COMMA_T))
    {
        toks_ext++;

        size_t elems_s = TUPLE_S, elems_n = 1;
        node_t *elems;
        alloc(elems, node_t, elems_s);
        *elems = fst;

        node_t elem;
        while (1)
        {
            elem = reg_parse_res(&res, assign());
            if (HERR(res))
            {
                free_nodes(elems, elems_n);
                return res;
            }

            if (elems_n == elems_s)
                ralloc(elems, node_t, elems_s += TUPLE_S);
            elems[elems_n++] = elem;

            if (!tok_type(*toks_ext, COMMA_T))
                break;
            toks_ext++;
        }

        ralloc(elems, node_t, elems_n);
        tuple_nd *nd = set_tuple_nd(elems_n, elems);
        node_t node = set_node(TUPLE_N, nd, POSS(fst), POSE(elem));

        parse_succ(&res, node);
        return res;
    }

    parse_succ(&res, fst);
    return res;
}

pres_t assign(void)
{
    if (tok_val(*toks_ext, "var"))
    {
        pres_t res;
        HERR(res) = 0;

        pos_t poss = POSS(*toks_ext);
        fre(VAL(*toks_ext++));

        int *props = var_props();

        if (!tok_type(*toks_ext, IDNT_T))
        {
            fre(props);

            inv_syn_err_t error = set_inv_syn_err("Expected identifier", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }

        tok_t name = *toks_ext++;

        tok_t asgn;
        node_t val;
        if (tok_type(*toks_ext, EQ_T))
        {
            asgn = *toks_ext++;

            val = reg_parse_res(&res, tuple());
            if (HERR(res))
            {
                fre(props);
                fre(VAL(name));
                return res;
            }
        }
        else
        {
            asgn = set_tok(0, NULL, NULL_POS, NULL_POS);
            val = set_node(0, NULL, NULL_POS, NULL_POS);
        }

        var_assign_nd *nd = set_var_assign_nd(props, name, asgn, val);
        node_t node = set_node(VAR_ASSIGN_N, nd, poss, POSE(val));

        parse_succ(&res, node);
        return res;
    }

    if (tok_type(*toks_ext, IDNT_T))
    {
        tok_t name = *toks_ext++;

        if (tok_type(*toks_ext, EQ_T) ||
            tok_type(*toks_ext, PLUSE_T) || tok_type(*toks_ext, MINUSE_T) ||
            tok_type(*toks_ext, MULE_T) || tok_type(*toks_ext, DIVE_T) ||
            tok_type(*toks_ext, MODE_T) || tok_type(*toks_ext, QUOTE_T) ||
            tok_type(*toks_ext, POWE_T) || tok_type(*toks_ext, LSHIFTE_T) ||
            tok_type(*toks_ext, RSHIFTE_T) || tok_type(*toks_ext, BANDE_T) ||
            tok_type(*toks_ext, BXORE_T) || tok_type(*toks_ext, BIORE_T))
        {
            pres_t res;
            HERR(res) = 0;

            tok_t asgn = *toks_ext++;
            node_t val = reg_parse_res(&res, tuple());
            if (HERR(res))
            {
                fre(VAL(name));
                return res;
            }

            int *props;
            salloc(props, int, 4);

            var_assign_nd *nd = set_var_assign_nd(props, name, asgn, val);
            node_t node = set_node(VAR_ASSIGN_N, nd, POSS(name), POSE(val));

            parse_succ(&res, node);
            return res;
        }
        if (tok_type(*toks_ext, INC_T) || tok_type(*toks_ext, DEC_T))
        {
            pres_t res;
            HERR(res) = 0;

            tok_t asgn = *toks_ext++;
            node_t val = set_node(0, NULL, NULL_POS, NULL_POS);

            int *props;
            salloc(props, int, 4);

            var_assign_nd *nd = set_var_assign_nd(props, name, asgn, val);
            node_t node = set_node(VAR_ASSIGN_N, nd, POSS(name), POSE(asgn));

            parse_succ(&res, node);
            return res;
        }

        toks_ext--;
    }

    return ior();
}

pres_t ior(void)
{
    const enum tok_t ops[1] = {IOR_T};
    const char *ops_id[1] = {"or"};
    return bin_opi(xor, ops, 1, ops_id, 1, xor);
}

pres_t xor(void)
{
    const enum tok_t ops[1] = {XOR_T};
    const char *ops_id[1] = {"xor"};
    return bin_opi(and, ops, 1, ops_id, 1, and);
}

pres_t and(void)
{
    const enum tok_t ops[1] = {AND_T};
    const char *ops_id[1] = {"and"};
    return bin_opi(bior, ops, 1, ops_id, 1, bior);
}

pres_t bior(void)
{
    const enum tok_t ops[1] = {BIOR_T};
    return bin_op(bxor, ops, 1, bxor);
}

pres_t bxor(void)
{
    const enum tok_t ops[1] = {BXOR_T};
    return bin_op(band, ops, 1, band);
}

pres_t band(void)
{
    const enum tok_t ops[1] = {BAND_T};
    return bin_op(type, ops, 1, type);
}

pres_t type(void)
{
    const char *ops_id[2] = {"is", "are"};
    return bin_opi(cmp1, NULL, 0, ops_id, 2, cmp1);
}

pres_t cmp1(void)
{
    const enum tok_t ops[2] = {EE_T, NE_T};
    return bin_op(cmp2, ops, 2, cmp2);
}

pres_t cmp2(void)
{
    const enum tok_t ops[4] = {LT_T, GT_T, LTE_T, GTE_T};
    return bin_op(shift, ops, 4, shift);
}

pres_t shift(void)
{
    const enum tok_t ops[2] = {LSHIFT_T, RSHIFT_T};
    return bin_op(expr, ops, 2, expr);
}

pres_t expr(void)
{
    const enum tok_t ops[2] = {PLUS_T, MINUS_T};
    return bin_op(term, ops, 2, term);
}

pres_t term(void)
{
    const enum tok_t ops[4] = {MUL_T, DIV_T, MOD_T, QUOT_T};
    return bin_op(fact, ops, 4, fact);
}

pres_t fact(void)
{
    if (tok_type(*toks_ext, PLUS_T) || tok_type(*toks_ext, MINUS_T) ||
        tok_type(*toks_ext, BNOT_T) ||
        tok_type(*toks_ext, NOT_T) || tok_val(*toks_ext, "not"))
    {
        pres_t res;
        HERR(res) = 0;

        tok_t op = *toks_ext++;

        node_t op_nd = reg_parse_res(&res, fact());
        if (HERR(res))
        {
            free_tok(op);
            return res;
        }

        unary_op_nd *nd = set_unary_op_nd(op, op_nd);
        node_t node = set_node(UNARY_OP_N, nd, POSS(op), POSE(op_nd));

        parse_succ(&res, node);
        return res;
    }

    return expo();
}

pres_t expo()
{
    const enum tok_t ops[1] = {POW_T};
    return bin_op(idx, ops, 1, fact);
}

pres_t idx()
{
    pres_t res;
    HERR(res) = 0;

    node_t op = reg_parse_res(&res, core());
    if (HERR(res))
        return res;

    if (tok_type(*toks_ext, LSQBRAC_T))
    {
        toks_ext++;

        node_t idxs = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_node(op);
            return res;
        }

        if (!tok_type(*toks_ext, RSQBRAC_T))
        {
            free_node(op);
            free_node(idxs);

            inv_syn_err_t error = set_inv_syn_err("Expected ']'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        pos_t pose = POSE(*toks_ext++);

        idx_nd *nd = set_idx_nd(op, idxs);
        node_t node = set_node(IDX_N, nd, POSS(op), pose);

        parse_succ(&res, node);
        return res;
    }

    parse_succ(&res, op);
    return res;
}

pres_t core(void)
{
    tok_t tok = *toks_ext++;

    if (tok_type(tok, INT_T))
    {
        int_nd *nd = set_int_nd(tok);
        node_t node = set_node(INT_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_type(tok, FLOAT_T))
    {
        float_nd *nd = set_float_nd(tok);
        node_t node = set_node(FLOAT_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_type(tok, CMPX_T))
    {
        cmpx_nd *nd = set_cmpx_nd(tok);
        node_t node = set_node(CMPX_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_val(tok, "true") || tok_val(tok, "false"))
    {
        bool_nd *nd = set_bool_nd(tok);
        node_t node = set_node(BOOL_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_val(tok, "none"))
    {
        node_t node = set_node(NONE_N, NULL, POSS(tok), POSE(tok));
        fre(VAL(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_type(tok, STR_T))
    {
        str_nd *nd = set_str_nd(tok);
        node_t node = set_node(STR_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_type(tok, LSQBRAC_T))
        return list_expr(tok);

    if (tok_type(tok, LBRAC_T))
        return dict_expr(tok);

    if (tok_type(tok, IDNT_T))
    {
        var_access_nd *nd = set_var_access_nd(tok);
        node_t node = set_node(VAR_ACCESS_N, nd, POSS(tok), POSE(tok));

        pres_t res;
        parse_succ(&res, node);
        return res;
    }

    if (tok_val(tok, "if"))
    {
        fre(VAL(tok));
        return if_expr(POSS(tok));
    }

    if (tok_val(tok, "switch"))
    {
        fre(VAL(tok));
        return switch_expr(POSS(tok));
    }

    if (tok_type(tok, LPAREN_T))
    {
        pres_t res;
        HERR(res) = 0;

        node_t node = reg_parse_res(&res, tuple());
        if (HERR(res))
            return res;

        if (!tok_type(*toks_ext, RPAREN_T))
        {
            free_node(node);

            inv_syn_err_t error = set_inv_syn_err("Expected ')'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }

        POSS(node) = POSS(tok);
        POSE(node) = POSE(*toks_ext++);

        parse_succ(&res, node);
        return res;
    }

    toks_ext--;

    pres_t res;
    inv_syn_err_t error = set_inv_syn_err(NULL, POSS(tok), POSE(tok));
    parse_fail(&res, error);
    return res;
}

pres_t bin_op(pres_t (*func1)(void), const enum tok_t *ops, size_t ops_s, pres_t (*func2)(void))
{
    pres_t res;
    HERR(res) = 0;

    node_t op1 = reg_parse_res(&res, func1());
    if (HERR(res))
        return res;

    while (tok_types(*toks_ext, ops, ops_s))
    {
        tok_t op = *toks_ext++;

        node_t op2 = reg_parse_res(&res, func2());
        if (HERR(res))
        {
            free_node(op1);
            return res;
        }

        bin_op_nd *nd = set_bin_op_nd(op1, op, op2);
        op1 = set_node(BIN_OP_N, nd, POSS(op1), POSE(op2));
    }

    parse_succ(&res, op1);
    return res;
}

pres_t bin_opi(pres_t (*func1)(void), const enum tok_t *ops, size_t ops_s, const char **opsi, size_t opsi_s, pres_t (*func2)(void))
{
    pres_t res;
    HERR(res) = 0;

    node_t op1 = reg_parse_res(&res, func1());
    if (HERR(res))
        return res;

    while (tok_types(*toks_ext, ops, ops_s) || tok_vals(*toks_ext, opsi, opsi_s))
    {
        tok_t op = *toks_ext++;

        node_t op2 = reg_parse_res(&res, func2());
        if (HERR(res))
        {
            free_tok(op);
            free_node(op1);
            return res;
        }

        bin_op_nd *nd = set_bin_op_nd(op1, op, op2);
        op1 = set_node(BIN_OP_N, nd, POSS(op1), POSE(op2));
    }

    parse_succ(&res, op1);
    return res;
}

pres_t list_expr(tok_t tok)
{
    pres_t res;
    HERR(res) = 0;

    if (tok_type(*toks_ext, RSQBRAC_T))
    {
        list_nd *nd = set_list_nd(0, NULL);
        node_t node = set_node(LIST_N, nd, POSS(tok), POSE(*toks_ext++));

        parse_succ(&res, node);
        return res;
    }

    size_t elems_s = LIST_S, elems_n = 0;
    node_t *elems;
    alloc(elems, node_t, elems_s);

    while (1)
    {
        node_t elem = reg_parse_res(&res, assign());
        if (HERR(res))
        {
            free_nodes(elems, elems_n);
            return res;
        }

        if (elems_n == elems_s)
            ralloc(elems, node_t, elems_s += LIST_S);
        elems[elems_n++] = elem;

        if (!tok_type(*toks_ext, COMMA_T))
            break;
        toks_ext++;
    }

    if (!tok_type(*toks_ext, RSQBRAC_T))
    {
        free_nodes(elems, elems_n);

        inv_syn_err_t error = set_inv_syn_err("Expected ']'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }

    ralloc(elems, node_t, elems_n);
    list_nd *nd = set_list_nd(elems_n, elems);
    node_t node = set_node(LIST_N, nd, POSS(tok), POSE(*toks_ext++));

    parse_succ(&res, node);
    return res;
}

pres_t dict_expr(tok_t tok)
{
    pres_t res;
    HERR(res) = 0;

    if (tok_type(*toks_ext, RBRAC_T))
    {
        dict_nd *nd = set_dict_nd(0, NULL);
        node_t node = set_node(DICT_N, nd, POSS(tok), POSE(*toks_ext++));

        parse_succ(&res, node);
        return res;
    }

    node_t fst_k = reg_parse_res(&res, assign());
    if (HERR(res))
        return res;

    if (tok_type(*toks_ext, COMMA_T))
    {
        toks_ext++;
        return set_expr(tok, fst_k);
    }

    if (tok_type(*toks_ext, RBRAC_T))
    {
        node_t *elems;
        alloc(elems, node_t, 1);
        *elems = fst_k;

        set_nd *nd = set_set_nd(1, elems);
        node_t node = set_node(SET_N, nd, POSS(tok), POSE(*toks_ext++));

        parse_succ(&res, node);
        return res;
    }

    if (!tok_type(*toks_ext, COL_T))
    {
        free_node(fst_k);

        inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }
    toks_ext++;

    node_t fst_v = reg_parse_res(&res, assign());
    if (HERR(res))
    {
        free_node(fst_k);
        return res;
    }

    size_t kvs_s = DICT_S, kvs_n = 1;
    struct __kv *kvs;
    alloc(kvs, struct __kv, kvs_s);
    *kvs = set_kv(fst_k, fst_v);

    while (tok_type(*toks_ext, COMMA_T))
    {
        toks_ext++;

        node_t key = reg_parse_res(&res, assign());
        if (HERR(res))
        {
            free_kvs(kvs, kvs_n);
            return res;
        }

        if (!tok_type(*toks_ext, COL_T))
        {
            free_node(key);
            free_kvs(kvs, kvs_n);

            inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        toks_ext++;

        node_t val = reg_parse_res(&res, assign());
        if (HERR(res))
        {
            free_node(key);
            free_kvs(kvs, kvs_n);
            return res;
        }

        if (kvs_n == kvs_s)
            ralloc(kvs, struct __kv, kvs_s += DICT_S);
        kvs[kvs_n++] = set_kv(key, val);
    }

    if (!tok_type(*toks_ext, RBRAC_T))
    {
        free_kvs(kvs, kvs_n);

        inv_syn_err_t error = set_inv_syn_err("Expected '}'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }

    ralloc(kvs, struct __kv, kvs_n);
    dict_nd *nd = set_dict_nd(kvs_n, kvs);
    node_t node = set_node(DICT_N, nd, POSS(tok), POSE(*toks_ext++));

    parse_succ(&res, node);
    return res;
}

pres_t set_expr(tok_t tok, node_t fst_nd)
{
    pres_t res;
    HERR(res) = 0;

    size_t elems_s = SET_S, elems_n = 1;
    node_t *elems;
    alloc(elems, node_t, elems_s);
    *elems = fst_nd;

    while (1)
    {
        node_t elem = reg_parse_res(&res, assign());
        if (HERR(res))
        {
            free_nodes(elems, elems_n);
            return res;
        }

        if (elems_n == elems_s)
            ralloc(elems, node_t, elems_s += SET_S);
        elems[elems_n++] = elem;

        if (!tok_type(*toks_ext, COMMA_T))
            break;
        toks_ext++;
    }

    if (!tok_type(*toks_ext, RBRAC_T))
    {
        free_nodes(elems, elems_n);

        inv_syn_err_t error = set_inv_syn_err("Expected '}'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }

    ralloc(elems, node_t, elems_n);
    set_nd *nd = set_set_nd(elems_n, elems);
    node_t node = set_node(SET_N, nd, POSS(tok), POSE(*toks_ext++));

    parse_succ(&res, node);
    return res;
}

pres_t if_expr(pos_t poss)
{
    pres_t res;
    HERR(res) = 0;

    size_t case_s = IF_CASE_S, case_n = 0;
    struct __case *cases;
    alloc(cases, struct __case, case_s);

    node_t cond = reg_parse_res(&res, tuple());
    if (HERR(res))
    {
        fre(cases);
        return res;
    }

    if (!tok_type(*toks_ext, COL_T))
    {
        fre(cases);
        free_node(cond);

        inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }
    toks_ext++;

    node_t body = reg_parse_res(&res, tuple());
    if (HERR(res))
    {
        fre(cases);
        free_node(cond);
        return res;
    }

    cases[case_n++] = set_case(cond, body);

    while (tok_val(*toks_ext, "elif"))
    {
        fre(VAL(*toks_ext++));

        cond = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_cases(cases, case_n);
            return res;
        }

        if (!tok_type(*toks_ext, COL_T))
        {
            free_cases(cases, case_n);
            free_node(cond);

            inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        toks_ext++;

        body = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_cases(cases, case_n);
            free_node(cond);
            return res;
        }

        if (case_n == case_s)
            ralloc(cases, struct __case, case_s += IF_CASE_S);
        cases[case_n++] = set_case(cond, body);
    }

    body = set_node(0, NULL, NULL_POS, NULL_POS);

    if (tok_val(*toks_ext, "else"))
    {
        fre(VAL(*toks_ext++));

        if (!tok_type(*toks_ext, COL_T))
        {
            free_cases(cases, case_n);

            inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        toks_ext++;

        body = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_cases(cases, case_n);
            return res;
        }
    }

    ralloc(cases, struct __case, case_n);
    if_nd *nd = set_if_nd(case_n, cases, body);
    node_t node = set_node(IF_N, nd, poss, POSE(body));

    parse_succ(&res, node);
    return res;
}

pres_t switch_expr(pos_t poss)
{
    pres_t res;
    HERR(res) = 0;

    size_t case_s = SWITCH_CASE_S, case_n = 0;
    struct __case *cases;
    alloc(cases, struct __case, case_s);

    node_t check = reg_parse_res(&res, tuple());
    if (HERR(res))
    {
        fre(cases);
        return res;
    }

    if (!tok_type(*toks_ext, LBRAC_T))
    {
        fre(cases);
        free_node(check);

        inv_syn_err_t error = set_inv_syn_err("Expected '{'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }
    toks_ext++;

    while (tok_val(*toks_ext, "case"))
    {
        fre(VAL(*toks_ext++));

        node_t cond = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_node(check);
            free_cases(cases, case_n);
            return res;
        }

        if (!tok_type(*toks_ext, COL_T))
        {
            free_node(check);
            free_node(cond);
            free_cases(cases, case_n);

            inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        toks_ext++;

        node_t body = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_node(check);
            free_node(cond);
            free_cases(cases, case_n);
            return res;
        }

        if (case_n == case_s)
            ralloc(cases, struct __case, case_s += SWITCH_CASE_S);
        cases[case_n++] = set_case(cond, body);
    }

    node_t dbody = set_node(0, NULL, NULL_POS, NULL_POS);

    if (tok_val(*toks_ext, "default"))
    {
        fre(VAL(*toks_ext++));

        if (!tok_type(*toks_ext, COL_T))
        {
            free_node(check);
            free_cases(cases, case_n);

            inv_syn_err_t error = set_inv_syn_err("Expected ':'", POSS(*toks_ext), POSE(*toks_ext));
            parse_fail(&res, error);
            return res;
        }
        toks_ext++;

        dbody = reg_parse_res(&res, tuple());
        if (HERR(res))
        {
            free_node(check);
            free_cases(cases, case_n);
            return res;
        }
    }

    if (!tok_type(*toks_ext, RBRAC_T))
    {
        free_node(check);
        free_cases(cases, case_n);
        free_node(dbody);

        inv_syn_err_t error = set_inv_syn_err("Expected '}'", POSS(*toks_ext), POSE(*toks_ext));
        parse_fail(&res, error);
        return res;
    }

    ralloc(cases, struct __case, case_n);
    switch_nd *nd = set_switch_nd(check, case_n, cases, dbody);
    node_t node = set_node(SWITCH_N, nd, poss, POSE(*toks_ext++));

    parse_succ(&res, node);
    return res;
}

int *var_props(void)
{
    int *props;
    salloc(props, int, 4);

    int h_acc = 0, h_scp = 0, h_stc = 0, h_cnt = 0;
    while (1)
    {
        if (!h_acc && tok_val(*toks_ext, "public"))
        {
            fre(VAL(*toks_ext++));
            props[0] = 1;
            continue;
        }
        if (!h_acc && tok_val(*toks_ext, "private"))
        {
            fre(VAL(*toks_ext++));
            continue;
        }

        if (!h_scp && tok_val(*toks_ext, "global"))
        {
            fre(VAL(*toks_ext++));
            props[1] = 1;
            continue;
        }
        if (!h_scp && tok_val(*toks_ext, "local"))
        {
            fre(VAL(*toks_ext++));
            continue;
        }

        if (!h_stc && tok_val(*toks_ext, "static"))
        {
            fre(VAL(*toks_ext++));
            props[2] = 1;
            continue;
        }

        if (!h_cnt && tok_val(*toks_ext, "const"))
        {
            fre(VAL(*toks_ext++));
            props[3] = 1;
            continue;
        }

        break;
    }

    return props;
}
