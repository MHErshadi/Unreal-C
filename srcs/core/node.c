#include <stdio.h>

#include "node.h"
#include "../errlib/errlib.h"
#include "../macro.h"

node_t set_node(enum node_t type, void *nd, pos_t poss, pos_t pose)
{
    node_t node;

    TYP(node) = type;
    NOD(node) = nd;
    POSS(node) = poss;
    POSE(node) = pose;

    return node;
}

void disp_node(node_t node)
{
    if (!TYP(node))
        return;
    if (node_type(node, INT_N))
    {
        int_nd *nd = (int_nd*)NOD(node);

        printf("(%s)", VAL(nd->num_tok));
        return;
    }
    if (node_type(node, FLOAT_N))
    {
        float_nd *nd = (float_nd*)NOD(node);

        printf("(%s)", VAL(nd->num_tok));
        return;
    }
    if (node_type(node, CMPX_N))
    {
        cmpx_nd *nd = (cmpx_nd*)NOD(node);

        printf("(%s)", VAL(nd->num_tok));
        return;
    }
    if (node_type(node, BOOL_N))
    {
        bool_nd *nd = (bool_nd*)NOD(node);

        printf("(%s)", VAL(nd->bool_tok));
        return;
    }
    if (node_type(node, NONE_N))
    {
        printf("(none)");
        return;
    }
    if (node_type(node, STR_N))
    {
        str_nd *nd = (str_nd*)NOD(node);

        printf("(\"%s\")", VAL(nd->str_tok));
        return;
    }
    if (node_type(node, LIST_N))
    {
        list_nd *nd = (list_nd*)NOD(node);

        printf("([");
        size_t i;
        for (i = 0; i < nd->elems_n; i++)
        {
            disp_node(nd->elems_nd[i]);
            printf(", ");
        }
        printf("])");
        return;
    }
    if (node_type(node, TUPLE_N))
    {
        tuple_nd *nd = (tuple_nd*)NOD(node);

        printf("((");
        size_t i;
        for (i = 0; i < nd->elems_n; i++)
        {
            disp_node(nd->elems_nd[i]);
            printf(", ");
        }
        printf("))");
        return;
    }
    if (node_type(node, DICT_N))
    {
        dict_nd *nd = (dict_nd*)NOD(node);

        printf("({");
        size_t i;
        for (i = 0; i < nd->kvs_n; i++)
        {
            disp_node(nd->kvs_nd[i].key_nd);
            printf(": ");
            disp_node(nd->kvs_nd[i].val_nd);
            printf(", ");
        }
        printf("})");
        return;
    }
    if (node_type(node, SET_N))
    {
        set_nd *nd = (set_nd*)NOD(node);

        printf("({");
        size_t i;
        for (i = 0; i < nd->elems_n; i++)
        {
            disp_node(nd->elems_nd[i]);
            printf(", ");
        }
        printf("})");
        return;
    }
    if (node_type(node, BIN_OP_N))
    {
        bin_op_nd *nd = (bin_op_nd*)NOD(node);

        printf("(");
        disp_node(nd->op1_nd);
        printf(", <%d : %s>, ", TYP(nd->op_tok), VAL(nd->op_tok));
        disp_node(nd->op2_nd);
        printf(")");
        return;
    }
    if (node_type(node, UNARY_OP_N))
    {
        unary_op_nd *nd = (unary_op_nd*)NOD(node);

        printf("(");
        printf("<%d : %s>, ", TYP(nd->op_tok), VAL(nd->op_tok));
        disp_node(nd->op_nd);
        printf(")");
        return;
    }
    if (node_type(node, IDX_N))
    {
        idx_nd *nd = (idx_nd*)NOD(node);

        printf("(");
        disp_node(nd->op_nd);
        printf(", ");
        disp_node(nd->idxs_nd);
        printf(")");
    }
    if (node_type(node, VAR_ASSIGN_N))
    {
        var_assign_nd *nd = (var_assign_nd*)NOD(node);

        printf("([%d, %d, %d, %d], %s, %d, ",
            nd->props[0], nd->props[1], nd->props[2], nd->props[3], VAL(nd->name_tok), TYP(nd->assign_tok));
        disp_node(nd->val_nd);
        printf(")");
        return;
    }
    if (node_type(node, VAR_ACCESS_N))
    {
        var_access_nd *nd = (var_access_nd*)NOD(node);

        printf("(%s)", VAL(nd->name_tok));
        return;
    }
    if (node_type(node, IF_N))
    {
        if_nd *nd = (if_nd*)NOD(node);

        printf("(");
        size_t i;
        for (i = 0; i < nd->case_n; i++)
        {
            printf("[");
            disp_node(nd->cases[i].cond_nd);
            disp_node(nd->cases[i].body_nd);
            printf("], ");
        }
        disp_node(nd->ebody_nd);
        printf(")");
        return;
    }
    if (node_type(node, SWITCH_N))
    {
        switch_nd *nd = (switch_nd*)NOD(node);

        printf("(");
        disp_node(nd->check_nd);
        size_t i;
        for (i = 0; i < nd->case_n; i++)
        {
            printf(", [");
            disp_node(nd->cases[i].cond_nd);
            disp_node(nd->cases[i].body_nd);
            printf("]");
        }
        printf(", ");
        disp_node(nd->dbody_nd);
        printf(")");
        return;
    }
    if (node_type(node, FOR_N))
    {
        for_nd *nd = (for_nd*)NOD(node);

        printf("(%s, ", VAL(nd->name_tok));
        disp_node(nd->start_nd);
        printf(", ");
        disp_node(nd->end_nd);
        printf(", ");
        disp_node(nd->step_nd);
        printf(", ");
        disp_node(nd->body_nd);
        printf(")");
        return;
    }
    if (node_type(node, FOREACH_N))
    {
        foreach_nd *nd = (foreach_nd*)NOD(node);

        printf("(%s, ", VAL(nd->name_tok));
        disp_node(nd->iter_nd);
        printf(", ");
        disp_node(nd->body_nd);
        printf(")");
        return;
    }
    if (node_type(node, WHILE_N))
    {
        while_nd *nd = (while_nd*)NOD(node);

        printf("(");
        disp_node(nd->cond_nd);
        printf(", ");
        disp_node(nd->body_nd);
        printf(")");
        return;
    }
    if (node_type(node, DOWHILE_N))
    {
        dowhile_nd *nd = (dowhile_nd*)NOD(node);

        printf("(");
        disp_node(nd->body_nd);
        printf(", ");
        disp_node(nd->cond_nd);
        printf(")");
        return;
    }
    if (node_type(node, LOOP_N))
    {
        loop_nd *nd = (loop_nd*)NOD(node);

        printf("(");
        disp_node(nd->init_nd);
        printf(", ");
        disp_node(nd->cond_nd);
        printf(", ");
        disp_node(nd->step_nd);
        printf(", ");
        disp_node(nd->body_nd);
        printf(")");
        return;
    }

    un_crash("disp_node function: invalid node type `%d`", TYP(node));
}

