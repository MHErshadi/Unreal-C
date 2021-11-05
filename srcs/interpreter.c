#include "interpreter.h"
#include "term/operation.h"
#include "macro.h"

void visit_succ(vres_t *res, val_t value);
void visit_fail(vres_t *res, runtime_err_t error);
val_t reg_visit_res(vres_t *res, vres_t other);

vres_t visit_int(int_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_float(float_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_cmpx(cmpx_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_bool(bool_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_none(pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_str(str_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_list(list_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_tuple(tuple_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_bin_op(bin_op_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_unary_op(unary_op_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_idx(idx_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_var_assign(var_assign_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_var_access(var_access_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_if(if_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);
vres_t visit_switch(switch_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx);

vres_t visit(node_t node, struct __ctx *ctx)
{
    switch (TYP(node))
    {
    case INT_N:
        return visit_int((int_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case FLOAT_N:
        return visit_float((float_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case CMPX_N:
        return visit_cmpx((cmpx_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case BOOL_N:
        return visit_bool((bool_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case NONE_N:
        return visit_none(POSS(node), POSE(node), ctx);
    case STR_N:
        return visit_str((str_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case LIST_N:
        return visit_list((list_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case TUPLE_N:
        return visit_tuple((tuple_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case BIN_OP_N:
        return visit_bin_op((bin_op_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case UNARY_OP_N:
        return visit_unary_op((unary_op_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case IDX_N:
        return visit_idx((idx_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case VAR_ASSIGN_N:
        return visit_var_assign((var_assign_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case VAR_ACCESS_N:
        return visit_var_access((var_access_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case IF_N:
        return visit_if((if_nd*)NOD(node), POSS(node), POSE(node), ctx);
    case SWITCH_N:
        return visit_switch((switch_nd*)NOD(node), POSS(node), POSE(node), ctx);
    default:
        un_crash("visit function: invalid node type `%d`\n", TYP(node));
        break;
    }
}

void visit_succ(vres_t *res, val_t value)
{
    HERR(*res) = 0;
    VAL(*res) = value;
}

void visit_fail(vres_t *res, runtime_err_t error)
{
    HERR(*res) = 1;
    ERR(*res) = error;
}

val_t reg_visit_res(vres_t *res, vres_t other)
{
    if (HERR(other))
    {
        HERR(*res) = 1;
        ERR(*res) = ERR(other);
    }

    return VAL(other);
}

vres_t visit_int(int_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    int_t *val = int_set_str(VAL(node->num_tok), 10);
    val_t value = set_val(INT_V, val, ctx, poss, pose);

    fre(VAL(node->num_tok));
    fre(node);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_float(float_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    float_t *val = float_set_str(VAL(node->num_tok), 10);
    val_t value = set_val(FLOAT_V, val, ctx, poss, pose);

    fre(VAL(node->num_tok));
    fre(node);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_cmpx(cmpx_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    cmpx_t *val = cmpx_set_str(VAL(node->num_tok), 10);
    val_t value = set_val(CMPX_V, val, ctx, poss, pose);

    fre(VAL(node->num_tok));
    fre(node);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_bool(bool_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    bool_t *val = bool_set_str(VAL(node->bool_tok));
    val_t value = set_val(BOOL_V, val, ctx, poss, pose);

    fre(VAL(node->bool_tok));
    fre(node);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_none(pos_t poss, pos_t pose, struct __ctx *ctx)
{
    val_t value = set_val(NONE_V, NULL, ctx, poss, pose);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_str(str_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    str_t *val = str_set_str(VAL(node->str_tok));
    val_t value = set_val(STR_V, val, ctx, poss, pose);

    fre(VAL(node->str_tok));
    fre(node);

    vres_t res;
    visit_succ(&res, value);
    return res;
}

vres_t visit_list(list_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t *elems;
    alloc(elems, val_t, node->elems_n);

    size_t i;
    for (i = 0; i < node->elems_n; i++)
    {
        val_t elem = reg_visit_res(&res, visit(node->elems_nd[i], ctx));
        if (HERR(res))
        {
            free_vals(elems, i);
            goto ret;
        }

        elems[i] = elem;
    }

    list_t *val = list_set(node->elems_n, elems);
    val_t value = set_val(LIST_V, val, ctx, poss, pose);

    visit_succ(&res, value);

    ret:
    free_nodes(node->elems_nd, node->elems_n);
    fre(node);
    return res;
}

vres_t visit_tuple(tuple_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t *elems;
    alloc(elems, val_t, node->elems_n);

    size_t i;
    for (i = 0; i < node->elems_n; i++)
    {
        val_t elem = reg_visit_res(&res, visit(node->elems_nd[i], ctx));
        if (HERR(res))
        {
            free_vals(elems, i);
            goto ret;
        }

        elems[i] = elem;
    }

    list_t *val = list_set(node->elems_n, elems);
    val_t value = set_val(TUPLE_V, val, ctx, poss, pose);

    visit_succ(&res, value);

    ret:
    free_nodes(node->elems_nd, node->elems_n);
    fre(node);
    return res;
}

vres_t visit_bin_op(bin_op_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t op1 = reg_visit_res(&res, visit(node->op1_nd, ctx));
    if (HERR(res))
    {
        free_node(node->op2_nd);
        free_tok(node->op_tok);
        goto ret;
    }

    val_t op2 = reg_visit_res(&res, visit(node->op2_nd, ctx));
    if (HERR(res))
    {
        free_val(op1);
        free_tok(node->op_tok);
        goto ret;
    }

    ores_t prod;
    if (VAL(node->op_tok))
    {
        if (tok_val(node->op_tok, "and"))
            prod = b_and(op1, op2, ctx);
        else if (tok_val(node->op_tok, "or"))
            prod = b_ior(op1, op2, ctx);
        else if (tok_val(node->op_tok, "xor"))
            prod = b_xor(op1, op2, ctx);
        else if (tok_val(node->op_tok, "is"))
            prod = b_is(op1, op2, ctx);
        else if (tok_val(node->op_tok, "are"))
            prod = b_are(op1, op2, ctx);
        else
            un_crash("visit_bin_op function: invalid operator value \"%s\"", VAL(node->op_tok));
        fre(VAL(node->op_tok));
    }
    else
        switch (TYP(node->op_tok))
        {
        case PLUS_T:
            prod = b_add(op1, op2, ctx);
            break;
        case MINUS_T:
            prod = b_sub(op1, op2, ctx);
            break;
        case MUL_T:
            prod = b_mul(op1, op2, ctx);
            break;
        case DIV_T:
            prod = b_div(op1, op2, ctx);
            break;
        case MOD_T:
            prod = b_mod(op1, op2, ctx);
            break;
        case QUOT_T:
            prod = b_quot(op1, op2, ctx);
            break;
        case POW_T:
            prod = b_pow(op1, op2, ctx);
            break;
        case LSHIFT_T:
            prod = b_lshift(op1, op2, ctx);
            break;
        case RSHIFT_T:
            prod = b_rshift(op1, op2, ctx);
            break;
        case BAND_T:
            prod = b_band(op1, op2, ctx);
            break;
        case BIOR_T:
            prod = b_bior(op1, op2, ctx);
            break;
        case BXOR_T:
            prod = b_bxor(op1, op2, ctx);
            break;
        case EE_T:
            prod = b_equal(op1, op2, ctx);
            break;
        case NE_T:
            prod = b_nequal(op1, op2, ctx);
            break;
        case LT_T:
            prod = b_less(op1, op2, ctx);
            break;
        case GT_T:
            prod = b_great(op1, op2, ctx);
            break;
        case LTE_T:
            prod = b_lesseq(op1, op2, ctx);
            break;
        case GTE_T:
            prod = b_greateq(op1, op2, ctx);
            break;
        case AND_T:
            prod = b_and(op1, op2, ctx);
            break;
        case IOR_T:
            prod = b_ior(op1, op2, ctx);
            break;
        case XOR_T:
            prod = b_xor(op1, op2, ctx);
            break;
        default:
            un_crash("visit_bin_op function: invalid operator type `%d`", TYP(node->op_tok));
            break;
        }
    if (HERR(prod))
        visit_fail(&res, ERR(prod));
    else
        visit_succ(&res, VAL(prod));

    ret:
    fre(node);
    return res;
}

vres_t visit_unary_op(unary_op_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t op = reg_visit_res(&res, visit(node->op_nd, ctx));
    if (HERR(res))
    {
        free_tok(node->op_tok);
        goto ret;
    }

    ores_t prod;
    if (VAL(node->op_tok))
    {
        if (tok_val(node->op_tok, "not"))
            prod = u_not(op, ctx);
        else
            un_crash("visit_bin_op function: invalid operator value \"%s\"", VAL(node->op_tok));
        fre(VAL(node->op_tok));
    }
    else
        switch (TYP(node->op_tok))
        {
        case PLUS_T:
            prod = u_pos(op, ctx);
            break;
        case MINUS_T:
            prod = u_neg(op, ctx);
            break;
        case BNOT_T:
            prod = u_bnot(op, ctx);
            break;
        case NOT_T:
            prod = u_not(op, ctx);
            break;
        default:
            un_crash("visit_unary_op function: invalid operator type `%d`", TYP(node->op_tok));
            break;
        }
    if (HERR(prod))
        visit_fail(&res, ERR(prod));
    else
        visit_succ(&res, VAL(prod));

    ret:
    fre(node);
    return res;
}

vres_t visit_idx(idx_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t op = reg_visit_res(&res, visit(node->op_nd, ctx));
    if (HERR(res))
    {
        free_node(node->idxs_nd);
        goto ret;
    }

    val_t idxs = reg_visit_res(&res, visit(node->idxs_nd, ctx));
    if (HERR(res))
    {
        free_val(op);
        goto ret;
    }

    ores_t prod = u_idx(op, idxs, pose, ctx);
    if (HERR(prod))
        visit_fail(&res, ERR(prod));
    else
        visit_succ(&res, VAL(prod));

    ret:
    fre(node);
    return res;
}

vres_t visit_var_assign(var_assign_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    struct __ctx *ctx_c = ctx;
    char *name = VAL(node->name_tok);
    enum tok_t asgn = TYP(node->assign_tok);

    if (node->props[1])
        while (PRN(ctx_c))
            ctx_c = PRN(ctx_c);

    if (!asgn)
    {
        val_t null = set_val(0, NULL, ctx_c, poss, pose);

        int err;
        if (!node->props[0])
            err = svar_sym(&TBL(ctx_c), name, null, node->props[3], node->props[2]);
        else
            err = spar_sym(&TBL(ctx_c), name, null, node->props[3], node->props[2]);

        if (err)
        {
            runtime_err_t error = set_runtime_err("'%s' is not accessable because it is const",
                CNT_ACC_E, ctx, poss, pose, name);
            visit_fail(&res, error);
            goto ret;
        }

        visit_succ(&res, null);
        goto ret;
    }
    if (asgn == EQ_T)
    {
        val_t val = reg_visit_res(&res, visit(node->val_nd, ctx));
        if (HERR(res))
            goto ret;

        int err;
        if (!node->props[0])
            err = svar_sym(&TBL(ctx_c), name, cpy_val(val), node->props[3], node->props[2]);
        else
            err = spar_sym(&TBL(ctx_c), name, cpy_val(val), node->props[3], node->props[2]);

        if (err)
        {
            runtime_err_t error = set_runtime_err("'%s' is not accessable because it is const",
                CNT_ACC_E, ctx, poss, pose, name);
            visit_fail(&res, error);
            goto ret;
        }

        visit_succ(&res, val);
        goto ret;
    }

    int err;
    var_t ovar;
    int isp = 0;

    err = getv_ctx(ctx_c, &ovar, &isp, name);
    if (err)
    {
        runtime_err_t error = set_runtime_err("'%s' is not defined",
            NOT_DEF_E, ctx, poss, pose, name);
        visit_fail(&res, error);
        goto ret;
    }
    if (ICNT(ovar))
    {
        runtime_err_t error = set_runtime_err("'%s' is not accessable because it is const",
            CNT_ACC_E, ctx, poss, pose, name);
        visit_fail(&res, error);
        goto ret;
    }

    val_t oval = cpy_val(VAL(ovar));

    ores_t prod;
    switch (asgn)
    {
    case INC_T:
        prod = u_inc(oval, ctx);
        break;
    case DEC_T:
        prod = u_dec(oval, ctx);
        break;
    default:
        do
        {
            val_t val = reg_visit_res(&res, visit(node->val_nd, ctx));
            if (HERR(res))
            {
                free_val(oval);
                goto ret;
            }

            switch (asgn)
            {
            case PLUSE_T:
                prod = b_add(oval, val, ctx_c);
                break;
            case MINUSE_T:
                prod = b_sub(oval, val, ctx_c);
                break;
            case MULE_T:
                prod = b_mul(oval, val, ctx_c);
                break;
            case DIVE_T:
                prod = b_div(oval, val, ctx_c);
                break;
            case MODE_T:
                prod = b_mod(oval, val, ctx);
                break;
            case QUOTE_T:
                prod = b_quot(oval, val, ctx);
                break;
            case POWE_T:
                prod = b_pow(oval, val, ctx);
                break;
            case LSHIFTE_T:
                prod = b_lshift(oval, val, ctx);
                break;
            case RSHIFTE_T:
                prod = b_rshift(oval, val, ctx);
                break;
            case BANDE_T:
                prod = b_band(oval, val, ctx);
                break;
            case BIORE_T:
                prod = b_bior(oval, val, ctx);
                break;
            case BXORE_T:
                prod = b_bxor(oval, val, ctx);
                break;
            default:   
                un_crash("visit_var_assign function: invalid assign type `%d`", asgn);
                break;
            }
        } while (0);

        break;
    }
    if (HERR(prod))
    {
        visit_fail(&res, ERR(prod));
        goto ret;
    }
    visit_succ(&res, oval = VAL(prod));

    if (!isp)
        svar_sym(&TBL(ctx_c), name, cpy_val(oval), 0, ISTC(ovar));
    else
        spar_sym(&TBL(ctx_c), name, cpy_val(oval), 0, ISTC(ovar));

    ret:
    fre(node->props);
    fre(name);
    fre(node);
    return res;
}

vres_t visit_var_access(var_access_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    char *name = VAL(node->name_tok);
    var_t var;

    int err = getv_ctx(ctx, &var, NULL, name);
    if (err)
    {
        runtime_err_t error = set_runtime_err("'%s' is not defined",
            NOT_DEF_E, ctx, poss, pose, name);
        visit_fail(&res, error);
        goto ret;
    }

    val_t val = cpy_val(VAL(var));
    CTX(val) = ctx;
    POSS(val) = poss;
    POSE(val) = pose;

    visit_succ(&res, val);

    ret:
    fre(name);
    fre(node);
    return res;
}

vres_t visit_if(if_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    size_t i;
    for (i = 0; i < node->case_n; i++)
    {
        struct __case curr_case = node->cases[i];

        val_t cond = reg_visit_res(&res, visit(curr_case.cond_nd, ctx));
        if (HERR(res))
            goto ret;

        if (val_logic(cond))
        {
            val_t body = reg_visit_res(&res, visit(curr_case.body_nd, ctx));
            if (HERR(res))
            {
                free_val(cond);
                goto ret;
            }

            visit_succ(&res, body);
            free_val(cond);
            goto ret;
        }

        free_val(cond);
    }

    if (TYP(node->ebody_nd))
    {
        val_t ebody = reg_visit_res(&res, visit(node->ebody_nd, ctx));
        if (HERR(res))
            goto ret;

        visit_succ(&res, ebody);
        goto ret;
    }

    visit_succ(&res, set_val(0, NULL, ctx, NULL_POS, NULL_POS));

    ret:
    free_cases(node->cases, node->case_n);
    free_node(node->ebody_nd);
    fre(node);
    return res;
}

vres_t visit_switch(switch_nd *node, pos_t poss, pos_t pose, struct __ctx *ctx)
{
    vres_t res;
    HERR(res) = 0;

    val_t check = reg_visit_res(&res, visit(node->check_nd, ctx));
    if (HERR(res))
        goto ret;

    size_t i;
    for (i = 0; i < node->case_n; i++)
    {
        struct __case curr_case = node->cases[i];

        val_t cond = reg_visit_res(&res, visit(curr_case.cond_nd, ctx));
        if (HERR(res))
            goto ret;

        if (cmp_op(check, cond))
        {
            val_t body = reg_visit_res(&res, visit(curr_case.body_nd, ctx));
            if (HERR(res))
            {
                free_val(cond);
                goto ret;
            }

            visit_succ(&res, body);
            free_val(cond);
            goto ret;
        }

        free_val(cond);
    }

    if (TYP(node->dbody_nd))
    {
        val_t dbody = reg_visit_res(&res, visit(node->dbody_nd, ctx));
        if (HERR(res))
            goto ret;

        visit_succ(&res, dbody);
        goto ret;
    }

    visit_succ(&res, set_val(0, NULL, ctx, NULL_POS, NULL_POS));

    ret:
    free_val(check);
    free_node(node->check_nd);
    free_cases(node->cases, node->case_n);
    free_node(node->dbody_nd);
    fre(node);
    return res;
}