void free_node(node_t node)
{
    TYP(node) = 0;
    if (!TYP(node))
        return;
    if (node_type(node, INT_N))
    {
        int_nd *nd = (int_nd*)NOD(node);

        fre(VAL(nd->num_tok));
        fre(nd);
        return;
    }
    if (node_type(node, FLOAT_N))
    {
        float_nd *nd = (float_nd*)NOD(node);

        fre(VAL(nd->num_tok));
        fre(nd);
        return;
    }
    if (node_type(node, CMPX_N))
    {
        cmpx_nd *nd = (cmpx_nd*)NOD(node);

        fre(VAL(nd->num_tok));
        fre(nd);
        return;
    }
    if (node_type(node, BOOL_N))
    {
        bool_nd *nd = (bool_nd*)NOD(node);

        fre(VAL(nd->bool_tok));
        fre(nd);
        return;
    }
    if (node_type(node, NONE_N))
        return;
    if (node_type(node, STR_N))
    {
        str_nd *nd = (str_nd*)NOD(node);

        fre(VAL(nd->str_tok));
        fre(nd);
        return;
    }
    if (node_type(node, LIST_N))
    {
        list_nd *nd = (list_nd*)NOD(node);

        free_nodes(nd->elems_nd, nd->elems_n);
        fre(nd);
        return;
    }
    if (node_type(node, TUPLE_N))
    {
        tuple_nd *nd = (tuple_nd*)NOD(node);

        free_nodes(nd->elems_nd, nd->elems_n);
        fre(nd);
        return;
    }
    if (node_type(node, DICT_N))
    {
        dict_nd *nd = (dict_nd*)NOD(node);

        free_kvs(nd->kvs_nd, nd->kvs_n);
        fre(nd);
        return;
    }
    if (node_type(node, SET_N))
    {
        set_nd *nd = (set_nd*)NOD(node);

        free_nodes(nd->elems_nd, nd->elems_n);
        fre(nd);
        return;
    }
    if (node_type(node, BIN_OP_N))
    {
        bin_op_nd *nd = (bin_op_nd*)NOD(node);

        free_node(nd->op1_nd);
        free_tok(nd->op_tok);
        free_node(nd->op2_nd);
        fre(nd);
        return;
    }
    if (node_type(node, UNARY_OP_N))
    {
        unary_op_nd *nd = (unary_op_nd*)NOD(node);

        free_tok(nd->op_tok);
        free_node(nd->op_nd);
        fre(nd);
        return;
    }
    if (node_type(node, IDX_N))
    {
        idx_nd *nd = (idx_nd*)NOD(node);

        free_node(nd->op_nd);
        free_node(nd->idxs_nd);
        fre(nd);
        return;
    }
    if (node_type(node, VAR_ASSIGN_N))
    {
        var_assign_nd *nd = (var_assign_nd*)NOD(node);

        fre(nd->props);
        fre(VAL(nd->name_tok));
        free_node(nd->val_nd);
        fre(nd);
        return;
    }
    if (node_type(node, VAR_ACCESS_N))
    {
        var_access_nd *nd = (var_access_nd*)NOD(node);

        fre(VAL(nd->name_tok));
        fre(nd);
        return;
    }
    if (node_type(node, IF_N))
    {
        if_nd *nd = (if_nd*)NOD(node);

        free_cases(nd->cases, nd->case_n);
        free_node(nd->ebody_nd);
        fre(nd);
        return;
    }
    if (node_type(node, SWITCH_N))
    {
        switch_nd *nd = (switch_nd*)NOD(node);

        free_node(nd->check_nd);
        free_cases(nd->cases, nd->case_n);
        free_node(nd->dbody_nd);
        fre(nd);
        return;
    }
    if (node_type(node, FOR_N))
    {
        for_nd *nd = (for_nd*)NOD(node);

        fre(VAL(nd->name_tok));
        free_node(nd->start_nd);
        free_node(nd->end_nd);
        free_node(nd->step_nd);
        free_node(nd->body_nd);
        fre(nd);
        return;
    }
    if (node_type(node, FOREACH_N))
    {
        foreach_nd *nd = (foreach_nd*)NOD(node);

        fre(VAL(nd->name_tok));
        free_node(nd->iter_nd);
        free_node(nd->body_nd);
        fre(nd);
        return;
    }
    if (node_type(node, WHILE_N))
    {
        while_nd *nd = (while_nd*)NOD(node);

        free_node(nd->cond_nd);
        free_node(nd->body_nd);
        fre(nd);
        return;
    }
    if (node_type(node, DOWHILE_N))
    {
        dowhile_nd *nd = (dowhile_nd*)NOD(node);

        free_node(nd->body_nd);
        free_node(nd->cond_nd);
        fre(nd);
        return;
    }
    if (node_type(node, LOOP_N))
    {
        loop_nd *nd = (loop_nd*)NOD(node);

        free_node(nd->init_nd);
        free_node(nd->cond_nd);
        free_node(nd->step_nd);
        free_node(nd->body_nd);
        fre(nd);
        return;
    }

    un_crash("free_node function: invalid node type `%d`", TYP(node));
}

void free_nodes(node_t *nodes, size_t nodes_s)
{
    size_t i;
    for (i = 0; i < nodes_s; i++)
        free_node(nodes[i]);
    fre(nodes);
}

int_nd *set_int_nd(tok_t num_tok)
{
    int_nd *num;
    alloc(num, int_nd, 1);

    num->num_tok = num_tok;

    return num;
}

float_nd *set_float_nd(tok_t num_tok)
{
    float_nd *num;
    alloc(num, float_nd, 1);

    num->num_tok = num_tok;

    return num;
}

cmpx_nd *set_cmpx_nd(tok_t num_tok)
{
    cmpx_nd *num;
    alloc(num, cmpx_nd, 1);

    num->num_tok = num_tok;

    return num;
}

bool_nd *set_bool_nd(tok_t bool_tok)
{
    bool_nd *bool;
    alloc(bool, bool_nd, 1);

    bool->bool_tok = bool_tok;

    return bool;
}

str_nd *set_str_nd(tok_t str_tok)
{
    str_nd *str;
    alloc(str, str_nd, 1);

    str->str_tok = str_tok;

    return str;
}

list_nd *set_list_nd(size_t elems_n, node_t *elems_nd)
{
    list_nd *list;
    alloc(list, list_nd, 1);

    list->elems_n = elems_n;
    list->elems_nd = elems_nd;

    return list;
}

tuple_nd *set_tuple_nd(size_t elems_n, node_t *elems_nd)
{
    tuple_nd *tuple;
    alloc(tuple, tuple_nd, 1);

    tuple->elems_n = elems_n;
    tuple->elems_nd = elems_nd;

    return tuple;
}

dict_nd *set_dict_nd(size_t kvs_n, struct __kv *kvs_nd)
{
    dict_nd *dict;
    alloc(dict, dict_nd, 1);

    dict->kvs_n = kvs_n;
    dict->kvs_nd = kvs_nd;

    return dict;
}

set_nd *set_set_nd(size_t elems_n, node_t *elems_nd)
{
    set_nd *set;
    alloc(set, set_nd, 1);

    set->elems_n = elems_n;
    set->elems_nd = elems_nd;

    return set;
}

bin_op_nd *set_bin_op_nd(node_t op1_nd, tok_t op_tok, node_t op2_nd)
{
    bin_op_nd *bin_op;
    alloc(bin_op, bin_op_nd, 1);

    bin_op->op1_nd = op1_nd;
    bin_op->op_tok = op_tok;
    bin_op->op2_nd = op2_nd;

    return bin_op;
}

unary_op_nd *set_unary_op_nd(tok_t op_tok, node_t op_nd)
{
    unary_op_nd *unary_op;
    alloc(unary_op, unary_op_nd, 1);

    unary_op->op_tok = op_tok;
    unary_op->op_nd = op_nd;

    return unary_op;
}

idx_nd *set_idx_nd(node_t op_nd, node_t idxs_nd)
{
    idx_nd *idx;
    alloc(idx, idx_nd, 1);

    idx->op_nd = op_nd;
    idx->idxs_nd = idxs_nd;

    return idx;
}

var_assign_nd *set_var_assign_nd(int *props, tok_t name_tok, tok_t assign_tok, node_t val_nd)
{
    var_assign_nd *var_assign;
    alloc(var_assign, var_assign_nd, 1);

    var_assign->props = props;
    var_assign->name_tok = name_tok;
    var_assign->assign_tok = assign_tok;
    var_assign->val_nd = val_nd;

    return var_assign;
}

var_access_nd *set_var_access_nd(tok_t name_tok)
{
    var_access_nd *var_access;
    alloc(var_access, var_access_nd, 1);

    var_access->name_tok = name_tok;

    return var_access;
}

if_nd *set_if_nd(size_t case_n, struct __case *cases, node_t ebody_nd)
{
    if_nd *if_;
    alloc(if_, if_nd, 1);

    if_->case_n = case_n;
    if_->cases = cases;
    if_->ebody_nd = ebody_nd;

    return if_;
}

switch_nd *set_switch_nd(node_t check_nd, size_t case_n, struct __case *cases, node_t dbody_nd)
{
    switch_nd *switch_;
    alloc(switch_, switch_nd, 1);

    switch_->check_nd = check_nd;
    switch_->case_n = case_n;
    switch_->cases = cases;
    switch_->dbody_nd = dbody_nd;

    return switch_;
}

for_nd *set_for_nd(tok_t name_tok, node_t start_nd, node_t end_nd, node_t step_nd, node_t body_nd)
{
    for_nd *for_;
    alloc(for_, for_nd, 1);

    for_->name_tok = name_tok;
    for_->start_nd = start_nd;
    for_->end_nd = end_nd;
    for_->step_nd = step_nd;
    for_->body_nd = body_nd;

    return for_;
}

foreach_nd *set_foreach_nd(tok_t name_tok, node_t iter_nd, node_t body_nd)
{
    foreach_nd *foreach;
    alloc(foreach, foreach_nd, 1);

    foreach->name_tok = name_tok;
    foreach->iter_nd = iter_nd;
    foreach->body_nd = body_nd;

    return foreach;
}

while_nd *set_while_nd(node_t cond_nd, node_t body_nd)
{
    while_nd *while_;
    alloc(while_, while_nd, 1);

    while_->cond_nd = cond_nd;
    while_->body_nd = body_nd;

    return while_;
}

dowhile_nd *set_dowhile_nd(node_t body_nd, node_t cond_nd)
{
    dowhile_nd *dowhile;
    alloc(dowhile, dowhile_nd, 1);

    dowhile->body_nd = body_nd;
    dowhile->cond_nd = cond_nd;

    return dowhile;
}

loop_nd *set_loop_nd(node_t init_nd, node_t cond_nd, node_t step_nd, node_t body_nd)
{
    loop_nd *loop;
    alloc(loop, loop_nd, 1);

    loop->init_nd = init_nd;
    loop->cond_nd = cond_nd;
    loop->step_nd = step_nd;
    loop->body_nd = body_nd;

    return loop;
}

struct __kv set_kv(node_t key, node_t val)
{
    struct __kv kv;

    kv.key_nd = key;
    kv.val_nd = val;

    return kv;
}

void free_kvs(struct __kv *kvs, size_t kv_n)
{
    size_t i;
    for (i = 0; i < kv_n; i++)
        free_kv(kvs[i]);
    fre(kvs);
}

void free_kv(struct __kv kv)
{
    free_node(kv.key_nd);
    free_node(kv.val_nd);
}

struct __case set_case(node_t cond, node_t body)
{
    struct __case case_;

    case_.cond_nd = cond;
    case_.body_nd = body;

    return case_;
}

void free_cases(struct __case *cases, size_t case_n)
{
    size_t i;
    for (i = 0; i < case_n; i++)
        free_case(cases[i]);
    fre(cases);
}

void free_case(struct __case case_)
{
    free_node(case_.cond_nd);
    free_node(case_.body_nd);
}

int node_type(node_t node, enum node_t type)
{
    return TYP(node) == type;
}
